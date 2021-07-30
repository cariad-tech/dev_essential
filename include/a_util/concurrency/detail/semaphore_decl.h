/**
 * @file
 * Public API for @ref a_util::concurrency::detail::basic_semaphore "basic_semaphore" type
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

#ifndef A_UTIL_UTIL_CONCURRENCY_DETAIL_SEMAPHORE_DECL_HEADER_INCLUDED
#define A_UTIL_UTIL_CONCURRENCY_DETAIL_SEMAPHORE_DECL_HEADER_INCLUDED

#include <chrono>

namespace a_util {
namespace concurrency {
namespace detail {
/**
 * Semaphore implementation, combining a mutex and a condition variable to manage a counter
 *
 * For the default semaphore type to use, check @ref a_util::concurrency::semaphore.
 * @tparam Mutex The mutex type
 * @tparam CondVar The condition variable type
 */
template <typename Mutex, typename CondVar>
class basic_semaphore {
    basic_semaphore(const basic_semaphore&);            // = delete;
    basic_semaphore& operator=(const basic_semaphore&); // = delete;

public:
    /**
     * CTOR
     * @param[in] count Initial value of the counter
     */
    explicit basic_semaphore(int count = 0);

    /// Increment the counter and notify any waiters
    void notify();

    /// Decrement the counter, blocks until the count becomes non-zero (if neccessary)
    void wait();

    /**
     * Try decrementing the counter
     * @return @c true if the counter is greater than zero, @c false otherwise
     */
    bool try_wait();

    /**
     * Wait for a specified duration of time and decrement the counter afterwards
     * @tparam Rep An arithmetic type representing the number of ticks
     * @tparam std::ratio representing the tick period (i.e. the number of seconds per tick)
     * @param[in] timeout The duration to wait.
     * @throw std::invalid_argument If <tt>timeout.count < 0</tt>
     * @return @c true if the counter could be decreased after the waiting time, @c false otherwise
     */
    template <typename Rep, typename Period>
    bool wait_for(const std::chrono::duration<Rep, Period>& timeout);

    /// Reset the counter to 0
    void reset();

    /**
     * Check whether the counter is set.
     * @return @c true if the counter is greater than zero, @c false otherwise.
     */
    bool is_set();

protected:
    Mutex _mutex;    //!< The mutex to handle
    CondVar _cv;     //!< The condition variable used to handle the notifications
    int _lock_count; //!< The lock count
};

} // namespace detail
} // namespace concurrency
} // namespace a_util

/** @cond INTERNAL_DOCUMENTATION */
#include "a_util/concurrency/detail/semaphore_impl.h"
/** @endcond */

#endif // A_UTIL_UTIL_CONCURRENCY_DETAIL_SEMAPHORE_DECL_HEADER_INCLUDED
