/**
 * @file
 * Test implementation for result backwards and forward compatibility.
 *
 * Copyright @ 2021 VW Group. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <a_util/preprocessor/dll_export.h> // A_UTIL_DLL_EXPORT in a_util_result_legacy.h

// include guard for result_type_impl.h so we skip it and only use the declaration
#define A_UTIL_UTIL_RESULT_DETAIL_RESULT_TYPE_IMPL_HEADER_INCLUDED

#include "lib/a_util_result_legacy.h"

#undef A_UTIL_UTIL_RESULT_DETAIL_RESULT_TYPE_IMPL_HEADER_INCLUDED

#include <gtest/gtest.h>

#include <memory>

namespace {

/**
 * Test whether the legacy implementation of Result is compatible with the current implementation
 */
TEST(ResultABI, isBackwardCompatible)
{
    using CurrentResult = a_util::result::Result;
    {
        const CurrentResult result = LegacyResult::createWithDefaultCtor();
        EXPECT_EQ(result.getErrorCode(), 0);
        EXPECT_EQ(result.getLine(), -1);
        EXPECT_STREQ(result.getErrorLabel(), "SUCCESS");
        EXPECT_STREQ(result.getDescription(), "No error occurred");
        EXPECT_STREQ(result.getFile(), "");
        EXPECT_STREQ(result.getFunction(), "");
    }

    {
        const CurrentResult result = LegacyResult::createWithErrorCode(0);
        EXPECT_EQ(result.getErrorCode(), 0);
        EXPECT_EQ(result.getLine(), -1);
        EXPECT_STREQ(result.getErrorLabel(), "SUCCESS");
        EXPECT_STREQ(result.getDescription(), "No error occurred");
        EXPECT_STREQ(result.getFile(), "");
        EXPECT_STREQ(result.getFunction(), "");
    }

    {
        const CurrentResult result = LegacyResult::createWithErrorCode(42);
        EXPECT_EQ(result.getErrorCode(), 42);
        EXPECT_EQ(result.getLine(), -1);
        EXPECT_STREQ(result.getErrorLabel(), "(unknown)");
        EXPECT_STREQ(result.getDescription(), "");
        EXPECT_STREQ(result.getFile(), "");
        EXPECT_STREQ(result.getFunction(), "");
    }

    {
        const CurrentResult result = LegacyResult::createWithSUCCESS();
        EXPECT_EQ(result.getErrorCode(), 0);
        EXPECT_EQ(result.getLine(), -1);
        EXPECT_STREQ(result.getErrorLabel(), "SUCCESS");
        EXPECT_STREQ(result.getDescription(), "No error occurred");
        EXPECT_STREQ(result.getFile(), "");
        EXPECT_STREQ(result.getFunction(), "");
    }

    {
        const CurrentResult result = LegacyResult::createWithBool(true);
        EXPECT_EQ(result.getErrorCode(), 0);
        EXPECT_EQ(result.getLine(), -1);
        EXPECT_STREQ(result.getErrorLabel(), "SUCCESS");
        EXPECT_STREQ(result.getDescription(), "No error occurred");
        EXPECT_STREQ(result.getFile(), "");
        EXPECT_STREQ(result.getFunction(), "");
    }

    {
        const CurrentResult result = LegacyResult::createWithBool(false);
        EXPECT_EQ(result.getErrorCode(), -1);
        EXPECT_EQ(result.getLine(), -1);
        EXPECT_STREQ(result.getErrorLabel(), "(unknown)");
        EXPECT_STREQ(result.getDescription(), "");
        EXPECT_STREQ(result.getFile(), "");
        EXPECT_STREQ(result.getFunction(), "");
    }

    {
        const CurrentResult result = LegacyResult::createWithERR_FAILED();
        EXPECT_EQ(result.getErrorCode(), ERR_FAILED.getCode());
        EXPECT_EQ(result.getLine(), -1);
        EXPECT_STREQ(result.getErrorLabel(), "(unknown)");
        EXPECT_STREQ(result.getDescription(), "");
        EXPECT_STREQ(result.getFile(), "");
        EXPECT_STREQ(result.getFunction(), "");
    }

    {
        const CurrentResult result =
            LegacyResult::createWithDetailedERR_FAILED("Error failed",
                                                       __LINE__,
                                                       "result_type_compat_test.cpp",
                                                       "createWithDetailedERR_FAILED");
        EXPECT_EQ(result.getErrorCode(), ERR_FAILED.getCode());
        EXPECT_EQ(result.getLine(), 106);
        EXPECT_STREQ(result.getErrorLabel(), ERR_FAILED.getLabel());
        EXPECT_STREQ(result.getDescription(), "Error failed");
        EXPECT_STREQ(result.getFile(), "result_type_compat_test.cpp");
        EXPECT_STREQ(result.getFunction(), "createWithDetailedERR_FAILED");
    }
}

} // namespace

// now use the current implementation to create current result objects
#include "a_util/result/detail/result_type_impl.h"

namespace {

/**
 * Test whether the current implementation of Result is compatible with the legacy implementation
 */
TEST(ResultABI, isForwardCompatible)
{
    using CurrentResult = a_util::result::Result;
    {
        // create current result object and pass ownership to legacy result implementation
        // this way the result is destroyed in library code
        // error description is heap allocated and reference counted
        CurrentResult result = a_util::result::Result(ERR_FAILED,
                                                      "Error failed",
                                                      __LINE__,
                                                      "result_type_compat_test.cpp",
                                                      "createWithDetailedERR_FAILED");
        LegacyResult::testAndDestroyCurrentResult(std::move(result),
                                                  ERR_FAILED.getCode(),
                                                  ERR_FAILED.getLabel(),
                                                  "Error failed",
                                                  137,
                                                  "result_type_compat_test.cpp",
                                                  "createWithDetailedERR_FAILED");
        // result was moved and destroyed in library code
        EXPECT_EQ(result, CurrentResult());
    }

    {
        // create current result object and copy to legacy result implementation
        // error description is heap allocated and reference counted
        const CurrentResult result = a_util::result::Result(ERR_FAILED,
                                                            "Error failed",
                                                            __LINE__,
                                                            "result_type_compat_test.cpp",
                                                            "createWithDetailedERR_FAILED");
        LegacyResult::testAndDestroyCurrentResult(result,
                                                  ERR_FAILED.getCode(),
                                                  ERR_FAILED.getLabel(),
                                                  "Error failed",
                                                  156,
                                                  "result_type_compat_test.cpp",
                                                  "createWithDetailedERR_FAILED");
        // result was copied, so still exists
        EXPECT_NE(result, CurrentResult());
        EXPECT_EQ(ERR_FAILED.getCode(), result.getErrorCode());
        EXPECT_EQ(156, result.getLine());
        EXPECT_STREQ(ERR_FAILED.getLabel(), result.getErrorLabel());
        EXPECT_STREQ("Error failed", result.getDescription());
        EXPECT_STREQ("result_type_compat_test.cpp", result.getFile());
        EXPECT_STREQ("createWithDetailedERR_FAILED", result.getFunction());
    }

    {
        // create current result object and pass ownership to legacy result implementation
        // this way the result is destroyed in library code
        // error description is statically allocated and not reference counted
        CurrentResult result = a_util::result::Result(ERR_FAILED);
        LegacyResult::testAndDestroyCurrentResult(
            std::move(result), ERR_FAILED.getCode(), ERR_FAILED.getLabel());
        // result was moved and destroyed in library code
        EXPECT_EQ(result, CurrentResult());
    }

    {
        // create current result object and copy to legacy result implementation
        // error description is statically allocated and not reference counted
        const CurrentResult result = a_util::result::Result(ERR_FAILED);
        LegacyResult::testAndDestroyCurrentResult(
            result, ERR_FAILED.getCode(), ERR_FAILED.getLabel());
        // result was copied, so still exists
        EXPECT_NE(result, CurrentResult());
        EXPECT_EQ(ERR_FAILED.getCode(), result.getErrorCode());
        EXPECT_EQ(-1, result.getLine());
        EXPECT_STREQ(ERR_FAILED.getLabel(), result.getErrorLabel());
        EXPECT_STREQ("", result.getDescription());
        EXPECT_STREQ("", result.getFile());
        EXPECT_STREQ("", result.getFunction());
    }

    {
        // create current result object and pass ownership to legacy result implementation
        // this way the result is destroyed in library code
        // error description is not set
        CurrentResult result = a_util::result::Result(ERR_FAILED.getCode());
        LegacyResult::testAndDestroyCurrentResult(std::move(result), ERR_FAILED.getCode());
        // result was moved and destroyed in library code
        EXPECT_EQ(result, CurrentResult());
    }

    {
        // create current result object and copy to legacy result implementation
        // error description not set
        const CurrentResult result = a_util::result::Result(ERR_FAILED.getCode());
        LegacyResult::testAndDestroyCurrentResult(result, ERR_FAILED.getCode());
        // result was copied, so still exists
        EXPECT_NE(result, CurrentResult());
        EXPECT_EQ(ERR_FAILED.getCode(), result.getErrorCode());
        EXPECT_EQ(-1, result.getLine());
        EXPECT_STREQ("(unknown)", result.getErrorLabel());
        EXPECT_STREQ("", result.getDescription());
        EXPECT_STREQ("", result.getFile());
        EXPECT_STREQ("", result.getFunction());
    }
}

} // namespace
