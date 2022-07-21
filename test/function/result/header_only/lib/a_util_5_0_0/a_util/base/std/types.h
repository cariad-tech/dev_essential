/**
 * @file
 * Standard type declarations.
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

#ifndef A_UTIL_UTIL_BASE_STD_TYPES_H
#define A_UTIL_UTIL_BASE_STD_TYPES_H

#include <a_util/base/std/config.h>

#if defined(HAVE_CSTDINT) /* Include <cstdint> if available */
#include <cstdint>
#elif defined(HAVE_STDINT_H) /* Do we at least have an stdint.h ? */
#include <a_util/base/std/stdint.h>

#include <stdint.h>
#else /* As a last resort use the provided one */
#include "3rdparty/pstdint/pstdint-0.1.16.0/pstdint.h"

#include <a_util/base/std/stdint.h>
#endif

/// type definition for a handle value
typedef void* handle_t; /* h */
/// type definition for a time value. This is always in microseconds
/// (if not the declaration of a use needs to mark this explicit).
typedef std::int64_t timestamp_t; /* n */

#endif /* A_UTIL_UTIL_BASE_STD_TYPES_H */
