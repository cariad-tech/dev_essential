/**
 * @file
 * Public API for @ref a_util::memory::StackPtr "StackPtr" type and functions
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

#ifndef A_UTIL_UTIL_MEMORY_DETAIL_STACK_PTR_DECL_HEADER_INCLUDED
#define A_UTIL_UTIL_MEMORY_DETAIL_STACK_PTR_DECL_HEADER_INCLUDED

#ifndef _MSC_VER
#include <cstdint>
#endif

#include <cstddef>

namespace a_util {
namespace memory {
/**
 * A smart pointer allocating its memory only on the stack
 * @tparam T The storaged type - must be default and copy constructable
 * @tparam StackSize Size of the stack to allocate - must at least be the sizeof the object of
 *                   type @c T.
 * @tparam Alignment Alignment of the storage
 */
template <typename T, std::size_t StackSize, std::size_t Alignment = alignof(std::size_t)>
class alignas(Alignment) StackPtr {
public:
    /// Initializes the storage and constructs object ot type @c T by calling its default ctor
    StackPtr();

    /// Constructor overload t zero-initialize the storage - no object of type @c T is constructed!
    StackPtr(std::nullptr_t);

    /**
     * Initializes the storage by calling the copy constructor of type @c T
     * @param[in] data Object of type @c T whose content gets copy constructed into the storage
     */
    StackPtr(const T& data);

    /// Destructor
    ~StackPtr();

    /**
     * Copy constructor
     * @param[in] other Object of type @c StackPtr whose content gets copy constructed into the
     *                  storage only if the object inside its storage was constructed
     */
    StackPtr(const StackPtr& other);

    /**
     * Assignment operator
     * @param[in] other Object of type @c StackPtr whose content gets copy constructed into the
     *                  storage only if the object inside its storage was constructed
     * @return *this
     */
    StackPtr& operator=(StackPtr other);

    /**
     * Operator overload to use this class like a raw pointer
     * @returns Pointer to the storaged object of type @c T
     * @warning No check is performed whether the object was really constructed
     */
    T* operator->();

    /**
     * Operator overload to use this class like a raw pointer - for const correctness
     * @returns Pointer to the storaged object of type @c T
     * @warning No check is performed whether the object was really constructed
     */
    const T* operator->() const;

    /**
     * Operator overload to use this class like a raw pointer
     * @returns Reference to the storaged object of type @c T
     * @warning No check is performed whether the object was really constructed
     */
    T& operator*();

    /**
     * Operator overload to use this class like a raw pointer - for const correctness
     * @returns Reference to the storaged object of type @c T
     * @warning No check is performed whether the object was really constructed
     */
    const T& operator*() const;

    /// Unitializes the storage and, if constructed, destructs the storaged object
    void reset();

    /**
     * Resets storage with @c data and, if constructed, destructs previously storaged object
     * @param[in] data Object of type @c T whose content gets copy constructed into the storage
     */
    void reset(const T& data);

    /**
     * Swap storage of *this with storage of @c other stack pointer
     * @param[in] other Object of type @c StackPtr whose content gets swapped with *this
     * @note The storage is swapped byte-by-byte, so depending on the size this operation might be
     *       very time consuming.
     */
    void swap(StackPtr& other);

private: // types and constants
    /// Calculate entire size of the _storage - adds padding bytes to comply to the alignment
    enum StackPtrIntrospection {
        /// Number of bytes required to store control flags
        NumFlagBytes = 1,
        /// Size of the overhead containing the flags (complying to alignment)
        OverheadSize = (Alignment - (StackSize % Alignment)),
        /// Entire size of the _storage, including aligned overhead for necessary flags
        StorageSize = StackSize + OverheadSize,
        /// First byte of the StackPtr flag area
        FirstFlagPosition = StackSize
    };

    /// Absolute bit positions of the flags inside the overhead area of the storage
    enum StackPtrFlagPositions {
        InitializeStatus = FirstFlagPosition //!< Flag position for the initialize status
    };

    /// Flags for @c StackPtrFlagPositions
    enum class StackPtrFlags : char {
        IsDestroyed = 0x00,  //!< for InitializeStatus --> contained object not constructed
        IsConstructed = 0x01 //!< for InitializeStatus --> contained object constructed
    };

private:
    /**
     * Get pointer to beginning of the storage
     * @return Pointer to the beginning of the storage
     */
    void* address();

    /**
     * Get pointer to beginning of the storage provided for const correctness
     * @return Pointer to the beginning of the storage
     */
    const void* address() const;

    /**
     * Sets a @c flag to the appropriate @c position in the overhead section of the storage
     * @param[in] position Position of the @c flag in the overhead area
     * @param[in] flag The flag to set
     */
    void setFlag(StackPtrFlagPositions position, StackPtrFlags flag);

    /**
     * Check whether the object hidden by the storage got constructed
     * @return @c true if the object was constructed, @c false otherwise.
     */
    bool isConstructed() const;

    /**
     * Compare for equality
     * @tparam U Managed type
     * @tparam SSize Size the managed type occupies
     * @tparam A Byte alignment of the storage
     * @param[in] lhs Left hand side object
     * @param[in] rhs Right hand side object
     * @return @c true if both objects are equal, @c false otherwise
     */
    template <typename U, std::size_t SSize, std::size_t A>
    friend bool operator==(const StackPtr<U, SSize, A>& lhs, const StackPtr<U, SSize, A>& rhs);

private:
    /// Aligned storage large enough to contain one object of type @c T and some overhead for flags
    char _storage[StorageSize];
};

/**
 * Swap storages of two objects of type @c StackPtr
 * @tparam T The storaged type
 * @tparam StackSize Size of allocated storage size
 * @tparam Alignment Alignment of the storage
 * @param[in,out] lhs Left hand side object
 * @param[in,out] rhs Right hand side object
 * @note The storages are swapped byte-by-byte, so depending on the @c StackSize this operation
 *       might be very time consuming.
 */
template <typename T, std::size_t StackSize, std::size_t Alignment>
void swap(StackPtr<T, StackSize, Alignment>& lhs, StackPtr<T, StackSize, Alignment>& rhs);

/**
 * Compare for inequality
 * @tparam T Managed type
 * @tparam StackSize Size the managed type occupies
 * @tparam Alignment Byte alignment of the storage
 * @param[in] lhs Left hand side object
 * @param[in] rhs Right hand side object
 * @return @c true if both objects are inequal, @c false otherwise
 */
template <typename T, std::size_t StackSize, std::size_t Alignment>
bool operator!=(const StackPtr<T, StackSize, Alignment>& lhs,
                const StackPtr<T, StackSize, Alignment>& rhs);

} // namespace memory
} // namespace a_util

/** @cond INTERNAL_DOCUMENTATION */
#include <a_util/memory/detail/stack_ptr_impl.h>
/** @endcond */

#endif // A_UTIL_UTIL_MEMORY_DETAIL_STACK_PTR_DECL_HEADER_INCLUDED
