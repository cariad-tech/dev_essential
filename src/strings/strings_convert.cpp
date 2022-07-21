/**
 * @file
 * String convert functions implementation
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

#include <cctype>  //std::is_digit
#include <cstdlib> // strtod
#ifdef __QNX__
#include <cstdio> // sscanf
#endif
#include "std_to_detail.h"
#include "strings_format_detail.h"

#include <a_util/strings/strings_convert.h>
#include <a_util/strings/strings_functions.h>

namespace a_util {
namespace strings {
namespace detail {
template <typename T>
class StringToUnsigned {
    StringToUnsigned(const StringToUnsigned&);
    StringToUnsigned& operator=(const StringToUnsigned&);

public:
    StringToUnsigned(T& to) : _to(to)
    {
    }

    template <typename Return>
    bool tryConvert(Return (*func_strtoUXX)(const char*, char**, int), const char* from)
    {
        Return tmp = 0;
        const bool is_convertible = this->stringToXX(from, tmp, func_strtoUXX);
        if (is_convertible && (std::numeric_limits<T>::max() >= tmp)) {
            _to = static_cast<T>(tmp);
            return true;
        }
        return false;
    }

    template <typename U, typename Func>
    bool stringToXX(const char* from, U& tmp, Func func_strtoxx)
    {
        const char* end = " ";
        tmp = isEmpty(from) ? 0 : func_strtoxx(from, const_cast<char**>(&end), 10);
        return (0x00 == *end);
    }

protected:
    T& _to;
};

template <typename T>
class StringToSigned : public StringToUnsigned<T> {
public:
    StringToSigned(T& to) : StringToUnsigned<T>(to)
    {
    }

    template <typename Return>
    bool tryConvert(Return (*func_strtoXX)(const char*, char**, int), const char* from)
    {
        Return tmp = 0;
        bool is_convertible = isEmpty(from) ? false : this->stringToXX(from, tmp, func_strtoXX);
        if (is_convertible) {
            if (0 > tmp) {
                is_convertible = (std::numeric_limits<T>::min() <= tmp);
            }
            else {
                is_convertible = (std::numeric_limits<T>::max() >= tmp);
            }
        }
        this->_to = is_convertible ? static_cast<T>(tmp) : this->_to;
        return is_convertible;
    }
};

bool isIntegral(const char* first, std::size_t str_length, const char** last = nullptr)
{
    const char* it = first;
    const char* const end = first + str_length;
    bool contains_only_digits = (end != it);
    const char** str_last = (nullptr != last) ? last : &first;

    while (end != it) {
        contains_only_digits = (0 != std::isdigit(*it));
        it = contains_only_digits ? it + 1 : end;
    }
    *str_last = end - 1;

    return contains_only_digits;
}

bool isFloatingPoint(const char* first, std::size_t str_length)
{
    const char* fractional = nullptr;
    bool contains_floating_point = isIntegral(first, str_length, &fractional);
    if (!contains_floating_point && (first != fractional)) {
        // dot or comma must not be first char
        fractional = ((',' == *fractional) || ('.' == *fractional)) ? fractional + 1 : fractional;
        str_length -= static_cast<std::size_t>((fractional - first));
        contains_floating_point = isIntegral(fractional, str_length);
    }
    return contains_floating_point;
}

} // namespace detail

#if defined(__GNUC__) && (__GNUC__ == 5) && defined(__QNX__)
#pragma GCC diagnostic ignored                                                                     \
    "-Wattributes" // standard type attributes are ignored when used in templates
#endif

bool toInt8(const char* from, std::int8_t& to)
{
    return detail::StringToSigned<std::int8_t>(to).tryConvert<long>(&std::strtol, from);
}

bool toInt16(const char* from, std::int16_t& to)
{
    return detail::StringToSigned<std::int16_t>(to).tryConvert<long>(&std::strtol, from);
}

bool toInt32(const char* from, std::int32_t& to)
{
    return detail::StringToSigned<std::int32_t>(to).tryConvert<std::int64_t>(&detail::strtoll,
                                                                             from);
}

bool toInt64(const char* from, std::int64_t& to)
{
    // do not use errno as it is not thread safe in pre C++11 times
    const char* end = " ";
    const std::int64_t tmp =
        isEmpty(from) ? 0 : detail::strtoll(from, const_cast<char**>(&end), 10);
    bool is_convertible = (0x00 == *end);
    if (is_convertible) {
        is_convertible = (tmp != std::numeric_limits<std::int64_t>::max()) &&
                         (tmp != std::numeric_limits<std::int64_t>::min());
        if (!is_convertible) {
            const std::string* str_int64_t_numeric_limit = nullptr;
            if (std::numeric_limits<std::int64_t>::max() == tmp) {
                str_int64_t_numeric_limit = &(detail::numericMaxToString<std::int64_t>());
                from =
                    (('+' == *from) && ('+' != (*str_int64_t_numeric_limit)[0])) ? from + 1 : from;
            }
            else {
                str_int64_t_numeric_limit = &(detail::numericMinToString<std::int64_t>());
            }
            const std::string::size_type back = str_int64_t_numeric_limit->size() - 1;
            is_convertible =
                static_cast<std::size_t>((end - from)) == str_int64_t_numeric_limit->size() &&
                (*str_int64_t_numeric_limit)[back] == *(--end);
        }
        to = is_convertible ? tmp : to;
    }

    return is_convertible;
}

bool toUInt8(const char* from, std::uint8_t& to)
{
    return detail::StringToUnsigned<std::uint8_t>(to).tryConvert<unsigned long>(&std::strtoul,
                                                                                from);
}

bool toUInt16(const char* from, std::uint16_t& to)
{
    return detail::StringToUnsigned<std::uint16_t>(to).tryConvert<unsigned long>(&std::strtoul,
                                                                                 from);
}

bool toUInt32(const char* from, std::uint32_t& to)
{
    return detail::StringToUnsigned<std::uint32_t>(to).tryConvert<std::uint64_t>(&detail::strtoull,
                                                                                 from);
}

bool toUInt64(const char* from, std::uint64_t& to)
{
    const char* end = " ";
    const std::uint64_t tmp =
        isEmpty(from) ? 0 : detail::strtoull(from, const_cast<char**>(&end), 10);
    bool is_convertible = (0x00 == *end) && (*from != '-');

    // boundary check
    if (is_convertible && (std::numeric_limits<std::uint64_t>::max() == tmp)) {
        const char* const str_uint64_t_numeric_limit =
            detail::numericMaxToString<std::uint64_t>().c_str();
        from = (('+' == *from) && ('+' != *str_uint64_t_numeric_limit)) ? from + 1 : from;
        is_convertible = strings::isEqual(str_uint64_t_numeric_limit, from);
    }

    to = is_convertible ? tmp : to;
    return is_convertible;
}

#if defined(__GNUC__) && (__GNUC__ == 5) && defined(__QNX__)
#pragma GCC diagnostic warning                                                                     \
    "-Wattributes" // standard type attributes are ignored when used in templates
#endif

bool toFloat(const char* from, float& to)
{
    double tmp = 0.0;
    bool is_convertible = toDouble(from, tmp);
    if (is_convertible) {
        // check boundaries of floating point limits - possible rounding error?!
        is_convertible = (static_cast<double>(std::numeric_limits<float>::max()) >= tmp) &&
                         (static_cast<double>(-(std::numeric_limits<float>::max())) <= tmp);
        to = is_convertible ? static_cast<float>(tmp) : to;
    }
    return is_convertible;
}

bool toDouble(const char* from, double& to)
{
#if defined(__QNX__)
    // for QNX sscanf is providing better results for values near the floating point limits
    double tmp = 0.0;
    char ch;
    const bool ok = (!from || !from[0]) ? false : (1 == sscanf(from, "%lf%c", &tmp, &ch));
#else
    const char* end = " ";
    const double tmp = isEmpty(from) ? 0 : strtod(from, const_cast<char**>(&end));
    const bool ok = (*end == 0x00);
#endif
    bool is_convertible = ok && (tmp != std::numeric_limits<double>::quiet_NaN()) &&
                          (tmp != std::numeric_limits<double>::infinity()) &&
                          (tmp != -(std::numeric_limits<double>::infinity()));
    to = is_convertible ? tmp : to;
    return is_convertible;
}

bool toBool(const char* from, bool& to)
{
    return isEmpty(from) ? false : toBool(std::string(from), to);
}

bool toInt8(const std::string& from, std::int8_t& to)
{
    return toInt8(from.c_str(), to);
}

bool toInt16(const std::string& from, std::int16_t& to)
{
    return toInt16(from.c_str(), to);
    ;
}

bool toInt32(const std::string& from, std::int32_t& to)
{
    return toInt32(from.c_str(), to);
    ;
}

bool toInt64(const std::string& from, std::int64_t& to)
{
    return toInt64(from.c_str(), to);
}

bool toUInt8(const std::string& from, std::uint8_t& to)
{
    return toUInt8(from.c_str(), to);
}

bool toUInt16(const std::string& from, std::uint16_t& to)
{
    return toUInt16(from.c_str(), to);
}

bool toUInt32(const std::string& from, std::uint32_t& to)
{
    return toUInt32(from.c_str(), to);
}

bool toUInt64(const std::string& from, std::uint64_t& to)
{
    return toUInt64(from.c_str(), to);
}

bool toFloat(const std::string& from, float& to)
{
    return toFloat(from.c_str(), to);
}

bool toDouble(const std::string& from, double& to)
{
    return toDouble(from.c_str(), to);
}

bool toBool(const std::string& from, bool& to)
{
    // integral is also a floating point; floating points can be converted to bool
    bool is_convertible = detail::isFloatingPoint(from.c_str(), from.size());

    if (is_convertible) {
        const std::string::size_type dot_index = from.find_first_of('.');
        if (std::string::npos == dot_index) {
            // no dot
            to = (std::string::npos != from.find_first_not_of('0'));
        }
        else {
            const std::string::size_type non_zero_integral_index =
                from.find_first_not_of("0", 0, dot_index + 1);
            const bool integral_contains_only_zeros = ('.' == from[non_zero_integral_index]);
            const bool fractional_contains_only_zeros =
                (std::string::npos == from.find_first_not_of('0', dot_index + 1));
            to = (integral_contains_only_zeros && fractional_contains_only_zeros) ? false : true;
        }
    }
    else {
        bool is_convertible_to_true = strings::isEqualNoCase("true", from.c_str());
        is_convertible = is_convertible_to_true || strings::isEqualNoCase("false", from.c_str());
        to = is_convertible ? (is_convertible_to_true || false) : to;
    }

    return is_convertible;
}

std::int8_t toInt8(const char* from)
{
    return toNumeric<std::int8_t>(from);
}

std::int16_t toInt16(const char* from)
{
    return toNumeric<std::int16_t>(from);
}

std::int32_t toInt32(const char* from)
{
    return toNumeric<std::int32_t>(from);
}

std::int64_t toInt64(const char* from)
{
    return toNumeric<std::int64_t>(from);
}

std::uint8_t toUInt8(const char* from)
{
    return toNumeric<std::uint8_t>(from);
}

std::uint16_t toUInt16(const char* from)
{
    return toNumeric<std::uint16_t>(from);
}

std::uint32_t toUInt32(const char* from)
{
    return toNumeric<std::uint32_t>(from);
}

std::uint64_t toUInt64(const char* from)
{
    return toNumeric<std::uint64_t>(from);
}

float toFloat(const char* from)
{
    return toNumeric<float>(from);
}

double toDouble(const char* from)
{
    return toNumeric<double>(from);
}

bool toBool(const char* from)
{
    return toNumeric<bool>(from);
}

std::int8_t toInt8(const std::string& from)
{
    return toInt8(from.c_str());
}

std::int16_t toInt16(const std::string& from)
{
    return toInt16(from.c_str());
}

std::int32_t toInt32(const std::string& from)
{
    return toInt32(from.c_str());
}

std::int64_t toInt64(const std::string& from)
{
    return toInt64(from.c_str());
}

std::uint8_t toUInt8(const std::string& from)
{
    return toUInt8(from.c_str());
}

std::uint16_t toUInt16(const std::string& from)
{
    return toUInt16(from.c_str());
}

std::uint32_t toUInt32(const std::string& from)
{
    return toUInt32(from.c_str());
}

std::uint64_t toUInt64(const std::string& from)
{
    return toUInt64(from.c_str());
}

float toFloat(const std::string& from)
{
    return toFloat(from.c_str());
}

double toDouble(const std::string& from)
{
    return toDouble(from.c_str());
}

bool toBool(const std::string& from)
{
    return toBool(from.c_str());
}

bool ToNumericConverter::convert(const char* from, std::int8_t& to)
{
    return toInt8(from, to);
}

bool ToNumericConverter::convert(const char* from, std::int16_t& to)
{
    return toInt16(from, to);
}

bool ToNumericConverter::convert(const char* from, std::int32_t& to)
{
    return toInt32(from, to);
}

bool ToNumericConverter::convert(const char* from, std::int64_t& to)
{
    return toInt64(from, to);
}

bool ToNumericConverter::convert(const char* from, std::uint8_t& to)
{
    return toUInt8(from, to);
}

bool ToNumericConverter::convert(const char* from, std::uint16_t& to)
{
    return toUInt16(from, to);
}

bool ToNumericConverter::convert(const char* from, std::uint32_t& to)
{
    return toUInt32(from, to);
}

bool ToNumericConverter::convert(const char* from, std::uint64_t& to)
{
    return toUInt64(from, to);
}

bool ToNumericConverter::convert(const char* from, float& to)
{
    return toFloat(from, to);
}

bool ToNumericConverter::convert(const char* from, double& to)
{
    return toDouble(from, to);
}

bool ToNumericConverter::convert(const char* from, bool& to)
{
    return toBool(from, to);
}

} // namespace strings
} // namespace a_util
