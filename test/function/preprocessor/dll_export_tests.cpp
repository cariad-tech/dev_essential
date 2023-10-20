/**
 * @file
 * A_UTIL_DLL_EXPORT macro test for dll_export
 *
 * Copyright @ 2021 VW Group. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef A_UTIL_DO_EXPORT
#define A_UTIL_DO_EXPORT
#endif // !A_UTIL_DO_EXPORT

#include <a_util/preprocessor/dll_export.h>
#include <a_util/preprocessor/to_string.h>

#include <gtest/gtest.h>

/**
 * Test whether A_UTIL_DDL_EXPORT sets the correct symbol visibility.
 * @req_id TODO
 */
TEST(DLLExportMacro, exportsCorrectSymbolVisibility)
{ // Currently I have no better idea how to test this
    const char declspec[] = "" A_UTIL_TO_STRING(A_UTIL_DLL_EXPORT);
#if defined(__MINGW32__)
    EXPECT_STREQ(declspec, "__attribute__((dllexport))");
#elif defined(_WIN32)
    EXPECT_STREQ(declspec, "__declspec(dllexport)");
#else
    EXPECT_STREQ(declspec, "__attribute__((visibility(\"default\")))");
#endif // defined(_WIN32)
}
