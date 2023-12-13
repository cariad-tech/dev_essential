/**
 * @file
 * Several enums for project wide use
 *
 * @copyright
 * @verbatim
Copyright @ 2022 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */

#ifndef A_UTIL_BASE_ENUMS_HEADER_INCLUDED
#define A_UTIL_BASE_ENUMS_HEADER_INCLUDED

#include <cstdint>

namespace a_util {

/// Flags for functions that need to sort elements
enum class SortingOrder : std::int32_t {
    ascending = 0x00, //!< Sort in ascending order
    descending = 0x01 //!< Sort in descending order
};

} // namespace a_util

#endif // A_UTIL_BASE_ENUMS_HEADER_INCLUDED
