/**
 * @file
 * Private implementation for @ref a_util::memory::StackPtr "StackPtr" type and functions
 *
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */

#ifndef A_UTIL_UTIL_MEMORY_DETAIL_STACK_PTR_IMPL_HEADER_INCLUDED
#define A_UTIL_UTIL_MEMORY_DETAIL_STACK_PTR_IMPL_HEADER_INCLUDED

#include <a_util/memory/detail/stack_ptr_decl.h>

namespace a_util {
namespace memory {

template <typename T, std::size_t StackSize, std::size_t Alignment>
inline StackPtr<T, StackSize, Alignment>::StackPtr() : _storage()
{
    new (address()) T();
    setFlag(InitializeStatus, StackPtrFlags::IsConstructed);
}

template <typename T, std::size_t StackSize, std::size_t Alignment>
inline StackPtr<T, StackSize, Alignment>::StackPtr(std::nullptr_t) : _storage()
{
}

template <typename T, std::size_t StackSize, std::size_t Alignment>
inline StackPtr<T, StackSize, Alignment>::StackPtr(const T& data) : _storage()
{
    reset(data);
}

template <typename T, std::size_t StackSize, std::size_t Alignment>
inline StackPtr<T, StackSize, Alignment>::StackPtr(T&& data) : _storage()
{
    reset(std::move(data));
}

template <typename T, std::size_t StackSize, std::size_t Alignment>
inline StackPtr<T, StackSize, Alignment>::~StackPtr()
{
    static_assert(StackSize >= sizeof(T), "Array size must be larger");
    static_assert(alignof(T) <= Alignment, "Alignment must match alignment requirement of T");
    reset();
}

template <typename T, std::size_t StackSize, std::size_t Alignment>
inline StackPtr<T, StackSize, Alignment>::StackPtr(const StackPtr& other) : _storage()
{
    if (other.isConstructed()) {
        reset(*other);
    }
}

template <typename T, std::size_t StackSize, std::size_t Alignment>
inline StackPtr<T, StackSize, Alignment>& StackPtr<T, StackSize, Alignment>::operator=(
    StackPtr other)
{
    other.swap(*this);
    return *this;
}

template <typename T, std::size_t StackSize, std::size_t Alignment>
inline StackPtr<T, StackSize, Alignment>::StackPtr(StackPtr&& other) : _storage()
{
    if (other.isConstructed()) {
        this->reset(std::move(*other));
    }
}

template <typename T, std::size_t StackSize, std::size_t Alignment>
inline T* StackPtr<T, StackSize, Alignment>::operator->()
{
    return static_cast<T*>(address());
}

template <typename T, std::size_t StackSize, std::size_t Alignment>
inline const T* StackPtr<T, StackSize, Alignment>::operator->() const
{
    return static_cast<const T*>(address());
}

template <typename T, std::size_t StackSize, std::size_t Alignment>
inline T& StackPtr<T, StackSize, Alignment>::operator*()
{
    return *(operator->());
}

template <typename T, std::size_t StackSize, std::size_t Alignment>
inline const T& StackPtr<T, StackSize, Alignment>::operator*() const
{
    return *(operator->());
}

template <typename T, std::size_t StackSize, std::size_t Alignment>
inline StackPtr<T, StackSize, Alignment>::operator bool() const noexcept
{
    return isConstructed();
}

template <typename T, std::size_t StackSize, std::size_t Alignment>
inline void StackPtr<T, StackSize, Alignment>::reset()
{
    if (isConstructed()) {
        static_cast<T*>(address())->~T();
        setFlag(InitializeStatus, StackPtrFlags::IsDestroyed);
    }
}

template <typename T, std::size_t StackSize, std::size_t Alignment>
template <typename... Args>
inline void StackPtr<T, StackSize, Alignment>::reset(Args&&... args)
{
    reset();
    new (address()) T(std::forward<Args>(args)...);
    setFlag(InitializeStatus, StackPtrFlags::IsConstructed);
}

template <typename T, std::size_t StackSize, std::size_t Alignment>
inline void StackPtr<T, StackSize, Alignment>::swap(StackPtr& other)
{
    using std::swap;
    StackPtr* reset_after_swap = nullptr;
    std::size_t remaining_bytes_to_swap = OverheadSize;

    if (this->isConstructed()) {
        if (!other.isConstructed()) {
            other.reset(T());
            reset_after_swap = this;
        }
        swap(*(*this), *other);
    }
    else if (other.isConstructed()) {
        this->reset(T());
        reset_after_swap = &other;
        swap(*(*this), *other);
    }
    else {
        remaining_bytes_to_swap = StorageSize;
    }

    // either swap overhead or the entire storage
    while (remaining_bytes_to_swap) {
        using std::swap;
        const std::size_t storage_index = StorageSize - remaining_bytes_to_swap--;
        swap(this->_storage[storage_index], other._storage[storage_index]);
    }

    if (reset_after_swap) {
        reset_after_swap->reset();
    }
}

template <typename T, std::size_t StackSize, std::size_t Alignment>
inline void* StackPtr<T, StackSize, Alignment>::address()
{
    return &_storage;
}

template <typename T, std::size_t StackSize, std::size_t Alignment>
inline const void* StackPtr<T, StackSize, Alignment>::address() const
{
    return &_storage;
}

template <typename T, std::size_t StackSize, std::size_t Alignment>
inline void StackPtr<T, StackSize, Alignment>::setFlag(StackPtrFlagPositions position,
                                                       StackPtrFlags flag)
{
    _storage[position] = static_cast<typename std::underlying_type<StackPtrFlags>::type>(flag);
}

template <typename T, std::size_t StackSize, std::size_t Alignment>
inline bool StackPtr<T, StackSize, Alignment>::isConstructed() const noexcept
{
    return _storage[InitializeStatus] ==
           static_cast<typename std::underlying_type<StackPtrFlags>::type>(
               StackPtrFlags::IsConstructed);
}

template <typename T, std::size_t StackSize, std::size_t Alignment>
inline void swap(StackPtr<T, StackSize, Alignment>& lhs, StackPtr<T, StackSize, Alignment>& rhs)
{
    lhs.swap(rhs);
}

template <typename T, std::size_t StackSize, std::size_t Alignment>
inline bool operator==(const StackPtr<T, StackSize, Alignment>& lhs,
                       const StackPtr<T, StackSize, Alignment>& rhs)
{
    std::size_t remaining_bytes_to_compare = StackPtr<T, StackSize, Alignment>::OverheadSize;
    if (!lhs.isConstructed() && !rhs.isConstructed()) {
        remaining_bytes_to_compare = StackPtr<T, StackSize, Alignment>::StorageSize;
    }
    else if (lhs.isConstructed() ^ rhs.isConstructed()) {
        return false;
    }
    else if (!(*lhs == *rhs)) {
        // fully intended, requires only comparison operator!
        return false;
    }

    // don't use memory::compare to avoid link dependency to memory component
    bool is_equal = true;
    while (remaining_bytes_to_compare && is_equal) {
        const std::size_t storage_index =
            StackPtr<T, StackSize, Alignment>::StorageSize - remaining_bytes_to_compare--;
        is_equal = lhs._storage[storage_index] == rhs._storage[storage_index];
    }
    return is_equal;
}

template <typename T, std::size_t StackSize, std::size_t Alignment>
inline bool operator!=(const StackPtr<T, StackSize, Alignment>& lhs,
                       const StackPtr<T, StackSize, Alignment>& rhs)
{
    return !(lhs == rhs);
}

template <typename T, std::size_t StackSize, std::size_t Alignment, typename... Args>
constexpr inline auto makeStackPtr(Args&&... args) -> StackPtr<T, StackSize, Alignment>
{
    return StackPtr<T, StackSize, Alignment>(T(std::forward<Args>(args)...));
}

} // namespace memory
} // namespace a_util

#endif // A_UTIL_UTIL_MEMORY_DETAIL_STACK_PTR_IMPLL_HEADER_INCLUDED
