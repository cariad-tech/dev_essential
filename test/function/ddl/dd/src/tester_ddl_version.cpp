/**
 * @file
 * Implementation of the tester for the DataDefinition representation and its versions
 *
 * Copyright @ 2022 VW Group. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "static_ddl_version_test_lib.h"

#include <ddl/dd/ddstructure.h>

#include <gtest/gtest.h>

/**
 * @detail Check if the right initial version is set
 */
TEST(TesterVersionChecks, versionCheckStructure)
{
    const auto& structure_version_check_1 = getTestStructure1();
    EXPECT_EQ(structure_version_check_1.getDD().getVersion(), ddl::dd::Version(4, 1));
    std::cout << structure_version_check_1.getStructDescription();
    const auto& structure_version_check_2 = getTestStructure2();
    EXPECT_EQ(structure_version_check_2.getDD().getVersion(), ddl::dd::Version(4, 1));
    std::cout << structure_version_check_2.getStructDescription();
}

/**
 * @detail Check if the right initial version is set
 */
TEST(TesterVersionChecks, versionCheckDD)
{
    const auto& dd_default41 = getDefaultTestDD();
    EXPECT_EQ(dd_default41.getVersion(), ddl::dd::Version(4, 1));

    const auto& dd_30 = getTestDD30();
    EXPECT_EQ(dd_30.getVersion(), ddl::dd::Version(3, 0));
}
