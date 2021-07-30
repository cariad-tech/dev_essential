/**
 * @file
 * Public API for @ref a_util::system "system" classes and functions
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

#ifndef A_UTIL_UTIL_SYSTEM_SYSTEM_HEADER_INCLUDED
#define A_UTIL_UTIL_SYSTEM_SYSTEM_HEADER_INCLUDED

#include "a_util/base/types.h"      //timestamp_t
#include "a_util/filesystem/path.h" //Path

#include <string>

namespace a_util {
namespace system {
/**
 * Get the current user name
 * @return The current name of the user
 */
std::string getCurrentUserName();

/**
 * Get the current hostname
 * @return The hostname
 */
std::string getHostname();

/**
 * Get full path to the executable that started the calling system process
 * @return Absolute path and including to the executable
 */
a_util::filesystem::Path getExecutablePath();

/**
 * Get the last system error (platform specific, errno or GetLastError)
 * @return The last system error code
 */
int getLastSystemError();

/// Reset last system error to its initial value
void resetLastSystemError();

/**
 * Get the description message associated with the given system error code
 * @param[in] system_error_code The error code to get the description for.
 * @return Description associated with the system error code
 */
std::string formatSystemError(int system_error_code);

/**
 * Yield calling thread to the operating system, scheduled to awake after a period of milliseconds.
 * @param[in] ms Duration to yield the thread in milliseconds
 */
void sleepMilliseconds(std::uint32_t ms);

/**
 * Yields calling thread to the operating system, scheduled to awake after a period of microseconds.
 * @note To achieve the high resolution, use @ref HighResSchedulingSupport
 * @param[in] us Duration to yield the thread in microseconds
 */
void sleepMicroseconds(std::uint64_t us);

/**
 * Get the current milliseconds passed since system start
 * @return System running time in milliseconds
 */
timestamp_t getCurrentMilliseconds();

/**
 * Get the current microseconds passed since the first invocation (or -1 if not available)
 * @note This method requires high-resolution timer support in the OS
 * @return System running time in microseconds
 */
timestamp_t getCurrentMicroseconds();

/**
 * RAII class for high resolution scheduling support from the OS.
 * @details Requests high resolution scheduling support from the OS on instantiation, drops
 *          it on destruction.
 */
class HighResSchedulingSupport {
public:
    /// CTOR - requests support from OS
    HighResSchedulingSupport();

    /// DTOR - relinquishes support from the OS
    ~HighResSchedulingSupport();

    /**
     * Check whether high resolution scheduling is supported by the OS
     * @return @c true if high resolution scheduling is supported, @c false otherwise.
     */
    bool isSupported() const;

private:
    HighResSchedulingSupport(const HighResSchedulingSupport&);            // = delete;
    HighResSchedulingSupport& operator=(const HighResSchedulingSupport&); // = delete;

private:
    unsigned int _handle;
};

} // namespace system
} // namespace a_util

#endif // A_UTIL_UTIL_SYSTEM_SYSTEM_HEADER_INCLUDED
