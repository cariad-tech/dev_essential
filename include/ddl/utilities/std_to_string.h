/**
 * @file
 * Utility for the Neutrino gcc5 compiler which has really no std::to_string implementation!
 *
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

    This Source Code Form is subject to the terms of the Mozilla
    Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

If it is not possible or desirable to put the notice in a particular file, then
You may include the notice in a location (such as a LICENSE file in a
relevant directory) where a recipient would be likely to look for such a notice.

You may add additional accurate notices of copyright ownership.
@endverbatim
 */

#ifndef DD_STD_TO_STRING_WRAP
#define DD_STD_TO_STRING_WRAP

#ifdef __QNX__
#include <sstream>
#include <stdlib.h>
#include <string>

namespace std {
/**
 * Its the truth ... there is really no to_string method on gcc5 in Neutrino compiler for QNX!
 */
template <typename T>
string to_string(T value)
{
    ostringstream convert;
    convert << value;
    return convert.str();
}

/**
 * Its the truth ... there is really no stoul method on gcc5 in Neutrino compiler for QNX!
 */
inline unsigned long stoul(const std::string& string_value)
{
    char* endptr = {};
    return strtoul(string_value.c_str(), &endptr, 10);
}

/**
 * Its the truth ... there is really no stoi method on gcc5 in Neutrino compiler for QNX!
 */
inline int stoi(const std::string& string_value)
{
    return atoi(string_value.c_str());
}

} // namespace std

#endif

#endif // DD_STD_TO_STRING_WRAP
