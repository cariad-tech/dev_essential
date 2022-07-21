/**
 * @file
 * String numeric traits functions implementations
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

#include <a_util/strings/strings_convert.h>
#include <a_util/strings/strings_numeric_traits.h>

#include <string>

namespace a_util {
namespace strings {
bool isInt8(const char* str)
{
    std::int8_t tmp = 0;
    return toInt8(str, tmp);
}

bool isInt16(const char* str)
{
    std::int16_t tmp = 0;
    return toInt16(str, tmp);
}

bool isInt32(const char* str)
{
    std::int32_t tmp = 0;
    return toInt32(str, tmp);
}

bool isInt64(const char* str)
{
    std::int64_t tmp = 0;
    return toInt64(str, tmp);
}

bool isUInt8(const char* str)
{
    std::uint8_t tmp = 0;
    return toUInt8(str, tmp);
}

bool isUInt16(const char* str)
{
    std::uint16_t tmp = 0;
    return toUInt16(str, tmp);
}

bool isUInt32(const char* str)
{
    std::uint32_t tmp = 0;
    return toUInt32(str, tmp);
}

bool isUInt64(const char* str)
{
    std::uint64_t tmp = 0;
    return toUInt64(str, tmp);
}

bool isFloat(const char* str)
{
    float tmp = 0.0;
    return toFloat(str, tmp);
}

bool isDouble(const char* str)
{
    double tmp = 0.0;
    return toDouble(str, tmp);
}

bool isBool(const char* str)
{
    bool tmp = 0;
    return toBool(str, tmp);
}

bool isInt8(const std::string& str)
{
    return isInt8(str.c_str());
}

bool isInt16(const std::string& str)
{
    return isInt16(str.c_str());
}

bool isInt32(const std::string& str)
{
    return isInt32(str.c_str());
}

bool isInt64(const std::string& str)
{
    return isInt64(str.c_str());
}

bool isUInt8(const std::string& str)
{
    return isUInt8(str.c_str());
}

bool isUInt16(const std::string& str)
{
    return isUInt16(str.c_str());
}

bool isUInt32(const std::string& str)
{
    return isUInt32(str.c_str());
}

bool isUInt64(const std::string& str)
{
    return isUInt64(str.c_str());
}

bool isFloat(const std::string& str)
{
    return isFloat(str.c_str());
}

bool isDouble(const std::string& str)
{
    return isDouble(str.c_str());
}

bool isBool(const std::string& str)
{
    return isBool(str.c_str());
}

bool IsNumericConvertible::check(const char* from, const std::int8_t&)
{
    return isInt8(from);
}

bool IsNumericConvertible::check(const char* from, const std::int16_t&)
{
    return isInt16(from);
}

bool IsNumericConvertible::check(const char* from, const std::int32_t&)
{
    return isInt32(from);
}

bool IsNumericConvertible::check(const char* from, const std::int64_t&)
{
    return isInt64(from);
}

bool IsNumericConvertible::check(const char* from, const std::uint8_t&)
{
    return isUInt8(from);
}

bool IsNumericConvertible::check(const char* from, const std::uint16_t&)
{
    return isUInt16(from);
}

bool IsNumericConvertible::check(const char* from, const std::uint32_t&)
{
    return isUInt32(from);
}

bool IsNumericConvertible::check(const char* from, const std::uint64_t&)
{
    return isUInt64(from);
}

bool IsNumericConvertible::check(const char* from, const float&)
{
    return isFloat(from);
}

bool IsNumericConvertible::check(const char* from, const double&)
{
    return isDouble(from);
}

bool IsNumericConvertible::check(const char* from, const bool&)
{
    return isBool(from);
}

} // namespace strings
} // namespace a_util
