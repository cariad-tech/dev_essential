/**
 * @file
 * Regex test implementation
 *
 * Copyright @ 2021 VW Group. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <a_util/regex/regularexpression.h>

#include <gtest/gtest.h>

// Test ctors, cctor, assign op, Set/getPattern, getError
TEST(regex_test, TestBasics)
{
    using a_util::regex::RegularExpression;

    // default ctor
    RegularExpression re;
    EXPECT_EQ(re.getPattern(), "");
    EXPECT_EQ(re.getError(), "");

    // initializing ctor
    RegularExpression re2(".*");
    EXPECT_EQ(re2.getPattern(), ".*");
    EXPECT_EQ(re2.getError(), "");

    // assignment op
    re = re2;
    EXPECT_EQ(re.getPattern(), ".*");
    EXPECT_EQ(re.getError(), "");

    // Get-/setPattern/getError
    EXPECT_FALSE(re.setPattern(""));
    EXPECT_EQ(re.getError(), "Empty pattern is invalid");
    EXPECT_TRUE(re.setPattern("a"));
    EXPECT_EQ(re.getError(), "");
}

// Test fullMatch
TEST(regex_test, TestFullMatch)
{
    using a_util::regex::RegularExpression;

    RegularExpression re;

    // successful match
    EXPECT_TRUE(re.setPattern("h.*o"));
    EXPECT_TRUE(re.fullMatch("hello"));

    // unsuccessful match
    EXPECT_TRUE(re.setPattern("e"));
    EXPECT_FALSE(re.fullMatch("hello"));

    // temporary object
    EXPECT_TRUE(RegularExpression("h.*o").fullMatch("hello"));

    // case sensitivity
    EXPECT_FALSE(RegularExpression("hello").fullMatch("HeLLo"));
    EXPECT_TRUE(RegularExpression("hello", false).fullMatch("HeLLo"));

    // capture extraction
    std::string s1, s2;
    EXPECT_TRUE(re.setPattern("(\\w+):(\\d+)"));
    EXPECT_TRUE(re.fullMatch("ruby:1234", s1, s2));
    EXPECT_EQ(s1, "ruby");
    EXPECT_EQ(s2, "1234");
    EXPECT_FALSE(re.fullMatch("1234:ruby", s1, s2));

    // one arg less than required
    EXPECT_TRUE(re.fullMatch("ruby:1234", s1));

    // No groups
    EXPECT_FALSE(RegularExpression("\\w+:\\d+").fullMatch("ruby:1234", s1));

    // PARSING HEX/OCTAL/C-RADIX NUMBERS
    std::string a, b, c, d;
    re.setPattern("(.*) (.*) (.*) (.*)");
    EXPECT_TRUE(re.fullMatch("100 40 0100 0x40", a, b, c, d));

    RegularExpression big(
        "([0-9]*[.][0-9]+)\\s+(\\d+)\\s+([0-9a-f]*)x?\\s+(\\w{2})\\s+\\w\\s(\\d)\\s"
        "([0-9a-f]{2})(\\s[0-9a-f]{2})?(\\s[0-9a-f]{2})?(\\s[0-9a-f]{2})?(\\s[0-9a-f]{2})?"
        "(\\s[0-9a-f]{2})?(\\s[0-9a-f]{2})?(\\s[0-9a-f]{2})?",
        false);

    std::string args[13];
    EXPECT_TRUE(big.fullMatch("0.0000 1  20x              Rx   d 8 01 02 03 04 05 06 07 08",
                              args[0],
                              args[1],
                              args[2],
                              args[3],
                              args[4],
                              args[5],
                              args[6],
                              args[7],
                              args[8],
                              args[9],
                              args[10],
                              args[11],
                              args[12]));
    EXPECT_EQ(args[0], "0.0000");
    EXPECT_EQ(args[1], "1");
    EXPECT_EQ(args[2], "20");
    EXPECT_EQ(args[3], "Rx");
    EXPECT_EQ(args[4], "8");
    EXPECT_EQ(args[5], "01");
    EXPECT_EQ(args[6], " 02");
    EXPECT_EQ(args[7], " 03");
    EXPECT_EQ(args[8], " 04");
    EXPECT_EQ(args[9], " 05");
    EXPECT_EQ(args[10], " 06");
    EXPECT_EQ(args[11], " 07");
    EXPECT_EQ(args[12], " 08");
}

// Test DoMatch
TEST(regex_test, TestDoMatch)
{
    using a_util::regex::RegularExpression;

    int consumed = 0;
    RegularExpression text_exp("(\\d+)-(.*)-(\\d+)");
    EXPECT_TRUE(text_exp.match("1234-test-4321", RegularExpression::AT_Start, consumed));
    EXPECT_EQ(consumed, 14);
    EXPECT_TRUE(text_exp.match("1234-test-4321", RegularExpression::AT_Both, consumed));
    EXPECT_EQ(consumed, 14);

    EXPECT_TRUE(text_exp.match("1234-test-4321a", RegularExpression::AT_Start, consumed));
    EXPECT_EQ(consumed, 14);
    EXPECT_FALSE(text_exp.match("1234-test-4321a", RegularExpression::AT_Both, consumed));

    EXPECT_FALSE(text_exp.match("a1234-test-4321", RegularExpression::AT_Both, consumed));

    EXPECT_FALSE(text_exp.match("a1234-test-4321a", RegularExpression::AT_Start, consumed));
    EXPECT_TRUE(text_exp.match("a1234-test-4321a", RegularExpression::AT_Unanchored, consumed));
    EXPECT_EQ(consumed, 15);
}
