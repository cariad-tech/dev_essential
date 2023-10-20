/**
 * @file
 * Unicode function
 *
 * Copyright @ 2021 VW Group. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <a_util/strings/unicode.h>

#include <3rdparty/utfcpp/utf8_v2_3_4/source/utf8.h>

#include <stdexcept>

std::wstring a_util::strings::unicode::utf8ToUtf16(const std::string& utf8_str)
{
    std::wstring utf16_str;

    try {
        utf8::utf8to16(utf8_str.begin(), utf8_str.end(), std::back_inserter(utf16_str));
    }
    catch (utf8::invalid_utf8&) {
        throw std::invalid_argument("utf8_str");
    }

    return utf16_str;
}

std::string a_util::strings::unicode::utf16ToUtf8(const std::wstring& utf16_str)
{
    std::string utf8_str;

    try {
        utf8::utf16to8(utf16_str.begin(), utf16_str.end(), std::back_inserter(utf8_str));
    }
    catch (utf8::invalid_utf16&) {
        throw std::invalid_argument("utf16_str");
    }

    return utf8_str;
}
