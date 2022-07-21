/**
 * @file
 * Memory buffer test implementation
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

#include <a_util/memory/memory.h>
#include <a_util/memory/memorybuffer.h>

#include <gtest/gtest.h>

#include <algorithm>
#include <array>

class MemoryBufferTestFixture : public ::testing::Test {
public:
    MemoryBufferTestFixture()
        : _random_alloc_sizes{0x02,
                              0x04,
                              0x08,
                              0x10,
                              0x20,
                              0x40,
                              0x80,
                              0x100,
                              0x200,
                              0x400,
                              0x800,
                              0x1000,
                              0x2000,
                              0x4000,
                              0x8000,
                              0x10000}
    {
    }

protected:
    using MemoryBuffer = a_util::memory::MemoryBuffer;

    const std::array<std::size_t, 16>& allocSizes() const noexcept
    {
        return _random_alloc_sizes;
    }

private:
    std::array<std::size_t, 16> _random_alloc_sizes;
};

/// Test a_util::memory::MemoryBuffer
TEST_F(MemoryBufferTestFixture, TestMemoryBuffer)
{
    // CTOR
    MemoryBuffer buf1;
    EXPECT_TRUE(buf1.getPtr() == nullptr);
    EXPECT_TRUE(buf1.getSize() == 0);

    // Assignment op & initializing CTOR
    MemoryBuffer buf2(42);
    buf1 = buf2;
    EXPECT_TRUE(buf2.getPtr() != nullptr);
    EXPECT_TRUE(buf2.getSize() == 42);
    EXPECT_TRUE(buf1.getPtr() != nullptr);
    EXPECT_TRUE(buf1.getSize() == 42);

    EXPECT_TRUE(set(buf1, 0, buf1.getSize()));
    EXPECT_TRUE(set(buf2, 1, buf2.getSize()));

    // Comparison operator
    EXPECT_FALSE(buf1 == buf2);
    EXPECT_TRUE(buf1 != buf2);
    EXPECT_TRUE(zero(buf2, buf2.getSize()));
    EXPECT_TRUE(buf1 == buf2);
    EXPECT_FALSE(buf1 != buf2);

    EXPECT_TRUE(buf1.allocate(0));
    EXPECT_TRUE(buf1.getPtr() == nullptr);
    EXPECT_TRUE(buf1.getSize() == 0);

    EXPECT_TRUE(buf1.allocate(21));
    EXPECT_TRUE(buf1.getPtr() != nullptr);
    EXPECT_TRUE(buf1.getSize() == 21);

    // swap
    EXPECT_TRUE(buf1.getSize() == 21);
    EXPECT_TRUE(buf2.getSize() == 42);
    buf1.swap(buf2);
    EXPECT_TRUE(buf2.getSize() == 21);
    EXPECT_TRUE(buf1.getSize() == 42);
}

/**
 * New memory buffer is zero initialized
 * @req_id ODAUTIL-142
 */
TEST_F(MemoryBufferTestFixture, newMemoryBufferIsZeroInitialized)
{
    std::for_each(allocSizes().cbegin(), allocSizes().cend(), [](std::size_t size_in_bytes) {
        MemoryBuffer memory_buffer(size_in_bytes);
        EXPECT_TRUE(a_util::memory::isZero(memory_buffer.getPtr(), size_in_bytes));
    });
}

/**
 * Allocated buffer is zero initialized
 * @req_id ODAUTIL-142
 */
TEST_F(MemoryBufferTestFixture, allocatedBufferIsZeroInitialized)
{
    std::for_each(allocSizes().cbegin(), allocSizes().cend(), [](std::size_t size_in_bytes) {
        MemoryBuffer memory_buffer;
        EXPECT_TRUE(memory_buffer.allocate(size_in_bytes));
        EXPECT_TRUE(a_util::memory::isZero(memory_buffer.getPtr(), size_in_bytes));
    });
}

/**
 * A reallocated buffer is always zero initialized
 * @req_id ODAUTIL-142
 */
TEST_F(MemoryBufferTestFixture, reallocatedBufferIsZeroInitialized)
{
    using a_util::memory::isZero;
    using a_util::memory::set;

    std::for_each(allocSizes().cbegin(), allocSizes().cend(), [](std::size_t size_in_bytes) {
        const std::size_t smaller_size_in_bytes = size_in_bytes - 1;
        const std::size_t larger_size_in_bytes = size_in_bytes + 1;
        MemoryBuffer memory_buffer(size_in_bytes);

        EXPECT_TRUE(set(memory_buffer, 42, memory_buffer.getSize()));
        EXPECT_TRUE(memory_buffer.allocate(size_in_bytes));
        EXPECT_TRUE(isZero(memory_buffer.getPtr(), size_in_bytes));

        EXPECT_TRUE(set(memory_buffer, 42, memory_buffer.getSize()));
        EXPECT_TRUE(memory_buffer.allocate(smaller_size_in_bytes));
        EXPECT_TRUE(isZero(memory_buffer.getPtr(), smaller_size_in_bytes));

        EXPECT_TRUE(set(memory_buffer, 42, memory_buffer.getSize()));
        EXPECT_TRUE(memory_buffer.allocate(larger_size_in_bytes));
        EXPECT_TRUE(isZero(memory_buffer.getPtr(), larger_size_in_bytes));
    });
}
