/**
 * DEV_ESSENTIAL_DEPRECATED function macro test for
 *
 * Copyright @ 2022 VW Group. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef DEV_ESSENTIAL_DISABLE_DEPRECATED_WARNINGS
#define DEV_ESSENTIAL_DISABLE_DEPRECATED_WARNINGS
#endif // !DEV_ESSENTIAL_DISABLE_DEPRECATED_WARNINGS

#include <a_util/preprocessor/deprecated.h>
#include <a_util/preprocessor/to_string.h>

#include <gtest/gtest.h>

/**
 * Test whether DEV_ESSENTIAL_DEPRECATED() function macro expands to empty if
 * DEV_ESSENTIAL_DISABLE_DEPRECATED_WARNINGS is defined.
 */
TEST(DeprecatedMacro, expandsToEmptyIfDeprecatedWarningsAreDisabled)
{
    const auto deprecated_message = "" DEV_ESSENTIAL_DEPRECATED("Expands empty");
    EXPECT_STREQ("", deprecated_message);
}

#undef A_UTIL_UTIL_PREPROCESSOR_DEPRECATED_HEADER_INCLUDED
#undef DEV_ESSENTIAL_DISABLE_DEPRECATED_WARNINGS
#undef DEV_ESSENTIAL_DEPRECATED

#include <a_util/preprocessor/deprecated.h>

/**
 * Test whether DEV_ESSENTIAL_DEPRECATED() function macro expands to [[deprecated(string-literal)]]
 * attribute if DEV_ESSENTIAL_DISABLE_DEPRECATED_WARNINGS is not defined.
 */
TEST(DeprecatedMacro, expandsToAttributeDeprecatedIfDeprecatedWarningsAreEnabled)
{
    const auto deprecated_message = A_UTIL_TO_STRING(DEV_ESSENTIAL_DEPRECATED("dEpReCaTeD"));
    EXPECT_STREQ("[[deprecated(\"dEpReCaTeD\" \" To disable this warning, define \" "
                 "\"DEV_ESSENTIAL_DISABLE_DEPRECATED_WARNINGS.\")]]",
                 deprecated_message);
}
