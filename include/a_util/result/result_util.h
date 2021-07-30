/**
 * @file
 * Public API for @ref a_util::result "result" utility functions
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

#ifndef A_UTIL_UTIL_RESULT_RESULT_UTIL_HEADER_INCLUDED
#define A_UTIL_UTIL_RESULT_RESULT_UTIL_HEADER_INCLUDED

#include <string>

namespace a_util {
namespace result {
// forward declarations
class Result;

/**
 * Define flags
 */
enum ResultFormatFlags {
    DisableNone = 0x00,    //!< Disable nothing, stringify everything
    DisableCode = 0x01,    //!< Do not use the string representation of the error code
    DisableName = 0x02,    //!< Do not use the string representation of the error string
    DisableMessage = 0x04, //!< Do not use the string representation of the error message
    DisableFile = 0x08,    //!< Do not use the string representation of the file name
    DisableLine = 0x10,    //!< Do not use the string representation of the line
    DisableFunction = 0x20 //!< Do not use the string representation of the function
};                         // enum ResultFormatFlags

/**
 * Copy all information of an assigned result object to a (formatted) string
 * @param[in] result The result instance getting stringified
 * @param[in] formatting_flags Flags to disable stringification of specific information offered by
 *                             @c Result
 * @param[in] format Optional format specifier - if left empty, the default format is used.
 *                   Currently not implemented.
 * @return String representation of all needed information from the result object
 */
std::string toString(const Result& result,
                     ResultFormatFlags formatting_flags = DisableNone,
                     const char* format = nullptr);

} // namespace result
} // namespace a_util

#endif // A_UTIL_UTIL_RESULT_RESULT_UTIL_HEADER_INCLUDED
