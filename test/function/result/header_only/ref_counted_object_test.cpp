/**
 * @file
 * Test implementation for error definitions.
 *
 * Copyright @ 2021 VW Group. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <a_util/result/detail/reference_counted_object.h>

#include <gtest/gtest.h>

/****************************** PREPARATION ***********************/
struct ReferencedObjectInterface {
    virtual void setContent(int max_instances) = 0;
    virtual int getContent() const = 0;

    ReferencedObjectInterface()
    {
        ReferencedObjectInterface::instances()++;
    }
    virtual ~ReferencedObjectInterface()
    {
        ReferencedObjectInterface::instances()--;
    }

    static int& instances()
    {
        static int instances = 0;
        return instances;
    }
};

struct ReferencedObject : public ReferencedObjectInterface {
    ReferencedObject(const ReferencedObject&) = delete;
    ReferencedObject& operator=(const ReferencedObject&) = delete;
    ReferencedObject(ReferencedObject&&) = delete;
    ReferencedObject& operator=(ReferencedObject&&) = delete;

    // tests std:forward
    ReferencedObject(int max_instances) : _max_instances(max_instances)
    {
    }

    // tests const correct access via ref counter
    void setContent(int max_instances) override
    {
        this->_max_instances = max_instances;
    }
    int getContent() const override
    {
        return this->_max_instances;
    }

private:
    int _max_instances;
};

/****************************** TEST *****************************/
TEST(ref_counted_object_test, TestReferenceCountedObject)
{
    using a_util::result::detail::ReferenceCountedObjectInterface;
    using RefCountedObjectInterface = ReferenceCountedObjectInterface<ReferencedObjectInterface>;

    using a_util::result::detail::ReferenceCountedObject;
    using RefCountedObject = ReferenceCountedObject<ReferencedObjectInterface, ReferencedObject>;

    RefCountedObjectInterface* const self_deleting_ref_counter = new RefCountedObject(1);
    self_deleting_ref_counter->addReference(); // 1
    ASSERT_EQ(ReferencedObjectInterface::instances(), 1);
    ASSERT_EQ(self_deleting_ref_counter->getObject().getContent(), 1);
    self_deleting_ref_counter->getObject().setContent(0);
    ASSERT_EQ(self_deleting_ref_counter->getObject().getContent(), 0);

    self_deleting_ref_counter->addReference(); // 2
    ASSERT_EQ(ReferencedObjectInterface::instances(), 1);
    self_deleting_ref_counter->removeReference(); // 1
    ASSERT_EQ(ReferencedObjectInterface::instances(), 1);

    // NOLINTNEXTLINE(clang-analyzer-cplusplus.NewDelete)
    self_deleting_ref_counter->removeReference(); // 0
    ASSERT_EQ(ReferencedObjectInterface::instances(), 0);
}
