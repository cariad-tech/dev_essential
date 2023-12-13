/**
 * @file
 * Common include for component a_util::concurrency
 *
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */

#ifndef A_UTIL_CONCURRENCY_HEADER_INCLUDED
#define A_UTIL_CONCURRENCY_HEADER_INCLUDED

namespace a_util {
/**
 * Serves as component for concurrency handling, mainly but not exclusively for code
 * compatibility to
 * <a href="https://en.cppreference.com/w/cpp/thread">standard thread support</a>.
 */
namespace concurrency {
/**
 * Serves as namespace for code compatibility to
 * <a href="https://en.cppreference.com/w/cpp/thread#Functions_managing_the_current_thread">
 * std::thread::this_thread</a>.
 */
namespace this_thread {
}
/// Serves as namespace for concurrency implementation details.
namespace detail {
}
} // namespace concurrency
/**
 * Serves as component for code compatibility to
 *        <a href="https://en.cppreference.com/w/cpp/chrono">std::chrono</a>.
 */
namespace chrono {
}
} // namespace a_util

#include <a_util/concurrency/atomic.h>
#include <a_util/concurrency/condition_variable.h>
#include <a_util/concurrency/fast_mutex.h>
#include <a_util/concurrency/mutex.h>
#include <a_util/concurrency/semaphore.h>
#include <a_util/concurrency/shared_mutex.h>
#include <a_util/concurrency/thread.h>

#endif // A_UTIL_CONCURRENCY_HEADER_INCLUDED
