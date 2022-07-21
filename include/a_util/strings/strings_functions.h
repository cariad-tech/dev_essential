/**
 * @file
 * Public API for @ref strings_functions.h "string modification and inspection" functions
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

#ifndef A_UTIL_UTIL_STRINGS_STRINGS_FUNCTIONS_HEADER_INCLUDED
#define A_UTIL_UTIL_STRINGS_STRINGS_FUNCTIONS_HEADER_INCLUDED

#include <string> //std::string
#include <vector> //std::vector

namespace a_util {
namespace strings {
/// Contains all whitespaces (" \t\n\r")
extern const std::string white_space;

/// Contains the empty string
extern const std::string empty_string;

/**
 * Compares two 0-terminated C-strings
 *
 * @param left [in] The left string in the comparison
 * @param right [in] The right string in the comparison
 * @return Zero if the strings are identical, < 0 if the
 *         left string is less than the right string or a @c nullptr,
 *         or > 0 if the left string is greater than the right string or a @c nullptr.
 */
int compare(const char* left, const char* right);

/**
 * Compares two 0-terminated C-strings
 *
 * @param left [in] The left string in the comparison
 * @param right [in] The right string in the comparison
 * @param pos [in] Compare from position pos
 * @return Zero if the strings are identical, < 0 if the
 *         left string is less than the right string or a @c nullptr,
 *         or > 0 if the left string is greater than the right string or a @c nullptr.
 */
int compare(const char* left, const char* right, std::size_t pos);

/**
 * Compares two 0-terminated C-strings
 *
 * @param left [in] The left string in the comparison
 * @param right [in] The right string in the comparison
 * @param pos [in] Compare from position pos
 * @param length [in] Compare only length characters
 * @return Zero if the strings are identical, < 0 if the
 *         left string is less than the right string or a @c nullptr,
 *         or > 0 if the left string is greater than the right string or a @c nullptr.
 */
int compare(const char* left, const char* right, std::size_t pos, std::size_t length);

/**
 * Compares two 0-terminated C-strings, ignoring the case using _tcsicmp.
 * The generic-text function _tcsicmp, maps to either _stricmp,
 * _wcsicmp, _mbsicmp depending on the character set that is defined at compile time.
 * Each of these functions performs a case-insensitive comparison of the strings, and is
 * not affected by locale.
 *
 * @param left [in] The left string in the comparison
 * @param right [in] The right string in the comparison
 * @return Zero if the strings are identical (ignoring case), < 0 if the
 *         left string is less than the right string (ignoring case) or a @c nullptr,
 *         or > 0 if the left string is greater the right string (ignoring case) or a @c nullptr.
 */
int compareNoCase(const char* left, const char* right);

/**
 * Compares two 0-terminated C-strings, ignoring the case using _tcsicmp.
 * The generic-text function _tcsicmp, maps to either _stricmp,
 * _wcsicmp, _mbsicmp depending on the character set that is defined at compile time.
 * Each of these functions performs a case-insensitive comparison of the strings, and is
 * not affected by locale.
 *
 * @param left [in] The left string in the comparison
 * @param right [in] The right string in the comparison
 * @param pos [in] Compare from position pos
 * @return Zero if the strings are identical (ignoring case), < 0 if the
 *         left string is less than the right string (ignoring case) or a @c nullptr,
 *         or > 0 if the left string is greater the right string (ignoring case) or a @c nullptr.
 */
int compareNoCase(const char* left, const char* right, std::size_t pos);

/**
 * Compares two 0-terminated C-strings, ignoring the case using _tcsicmp.
 * The generic-text function _tcsicmp, maps to either _stricmp,
 * _wcsicmp, _mbsicmp depending on the character set that is defined at compile time.
 * Each of these functions performs a case-insensitive comparison of the strings, and is
 * not affected by locale.
 *
 * @param left [in] The left string in the comparison
 * @param right [in] The right string in the comparison
 * @param pos [in] Compare from position pos
 * @param length [in] Compare only length characters
 * @return Zero if the strings are identical (ignoring case), < 0 if the
 *         left string is less than the right string (ignoring case) or a @c nullptr,
 *         or > 0 if the left string is greater the right string (ignoring case) or a @c nullptr.
 */
int compareNoCase(const char* left, const char* right, std::size_t pos, std::size_t length);

/**
 * Compares two 0-terminated C-strings for equality
 *
 * @note This function is equivalent to Compare(left, right) == 0
 * @param left [in] The left string in the comparison
 * @param right [in] The right string in the comparison
 * @return true if both strings are considered equal, false otherwise
 */
bool isEqual(const char* left, const char* right);

/**
 * Compares two 0-terminated C-strings for equality
 *
 * @note This function is equivalent to Compare(left, right) == 0
 * @param left [in] The left string in the comparison
 * @param right [in] The right string in the comparison
 * @param pos [in] Compare from position pos
 * @return true if both strings are considered equal, false otherwise
 */
bool isEqual(const char* left, const char* right, std::size_t pos);

/**
 * Compares two 0-terminated C-strings for equality
 *
 * @note This function is equivalent to Compare(left, right) == 0
 * @param left [in] The left string in the comparison
 * @param right [in] The right string in the comparison
 * @param pos [in] Compare from position pos
 * @param length [in] Compare only length characters
 * @return true if both strings are considered equal, false otherwise
 */
bool isEqual(const char* left, const char* right, std::size_t pos, std::size_t length);

/**
 * Compares two 0-terminated C-strings for equality ignoring their case
 *
 * @note This function is equivalent to compareNoCase(left, right) == 0
 * @param left [in] The left string in the comparison
 * @param right [in] The right string in the comparison
 * @return true if both strings are considered equal, false otherwise
 */
bool isEqualNoCase(const char* left, const char* right);

/**
 * Compares two 0-terminated C-strings for equality ignoring their case
 *
 * @note This function is equivalent to compareNoCase(left, right) == 0
 * @param left [in] The left string in the comparison
 * @param right [in] The right string in the comparison
 * @param pos [in] Compare from position pos
 * @return true if both strings are considered equal, false otherwise
 */
bool isEqualNoCase(const char* left, const char* right, std::size_t pos);

/**
 * Compares two 0-terminated C-strings for equality ignoring their case
 *
 * @note This function is equivalent to compareNoCase(left, right) == 0
 * @param left [in] The left string in the comparison
 * @param right [in] The right string in the comparison
 * @param pos [in] Compare from position pos
 * @param length [in] Compare only length characters
 * @return true if both strings are considered equal, false otherwise
 */
bool isEqualNoCase(const char* left, const char* right, std::size_t pos, std::size_t length);

/**
 * Counts the number of characters in a 0-terminated C-string
 *
 * @param str [in] The C-string
 * @return The length of the string or std::string::npos, if str is a @c nullptr
 */
std::size_t getLength(const char* str);

/**
 * Copies up to dest_size-1 characters from dest to source, ensuring null termination
 *
 * @param dest [in] Destination buffer
 * @param dest_size [in] Destination buffer size
 * @param source [in] Source string (0-terminated)
 * @return Number of characters copied to dest (excluding the null character)
 * @note Throws std::invalid_argument if either of the buffer pointers are null!
 */
std::size_t copy(char* dest, std::size_t dest_size, const char* source);

/**
 * Copies up to count characters from dest to source, ensuring null termination
 *
 * @param dest [in] Destination buffer
 * @param dest_size [in] Destination buffer size
 * @param source [in] Source string (0-terminated)
 * @param count [in] Maximum number of characters to copy
 * @return Number of characters copied to dest (excluding the null character)
 * @note Throws std::invalid_argument if either of the buffer pointers are null!
 */
std::size_t copy(char* dest, std::size_t dest_size, const char* source, std::size_t count);

/**
 * Splits a string using a specified separator string
 *
 * @param str [in] The string
 * @param separator [in] The separator string
 * @return A vector of strings containing all split result parts
 */
std::vector<std::string> split(const std::string& str, const std::string& separator);

/**
 * Splits a string using a specified separator string
 *
 * @param str [in] The string
 * @param separator [in] The separator string
 * @param keep_empty [in] Flag whether to keep empty parts in the result vector
 * @return A vector of strings containing all split result parts
 */
std::vector<std::string> split(const std::string& str,
                               const std::string& separator,
                               bool keep_empty);

/**
 * Splits a string on whitespace characters
 *
 * @param str [in] The string
 * @return A vector of strings containing all split result parts
 */
std::vector<std::string> splitToken(const std::string& str);

/**
 * Splits a string using a set of separator characters
 *
 * @param str [in] The string
 * @param separators [in] The set of separator characters
 * @return A vector of strings containing all split result parts
 */
std::vector<std::string> splitToken(const std::string& str, const std::string& separators);

/**
 * Splits a string using a set of separator characters
 *
 * @param str [in] The string
 * @param separators [in] The set of separator characters
 * @param keep_empty [in] Flag whether to keep empty parts in the result vector
 * @return A vector of strings containing all split result parts
 */
std::vector<std::string> splitToken(const std::string& str,
                                    const std::string& separators,
                                    bool keep_empty);

/**
 * Trims all whitespace characters from the start and end of a string
 *
 * @param str [inout] The string that is trimmed
 * @return Returns str, trimmed as specified
 */
std::string& trim(std::string& str);

/**
 * Trims a specified set of characters from the start and end of a string
 *
 * @param str [inout] The string that is trimmed
 * @param trimmed_chars [in] The set of characters to trim
 * @return Returns str, trimmed as specified
 */
std::string& trim(std::string& str, const std::string& trimmed_chars);

/**
 * Checks whether a C-style string is empty, meaning nullptr or ""
 * @param[in] str C-style string to check - must either be nullptr or 0-terminated
 * @return true if empty, false otherwise
 */
bool isEmpty(const char* str);

/**
 * Checks whether a C-style string is not empty, meaning neither nullptr nor ""
 * @param[in] str C-style string to check - must either be nullptr or 0-terminated
 * @return @c !isEmpty(str)
 */
bool isNotEmpty(const char* str);

/**
 * In-place replacement of all occurences of @c subject with @c replacement
 * @param[in, out] str The string
 * @param[in] subject The subject string to search for
 * @param[in] replacement The replacement string to substitute
 * @return Reference to @c str, after replacement
 */
std::string& replace(std::string& str, const std::string& subject, const std::string& replacement);

/**
 * Replace all occurences of @c subject with @c replacement
 * @param[in] str The string
 * @param[in] subject The subject string to search for
 * @param[in] replacement The replacement string to substitute
 * @return New string with all replacements done accordingly
 */
std::string replace(const std::string& str,
                    const std::string& subject,
                    const std::string& replacement);

/**
 * Joins all strings from the vector into one string, separated by the specified delimiter string
 * @param[in] strings The vector of strings
 * @param[in] delimiter The delimiter string
 * @return Returns the joined string
 */
std::string join(const std::vector<std::string>& strings, const std::string& delimiter);

} // namespace strings
} // namespace a_util

#endif // A_UTIL_UTIL_STRINGS_STRINGS_FUNCTIONS_HEADER_INCLUDED
