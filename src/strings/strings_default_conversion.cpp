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

#include "strings_default_conversion.h"

#include <cstdint>

namespace a_util {
namespace strings {
namespace detail {
#if defined(__GNUC__) && (__GNUC__ == 5) && defined(__QNX__)
#pragma GCC diagnostic ignored                                                                     \
    "-Wattributes" // standard type attributes are ignored when used in templates
#endif

const char defaultConversion<std::int8_t>::specifier[] = "%d";
const char defaultConversion<std::int16_t>::specifier[] = "%hd";
const char defaultConversion<std::int32_t>::specifier[] = "%d";
const char defaultConversion<std::int64_t>::specifier[] = "%lld";
const char defaultConversion<std::uint8_t>::specifier[] = "%u";
const char defaultConversion<std::uint16_t>::specifier[] = "%u";
const char defaultConversion<std::uint32_t>::specifier[] = "%u";
const char defaultConversion<std::uint64_t>::specifier[] = "%llu";
const char defaultConversion<float>::specifier[] = "%f";
const char defaultConversion<double>::specifier[] = "%f";

#if defined(__GNUC__) && (__GNUC__ == 5) && defined(__QNX__)
#pragma GCC diagnostic warning                                                                     \
    "-Wattributes" // standard type attributes are ignored when used in templates
#endif

} // namespace detail
} // namespace strings
} // namespace a_util
