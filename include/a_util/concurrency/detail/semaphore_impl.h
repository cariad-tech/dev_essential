/**
 * @file
 * Semapore implementation file
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

#ifndef A_UTIL_UTIL_CONCURRENCY_DETAIL_SEMAPHORE_IMPL_HEADER_INCLUDED
#define A_UTIL_UTIL_CONCURRENCY_DETAIL_SEMAPHORE_IMPL_HEADER_INCLUDED

#include <a_util/concurrency/detail/semaphore_decl.h>
#include <a_util/system/system.h>

#ifndef _MSC_VER
#include <condition_variable>
#include <thread>
#else
#include <mutex>
#endif // !_MSC_VER

#include <stdexcept>

namespace a_util {
namespace concurrency {
namespace detail {
template <typename Mutex, typename CondVar>
inline basic_semaphore<Mutex, CondVar>::basic_semaphore(int count)
    : _mutex(), _cv(), _lock_count(count)
{
}

template <typename Mutex, typename CondVar>
inline void basic_semaphore<Mutex, CondVar>::notify()
{
    std::unique_lock<Mutex> lock(_mutex);
    _lock_count++;
    _cv.notify_all();
}

template <typename Mutex, typename CondVar>
inline void basic_semaphore<Mutex, CondVar>::wait()
{
    std::unique_lock<Mutex> lock(_mutex);

    while (_lock_count == 0) {
        _cv.wait(lock);
    }
    _lock_count--;
}

template <typename Mutex, typename CondVar>
inline bool basic_semaphore<Mutex, CondVar>::try_wait()
{
    std::unique_lock<Mutex> lock(_mutex);
    bool is_success = false;
    if (_lock_count > 0) {
        _lock_count--;
        is_success = true;
    }
    return is_success;
}

template <typename Mutex, typename CondVar>
inline void basic_semaphore<Mutex, CondVar>::reset()
{
    std::unique_lock<Mutex> lock(_mutex);
    _lock_count = 0;
}

template <typename Mutex, typename CondVar>
inline bool basic_semaphore<Mutex, CondVar>::is_set()
{
    std::unique_lock<Mutex> lock(_mutex);
    return _lock_count > 0;
}

template <typename Mutex, typename CondVar>
template <typename Rep, typename Period>
inline bool basic_semaphore<Mutex, CondVar>::wait_for(
    const std::chrono::duration<Rep, Period>& timeout)
{
    if (timeout.count() < 0) {
        throw std::invalid_argument("timeout.count() < 0");
    }

    // can compute this before locking
    const timestamp_t until =
        a_util::system::getCurrentMicroseconds() +
        std::chrono::duration_cast<std::chrono::microseconds>(timeout).count();

    std::unique_lock<Mutex> lock(_mutex);

    bool is_timeout = false;
    while (_lock_count == 0 && !is_timeout) {
        timestamp_t wait_time = until - a_util::system::getCurrentMicroseconds();
        is_timeout =
            (wait_time <= 0) ||
            (_cv.wait_for(lock, std::chrono::microseconds(wait_time)) == std::cv_status::timeout);
    }

    // can lock ?
    if (_lock_count > 0) {
        _lock_count--;
        return true;
    }

    return false;
}

} // namespace detail
} // namespace concurrency
} // namespace a_util

#endif // A_UTIL_UTIL_CONCURRENCY_DETAIL_SEMAPHORE_IMPL_HEADER_INCLUDED
