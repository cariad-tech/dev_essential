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
@endverbatim
 */

#ifndef PKG_RPC_THIRD_PARTY_LIBJSON_RPC_CPP_COMPAT_MSVC_HEADER_INCLUDED_
#define PKG_RPC_THIRD_PARTY_LIBJSON_RPC_CPP_COMPAT_MSVC_HEADER_INCLUDED_

#include <io.h>
typedef unsigned int ssize_t;

#endif // PKG_RPC_THIRD_PARTY_LIBJSON_RPC_CPP_COMPAT_MSVC_HEADER_INCLUDED_