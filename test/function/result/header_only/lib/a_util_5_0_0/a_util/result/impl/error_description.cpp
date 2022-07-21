/**
 * @file
 * Define the default error description class for @c Result return values
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

#include <a_util/result/impl/error_description.h>

namespace a_util {
namespace result {
namespace detail {
ErrorDescription& ErrorDescription::operator=(const ErrorDescription&)
{
    return *this;
}

ErrorDescription::ErrorDescription(std::int32_t error_code,
                                   const char* error_description,
                                   std::int32_t line,
                                   const char* file,
                                   const char* function,
                                   const char* error_label)
    : _error_code(error_code),
      _line_number(line),
      _error_description(error_description),
      _file_name(file),
      _function_name(function),
      _error_label(error_label)
{
}

ErrorDescription::~ErrorDescription()
{
}

ErrorDescription::ErrorDescription(const ErrorDescription& other)
    : _error_code(other._error_code),
      _line_number(other._line_number),
      _error_description(other._error_description),
      _file_name(other._file_name),
      _function_name(other._function_name),
      _error_label(other._error_label)
{
}

const std::int32_t ErrorDescription::getErrorCode() const
{
    return _error_code;
}

const std::int32_t ErrorDescription::getLine() const
{
    return _line_number;
}

const char* ErrorDescription::getErrorDescription() const
{
    return _error_description.c_str();
}

const char* ErrorDescription::getFileName() const
{
    return _file_name.c_str();
}

const char* ErrorDescription::getFunctionName() const
{
    return _function_name.c_str();
}

const char* ErrorDescription::getErrorCodeLabel() const
{
    return _error_label.c_str();
}

} // namespace detail
} // namespace result
} // namespace a_util
