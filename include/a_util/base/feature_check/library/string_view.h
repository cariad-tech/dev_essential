/**
 * @file
 * Checks for std::string_view and defines HAS_STRING_VIEW accordingly.
 *
 * @copyright
 * @verbatim
Copyright @ 2022 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */

#ifndef A_UTIL_BASE_FEATURE_CHECK_LIBRARY_STRING_VIEW_H
#define A_UTIL_BASE_FEATURE_CHECK_LIBRARY_STRING_VIEW_H

/// @cond INTERNAL_DOCUMENTATION

#if __has_include(<string_view>)

#if (defined(_MSC_VER) && (_MSVC_LANG > 201402L)) || (__cplusplus > 201402L)
#define HAS_STRING_VIEW 1
#else
#define HAS_STRING_VIEW 0
#endif

#else // !__has_include(<string_view>)

#define HAS_STRING_VIEW 0

#endif // __has_include(<string_view>)

/// @endcond

#endif // A_UTIL_BASE_FEATURE_CHECK_LIBRARY_STRING_VIEW_H
