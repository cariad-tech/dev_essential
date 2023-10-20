/**
 * @file
 * Define the default error description class for @c Result return values
 *
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */

#ifndef A_UTIL_UTIL_RESULT_IMPL_ERROR_DESCRIPTION_IMPL_HEADER_INCLUDED
#define A_UTIL_UTIL_RESULT_IMPL_ERROR_DESCRIPTION_IMPL_HEADER_INCLUDED

#include <a_util/result/detail/error_description.h>

namespace a_util {
namespace result {
namespace detail {
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

std::int32_t ErrorDescription::getErrorCode() const
{
    return _error_code;
}

std::int32_t ErrorDescription::getLine() const
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

template <typename T, template <typename = T> class ResultInfo>
std::int32_t ResultInfoDescription<T, ResultInfo>::getErrorCode() const
{
    return ::a_util::result::ResultInfo<T>::getCode();
}

template <typename T, template <typename = T> class ResultInfo>
std::int32_t ResultInfoDescription<T, ResultInfo>::getLine() const
{
    return -1;
}

template <typename T, template <typename = T> class ResultInfo>
const char* ResultInfoDescription<T, ResultInfo>::getErrorDescription() const
{
    return "";
}

template <typename T, template <typename = T> class ResultInfo>
const char* ResultInfoDescription<T, ResultInfo>::getFileName() const
{
    return "";
}

template <typename T, template <typename = T> class ResultInfo>
const char* ResultInfoDescription<T, ResultInfo>::getFunctionName() const
{
    return "";
}

template <typename T, template <typename = T> class ResultInfo>
const char* ResultInfoDescription<T, ResultInfo>::getErrorCodeLabel() const
{
    return ::a_util::result::ResultInfo<T>::getLabel();
}

} // namespace detail
} // namespace result
} // namespace a_util

#endif // A_UTIL_UTIL_RESULT_IMPL_ERROR_DESCRIPTION_IMPL_HEADER_INCLUDED
