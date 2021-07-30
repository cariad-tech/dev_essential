/**
 * @file
 * String default conversion specifications
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

#ifndef A_UTIL_UTIL_DETAIL_STRINGS_DEFAULT_CONVERSION_HEADER_INCLUDED
#define A_UTIL_UTIL_DETAIL_STRINGS_DEFAULT_CONVERSION_HEADER_INCLUDED

#include <cstdint>

namespace a_util {
namespace strings {
namespace detail {
#if defined(__GNUC__) && (__GNUC__ == 5) && defined(__QNX__)
#pragma GCC diagnostic ignored                                                                     \
    "-Wattributes" // standard type attributes are ignored when used in templates
#endif

template <typename T>
struct defaultConversion;

template <>
struct defaultConversion<std::int8_t> {
    static const char specifier[3];
};
template <>
struct defaultConversion<std::int16_t> {
    static const char specifier[4];
};
template <>
struct defaultConversion<std::int32_t> {
    static const char specifier[3];
};
template <>
struct defaultConversion<std::int64_t> {
    static const char specifier[5];
};
template <>
struct defaultConversion<std::uint8_t> {
    static const char specifier[3];
};
template <>
struct defaultConversion<std::uint16_t> {
    static const char specifier[3];
};
template <>
struct defaultConversion<std::uint32_t> {
    static const char specifier[3];
};
template <>
struct defaultConversion<std::uint64_t> {
    static const char specifier[5];
};
template <>
struct defaultConversion<float> {
    static const char specifier[3];
};
template <>
struct defaultConversion<double> {
    static const char specifier[3];
};

#if defined(__GNUC__) && (__GNUC__ == 5) && defined(__QNX__)
#pragma GCC diagnostic warning                                                                     \
    "-Wattributes" // standard type attributes are ignored when used in templates
#endif

} // namespace detail
} // namespace strings
} // namespace a_util

#endif // A_UTIL_UTIL_DETAIL_STRINGS_DEFAULT_CONVERSION_HEADER_INCLUDED
