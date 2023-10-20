/**
 * @file
 * Common include for component a_util::result
 *
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */

#ifndef A_UTIL_RESULT_HEADER_INCLUDED
#define A_UTIL_RESULT_HEADER_INCLUDED

namespace a_util {
/// Serves as component for functionality handling error and return types
namespace result {
/// Serves as namespace for result implementation details.
namespace detail {
}
} // namespace result
} // namespace a_util

#include <a_util/result/error_def.h>
#include <a_util/result/result_info.h>
#include <a_util/result/result_type.h>
#include <a_util/result/result_util.h>

#endif // A_UTIL_RESULT_HEADER_INCLUDED
