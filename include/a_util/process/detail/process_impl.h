/**
 * @file
 * Private implementation for @ref a_util::process "process" functions
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

#ifndef A_UTIL_UTIL_PROCESS_DETAIL_PROCESS_IMPL_HEADER_INCLUDED
#define A_UTIL_UTIL_PROCESS_DETAIL_PROCESS_IMPL_HEADER_INCLUDED

#include <a_util/filesystem/path.h>

namespace a_util {
namespace process {
namespace detail {
inline void preparePaths(a_util::filesystem::Path& exe_path, a_util::filesystem::Path& working_path)
{
    // add quotes if executable contains spaces
    exe_path = (std::string::npos == exe_path.toString().find(' ')) ? (exe_path) :
                                                                      ("\"" + exe_path + "\"");
    // an empty path can not be handled by CreateProcess so we set working path to current (".")
    working_path = working_path.isEmpty() ? "." : working_path;
}

uint32_t execute(const a_util::filesystem::Path& executable_file,
                 const std::string& arguments,
                 const a_util::filesystem::Path& working_path,
                 std::string& std_out,
                 std::string& std_err);

} // namespace detail
} // namespace process
} // namespace a_util

#endif // A_UTIL_UTIL_PROCESS_DETAIL_PROCESS_IMPL_HEADER_INCLUDED
