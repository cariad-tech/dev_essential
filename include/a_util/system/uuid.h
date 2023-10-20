/**
 * @file
 * Public API for @ref a_util::system::generateUUIDv4 "generateUUIDv4" function
 *
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */

#ifndef A_UTIL_UTIL_SYSTEM_UUID_INCLUDED
#define A_UTIL_UTIL_SYSTEM_UUID_INCLUDED

#include <string>

namespace a_util {
namespace system {
/**
 * Generate a UUIDv4 string
 * @return A fully randomized UUIDv4 string
 */
std::string generateUUIDv4();

} // namespace system
} // namespace a_util

#endif // A_UTIL_UTIL_SYSTEM_UUID_INCLUDED
