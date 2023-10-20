/**
 * @file
 * String convert test implementation
 *
 * Copyright @ 2021 VW Group. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <a_util/strings/strings_convert.h>

#include <gtest/gtest.h>

using namespace a_util;

#if defined(__GNUC__) && (__GNUC__ == 5) && defined(__QNX__)
#pragma GCC diagnostic ignored                                                                     \
    "-Wattributes" // standard type attributes are ignored when used in templates
#endif

TEST(string_convert_test, TestStringToInt8)
{
    const char* const str_min = "-128";
    const char* const str_max = "127";
    const char* const str_max_signed = "+127";

    // positive tests (char*)
    EXPECT_EQ(1, strings::toInt8("1"));
    EXPECT_EQ(42, strings::toInt8("42"));
    EXPECT_EQ(1, strings::toInt8("+1"));
    EXPECT_EQ(42, strings::toInt8("+42"));
    EXPECT_EQ(-1, strings::toInt8("-1"));
    EXPECT_EQ(0, strings::toInt8("+0"));
    EXPECT_EQ(0, strings::toInt8("-0"));
    EXPECT_EQ(std::numeric_limits<std::int8_t>::min(), strings::toInt8(str_min));
    EXPECT_EQ(std::numeric_limits<std::int8_t>::max(), strings::toInt8(str_max));
    EXPECT_EQ(std::numeric_limits<std::int8_t>::max(), strings::toInt8(str_max_signed));

    // positive tests (std::string overload)
    EXPECT_EQ(std::numeric_limits<std::int8_t>::min(), strings::toInt8(std::string(str_min)));
    EXPECT_EQ(std::numeric_limits<std::int8_t>::max(), strings::toInt8(std::string(str_max)));

    // negative tests
    EXPECT_EQ(0, strings::toInt8(""));
    EXPECT_EQ(0, strings::toInt8(nullptr));
    EXPECT_EQ(0, strings::toInt8("+"));
    EXPECT_EQ(0, strings::toInt8("-"));
    EXPECT_EQ(0, strings::toInt8("1.0"));
    EXPECT_EQ(0, strings::toInt8("-129"));
    EXPECT_EQ(0, strings::toInt8("128"));
    EXPECT_EQ(0, strings::toInt8("+128"));
}

TEST(string_convert_test, TestStringToInt16)
{
    const char* const str_min = "-32768";
    const char* const str_max = "32767";
    const char* const str_max_signed = "+32767";

    // positive tests (char*)
    EXPECT_EQ(1, strings::toInt16("1"));
    EXPECT_EQ(42, strings::toInt16("42"));
    EXPECT_EQ(1, strings::toInt16("+1"));
    EXPECT_EQ(42, strings::toInt16("+42"));
    EXPECT_EQ(-1, strings::toInt16("-1"));
    EXPECT_EQ(0, strings::toInt16("+0"));
    EXPECT_EQ(0, strings::toInt16("-0"));
    EXPECT_EQ(std::numeric_limits<std::int16_t>::min(), strings::toInt16(str_min));
    EXPECT_EQ(std::numeric_limits<std::int16_t>::max(), strings::toInt16(str_max));
    EXPECT_EQ(std::numeric_limits<std::int16_t>::max(), strings::toInt16(str_max_signed));

    // positive tests (std::string overload)
    EXPECT_EQ(std::numeric_limits<std::int16_t>::min(), strings::toInt16(std::string(str_min)));
    EXPECT_EQ(std::numeric_limits<std::int16_t>::max(), strings::toInt16(std::string(str_max)));

    // negative tests
    EXPECT_EQ(0, strings::toInt16(""));
    EXPECT_EQ(0, strings::toInt16(nullptr));
    EXPECT_EQ(0, strings::toInt16("+"));
    EXPECT_EQ(0, strings::toInt16("-"));
    EXPECT_EQ(0, strings::toInt16("1.0"));
    EXPECT_EQ(0, strings::toInt16("-32769"));
    EXPECT_EQ(0, strings::toInt16("32768"));
    EXPECT_EQ(0, strings::toInt16("+32768"));
}

TEST(string_convert_test, TestStringToInt32)
{
    const char* const str_min = "-2147483648";
    const char* const str_max = "2147483647";
    const char* const str_max_signed = "+2147483647";

    // positive tests (char*)
    EXPECT_EQ(1, strings::toInt32("1"));
    EXPECT_EQ(42, strings::toInt32("42"));
    EXPECT_EQ(1, strings::toInt32("+1"));
    EXPECT_EQ(42, strings::toInt32("+42"));
    EXPECT_EQ(-1, strings::toInt32("-1"));
    EXPECT_EQ(0, strings::toInt32("+0"));
    EXPECT_EQ(0, strings::toInt32("-0"));
    EXPECT_EQ(std::numeric_limits<std::int32_t>::min(), strings::toInt32(str_min));
    EXPECT_EQ(std::numeric_limits<std::int32_t>::max(), strings::toInt32(str_max));
    EXPECT_EQ(std::numeric_limits<std::int32_t>::max(), strings::toInt32(str_max_signed));

    // positive tests (std::string overload)
    EXPECT_EQ(std::numeric_limits<std::int32_t>::min(), strings::toInt32(std::string(str_min)));
    EXPECT_EQ(std::numeric_limits<std::int32_t>::max(), strings::toInt32(std::string(str_max)));

    // negative tests
    EXPECT_EQ(0, strings::toInt32(""));
    EXPECT_EQ(0, strings::toInt32(nullptr));
    EXPECT_EQ(0, strings::toInt32("+"));
    EXPECT_EQ(0, strings::toInt32("-"));
    EXPECT_EQ(0, strings::toInt32("1.0"));
    EXPECT_EQ(0, strings::toInt32("-2147483649"));
    EXPECT_EQ(0, strings::toInt32("2147483648"));
    EXPECT_EQ(0, strings::toInt32("+2147483648"));
}

TEST(string_convert_test, TestStringToInt64)
{
    const char* const str_min = "-9223372036854775808";
    const char* const str_max = "9223372036854775807";
    const char* const str_max_signed = "+9223372036854775807";

    // positive tests (char*)
    EXPECT_EQ(1, strings::toInt64("1"));
    EXPECT_EQ(42, strings::toInt64("42"));
    EXPECT_EQ(1, strings::toInt64("+1"));
    EXPECT_EQ(42, strings::toInt64("+42"));
    EXPECT_EQ(-1, strings::toInt64("-1"));
    EXPECT_EQ(0, strings::toInt64("+0"));
    EXPECT_EQ(0, strings::toInt64("-0"));
    EXPECT_EQ(std::numeric_limits<std::int64_t>::min(), strings::toInt64(str_min));
    EXPECT_EQ(std::numeric_limits<std::int64_t>::max(), strings::toInt64(str_max));
    EXPECT_EQ(std::numeric_limits<std::int64_t>::max(), strings::toInt64(str_max_signed));

    // positive tests (std::string overload)
    EXPECT_EQ(std::numeric_limits<std::int64_t>::min(), strings::toInt64(std::string(str_min)));
    EXPECT_EQ(std::numeric_limits<std::int64_t>::max(), strings::toInt64(std::string(str_max)));

    // negative tests
    EXPECT_EQ(0, strings::toInt64(""));
    EXPECT_EQ(0, strings::toInt64(nullptr));
    EXPECT_EQ(0, strings::toInt64("+"));
    EXPECT_EQ(0, strings::toInt64("-"));
    EXPECT_EQ(0, strings::toInt64("1.0"));
    EXPECT_EQ(0, strings::toInt64("-9223372036854775809"));
    EXPECT_EQ(0, strings::toInt64("9223372036854775808"));
    EXPECT_EQ(0, strings::toInt64("+9223372036854775808"));
}

TEST(string_convert_test, TestStringToUInt8)
{
    const char* const str_min = "0";
    const char* const str_max = "255";
    const char* const str_max_signed = "+255";

    // positive tests (char*)
    EXPECT_EQ(1, strings::toUInt8("1"));
    EXPECT_EQ(42, strings::toUInt8("42"));
    EXPECT_EQ(1, strings::toUInt8("+1"));
    EXPECT_EQ(42, strings::toUInt8("+42"));
    EXPECT_EQ(0, strings::toUInt8("+0"));
    EXPECT_EQ(0, strings::toUInt8("-0"));
    EXPECT_EQ(std::numeric_limits<std::uint8_t>::min(), strings::toUInt8(str_min));
    EXPECT_EQ(std::numeric_limits<std::uint8_t>::max(), strings::toUInt8(str_max));
    EXPECT_EQ(std::numeric_limits<std::uint8_t>::max(), strings::toUInt8(str_max_signed));

    // positive tests (std::string overload)
    EXPECT_EQ(std::numeric_limits<std::uint8_t>::min(), strings::toUInt8(std::string(str_min)));
    EXPECT_EQ(std::numeric_limits<std::uint8_t>::max(), strings::toUInt8(std::string(str_max)));

    // negative tests
    EXPECT_EQ(0, strings::toUInt8(""));
    EXPECT_EQ(0, strings::toUInt8(nullptr));
    EXPECT_EQ(0, strings::toUInt8("+"));
    EXPECT_EQ(0, strings::toUInt8("-"));
    EXPECT_EQ(0, strings::toUInt8("1.0"));
    EXPECT_EQ(0, strings::toUInt8("-2"));
    EXPECT_EQ(0, strings::toUInt8("256"));
    EXPECT_EQ(0, strings::toUInt8("+256"));
}

TEST(string_convert_test, TestStringToUInt16)
{
    const char* const str_min = "0";
    const char* const str_max = "65535";
    const char* const str_max_signed = "+65535";

    // positive tests (char*)
    EXPECT_EQ(1, strings::toUInt16("1"));
    EXPECT_EQ(42, strings::toUInt16("42"));
    EXPECT_EQ(1, strings::toUInt16("+1"));
    EXPECT_EQ(42, strings::toUInt16("+42"));
    EXPECT_EQ(0, strings::toUInt16("+0"));
    EXPECT_EQ(0, strings::toUInt16("-0"));
    EXPECT_EQ(std::numeric_limits<std::uint16_t>::min(), strings::toUInt16(str_min));
    EXPECT_EQ(std::numeric_limits<std::uint16_t>::max(), strings::toUInt16(str_max));
    EXPECT_EQ(std::numeric_limits<std::uint16_t>::max(), strings::toUInt16(str_max_signed));

    // positive tests (std::string overload)
    EXPECT_EQ(std::numeric_limits<std::uint16_t>::min(), strings::toUInt16(std::string(str_min)));
    EXPECT_EQ(std::numeric_limits<std::uint16_t>::max(), strings::toUInt16(std::string(str_max)));

    // negative tests
    EXPECT_EQ(0, strings::toUInt16(""));
    EXPECT_EQ(0, strings::toUInt16(nullptr));
    EXPECT_EQ(0, strings::toUInt16("+"));
    EXPECT_EQ(0, strings::toUInt16("-"));
    EXPECT_EQ(0, strings::toUInt16("1.0"));
    EXPECT_EQ(0, strings::toUInt16("-2"));
    EXPECT_EQ(0, strings::toUInt16("65536"));
    EXPECT_EQ(0, strings::toUInt16("+65536"));
}

TEST(string_convert_test, TestStringToUInt32)
{
    const char* const str_min = "0";
    const char* const str_max = "4294967295";
    const char* const str_max_signed = "+4294967295";

    // positive tests (char*)
    EXPECT_EQ(1u, strings::toUInt32("1"));
    EXPECT_EQ(42u, strings::toUInt32("42"));
    EXPECT_EQ(1u, strings::toUInt32("+1"));
    EXPECT_EQ(42u, strings::toUInt32("+42"));
    EXPECT_EQ(0u, strings::toUInt32("+0"));
    EXPECT_EQ(0u, strings::toUInt32("-0"));
    EXPECT_EQ(std::numeric_limits<std::uint32_t>::min(), strings::toUInt32(str_min));
    EXPECT_EQ(std::numeric_limits<std::uint32_t>::max(), strings::toUInt32(str_max));
    EXPECT_EQ(std::numeric_limits<std::uint32_t>::max(), strings::toUInt32(str_max_signed));

    // positive tests (std::string overload)
    EXPECT_EQ(std::numeric_limits<std::uint32_t>::min(), strings::toUInt32(std::string(str_min)));
    EXPECT_EQ(std::numeric_limits<std::uint32_t>::max(), strings::toUInt32(std::string(str_max)));

    // negative tests
    EXPECT_EQ(0u, strings::toUInt32(""));
    EXPECT_EQ(0u, strings::toUInt32(nullptr));
    EXPECT_EQ(0u, strings::toUInt32("+"));
    EXPECT_EQ(0u, strings::toUInt32("-"));
    EXPECT_EQ(0u, strings::toUInt32("1.0"));
    EXPECT_EQ(0u, strings::toUInt32("-2"));
    EXPECT_EQ(0u, strings::toUInt32("4294967296"));
    EXPECT_EQ(0u, strings::toUInt32("+4294967296"));
}

TEST(string_convert_test, TestStringToUInt64)
{
    const char* const str_min = "0";
    const char* const str_max = "18446744073709551615";
    const char* const str_max_signed = "+18446744073709551615";

    // positive tests (char*)
    EXPECT_EQ(1U, strings::toUInt64("1"));
    EXPECT_EQ(42U, strings::toUInt64("42"));
    EXPECT_EQ(1U, strings::toUInt64("+1"));
    EXPECT_EQ(42U, strings::toUInt64("+42"));
    EXPECT_EQ(0U, strings::toUInt64("+0"));
    EXPECT_EQ(0U, strings::toUInt64("-0"));
    EXPECT_EQ(std::numeric_limits<std::uint64_t>::min(), strings::toUInt64(str_min));
    EXPECT_EQ(std::numeric_limits<std::uint64_t>::max(), strings::toUInt64(str_max));
    EXPECT_EQ(std::numeric_limits<std::uint64_t>::max(), strings::toUInt64(str_max_signed));

    // positive tests (std::string overload)
    EXPECT_EQ(std::numeric_limits<std::uint64_t>::min(), strings::toUInt64(std::string(str_min)));
    EXPECT_EQ(std::numeric_limits<std::uint64_t>::max(), strings::toUInt64(std::string(str_max)));

    // negative tests
    EXPECT_EQ(0U, strings::toUInt64(""));
    EXPECT_EQ(0U, strings::toUInt64(nullptr));
    EXPECT_EQ(0U, strings::toUInt64("+"));
    EXPECT_EQ(0U, strings::toUInt64("++5"));
    EXPECT_EQ(0U, strings::toUInt64("-"));
    EXPECT_EQ(0U, strings::toUInt64("1.0"));
    EXPECT_EQ(0U, strings::toUInt64("-2"));
    EXPECT_EQ(0U, strings::toUInt64("--5"));
    EXPECT_EQ(0U, strings::toUInt64("18446744073709551616"));
    EXPECT_EQ(0U, strings::toUInt64("+18446744073709551616"));
    EXPECT_EQ(0U, strings::toUInt64("18446744073709551630"));
    EXPECT_EQ(0U, strings::toUInt64("+18446744073709551635"));
}

TEST(string_convert_test, TestStringToFloat)
{
    const char* const str_min = "-340282346638528859811704183484516925440.000000";
    const char* const str_max = "340282346638528859811704183484516925440.000000";
    const char* const str_max_signed = "+340282346638528859811704183484516925440.000000";

    // positive tests (char*)
    EXPECT_FLOAT_EQ(1, strings::toFloat("+1"));
    EXPECT_FLOAT_EQ(42, strings::toFloat("+42"));
    EXPECT_FLOAT_EQ(-1, strings::toFloat("-1"));
    EXPECT_FLOAT_EQ(0, strings::toFloat("+0"));
    EXPECT_FLOAT_EQ(0, strings::toFloat("-0"));
    EXPECT_FLOAT_EQ(1, strings::toFloat("1.0"));
    EXPECT_FLOAT_EQ(-(std::numeric_limits<float>::max()), strings::toFloat(str_min));
    EXPECT_FLOAT_EQ(std::numeric_limits<float>::max(), strings::toFloat(str_max));
    EXPECT_FLOAT_EQ(std::numeric_limits<float>::max(), strings::toFloat(str_max_signed));
    EXPECT_FLOAT_EQ(float(42.57), strings::toFloat("42.57"));

    // positive tests (std::string overload)
    EXPECT_FLOAT_EQ(-(std::numeric_limits<float>::max()), strings::toFloat(std::string(str_min)));
    EXPECT_FLOAT_EQ(std::numeric_limits<float>::max(), strings::toFloat(std::string(str_max)));

    // negative tests
    EXPECT_FLOAT_EQ(0, strings::toFloat(""));
    EXPECT_FLOAT_EQ(0, strings::toFloat(nullptr));
    EXPECT_FLOAT_EQ(0, strings::toFloat("+"));
    EXPECT_FLOAT_EQ(0, strings::toFloat("-"));
    EXPECT_FLOAT_EQ(0, strings::toFloat("--2147483649"));
    EXPECT_FLOAT_EQ(0, strings::toFloat("-+2147483649"));
    EXPECT_FLOAT_EQ(0, strings::toFloat("+-2147483649"));
    EXPECT_FLOAT_EQ(0, strings::toFloat("++2147483648"));

    // a little greater/less than max/lowest
    EXPECT_FLOAT_EQ(0, strings::toFloat("340282349638528859811704183484516925440.000001"));
    EXPECT_FLOAT_EQ(0, strings::toFloat("+340282349638528859811704183484516925440.000001"));
    EXPECT_FLOAT_EQ(0, strings::toFloat("-340282349638528859811704183484516925440.000001"));
}

TEST(string_convert_test, TestStringToDouble)
{
    const char* const str_min = "-179769313486231570814527423731704356798070567525844996598917476\
8031572607800285387605895586327668781715404589535143824642343213268894641827684675467035375169860\
4991057655128207624549009038932894407586850845513394230458323690322294816580855933212334827479782\
6204144723168738177180919299881250404026184124858368.000000";
    const char* const str_max = "1797693134862315708145274237317043567980705675258449965989174768\
0315726078002853876058955863276687817154045895351438246423432132688946418276846754670353751698604\
9910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826\
204144723168738177180919299881250404026184124858368.000000";
    const char* const str_max_signed = "179769313486231570814527423731704356798070567525844996598\
9174768031572607800285387605895586327668781715404589535143824642343213268894641827684675467035375\
1698604991057655128207624549009038932894407586850845513394230458323690322294816580855933212334827\
4797826204144723168738177180919299881250404026184124858368.000000";

    // positive tests (char*)
    EXPECT_DOUBLE_EQ(1, strings::toDouble("+1"));
    EXPECT_DOUBLE_EQ(42, strings::toDouble("+42"));
    EXPECT_DOUBLE_EQ(-1, strings::toDouble("-1"));
    EXPECT_DOUBLE_EQ(0, strings::toDouble("+0"));
    EXPECT_DOUBLE_EQ(0, strings::toDouble("-0"));
    EXPECT_DOUBLE_EQ(1, strings::toDouble("1.0"));
    EXPECT_DOUBLE_EQ(-(std::numeric_limits<double>::max()), strings::toDouble(str_min));
    EXPECT_DOUBLE_EQ(std::numeric_limits<double>::max(), strings::toDouble(str_max));
    EXPECT_DOUBLE_EQ(std::numeric_limits<double>::max(), strings::toDouble(str_max_signed));
    EXPECT_DOUBLE_EQ(42.57, strings::toDouble("42.57"));

    // positive tests (std::string overload)
    EXPECT_DOUBLE_EQ(-(std::numeric_limits<double>::max()),
                     strings::toDouble(std::string(str_min)));
    EXPECT_DOUBLE_EQ(std::numeric_limits<double>::max(), strings::toDouble(std::string(str_max)));

    // negative tests
    EXPECT_DOUBLE_EQ(0, strings::toDouble(""));
    EXPECT_DOUBLE_EQ(0, strings::toDouble(nullptr));
    EXPECT_DOUBLE_EQ(0, strings::toDouble("+"));
    EXPECT_DOUBLE_EQ(0, strings::toDouble("-"));
    EXPECT_DOUBLE_EQ(0, strings::toDouble("--2147483649"));
    EXPECT_DOUBLE_EQ(0, strings::toDouble("-+2147483649"));
    EXPECT_DOUBLE_EQ(0, strings::toDouble("+-2147483649"));
    EXPECT_DOUBLE_EQ(0, strings::toDouble("++2147483648"));

    // a little greater/less than max/lowest
    const char* const str_ltm = "-179769413486231570814527423731704356798070567525844996598917476\
8031572607800285387605895586327668781715404589535143824642343213268894641827684675467035375169860\
4991057655128207624549009038932894407586850845513394230458323690322294816580855933212334827479782\
6204144723168738177180919299881250404026184124858368.000000";
    const char* const str_gtm = "1797694134862315708145274237317043567980705675258449965989174768\
0315726078002853876058955863276687817154045895351438246423432132688946418276846754670353751698604\
9910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826\
204144723168738177180919299881250404026184124858368.000000";
    const char* const str_gtm_signed = "179769413486231570814527423731704356798070567525844996598\
9174768031572607800285387605895586327668781715404589535143824642343213268894641827684675467035375\
1698604991057655128207624549009038932894407586850845513394230458323690322294816580855933212334827\
4797826204144723168738177180919299881250404026184124858368.000000";

    EXPECT_DOUBLE_EQ(0, strings::toDouble(str_gtm));
    EXPECT_DOUBLE_EQ(0, strings::toDouble(str_gtm_signed));
    EXPECT_DOUBLE_EQ(0, strings::toDouble(str_ltm));
}

#if defined(__GNUC__) && (__GNUC__ == 5) && defined(__QNX__)
#pragma GCC diagnostic warning                                                                     \
    "-Wattributes" // standard type attributes are ignored when used in templates
#endif

TEST(string_convert_test, TestStringToBool)
{
    // explicit
    EXPECT_TRUE(strings::toBool("true"));
    EXPECT_TRUE(strings::toBool("TrUe")); // case insensitive
    EXPECT_TRUE(strings::toBool("1"));
    EXPECT_TRUE(strings::toBool("146.7"));
    EXPECT_TRUE(strings::toBool("0.1"));
    EXPECT_TRUE(strings::toBool(".1"));
    EXPECT_TRUE(strings::toBool("1.0"));
    EXPECT_TRUE(strings::toBool("1.00000"));

    EXPECT_FALSE(strings::toBool("false"));
    EXPECT_FALSE(strings::toBool("FaLSe")); // case insensitive
    EXPECT_FALSE(strings::toBool("0"));
    EXPECT_FALSE(strings::toBool("0.0"));
    EXPECT_FALSE(strings::toBool(".0"));
    EXPECT_FALSE(strings::toBool("00"));
    EXPECT_FALSE(strings::toBool("tr ue"));

    // generic
    EXPECT_TRUE(strings::toNumeric<bool>("true"));
    EXPECT_TRUE(strings::toNumeric<bool>("tRuE")); // case insensitive
    EXPECT_TRUE(strings::toNumeric<bool>("1"));
    EXPECT_TRUE(strings::toNumeric<bool>("146.7"));
    EXPECT_TRUE(strings::toNumeric<bool>("0.1"));
    EXPECT_TRUE(strings::toNumeric<bool>(".1"));
    EXPECT_TRUE(strings::toNumeric<bool>("1.0"));
    EXPECT_TRUE(strings::toNumeric<bool>("1.00000"));

    EXPECT_FALSE(strings::toNumeric<bool>("false"));
    EXPECT_FALSE(strings::toNumeric<bool>("fAlSe")); // case insensitive
    EXPECT_FALSE(strings::toNumeric<bool>("0"));
    EXPECT_FALSE(strings::toNumeric<bool>("0.0"));
    EXPECT_FALSE(strings::toNumeric<bool>(".0"));
    EXPECT_FALSE(strings::toNumeric<bool>("00"));
    EXPECT_FALSE(strings::toNumeric<bool>("tr ue"));
}
