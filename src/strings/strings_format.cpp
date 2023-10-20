/**
 * @file
 * String format functions implementation
 *
 * Copyright @ 2021 VW Group. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <a_util/strings/strings_format.h>

#include <cstdarg>
#include <stdio.h>
#include <string>

namespace a_util {
namespace strings {
namespace detail {
std::size_t format(std::string& buffer,
                   std::size_t start_index,
                   const char* str_format,
                   std::va_list args)
{
    // from: http://tinyurl.com/y8gorb27
    // ISO C++ does require &str[0] to cough up a pointer to contiguous string data
    //(but not necessarily null - terminated!)
    char* buffer_begin = &buffer[0];
    const std::int32_t null_terminated_buffer_size = static_cast<std::int32_t>(buffer.size());
#ifdef _MSC_VER
    std::int32_t bytes_written = _vsnprintf_s(buffer_begin + start_index,
                                              null_terminated_buffer_size - start_index,
                                              null_terminated_buffer_size - start_index - 1,
                                              str_format,
                                              args);
    bytes_written = (0 > bytes_written) ?
                        null_terminated_buffer_size + null_terminated_buffer_size / 2 :
                        bytes_written;
#else // linux
    // either the correct bytes written or the needed size to write all
    // returns without 0-terminating character
    std::int32_t bytes_written = vsnprintf(
        buffer_begin + start_index, null_terminated_buffer_size - start_index, str_format, args);
#endif
    // return with 0-terminating character
    return static_cast<std::size_t>(++bytes_written);
}

} // namespace detail

std::string format(const char* str_format, ...)
{
    if (!str_format || str_format[0] == '\0') {
        return std::string();
    }

    std::string formatted_string;
    std::size_t null_terminated_buffer_size = 128;
    do {
        formatted_string.resize(null_terminated_buffer_size);
        std::va_list args;
        va_start(args, str_format);
        null_terminated_buffer_size = detail::format(formatted_string, 0, str_format, args);
        va_end(args);
    } while (formatted_string.size() < null_terminated_buffer_size);
    formatted_string.resize(--null_terminated_buffer_size); // string takes care of 0-termination
    return formatted_string;
}

std::string format(std::size_t initial_buffer_size, const char* str_format, ...)
{
    if (!str_format || str_format[0] == '\0' || (0 == initial_buffer_size)) {
        return std::string();
    }

    std::string formatted_string;
    std::size_t null_terminated_buffer_size = initial_buffer_size;
    do {
        formatted_string.resize(null_terminated_buffer_size);
        std::va_list args;
        va_start(args, str_format);
        null_terminated_buffer_size = detail::format(formatted_string, 0, str_format, args);
        va_end(args);
    } while (formatted_string.size() < null_terminated_buffer_size);
    formatted_string.resize(--null_terminated_buffer_size); // string takes care of 0-termination
    return formatted_string;
}

std::string& format(std::string& buffer, std::size_t& pos, const char* str_format, ...)
{
    if (!str_format || str_format[0] == '\0') {
        return buffer;
    }

    // 128 bytes more as buffer if current buffer is not large enough
    std::size_t new_buffer_size = (pos >= buffer.size()) ? (pos + 128) : buffer.size();
    do {
        buffer.resize(new_buffer_size);
        std::va_list args;
        va_start(args, str_format);
        new_buffer_size = detail::format(buffer, pos, str_format, args) + pos;
        va_end(args);
    } while (buffer.size() < new_buffer_size);
    pos = new_buffer_size - 1; // string takes care of 0-termination
    return buffer;
}

} // namespace strings
} // namespace a_util
