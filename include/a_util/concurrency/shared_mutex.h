/**
 * @file
 * Public API for @ref a_util::concurrency::shared_mutex "shared_mutex" type
 *
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */

#ifndef A_UTIL_UTIL_CONCURRENCY_SHARED_MUTEX_HEADER_INCLUDED
#define A_UTIL_UTIL_CONCURRENCY_SHARED_MUTEX_HEADER_INCLUDED

#include <memory>

namespace a_util {
namespace concurrency {
/**
 * A shared_mutex class as a workaround for std::shared_timed_mutex
 * @see https://stackoverflow.com/a/42302130
 */
class shared_mutex {
    shared_mutex(const shared_mutex&);            // = delete;
    shared_mutex& operator=(const shared_mutex&); // = delete;

    struct impl;
    std::unique_ptr<impl> p;

public:
    /// CTOR - creates the mutex in an unlocked state
    shared_mutex();

    /// DTOR - does not call unlock!
    ~shared_mutex();

    /// Lock the mutex, blocks if the mutex is not available
    void lock();

    /**
     * Try to lock the mutex.
     * @return @c true if successfully locked, @c false otherwise.
     */
    bool try_lock();

    /// Unlock the mutex
    void unlock();

    /// Lock the mutex for shared ownership, blocks if the mutex is not available
    void lock_shared();

    /**
     * Try to lock the mutex for shared ownership
     * @return @c true if successfully locked, @c false otherwise
     */
    bool try_lock_shared();

    /// Unlock the mutex (shared ownership)
    void unlock_shared();
};

} // namespace concurrency
} // namespace a_util

#endif // A_UTIL_UTIL_CONCURRENCY_SHARED_MUTEX_HEADER_INCLUDED
