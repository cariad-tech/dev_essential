/**
 * @file
 * Config header file setting necessary predefined macros to use the correct types
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

#ifndef A_UTIL_UTIL_BASE_STD_CONFIG_HEADER_INCLUDED
#define A_UTIL_UTIL_BASE_STD_CONFIG_HEADER_INCLUDED

#ifndef HAVE_ATOMIC
#define HAVE_ATOMIC
#endif

#ifndef HAVE_CSTDINT
#define HAVE_CSTDINT
#endif

#ifndef HAVE_STDINT_H
/* #undef HAVE_STDINT_H */
#endif

#ifndef HAVE_TYPETRAITS
#define HAVE_TYPETRAITS
#endif

#endif // A_UTIL_UTIL_BASE_STD_CONFIG_HEADER_INCLUDED
