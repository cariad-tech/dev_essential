/**
 * @file
 * Unique pointer test implementation
 *
 * Copyright @ 2021 VW Group. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <a_util/memory/unique_ptr.h>

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

// Test a_util::unique_ptr
TEST(memory_test, TestUniquePtr)
{
    using a_util::memory::unique_ptr;

    bool was_destroyed = false;

    {
        unique_ptr<TestClass> p;
        const unique_ptr<TestClass>& const_p = static_cast<const unique_ptr<TestClass>&>(p);
        EXPECT_TRUE(p.get() == nullptr);
        EXPECT_FALSE(p);

        was_destroyed = false;
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

    {
        was_destroyed = false;
        unique_ptr<TestClass> p(new TestClass(was_destroyed));
        EXPECT_FALSE(p->destroyed);
    }

    EXPECT_TRUE(was_destroyed);

    {
        unique_ptr<TestClass> a, b;
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

    {
        int* p = nullptr;
        {
            unique_ptr<int> a(new int(42));
            p = a.release();
            EXPECT_EQ(*p, 42);
            EXPECT_TRUE(a.get() == nullptr);
        }

        // under valgrind this would warn if the unique_ptr mistakenly destroyed the instance
        EXPECT_EQ(*p, 42);
        delete p;
    }
}
