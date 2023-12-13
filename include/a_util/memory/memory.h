/**
 * @file
 * Public API for @ref a_util::memory "memory" functions
 *
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */

#ifndef _A_UTILS_UTIL_MEMORY_MEMORY_INCLUDED_
#define _A_UTILS_UTIL_MEMORY_MEMORY_INCLUDED_

#include <cstddef>
#include <cstdint>

namespace a_util {
namespace memory {
class MemoryBuffer;

/**
 * Portable safe memcopy
 * @param[out] dest Pointer to destination buffer
 * @param[in] dest_size Available bytes in @c dest
 * @param[in] source Pointer to source buffer
 * @param[in] bytes_to_copy Number of bytes to copy from @c source buffer to @c dest buffer
 * @retval true if copying was successful
 * @retval false if either of the pointer arguments is nullptr or bytes_to_copy > dest_size
 */
bool copy(void* dest, std::size_t dest_size, const void* source, std::size_t bytes_to_copy);

/**
 * Portable memcopy
 * @param[out] dest Pointer to destination buffer
 * @param[in] source Pointer to source buffer
 * @param[in] bytes_to_copy Number of bytes to copy from @c source buffer to @c dest buffer
 * @retval true if copying is sucessful, false if either of the pointer arguments is nullptr
 */
bool copy(void* dest, const void* source, std::size_t bytes_to_copy);

/**
 * Copies a buffer into a MemoryBuffer instance, allocating memory if neccessary
 * @param[out] buffer Destination memory buffer
 * @param[in] source Pointer to source buffer
 * @param[in] bytes_to_copy Number of bytes to copy from the source buffer
 * @retval true if successful, false if source is nullptr or allocation fails
 */
bool copy(MemoryBuffer& buffer, const void* source, std::size_t bytes_to_copy);

/**
 * Portable safe memset
 * @param[out] dest Pointer to destination buffer
 * @param[in] dest_size Available bytes in the destination memory buffer
 * @param[in] value Byte value assigned to the buffer
 * @param[in] bytes_to_set Number of bytes to set at the destination
 * @retval true if successful, false if dest is nullptr or bytes_to_set > dest_size
 */
bool set(void* dest, std::size_t dest_size, std::uint8_t value, std::size_t bytes_to_set);

/**
 * Portable memset
 * @param[out] dest Pointer to destination buffer
 * @param[in] value Byte value assigned to the buffer
 * @param[in] bytes_to_set Number of bytes to set at the destination
 * @retval true if sucessful, false if dest is nullptr
 */
bool set(void* dest, std::uint8_t value, std::size_t bytes_to_set);

/**
 * memset for a MemoryBuffer instance, allocating memory if neccessary
 * @param[out] buffer Destination memory buffer
 * @param[in] bytes_to_set Number of bytes to set at the destination
 * @param[in] value Byte value assigned to the buffer
 * @retval true if successful, false if memory allocation fails
 */
bool set(MemoryBuffer& buffer, std::uint8_t value, std::size_t bytes_to_set);

/**
 * Portable safe memzero
 * @param[out] dest Pointer to the destination buffer
 * @param[in] dest_size Available bytes in the destination memory buffer
 * @param[in] bytes_to_zero Number of bytes to zero-assign
 * @retval true if successful, false if dest is nullptr or bytes_to_zero > dest_size
 */
bool zero(void* dest, std::size_t dest_size, std::size_t bytes_to_zero);

/**
 * Portable memzero
 * @param[out] dest Pointer to destination buffer
 * @param[in] bytes_to_zero Number of bytes to zero-assign
 * @retval true if successful, false if dest is nullptr
 */
bool zero(void* dest, std::size_t bytes_to_zero);

/**
 * memzero for a MemoryBuffer instance, allocating memory if neccessary
 * @param[out] buffer Destination memory buffer
 * @param[in] bytes_to_zero Number of bytes to zero-assign
 * @retval true if succesful, false if memory allocation fails
 */
bool zero(MemoryBuffer& buffer, std::size_t bytes_to_zero);

/**
 * Portable safe memcmp
 * @param[in] buf1 First buffer to compare
 * @param[in] buf1_size Available bytes in the first memory buffer
 * @param[in] buf2 Second buffer to compare
 * @param[in] buf2_size Available bytes in the second memory buffer
 * @retval <0 buf1 is less than buf2
 * @retval >0 buf1 is greater than buf2
 * @retval 0 both buffers are equal
 * @throw std::invalid_argument if either of the buffers is a @c nullptr or the sizes are different
 */
int compare(const void* buf1, std::size_t buf1_size, const void* buf2, std::size_t buf2_size);

/**
 * Check whether the given memory range is entirely zero-initialized
 * @param[in] buffer Pointer to beginning of buffer to check for zero-initialized values
 * @param[in] buffer_size Size of the buffer
 * @retval true if all values of @c buffer are zero-initialized, false otherwise
 */
bool isZero(const void* buffer, std::size_t buffer_size);

/**
 * Swap the byte order of parameter @c value
 * @retval value with its byte order swapped
 */
bool swapEndianess(bool value);
/// @copydoc swapEndianess(bool value)
std::int8_t swapEndianess(std::int8_t value);
/// @copydoc swapEndianess(bool value)
std::uint8_t swapEndianess(std::uint8_t value);
/// @copydoc swapEndianess(bool value)
std::int16_t swapEndianess(std::int16_t value);
/// @copydoc swapEndianess(bool value)
std::uint16_t swapEndianess(std::uint16_t value);
/// @copydoc swapEndianess(bool value)
std::int32_t swapEndianess(std::int32_t value);
/// @copydoc swapEndianess(bool value)
std::uint32_t swapEndianess(std::uint32_t value);
/// @copydoc swapEndianess(bool value)
std::int64_t swapEndianess(std::int64_t value);
/// @copydoc swapEndianess(bool value)
std::uint64_t swapEndianess(std::uint64_t value);

} // namespace memory
} // namespace a_util

#endif // _A_UTILS_UTIL_MEMORY_MEMORY_INCLUDED_
