/**
 * @file
 * Public types and functions defining a_util @ref a_util_core
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

#ifndef A_UTIL_UTIL_BASE_TYPES_H
#define A_UTIL_UTIL_BASE_TYPES_H

#include <a_util/base/enums.h>

#include <atomic>

/**
 * @addtogroup a_util_core
 * @{
 */
/// Type of a handle value.
typedef void* handle_t; /* h */
/// Type of a timestamp value. If not otherwise stated, always in microseconds.
typedef std::int64_t timestamp_t; /* n */
/// Type of an atomic timestamp_t.
#ifdef __QNX__
typedef std::atomic_int_fast64_t atomic_timestamp_t;
#else
typedef std::atomic<timestamp_t> atomic_timestamp_t;
#endif

namespace a_util {
/**
 * @addtogroup a_util_core
 * @{
 */
/**
 * Mimics C++17 attribute @c maybe_unused to silence compiler warnings on potentially unused
 * enitities.
 * @tparam T Type of the potentially unused entity
 * @see https://en.cppreference.com/w/cpp/language/attributes/maybe_unused
 */
template <typename T>
void maybe_unused(T&&)
{
}

} // namespace a_util
/** @} */
#endif /* A_UTIL_UTIL_BASE_TYPES_H */
