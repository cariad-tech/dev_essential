/**
 * @file
 * Shared pointer test implementation
 *
 * Copyright @ 2021 VW Group. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <a_util/memory/shared_ptr.h>

#include <gtest/gtest.h>

class TestClass {
public:
    bool& destroyed;
    TestClass(bool& destroyed) : destroyed(destroyed)
    {
        destroyed = false;
    }

    ~TestClass()
    {
        destroyed = true;
    }
};

TEST(memory_test, TestSharedPtrBasics)
{
    using a_util::memory::shared_ptr;

    shared_ptr<TestClass> p;
    const shared_ptr<TestClass>& const_p = static_cast<const shared_ptr<TestClass>&>(p);
    EXPECT_TRUE(p.get() == nullptr);
    EXPECT_FALSE(p);

    bool was_destroyed = false;
    p.reset(new TestClass(was_destroyed));

    EXPECT_TRUE(p.get() != nullptr);
    EXPECT_TRUE(const_p.get() != nullptr);
    EXPECT_TRUE(p);
    EXPECT_TRUE(const_p);
    EXPECT_FALSE(p->destroyed);
    EXPECT_FALSE(const_p->destroyed);
    EXPECT_FALSE((*p).destroyed);
    EXPECT_FALSE((*const_p).destroyed);

    p.reset();
    EXPECT_TRUE(p.get() == nullptr);
    EXPECT_FALSE(p);
    EXPECT_TRUE(was_destroyed);
}

TEST(memory_test, TestSharedPtrScope)
{
    bool was_destroyed = false;

    {
        using a_util::memory::shared_ptr;
        was_destroyed = false;
        shared_ptr<TestClass> p(new TestClass(was_destroyed));
        EXPECT_FALSE(p->destroyed);
    }

    EXPECT_TRUE(was_destroyed);
}

TEST(memory_test, TestSharedPtrComparisonSwap)
{
    using a_util::memory::shared_ptr;
    shared_ptr<TestClass> a, b;
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);

    bool dummy;
    a.reset(new TestClass(dummy));
    b.reset(new TestClass(dummy));
    EXPECT_FALSE(a == b);
    EXPECT_TRUE(a != b);

    TestClass* p_a = a.get();
    TestClass* p_b = b.get();
    a.swap(b);
    EXPECT_EQ(a.get(), p_b);
    EXPECT_EQ(b.get(), p_a);

    swap(a, b);
    EXPECT_EQ(a.get(), p_a);
    EXPECT_EQ(b.get(), p_b);
}

TEST(memory_test, TestSharedPtrRef)
{
    using a_util::memory::shared_ptr;

    bool destroyed = false;
    shared_ptr<TestClass> a, b;
    a = b;
    EXPECT_EQ(a.use_count(), 0);
    EXPECT_EQ(b.use_count(), 0);

    a.reset(new TestClass(destroyed));
    EXPECT_EQ(a.use_count(), 1);

    b = a;
    EXPECT_EQ(a.use_count(), 2);
    EXPECT_EQ(b.use_count(), 2);
    b.reset();
    EXPECT_EQ(b.use_count(), 0);
    EXPECT_EQ(a.use_count(), 1);
    EXPECT_FALSE(destroyed);
    a.reset();
    EXPECT_EQ(a.use_count(), 0);
    EXPECT_TRUE(destroyed);

    a.reset(new TestClass(destroyed));
    EXPECT_EQ(a.use_count(), 1);

    shared_ptr<TestClass> c = a;
    EXPECT_EQ(a.use_count(), 2);
    EXPECT_EQ(c.use_count(), 2);
}

struct Incomplete;
a_util::memory::shared_ptr<Incomplete> get_new_instance(bool& destroyed);

TEST(memory_test, TestSharedPtrTypeErasure)
{
    using a_util::memory::shared_ptr;
    shared_ptr<Incomplete> test;

    bool destroyed = false;
    test = get_new_instance(destroyed);
    EXPECT_FALSE(destroyed);
    test.reset();
    EXPECT_TRUE(destroyed);
}
