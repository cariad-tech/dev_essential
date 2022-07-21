/**
 * @file
 * Common include for root component a_util and the @ref a_util_core
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

#ifndef A_UTIL_BASE_HEADER_INCLUDED
#define A_UTIL_BASE_HEADER_INCLUDED

/**
 * @defgroup a_util_core core functionality
 * Fundamental types, classes, functions and macros - applicable using@n@n
 * <tt>\#include <a_util/base.h></tt>
 */
/// Serves as the root component, with common functionality documented in @ref a_util_core.
namespace a_util {
/// Serves as component for features which are considered experimental
namespace experimental {
}
} // namespace a_util

#include <a_util/base/delegate.h>
#include <a_util/base/types.h>

#endif // A_UTIL_BASE_HEADER_INCLUDED
