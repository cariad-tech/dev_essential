/**
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

#include <a_util/concurrency/fast_mutex.h>

#if defined(_WIN32) && !defined(__MINGW32__)
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

namespace {

struct PlatformSpecific {
    using HandleType = CRITICAL_SECTION;
    static constexpr VOID (*initialize)(HandleType*) = &InitializeCriticalSection;
    static constexpr VOID (*destroy)(HandleType*) = &DeleteCriticalSection;
    static constexpr BOOL (*tryLock)(HandleType*) = &TryEnterCriticalSection;
    static constexpr VOID (*lock)(HandleType*) = &EnterCriticalSection;
    static constexpr VOID (*unlock)(HandleType*) = &LeaveCriticalSection;
};

} // namespace

#else // POSIX

#include <pthread.h>

namespace {

struct PlatformSpecific {
    using HandleType = pthread_mutex_t;
    static inline int initialize(HandleType* __restrict__ mutex,
                                 const pthread_mutexattr_t* __restrict__ attr = nullptr)
    {
        return pthread_mutex_init(mutex, attr);
    }
    static constexpr int (*destroy)(HandleType*) = &pthread_mutex_destroy;
    static inline bool tryLock(HandleType* __restrict__ mutex)
    {
        return pthread_mutex_trylock(mutex) == 0;
    }
    static constexpr int (*lock)(HandleType*) = &pthread_mutex_lock;
    static constexpr int (*unlock)(HandleType*) = &pthread_mutex_unlock;
};

} // namespace

#endif // _WIN32

namespace a_util {
namespace concurrency {

class fast_mutex::Implementation {
public:
    Implementation() : _handle{}
    {
        PlatformSpecific::initialize(&_handle);
    }

    ~Implementation()
    {
        PlatformSpecific::destroy(&_handle);
    }

    Implementation(const Implementation&) = delete;
    Implementation& operator=(const Implementation&) = delete;
    Implementation(Implementation&&) = delete;
    Implementation& operator=(Implementation&&) = delete;

    PlatformSpecific::HandleType* getHandle() noexcept
    {
        return &_handle;
    }

private:
    PlatformSpecific::HandleType _handle;
};

fast_mutex::fast_mutex() : _impl{}
{
}

fast_mutex::~fast_mutex() = default;

void fast_mutex::lock()
{
    PlatformSpecific::lock(_impl->getHandle());
}

bool fast_mutex::try_lock()
{
    return PlatformSpecific::tryLock(_impl->getHandle()) != 0;
}

void fast_mutex::unlock()
{
    PlatformSpecific::unlock(_impl->getHandle());
}

fast_mutex::native_handle_type fast_mutex::native_handle()
{
    return _impl->getHandle();
}

} // namespace concurrency
} // namespace a_util
