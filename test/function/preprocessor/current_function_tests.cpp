/**
 * @file
 * A_UTIL_CURRENT_FUNCTION macro test
 *
 * Copyright @ 2021 VW Group. All rights reserved.
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

#include <a_util/preprocessor/current_function.h>

#include <gtest/gtest.h>

/**
 * Check whether A_UTIL_CURRENT_FUNCTION redirects to BOOST_CURRENT_FUNCTION.
 * @req_id TODO
 */
TEST(CurrentFunctionMacro, redirectsToBoostFunctionMacro)
{
    EXPECT_STREQ(A_UTIL_CURRENT_FUNCTION, BOOST_CURRENT_FUNCTION);
    EXPECT_NE(A_UTIL_CURRENT_FUNCTION, nullptr);
    EXPECT_NE(A_UTIL_CURRENT_FUNCTION, "");
}
