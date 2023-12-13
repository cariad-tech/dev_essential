/**
 * @file
 * Test a_util::maybe_unused function template
 *
 * Copyright @ 2021 VW Group. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <a_util/base/types.h>

#include <gtest/gtest.h>

class MaybeUnused : public ::testing::Test {
protected:
    bool faultyCompareWithUnreferencedFormalParameter(int left, int right) noexcept
    {
        using a_util::maybe_unused;
        const auto val = left;
        maybe_unused(right);
        return val == left;
    }

    bool faultyCompareWithLocalVariableIsInitializedButNotReferenced(int left, int right) noexcept
    {
        using a_util::maybe_unused;
        bool val = (left == right);
        maybe_unused(val);
        return true;
    }
};

TEST_F(MaybeUnused, silencesCompilerWarnings)
{
    EXPECT_TRUE(faultyCompareWithUnreferencedFormalParameter(42, 43));
    EXPECT_TRUE(faultyCompareWithLocalVariableIsInitializedButNotReferenced(42, 43));
}
