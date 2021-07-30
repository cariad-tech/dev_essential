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

If it is not possible or desirable to put the notice in a particular file, then
You may include the notice in a location (such as a LICENSE file in a
relevant directory) where a recipient would be likely to look for such a notice.

You may add additional accurate notices of copyright ownership.
@endverbatim
 */

#ifndef A_UTIL_UTIL_CONCURRENCY_DETAIL_FAST_MUTEX_DECL_HEADER_INCLUDED
#define A_UTIL_UTIL_CONCURRENCY_DETAIL_FAST_MUTEX_DECL_HEADER_INCLUDED

#ifdef WIN32
#ifndef NOMINMAX
#define UNDEF_NOMINMAX
#define NOMINMAX
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define UNDEF_WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>

#ifdef UNDEF_NOMINMAX
#undef NOMINMAX
#undef UNDEF_NOMINMAX
#endif

#ifdef UNDEF_WIN32_LEAN_AND_MEAN
#undef WIN32_LEAN_AND_MEAN
#undef UNDEF_WIN32_LEAN_AND_MEAN
#endif
#else
#include <pthread.h>
#endif

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
     * @return Pointer to the native handle of the mutex
     */
    native_handle_type native_handle();

private:
#ifdef WIN32
    CRITICAL_SECTION _handle;
#else
    pthread_mutex_t _handle;
#endif
};

} // namespace concurrency
} // namespace a_util

/** @cond INTERNAL_DOCUMENTATION */
#include "a_util/concurrency/detail/fast_mutex_impl.h"
/** @endcond */

#endif // A_UTIL_UTIL_CONCURRENCY_DETAIL_FAST_MUTEX_DECL_HEADER_INCLUDED
