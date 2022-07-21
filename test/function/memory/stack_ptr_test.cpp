/**
 * @file
 * Shared pointer test implementation
 *
 * Copyright @ 2021 VW Group. All rights reserved.
 *
 *     This Source Code Form is subject to the terms of the Mozilla
 *     Public License, v. 2.0. If a copy of the MPL was not distributed
 *     with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * If it is not possible or desirable to put the notice in a particular file, then
 * You may include the notice in a location (such as a LICENSE file in a
 * relevant directory) where a recipient would be likely to look for such a notice.
 *
 * You may add additional accurate notices of copyright ownership.
 */

#include <a_util/memory/memory.h> //compare
#include <a_util/memory/stack_ptr.h>
#include <a_util/strings/strings_format.h> //toString

#include <gtest/gtest.h>

// non pod type due to std::string!
// This is intended to test the stack ptr with non pod types!!!
class TestClass {
public:
    TestClass(const TestClass& other) : _count_as_string(other._count_as_string)
    {
        ++TestClass::instances();
    }

    TestClass() : _count_as_string()
    {
        ++TestClass::instances();
    }

    TestClass& operator=(const TestClass& other)
    {
        if (this != &other) {
            _count_as_string = other._count_as_string;
        }
        return *this;
    }

    ~TestClass()
    {
        --instances();
    }

    static int& instances()
    {
        static int count = 0;
        return count;
    }

    const std::string& instanceAsString() const
    {
        if (_count_as_string.empty()) {
            _count_as_string = a_util::strings::toString(TestClass::instances());
        }
        return _count_as_string;
    }

private:
    // non pod type
    mutable std::string _count_as_string;
};

inline bool operator==(const TestClass& lhs, const TestClass& rhs)
{
    return lhs.instanceAsString() == rhs.instanceAsString();
}

TEST(memory_test, TestStackPtrConstructor)
{
    {
        a_util::memory::StackPtr<TestClass, sizeof(TestClass)> test_object;
        EXPECT_EQ(1, TestClass::instances());
        EXPECT_EQ("1", test_object->instanceAsString());
    }
    EXPECT_EQ(0, TestClass::instances());
}

TEST(memory_test, TestStackPtrNullptrConstructor)
{
    a_util::memory::StackPtr<TestClass, sizeof(TestClass)> test_object(nullptr);
    EXPECT_EQ(0, TestClass::instances());
    EXPECT_TRUE(a_util::memory::isZero(&test_object, sizeof(test_object)));
}

TEST(memory_test, TestStackPtrComparison)
{
    {
        a_util::memory::StackPtr<TestClass, sizeof(TestClass)> test_object1(nullptr);
        a_util::memory::StackPtr<TestClass, sizeof(TestClass)> test_object2(nullptr);
        EXPECT_EQ(test_object1, test_object2);
        EXPECT_FALSE(test_object1 != test_object2);

        test_object1 = test_object2;
        EXPECT_EQ(test_object1, test_object2);
        EXPECT_FALSE(test_object1 != test_object2);

        test_object1.reset(TestClass());
        EXPECT_NE(test_object1, test_object2);
        EXPECT_FALSE(test_object1 == test_object2);

        // content is checked, both have the same content
        test_object2.reset(TestClass());
        EXPECT_EQ(test_object1, test_object2);
        EXPECT_FALSE(test_object1 != test_object2);
    }
    EXPECT_EQ(0, TestClass::instances());
}

TEST(memory_test, TestStackPtrSwapNoneEmpty)
{
    {
        a_util::memory::StackPtr<TestClass, sizeof(TestClass)> test_object1;
        EXPECT_EQ(1, TestClass::instances());
        EXPECT_EQ("1", test_object1->instanceAsString());

        a_util::memory::StackPtr<TestClass, sizeof(TestClass)> test_object2;
        EXPECT_EQ(2, TestClass::instances());
        EXPECT_EQ("2", test_object2->instanceAsString());

        a_util::memory::swap(test_object1, test_object2);
        EXPECT_EQ(2, TestClass::instances());
        EXPECT_EQ("2", test_object1->instanceAsString());
        EXPECT_EQ("1", test_object2->instanceAsString());

        test_object1.swap(test_object2);
        EXPECT_EQ(2, TestClass::instances());
        EXPECT_EQ("1", test_object1->instanceAsString());
        EXPECT_EQ("2", test_object2->instanceAsString());
    }
    EXPECT_EQ(0, TestClass::instances());
}

TEST(memory_test, TestStackPtrSwapOneEmpty)
{
    const a_util::memory::StackPtr<TestClass, sizeof(TestClass)> reference_object(nullptr);

    {
        a_util::memory::StackPtr<TestClass, sizeof(TestClass)> test_object1(nullptr);
        EXPECT_EQ(0, TestClass::instances());
        EXPECT_EQ(reference_object, test_object1);

        a_util::memory::StackPtr<TestClass, sizeof(TestClass)> test_object2;
        EXPECT_EQ(1, TestClass::instances());
        EXPECT_EQ("1", test_object2->instanceAsString());

        a_util::memory::swap(test_object1, test_object2);
        EXPECT_EQ(1, TestClass::instances());
        EXPECT_EQ(reference_object, test_object2);
        EXPECT_EQ("1", test_object1->instanceAsString());
        EXPECT_TRUE(a_util::memory::isZero(&test_object2, sizeof(test_object2)));

        test_object2.swap(test_object1);
        EXPECT_EQ(1, TestClass::instances());
        EXPECT_EQ(reference_object, test_object1);
        EXPECT_EQ("1", test_object2->instanceAsString());
        EXPECT_TRUE(a_util::memory::isZero(&test_object1, sizeof(test_object1)));
    }
    EXPECT_EQ(0, TestClass::instances());
}

TEST(memory_test, TestStackPtrSwapBothEmpty)
{
    const a_util::memory::StackPtr<TestClass, sizeof(TestClass)> reference_object(nullptr);

    a_util::memory::StackPtr<TestClass, sizeof(TestClass)> test_object1(nullptr);
    a_util::memory::StackPtr<TestClass, sizeof(TestClass)> test_object2(nullptr);

    a_util::memory::swap(test_object1, test_object2);
    EXPECT_EQ(reference_object, test_object1);
    EXPECT_EQ(reference_object, test_object2);
    EXPECT_EQ(0, TestClass::instances());

    test_object2.swap(test_object1);
    EXPECT_EQ(reference_object, test_object1);
    EXPECT_EQ(reference_object, test_object2);
    EXPECT_EQ(0, TestClass::instances());
}

TEST(memory_test, TestStackPtrCopyConstructor)
{
    a_util::memory::StackPtr<TestClass, sizeof(TestClass)> test_object1(nullptr);
    a_util::memory::StackPtr<TestClass, sizeof(TestClass)> test_object2(test_object1);
    EXPECT_EQ(0, TestClass::instances());

    {
        a_util::memory::StackPtr<TestClass, sizeof(TestClass)> test_object3;
        EXPECT_EQ(1, TestClass::instances());
        EXPECT_EQ("1", test_object3->instanceAsString());

        a_util::memory::StackPtr<TestClass, sizeof(TestClass)> test_object4(test_object3);
        EXPECT_EQ(2, TestClass::instances());
        EXPECT_EQ("1", test_object4->instanceAsString()); // copied from test_object1
    }
    EXPECT_EQ(0, TestClass::instances());
}

TEST(memory_test, TestStackPtrAssignmentOperator)
{
    {
        a_util::memory::StackPtr<TestClass, sizeof(TestClass)> test_object1(nullptr);
        EXPECT_EQ(0, TestClass::instances());

        a_util::memory::StackPtr<TestClass, sizeof(TestClass)> test_object2 = test_object1;
        EXPECT_EQ(0, TestClass::instances());

        test_object1 = a_util::memory::StackPtr<TestClass, sizeof(TestClass)>();
        EXPECT_EQ(1, TestClass::instances());
        EXPECT_EQ("1", test_object1->instanceAsString());

        // test assignment with self
        test_object1 = test_object1;
        EXPECT_EQ(1, TestClass::instances());
        EXPECT_EQ("1", test_object1->instanceAsString());

        test_object2 = test_object1;
        EXPECT_EQ(2, TestClass::instances());
        EXPECT_EQ("1", test_object2->instanceAsString()); // copied from test_object1
        EXPECT_EQ(test_object1, test_object2);
    }
    EXPECT_EQ(0, TestClass::instances());
}

TEST(memory_test, TestStackPtrWithCopyConstructorOfStoragedType)
{
    {
        a_util::memory::StackPtr<TestClass, sizeof(TestClass)> test_object;
        EXPECT_EQ(1, TestClass::instances());
        EXPECT_EQ("1", test_object->instanceAsString());

        a_util::memory::StackPtr<TestClass, sizeof(TestClass)> test_object_copy(*test_object);
        EXPECT_EQ(2, TestClass::instances());
        EXPECT_EQ("1", test_object_copy->instanceAsString()); // copied from test_object
    }
    EXPECT_EQ(0, TestClass::instances());
}

#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable : 4324) // structure was padded due to alignment specifier
#endif

template <class T, size_t Alignment>
struct alignas(128) AlignmentTestStruct {
    char pad = true;
    a_util::memory::StackPtr<T, sizeof(T), Alignment> obj;
};

#ifdef _WIN32
#pragma warning(pop) // warning(disable : 4324)
#endif

TEST(memory_test, TestStackPtrAlignment)
{
    const auto misalignment = [](auto* p, size_t alignment) {
        return reinterpret_cast<std::uintptr_t>(p) % alignment;
    };

    static constexpr auto char_align = alignof(char);
    static constexpr auto float_align = alignof(float);
    static constexpr auto dbl_ptr_align = alignof(double*);

    using TestStruct1 = AlignmentTestStruct<char, char_align>;
    using TestStruct2 = AlignmentTestStruct<char, 4 * char_align>;
    using TestStruct3 = AlignmentTestStruct<float, float_align>;
    using TestStruct4 = AlignmentTestStruct<double*, dbl_ptr_align>;

    TestStruct1 s1;
    EXPECT_EQ(misalignment(&*s1.obj, char_align), 0);
    TestStruct2 s2; // Alignment on larger than required intervals should be ok, too.
    EXPECT_EQ(misalignment(&*s2.obj, 4 * char_align), 0);
    TestStruct3 s3;
    EXPECT_EQ(misalignment(&*s3.obj, float_align), 0);
    TestStruct4 s4;
    EXPECT_EQ(misalignment(&*s4.obj, dbl_ptr_align), 0);

    // using TestStruct5 = AlignmentTestStruct<float, 2>;  // Bad! Alignment too small!
    // TestStruct5 s5; // Static assert correctly triggering!

    // All the tester structs are aligned at 128 byte boundaries.
    // Within these 128 byte "buckets", the StackPtr instances
    // are shifted around to have the proper offsets. They cannot start
    // right at the 128 byte boundary because of the padding char.
    EXPECT_EQ(misalignment(&s1, 128), 0);
    EXPECT_EQ(misalignment(&s2, 128), 0);
    EXPECT_EQ(misalignment(&s3, 128), 0);
    EXPECT_EQ(misalignment(&s4, 128), 0);

    EXPECT_GT(offsetof(TestStruct1, obj), 0);
    EXPECT_GT(offsetof(TestStruct2, obj), 0);
    EXPECT_GT(offsetof(TestStruct3, obj), 0);
    EXPECT_GT(offsetof(TestStruct4, obj), 0);
}
