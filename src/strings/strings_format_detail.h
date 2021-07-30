/**
 * @file
 * Strings private helper implementations
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

#ifndef A_UTIL_UTIL_DETAIL_STRINGS_FORMAT_DETAIL_HEADER_INCLUDED
#define A_UTIL_UTIL_DETAIL_STRINGS_FORMAT_DETAIL_HEADER_INCLUDED

#include "a_util/strings/strings_format.h"
#include "strings_default_conversion.h"

#include <limits>

namespace a_util {
namespace strings {
namespace detail {
template <typename Numeric>
const std::string& numericMaxToString(const Numeric* = nullptr)
{
    static const std::string str_numeric_limit = a_util::strings::format(
        defaultConversion<Numeric>::specifier, std::numeric_limits<Numeric>::max());
    return str_numeric_limit;
}

template <typename Numeric>
const std::string& numericMinToString(const Numeric* = nullptr)
{
    static const std::string str_numeric_limit = a_util::strings::format(
        defaultConversion<Numeric>::specifier, std::numeric_limits<Numeric>::min());
    return str_numeric_limit;
}

template <typename Numeric>
std::string signedNumericToString(Numeric from)
{ //+1 for terminating '\0'
    const std::string::size_type buffer_size = numericMinToString<Numeric>().size() + 1;
    const char* const specifier = defaultConversion<Numeric>::specifier;
    return a_util::strings::format(buffer_size, specifier, from);
}

template <typename Numeric>
std::string unsignedNumericToString(Numeric from)
{ //+1 for terminating '\0'
    const std::string::size_type buffer_size = numericMaxToString<Numeric>().size() + 1;
    const char* const specifier = defaultConversion<Numeric>::specifier;
    return a_util::strings::format(buffer_size, specifier, from);
}

} // namespace detail
} // namespace strings
} // namespace a_util

#endif // A_UTIL_UTIL_DETAIL_STRINGS_FORMAT_DETAIL_HEADER_INCLUDED
