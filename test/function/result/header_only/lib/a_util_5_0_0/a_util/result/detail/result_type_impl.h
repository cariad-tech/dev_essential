/**
 * @file
 * Declaring the result class which might be used as generalized return type
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

#include <a_util/base/config.h>
#include <a_util/base/std/types.h>
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
    : _result_handle(DescriptionType::makeResultDescription(error.getCode()))
{
}

template <typename ErrorType>
inline Result::Result(ResultInfo<ErrorType> error,
                      const char* error_description,
                      std::int32_t line,
                      const char* file,
                      const char* function)
    : _result_handle()
{
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
    *this = CreateFrom(error_code, error_description, line, file, function);
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

#if A_UTIL_ENABLE_COMPILER_CXX_RVALUE_REFERENCES
inline Result::Result(Result&& other) : _result_handle()
{
    swap(*this, other);
    if (SUCCESS != other) {
        other = Result();
    }
}
#endif //#if A_UTIL_ENABLE_COMPILER_CXX_RVALUE_REFERENCES

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
{ // enable ADL
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
