/**
 * @file
 * Unicode test implementation
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

#include <a_util/strings/unicode.h>

#include <gtest/gtest.h>

using namespace a_util;

TEST(unicode_test, TestUTFConversion)
{
    unsigned char utf8_test[] = {0x61, 0xE4, 0xB8, 0xAD, 0xD0, 0xAF};
    wchar_t utf16_test[] = {0x0061, 0x4e2d, 0x042f};
    std::string utf8(reinterpret_cast<const char*>(&utf8_test[0]),
                     sizeof(utf8_test) / sizeof(utf8_test[0]));
    std::wstring utf16(&utf16_test[0], sizeof(utf16_test) / sizeof(utf16_test[0]));

    // validity
    ASSERT_NO_THROW(strings::unicode::utf8ToUtf16(utf8));
    ASSERT_NO_THROW(strings::unicode::utf16ToUtf8(utf16));

    // roundtrip
    std::string utf8_conv = strings::unicode::utf16ToUtf8(strings::unicode::utf8ToUtf16(utf8));
    ASSERT_EQ(utf8_conv, utf8);
    std::wstring utf16_conv = strings::unicode::utf8ToUtf16(strings::unicode::utf16ToUtf8(utf16));
    ASSERT_EQ(utf16_conv, utf16);

    // equality
    ASSERT_EQ(strings::unicode::utf8ToUtf16(utf8), utf16);
    ASSERT_EQ(strings::unicode::utf16ToUtf8(utf16), utf8);
}
