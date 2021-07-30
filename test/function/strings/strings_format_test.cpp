/**
 * @file
 * String format test implementation
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

#include "a_util/strings/strings_convert.h"
#include "a_util/strings/strings_format.h"

#include <gtest/gtest.h>

using namespace a_util;

template <typename T>
void test_integral_to_string_with_numeric_limits()
{
    const T numeric_max = std::numeric_limits<T>::max();
    const T numeric_min = std::numeric_limits<T>::min();
    const std::string str_numeric_max = strings::toString(numeric_max);
    const std::string str_numeric_min = strings::toString(numeric_min);
    EXPECT_EQ(numeric_max, strings::toNumeric<T>(str_numeric_max));
    EXPECT_EQ(numeric_min, strings::toNumeric<T>(str_numeric_min));
}

static const std::string str_127(
    " This string consists of %d chars which is exactly the maximum size needed to not perform a \
reallocation in format() function.");

static const std::string str_127_formatted(
    " This string consists of 127 chars which is exactly the maximum size needed to not perform a \
reallocation in format() function.");

static const std::string str_128(
    "This is a string consisting of %d characters and exactly the minimum size needed to test the \
reallocation in format() function.");

static const std::string str_128_formatted(
    "This is a string consisting of 128 characters and exactly the minimum size needed to test the \
reallocation in format() function.");

static const std::string str_237(
    "This is an even larger string, consisting of %lu characters. This string is intended to test \
several %s %s. A little %u speek might help here. Or negative numbers like %hd or %d. Let's see \
what happens in the tester.");

static const std::string str_237_formatted(
    "This is an even larger string, consisting of 237 characters. This string is intended to test \
several format specifiers. A little 1337 speek might help here. Or negative numbers like -28000 \
or -64000. Let's see what happens in the tester.");

TEST(string_format_test, TestStringFormat)
{
    EXPECT_EQ(std::string(), strings::format(nullptr));
    EXPECT_EQ(std::string(), strings::format(""));
    EXPECT_EQ(std::string("Hello World!"), strings::format("Hello World!"));
    EXPECT_EQ(std::string("Hello World!").size(), strings::format("Hello World!").size());

    EXPECT_EQ(str_128_formatted, strings::format(str_128.c_str(), 128));
    EXPECT_EQ(128, strings::format(str_128.c_str(), 128).size());
    EXPECT_EQ(str_237_formatted,
              strings::format(str_237.c_str(), 237, "format", "specifiers", 1337, -28000, -64000));
    EXPECT_EQ(
        237,
        strings::format(str_237.c_str(), 237, "format", "specifiers", 1337, -28000, -64000).size());

    // with given buffer size
    EXPECT_EQ(std::string(), strings::format(42, nullptr));
    EXPECT_EQ(std::string(), strings::format(32, ""));
    EXPECT_EQ(std::string(), strings::format(0, "Hello World!"));

    EXPECT_EQ(std::string("Hello World!"), strings::format(13, "Hello World!"));
    EXPECT_EQ(std::string("Hello World!").size(), strings::format(10, "Hello World!").size());

    EXPECT_EQ(str_128_formatted, strings::format(129, str_128.c_str(), 128));
    EXPECT_EQ(128, strings::format(42, str_128.c_str(), 128).size()); // force resize, buffer to low
    EXPECT_EQ(
        str_237_formatted,
        strings::format(1, str_237.c_str(), 237, "format", "specifiers", 1337, -28000, -64000));
}

TEST(string_format_test, TestStringFormatExistingBuffer)
{
    // with empty buffer default allocated size shall be used
    std::string buffer;
    std::string& ref_buffer = buffer;
    std::size_t pos = 0;
    ref_buffer = strings::format(buffer, pos, str_128.c_str(), 128);
    EXPECT_GT(buffer.size(), 128);
    ref_buffer.resize(pos);
    EXPECT_EQ(buffer, str_128_formatted);

    // no reallocation shall happen when preallocated buffer is large enough
    pos = 0;
    ref_buffer = strings::format(buffer, pos, str_127.c_str(), 127);
    EXPECT_EQ(buffer.size(), 128); // still the same size, no realloc!
    EXPECT_NE(buffer, str_127_formatted);
    ref_buffer.resize(pos);
    EXPECT_EQ(buffer, str_127_formatted);

    // use current position to add new stuff - must not overwrite the already existing content
    ref_buffer = strings::format(
        buffer, pos, str_237.c_str(), 237, "format", "specifiers", 1337, -28000, -64000);
    EXPECT_GT(buffer.size(), 364);
    buffer.resize(pos);
    EXPECT_EQ(buffer, str_127_formatted + str_237_formatted);
}

TEST(string_format_test, TestInt8ToString)
{
    test_integral_to_string_with_numeric_limits<std::int8_t>();
}

TEST(string_format_test, TestInt16ToString)
{
    test_integral_to_string_with_numeric_limits<std::int16_t>();
}

TEST(string_format_test, TestInt32ToString)
{
    test_integral_to_string_with_numeric_limits<std::int32_t>();
}

TEST(string_format_test, TestInt64ToString)
{
    test_integral_to_string_with_numeric_limits<std::int64_t>();
}

TEST(string_format_test, TestUInt8ToString)
{
    test_integral_to_string_with_numeric_limits<std::uint8_t>();
}

TEST(string_format_test, TestUInt16ToString)
{
    test_integral_to_string_with_numeric_limits<std::uint16_t>();
}

TEST(string_format_test, TestUInt32ToString)
{
    test_integral_to_string_with_numeric_limits<std::uint32_t>();
}

TEST(string_format_test, TestUInt64ToString)
{
    test_integral_to_string_with_numeric_limits<std::uint64_t>();
}

TEST(string_format_test, TestFloatToString)
{
    EXPECT_STREQ("0.000000", strings::toString(float()).c_str());

    const float numeric_max = std::numeric_limits<float>::max();
    const float numeric_min = -(numeric_max);
    const std::string str_numeric_max = strings::toString(numeric_max);
    const std::string str_numeric_min = strings::toString(numeric_min);
    EXPECT_FLOAT_EQ(numeric_max, strings::toNumeric<float>(str_numeric_max));
    EXPECT_FLOAT_EQ(numeric_min, strings::toNumeric<float>(str_numeric_min));
}

TEST(string_format_test, TestDoubleToString)
{
    EXPECT_STREQ("0.000000", strings::toString(double()).c_str());

    const double numeric_max = std::numeric_limits<double>::max();
    const double numeric_min = -(numeric_max);
    const std::string str_numeric_max = strings::toString(numeric_max);
    const std::string str_numeric_min = strings::toString(numeric_min);
    EXPECT_DOUBLE_EQ(numeric_max, strings::toNumeric<double>(str_numeric_max));
    EXPECT_DOUBLE_EQ(numeric_min, strings::toNumeric<double>(str_numeric_min));
}

TEST(string_format_test, TestBoolToString)
{
    EXPECT_EQ("false", strings::toString(bool()));
    EXPECT_EQ("false", strings::toString(bool(0)));
    EXPECT_EQ("false", strings::toString(bool(-0)));
    EXPECT_EQ("false", strings::toString(bool(+0)));
    EXPECT_EQ("false", strings::toString(bool(0.000)));
    EXPECT_EQ("true", strings::toString(bool(0.001)));
    EXPECT_EQ("true", strings::toString(bool(1)));
    EXPECT_EQ("true", strings::toString(bool(-1)));
}
