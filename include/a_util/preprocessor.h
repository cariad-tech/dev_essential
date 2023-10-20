/**
 * @file
 * Common include for component @ref a_util_preprocessor
 *
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */

#ifndef A_UTIL_PREPROCESSOR_HEADER_INCLUDED
#define A_UTIL_PREPROCESSOR_HEADER_INCLUDED

/**
 * @defgroup a_util_preprocessor preprocessor functionality
 * Component for common preprocessor functionality and macros - applicable using@n@n
 * <tt>\#include <a_util/preprocessor.h></tt>
 *
 */

namespace a_util {
/// Solely used to structure documentation for @ref a_util_preprocessor
namespace preprocessor {
}
} // namespace a_util

#include <a_util/preprocessor/current_function.h>
#include <a_util/preprocessor/deprecated.h>
#include <a_util/preprocessor/detail/disable_warnings.h>
#include <a_util/preprocessor/dll_export.h>
#include <a_util/preprocessor/to_string.h>

#endif // A_UTIL_PREPROCESSOR_HEADER_INCLUDED
