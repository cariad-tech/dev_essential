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

#ifndef WIN32

#if defined(__QNX__) && !defined(_QNX_SOURCE)
#define _QNX_SOURCE
#endif

#ifdef __QNX__
#include <cstdio>
#include <stdexcept>
#endif

#include <malloc.h>
#include <pwd.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <system_error>
#include <unistd.h>

#ifndef __QNX__
#include <features.h>
#include <sys/syscall.h>
#include <sys/sysinfo.h>
#endif

#include <a_util/filesystem/filesystem.h>
#include <a_util/process.h>
#include <a_util/strings/strings_format.h>
#include <a_util/system/system.h>

namespace a_util {
namespace process {
namespace detail {
uint32_t execute(const a_util::filesystem::Path& executable_file,
                 const std::string& arguments,
                 const a_util::filesystem::Path& working_path,
                 std::string& std_out,
                 const a_util::filesystem::Path& temp_std_err_file)
{
    a_util::filesystem::Path exe_path = executable_file;
    a_util::filesystem::Path work_path = working_path;

    detail::preparePaths(exe_path, work_path);

    std::string err_forward_arg;
    if (!temp_std_err_file.isEmpty()) {
        err_forward_arg = " 2>";
        err_forward_arg += temp_std_err_file.toString();
    }
    FILE* fd_pipe = nullptr;

    std::string call_command = a_util::strings::format("cd \"%s\" && %s %s%s",
                                                       work_path.toString().c_str(),
                                                       exe_path.toString().c_str(),
                                                       arguments.c_str(),
                                                       err_forward_arg.c_str());
    fd_pipe = popen(call_command.c_str(), "r");

    if (!fd_pipe) {
        return 1;
    }

    char buf[1024];
    while (fgets(buf, 1024, fd_pipe) != nullptr) {
        std_out.append(buf);
    }

    uint32_t exit_status = pclose(fd_pipe);
    // convert the 8 bit return value into our 32 bit return value
    // (also check for negative return values!)
    exit_status = (int32_t)WEXITSTATUS(exit_status);
    if (exit_status & 0x80) {
        exit_status |= 0xFFFFFF00;
    }
    return exit_status;
}

uint32_t execute(const a_util::filesystem::Path& executable_file,
                 const std::string& arguments,
                 const a_util::filesystem::Path& working_path,
                 std::string& std_out,
                 std::string& std_err)
{
    uint64_t proc_id = getCurrentProcessId();
    a_util::filesystem::Path temp_error_file = filesystem::getTempDirectory();

    timestamp_t easymillisec = a_util::system::getCurrentMilliseconds();
    temp_error_file.append(
        a_util::strings::format("a_util_ex_stderr_%uld_%ld", proc_id, easymillisec));

    uint32_t ret_val = execute(executable_file, arguments, working_path, std_out, temp_error_file);

    a_util::filesystem::readTextFile(temp_error_file, std_err);
    a_util::filesystem::remove(temp_error_file);
    return ret_val;
}
} // namespace detail

uint64_t getCurrentProcessId()
{
    return static_cast<uint64_t>(getpid());
}

uint32_t execute(const a_util::filesystem::Path& executable_file,
                 const std::string& arguments,
                 const a_util::filesystem::Path& working_path,
                 bool wait_for_exit)
{
    a_util::filesystem::Path exe_path = executable_file;
    a_util::filesystem::Path work_path = working_path;

    detail::preparePaths(exe_path, work_path);
    int32_t exit_status = 0;

    if (!wait_for_exit) {
#ifndef __QNX__
        std::string call_command = a_util::strings::format(
            "cd \"%s\" && which %s", work_path.toString().c_str(), exe_path.toString().c_str());
#else
        // There is no 'which' command available in our case, but we have a Bourne shell...
        // Actually, this is not really an QNX issue.
        std::string call_command = a_util::strings::format("cd \"%s\" && command -v %s",
                                                           work_path.toString().c_str(),
                                                           exe_path.toString().c_str());
#endif

        exit_status = ::system(call_command.c_str());
        if (static_cast<int32_t>((WEXITSTATUS(exit_status))) != 0) {
            return 1;
        }
    }

    exit_status = ::system(a_util::strings::format("cd \"%s\" && %s %s %s",
                                                   work_path.toString().c_str(),
                                                   exe_path.toString().c_str(),
                                                   arguments.c_str(),
                                                   wait_for_exit ? "" : "&")
                               .c_str());

    // convert the 8 bit return value into our 32 bit return value
    // (also check for negative return values!)
    exit_status = static_cast<int32_t>((WEXITSTATUS(exit_status)));
    if (exit_status & 0x80) {
        exit_status |= 0xFFFFFF00;
    }

    return exit_status;
}

std::string getEnvVar(const std::string& environment_variable_name)
{
    char* value = ::getenv(environment_variable_name.c_str());
    if (value) {
        return std::string(value);
    }
    else {
#ifdef __GNU_PREREQ
#if __GNUC_PREREQ(4, 8)
        throw std::system_error(
            std::error_code(system::getLastSystemError(), std::system_category()));
#else
        throw std::runtime_error(system::formatSystemError(system::getLastSystemError()));
#endif // __GNUC_PREREQ (4,6)
#else
        throw std::runtime_error(system::formatSystemError(system::getLastSystemError()));
#endif
    }
}

void setEnvVar(const std::string& environment_variable_name,
               const std::string& environment_variable_value)
{
    if (setenv(environment_variable_name.c_str(), environment_variable_value.c_str(), 1) != 0) {
#ifdef __GNU_PREREQ
#if __GNUC_PREREQ(4, 8)
        throw std::system_error(
            std::error_code(system::getLastSystemError(), std::system_category()));
#else
        throw std::runtime_error(system::formatSystemError(system::getLastSystemError()));
#endif // __GNUC_PREREQ (4,6)
#else
        throw std::runtime_error(system::formatSystemError(system::getLastSystemError()));
#endif
    }
    else {
        return;
    }
}

} // namespace process
} // namespace a_util

#endif
