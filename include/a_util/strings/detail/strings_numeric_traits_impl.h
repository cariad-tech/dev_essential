/**
 * @file
 * Private implementation for @ref strings_numeric_traits_decl.h "string numeric trait" functions
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

#ifndef A_UTIL_UTIL_STRINGS_DETAIL_STRINGS_NUMERIC_TRAITS_IMPL_HEADER_INCLUDED
#define A_UTIL_UTIL_STRINGS_DETAIL_STRINGS_NUMERIC_TRAITS_IMPL_HEADER_INCLUDED

#include "a_util/strings/strings_numeric_traits_decl.h"

namespace a_util {
namespace strings {
template <typename Numeric>
bool isNumeric(const char* str)
{
    return IsNumericConvertible::check(str, Numeric());
}

template <typename Numeric>
bool isNumeric(const std::string& str)
{
    return isNumeric<Numeric>(str.c_str());
}

} // namespace strings
} // namespace a_util

#endif // A_UTIL_UTIL_STRINGS_DETAIL_STRINGS_NUMERIC_TRAITS_IMPL_HEADER_INCLUDED
