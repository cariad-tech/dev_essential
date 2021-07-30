/**
 * @file
 * Variant test implementation
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

#include "a_util/strings/strings_functions.h"
#include "a_util/variant/variant.h"

#include <algorithm>
#include <array>
#include <gtest/gtest.h>

using namespace a_util;
using namespace a_util::variant;

namespace {

template <typename T, std::size_t N>
class VariantCopyAndMoveSemanticsTester {
public:
    VariantCopyAndMoveSemanticsTester(T (Variant::*getTypeFunc)(std::size_t) const,
                                      const char* const func_name)
        : func_under_test(getTypeFunc), func_under_test_name(func_name)
    {
        // assign alternating reference values
        bool increase_val = {};
        std::for_each(reference_values.begin(), reference_values.end(), [&increase_val](T& val) {
            increase_val = !increase_val;
            val = increase_val ? T{} : T{} + 1;
        });
        // reset Variant with the reference values
        variant_under_test.reset(reference_values.data(), reference_values.size());
        checkEqualToReference(variant_under_test);
    }

    void checkCopySemantics() const
    {
        // copy construction
        const Variant variant_copy_constructed(variant_under_test);
        checkEqualToReference(variant_copy_constructed);
        // copy assignment
        Variant variant_copy_assigned;
        variant_copy_assigned = variant_under_test;
        checkEqualToReference(variant_copy_assigned);
    }

    void checkMoveSemantics()
    {
        // move construction
        Variant variant_move_constructed(std::move(variant_under_test));
        checkEqualToReference(variant_move_constructed);
        ASSERT_EQ(Variant(), variant_under_test);
        // move assignment
        variant_under_test = std::move(variant_move_constructed);
        EXPECT_EQ(Variant(), variant_move_constructed);
        checkEqualToReference(variant_under_test);
    }

private:
    void checkEqualToReference(const Variant& variant) const noexcept
    {
        std::size_t i = {};
        std::for_each(reference_values.begin(), reference_values.end(), [&](const T& val) {
            EXPECT_EQ(val, (variant.*func_under_test)(i++))
                << "Calling member function 'Variant::" << func_under_test_name << "(" << i << ")'";
        });
    }

private:
    std::array<T, N> reference_values = {};
    Variant variant_under_test = {};
    T (Variant::*func_under_test)(std::size_t) const;
    const char* const func_under_test_name;
};

/**
 * @brief Test whether copy and move semantics work correctly for all possible array types
 *
 * For all possible array types, create a reference array with a fixed value set.
 * Each Variant object gets reset() with the reference values and the size of the array.
 * Afterwards the variant object gets copied and moved, each time comparing the reference values
 * with the copied/moved values the Variant object contains.
 * @req_id ODAUTIL-122
 */
TEST(variant_test, CopyAndMoveSemantics)
{
    constexpr std::size_t array_size = 40;

    {
        using VariantTester = VariantCopyAndMoveSemanticsTester<bool, array_size>;
        VariantTester variant_tester(&Variant::getBool, "getBool");
        variant_tester.checkCopySemantics();
        variant_tester.checkMoveSemantics();
    }

    {
        using VariantTester = VariantCopyAndMoveSemanticsTester<std::int8_t, array_size>;
        VariantTester variant_tester(&Variant::getInt8, "getInt8");
        variant_tester.checkCopySemantics();
        variant_tester.checkMoveSemantics();
    }

    {
        using VariantTester = VariantCopyAndMoveSemanticsTester<std::uint8_t, array_size>;
        VariantTester variant_tester(&Variant::getUInt8, "getUInt8");
        variant_tester.checkCopySemantics();
        variant_tester.checkMoveSemantics();
    }

    {
        using VariantTester = VariantCopyAndMoveSemanticsTester<std::int16_t, array_size>;
        VariantTester variant_tester(&Variant::getInt16, "getInt16");
        variant_tester.checkCopySemantics();
        variant_tester.checkMoveSemantics();
    }

    {
        using VariantTester = VariantCopyAndMoveSemanticsTester<std::uint16_t, array_size>;
        VariantTester variant_tester(&Variant::getUInt16, "getUInt16");
        variant_tester.checkCopySemantics();
        variant_tester.checkMoveSemantics();
    }

    {
        using VariantTester = VariantCopyAndMoveSemanticsTester<std::int32_t, array_size>;
        VariantTester variant_tester(&Variant::getInt32, "getInt32");
        variant_tester.checkCopySemantics();
        variant_tester.checkMoveSemantics();
    }

    {
        using VariantTester = VariantCopyAndMoveSemanticsTester<std::int64_t, array_size>;
        VariantTester variant_tester(&Variant::getInt64, "getInt64");
        variant_tester.checkCopySemantics();
        variant_tester.checkMoveSemantics();
    }

    {
        using VariantTester = VariantCopyAndMoveSemanticsTester<std::uint64_t, array_size>;
        VariantTester variant_tester(&Variant::getUInt64, "getUInt64");
        variant_tester.checkCopySemantics();
        variant_tester.checkMoveSemantics();
    }

    {
        using VariantTester = VariantCopyAndMoveSemanticsTester<double, array_size>;
        VariantTester variant_tester(&Variant::getDouble, "getDouble");
        variant_tester.checkCopySemantics();
        variant_tester.checkMoveSemantics();
    }

    {
        using VariantTester = VariantCopyAndMoveSemanticsTester<float, array_size>;
        VariantTester variant_tester(&Variant::getFloat32, "getFloat32");
        variant_tester.checkCopySemantics();
        variant_tester.checkMoveSemantics();
    }

    {
        using VariantTester = VariantCopyAndMoveSemanticsTester<double, array_size>;
        VariantTester variant_tester(&Variant::getFloat64, "getFloat64");
        variant_tester.checkCopySemantics();
        variant_tester.checkMoveSemantics();
    }
}

TEST(variant_test, TestVariantCTORS)
{
    // CTORS, getType, isEmpty, CCTOR
    const Variant var;
    ASSERT_TRUE(var.isEmpty());
    ASSERT_EQ(var.getType(), VT_Empty);

    const Variant var2 = true;
    ASSERT_FALSE(var2.isEmpty());
    ASSERT_EQ(var2.getType(), VT_Bool);

    const Variant var3 = (int8_t)1;
    ASSERT_FALSE(var3.isEmpty());
    ASSERT_EQ(var3.getType(), VT_Int8);

    const Variant var4 = (uint8_t)1;
    ASSERT_FALSE(var4.isEmpty());
    ASSERT_EQ(var4.getType(), VT_UInt8);

    const Variant var5 = (int16_t)1;
    ASSERT_FALSE(var5.isEmpty());
    ASSERT_EQ(var5.getType(), VT_Int16);

    const Variant var6 = (uint16_t)1;
    ASSERT_FALSE(var6.isEmpty());
    ASSERT_EQ(var6.getType(), VT_UInt16);

    const Variant var7 = 1;
    ASSERT_FALSE(var7.isEmpty());
    ASSERT_EQ(var7.getType(), VT_Int32);

    const Variant var8 = 1u;
    ASSERT_FALSE(var8.isEmpty());
    ASSERT_EQ(var8.getType(), VT_UInt32);

    const Variant var9 = (int64_t)1;
    ASSERT_FALSE(var9.isEmpty());
    ASSERT_EQ(var9.getType(), VT_Int64);

    const Variant var10 = (uint64_t)1;
    ASSERT_FALSE(var10.isEmpty());
    ASSERT_EQ(var10.getType(), VT_UInt64);

    const Variant var11 = 1.0f;
    ASSERT_FALSE(var11.isEmpty());
    ASSERT_EQ(var11.getType(), VT_Float);

    const Variant var12 = 1.0;
    ASSERT_FALSE(var12.isEmpty());
    ASSERT_EQ(var12.getType(), VT_Double);

    const Variant var13 = "abc";
    ASSERT_FALSE(var13.isEmpty());
    ASSERT_EQ(var13.getType(), VT_String);

    // CCTOR
    const Variant var14 = var13;
    ASSERT_EQ(var14.getType(), VT_String);
    ASSERT_TRUE(a_util::strings::isEqual(var14.getString(), "abc"));

    Variant var15 = var12;
    ASSERT_EQ(var15.getType(), VT_Double);
    ASSERT_EQ(var15.asDouble(), 1.0);

    const bool arr[] = {true, true, false};
    ASSERT_NO_THROW(var15.reset(arr, 3));

    const Variant var16 = var15;
    ASSERT_EQ(var16.getArraySize(), 3);
    ASSERT_TRUE(var16.isArray());
    ASSERT_EQ(var16.getBool(0), true);
    ASSERT_EQ(var16.getBool(1), true);
    ASSERT_EQ(var16.getBool(2), false);
}

TEST(variant_test, TestVariantReset)
{
    constexpr std::int8_t int8_val = -42;
    constexpr std::uint8_t uint8_val = 42;
    constexpr std::int16_t int16_val = -42;
    constexpr std::uint16_t uint16_val = 42;
    constexpr std::int32_t int32_val = 42;
    constexpr std::uint32_t uint32_val = 42;
    constexpr std::int64_t int64_val = 42;
    constexpr std::uint64_t uint64_val = 42;
    constexpr float float_val = 42.0;
    constexpr double double_val = 42.0;

    // reset overloads, Get overloads, assignment operators
    Variant var;
    EXPECT_NO_THROW(var.reset(true));
    EXPECT_EQ(var.getType(), VT_Bool);
    EXPECT_EQ(var.getBool(), true);

    EXPECT_NO_THROW(var.reset(int8_val));
    EXPECT_EQ(var.getType(), VT_Int8);
    EXPECT_EQ(var.getInt8(), int8_val);

    EXPECT_NO_THROW(var.reset(uint8_val));
    EXPECT_EQ(var.getType(), VT_UInt8);
    EXPECT_EQ(var.getUInt8(), uint8_val);

    EXPECT_NO_THROW(var.reset(int16_val));
    EXPECT_EQ(var.getType(), VT_Int16);
    EXPECT_EQ(var.getInt16(), int16_val);

    EXPECT_NO_THROW(var.reset(uint16_val));
    EXPECT_EQ(var.getType(), VT_UInt16);
    EXPECT_EQ(var.getUInt16(), uint16_val);

    EXPECT_NO_THROW(var.reset(int32_val));
    EXPECT_EQ(var.getType(), VT_Int32);
    EXPECT_EQ(var.getInt32(), int32_val);

    EXPECT_NO_THROW(var.reset(uint32_val));
    EXPECT_EQ(var.getType(), VT_UInt32);
    EXPECT_EQ(var.getUInt32(), uint32_val);

    EXPECT_NO_THROW(var.reset(int64_val));
    EXPECT_EQ(var.getType(), VT_Int64);
    EXPECT_EQ(var.getInt64(), int64_val);

    EXPECT_NO_THROW(var.reset(uint64_val));
    EXPECT_EQ(var.getType(), VT_UInt64);
    EXPECT_EQ(var.getUInt64(), uint64_val);

    EXPECT_NO_THROW(var.reset(float_val));
    EXPECT_EQ(var.getType(), VT_Float);
    EXPECT_EQ(var.getFloat(), float_val);

    EXPECT_NO_THROW(var.reset(double_val));
    EXPECT_EQ(var.getType(), VT_Double);
    EXPECT_EQ(var.getDouble(), double_val);

    EXPECT_NO_THROW(var.reset("Hallo"));
    EXPECT_EQ(var.getType(), VT_String);
    EXPECT_EQ(std::string(var.getString()), "Hallo");

    EXPECT_NO_THROW(var.reset());
    EXPECT_EQ(var.getType(), VT_Empty);

    // assignment operators
    var = true;
    EXPECT_EQ(var.getType(), VT_Bool);
    EXPECT_EQ(var.getBool(), true);

    var = int8_val;
    EXPECT_EQ(var.getType(), VT_Int8);
    EXPECT_EQ(var.getInt8(), int8_val);

    var = uint8_val;
    EXPECT_EQ(var.getType(), VT_UInt8);
    EXPECT_EQ(var.getUInt8(), uint8_val);

    var = int16_val;
    EXPECT_EQ(var.getType(), VT_Int16);
    EXPECT_EQ(var.getInt16(), int16_val);

    var = uint16_val;
    EXPECT_EQ(var.getType(), VT_UInt16);
    EXPECT_EQ(var.getUInt16(), uint16_val);

    var = int32_val;
    EXPECT_EQ(var.getType(), VT_Int32);
    EXPECT_EQ(var.getInt32(), int32_val);

    var = uint32_val;
    EXPECT_EQ(var.getType(), VT_UInt32);
    EXPECT_EQ(var.getUInt32(), uint32_val);

    var = int64_val;
    EXPECT_EQ(var.getType(), VT_Int64);
    EXPECT_EQ(var.getInt64(), int64_val);

    var = uint64_val;
    EXPECT_EQ(var.getType(), VT_UInt64);
    EXPECT_EQ(var.getUInt64(), uint64_val);

    var = float_val;
    EXPECT_EQ(var.getType(), VT_Float);
    EXPECT_EQ(var.getFloat(), float_val);

    var = double_val;
    EXPECT_EQ(var.getType(), VT_Double);
    EXPECT_EQ(var.getDouble(), double_val);

    var = "Blubb";
    EXPECT_EQ(var.getType(), VT_String);
    EXPECT_EQ(std::string(var.getString()), "Blubb");
    var = "Bla";
    EXPECT_EQ(std::string(var.getString()), "Bla");
    var = "Blabber";
    EXPECT_EQ(std::string(var.getString()), "Blabber");

    const Variant var2 = false;
    var = var2;
    EXPECT_EQ(var2.getType(), VT_Bool);
    EXPECT_EQ(var2.getBool(), false);
    EXPECT_EQ(var.getType(), VT_Bool);
    EXPECT_EQ(var.getBool(), false);
}

void TestConversion(Variant& var,
                    bool b,
                    int8_t i8,
                    uint8_t u8,
                    int16_t i16,
                    uint16_t u16,
                    int32_t i32,
                    uint32_t u32,
                    int64_t i64,
                    uint64_t u64,
                    float f,
                    double d,
                    const char* s)
{
    EXPECT_EQ(var.asBool(), b);
    EXPECT_EQ(var.asInt8(), i8);
    EXPECT_EQ(var.asUInt8(), u8);
    EXPECT_EQ(var.asInt16(), i16);
    EXPECT_EQ(var.asUInt16(), u16);
    EXPECT_EQ(var.asInt32(), i32);
    EXPECT_EQ(var.asUInt32(), u32);
    EXPECT_EQ(var.asInt64(), i64);
    EXPECT_EQ(var.asUInt64(), u64);
    EXPECT_EQ(var.asFloat(), f);
    EXPECT_EQ(var.asDouble(), f);
    EXPECT_EQ(var.asString(), s);
    EXPECT_EQ(var.operator bool(), b);
    EXPECT_EQ(var.operator int8_t(), i8);
    EXPECT_EQ(var.operator uint8_t(), u8);
    EXPECT_EQ(var.operator int16_t(), i16);
    EXPECT_EQ(var.operator uint16_t(), u16);
    EXPECT_EQ(var.operator int32_t(), i32);
    EXPECT_EQ(var.operator uint32_t(), u32);
    EXPECT_EQ(var.operator int64_t(), i64);
    EXPECT_EQ(var.operator uint64_t(), u64);
    EXPECT_EQ(var.operator float(), f);
    EXPECT_EQ(var.operator double(), d);
    EXPECT_EQ(var.operator std::string(), s);
}

TEST(variant_test, TestVariantConversion)
{
    // As, conversion operators
    Variant var;
    var = true;
    TestConversion(var, true, 1, 1, 1, 1, 1, 1, 1, 1, 1.0f, 1.0, "true");

    var = (int8_t)0;
    TestConversion(var, false, 0, 0, 0, 0, 0, 0, 0, 0, 0.0f, 0.0, "0");

    var = (uint8_t)1;
    TestConversion(var, true, 1, 1, 1, 1, 1, 1, 1, 1, 1.0f, 1.0, "1");

    var = (int16_t)-1;
    TestConversion(var,
                   true,
                   -1,
                   (uint8_t)-1,
                   -1,
                   (uint16_t)-1,
                   -1,
                   (uint32_t)-1,
                   -1,
                   (uint64_t)-1,
                   -1.0f,
                   -1.0,
                   "-1");

    var = (uint16_t)1;
    TestConversion(var, true, 1, 1, 1, 1, 1, 1, 1, 1, 1.0f, 1.0, "1");

    var = (int32_t)1;
    TestConversion(var, true, 1, 1, 1, 1, 1, 1, 1, 1, 1.0f, 1.0, "1");

    var = (uint32_t)0;
    TestConversion(var, false, 0, 0, 0, 0, 0, 0, 0, 0, 0.0f, 0.0, "0");

    var = (int64_t)1;
    TestConversion(var, true, 1, 1, 1, 1, 1, 1, 1, 1, 1.0f, 1.0, "1");

    var = (uint64_t)1;
    TestConversion(var, true, 1, 1, 1, 1, 1, 1, 1, 1, 1.0f, 1.0, "1");

    var = 1.0f;
    TestConversion(var, true, 1, 1, 1, 1, 1, 1, 1, 1, 1.0f, 1.0, "1.000000");

    var = 1.0;
    TestConversion(var, true, 1, 1, 1, 1, 1, 1, 1, 1, 1.0f, 1.0, "1.000000");

    var = "1";
    TestConversion(var, true, 1, 1, 1, 1, 1, 1, 1, 1, 1.0f, 1.0, "1");

    var = "true";
    TestConversion(var, true, 0, 0, 0, 0, 0, 0, 0, 0, 0.0f, 0.0, "true");

    var = "false";
    TestConversion(var, false, 0, 0, 0, 0, 0, 0, 0, 0, 0.0f, 0.0, "false");
}

TEST(variant_test, TestVariantArrays)
{
    // getArraySize, isArray, reset overloads, Get overloads
    Variant var;
    ASSERT_EQ(var.getArraySize(), 0);
    ASSERT_FALSE(var.isArray());
    var = true;
    ASSERT_FALSE(var.isArray());

    bool arr[] = {true, true, false};
    ASSERT_NO_THROW(var.reset(arr, 3));
    ASSERT_EQ(var.getArraySize(), 3);
    ASSERT_TRUE(var.isArray());
    ASSERT_EQ(var.getBool(0), true);
    ASSERT_EQ(var.getBool(1), true);
    ASSERT_EQ(var.getBool(2), false);

    ASSERT_THROW(var.reset((uint8_t*)nullptr, 0), std::invalid_argument);
    ASSERT_THROW(var.getBool(4), std::out_of_range);

    int8_t arr2[] = {1, 2, 3};
    ASSERT_NO_THROW(var.reset(arr2, 3));
    ASSERT_EQ(var.getArraySize(), 3);
    ASSERT_TRUE(var.isArray());
    ASSERT_EQ(var.getInt8(0), 1);
    ASSERT_EQ(var.getInt8(1), 2);
    ASSERT_EQ(var.getInt8(2), 3);

    double arr3[] = {1.0, 2.0, 3.0};
    ASSERT_NO_THROW(var.reset(arr3, 3));
    ASSERT_EQ(var.getArraySize(), 3);
    ASSERT_TRUE(var.isArray());
    ASSERT_EQ(var.getDouble(0), 1.0);
    ASSERT_EQ(var.getDouble(1), 2.0);
    ASSERT_EQ(var.getDouble(2), 3.0);
}

TEST(variant_test, TestVariantComparison)
{
    // Comparison operators
    EXPECT_TRUE(Variant(true) == Variant(true));
    EXPECT_TRUE(Variant(true) != Variant(false));
    EXPECT_TRUE(Variant((int8_t)-42) == Variant((int8_t)-42));
    EXPECT_TRUE(Variant((int8_t)-42) != Variant((int8_t)-21));
    EXPECT_TRUE(Variant((uint8_t)42) == Variant((uint8_t)42));
    EXPECT_TRUE(Variant((uint8_t)42) != Variant((uint8_t)21));
    EXPECT_TRUE(Variant((int16_t)-42) == Variant((int16_t)-42));
    EXPECT_TRUE(Variant((int16_t)-42) != Variant((int16_t)-21));
    EXPECT_TRUE(Variant((uint16_t)42) == Variant((uint16_t)42));
    EXPECT_TRUE(Variant((uint16_t)42) != Variant((uint16_t)21));
    EXPECT_TRUE(Variant((int32_t)-42) == Variant((int32_t)-42));
    EXPECT_TRUE(Variant((int32_t)-42) != Variant((int32_t)-21));
    EXPECT_TRUE(Variant((uint32_t)42) == Variant((uint32_t)42));
    EXPECT_TRUE(Variant((uint32_t)42) != Variant((uint32_t)21));
    EXPECT_TRUE(Variant((int64_t)-42) == Variant((int64_t)-42));
    EXPECT_TRUE(Variant((int64_t)-42) != Variant((int64_t)-21));
    EXPECT_TRUE(Variant((uint64_t)42) == Variant((uint64_t)42));
    EXPECT_TRUE(Variant((uint64_t)42) != Variant((uint64_t)21));
    EXPECT_TRUE(Variant((float)42.0f) == Variant((float)42.0f));
    EXPECT_TRUE(Variant((float)42.0f) != Variant((float)21.0f));
    EXPECT_TRUE(Variant((double)42.0) == Variant((double)42.0));
    EXPECT_TRUE(Variant((double)42.0) != Variant((double)21.0));
    EXPECT_TRUE(Variant("Hello") == Variant("Hello"));
    EXPECT_TRUE(Variant("Hi") != Variant("Hello"));

    EXPECT_TRUE(Variant(false) != Variant((int8_t)1));
    EXPECT_TRUE(Variant((int8_t)1) != Variant((uint8_t)1));
    EXPECT_TRUE(Variant((uint8_t)1) != Variant((int16_t)1));
    EXPECT_TRUE(Variant((int16_t)1) != Variant((uint16_t)1));
    EXPECT_TRUE(Variant((uint16_t)1) != Variant((int32_t)1));
    EXPECT_TRUE(Variant((int32_t)1) != Variant((uint32_t)1));
    EXPECT_TRUE(Variant((uint32_t)1) != Variant((int64_t)1));
    EXPECT_TRUE(Variant((int64_t)1) != Variant((uint64_t)1));
    EXPECT_TRUE(Variant((uint64_t)1) != Variant(1.0f));
    EXPECT_TRUE(Variant(1.0f) != Variant(1.0));
    EXPECT_TRUE(Variant(1.0) != Variant(""));

    Variant var1, var2;
    double arr1[] = {1.0, 2.0, 3.0, 4.0, 5.0};
    double arr2[] = {1.0, 2.0, 3.0, 4.0, 5.0};

    ASSERT_NO_THROW(var1.reset(arr1, 5));
    ASSERT_NO_THROW(var2.reset(arr2, 5));
    EXPECT_TRUE(var1 == var2);

    ASSERT_NO_THROW(var1.reset(arr1, 4));
    EXPECT_FALSE(var1 == var2);

    double arr3[] = {1.0, 2.0, 3.0, 3.0, 3.0};
    ASSERT_NO_THROW(var1.reset(arr3, 5));
    EXPECT_FALSE(var1 == var2);
}

} // unnamed namespace
