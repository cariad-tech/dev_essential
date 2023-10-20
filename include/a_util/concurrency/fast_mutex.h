/**
 * @file
 * Public API for @ref a_util::concurrency::fast_mutex "fast_mutex" type
 *
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */

#ifndef A_UTIL_UTIL_CONCURRENCY_FAST_MUTEX_HEADER_INCLUDED
#define A_UTIL_UTIL_CONCURRENCY_FAST_MUTEX_HEADER_INCLUDED

#include <a_util/memory/stack_ptr.h>

namespace a_util {
namespace concurrency {
/// Fast mutex class, implemented as an atomic spin lock with very low CPU overhead
class fast_mutex {
public: // types
    /// Type of a native handle
    typedef void* native_handle_type;

public:
    /// CTOR
    fast_mutex();

    /// DTOR
    ~fast_mutex();

    /// Lock the mutex
    void lock();

    /**
     * Try to lock the mutex
     * @return @c true if locking succeeded, @c false otherwise.
     */
    bool try_lock();

    /// Unlock the mutex
    void unlock();

    /**
     * Get the native handle of the mutex
     * @return Pointer to the native handle of the mutex.
     *         On Windows this is a pointer to struct CRITICAL_SECTION.
     *         On POSIX this is a pointer to struct pthread_mutex_t.
     */
    native_handle_type native_handle();

private:
    class Implementation;
    a_util::memory::StackPtr<Implementation, 64> _impl;
};

} // namespace concurrency
} // namespace a_util

#endif // A_UTIL_UTIL_CONCURRENCY_FAST_MUTEX_HEADER_INCLUDED
