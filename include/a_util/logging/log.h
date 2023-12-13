/**
 * @file
 * Public API for @ref a_util::logging::Logger "Logger" type, functions and macros
 *
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */

#ifndef A_UTIL_UTIL_LOG_HEADER_INCLUDED
#define A_UTIL_UTIL_LOG_HEADER_INCLUDED

#include <a_util/base/delegate.h>
#include <a_util/base/types.h>
#include <a_util/preprocessor/to_string.h> // A_UTIL_TO_STRING()
#include <a_util/strings/strings_format.h>
#include <a_util/strings/strings_functions.h>

namespace a_util {
namespace logging {
/**
 * Usable log levels for LogEntry::log_level
 * @details Severity ordered from high to low.
 */
enum LogLevel {
    None = 0,     //!< Log nothing
    Error = 10,   //!< Log errors only
    Warning = 20, //!< Log errors and warnings
    Info = 30,    //!< Log errors, warnings and info messages
    Dump = 40,    //!< Log errors, warnings, info and debug messages
    Debug = Dump, //!< Same as Dump
    All = 0xFF    //!< Log all messages
};

/// A log entry
struct LogEntry {
    timestamp_t time_stamp; ///< time stamp of the log message.
    std::uint8_t log_level; ///< the log level, see LogLevel.
    std::string message;    ///< the message text
    std::string source;     ///< the origin of the entry
};

/**
 * Adds a new log entry to the current logger.
 * @param[in] entry The new log entry.
 */
void addEntry(const LogEntry& entry);

/**
 * Adds a new log entry to the current logger.
 * Automatically timestamps the entry.
 * @param[in] log_level The severity, @ref LogLevel.
 * @param[in] message The message text (optional)
 * @param[in] source A string describing the source location (optional).
 */
void addEntry(std::uint8_t log_level,
              const std::string& message = a_util::strings::empty_string,
              const std::string& source = a_util::strings::empty_string);

/**
 * Logger interface definition
 */
typedef experimental::UnaryDelegate<void, const LogEntry&> Logger;

/**
 * Sets the currently used logger.
 * @param[in] log The new logger method.
 */
void setLogger(Logger log);

/**
 * Get the currently used logger.
 * @return The currently used logger.
 */
Logger getLogger();

/**
 * Get the default string representation of a log entry.
 * @param[in] entry The log entry.
 * @return the string representation of the log entry.
 */
std::string defaultFormat(const LogEntry& entry);

/**
 * Default logging method, that writes log messages to stdout.
 * @param[in] entry The log entry.
 */
void defaultLogger(const LogEntry& entry);

/**
 * Add a log entry to the current logger, including current filename and line number
 * @param[in] __level The @ref a_util::logging::LogLevel "log level"
 * @param[in] ... Arguments forwarded to printf()-like formatting
 */
#define LOG_ADD_ENTRY(__level, ...)                                                                \
    a_util::logging::addEntry(__level,                                                             \
                              a_util::strings::format(__VA_ARGS__),                                \
                              __FILE__ "(" A_UTIL_TO_STRING(__LINE__) ")")

/**
 * @def LOG_DUMP
 * Log a message with @ref a_util::logging::LogLevel "LogLevel" @ref a_util::logging::Dump "Dump"
 * (only in Debug mode)
 */
#ifdef _DEBUG
#define LOG_DUMP(...) LOG_ADD_ENTRY(a_util::logging::Dump, __VA_ARGS__)
#else
#define LOG_DUMP(...)
#endif

/// Log a message with @ref a_util::logging::LogLevel "LogLevel" @ref a_util::logging::Info "Info"
#define LOG_INFO(...) LOG_ADD_ENTRY(a_util::logging::Info, __VA_ARGS__)

/**
 * Log a message with @ref a_util::logging::LogLevel "LogLevel" @ref a_util::logging::Warning
 * "Warning"
 */
#define LOG_WARNING(...) LOG_ADD_ENTRY(a_util::logging::Warning, __VA_ARGS__)

/**
 * Log a message with @ref a_util::logging::LogLevel "LogLevel" @ref a_util::logging::Error
 * "Error"
 */
#define LOG_ERROR(...) LOG_ADD_ENTRY(a_util::logging::Error, __VA_ARGS__)

} // namespace logging
} // namespace a_util

#endif // A_UTIL_UTIL_LOG_HEADER_INCLUDED
