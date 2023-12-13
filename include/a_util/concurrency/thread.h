/**
 * @file
 * Public API for compatibility to C++11
 * <a href="https://en.cppreference.com/w/cpp/header/thread">\<thread\></a>
 *
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */
#ifndef A_UTIL_UTIL_CONCURRENCY_THREAD_HEADER_INCLUDED
#define A_UTIL_UTIL_CONCURRENCY_THREAD_HEADER_INCLUDED

#include <thread>

namespace a_util {
namespace concurrency {
/// Compatibility to C++11 <a href="https://en.cppreference.com/w/cpp/thread/mutex">std::mutex</a>
using std::thread;

namespace this_thread {
/**
 * Compatibility to C++11
 * <a href="https://en.cppreference.com/w/cpp/thread/get_id">std::this_thread::get_id</a>
 */
using std::this_thread::get_id;
/**
 * Compatibility to C++11
 * <a href="https://en.cppreference.com/w/cpp/thread/yield">std::this_thread::yield</a>
 */
using std::this_thread::yield;

} // namespace this_thread
} // namespace concurrency
} // namespace a_util

#endif // A_UTIL_UTIL_CONCURRENCY_THREAD_HEADER_INCLUDED
