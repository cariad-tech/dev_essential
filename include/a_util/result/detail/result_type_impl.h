/**
 * @file
 * Private implementation for @ref a_util::result::Result "Result" type and functions
 *
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */

#ifndef A_UTIL_UTIL_RESULT_DETAIL_RESULT_TYPE_IMPL_HEADER_INCLUDED
#define A_UTIL_UTIL_RESULT_DETAIL_RESULT_TYPE_IMPL_HEADER_INCLUDED

#include <a_util/base/types.h> //a_util::maybe_unused
#include <a_util/result/detail/error_description.h>
#include <a_util/result/detail/result_description_decl.h>
#include <a_util/result/detail/result_description_impl.h>
#include <a_util/result/result_info.h>
#include <a_util/result/result_type_decl.h>

#include <utility> //std::swap

namespace a_util {
namespace result {

inline Result::Result() : _result_handle()
{
}

inline Result::Result(bool val)
    : _result_handle(DescriptionType::makeResultDescription(static_cast<std::int32_t>(!val) * -1))
{
}

inline Result::Result(const ResultInfo<void>&) : _result_handle()
{
}

template <typename ErrorType>
inline Result::Result(const ResultInfo<ErrorType>& error)
    : _result_handle(DescriptionType::makeResultDescription(error))
{
    a_util::maybe_unused(error); // C4100
}

template <typename ErrorType>
inline Result::Result(ResultInfo<ErrorType> error,
                      const char* error_description,
                      std::int32_t line,
                      const char* file,
                      const char* function)
    : _result_handle()
{
    a_util::maybe_unused(error); // C4100
    *this = Result(error.getCode(),
                   error_description,
                   line,
                   file,
                   function,
                   ResultInfo<ErrorType>::getLabel());
}

inline Result::Result(std::int32_t error_code)
    : _result_handle(DescriptionType::makeResultDescription(error_code))
{
}

inline Result::Result(std::int32_t error_code,
                      const char* error_description,
                      std::int32_t line,
                      const char* file,
                      const char* function)
    : _result_handle()
{
    *this = createFrom(error_code, error_description, line, file, function);
}

inline Result::Result(const Result& other_result,
                      const char* error_description,
                      std::int32_t line,
                      const char* file,
                      const char* function)
    : _result_handle()
{
    *this = Result(other_result.getErrorCode(),
                   error_description,
                   line,
                   file,
                   function,
                   other_result.getErrorLabel());
}

inline Result::~Result()
{
    static_assert(sizeof(*this) == 8, "sizeof(*this) != 8");
    static_assert(std::is_standard_layout<Result>::value, "Result must be standard layout type");
}

inline Result::Result(const Result& other) : _result_handle(other._result_handle)
{
}

inline Result& Result::operator=(Result other)
{
    swap(*this, other);
    return *this;
}

inline Result& Result::operator=(bool val)
{
    return Result::operator=(Result(val));
}

inline Result& Result::operator=(std::int32_t val)
{
    return Result::operator=(Result(val));
}

inline Result& Result::operator=(const ResultInfo<void>&)
{
    return Result::operator=(Result());
}

inline Result::Result(Result&& other) : _result_handle()
{
    swap(*this, other);
    if (SUCCESS != other) {
        other = Result();
    }
}

inline std::int32_t Result::getErrorCode() const
{ // get error code from the user space if available
    if (_result_handle.getDetailedDescription()) {
        return _result_handle.getDetailedDescription()->getErrorCode();
    }
    return _result_handle.getErrorCode();
}

inline const char* Result::getErrorLabel() const
{
    if (_result_handle.getDetailedDescription()) {
        // we can get the stringified error code from the detailed description
        return _result_handle.getDetailedDescription()->getErrorCodeLabel();
    }

    return *this ? "SUCCESS" : "(unknown)";
}

inline const char* Result::getDescription() const
{
    if (0 != _result_handle.getErrorCode()) {
        // only the error code was set, no description available
        return "";
    }

    if (_result_handle.getDetailedDescription()) {
        return _result_handle.getDetailedDescription()->getErrorDescription();
    }

    return "No error occurred";
}

inline std::int32_t Result::getLine() const
{
    if (_result_handle.getDetailedDescription()) {
        return _result_handle.getDetailedDescription()->getLine();
    }

    return -1;
}

inline const char* Result::getFile() const
{
    if (_result_handle.getDetailedDescription()) {
        return _result_handle.getDetailedDescription()->getFileName();
    }

    return "";
}

inline const char* Result::getFunction() const
{
    if (_result_handle.getDetailedDescription()) {
        return _result_handle.getDetailedDescription()->getFunctionName();
    }

    return "";
}

inline Result::operator bool() const noexcept
{
    return getErrorCode() == 0;
}

inline Result::Result(std::int32_t error_code,
                      const char* error_description,
                      std::int32_t line,
                      const char* file,
                      const char* function,
                      const char* label)
    : _result_handle(DescriptionType::makeResultDescription<detail::ErrorDescriptionType>(
          error_code, error_description, line, file, function, label))
{
}

inline Result Result::createFrom(std::int32_t error_code,
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

inline bool operator==(const Result& lhs, const Result& rhs)
{
    return lhs._result_handle == rhs._result_handle;
}

inline bool operator!=(const Result& lhs, const Result& rhs)
{
    return !(lhs == rhs);
}

template <typename T>
inline bool operator==(const Result& result, const ResultInfo<T>&)
{
    return result.getErrorCode() == ResultInfo<T>::getCode();
}

template <typename T>
inline bool operator!=(const Result& result, const ResultInfo<T>& error_code)
{
    return !(result == error_code);
}

template <typename T>
inline bool operator==(const ResultInfo<T>& error_code, const Result& result)
{
    return result == error_code;
}

template <typename T>
inline bool operator!=(const ResultInfo<T>& error_code, const Result& result)
{
    return result != error_code;
}

inline void swap(Result& lhs, Result& rhs)
{
    // enable ADL
    using std::swap;
    swap(lhs._result_handle, rhs._result_handle);
}

template <>
inline bool isOk<Result>(const Result& result)
{
    return 0 == result.getErrorCode();
}

} // namespace result
} // namespace a_util

namespace std {
template <>
inline void swap<a_util::result::Result>(a_util::result::Result& lhs, a_util::result::Result& rhs)
{
    a_util::result::swap(lhs, rhs);
}
} // namespace std

#endif // A_UTIL_UTIL_RESULT_DETAIL_RESULT_TYPE_IMPL_HEADER_INCLUDED
