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

#include <a_util/memory.h>

namespace a_util {
namespace memory {
// internal implementation class
struct MemoryBuffer::Implementation {
    std::uint8_t* buffer;
    std::size_t size;
    bool owned;

    Implementation() : buffer(nullptr), size(0), owned(true)
    {
    }
};

MemoryBuffer::MemoryBuffer() : _impl(new Implementation())
{
}

MemoryBuffer::MemoryBuffer(size_t initial_size) : _impl(new Implementation())
{
    allocate(initial_size);
}

MemoryBuffer::MemoryBuffer(void* buffer, std::size_t size) : _impl(new Implementation())
{
    attach(buffer, size);
}

MemoryBuffer& MemoryBuffer::operator=(const MemoryBuffer& other)
{
    if (this != &other) {
        allocate(other.getSize());
        if (getSize() > 0) {
            a_util::memory::copy(getPtr(), getSize(), other.getPtr(), other.getSize());
        }
    }
    return *this;
}

MemoryBuffer::~MemoryBuffer()
{
    reset();
}

MemoryBuffer::MemoryBuffer(const MemoryBuffer& other) : _impl(new Implementation())
{
    allocate(other.getSize());
    if (getSize() > 0) {
        a_util::memory::copy(getPtr(), getSize(), other.getPtr(), other.getSize());
    }
}

bool MemoryBuffer::allocate(std::size_t new_size)
{
    if (_impl->owned && _impl->buffer) {
        delete[] _impl->buffer;
    }

    _impl->buffer = nullptr;
    _impl->size = new_size;
    _impl->owned = true;

    if (_impl->size > 0) {
        _impl->buffer = new (std::nothrow) std::uint8_t[_impl->size]();
        if (!_impl->buffer) {
            reset();
            return false;
        }
    }

    return true;
}

void MemoryBuffer::reset()
{
    allocate(0);
}

std::size_t MemoryBuffer::getSize() const
{
    return _impl->size;
}

void* MemoryBuffer::getPtr() const
{
    return _impl->buffer;
}

void MemoryBuffer::attach(void* buffer, std::size_t size)
{
    reset();
    _impl->buffer = static_cast<std::uint8_t*>(buffer);
    _impl->size = size;
    _impl->owned = false;
}

void MemoryBuffer::swap(MemoryBuffer& other)
{
    _impl.swap(other._impl);
}

bool operator==(const MemoryBuffer& buf1, const MemoryBuffer& buf2)
{
    if (buf1.getPtr() == buf2.getPtr()) {
        return buf1.getSize() == buf2.getSize();
    }

    if (buf1.getSize() == 0 || buf2.getSize() == 0) {
        return false;
    }

    return compare(buf1.getPtr(), buf1.getSize(), buf2.getPtr(), buf2.getSize()) == 0;
}

bool operator!=(const MemoryBuffer& buf1, const MemoryBuffer& buf2)
{
    return !(buf1 == buf2);
}

} // namespace memory
} // namespace a_util
