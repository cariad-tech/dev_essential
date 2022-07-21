/**
 * @file
 * Private implementation for @ref strings_convert_decl.h "string conversion" functions
 *
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

    This Source Code Form is subject to the terms of the Mozilla
    Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

If it is not possible or desirable to put the notice in a particular file, then
You may include the notice in a location (such as a LICENSE file in a
relevant directory) where a recipient would be likely to look for such a notice.

You may add additional accurate notices of copyright ownership.
@endverbatim
 */

#ifndef A_UTIL_UTIL_STRINGS_DETAIL_STRINGS_CONVERT_IMPL_HEADER_INCLUDED
#define A_UTIL_UTIL_STRINGS_DETAIL_STRINGS_CONVERT_IMPL_HEADER_INCLUDED

#include <a_util/strings/strings_convert_decl.h>

namespace a_util {
namespace strings {
template <typename Numeric>
bool toNumeric(const char* from, Numeric& to)
{
    return ToNumericConverter::convert(from, to);
}

template <typename Numeric>
bool toNumeric(const std::string& from, Numeric& to)
{
    return ToNumericConverter::convert(from.c_str(), to);
}

template <typename Numeric>
Numeric toNumeric(const char* from)
{
    Numeric to = Numeric();
    ToNumericConverter::convert(from, to);
    return to;
}

template <typename Numeric>
Numeric toNumeric(const std::string& from)
{
    Numeric to = Numeric();
    ToNumericConverter::convert(from.c_str(), to);
    return to;
}

} // namespace strings
} // namespace a_util

#endif // A_UTIL_UTIL_STRINGS_DETAIL_STRINGS_CONVERT_IMPL_HEADER_INCLUDED
