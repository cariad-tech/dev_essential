/**
 * @file
 * Definition of preprocessor macro @ref A_UTIL_TO_STRING
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

#ifndef A_UTIL_UTIL_PREPROCESSOR_TO_STRING_HEADER_INCLUDED
#define A_UTIL_UTIL_PREPROCESSOR_TO_STRING_HEADER_INCLUDED

/** @cond INTERNAL_DOCUMENTATION */
#ifndef A_UTIL_STRINGIFY
#define A_UTIL_STRINGIFY(expression) #expression
#endif // !A_UTIL_STRINGIFY
/** @endcond */

/**
 * @def A_UTIL_TO_STRING
 * Converts an expression to its string representation during preprocessing.
 * @param[in] expression The expression evaluating to the string representation.
 * @ingroup a_util_preprocessor
 */
#ifndef A_UTIL_TO_STRING
#define A_UTIL_TO_STRING(expression) A_UTIL_STRINGIFY(expression)
#endif // !A_UTIL_TO_STRING

#endif // A_UTIL_UTIL_PREPROCESSOR_TO_STRING_HEADER_INCLUDED
