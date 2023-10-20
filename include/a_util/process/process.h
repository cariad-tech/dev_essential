/**
 * @file
 * Public API for @ref a_util::process "process" functions
 *
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */

#ifndef A_UTIL_UTIL_PROCESS_PROCESS_HEADER_INCLUDED
#define A_UTIL_UTIL_PROCESS_PROCESS_HEADER_INCLUDED

#include <a_util/base/types.h>
#include <a_util/filesystem/path.h>

#include <string>

namespace a_util {
namespace process {
/**
 * Get the current process id.
 * @return Id of the current process
 */
uint64_t getCurrentProcessId();

/**
 * Run an executable as detached process and optionally wait for the application to exit.
 *
 * @note The new detached process will use the environment block af the calling process.
 * @param[in] executable_file Path to executable or command to execute.
 * @param[in] arguments Arguments passing to the executable.
 * @param[in] working_dir Working directory. If empty the current working directory is used.
 * @param[in] wait_for_exit Specifies whether the function should wait for the application to close
 *                          down. only if set to true the return value will be used for
 *                          applications return value.
 * @return If the wait_for_exit is set to bool exit code of the application only, only -1 if
 * starting failed
 */
uint32_t execute(const a_util::filesystem::Path& executable_file,
                 const std::string& arguments,
                 const a_util::filesystem::Path& working_dir,
                 bool wait_for_exit);

/**
 * Run an executable and wait for the application to exit.
 *
 * @note The new process will use the environment block af the calling process.
 * @param[in] executable_file Path to executable or command to execute.
 * @param[in] arguments Arguments passing to the executable.
 * @param[in] working_dir Working directory. If empty the current working directory is used.
 * @param[in] std_out Standard out of the executed application
 * @return exit code of the application.
 */
uint32_t execute(const a_util::filesystem::Path& executable_file,
                 const std::string& arguments,
                 const a_util::filesystem::Path& working_dir,
                 std::string& std_out);

/**
 * Start an executable and wait for the application to exit.
 *
 * @note The new process will use the environment block of the calling process.
 * @param[in] executable_file executable file or command to call.
 * @param[in] arguments Arguments passing to the executable.
 * @param[in] working_dir Working directory. If empty the current working directory is used.
 * @param[in] std_out Standard out of the program started.
 * @param[in] std_err Standard error of the program started.
 * @return exit code of the application.
 */
uint32_t execute(const a_util::filesystem::Path& executable_file,
                 const std::string& arguments,
                 const a_util::filesystem::Path& working_dir,
                 std::string& std_out,
                 std::string& std_err);

/**
 * Retrieve the current value of the environment variable given in @c environment_variable_name.
 *
 * @note The retrieved environment variable is only valid within the current process.
 * @param[in] environment_variable_name The environment variable name to retrieve the value for.
 * @return Value of the environment variable given in @c environment_variable_name.
 * @throw std::system_error If the environment variable could not be retrieved.
 * @throw std::runtime_error If the environment variable could not be retrieved.
 */
std::string getEnvVar(const std::string& environment_variable_name);

/**
 * Retrieve the current value of the environment variable given in @c environment_variable_name.
 *
 * If the environment variable is not set the @c default_value is returned.
 * @note The environment variable retrieved is only valid within the current process.
 * @param[in] environment_variable_name The environment variable name to retrieve the value for.
 * @param[in] default_value The default value if variable is not set.
 * @return Value of environment variable given in @c environment_variable_name or @c default_value
 */
std::string getEnvVar(const std::string& environment_variable_name,
                      const std::string& default_value);

/**
 * Set an environment variable with a given value to the current process environment areas.
 *
 * Depending on the underlying process API the environment will be either copied to any child
 * process or not. When using any of the @ref execute() functions, the new process uses a copy
 * of the environment of the calling process.
 * @note The environment variable is only valid within the current process.
 * @param[in] environment_variable_name the environment variable name to set the value for.
 * @param[in] environment_variable_value the environment variable value.
 * @throw std::system_error If environment variable could not be set.
 * @throw std::runtime_error If environment variable could not be set.
 */
void setEnvVar(const std::string& environment_variable_name,
               const std::string& environment_variable_value);

} // namespace process
} // namespace a_util

/** @cond INTERNAL_DOCUMENTATION */
#include <a_util/process/detail/process_impl.h>
/** @endcond */

#endif // A_UTIL_UTIL_PROCESS_PROCESS_HEADER_INCLUDED
