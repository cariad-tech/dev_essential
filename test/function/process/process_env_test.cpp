/**
 * @file
 * Thread test implementation
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

#include <a_util/process.h>
#include <a_util/strings/strings_convert.h>

#include <gtest/gtest.h>

using namespace a_util;

TEST(process_env_test, test_env_set)
{
    ASSERT_NO_THROW(process::setEnvVar("A_UTIL_TEST_VAR", "42"));
    std::string res = process::getEnvVar("A_UTIL_TEST_VAR");
    ASSERT_EQ(42, a_util::strings::toInt32(res));

    // try it with long long values
    std::string env_long = "01234567890123456789012345678901234567890123456789012345678901234567890"
                           "12345678901234567890123456789";
    while (env_long.length() < 8192) {
        env_long = env_long + env_long;
    }
    ASSERT_NO_THROW(process::setEnvVar("A_UTIL_TEST_VAR", env_long));
    res = process::getEnvVar("A_UTIL_TEST_VAR");
    ASSERT_EQ(res, env_long);

    bool function_call_did_throw = false;
    std::string res_backup = res;
    try {
        // test empty value for unknown vars
        res = process::getEnvVar("A_UTIL_TEST_VAR_UNKNOWN");
    }
    catch (const std::exception&) {
        function_call_did_throw = true;
    }
    ASSERT_TRUE(function_call_did_throw);
    // test if res is untouched
    ASSERT_EQ(res_backup, res);

    // test empty value for unknown vars
    function_call_did_throw = false;
    res_backup = res;
    try {
        res = process::getEnvVar("");
    }
    catch (const std::exception&) {
        function_call_did_throw = true;
    }
    ASSERT_TRUE(function_call_did_throw);
    // test if res is untouched
    ASSERT_EQ(res_backup, res);
}

TEST(process_env_test, test_env_set_with_default)
{
    ASSERT_NO_THROW(process::setEnvVar("A_UTIL_TEST_VAR", "42"));
    std::string res = process::getEnvVar("A_UTIL_TEST_VAR", "0");
    ASSERT_EQ(42, a_util::strings::toInt32(res));

    // try it with long long values
    std::string env_long = "01234567890123456789012345678901234567890123456789012345678901234567890"
                           "12345678901234567890123456789";
    while (env_long.length() < 8192) {
        env_long = env_long + env_long;
    }
    ASSERT_NO_THROW(process::setEnvVar("A_UTIL_TEST_VAR", env_long));
    res = process::getEnvVar("A_UTIL_TEST_VAR", "empty");
    ASSERT_EQ(res, env_long);

    // test empty value for unknown vars
    res = process::getEnvVar("A_UTIL_TEST_VAR_UNKNOWN", "this_is_default");
    ASSERT_EQ(std::string("this_is_default"), res);

    // test empty value for unknown vars
    res = process::getEnvVar("", "this_is_default");
    ASSERT_EQ(std::string("this_is_default"), res);
}
