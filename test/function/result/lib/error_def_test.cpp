/**
 * @file
 * Test implementation for error definitions.
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

#include "a_util/result/error_def.h"

#include <gtest/gtest.h>

_MAKE_RESULT(666, TEST_ERROR);

a_util::result::Result makeErrorAndGetFunctionName(std::string& function_name)
{
    function_name = A_UTIL_CURRENT_FUNCTION;
    RETURN_ERROR_DESCRIPTION(TEST_ERROR, "Hello World!");
}

TEST(error_def_test, TestReturnErrorDescription)
{
    const std::string file_name(__FILE__);
    std::string function_name;
    const a_util::result::Result result = makeErrorAndGetFunctionName(function_name);

    EXPECT_EQ(result.getErrorCode(), TEST_ERROR.getCode());
    EXPECT_EQ(result.getErrorCode(), 666);
    EXPECT_EQ(result.getLine(),
              27); // this is the line of the above  ... RETURN_ERROR_DESCRIPTION(TEST_ERROR, "Hello
                   // World!"); statement!
    EXPECT_STREQ(result.getErrorLabel(), TEST_ERROR.getLabel());
    EXPECT_STREQ(result.getErrorLabel(), "TEST_ERROR");
    EXPECT_STREQ(result.getDescription(), "Hello World!");
    EXPECT_STREQ(result.getFunction(), function_name.c_str());
    EXPECT_STREQ(result.getFile(), file_name.c_str());
}
