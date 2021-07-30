/**
 * @file
 * A_UTIL_TO_STRING macro test
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

#include "a_util/preprocessor/to_string.h"

#include <gtest/gtest.h>

/**
 * Test whether A_UTIL_TO_STRING evaluates to a string representation of a preprocessor expression.
 * @req_id TODO
 */
TEST(ToStringMacro, evaluatesToStringRepresentation)
{
    {
        const char line[] = A_UTIL_TO_STRING(__LINE__);
        EXPECT_STREQ(line, "29");
    }
    {
        const char line[] = A_UTIL_TO_STRING(__LINE__);
        EXPECT_STREQ(line, "33");
    }

#define SomeRandomString Some random string
    const char some_random_string[] = A_UTIL_TO_STRING(SomeRandomString);
    EXPECT_STREQ(some_random_string, "Some random string");
}
