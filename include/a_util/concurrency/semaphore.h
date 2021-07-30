/**
 * @file
 * Public API for @ref a_util::concurrency::semaphore "semaphore" type
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

#ifndef A_UTIL_UTIL_CONCURRENCY_SEMAPHORE_HEADER_INCLUDED
#define A_UTIL_UTIL_CONCURRENCY_SEMAPHORE_HEADER_INCLUDED

#include "a_util/concurrency/detail/semaphore_decl.h"

#include <condition_variable>
#include <mutex>

namespace a_util {
namespace concurrency {
/**
 * Default semaphore using
 * <a href="https://en.cppreference.com/w/cpp/thread/mutex">std::mutex</a> and
 * <a href="https://en.cppreference.com/w/cpp/thread/condition_variable">std::condition_variable
 * </a>
 */
typedef detail::basic_semaphore<std::mutex, std::condition_variable> semaphore;

} // namespace concurrency
} // namespace a_util

#endif // A_UTIL_UTIL_CONCURRENCY_SEMAPHORE_HEADER_INCLUDED
