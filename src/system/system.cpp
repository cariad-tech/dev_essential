/**
 * @file
 * System API abstractions
 *
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

#include <a_util/result/result_info.h>
#include <a_util/system.h>

#ifdef _WIN32

#ifndef __MINGW32__
#define NOMINMAX
#endif // __MINGW32__

#include <Windows.h> // Sleep, GetUserName, timeBeginPeriod, timeEndPeriod, GetLastError, FormatMessage
// must be after Windows.h
#include <Lmcons.h> // UNLEN
#else
#if defined(__QNX__) && !defined(_QNX_SOURCE)

#define _QNX_SOURCE
#endif

#include <cstring>     // strerror
#include <errno.h>     // errno
#include <pwd.h>       // getpwuid
#include <sys/types.h> // passwd
#include <time.h>      // timespec, nanosleep
#include <unistd.h>    // getuid

#endif

namespace a_util {
namespace system {
#ifdef __QNX__
std::string& currentUserName()
{
    static std::string current_user_name;
    return current_user_name;
}
#endif

std::string getCurrentUserName()
{
#ifdef _WIN32
#if defined(_MSC_VER) || defined(__MINGW32__)
    char array[UNLEN];
#else
    char array[256];
#endif
    DWORD array_size = sizeof(array) / sizeof(char);

#ifdef UNICODE
    ::GetUserNameW(array, &array_size);
#else
    ::GetUserNameA(array, &array_size);
#endif

    return array;

#elif defined(__QNX__)
    if (!currentUserName().empty()) {
        return currentUserName();
    }
    passwd* passwd = getpwuid(getuid());
    if (passwd) {
        currentUserName() = passwd->pw_name;
        return currentUserName();
    }
    return std::string();
#else  // _WIN32
    passwd* passwd = getpwuid(getuid());
    if (passwd) {
        return passwd->pw_name;
    }
    return std::string();
#endif // _WIN32
}

std::string getHostname()
{
#ifdef _WIN32
#if defined(_MSC_VER) || defined(__MINGW32__)
    char array[MAX_COMPUTERNAME_LENGTH + 1];
#else
    char array[256];
#endif
    DWORD size = (DWORD)(sizeof(array) / sizeof(char));

#ifdef UNICODE
    ::GetComputerNameW(array, &size);
#else
    ::GetComputerNameA(array, &size);
#endif

    return array;
#else
    char array[512];
    if (gethostname(array, 512) != 0) {
        return std::string();
    }

    return array;
#endif // _WIN32
}

a_util::filesystem::Path getExecutablePath()
{
#if defined(_MSC_VER) || (defined(_WIN32) && defined(__MINGW32__))
    // Get path of current running exec
    char curPath[MAX_PATH]{};
    GetModuleFileName(NULL, curPath, sizeof(curPath));
#else  // unix based
    // Get path of current running exec
    char curPath[4096]{};
    ssize_t len = readlink("/proc/self/exe", curPath, sizeof(curPath) - 1);
    curPath[len] = '\0';
#endif // Version check
    return a_util::filesystem::Path(curPath);
}

int getLastSystemError()
{
#ifdef _WIN32
    return static_cast<int>(::GetLastError());
#else
    return errno;
#endif
}

void resetLastSystemError()
{
#ifdef _WIN32
    ::SetLastError(static_cast<DWORD>(a_util::result::SUCCESS.getCode()));
#else
    errno = a_util::result::SUCCESS.getCode();
#endif // _WIN32
}

std::string formatSystemError(int system_error_code)
{
#ifdef _WIN32
    void* dest_buffer = nullptr;
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
                      FORMAT_MESSAGE_IGNORE_INSERTS,
                  NULL,
                  static_cast<DWORD>(system_error_code),
                  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                  reinterpret_cast<LPTSTR>(&dest_buffer),
                  0,
                  NULL);

    std::string error_desc;
    if (nullptr != dest_buffer) {
        error_desc.assign(static_cast<const char*>(dest_buffer));

        // Free the buffer.
        LocalFree(dest_buffer);
    }

    return error_desc;
#else
    return std::string(strerror(system_error_code));
#endif // _WIN32
}

void sleepMilliseconds(std::uint32_t ms)
{
    sleepMicroseconds(ms * 1000);
}

void sleepMicroseconds(std::uint64_t us)
{
#ifdef _WIN32
    ::Sleep(DWORD((us + 999) / 1000));
#else
    struct timespec time_out, remains;
    time_out.tv_sec = (int)(us / 1000000);
    time_out.tv_nsec = (int)(us % 1000000) * 1000;

    while (EINTR == clock_nanosleep(CLOCK_MONOTONIC, 0, &time_out, &remains)) {
        time_out = remains;
    }
#endif // _WIN32
}

timestamp_t getCurrentMilliseconds()
{
#ifdef _WIN32
    return ((timestamp_t)::GetTickCount());
#else
    struct timespec time;
    // monotonic time since some unspecified starting point
    if (clock_gettime(CLOCK_MONOTONIC, &time) == 0) {
        return ((timestamp_t)time.tv_sec) * 1000 +    // sec to msec
               ((timestamp_t)time.tv_nsec) / 1000000; // nsec to msec
    }
    else {
        return 0;
    }

#endif // _WIN32
}

timestamp_t getCurrentMicroseconds()
{
    static bool initialized = false;

#ifdef _WIN32
    static LARGE_INTEGER frequency;
    static LARGE_INTEGER ref_time;

    if (!initialized) {
        if (!::QueryPerformanceFrequency(&frequency)) {
            return (timestamp_t)-1;
        }

        frequency.QuadPart /= 1000; // avoid overrun

        if (!::QueryPerformanceCounter(&ref_time)) {
            return (timestamp_t)-1;
        }

        initialized = true;
    }

    LARGE_INTEGER count;
    if (!::QueryPerformanceCounter(&count)) {
        return (timestamp_t)-1;
    }

    return (timestamp_t)(((count.QuadPart - ref_time.QuadPart) * 1000) / frequency.QuadPart);

#else // _WIN32

    static timestamp_t ref_time;
    const timestamp_t us_per_second = 1000000l;

    struct timespec time_spec;

#ifdef __APPLE__
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    time_spec.tv_sec = tv.tv_sec;
    time_spec.tv_nsec = tv.tv_usec * 1000;
#else
    if (0 != clock_gettime(CLOCK_MONOTONIC, &time_spec)) {
        return (timestamp_t)-1;
    }
#endif

    timestamp_t now = time_spec.tv_nsec / 1000l + time_spec.tv_sec * us_per_second;
    if (!initialized) {
        ref_time = now;
        initialized = true;
    }

    return now - ref_time;

#endif // _WIN32
}

HighResSchedulingSupport::HighResSchedulingSupport()
{
#ifdef _WIN32
    _handle = ::timeBeginPeriod(1);
#endif
}

HighResSchedulingSupport::~HighResSchedulingSupport()
{
#ifdef _WIN32
    if (_handle == 0) {
        ::timeEndPeriod(1);
    }
#endif
}

bool HighResSchedulingSupport::isSupported() const
{
#ifdef _WIN32
    return _handle == 0;
#else
    return true;
#endif
}

} // namespace system
} // namespace a_util
