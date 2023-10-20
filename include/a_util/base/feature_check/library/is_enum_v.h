/**
 * @file
 * Checks for std::is_enum_v and defines HAS_IS_ENUM_V accordingly.
 *
 * @copyright
 * @verbatim
Copyright @ 2022 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */

#ifndef A_UTIL_BASE_FEATURE_CHECK_LIBRARY_IS_ENUM_V_H
#define A_UTIL_BASE_FEATURE_CHECK_LIBRARY_IS_ENUM_V_H

/// @cond INTERNAL_DOCUMENTATION

#if defined(__clang__) && (__cplusplus > 201402L)

#if ((__clang_major__ == 3) && (__clang_minor__ >= 9)) || (__clang_major__ > 3)
#define HAS_IS_ENUM_V 1
#endif // ((__clang_major__ == 3) && (__clang_minor__ >= 9)) || (__clang_major__ > 3)

#elif defined(__GNUC__) && (__cplusplus >= 201703L)

#define HAS_IS_ENUM_V 1

#elif defined(_MSC_VER) && (_MSVC_LANG >= 201402L)

#define HAS_IS_ENUM_V 1

#else

#define HAS_IS_ENUM_V 0

#endif // defined(__GNUC__) && (__cplusplus >= 201703L)

/// @endcond

#endif // A_UTIL_BASE_FEATURE_CHECK_LIBRARY_IS_ENUM_V_H
