/**
 * @file
 * Utility for string checkking within the DDFromXML header only factory
 *
 * @copyright
 * @verbatim
Copyright @ 2022 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */

#ifndef DD_STRING_CHECK_H_INCLUDED
#define DD_STRING_CHECK_H_INCLUDED

#include <string>

namespace ddl {
namespace dd {
namespace utility {

/**
 * @brief Checks if the given string is an integer value.
 * @param[in] string_to_check the string to check
 * @return true/false
 */
bool isInteger(const std::string& string_to_check);

} // namespace utility
} // namespace dd
} // namespace ddl

#endif // DD_STRING_CHECK_H_INCLUDED
