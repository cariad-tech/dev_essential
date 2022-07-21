/**
 * @file
 * Implementation for the @c Result class
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

#ifdef A_UTIL_ENABLE_DYNAMIC_ERROR_DESCRIPTION
#include <a_util/result/impl/error_description.h>
#else
#include <a_util/result/impl/reference_counted_object_disable_dynamic_alloc.h>
#endif

#include <a_util/base/static_assert_fallback.h>
#include <a_util/result/detail/error_description_intf.h>
#include <a_util/result/impl/result_description_private.h>
#include <a_util/result/result_type.h>

namespace a_util {
namespace result {

typedef detail::ErrorDescriptionType ErrorDescriptionType;

Result::Result(std::int32_t error_code,
               const char* error_description,
               std::int32_t line,
               const char* file,
               const char* function,
               const char* label)
#if A_UTIL_ENABLE_COMPILER_CXX_VARIADIC_TEMPLATES
    : _result_handle(DescriptionType::makeResultDescription<ErrorDescriptionType>(
          error_code, error_description, line, file, function, label))
#else
    // function chaining with make_variadic()
    : _result_handle(DescriptionType::makeResultDescription<ErrorDescriptionType>(
          error_code, base::make_variadic(error_description)(line)(file)(function)(label)))
#endif //#ifdef A_UTIL_ENABLE_COMPILER_CXX_VARIADIC_TEMPLATES
{
}

Result::Result(const Result& other) : _result_handle(other._result_handle)
{
}

Result::~Result()
{
    static_assert(sizeof(*this) == 8, "sizeof(*this) != 8");
    // static_assert(std::is_standard_layout<Result>::value, "Result must be standard layout type");
}

std::int32_t Result::getErrorCode() const
{ // get error code from the user space if available
    if (_result_handle.getDetailedDescription()) {
        return _result_handle.getDetailedDescription()->getErrorCode();
    }
    return _result_handle.getErrorCode();
}

const char* Result::getErrorLabel() const
{
    if (_result_handle.getDetailedDescription()) { // we can get the stringified error code from the
                                                   // detailed description
        return _result_handle.getDetailedDescription()->getErrorCodeLabel();
    }

    return isOk(*this) ? "SUCCESS" : "(unknown)";
}

const char* Result::getDescription() const
{
    if (0 !=
        _result_handle.getErrorCode()) { // only the error code was set, no description available
        return "";
    }

    if (_result_handle.getDetailedDescription()) {
        return _result_handle.getDetailedDescription()->getErrorDescription();
    }

    return "No error occurred";
}

std::int32_t Result::getLine() const
{
    if (_result_handle.getDetailedDescription()) {
        return _result_handle.getDetailedDescription()->getLine();
    }

    return -1;
}

const char* Result::getFile() const
{
    if (_result_handle.getDetailedDescription()) {
        return _result_handle.getDetailedDescription()->getFileName();
    }

    return "";
}

const char* Result::getFunction() const
{
    if (_result_handle.getDetailedDescription()) {
        return _result_handle.getDetailedDescription()->getFunctionName();
    }

    return "";
}

Result Result::CreateFrom(std::int32_t error_code,
                          const char* error_description,
                          std::int32_t line,
                          const char* file,
                          const char* function)
{
    return Result(error_code,
                  error_description,
                  line,
                  file,
                  function,
                  0 == error_code ? "SUCCESS" : "(unknown)");
}

} // namespace result
} // namespace a_util
