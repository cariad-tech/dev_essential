/**
 * @file
 * Public API for @ref strings_format.h "string formatting" functions
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

#ifndef A_UTIL_UTIL_STRINGS_STRINGS_FORMAT_HEADER_INCLUDED
#define A_UTIL_UTIL_STRINGS_STRINGS_FORMAT_HEADER_INCLUDED

#include <string> //std::string

namespace a_util {
namespace strings {
/**
 * printf()-like formatting of an input string.
 * @param[in] str_format The formatting string.
 * @param[in] ... vargs to create the formatted string from.
 * @return Formatted string.
 */
std::string format(const char* str_format, ...);

/**
 * printf()-like formatting of an input string with a given destination string buffer size.
 * @param[in] initial_buffer_size Initial allocyted size of the string buffer.
                                  Always pass size +1 for terminating 0-character.
 * @param[in] str_format The formatting string.
 * @param[in] ... vargs to create the formatted string from.
 * @return Formatted string.
 */
std::string format(std::size_t initial_buffer_size, const char* str_format, ...);

/**
 * printf()-like formatting of an input string with a given destination string buffer.
 * @param[in,out] buffer Destination buffer for the formatted string.
 * @param[in,out] pos Starting position in the buffer where the formatted string gets written to.
                      After successful execution, gets set to the position of the last written
                      character in the @c buffer.
 * @param[in] str_format The formatting string.
 * @param[in] ... vargs to create the formatted string from.
 * @return Formatted string (input/output parameter @c buffer).
 */
std::string& format(std::string& buffer, std::size_t& pos, const char* str_format, ...);

/**
 * Convert the boolean value to its string representation
 * @param[in] from The value converted to its string representation.
 * @return "true" or "false", according to the input parameter
 */
std::string toString(bool from);

/**
 * Convert the given value to a standard string.
 * @param[in] from The value converted to its string representation.
 * @return String representation of the input parameter
 */
std::string toString(std::int8_t from);
/// @copydoc toString(std::int8_t from)
std::string toString(std::int16_t from);
/// @copydoc toString(std::int8_t from)
std::string toString(std::int32_t from);
/// @copydoc toString(std::int8_t from)
std::string toString(std::int64_t from);
/// @copydoc toString(std::int8_t from)
std::string toString(std::uint8_t from);
/// @copydoc toString(std::int8_t from)
std::string toString(std::uint16_t from);
/// @copydoc toString(std::int8_t from)
std::string toString(std::uint32_t from);
/// @copydoc toString(std::int8_t from)
std::string toString(std::uint64_t from);
/// @copydoc toString(std::int8_t from)
std::string toString(float from);
/// @copydoc toString(std::int8_t from)
std::string toString(double from);

} // namespace strings
} // namespace a_util

#endif // A_UTIL_UTIL_STRINGS_STRINGS_FORMAT_HEADER_INCLUDED
