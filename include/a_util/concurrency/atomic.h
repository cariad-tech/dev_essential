/**
 * @file
 * Public API for @ref a_util::concurrency::memoryBarrier "memoryBarrier" function
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

namespace a_util {
namespace concurrency {
/**
 * Inserts a memory fence instruction, causing the CPU to enforce strict memory ordering before
 * and after the instruction
 */
void memoryBarrier();

} // namespace concurrency
} // namespace a_util

#endif // A_UTIL_UTIL_CONCURRENCY_ATOMIC_HEADER_INCLUDED
