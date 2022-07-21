/**
 * Common error code and condition implementation test for C
 *
 * Copyright @ 2022 VW Group. All rights reserved.
 *
 *     This Source Code Form is subject to the terms of the Mozilla
 *     Public License, v. 2.0. If a copy of the MPL was not distributed
 *     with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * If it is not possible or desirable to put the notice in a particular file, then
 * You may include the notice in a location (such as a LICENSE file in a
 * relevant directory) where a recipient would be likely to look for such a notice.
 *
 * You may add additional accurate notices of copyright ownership.
 */

#ifdef __cplusplus
#error This file is supposed to be compiled with a C compiler!
#endif // __cplusplus
#include <a_util/base/error.h>

#ifdef __GNUC__
#if __GNUC__ < 5 || (__GNUC__ == 5 && (__GNUC_MINOR__ < 5))
#define static_assert(condition, msg) _Static_assert(condition, msg)
#else // __GNUC__ < 5 || (__GNUC__ == 5 && (__GNUC_MINOR__ < 5))
#include <assert.h>
#endif // __GNUC__ < 5 || (__GNUC__ == 5 && (__GNUC_MINOR__ < 5))
#else  // __GNUC__
#include <assert.h>
#endif // __GNUC__

#define STATIC_ASSERT_EQ(expected_val, enum_val)                                                   \
    static_assert(expected_val == enum_val,                                                        \
                  "'errc::" #enum_val "' does not have the expected value of '" #expected_val "'")

#include "errc_test.inc"
