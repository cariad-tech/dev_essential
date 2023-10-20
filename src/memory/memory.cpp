/**
 * @file
 * Class for file system access. This class wraps all function calls for file system access.
 *
 * Copyright @ 2021 VW Group. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <cstring>
#include <stdexcept>

#ifdef _WIN32
#include <intrin.h>

#elif !defined(__clang__) && !defined(__QNX__)
#include <byteswap.h> // seem to be __GNU__ functions, but a Linux include file?

#ifndef __builtin_bswap16
static inline unsigned short __builtin_bswap16(unsigned short a)
{
    return (a << 8) | (a >> 8);
}
#endif
#endif

#include <a_util/memory/memory.h>
#include <a_util/memory/memorybuffer.h>

namespace a_util {
namespace memory {
bool copy(void* dest, std::size_t dest_size, const void* source, std::size_t bytes_to_copy)
{
    if (!dest || !source || bytes_to_copy > dest_size) {
        return false;
    }

    ::memcpy(dest, source, bytes_to_copy);
    return true;
}

bool copy(void* dest, const void* source, std::size_t bytes_to_copy)
{
    return copy(dest, bytes_to_copy, source, bytes_to_copy);
}

bool copy(MemoryBuffer& buffer, const void* source, std::size_t bytes_to_copy)
{
    if (!source) {
        return false;
    }

    if (buffer.getSize() < bytes_to_copy) {
        if (!buffer.allocate(bytes_to_copy)) {
            return false;
        }
    }

    return copy(buffer.getPtr(), buffer.getSize(), source, bytes_to_copy);
}

bool set(void* dest, std::size_t dest_size, std::uint8_t value, std::size_t bytes_to_set)
{
    if (!dest || bytes_to_set > dest_size)
        return false;
    ::memset(dest, value, bytes_to_set);
    return true;
}

bool set(void* dest, std::uint8_t value, std::size_t bytes_to_set)
{
    return set(dest, bytes_to_set, value, bytes_to_set);
}

bool set(MemoryBuffer& buffer, std::uint8_t value, std::size_t bytes_to_set)
{
    if (buffer.getSize() < bytes_to_set) {
        if (!buffer.allocate(bytes_to_set)) {
            return false;
        }
    }

    return set(buffer.getPtr(), buffer.getSize(), value, bytes_to_set);
}

bool zero(void* dest, std::size_t dest_size, std::size_t bytes_to_zero)
{
    return set(dest, dest_size, 0, bytes_to_zero);
}

bool zero(void* dest, std::size_t bytes_to_zero)
{
    return set(dest, 0, bytes_to_zero);
}

bool zero(MemoryBuffer& buffer, std::size_t bytes_to_zero)
{
    if (buffer.getSize() < bytes_to_zero) {
        if (!buffer.allocate(bytes_to_zero)) {
            return false;
        }
    }

    return set(buffer.getPtr(), buffer.getSize(), 0, bytes_to_zero);
}

int compare(const void* buf1, std::size_t buf1_size, const void* buf2, std::size_t buf2_size)
{
    if (!buf1 || !buf2)
        throw std::invalid_argument("buffer is NULL");
    if (buf1_size != buf2_size)
        throw std::invalid_argument("different buffer sizes");
    return ::memcmp(buf1, buf2, buf1_size);
}

bool isZero(const void* buffer, std::size_t buffer_size)
{
    while ((--buffer_size > 0) &&
           (0x00 == *(static_cast<const std::uint8_t*>(buffer) + buffer_size)))
        ;
    return 0x00 == *(static_cast<const std::uint8_t*>(buffer) + buffer_size);
}

bool swapEndianess(bool value)
{
    return value;
}

std::int8_t swapEndianess(std::int8_t value)
{
    return value;
}

std::uint8_t swapEndianess(std::uint8_t value)
{
    return value;
}

std::int16_t swapEndianess(std::int16_t value)
{
    return static_cast<std::int16_t>(swapEndianess(static_cast<std::uint16_t>(value)));
}

std::uint16_t swapEndianess(std::uint16_t value)
{
#ifdef _WIN32
    return _byteswap_ushort(value);
#else
    return __builtin_bswap16(value);
#endif
}

std::int32_t swapEndianess(std::int32_t value)
{
    return static_cast<std::int32_t>(swapEndianess(static_cast<std::uint32_t>(value)));
}

std::uint32_t swapEndianess(std::uint32_t value)
{
#ifdef _WIN32
    return _byteswap_ulong(value);
#else
    return __builtin_bswap32(value);
#endif
}

std::int64_t swapEndianess(std::int64_t value)
{
    return static_cast<std::int64_t>(swapEndianess(static_cast<std::uint64_t>(value)));
}

std::uint64_t swapEndianess(std::uint64_t value)
{
#ifdef _WIN32
    return _byteswap_uint64(value);
#else
    return __builtin_bswap64(value);
#endif
}

} // namespace memory
} // namespace a_util
