/**
 * @file
 * Class for string handling. This class implements the basic string handling functionality
 *
 * Copyright @ 2021 VW Group. All rights reserved.
 *
 *     This Source Code Form is subject to the terms of the Mozilla
 *     Public License, v. 2.0. If a copy of the MPL was not distributed
 *     with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * If it is not possible or desirable to put the notice in a particular file, then
 * You may include the notice in a location (such as a LICENSE file in a
 * relevant directory) where a recipient would be likely to look for such a notice.
 *
 * You may add additional accurate notices of copyright ownership.
 */

#include <a_util/strings/strings_functions.h>

#include <algorithm>
#include <cstring>
#include <stdexcept>
#include <string>
#include <vector>

namespace a_util {
namespace strings {
const std::string white_space = " \t\r\n";
const std::string empty_string = "";

#ifndef _WIN32

/* ++++++++++++++++++++ SBCS ++++++++++++++++++++ */
#ifndef __TCHAR_DEFINED
typedef char _TCHAR;
typedef char _TXCHAR;
typedef int _TINT;
#define __TCHAR_DEFINED

#define _stricmp strcasecmp
#define _strnicmp strncasecmp
#include <strings.h>
#endif

/* String functions */
#define _tcscmp strcmp
#define _tcsicmp _stricmp
#define _tcsncmp strncmp
#define _tcsnicmp _strnicmp

#else

#include <tchar.h>

#endif // !_WIN32

int compare(const char* left, const char* right)
{
    return compare(left, right, 0, std::string::npos);
}

int compare(const char* left, const char* right, std::size_t pos)
{
    return compare(left, right, pos, std::string::npos);
}

int compare(const char* left, const char* right, std::size_t pos, std::size_t length)
{
    if (!left) {
        return -1;
    }
    if (!right) {
        return 1;
    }

    if (pos == 0 && length == std::string::npos) {
        return _tcscmp(left, right);
    }

    const std::size_t len1 = getLength(left);
    const std::size_t len2 = getLength(right);

    if (length == 0) {
        if (pos <= len1 && pos <= len2) {
            return 0;
        }
        else {
            return -1;
        }
    }

    if (0 == len1) {
        if (0 == len2) {
            return 0;
        }
        else {
            return -1;
        }
    }

    if (0 == len2) {
        return 1;
    }

    if (len1 < pos) {
        if (len1 == len2) {
            return 0;
        }
        else if (len1 < len2) {
            return -1;
        }
        else {
            // len1 > len2
            return 1;
        }
    }
    else if (len2 < pos) {
        return 1;
    }

    if (length == std::string::npos) {
        length = std::max(len1 - pos, len2 - pos);
    }
    else if ((len1 - pos) < length || (len2 - pos) < length) {
        // prevent out of bounds access by calling compareNoCase with smallest string length
        const std::size_t length_min = std::min(len1 - pos, len2 - pos);
        const int ret = compare(left, right, length_min);
        if (0 == ret) {
            // if both strings have same length, they must be equal, otherwise the longer one is
            // greater
            return len1 == len2 ? 0 : len1 > len2 ? 1 : -1;
        }
        else {
            // a diffenrence has already been found in the substrings
            return ret;
        }
    }

    return _tcsncmp(left + pos, right + pos, length);
}

int compareNoCase(const char* left, const char* right)
{
    return compareNoCase(left, right, 0, std::string::npos);
}

int compareNoCase(const char* left, const char* right, std::size_t pos)
{
    return compareNoCase(left, right, pos, std::string::npos);
}

int compareNoCase(const char* left, const char* right, std::size_t pos, std::size_t length)
{
    if (pos == 0 && length == std::string::npos) {
        return _tcsicmp(left, right);
    }

    const std::size_t len1 = getLength(left);
    const std::size_t len2 = getLength(right);

    if (0 == length) {
        if (pos <= len1 && pos <= len2) {
            return 0;
        }
        else {
            return -1;
        }
    }

    if (0 == len1) {
        if (0 == len2) {
            return 0;
        }
        else {
            return -1;
        }
    }

    if (0 == len2) {
        return 1;
    }

    if (len1 < pos) {
        if (len1 == len2) {
            return 0;
        }
        else if (len1 < len2) {
            return -1;
        }
        else {
            // len1 > len2
            return 1;
        }
    }
    else if (len2 < pos) {
        return 1;
    }

    if (length == std::string::npos) {
        length = std::max(len1 - pos, len2 - pos);
    }
    else if ((len1 - pos) < length || (len2 - pos) < length) {
        // prevent out of bounds access by calling compareNoCase with smallest string length
        const std::size_t length_min = std::min(len1 - pos, len2 - pos);
        const int ret = compareNoCase(left, right, length_min);
        if (0 == ret) {
            // if both strings have same length, they must be equal, otherwise the longer one is
            // greater
            return len1 == len2 ? 0 : len1 > len2 ? 1 : -1;
        }
        else {
            // a difference has already been found in the substrings
            return ret;
        }
    }

    return _tcsnicmp(left + pos, right + pos, length);
}

bool isEqual(const char* left, const char* right)
{
    return compare(left, right) == 0;
}

bool isEqual(const char* left, const char* right, std::size_t pos)
{
    return compare(left, right, pos) == 0;
}

bool isEqual(const char* left, const char* right, std::size_t pos, std::size_t length)
{
    return compare(left, right, pos, length) == 0;
}

bool isEqualNoCase(const char* left, const char* right)
{
    return compareNoCase(left, right) == 0;
}

bool isEqualNoCase(const char* left, const char* right, std::size_t pos)
{
    return compareNoCase(left, right, pos) == 0;
}

bool isEqualNoCase(const char* left, const char* right, std::size_t pos, std::size_t length)
{
    return compareNoCase(left, right, pos, length) == 0;
}

std::size_t getLength(const char* str)
{
    if (str == nullptr) {
        return std::string::npos;
    }
    return static_cast<std::size_t>(strlen(str));
}

std::size_t copy(char* dest, std::size_t dest_size, const char* source)
{
    if (!dest) {
        throw std::invalid_argument("dest");
    }

    if (!source) {
        throw std::invalid_argument("source");
    }

    // sadly stdcpy and stdcpy_s (msvc) don't return the actual count, hence we cannot use it.
    // TODO: manually unroll this loop, copying more than one character at once
    // (or trust that the compiler does it for us)

    std::size_t length = 0;
    while (length < dest_size - 1 && *source) {
        dest[length++] = *source++;
    }
    dest[length] = '\0';

    return length;
}

std::size_t copy(char* dest, std::size_t dest_size, const char* source, std::size_t count)
{
    if (!dest) {
        throw std::invalid_argument("dest");
    }

    if (!source) {
        throw std::invalid_argument("source");
    }

    // sadly stdcpy and stdcpy_s (msvc) don't return the actual count, hence we cannot use it.
    // TODO: manually unroll this loop, copying more than one character at once
    // (or trust that the compiler does it for us)

    std::size_t length = 0;
    while (length < dest_size - 1 && length < count && *source) {
        dest[length++] = *source++;
    }
    dest[length] = '\0';

    return length;
}

std::vector<std::string> splitToken(const std::string& str)
{
    return splitToken(str, white_space, false);
}

std::vector<std::string> splitToken(const std::string& str, const std::string& separators)
{
    return splitToken(str, separators, false);
}

std::vector<std::string> splitToken(const std::string& str,
                                    const std::string& separators,
                                    bool keep_empty)
{
    std::vector<std::string> vecResults;
    std::size_t begin = 0, end = 0;

    while (end != std::string::npos) {
        end = str.find_first_of(separators, begin);

        std::size_t length =
            end != std::string::npos ? end - begin : str.size() - std::min(begin, str.size());
        if (length > 0 || keep_empty) {
            vecResults.push_back(str.substr(begin, length));
        }

        // last end is new beginning skipping the separator
        begin = end + 1;
    }

    return vecResults;
}

std::vector<std::string> split(const std::string& str, const std::string& separator)
{
    return split(str, separator, false);
}

std::vector<std::string> split(const std::string& str,
                               const std::string& separator,
                               bool keep_empty)
{
    std::vector<std::string> vecResults;
    std::size_t begin = 0, end = 0;

    if (separator.empty()) {
        return vecResults;
    }

    for (;;) {
        end = str.find(separator, begin);
        if (end == std::string::npos) {
            if (str.size() - begin > 0 || keep_empty) {
                vecResults.push_back(str.substr(begin));
            }
            break;
        }

        std::size_t szLength = end - begin;
        if (szLength > 0 || keep_empty) {
            vecResults.push_back(str.substr(begin, end - begin));
        }

        // last end is new beginning (+ separator)
        begin = end + separator.size();
    }

    return vecResults;
}

std::string& trim(std::string& str)
{
    return trim(str, white_space);
}

std::string& trim(std::string& str, const std::string& trimmed_chars)
{
    // trim from end first, to reduce allocations
    std::size_t offset = str.find_last_not_of(trimmed_chars.c_str());
    if (offset != std::string::npos) {
        str.erase(offset + 1);

        offset = str.find_first_not_of(trimmed_chars.c_str());
        if (offset != std::string::npos) {
            str.erase(0, offset);
        }
    }
    else {
        // either str was empty or contains only chars from the trim set -> clear it
        str.clear();
    }

    return str;
}

bool isEmpty(const char* str)
{
    return (getLength(str) == 0) || (getLength(str) == std::string::npos);
}

bool isNotEmpty(const char* str)
{
    return !isEmpty(str);
}

std::string& replace(std::string& str, const std::string& subject, const std::string& replacement)
{
    if (subject.empty()) {
        return str;
    }

    std::size_t pos = 0;
    for (;;) {
        pos = str.find(subject, pos);
        if (pos == std::string::npos) {
            break;
        }

        str.erase(pos, subject.size());
        str.insert(pos, replacement);

        pos += replacement.size();
    }

    return str;
}

std::string replace(const std::string& str,
                    const std::string& subject,
                    const std::string& replacement)
{
    auto retval = str;
    return replace(retval, subject, replacement);
}

std::string join(const std::vector<std::string>& strings, const std::string& delimiter)
{
    std::string str;
    for (std::vector<std::string>::const_iterator it = strings.begin(); it != strings.end(); ++it) {
        if (it != strings.begin()) {
            str.append(delimiter);
        }
        str.append(*it);
    }

    return str;
}

} // namespace strings
} // namespace a_util
