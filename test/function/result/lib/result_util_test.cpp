/**
 * @file
 * Test implementation for a_util::result utility functions.
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

#include "a_util/result/result_type.h" // for Result
#include "a_util/result/result_util.h" // for toString

#include <gtest/gtest.h> // IWYU pragma: keep

/// Create error type ERR_FAILED with numeric representation -38
_MAKE_RESULT(-38, ERR_FAILED);

TEST(result_util_tester, TestToString)
{
    using a_util::result::Result;
    using a_util::result::toString;

    Result oResult(ERR_FAILED, "<description>", 1234, "<file>", "<function>");
    const std::string strResult = toString(oResult);

    EXPECT_TRUE(strResult.find("ERR_FAILED") != std::string::npos);
    EXPECT_TRUE(strResult.find("<description>") != std::string::npos);
    EXPECT_TRUE(strResult.find("<file>") != std::string::npos);
    EXPECT_TRUE(strResult.find("1234") != std::string::npos);
    EXPECT_TRUE(strResult.find("<function>") != std::string::npos);
    EXPECT_EQ(strResult.size(), 93);
}
