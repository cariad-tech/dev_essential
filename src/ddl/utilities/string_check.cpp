/**
 * @file
 * Utility for the Neutrino gcc5 compiler which has really no std::to_string implementation!
 *
 * Copyright @ 2022 VW Group. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <ddl/utilities/string_check.h>

#include <regex>

namespace ddl {
namespace dd {
namespace utility {

bool isInteger(const std::string& string_to_check)
{
    static const std::regex numeric_integer_check("^-{0,1}(\\d)+");
    return std::regex_match(string_to_check, numeric_integer_check);
}

} // namespace utility
} // namespace dd
} // namespace ddl
