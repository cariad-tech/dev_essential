/**
 * @file
 * Compat header to inject ssize_t to libjson-rpc-cpp
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

#ifndef PKG_RPC_THIRD_PARTY_LIBJSON_RPC_CPP_COMPAT_MSVC_HEADER_INCLUDED_
#define PKG_RPC_THIRD_PARTY_LIBJSON_RPC_CPP_COMPAT_MSVC_HEADER_INCLUDED_

#include <io.h>
typedef unsigned int ssize_t;

#endif // PKG_RPC_THIRD_PARTY_LIBJSON_RPC_CPP_COMPAT_MSVC_HEADER_INCLUDED_