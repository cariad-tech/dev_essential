/**
 * @file
 * Private implementation for @ref a_util::concurrency::fast_mutex "fast_mutex" type
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

#ifndef A_UTIL_UTIL_CONCURRENCY_DETAIL_FAST_MUTEX_IMPL_HEADER_INCLUDED
#define A_UTIL_UTIL_CONCURRENCY_DETAIL_FAST_MUTEX_IMPL_HEADER_INCLUDED

#include "a_util/concurrency/detail/fast_mutex_decl.h"

namespace a_util {
namespace concurrency {
#ifdef WIN32

inline fast_mutex::fast_mutex()
{
    InitializeCriticalSection(&_handle);
}

inline fast_mutex::~fast_mutex()
{
    DeleteCriticalSection(&_handle);
}

inline void fast_mutex::lock()
{
    EnterCriticalSection(&_handle);
}

inline bool fast_mutex::try_lock()
{
    return TryEnterCriticalSection(&_handle) ? true : false;
}

inline void fast_mutex::unlock()
{
    LeaveCriticalSection(&_handle);
}

#else // POSIX

inline fast_mutex::fast_mutex()
{
    pthread_mutex_init(&_handle, nullptr);
}

inline fast_mutex::~fast_mutex()
{
    pthread_mutex_destroy(&_handle);
}

inline void fast_mutex::lock()
{
    pthread_mutex_lock(&_handle);
}

inline bool fast_mutex::try_lock()
{
    return (pthread_mutex_trylock(&_handle) == 0) ? true : false;
}

inline void fast_mutex::unlock()
{
    pthread_mutex_unlock(&_handle);
}

#endif

inline fast_mutex::native_handle_type fast_mutex::native_handle()
{
    return static_cast<native_handle_type>(&_handle);
}

} // namespace concurrency
} // namespace a_util

#endif // A_UTIL_UTIL_CONCURRENCY_DETAIL_FAST_MUTEX_IMPL_HEADER_INCLUDED
