/**
 * @file
 * Public API for compatibility to C++11
 * <a href="https://en.cppreference.com/w/cpp/header/condition_variable">\<condition_variable\></a>
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

#ifndef A_UTIL_UTIL_CONCURRENCY_CONDITION_VARIABLE_HEADER_INCLUDED
#define A_UTIL_UTIL_CONCURRENCY_CONDITION_VARIABLE_HEADER_INCLUDED

#include <condition_variable>

namespace a_util {
namespace concurrency {
/**
 * Compatibility to C++11
 * <a href="https://en.cppreference.com/w/cpp/thread/condition_variable">std::condition_variable
 * </a>
 */
using std::condition_variable;

/**
 * Compatibility to C++11
 * <a href="https://en.cppreference.com/w/cpp/thread/cv_status">std::cv_status</a>
 */
using std::cv_status;

} // namespace concurrency
} // namespace a_util

#endif // A_UTIL_UTIL_CONCURRENCY_CONDITION_VARIABLE_HEADER_INCLUDED
