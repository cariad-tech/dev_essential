/**
 * @file
 * Public API for @ref a_util::memory::MemoryBuffer "MemoryBuffer" type and functions
 *
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */

#ifndef _A_UTILS_UTIL_MEMORY_MEMORYBUFFER_INCLUDED_
#define _A_UTILS_UTIL_MEMORY_MEMORYBUFFER_INCLUDED_

#include <memory>

namespace a_util {
namespace memory {
/// Memory buffer class to encapsulate and manage raw contiguously memory
class MemoryBuffer {
public:
    /// Default CTOR, not allocating any memory
    MemoryBuffer();

    /**
     * CTOR with initial size of the buffer (first allocates, then zero-initializes the buffer)
     * @param[in] initial_size The initial buffer size
     */
    MemoryBuffer(std::size_t initial_size);

    /**
     * CTOR attaching to memory from a an already existing buffer (non-owning!)
     * @param[in] buffer Pointer to the pre-allocated buffer that gets attached (non-owning!)
     * @param[in] size Size of the memory buffer
     */
    MemoryBuffer(void* buffer, std::size_t size);

    /// DTOR, either detaching any referenced buffer or deleting allocated memory
    ~MemoryBuffer();

    /**
     * Copy constructor, always allocating new memory and performing a deep copy.
     * @param[in] other Other memory buffer object to copy from.
     */
    MemoryBuffer(const MemoryBuffer& other);

    /**
     * Assignment operator, always allocating new memory and performing a deep copy.
     * @param[in] other Other memory buffer object to copy from.
     * @return @c *this
     */
    MemoryBuffer& operator=(const MemoryBuffer& other);

    /**
     * Allocate and zero-initialize a new memory buffer, freeing or detaching any managed memory
     * @param[in] new_size The size of the new buffer (a value of zero equals reset())
     * @return @c true if the allocation succeeded, @c false otherwise
     */
    bool allocate(std::size_t new_size);

    /**
     * Reset the memory (freeing or detaching any managed memory)
     */
    void reset();

    /**
     * Get the size of the currently managed memory.
     * @return Size of the currently managed memory.
     */
    std::size_t getSize() const;

    /**
     * Get raw pointer to the current buffer
     * @return Pointer to managed memory or @c nullptr if no memory is managed.
     */
    void* getPtr() const;

    /**
     * Attach a pre-allocated buffer to the instance (non-owning!)
     * @param[in] buffer Pointer to the pre-allocated memory buffer
     * @param[in] size Size of the memory buffer
     */
    void attach(void* buffer, std::size_t size);

    /**
     * Swap content of @c this object with @c other
     * @param[in] other Other memory buffer object to swap the content with.
     */
    void swap(MemoryBuffer& other);

private:
    struct Implementation;
    std::unique_ptr<Implementation> _impl;
};

/**
 * Compare content of the managed memory of two memory buffer objects for equality
 * @param[in] lhs Left-hand side object
 * @param[in] rhs Right-hand side object
 * @return @c true if the memory content is the same, @c false otherwise.
 */
bool operator==(const MemoryBuffer& lhs, const MemoryBuffer& rhs);

/**
 * Compare content of the managed memory of two memory buffer objects for inequality
 * @param[in] lhs Left-hand side object
 * @param[in] rhs Right-hand side object
 * @return @c true if the memory content is not the same, @c false otherwise.
 */
bool operator!=(const MemoryBuffer& lhs, const MemoryBuffer& rhs);

} // namespace memory
} // namespace a_util

#endif // _A_UTILS_UTIL_MEMORY_MEMORYBUFFER_INCLUDED_
