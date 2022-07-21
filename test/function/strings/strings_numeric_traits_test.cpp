/**
 * @file
 * String numeric traits test implementation
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

#include <a_util/strings/strings_numeric_traits.h>

#include <gtest/gtest.h>

using namespace a_util;

TEST(string_numeric_traits_test, TestIsStringInt8)
{
    EXPECT_FALSE(strings::isInt8(nullptr));
    EXPECT_FALSE(strings::isInt8(""));
}

TEST(string_numeric_traits_test, TestIsStringInt16)
{
    EXPECT_FALSE(strings::isInt16(nullptr));
    EXPECT_FALSE(strings::isInt16(""));
}

TEST(string_numeric_traits_test, TestIsStringInt32)
{
    EXPECT_FALSE(strings::isInt32(nullptr));
    EXPECT_FALSE(strings::isInt32(""));
}

TEST(string_numeric_traits_test, TestIsStringInt64)
{
    EXPECT_FALSE(strings::isInt64(nullptr));
    EXPECT_FALSE(strings::isInt64(""));
}

TEST(string_numeric_traits_test, TestIsStringUInt8)
{
    EXPECT_FALSE(strings::isUInt8(nullptr));
    EXPECT_FALSE(strings::isUInt8(""));
}

TEST(string_numeric_traits_test, TestIsStringUInt16)
{
    EXPECT_FALSE(strings::isUInt16(nullptr));
    EXPECT_FALSE(strings::isUInt16(""));
}

TEST(string_numeric_traits_test, TestIsStringUInt32)
{
    EXPECT_FALSE(strings::isUInt32(nullptr));
    EXPECT_FALSE(strings::isUInt32(""));
}

TEST(string_numeric_traits_test, TestIsStringUInt64)
{
    EXPECT_FALSE(strings::isUInt64(nullptr));
    EXPECT_FALSE(strings::isUInt64(""));
}

TEST(string_numeric_traits_test, TestIsStringFloat)
{
    EXPECT_FALSE(strings::isFloat(nullptr));
    EXPECT_FALSE(strings::isFloat(""));
}

TEST(string_numeric_traits_test, TestIsStringDouble)
{
    EXPECT_FALSE(strings::isDouble(nullptr));
    EXPECT_FALSE(strings::isDouble(""));
}

TEST(string_numeric_traits_test, TestIsStringBool)
{
    EXPECT_FALSE(strings::isBool(nullptr));
    EXPECT_FALSE(strings::isBool(""));
}
