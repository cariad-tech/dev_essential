/**
 * Copyright @ 2021 VW Group. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

// bring declspec into scope from here, otherwise it might interfere with a_util includes
#include "../../../../../include/a_util/preprocessor/dll_export.h"

// comment to tell clang-format to not put this include on top
#include "a_util_result_legacy.h"

// this is a_util result 5.0.0 include!
#include <a_util/result/detail/result_type_impl.h>

#include <gtest/gtest.h>

a_util::result::Result LegacyResult::createWithDefaultCtor()
{
    return a_util::result::Result();
}

a_util::result::Result LegacyResult::createWithErrorCode(std::int32_t error_code)
{
    return a_util::result::Result(error_code);
}

a_util::result::Result LegacyResult::createWithSUCCESS()
{
    return a_util::result::Result(a_util::result::SUCCESS);
}

a_util::result::Result LegacyResult::createWithBool(bool val)
{
    return a_util::result::Result(val);
}

a_util::result::Result LegacyResult::createWithERR_FAILED()
{
    return a_util::result::Result(ERR_FAILED);
}

a_util::result::Result LegacyResult::createWithDetailedERR_FAILED(const char* error_description,
                                                                  std::int32_t line,
                                                                  const char* file,
                                                                  const char* function)
{
    return a_util::result::Result(ERR_FAILED, error_description, line, file, function);
}

void LegacyResult::testAndDestroyCurrentResult(a_util::result::Result result,
                                               std::int32_t error_code,
                                               const char* label,
                                               const char* error_description,
                                               std::int32_t line,
                                               const char* file,
                                               const char* function)
{
    EXPECT_EQ(error_code, result.getErrorCode());
    EXPECT_EQ(line, result.getLine());
    EXPECT_STREQ(label, result.getErrorLabel());
    EXPECT_STREQ(error_description, result.getDescription());
    EXPECT_STREQ(file, result.getFile());
    EXPECT_STREQ(function, result.getFunction());
}
