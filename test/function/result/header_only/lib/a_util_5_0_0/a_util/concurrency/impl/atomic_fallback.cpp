/**
 * @file
 * Atomics
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

//#ifndef HAVE_ATOMIC

#include <a_util/base/std/types.h>
#include <a_util/concurrency/atomic.h>

#ifdef _MSC_VER
#include <intrin.h>

namespace a_util {
namespace concurrency {

std::int32_t atomicCompareExchange(std::int32_t& dest,
                                   std::int32_t value,
                                   std::int32_t expected_value)
{
    return ::_InterlockedCompareExchange((long volatile*)&dest, value, expected_value);
}

std::int64_t atomicCompareExchange(std::int64_t& dest,
                                   std::int64_t value,
                                   std::int64_t expected_value)
{
    return ::_InterlockedCompareExchange64((__int64 volatile*)&dest, value, expected_value);
}

std::int32_t atomicIncrement(std::int32_t& dest)
{
    return ::_InterlockedIncrement((long volatile*)&dest);
}

std::int64_t atomicIncrement(std::int64_t& dest)
{
#ifdef _WIN64
    return ::_InterlockedIncrement64((__int64 volatile*)&dest);
#else
    std::int64_t current;
    do {
        current = dest;
    } while (atomicCompareExchange(dest, current + 1, current) != current);
    return current + 1;
#endif
}

std::int32_t atomicDecrement(std::int32_t& dest)
{
    return ::_InterlockedDecrement((long volatile*)&dest);
}

std::int64_t atomicDecrement(std::int64_t& dest)
{
#ifdef _WIN64
    return ::_InterlockedDecrement64((__int64 volatile*)&dest);
#else
    std::int64_t current;
    do {
        current = dest;
    } while (atomicCompareExchange(dest, current - 1, current) != current);
    return current - 1;
#endif
}

std::int32_t atomicAdd(std::int32_t& dest, std::int32_t value)
{
    return ::_InterlockedExchangeAdd((long volatile*)&dest, value) + value;
}

std::int64_t atomicAdd(std::int64_t& dest, std::int64_t value)
{
#ifdef _WIN64
    return ::_InterlockedExchangeAdd64((__int64 volatile*)&dest, value) + value;
#else
    std::int64_t current;
    do {
        current = dest;
    } while (atomicCompareExchange(dest, current + value, current) != current);
    return current + value;
#endif
}

void memoryBarrier()
{
    ::_ReadWriteBarrier();
}

#else // gcc

namespace a_util {
namespace concurrency {

std::int32_t atomicIncrement(std::int32_t& dest)
{
    return __sync_fetch_and_add(&dest, 1) + 1;
}

std::int64_t atomicIncrement(std::int64_t& dest)
{
    return __sync_fetch_and_add(&dest, 1) + 1;
}

std::int32_t atomicDecrement(std::int32_t& dest)
{
    return __sync_fetch_and_sub(&dest, 1) - 1;
}

std::int64_t atomicDecrement(std::int64_t& dest)
{
    return __sync_fetch_and_sub(&dest, 1) - 1;
}

std::int32_t atomicAdd(std::int32_t& dest, std::int32_t value)
{
    return __sync_fetch_and_add(&dest, value) + value;
}

std::int64_t atomicAdd(std::int64_t& dest, std::int64_t value)
{
    return __sync_fetch_and_add(&dest, value) + value;
}

std::int32_t atomicCompareExchange(std::int32_t& dest,
                                   std::int32_t value,
                                   std::int32_t expected_value)
{
    return __sync_val_compare_and_swap(&dest, expected_value, value);
}

std::int64_t atomicCompareExchange(std::int64_t& dest,
                                   std::int64_t value,
                                   std::int64_t expected_value)
{
    return __sync_val_compare_and_swap(&dest, expected_value, value);
}

void memoryBarrier()
{
    __sync_synchronize();
}

#endif //_MSC_VER

} // ns concurrency
} // ns a_util

//#endif //HAVE_ATOMIC
