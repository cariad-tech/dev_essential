/**
 * @file
 * Atomic implementation header
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

#ifndef A_UTIL_UTIL_CONCURRENCY_ATOMIC_HEADER_INCLUDED
#define A_UTIL_UTIL_CONCURRENCY_ATOMIC_HEADER_INCLUDED

#include "a_util/base/std/types.h"

namespace a_util {
namespace concurrency {

/**
 * Atomically exchanges a variable, if the old value still holds an expected value
 * @param dest [in] The destination variable
 * @param value [in] The new value
 * @param expected_old_value [in] The expected old value
 * @retval The actual old value of dest, before the exchange
 */
std::int32_t atomicCompareExchange(std::int32_t& dest,
                                   std::int32_t value,
                                   std::int32_t expected_old_value);
// @overload
std::int64_t atomicCompareExchange(std::int64_t& dest,
                                   std::int64_t value,
                                   std::int64_t expected_old_value);

/**
 * Atomically increments a variable
 * @param dest [in] The destination variable
 * @retval The new value of the variable, after the increment
 */
std::int32_t atomicIncrement(std::int32_t& dest);
// @overload
std::int64_t atomicIncrement(std::int64_t& dest);

/**
 * Atomically decrements a variable
 * @param dest [in] The destination variable
 * @retval The new value of the variable, after the decrement
 */
std::int32_t atomicDecrement(std::int32_t& dest);
// @overload
std::int64_t atomicDecrement(std::int64_t& dest);

/**
 * Atomically adds a value to a variable
 * @param dest [in] The destination variable
 * @param value [in] The value to be added
 * @retval The new value of the variable, after the addition
 */
std::int32_t atomicAdd(std::int32_t& dest, std::int32_t value);
// @overload
std::int64_t atomicAdd(std::int64_t& dest, std::int64_t value);

/**
 * Inserts a memory fence instruction, causing the CPU to
 * enforce strict memory ordering before and after the instruction
 */
void memoryBarrier();

} // namespace concurrency
} // namespace a_util

#endif // A_UTIL_UTIL_CONCURRENCY_ATOMIC_HEADER_INCLUDED
