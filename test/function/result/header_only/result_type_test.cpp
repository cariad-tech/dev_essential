/**
 * @file
 * Test implementation.
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

#include <a_util/preprocessor/current_function.h> // for A_UTIL_CURRENT_F...
#include <a_util/result/result_info.h>            // for ResultInfo, is_...
#include <a_util/result/result_type.h>            // for Result, isOk

#include <gtest/gtest-message.h>   // for Message
#include <gtest/gtest-test-part.h> // for TestPartResult
#include <gtest/gtest.h>           // IWYU pragma: keep

#include <cstdlib> // for rand, srand
#include <ctime>   // for time
#include <string>  // for string

/// Create error type ERR_INVALID_ARG with numeric representation -5
_MAKE_RESULT(-5, ERR_INVALID_ARG);
/// Create error type ERR_INVALID_ADDRESS with numeric representation -7
_MAKE_RESULT(-7, ERR_INVALID_ADDRESS);
/// Create error type ERR_MEMORY with numeric representation -12
_MAKE_RESULT(-12, ERR_MEMORY);
/// Create error type ERR_TIMEOUT with numeric representation -13
_MAKE_RESULT(-13, ERR_TIMEOUT);
/// Create error type ERR_NOT_SUPPORTED with numeric representation -19
_MAKE_RESULT(-19, ERR_NOT_SUPPORTED);
/// Create error type ERR_NOT_FOUND with numeric representation -20
_MAKE_RESULT(-20, ERR_NOT_FOUND);
/// Create error type ERR_NOT_INITIALIZED with numeric representation -37
_MAKE_RESULT(-37, ERR_NOT_INITIALIZED);
/// Create error type ERR_FAILED with numeric representation -38
_MAKE_RESULT(-38, ERR_FAILED);

using a_util::result::isFailed;
using a_util::result::isOk;

TEST(result_type_tester, TestResultIsStandardLayout)
{
    // EXPECT_TRUE(std::is_standard_layout<a_util::result::Result>::value);
}

TEST(result_type_tester, TestDefaultSuccessCode)
{
    ASSERT_EQ(a_util::result::SUCCESS, a_util::result::ResultInfo<void>());
}

TEST(result_type_tester, TestSwapAndComparison)
{
    using a_util::result::Result;

    Result oSuccess; // a_util::result::SUCCESS
    Result oFailure(
        ERR_FAILED, "Test ERR_FAILED", 666, __FILE__, A_UTIL_CURRENT_FUNCTION); // ERR_FAILED
    EXPECT_TRUE(oSuccess != oFailure);
    EXPECT_TRUE(!(oSuccess == oFailure));

    {
        EXPECT_EQ(Result(), oSuccess);
        EXPECT_NE(oSuccess,
                  Result(a_util::result::SUCCESS,
                         "Test_ERR_NOERROR",
                         666,
                         __FILE__,
                         A_UTIL_CURRENT_FUNCTION));
    }

    {
        // using copy construction we backup the initial values for later comparison
        const Result oSuccessBackup(oSuccess);
        const Result oFailureBackup(oFailure);

        // swap forth and back, using both versions of swap
        std::swap(oSuccess, oFailure);
        EXPECT_TRUE(oFailure == oSuccessBackup);
        EXPECT_TRUE(oSuccess == oFailureBackup);

        a_util::result::swap(oSuccess, oFailure);
        EXPECT_TRUE(oSuccess == oSuccessBackup);
        EXPECT_TRUE(oFailure == oFailureBackup);
    }

    {
        EXPECT_EQ(1, sizeof(a_util::result::ResultInfo<void>));
    }
}

TEST(result_type_tester, TestConstructor)
{
    using a_util::result::Result;

    { // default constructor
        Result oResult;
        EXPECT_TRUE(isOk(Result()));
        EXPECT_STREQ("No error occurred", oResult.getDescription());
        EXPECT_STREQ("SUCCESS", oResult.getErrorLabel());
        EXPECT_STREQ("", oResult.getFile());
        EXPECT_STREQ("", oResult.getFunction());
        EXPECT_TRUE(-1 == oResult.getLine());
    }

    { // Constructor called through a_util::result::SUCCESS
        Result oResult(a_util::result::SUCCESS);
        EXPECT_TRUE(isOk(Result(a_util::result::SUCCESS)));
        EXPECT_STREQ("No error occurred", oResult.getDescription());
        EXPECT_STREQ("SUCCESS", oResult.getErrorLabel());
        EXPECT_STREQ("", oResult.getFile());
        EXPECT_STREQ("", oResult.getFunction());
        EXPECT_TRUE(-1 == oResult.getLine());
    }

    { // Constructor called through a_util::result::SUCCESS error code
        Result oResult(a_util::result::SUCCESS.getCode());
        EXPECT_TRUE(isOk(a_util::result::SUCCESS.getCode()));
        EXPECT_STREQ("No error occurred", oResult.getDescription());
        EXPECT_STREQ("SUCCESS", oResult.getErrorLabel());
        EXPECT_STREQ("", oResult.getFile());
        EXPECT_STREQ("", oResult.getFunction());
        EXPECT_TRUE(-1 == oResult.getLine());
    }

    { // Constructor called through ERR_NOT_INITIALIZED error code (no detailed information)
        Result oResult(ERR_NOT_INITIALIZED.getCode());
        EXPECT_TRUE(isFailed(Result(ERR_NOT_INITIALIZED.getCode())));
        EXPECT_TRUE(ERR_NOT_INITIALIZED.getCode() == oResult.getErrorCode());
        EXPECT_STREQ("", oResult.getDescription());
        EXPECT_STREQ("(unknown)", oResult.getErrorLabel());
        EXPECT_STREQ("", oResult.getFile());
        EXPECT_STREQ("", oResult.getFunction());
        EXPECT_TRUE(-1 == oResult.getLine());
    }

    { // Constructor called through ERR_NOT_FOUND (no detailed information)
        Result oResult(ERR_NOT_FOUND);
        EXPECT_TRUE(isFailed(Result(ERR_NOT_FOUND)));
        EXPECT_TRUE(ERR_NOT_FOUND.getCode() == oResult.getErrorCode());
        EXPECT_STREQ("", oResult.getDescription());
        EXPECT_STREQ(ERR_NOT_FOUND.getLabel(), oResult.getErrorLabel());
        EXPECT_STREQ("", oResult.getFile());
        EXPECT_STREQ("", oResult.getFunction());
        EXPECT_TRUE(-1 == oResult.getLine());
    }

    { // Constructor called through ERR_NOT_FOUND (detailed information)
        Result oResult(ERR_NOT_FOUND, "Test ERR_NOT_FOUND", 666, __FILE__, A_UTIL_CURRENT_FUNCTION);
        EXPECT_TRUE(isFailed(Result(ERR_NOT_FOUND)));
        EXPECT_TRUE(ERR_NOT_FOUND.getCode() == oResult.getErrorCode());
        EXPECT_STREQ("Test ERR_NOT_FOUND", oResult.getDescription());
        EXPECT_STREQ("ERR_NOT_FOUND", oResult.getErrorLabel());
        EXPECT_STREQ(__FILE__, oResult.getFile());
        EXPECT_STREQ(A_UTIL_CURRENT_FUNCTION, oResult.getFunction());
        EXPECT_TRUE(666 == oResult.getLine());
    }

    { // Constructor called through a_util::result::SUCCESS (detailed information - this indicates
      // no error!)
        Result oResult(a_util::result::SUCCESS,
                       "Test SUCCESS detailed",
                       666,
                       __FILE__,
                       A_UTIL_CURRENT_FUNCTION);
        EXPECT_TRUE(isOk(Result(a_util::result::SUCCESS,
                                "Test SUCCESS detailed",
                                666,
                                __FILE__,
                                A_UTIL_CURRENT_FUNCTION)));
        EXPECT_TRUE(a_util::result::SUCCESS.getCode() == oResult.getErrorCode());
        EXPECT_STREQ("Test SUCCESS detailed", oResult.getDescription());
        EXPECT_STREQ("SUCCESS", oResult.getErrorLabel());
        EXPECT_STREQ(__FILE__, oResult.getFile());
        EXPECT_STREQ(A_UTIL_CURRENT_FUNCTION, oResult.getFunction());
        EXPECT_TRUE(666 == oResult.getLine());
    }

    { // Constructor called through ERR_NOT_FOUND error code (no detailed information)
        Result oResult(ERR_NOT_FOUND.getCode());
        EXPECT_TRUE(isFailed(Result(ERR_NOT_FOUND.getCode())));
        EXPECT_TRUE(ERR_NOT_FOUND.getCode() == oResult.getErrorCode());
        EXPECT_STREQ("", oResult.getDescription());
        EXPECT_STREQ("(unknown)", oResult.getErrorLabel());
        EXPECT_STREQ("", oResult.getFile());
        EXPECT_STREQ("", oResult.getFunction());
        EXPECT_TRUE(-1 == oResult.getLine());
    }

    { // Constructor called through a_util::result::SUCCESS error code (no error!)
        Result oResult(a_util::result::SUCCESS.getCode());
        EXPECT_TRUE(isOk(Result(a_util::result::SUCCESS.getCode())));
        EXPECT_TRUE(a_util::result::SUCCESS.getCode() == oResult.getErrorCode());
        EXPECT_STREQ("No error occurred", oResult.getDescription());
        EXPECT_STREQ("SUCCESS", oResult.getErrorLabel());
        EXPECT_STREQ("", oResult.getFile());
        EXPECT_STREQ("", oResult.getFunction());
        EXPECT_TRUE(-1 == oResult.getLine());
    }

    { // Constructor called through ERR_NOT_FOUND error code (detailed information)
        Result oResult(
            ERR_NOT_FOUND.getCode(), "Test ERR_NOT_FOUND", 666, __FILE__, A_UTIL_CURRENT_FUNCTION);
        EXPECT_TRUE(isFailed(Result(ERR_NOT_FOUND.getCode(),
                                    "Test ERR_NOT_FOUND",
                                    666,
                                    __FILE__,
                                    A_UTIL_CURRENT_FUNCTION)));
        EXPECT_TRUE(ERR_NOT_FOUND.getCode() == oResult.getErrorCode());
        EXPECT_STREQ("Test ERR_NOT_FOUND", oResult.getDescription());
        EXPECT_STREQ("(unknown)", oResult.getErrorLabel());
        EXPECT_STREQ(__FILE__, oResult.getFile());
        EXPECT_STREQ(A_UTIL_CURRENT_FUNCTION, oResult.getFunction());
        EXPECT_TRUE(666 == oResult.getLine());
    }

    { // Constructor called through a_util::result::SUCCESS error code (detailed information - no
      // error!)
        Result oResult(a_util::result::SUCCESS.getCode(),
                       "Test SUCCESS",
                       666,
                       __FILE__,
                       A_UTIL_CURRENT_FUNCTION);
        EXPECT_TRUE(isOk(Result(a_util::result::SUCCESS.getCode(),
                                "Test SUCCESS",
                                666,
                                __FILE__,
                                A_UTIL_CURRENT_FUNCTION)));
        EXPECT_TRUE(a_util::result::SUCCESS.getCode() == oResult.getErrorCode());
        EXPECT_STREQ("Test SUCCESS", oResult.getDescription());
        EXPECT_STREQ("SUCCESS", oResult.getErrorLabel());
        EXPECT_STREQ(__FILE__, oResult.getFile());
        EXPECT_STREQ(A_UTIL_CURRENT_FUNCTION, oResult.getFunction());
        EXPECT_TRUE(666 == oResult.getLine());
    }

    { //"Forward" an error through overloaded copy constructor
        // Constructor called through ERR_INVALID_ARG (detailed information)
        Result oResultSrc(
            ERR_INVALID_ARG, "Test ERR_INVALID_ARG", 666, __FILE__, A_UTIL_CURRENT_FUNCTION);
        Result oResultDest(
            oResultSrc, "Test ERR_INVALID_ARG new", 777, "Another File", "Another Func");
        EXPECT_TRUE(isFailed(
            Result(oResultSrc, "Test ERR_INVALID_ARG new", 777, "Another File", "Another Func")));
        EXPECT_TRUE(ERR_INVALID_ARG.getCode() == oResultDest.getErrorCode());
        EXPECT_STREQ("Test ERR_INVALID_ARG new", oResultDest.getDescription());
        EXPECT_STREQ("ERR_INVALID_ARG", oResultDest.getErrorLabel());
        EXPECT_STREQ("Another File", oResultDest.getFile());
        EXPECT_STREQ("Another Func", oResultDest.getFunction());
        EXPECT_TRUE(777 == oResultDest.getLine());
    }

    { // Same as above, but with a_util::result::SUCCESS detailed (indicating no error)
        // Constructor called through a_util::result::SUCCESS (detailed information)
        Result oResultSrc(
            a_util::result::SUCCESS, "Test SUCCESS", 666, __FILE__, A_UTIL_CURRENT_FUNCTION);
        Result oResultDest(oResultSrc, "Test SUCCESS new", 777, "Another File", "Another Func");
        EXPECT_TRUE(
            isOk(Result(oResultSrc, "Test SUCCESS new", 777, "Another File", "Another Func")));
        EXPECT_TRUE(a_util::result::SUCCESS.getCode() == oResultDest.getErrorCode());
        EXPECT_STREQ("Test SUCCESS new", oResultDest.getDescription());
        EXPECT_STREQ("SUCCESS", oResultDest.getErrorLabel());
        EXPECT_STREQ("Another File", oResultDest.getFile());
        EXPECT_STREQ("Another Func", oResultDest.getFunction());
        EXPECT_TRUE(777 == oResultDest.getLine());
    }

    { // Call constructor just with a_util::result::SUCCESS and "forward" it with detailed
      // information
        Result oResultSrc(a_util::result::SUCCESS);
        Result oResultDest(oResultSrc, "Test SUCCESS new", 666, __FILE__, A_UTIL_CURRENT_FUNCTION);
        EXPECT_TRUE(
            isOk(Result(oResultSrc, "Test SUCCESS new", 666, __FILE__, A_UTIL_CURRENT_FUNCTION)));
        EXPECT_TRUE(a_util::result::SUCCESS.getCode() == oResultDest.getErrorCode());
        EXPECT_STREQ("Test SUCCESS new", oResultDest.getDescription());
        EXPECT_STREQ("SUCCESS", oResultDest.getErrorLabel());
        EXPECT_STREQ(__FILE__, oResultDest.getFile());
        EXPECT_STREQ(A_UTIL_CURRENT_FUNCTION, oResultDest.getFunction());
        EXPECT_TRUE(666 == oResultDest.getLine());
    }

    { // Call copy constructor with a_util::result::SUCCESS (both indicating no error)
        const Result oResultSrc(a_util::result::SUCCESS);
        const Result oResultDest(oResultSrc);
        EXPECT_TRUE(isOk(Result(oResultSrc)));
        // Due to the reference counting, the addresses of the detailed info must all be the same
        EXPECT_TRUE(oResultSrc.getErrorCode() == oResultDest.getErrorCode());
        EXPECT_TRUE(oResultSrc.getDescription() == oResultDest.getDescription());
        EXPECT_TRUE(oResultSrc.getErrorLabel() == oResultDest.getErrorLabel());
        EXPECT_TRUE(oResultSrc.getFile() == oResultDest.getFile());
        EXPECT_TRUE(oResultSrc.getFunction() == oResultDest.getFunction());
        EXPECT_TRUE(oResultSrc.getLine() == oResultDest.getLine());
    }

    { // Call copy constructor with ERR_MEMORY without detailed information (indicating an error)
        const Result oResultSrc(ERR_MEMORY);
        const Result oResultDest(oResultSrc);
        // Due to the reference counting, the addresses of the detailed info must all be the same
        EXPECT_TRUE(isFailed(Result(oResultSrc)));
        EXPECT_TRUE(oResultSrc.getErrorCode() == oResultDest.getErrorCode());
        EXPECT_TRUE(oResultSrc.getDescription() == oResultDest.getDescription());
        EXPECT_TRUE(oResultSrc.getErrorLabel() == oResultDest.getErrorLabel());
        EXPECT_TRUE(oResultSrc.getFile() == oResultDest.getFile());
        EXPECT_TRUE(oResultSrc.getFunction() == oResultDest.getFunction());
        EXPECT_TRUE(oResultSrc.getLine() == oResultDest.getLine());
    }

    { // Call copy constructor with ERR_MEMORY with detailed information (indicating an error)
        const Result oResultSrc(
            ERR_MEMORY, "Test ERR_MEMORY", 666, __FILE__, A_UTIL_CURRENT_FUNCTION);
        const Result oResultDest(oResultSrc);
        // Due to the reference counting, the addresses of the detailed info must all be the same
        EXPECT_TRUE(isFailed(Result(oResultSrc)));
        EXPECT_TRUE(oResultSrc.getErrorCode() == oResultDest.getErrorCode());
        EXPECT_TRUE(oResultSrc.getDescription() == oResultDest.getDescription());
        EXPECT_TRUE(oResultSrc.getErrorLabel() == oResultDest.getErrorLabel());
        EXPECT_TRUE(oResultSrc.getFile() == oResultDest.getFile());
        EXPECT_TRUE(oResultSrc.getFunction() == oResultDest.getFunction());
        EXPECT_TRUE(oResultSrc.getLine() == oResultDest.getLine());
    }

    { // Assignment operator called on result indicating an error through a_util::result::SUCCESS
        Result oResultSrc(ERR_INVALID_ADDRESS);
        EXPECT_TRUE(isOk(oResultSrc.operator=(a_util::result::SUCCESS)));
        EXPECT_TRUE(a_util::result::SUCCESS.getCode() == oResultSrc.getErrorCode());
        EXPECT_STREQ("No error occurred", oResultSrc.getDescription());
        EXPECT_STREQ("SUCCESS", oResultSrc.getErrorLabel());
        EXPECT_STREQ("", oResultSrc.getFile());
        EXPECT_STREQ("", oResultSrc.getFunction());
        EXPECT_TRUE(-1 == oResultSrc.getLine());
    }

    { // Assignment operator called on result indicating no error through ERR_TIMEOUT
        Result oResultSrc(a_util::result::SUCCESS);
        EXPECT_TRUE(isFailed(oResultSrc.operator=(ERR_TIMEOUT)));
        EXPECT_TRUE(ERR_TIMEOUT.getCode() == oResultSrc.getErrorCode());
        EXPECT_STREQ("", oResultSrc.getDescription());
        EXPECT_STREQ(ERR_TIMEOUT.getLabel(), oResultSrc.getErrorLabel());
        EXPECT_STREQ("", oResultSrc.getFile());
        EXPECT_STREQ("", oResultSrc.getFunction());
        EXPECT_TRUE(-1 == oResultSrc.getLine());
    }

    { // Move construction test, leaving the src in an empty but valid state (error --> no error)
        Result oResultSrc(
            ERR_NOT_SUPPORTED, "Test ERR_NOT_SUPPORTED", 666, __FILE__, A_UTIL_CURRENT_FUNCTION);
        // temporary values needed to check whether the result really has been moved
        const char* const pStrDesc = oResultSrc.getDescription();
        const char* const pStrErr = oResultSrc.getErrorLabel();
        const char* const pStrFile = oResultSrc.getFile();
        const char* const pStrFunc = oResultSrc.getFunction();

        const Result oResultDest(std::move(oResultSrc));
        EXPECT_TRUE(isFailed(oResultDest));
        EXPECT_TRUE(ERR_NOT_SUPPORTED.getCode() == oResultDest.getErrorCode());
        EXPECT_TRUE(pStrDesc == oResultDest.getDescription());
        EXPECT_TRUE(pStrErr == oResultDest.getErrorLabel());
        EXPECT_TRUE(pStrFile == oResultDest.getFile());
        EXPECT_TRUE(pStrFunc == oResultDest.getFunction());
        EXPECT_TRUE(666 == oResultDest.getLine());

        // the valild state the source is left in must always be a_util::result::SUCCESS
        EXPECT_TRUE(isOk(oResultSrc));
    }

    { // Move assignment test, leaving the src in an empty but valid state (error --> no error)
        Result oResultSrc(
            ERR_NOT_SUPPORTED, "Test ERR_NOT_SUPPORTED", 666, __FILE__, A_UTIL_CURRENT_FUNCTION);
        Result oResultDest;
        // temporary values needed to check whether the result really has been moved
        const char* const pStrDesc = oResultSrc.getDescription();
        const char* const pStrErr = oResultSrc.getErrorLabel();
        const char* const pStrFile = oResultSrc.getFile();
        const char* const pStrFunc = oResultSrc.getFunction();

        oResultDest = std::move(oResultSrc);
        EXPECT_TRUE(isFailed(oResultDest));
        EXPECT_TRUE(ERR_NOT_SUPPORTED.getCode() == oResultDest.getErrorCode());
        EXPECT_TRUE(pStrDesc == oResultDest.getDescription());
        EXPECT_TRUE(pStrErr == oResultDest.getErrorLabel());
        EXPECT_TRUE(pStrFile == oResultDest.getFile());
        EXPECT_TRUE(pStrFunc == oResultDest.getFunction());
        EXPECT_TRUE(666 == oResultDest.getLine());

        // the valild state the source is left in must always be a_util::result::SUCCESS
        EXPECT_TRUE(isOk(oResultSrc));
    }

    { // Move assignment test, leaving the src in an empty but valid state (no error --> error)
        Result oResultSrc;
        Result oResultDest(
            ERR_NOT_SUPPORTED, "Test ERR_NOT_SUPPORTED", 666, __FILE__, A_UTIL_CURRENT_FUNCTION);
        // temporary values needed to check whether the result really has been moved
        const char* const pStrDesc = oResultSrc.getDescription();
        const char* const pStrErr = oResultSrc.getErrorLabel();
        const char* const pStrFile = oResultSrc.getFile();
        const char* const pStrFunc = oResultSrc.getFunction();

        oResultDest = std::move(oResultSrc);
        EXPECT_TRUE(isOk(oResultDest));
        EXPECT_TRUE(a_util::result::SUCCESS.getCode() == oResultDest.getErrorCode());
        EXPECT_TRUE(pStrDesc == oResultDest.getDescription());
        EXPECT_TRUE(pStrErr == oResultDest.getErrorLabel());
        EXPECT_TRUE(pStrFile == oResultDest.getFile());
        EXPECT_TRUE(pStrFunc == oResultDest.getFunction());
        EXPECT_TRUE(-1 == oResultDest.getLine());

        // the valild state the source is left in must always be a_util::result::SUCCESS
        EXPECT_TRUE(isOk(oResultSrc));
    }

    { // Test with positive error code
        Result oResult(1234);
        EXPECT_TRUE(isFailed(Result(1234)));
        EXPECT_TRUE(1234 == oResult.getErrorCode());
        EXPECT_STREQ("", oResult.getDescription());
        EXPECT_STREQ("(unknown)", oResult.getErrorLabel());
        EXPECT_STREQ("", oResult.getFile());
        EXPECT_STREQ("", oResult.getFunction());
        EXPECT_TRUE(-1 == oResult.getLine());
    }

    { // Test with positive error code and detailed description
        Result oResult(1234, "Test ERR_POSITIVE", 666, __FILE__, A_UTIL_CURRENT_FUNCTION);
        EXPECT_TRUE(isFailed(Result(1234)));
        EXPECT_TRUE(1234 == oResult.getErrorCode());
        EXPECT_STREQ("Test ERR_POSITIVE", oResult.getDescription());
        EXPECT_STREQ("(unknown)", oResult.getErrorLabel());
        EXPECT_STREQ(__FILE__, oResult.getFile());
        EXPECT_STREQ(A_UTIL_CURRENT_FUNCTION, oResult.getFunction());
        EXPECT_TRUE(666 == oResult.getLine());
    }

    { // Test with random number to get the error code during runtime
        // use current time as seed for random generator
        std::srand(static_cast<std::uint32_t>(std::time(nullptr)));
        const std::int32_t nRandom = static_cast<std::int32_t>(std::rand());
        Result oResult(nRandom);
        EXPECT_TRUE(nRandom == oResult.getErrorCode());
        EXPECT_STREQ("", oResult.getDescription());
        EXPECT_STREQ("(unknown)", oResult.getErrorLabel());
        EXPECT_STREQ("", oResult.getFile());
        EXPECT_STREQ("", oResult.getFunction());
        EXPECT_TRUE(-1 == oResult.getLine());
    }
}

TEST(result_type_tester, TestIsOk)
{
    using a_util::result::Result;

    { // with a_util::result::SUCCESS
        Result oResult;
        EXPECT_TRUE(isOk(oResult));
        EXPECT_TRUE(oResult.getErrorCode() == a_util::result::SUCCESS.getCode());
    }

    { // with an error code
        Result oResult(ERR_FAILED);
        EXPECT_TRUE(!isOk(oResult));
        EXPECT_TRUE(oResult.getErrorCode() != a_util::result::SUCCESS.getCode());
    }
}

TEST(result_type_tester, TestIsFailed)
{
    using a_util::result::Result;

    { // with an error code
        Result oResult(ERR_FAILED);
        EXPECT_TRUE(isFailed(oResult));
        EXPECT_TRUE(oResult.getErrorCode() == ERR_FAILED.getCode());
    }

    { // with no error
        Result oResult;
        EXPECT_TRUE(!isFailed(oResult));
        EXPECT_TRUE(oResult.getErrorCode() != ERR_FAILED.getCode());
    }
}

TEST(result_type_tester, TestBoolConvert)
{
    using a_util::result::Result;

    // direct initialization
    EXPECT_TRUE(isOk(Result(true)));
    EXPECT_TRUE(isFailed(Result(false)));

    { // copy initialization
        Result oResult = false;
        EXPECT_TRUE(isFailed(oResult));

        // copy assignment
        oResult = true;
        EXPECT_TRUE(isOk(oResult));
    }

    { // copy initialization
        Result oResult = true;
        EXPECT_TRUE(isOk(oResult));

        // copy assignment
        oResult = false;
        EXPECT_TRUE(isFailed(oResult));
    }
}

TEST(result_type_tester, TestInt32Convert)
{
    using a_util::result::Result;

    // direct initialization
    EXPECT_TRUE(isOk(Result(0)));
    EXPECT_TRUE(isFailed(Result(1)));
    EXPECT_TRUE(isFailed(Result(-1)));

    { // copy initialization
        Result oResult = 1;
        EXPECT_TRUE(isFailed(oResult));

        // copy assignment
        oResult = 0;
        EXPECT_TRUE(isOk(oResult));
        oResult = -1;
        EXPECT_TRUE(isFailed(oResult));
    }

    { // copy initialization
        Result oResult = 0;
        EXPECT_TRUE(isOk(oResult));

        // copy assignment
        oResult = 1;
        EXPECT_TRUE(isFailed(oResult));
    }

    { // copy initialization
        Result oResult = -1;
        EXPECT_TRUE(isFailed(oResult));

        // copy assignment
        oResult = 0;
        EXPECT_TRUE(isOk(oResult));
    }
}
