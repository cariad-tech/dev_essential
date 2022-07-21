/**
 * @file
 * Logging API
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

#include <a_util/datetime/datetime.h>
#include <a_util/filesystem.h>
#include <a_util/logging/log.h>

#include <cstdio>

namespace a_util {
namespace logging {
static Logger& get_internal_logger()
{
    static Logger glob = defaultLogger;
    return glob;
}

void addEntry(const LogEntry& entry)
{
    if (get_internal_logger()) // calls operator bool() of delegate...
    {
        get_internal_logger()(entry);
    }
}

void addEntry(std::uint8_t log_level, const std::string& message, const std::string& source)
{
    LogEntry entry;
    entry.time_stamp = a_util::datetime::getCurrentLocalDateTime().toTimestamp();
    entry.log_level = log_level;
    entry.message = message;
    entry.source = source;
    addEntry(entry);
}

void setLogger(Logger oLogger)
{
    get_internal_logger() = oLogger;
}

Logger getLogger()
{
    return get_internal_logger();
}

std::string defaultFormat(const LogEntry& entry)
{
    a_util::datetime::DateTime oTime;
    oTime.set(entry.time_stamp);

    std::string log_msg = oTime.format("%Y-%m-%d %H:%M:%S ");

    if (entry.log_level <= Error) {
        log_msg += "[ERROR]";
    }
    else if (entry.log_level <= Warning) {
        log_msg += "[WARNING]";
    }
    else if (entry.log_level <= Info) {
        log_msg += "[INFO]";
    }
    else {
        log_msg += "[DUMP]";
    }

    if (!entry.message.empty()) {
        log_msg += ": ";
        log_msg += entry.message;
    }

    if (!entry.source.empty()) {
        log_msg += " [";
        log_msg += a_util::filesystem::Path(entry.source).getLastElement();
        log_msg += "]";
    }

    return log_msg;
}

void defaultLogger(const LogEntry& entry)
{
    fprintf(stdout, "%s\n", defaultFormat(entry).c_str());
}

} // namespace logging
} // namespace a_util
