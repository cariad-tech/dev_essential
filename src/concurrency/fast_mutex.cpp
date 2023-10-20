/**
 * Copyright @ 2021 VW Group. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#if defined(_WIN32) && !defined(__MINGW32__)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif // defined(_WIN32) && !defined(__MINGW32__)

#include <a_util/concurrency/fast_mutex.h>

#if defined(_WIN32) && !defined(__MINGW32__)

namespace {

struct PlatformSpecific {
    using HandleType = CRITICAL_SECTION;
    static inline VOID initialize(HandleType* mutex)
    {
        InitializeCriticalSection(mutex);
    }
    static inline VOID destroy(HandleType* mutex)
    {
        DeleteCriticalSection(mutex);
    }
    static inline BOOL tryLock(HandleType* mutex)
    {
        return TryEnterCriticalSection(mutex);
    };
    static inline VOID lock(HandleType* mutex)
    {
        EnterCriticalSection(mutex);
    }
    static inline VOID unlock(HandleType* mutex)
    {
        LeaveCriticalSection(mutex);
    }
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
