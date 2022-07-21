/**
 * @file
 * A_UTIL_DLL_EXPORT macro test for dll_import
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

#ifdef A_UTIL_DO_EXPORT
#undef A_UTIL_DO_EXPORT
#endif // !A_UTIL_DO_EXPORT

#include <a_util/preprocessor/dll_export.h>
#include <a_util/preprocessor/to_string.h>

#include <gtest/gtest.h>

/**
 * Test whether A_UTIL_DDL_EXPORT sets the correct symbol visibility.
 * @req_id TODO
 */
TEST(DLLImportMacro, exportsCorrectSymbolVisibility)
{ // Currently I have no better idea how to test this
    const char declspec[] = "" A_UTIL_TO_STRING(A_UTIL_DLL_EXPORT);
#if defined(__MINGW32__)
    EXPECT_STREQ(declspec, "__attribute__((dllimport))");
#elif defined(_WIN32)
    EXPECT_STREQ(declspec, "__declspec(dllimport)");
#else
    EXPECT_STREQ(declspec, "");
#endif // defined(_WIN32)
}
