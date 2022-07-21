/**
 * @file
 * Strings test implementation
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

#include <a_util/memory.h>
#include <a_util/strings/strings_functions.h>

#include <gtest/gtest.h>

using namespace a_util;

TEST(string_test, TestStringCompare)
{
    // compare/compareNoCase

    // negative
    EXPECT_GT(0, strings::compare(nullptr, ""));
    EXPECT_LT(0, strings::compare("", nullptr));
    EXPECT_NE(0, strings::compare(nullptr, nullptr));

    EXPECT_GT(0, strings::compare("", "abcdefg", 0, size_t(-1)));          // 0
    EXPECT_GT(0, strings::compare("", "abcdefg", size_t(-5), size_t(-1))); // 0
    EXPECT_GT(0, strings::compare("", "abcdefg", size_t(-5), 5));
    EXPECT_GT(0, strings::compare("", "abcdefg", 0, 5));

    EXPECT_GT(0, strings::compare("", "abcdefg", 3));  // 0
    EXPECT_GT(0, strings::compare("", "abcdefg", 99)); // 0
    EXPECT_GT(0, strings::compare("", "abcdefg", 0));  // 0
    EXPECT_GT(0, strings::compare("", "abcdefg"));     // 0
    EXPECT_GT(0, strings::compare("", "abcdefg"));     // 0

    EXPECT_GT(0, strings::compareNoCase("", "abcdefg", 0, size_t(-1)));          // 0
    EXPECT_GT(0, strings::compareNoCase("", "abcdefg", size_t(-5), size_t(-1))); // 0
    EXPECT_GT(0, strings::compareNoCase("", "abcdefg", size_t(-5), 5));
    EXPECT_GT(0, strings::compareNoCase("", "abcdefg", 0, 5));

    EXPECT_GT(0, strings::compareNoCase("", "abcdefg", 3));  // 0
    EXPECT_GT(0, strings::compareNoCase("", "abcdefg", 99)); // 0
    EXPECT_GT(0, strings::compareNoCase("", "abcdefg", 0));  // 0
    EXPECT_GT(0, strings::compareNoCase("", "abcdefg"));     // 0
    EXPECT_GT(0, strings::compareNoCase("", "abcdefg"));     // 0

    EXPECT_EQ(0, strings::compare("", "", 0, size_t(-1)));          // 0
    EXPECT_EQ(0, strings::compare("", "", size_t(-5), size_t(-1))); // 0
    EXPECT_EQ(0, strings::compare("", "", size_t(-5), 5));
    EXPECT_EQ(0, strings::compare("", "", 0, 5));

    EXPECT_EQ(0, strings::compare("", "", 3));  // 0
    EXPECT_EQ(0, strings::compare("", "", 99)); // 0
    EXPECT_EQ(0, strings::compare("", "", 0));  // 0
    EXPECT_EQ(0, strings::compare("", ""));     // 0
    EXPECT_EQ(0, strings::compare("", ""));     // 0

    EXPECT_EQ(0, strings::compareNoCase("", "", 0, size_t(-1)));          // 0
    EXPECT_EQ(0, strings::compareNoCase("", "", size_t(-5), size_t(-1))); // 0
    EXPECT_EQ(0, strings::compareNoCase("", "", size_t(-5), 5));
    EXPECT_EQ(0, strings::compareNoCase("", "", 0, 5));

    EXPECT_EQ(0, strings::compareNoCase("", "", 3));  // 0
    EXPECT_EQ(0, strings::compareNoCase("", "", 99)); // 0
    EXPECT_EQ(0, strings::compareNoCase("", "", 0));  // 0
    EXPECT_EQ(0, strings::compareNoCase("", ""));     // 0
    EXPECT_EQ(0, strings::compareNoCase("", ""));     // 0

    // static members
    EXPECT_EQ(0, strings::compare("", ""));
    EXPECT_EQ(0, strings::compare("", "", 10));
    EXPECT_EQ(0, strings::compare("", "", size_t(-10)));
    EXPECT_EQ(0, strings::compare("", ""));
    EXPECT_EQ(0, strings::compare("", "", 10));
    EXPECT_EQ(0, strings::compare("", "", size_t(-10)));
    EXPECT_EQ(0, strings::compare("a", "a", 10));
    EXPECT_EQ(0, strings::compare("a", "a", size_t(-10)));

    EXPECT_EQ(0, strings::compareNoCase("", ""));
    EXPECT_EQ(0, strings::compareNoCase("", "", 10));
    EXPECT_EQ(0, strings::compareNoCase("", "", size_t(-10)));
    EXPECT_EQ(0, strings::compareNoCase("", ""));
    EXPECT_EQ(0, strings::compareNoCase("", "", 10));
    EXPECT_EQ(0, strings::compareNoCase("", "", size_t(-10)));
    EXPECT_EQ(0, strings::compareNoCase("a", "a", 10));
    EXPECT_EQ(0, strings::compareNoCase("a", "a", size_t(-10)));

    // positive
    EXPECT_EQ(0, strings::compare("abcdefg", "abcdefg", 0, size_t(-1))); // 0
    EXPECT_LE(0, strings::compare("abcdefg", "abcdefg", 3));             // 0
    EXPECT_EQ(0, strings::compare("abcdefg", "abcdefg", 99));            // 0
    EXPECT_EQ(1, strings::compare("abcdefg", "abc", 5));                 // 1
    EXPECT_EQ(0, strings::compare("abcdefg", "abcdefg", 0, 2));          // 0
    EXPECT_EQ(0, strings::compare("abcdefg", "abcdefg", 3, 2));          // 0
    EXPECT_EQ(0, strings::compare("abcdefg", "abcdefg", 99, 2));         // 0
    EXPECT_LE(0, strings::compare("abcdefg", "abc", 2, 2));              // 1
    EXPECT_EQ(1, strings::compare("abcdefg", "abc", 5, 2));              // 1

    EXPECT_GT(0, strings::compare("abc", "abcdefg", 5));    // -1
    EXPECT_GT(0, strings::compare("abc", "abcdefg", 2, 2)); // -1
    EXPECT_GT(0, strings::compare("abc", "abcdefg", 5, 2)); // -1

    EXPECT_EQ(0, strings::compareNoCase("abcdefg", "ABCDEFG", 0, size_t(-1))); // 0
    EXPECT_EQ(0, strings::compareNoCase("abcdefg", "ABCDEFG", 3));             // 0
    EXPECT_EQ(0, strings::compareNoCase("abcdefg", "ABCDEFG", 99));            // 0
    EXPECT_EQ(1, strings::compareNoCase("abcdefg", "ABC", 5));                 // 1
    EXPECT_EQ(0, strings::compareNoCase("abcdefg", "ABCDEFG", 0, 2));          // 0
    EXPECT_EQ(0, strings::compareNoCase("abcdefg", "ABCDEFG", 3, 2));          // 0
    EXPECT_EQ(0, strings::compareNoCase("abcdefg", "ABCDEFG", 99, 2));         // 0
    EXPECT_LT(0, strings::compareNoCase("abcdefg", "ABC", 2, 2));              // 1
    EXPECT_LT(0, strings::compareNoCase("abcdefg", "ABC", 5, 2));              // 1

    EXPECT_GT(0, strings::compareNoCase("abc", "ABCDEFG", 5));    // -1
    EXPECT_GT(0, strings::compareNoCase("abc", "ABCDEFG", 2, 2)); // -1
    EXPECT_GT(0, strings::compareNoCase("abc", "ABCDEFG", 5, 2)); // -1
}

TEST(string_test, TestStringIsEqual)
{
    // isEqual, isEqualNoCase

    // negative
    EXPECT_FALSE(strings::isEqual("", "abcdefg", 0, size_t(-1)));          // 0
    EXPECT_FALSE(strings::isEqual("", "abcdefg", size_t(-5), size_t(-1))); // 0
    EXPECT_FALSE(strings::isEqual("", "abcdefg", size_t(-5), 5));
    EXPECT_FALSE(strings::isEqual("", "abcdefg", 0, 5));

    EXPECT_FALSE(strings::isEqual("", "abcdefg", 3));  // 0
    EXPECT_FALSE(strings::isEqual("", "abcdefg", 99)); // 0
    EXPECT_FALSE(strings::isEqual("", "abcdefg", 0));  // 0
    EXPECT_FALSE(strings::isEqual("", "abcdefg"));     // 0
    EXPECT_FALSE(strings::isEqual("", "abcdefg"));     // 0

    EXPECT_FALSE(strings::isEqual("", "abcdefg", 0, size_t(-1)));          // 0
    EXPECT_FALSE(strings::isEqual("", "abcdefg", size_t(-5), size_t(-1))); // 0
    EXPECT_FALSE(strings::isEqual("", "abcdefg", size_t(-5), 5));
    EXPECT_FALSE(strings::isEqual("", "abcdefg", 0, 5));

    EXPECT_FALSE(strings::isEqual("", "abcdefg", 3));  // 0
    EXPECT_FALSE(strings::isEqual("", "abcdefg", 99)); // 0
    EXPECT_FALSE(strings::isEqual("", "abcdefg", 0));  // 0
    EXPECT_FALSE(strings::isEqual("", "abcdefg"));     // 0
    EXPECT_FALSE(strings::isEqual("", "abcdefg"));     // 0

    EXPECT_TRUE(strings::isEqual("", "", 0, size_t(-1)));          // 0
    EXPECT_TRUE(strings::isEqual("", "", size_t(-5), size_t(-1))); // 0
    EXPECT_TRUE(strings::isEqual("", "", size_t(-5), 5));
    EXPECT_TRUE(strings::isEqual("", "", 0, 5));

    EXPECT_TRUE(strings::isEqual("", "", 3));  // 0
    EXPECT_TRUE(strings::isEqual("", "", 99)); // 0
    EXPECT_TRUE(strings::isEqual("", "", 0));  // 0
    EXPECT_TRUE(strings::isEqual("", ""));     // 0
    EXPECT_TRUE(strings::isEqual("", ""));     // 0

    EXPECT_TRUE(strings::isEqual("", "", 0, size_t(-1)));          // 0
    EXPECT_TRUE(strings::isEqual("", "", size_t(-5), size_t(-1))); // 0
    EXPECT_TRUE(strings::isEqual("", "", size_t(-5), 5));
    EXPECT_TRUE(strings::isEqual("", "", 0, 5));

    EXPECT_TRUE(strings::isEqual("", "", 3));  // 0
    EXPECT_TRUE(strings::isEqual("", "", 99)); // 0
    EXPECT_TRUE(strings::isEqual("", "", 0));  // 0
    EXPECT_TRUE(strings::isEqual("", ""));     // 0
    EXPECT_TRUE(strings::isEqual("", ""));     // 0

    // static members
    EXPECT_TRUE(strings::isEqual("", ""));
    EXPECT_TRUE(strings::isEqual("", "", 10));
    EXPECT_TRUE(strings::isEqual("", "", size_t(-10)));
    EXPECT_TRUE(strings::isEqual("", ""));
    EXPECT_TRUE(strings::isEqual("", "", 10));
    EXPECT_TRUE(strings::isEqual("", "", size_t(-10)));
    EXPECT_TRUE(strings::isEqual("a", "a", 10));
    EXPECT_TRUE(strings::isEqual("a", "a", size_t(-10)));

    EXPECT_TRUE(strings::isEqualNoCase("", ""));
    EXPECT_TRUE(strings::isEqualNoCase("", "", 10));
    EXPECT_TRUE(strings::isEqualNoCase("", "", size_t(-10)));
    EXPECT_TRUE(strings::isEqualNoCase("", ""));
    EXPECT_TRUE(strings::isEqualNoCase("", "", 10));
    EXPECT_TRUE(strings::isEqualNoCase("", "", size_t(-10)));
    EXPECT_TRUE(strings::isEqualNoCase("a", "a", 10));
    EXPECT_TRUE(strings::isEqualNoCase("a", "a", size_t(-10)));

    // positive
    EXPECT_TRUE(strings::isEqual("abcdefg", "abcdefg", 0, size_t(-1))); // 0
    EXPECT_TRUE(strings::isEqual("abcdefg", "abcdefg", 3));             // 0
    EXPECT_TRUE(strings::isEqual("abcdefg", "abcdefg", 99));            // 0
    EXPECT_FALSE(strings::isEqual("abcdefg", "abc", 5));                // 1
    EXPECT_TRUE(strings::isEqual("abcdefg", "abcdefg", 0, 2));          // 0
    EXPECT_TRUE(strings::isEqual("abcdefg", "abcdefg", 3, 2));          // 0
    EXPECT_TRUE(strings::isEqual("abcdefg", "abcdefg", 99, 2));         // 0
    EXPECT_FALSE(strings::isEqual("abcdefg", "abc", 2, 2));             // 1
    EXPECT_FALSE(strings::isEqual("abcdefg", "abc", 5, 2));             // 1

    EXPECT_FALSE(strings::isEqual("abc", "abcdefg", 5));    // -1
    EXPECT_FALSE(strings::isEqual("abc", "abcdefg", 2, 2)); // -1
    EXPECT_FALSE(strings::isEqual("abc", "abcdefg", 5, 2)); // -1

    EXPECT_TRUE(strings::isEqualNoCase("abcdefg", "ABCDEFG", 0, size_t(-1))); // 0
    EXPECT_TRUE(strings::isEqualNoCase("abcdefg", "ABCDEFG", 3));             // 0
    EXPECT_TRUE(strings::isEqualNoCase("abcdefg", "ABCDEFG", 99));            // 0
    EXPECT_FALSE(strings::isEqualNoCase("abcdefg", "ABC", 5));                // 1
    EXPECT_TRUE(strings::isEqualNoCase("abcdefg", "ABCDEFG", 0, 2));          // 0
    EXPECT_TRUE(strings::isEqualNoCase("abcdefg", "ABCDEFG", 3, 2));          // 0
    EXPECT_TRUE(strings::isEqualNoCase("abcdefg", "ABCDEFG", 99, 2));         // 0
    EXPECT_FALSE(strings::isEqualNoCase("abcdefg", "ABC", 2, 2));             // 1
    EXPECT_FALSE(strings::isEqualNoCase("abcdefg", "ABC", 5, 2));             // 1

    EXPECT_FALSE(strings::isEqualNoCase("abc", "ABCDEFG", 5));    // -1
    EXPECT_FALSE(strings::isEqualNoCase("abc", "ABCDEFG", 2, 2)); // -1
    EXPECT_FALSE(strings::isEqualNoCase("abc", "ABCDEFG", 5, 2)); // -1
}

TEST(string_test, TestStringGetLength)
{
    // getLength
    EXPECT_EQ(strings::getLength(nullptr), std::string::npos);
    EXPECT_EQ(strings::getLength(""), 0);
    EXPECT_EQ(strings::getLength("a"), 1);
    EXPECT_EQ(strings::getLength("abcdef"), 6);
}

TEST(string_test, TestStringSplitToken)
{
    // split
    std::vector<std::string> vec;
    vec = strings::splitToken("");
    EXPECT_TRUE(vec.empty());

    vec = strings::splitToken("  ");
    EXPECT_TRUE(vec.empty());

    vec = strings::splitToken("abc", "");
    ASSERT_TRUE(vec.size() == 1);
    EXPECT_TRUE(vec[0] == "abc");

    vec = strings::splitToken("  ", strings::white_space, true);
    ASSERT_TRUE(vec.size() == 3);
    EXPECT_TRUE(vec[0].empty());
    EXPECT_TRUE(vec[1].empty());
    EXPECT_TRUE(vec[2].empty());

    vec = strings::splitToken(" a ", strings::white_space, true);
    ASSERT_TRUE(vec.size() == 3);
    EXPECT_TRUE(vec[0].empty());
    EXPECT_TRUE(vec[1] == "a");
    EXPECT_TRUE(vec[2].empty());

    vec = strings::splitToken("a b\tc\nd\r\re ");
    ASSERT_TRUE(vec.size() == 5);
    EXPECT_TRUE(vec[0] == "a");
    EXPECT_TRUE(vec[1] == "b");
    EXPECT_TRUE(vec[2] == "c");
    EXPECT_TRUE(vec[3] == "d");
    EXPECT_TRUE(vec[4] == "e");

    vec = strings::splitToken("a,b;c", ",;");
    ASSERT_TRUE(vec.size() == 3);
    EXPECT_TRUE(vec[0] == "a");
    EXPECT_TRUE(vec[1] == "b");
    EXPECT_TRUE(vec[2] == "c");
}

TEST(string_test, TestStringSplit)
{
    // split
    std::vector<std::string> vec;

    vec = strings::split("abc", "");
    ASSERT_TRUE(vec.size() == 0);

    vec = strings::split("  ", " ", true);
    ASSERT_TRUE(vec.size() == 3);
    EXPECT_TRUE(vec[0].empty());
    EXPECT_TRUE(vec[1].empty());
    EXPECT_TRUE(vec[2].empty());

    vec = strings::split("abcd", "ab", true);
    ASSERT_TRUE(vec.size() == 2);
    EXPECT_TRUE(vec[0].empty());
    EXPECT_TRUE(vec[1] == "cd");

    vec = strings::split("abcd", "ab");
    ASSERT_TRUE(vec.size() == 1);
    EXPECT_TRUE(vec[0] == "cd");

    vec = strings::split("a b\tc\nd\r\re ", "\n");
    ASSERT_TRUE(vec.size() == 2);
    EXPECT_TRUE(vec[0] == "a b\tc");
    EXPECT_TRUE(vec[1] == "d\r\re ");

    vec = strings::split("a,b;c", ",;");
    ASSERT_TRUE(vec.size() == 1);
    EXPECT_TRUE(vec[0] == "a,b;c");
}

TEST(string_test, TestStringTrim)
{
    // trim
    std::string s = " \t\r\na b c \r\r\r";
    EXPECT_EQ(strings::trim(s), "a b c");

    s = " \t\r\na b c";
    EXPECT_EQ(strings::trim(s), "a b c");

    s = "a b c \t\r\n";
    EXPECT_EQ(strings::trim(s), "a b c");

    s = "";
    EXPECT_EQ(strings::trim(s), "");

    s = " \t\r\n";
    EXPECT_EQ(strings::trim(s), "");
}

TEST(string_test, TestIsEmpty)
{
    EXPECT_TRUE(strings::isEmpty(nullptr));
    EXPECT_TRUE(strings::isEmpty(""));
    EXPECT_FALSE(strings::isEmpty(" "));
    EXPECT_FALSE(strings::isEmpty("a"));

    EXPECT_TRUE(strings::isNotEmpty(" "));
    EXPECT_TRUE(strings::isNotEmpty("a"));
    EXPECT_FALSE(strings::isNotEmpty(nullptr));
    EXPECT_FALSE(strings::isNotEmpty(""));
}

TEST(string_test, TestStringReplace)
{
    std::string str = "aaabacdaab";
    EXPECT_EQ(strings::replace(str, "aa", "x"), "xabacdxb");
    EXPECT_EQ(strings::replace(str, "x", "xx"), "xxabacdxxb");
    EXPECT_EQ(strings::replace(str, "", "z"), "xxabacdxxb");
    EXPECT_EQ(strings::replace(str, "a", ""), "xxbcdxxb");
    str = "";
    EXPECT_EQ(strings::replace(str, "a", "b"), "");
}

TEST(string_test, TestStringJoin)
{
    std::vector<std::string> strings;
    EXPECT_EQ(a_util::strings::join(strings, ","), "");

    strings.push_back("a");
    strings.push_back("b");
    strings.push_back("");
    strings.push_back("d");
    EXPECT_EQ(a_util::strings::join(strings, ","), "a,b,,d");
    EXPECT_EQ(a_util::strings::join(strings, ""), "abd");
}

TEST(string_test, TestStringCopy)
{
    std::string str = "abcd";
    memory::MemoryBuffer buf(10);
    char* ptr = (char*)buf.getPtr();

    memory::set(buf, 42, 10);
    EXPECT_EQ(strings::copy(ptr, buf.getSize(), str.c_str()), str.size());
    EXPECT_EQ(ptr[0], 'a');
    EXPECT_EQ(ptr[1], 'b');
    EXPECT_EQ(ptr[2], 'c');
    EXPECT_EQ(ptr[3], 'd');
    EXPECT_EQ(ptr[4], '\0');

    EXPECT_EQ(strings::copy(ptr, 3, str.c_str()), 2);
    EXPECT_EQ(ptr[0], 'a');
    EXPECT_EQ(ptr[1], 'b');
    EXPECT_EQ(ptr[2], '\0');

    EXPECT_THROW(strings::copy(nullptr, 1, str.c_str()), std::invalid_argument);
    EXPECT_THROW(strings::copy(ptr, 1, nullptr), std::invalid_argument);
    EXPECT_THROW(strings::copy(nullptr, 1, nullptr), std::invalid_argument);

    memory::set(buf, 42, 10);
    EXPECT_EQ(strings::copy(ptr, buf.getSize(), str.c_str(), 2), 2);
    EXPECT_EQ(ptr[0], 'a');
    EXPECT_EQ(ptr[1], 'b');
    EXPECT_EQ(ptr[2], '\0');

    EXPECT_EQ(strings::copy(ptr, 3, str.c_str(), 4), 2);
    EXPECT_EQ(ptr[0], 'a');
    EXPECT_EQ(ptr[1], 'b');
    EXPECT_EQ(ptr[2], '\0');

    EXPECT_THROW(strings::copy(nullptr, 1, str.c_str(), 1), std::invalid_argument);
    EXPECT_THROW(strings::copy(ptr, 1, nullptr, 1), std::invalid_argument);
    EXPECT_THROW(strings::copy(nullptr, 1, nullptr, 1), std::invalid_argument);
}
