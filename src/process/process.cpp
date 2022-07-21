/**
 * @file
 * Concurrency APIs
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

#include <a_util/filesystem.h>
#include <a_util/process.h>

#include <string>

namespace a_util {
namespace process {
uint32_t execute(const a_util::filesystem::Path& executable_file,
                 const std::string& arguments,
                 const a_util::filesystem::Path& working_path,
                 std::string& std_out)
{
    std::string std_err;
    return detail::execute(executable_file, arguments, working_path, std_out, std_err);
}

// execution with std error!

uint32_t execute(const a_util::filesystem::Path& executable_file,
                 const std::string& arguments,
                 const a_util::filesystem::Path& working_path,
                 std::string& std_out,
                 std::string& std_err)
{
    return detail::execute(executable_file, arguments, working_path, std_out, std_err);
}

std::string getEnvVar(const std::string& environment_variable_name,
                      const std::string& default_value)
{
    try {
        std::string ret_val = getEnvVar(environment_variable_name);
        return ret_val;
    }
    catch (const std::exception&) {
        return default_value;
    }
}

/**
 * Starts a child process
 * @param [out] pChildProcessId The process id of the child process.
 * @param [in] strExeFile The executable that should be launched.
 * @param [in] strArgs Addidional arguments.
 * @return ERR_UNEXPECTED if something goes wrong, see @ref GetLastSystemError().
 */
uint64_t childExecute(const a_util::filesystem::Path& executable_file,
                      const std::string& arguments);

/**
 * Wait until a child process exits.
 * Note that after the process has exited, this method can only be called once.
 * @param [in] nChildProcessId The process id of the child process.
 * @param [in] tmTimeOut Timeout in microseconds: -1 -> no timeout, 0 -> return immediately.
 * @return ERR_TIMEOUT when the process has not exited.
 */
uint64_t childWait(uint64_t proc_id, timestamp_t timeout = -1);

/**
 * Terminates a child process.
 * @param [in] nChildProcessId The process id of the child process.
 * @return ERR_UNKNOWN when something goes wrong, see @ref GetLastSystemError().
 */
bool childTerminate(uint64_t nChildProcessId);

} // namespace process
} // namespace a_util
