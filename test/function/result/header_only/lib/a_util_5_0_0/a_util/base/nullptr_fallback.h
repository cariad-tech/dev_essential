/**
 * @file
 * nullptr fallback as described in "a name for the null pointer" (C22/WG21/ N2431 = J16/07 - 0301)
 *
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */

#ifndef A_UTIL_UTIL_BASE_NULLPTR_FALLBACK_HEADER_INCLUDED
#define A_UTIL_UTIL_BASE_NULLPTR_FALLBACK_HEADER_INCLUDED

#include <a_util/base/config.h>

#if A_UTIL_ENABLE_COMPILER_CXX_NULLPTR

// use c++-11 keyword 'nullptr'

#else

// disable nullptr compiler warning
// due to a bug in gcc4.6, we mark this file as system_header, which means, "I know what I'm doing!"
// diagnostic ignored "--Wc++0x-compat" doesn't work
#pragma GCC system_header

const class nullptr_fallback {
public:
    template <class T>
    operator T*() const
    {
        // of null non-member
        return 0;
    }

    template <class C, class T>
    operator T C::*() const
    {
        return 0;
    }

private:
    void operator&() const;
} nullptr = {};

namespace std {
typedef ::nullptr_fallback nullptr_t;
}

#endif

#endif // A_UTIL_UTIL_BASE_NULLPTR_FALLBACK_HEADER_INCLUDED
