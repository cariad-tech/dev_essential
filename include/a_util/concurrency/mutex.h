/**
 * @file
 * Public API for compatibility to C++11
 * <a href="https://en.cppreference.com/w/cpp/header/mutex">\<mutex\></a>
 *
 *
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */

#ifndef A_UTIL_UTIL_CONCURRENCY_MUTEX_HEADER_INCLUDED
#define A_UTIL_UTIL_CONCURRENCY_MUTEX_HEADER_INCLUDED

#include <mutex>

namespace a_util {
namespace concurrency {
/**
 * Compatibility to C++11
 * <a href="https://en.cppreference.com/w/cpp/thread/mutex">std::mutex</a>
 */
using std::mutex;
/**
 * Compatibility to C++11
 * <a href="https://en.cppreference.com/w/cpp/thread/recursive_mutex">std::recursive_mutex</a>
 */
using std::recursive_mutex;
/**
 * Compatibility to C++11
 * <a href="https://en.cppreference.com/w/cpp/thread/unique_lock">std::unique_lock</a>
 */
using std::unique_lock;

} // namespace concurrency
} // namespace a_util

#endif // A_UTIL_UTIL_CONCURRENCY_MUTEX_HEADER_INCLUDED
