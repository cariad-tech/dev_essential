/**
 * Common error code and condition implementation test
 *
 * Copyright @ 2022 VW Group. All rights reserved.
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

#include <a_util/base/error.h>

#include <gtest/gtest.h>

#include <type_traits>

constexpr bool operator==(std::underlying_type<a_util::errc>::type lhs, a_util::errc rhs) noexcept
{
    return static_cast<typename std::underlying_type_t<a_util::errc>>(rhs) == lhs;
}

#define STATIC_ASSERT_EQ(expected_val, enum_val)                                                   \
    static_assert(expected_val == a_util::errc::enum_val,                                          \
                  "'a_util::errc::" #enum_val                                                      \
                  "' does not have the expected value of '" #expected_val "'")

/**
 * Check whether the errc values comply to the defined values
 */
TEST(ErrorConditions, haveSpecifiedValues){
#include "errc_test.inc"
}

/**
 * Check whether the error conditions are defined in an enum
 */
TEST(ErrorConditions, areEnumValues)
{
    static_assert(std::is_enum<a_util::errc>::value, "a_util::errc needs to be an enum!");
}
