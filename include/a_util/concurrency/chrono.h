/**
 * @file
 * Public API for compatibility to C++11
 * <a href="https://en.cppreference.com/w/cpp/header/chrono">\<chrono\></a>
 *
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */

#ifndef A_UTIL_UTIL_CONCURRENCY_CHRONO_HEADER_INCLUDED
#define A_UTIL_UTIL_CONCURRENCY_CHRONO_HEADER_INCLUDED

#include <chrono>

namespace a_util {
namespace chrono {
/**
 * Compatibility to C++11
 * <a href="https://en.cppreference.com/w/cpp/chrono/duration">std::chrono::duration</a>
 */
using std::chrono::duration;

/**
 * Compatibility to C++11
 * <a href="https://en.cppreference.com/w/cpp/chrono/duration/duration_cast">
 * std::chrono::duration_cast</a>
 */
using std::chrono::duration_cast;

/**
 * Compatibility to C++11
 * <a href="https://en.cppreference.com/w/cpp/chrono/duration#Helper_types">
 * std::chrono::nanoseconds</a>
 */
typedef std::chrono::nanoseconds nanoseconds;

/**
 * Compatibility to C++11
 * <a href="https://en.cppreference.com/w/cpp/chrono/duration#Helper_types">
 * std::chrono::microseconds</a>
 */
typedef std::chrono::microseconds microseconds;

/**
 * Compatibility to C++11
 * <a href="https://en.cppreference.com/w/cpp/chrono/duration#Helper_types">
 * std::chrono::milliseconds</a>
 */
typedef std::chrono::milliseconds milliseconds;

/**
 * Compatibility to C++11
 * <a href="https://en.cppreference.com/w/cpp/chrono/duration#Helper_types">
 * std::chrono::seconds</a>
 */
typedef std::chrono::seconds seconds;

/**
 * Compatibility to C++11
 * <a href="https://en.cppreference.com/w/cpp/chrono/duration#Helper_types">
 * std::chrono::minutes</a>
 */
typedef std::chrono::minutes minutes;

/**
 * Compatibility to C++11
 * <a href="https://en.cppreference.com/w/cpp/chrono/duration#Helper_types">
 * std::chrono::hours</a>
 */
typedef std::chrono::hours hours;

} // namespace chrono
} // namespace a_util

#endif // A_UTIL_UTIL_CONCURRENCY_CHRONO_HEADER_INCLUDED
