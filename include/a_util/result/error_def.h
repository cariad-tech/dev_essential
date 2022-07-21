/**
 * @file
 * Public API for @ref a_util::result "result" macros
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

#ifndef A_UTIL_UTIL_RESULT_ERROR_DEF_HEADER_INCLUDED
#define A_UTIL_UTIL_RESULT_ERROR_DEF_HEADER_INCLUDED

#include <a_util/preprocessor/current_function.h> // A_UTIL_CURRENT_FUNCTION
#include <a_util/result/result_type.h>            // a_util::result::Result
#include <a_util/strings/strings_format.h>        // a_util::strings:format

/**
 * Return an a_util::result::Result object with detailed error information
 * @details The resolved macro instanciates and returns the result object with the error
 *          description containing line, file and function name this macro is evaluated from as
 *          well as the printf()-like values from the second parameter.
 * @param[in] _errcode The error code to distribute with the result object.
 * @param[in] ... printf()-like arguments creating the error message.
 */
#define RETURN_ERROR_DESCRIPTION(_errcode, ...)                                                    \
    return a_util::result::Result(_errcode,                                                        \
                                  a_util::strings::format(__VA_ARGS__).c_str(),                    \
                                  __LINE__,                                                        \
                                  __FILE__,                                                        \
                                  A_UTIL_CURRENT_FUNCTION)

#endif // A_UTIL_UTIL_RESULT_ERROR_DEF_HEADER_INCLUDED
