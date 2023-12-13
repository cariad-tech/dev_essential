/**
 * @file
 * Public API for @ref a_util::strings::unicode "unicode conversion" functions
 *
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */

#ifndef A_UTIL_UTIL_STRINGS_UNICODE_INCLUDED
#define A_UTIL_UTIL_STRINGS_UNICODE_INCLUDED

#include <string>

namespace a_util {
namespace strings {
namespace unicode {
/**
 * Convert an utf8 string to utf16 wstring
 * @param[in] utf8_str The utf8 string
 * @return Converted utf16 string
 * @throw std::invalid_argument on invalid utf8 code points
 */
std::wstring utf8ToUtf16(const std::string& utf8_str);

/**
 * Convert an utf16 wstring to utf8 string
 * @param[in] utf16_str The utf16 wstring
 * @return Converted utf8 string
 * @throw std::invalid_argument on invalid utf16 code points
 */
std::string utf16ToUtf8(const std::wstring& utf16_str);

} // namespace unicode
} // namespace strings
} // namespace a_util

#endif // A_UTIL_UTIL_STRINGS_UNICODE_INCLUDED
