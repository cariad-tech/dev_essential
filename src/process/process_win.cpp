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

#include "a_util/filesystem/filesystem.h"
#include "a_util/process.h"
#include "a_util/strings/strings_format.h"
#include "a_util/system/system.h"

#ifdef WIN32

#ifndef NOMINMAX
#define UNDEF_NOMINMAX
#define NOMINMAX
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define UNDEF_WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>

#ifdef UNDEF_NOMINMAX
#undef NOMINMAX
#undef UNDEF_NOMINMAX
#endif

#ifdef UNDEF_WIN32_LEAN_AND_MEAN
#undef WIN32_LEAN_AND_MEAN
#undef UNDEF_WIN32_LEAN_AND_MEAN
#endif

#include <io.h>
#include <system_error>

namespace a_util {
namespace process {
#define popen _popen
#define pclose _pclose

namespace detail {
uint32_t execute(const a_util::filesystem::Path& executable_file,
                 const std::string& arguments,
                 const a_util::filesystem::Path& working_path,
                 std::string& std_out,
                 std::string& std_err)
{
    a_util::filesystem::Path exe_path = executable_file;
    a_util::filesystem::Path work_path = working_path;

    detail::preparePaths(exe_path, work_path);

    std::string call_command =
        a_util::strings::format("%s %s", exe_path.toString().c_str(), arguments.c_str());

    // if we use popen() on windows systems, a console opens and closes immediately if none is
    // already present (e.g. start an app from GUI) to avoid this we generate a whole new process
    // and connect via "real" pipe see https://msdn.microsoft.com/en-us/library/ms682499.aspx for
    // details

    SECURITY_ATTRIBUTES sec_attributes;
    sec_attributes.nLength = sizeof(SECURITY_ATTRIBUTES);
    sec_attributes.bInheritHandle = TRUE;
    sec_attributes.lpSecurityDescriptor = nullptr;

    HANDLE childStd_OUT_Rd = nullptr;
    HANDLE childStd_OUT_Wr = nullptr;
    CreatePipe(&childStd_OUT_Rd, &childStd_OUT_Wr, &sec_attributes, 0);

    HANDLE childStd_ERR_Rd = nullptr;
    HANDLE childStd_ERR_Wr = nullptr;
    CreatePipe(&childStd_ERR_Rd, &childStd_ERR_Wr, &sec_attributes, 0);

    // connect pipe to file stream - the most tricky part here
    FILE* fd_pipe = _fdopen(_open_osfhandle((intptr_t)childStd_OUT_Rd, 0), "r");
    FILE* fd_pipe_err = _fdopen(_open_osfhandle((intptr_t)childStd_ERR_Rd, 0), "r");

    PROCESS_INFORMATION proc_info;
    memset(&proc_info, 0, sizeof(PROCESS_INFORMATION));

    STARTUPINFO startup_info;
    memset(&startup_info, 0, sizeof(STARTUPINFO));
    startup_info.cb = sizeof(STARTUPINFO);
    startup_info.dwFlags |= STARTF_USESTDHANDLES;
    startup_info.hStdOutput = childStd_OUT_Wr;
    startup_info.hStdError = childStd_ERR_Wr;

    BOOL bSuccess = CreateProcess(nullptr,
                                  (LPTSTR)(LPCTSTR)call_command.c_str(),
                                  nullptr,
                                  nullptr,
                                  TRUE,
                                  0,
                                  nullptr,
                                  work_path.toString().c_str(),
                                  &startup_info,
                                  &proc_info);

    CloseHandle(childStd_OUT_Wr);
    CloseHandle(childStd_ERR_Wr);

    if (!bSuccess) {
        CloseHandle(proc_info.hProcess);
        CloseHandle(proc_info.hThread);
        return (std::uint32_t)-1;
    }

    if (fd_pipe) {
        char buf[1024];
        while (fgets(buf, 1024, fd_pipe) != nullptr) {
            std_out.append(buf);
        }
    }
    if (fd_pipe_err) {
        char buf[1024];
        while (fgets(buf, 1024, fd_pipe_err) != nullptr) {
            std_err.append(buf);
        }
    }

    WaitForSingleObject(proc_info.hProcess, INFINITE);
    fclose(fd_pipe);
    fclose(fd_pipe_err);

    DWORD exit_status = 0;
    GetExitCodeProcess(proc_info.hProcess, &exit_status);

    CloseHandle(proc_info.hProcess);
    CloseHandle(proc_info.hThread);

    return exit_status;
}
} // namespace detail

uint64_t getCurrentProcessId()
{
    return static_cast<uint64_t>(::GetCurrentProcessId());
}

uint32_t execute(const a_util::filesystem::Path& executable_file,
                 const std::string& arguments,
                 const a_util::filesystem::Path& working_path,
                 bool wait_for_exit)
{
    a_util::filesystem::Path exe_path = executable_file;
    a_util::filesystem::Path work_path = working_path;

    detail::preparePaths(exe_path, work_path);

    STARTUPINFO startup_info;
    memset(&startup_info, 0, sizeof(startup_info));

    PROCESS_INFORMATION proc_info;
    memset(&proc_info, 0, sizeof(proc_info));

    std::string exe_command(exe_path);
    if (!arguments.empty()) {
        exe_command += " ";
        exe_command += arguments;
    }

    BOOL status_ret = CreateProcess(nullptr,
                                    (LPTSTR)(LPCTSTR)exe_command.c_str(),
                                    nullptr,
                                    nullptr,
                                    FALSE,
                                    0,
                                    nullptr,
                                    work_path.toString().c_str(),
                                    &startup_info,
                                    &proc_info);

    std::string lasterr = a_util::system::formatSystemError(a_util::system::getLastSystemError());

    if (status_ret == FALSE) {
        return (std::uint32_t)-1;
    }

    if (!wait_for_exit) {
        return 0;
    }

    WaitForInputIdle(proc_info.hProcess, INFINITE);
    WaitForSingleObject(proc_info.hProcess, INFINITE);

    DWORD exit_code = 0;
    GetExitCodeProcess(proc_info.hProcess, &exit_code);

    return exit_code;
}

std::string getEnvVar(const std::string& environment_variable_name)
{
    uint32_t buffer_size = ::GetEnvironmentVariable(environment_variable_name.c_str(), nullptr, 0);
    std::string return_val;
    if (buffer_size == 1) {
        // return_val is a empty string but valid
    }
    else if (buffer_size > 1) {
        return_val.resize(buffer_size); // std string in MS STL is kaputt ..
                                        // this must be initialized with the trailing \0
        uint32_t temp_buffer_size = ::GetEnvironmentVariable(
            environment_variable_name.c_str(), &return_val[0], buffer_size);
        if (temp_buffer_size == 0) {
            throw std::system_error(
                std::error_code(system::getLastSystemError(), std::system_category()));
        }
        return_val.resize(buffer_size - 1); // this will remanage the memory in return_val removes
                                            // trailing \0 and use intern one
    }
    else {
        throw std::system_error(
            std::error_code(system::getLastSystemError(), std::system_category()));
    }
    return return_val;
}

void setEnvVar(const std::string& environment_variable_name,
               const std::string& environment_variable_value)
{
    if (FALSE == ::SetEnvironmentVariable(environment_variable_name.c_str(),
                                          environment_variable_value.c_str())) {
        throw std::system_error(
            std::error_code(system::getLastSystemError(), std::system_category()));
    }
    else {
        return;
    }
}

} // namespace process
} // namespace a_util

#endif
