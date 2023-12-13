/**
 * @file
 * Definition of preprocessor macros to disable/enable compiler warnings
 *
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */

#ifndef A_UTIL_UTIL_PREPROCESSOR_DETAIL_DISABLE_WARNINGS_INCLUDED
#define A_UTIL_UTIL_PREPROCESSOR_DETAIL_DISABLE_WARNINGS_INCLUDED

/**
 * @def A_UTIL_DISABLE_COMPILER_WARNINGS
 * Disables all compiler warnings until @ref A_UTIL_ENABLE_COMPILER_WARNINGS is found.
 *
 * Might be used to disable warnings for third-party includes that cannot be changed. Example:
 * @code{.cpp}
// disables ALL wanings
A_UTIL_DISABLE_COMPILER_WARNINGS
#include <some/thirdparty/header.h>
// must be called to restore the previous warning state
A_UTIL_ENABLE_COMPILER_WARNINGS
 * @endcode
 * @note For internal usage only
 * @ingroup a_util_preprocessor
 */

/**
 * @def A_UTIL_ENABLE_COMPILER_WARNINGS
 * Restores the compiler warning level to the state before @ref A_UTIL_DISABLE_COMPILER_WARNINGS.
 *
 * For an example, see @ref A_UTIL_DISABLE_COMPILER_WARNINGS.
 * @note For internal usage only
 * @ingroup a_util_preprocessor
 */
#ifdef _MSC_VER
#define A_UTIL_DISABLE_COMPILER_WARNINGS __pragma(warning(push, 0))
#define A_UTIL_ENABLE_COMPILER_WARNINGS __pragma(warning(pop))
#else
#define A_UTIL_DISABLE_COMPILER_WARNINGS
#define A_UTIL_ENABLE_COMPILER_WARNINGS
#endif // _MSC_VER

#endif // A_UTIL_UTIL_PREPROCESSOR_DETAIL_DISABLE_WARNINGS_INCLUDED
