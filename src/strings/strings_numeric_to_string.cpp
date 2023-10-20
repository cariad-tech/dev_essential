/**
 * @file
 * Numeric to string conversions
 *
 * Copyright @ 2021 VW Group. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "strings_format_detail.h"

#include <string>

namespace a_util {
namespace strings {
std::string toString(std::int8_t from)
{
    return detail::signedNumericToString(from);
}

std::string toString(std::int16_t from)
{
    return detail::signedNumericToString(from);
}

std::string toString(std::int32_t from)
{
    return detail::signedNumericToString(from);
}

std::string toString(std::int64_t from)
{
    return detail::signedNumericToString(from);
}

std::string toString(std::uint8_t from)
{
    return detail::unsignedNumericToString(from);
}

std::string toString(std::uint16_t from)
{
    return detail::unsignedNumericToString(from);
}

std::string toString(std::uint32_t from)
{
    return detail::unsignedNumericToString(from);
}

std::string toString(std::uint64_t from)
{
    return detail::unsignedNumericToString(from);
}

std::string toString(float from)
{
    return detail::signedNumericToString(from);
}

std::string toString(double from)
{
    return detail::signedNumericToString(from);
}

std::string toString(bool from)
{
    return from ? std::string("true") : std::string("false");
}

} // namespace strings
} // namespace a_util
