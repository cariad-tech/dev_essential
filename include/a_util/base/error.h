/**
 * @file
 * Common error conditions for cross-project usage
 *
 * @copyright
 * @verbatim
Copyright @ 2022 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */

#ifndef A_UTIL_BASE_ERROR_HEADER_INCLUDED
#define A_UTIL_BASE_ERROR_HEADER_INCLUDED

#ifndef __cplusplus
#define DEV_ESSENTIAL_ENUM_CLASS enum
#else
/// Expands to @c enum if compiled as C code and to @c enum @c class if compiled as C++ code
#define DEV_ESSENTIAL_ENUM_CLASS enum class
namespace a_util {
#endif // __cplusplus

/**
 * @addtogroup a_util_core
 * @{
 */

#include "detail/error.inc"

/** @} */

#ifdef __cplusplus
} // a_util
#endif // __cplusplus

#undef DEV_ESSENTIAL_ENUM_CLASS

#endif // A_UTIL_BASE_ERROR_HEADER_INCLUDED
