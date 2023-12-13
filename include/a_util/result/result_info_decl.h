/**
 * @file
 * Public API for @ref a_util::result::ResultInfo "ResultInfo" type, functions and macros
 *
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */

#ifndef A_UTIL_UTIL_RESULT_RESULT_INFO_DECL_HEADER_INCLUDED
#define A_UTIL_UTIL_RESULT_RESULT_INFO_DECL_HEADER_INCLUDED

#include <a_util/preprocessor/deprecated.h>

#include <cstdint>
#include <limits>

/** @cond INTERNAL_DOCUMENTATION */
#ifndef DEV_ESSENTIAL_DISABLE_MAKE_RESULT_CHECK_RESERVED_ZERO
#define DEV_ESSENTIAL_INTERNAL_MAKE_RESULT_CHECK_NOT_ZERO(_no) _no != 0
#else
#define DEV_ESSENTIAL_INTERNAL_MAKE_RESULT_CHECK_NOT_ZERO(...) true
#endif // DEV_ESSENTIAL_DISABLE_MAKE_RESULT_CHECK_RESERVED_ZERO
/** @endcond */

/**
 * Create a result type and a constant instance of this type in an unnamed namespace
 * @param[in] _no Numeric representation of the result type
 * @param[in] _label Stringified version for the result type
 */
#define _MAKE_RESULT(_no, _label)                                                                  \
    namespace {                                                                                    \
    struct ResultType_##_label : public ::a_util::result::ResultInfo<ResultType_##_label> {        \
        static std::int32_t getCode()                                                              \
        {                                                                                          \
            static_assert(std::is_same<decltype((_no)), std::int32_t>::value,                      \
                          #_no " is not in range of std::int32_t.");                               \
            return _no;                                                                            \
        }                                                                                          \
        static const char* getLabel()                                                              \
        {                                                                                          \
            return #_label;                                                                        \
        }                                                                                          \
    };                                                                                             \
    const ::a_util::result::ResultInfo<ResultType_##_label> _label = ResultType_##_label();        \
    } /* ns anonymous */                                                                           \
    static_assert(DEV_ESSENTIAL_INTERNAL_MAKE_RESULT_CHECK_NOT_ZERO(_no),                          \
                  "'0' is not allowed, because it is reserved for results indicating no error. "   \
                  "Please use a_util::result::SUCCESS instead. To disable this check, define "     \
                  "'DEV_ESSENTIAL_DISABLE_MAKE_RESULT_CHECK_RESERVED_ZERO'")

namespace a_util {
namespace result {
/**
 * Check whether a return value indicates a successful function execution
 * @tparam ResultType Type of the result value
 * @param[in] result Value of the result type
 * @dev_essential_deprecated Usage of this function template is not type safe.
 * @return @c true if @c result indicates success, @c false otherwise.
 */
template <typename ResultType>
DEV_ESSENTIAL_DEPRECATED("Usage of this function template is not type safe.")
bool isOk(const ResultType& result);

/**
 * Check whether a return value indicates an unsuccessful function execution
 * @tparam ResultType Type of the result value
 * @param[in] result Value of the result type
 * @dev_essential_deprecated Usage of this function template is not type safe..
 * @return @c true if @c result indicates failure, @c false otherwise.
 */
template <typename ResultType>
DEV_ESSENTIAL_DEPRECATED("Usage of this function template is not type safe.")
bool isFailed(const ResultType& result);

/**
 * Basic result information template, used by @c a_util::result::Result
 * @tparam T Type required to implement static methods @c getLabel() and @c getCode()
 */
template <typename T>
struct ResultInfo {
    /**
     * Get the result code distributed with value of type T
     * @return The result code
     */
    static std::int32_t getCode();
    /**
     * Get the string representation of an error code distributed by value of type T
     * @return The string representation of an error code
     */
    static const char* getLabel();
};

/**
 * Implements basic success result information for usage with @c a_util::result::Result
 */
template <>
struct ResultInfo<void> {
    /**
     * Get result code indicating success.
     * @retval 0
     */
    static std::int32_t getCode();
    /**
     * Get the string representation of the result code.
     * @retval "SUCCESS"
     */
    static const char* getLabel();
};

namespace {
/// Instance of basic success type
const ResultInfo<void> SUCCESS = ResultInfo<void>();
} // namespace

/**
 * Compare two specialized result information objects for equality
 * @tparam ResultTypeLHS Left hand side result type
 * @tparam ResultTypeRHS Right hand side result type
 * @param[in] lhs Left hand side result information
 * @param[in] rhs Right hand side result information
 * @return @c true if rhs and lhs are equal, @c false otherwise.
 */
template <typename ResultTypeLHS, typename ResultTypeRHS>
bool operator==(const ResultInfo<ResultTypeLHS>& lhs, const ResultInfo<ResultTypeRHS>& rhs);

/**
 * Compare two specialized result information objects for inequality
 * @tparam ResultTypeLHS Left hand side result type
 * @tparam ResultTypeRHS Right hand side result type
 * @param[in] lhs Left hand side result information
 * @param[in] rhs Right hand side result information
 * @return @c false if rhs and lhs are equal, @c true otherwise.
 */
template <typename ResultTypeLHS, typename ResultTypeRHS>
bool operator!=(const ResultInfo<ResultTypeLHS>& lhs, const ResultInfo<ResultTypeRHS>& rhs);

/**
 * Less-than operator for two specialized result information objects
 * @tparam ResultTypeLHS Left hand side result type
 * @tparam ResultTypeRHS Right hand side result type
 * @param[in] lhs Left hand side result information
 * @param[in] rhs Right hand side result information
 * @return @c true if lhs is less than rhs, @c false otherwise.
 */
template <typename ResultTypeLHS, typename ResultTypeRHS>
bool operator<(const ResultInfo<ResultTypeLHS>& lhs, const ResultInfo<ResultTypeRHS>& rhs);

/**
 * Greater-than operator for two specialized result information objects
 * @tparam ResultTypeLHS Left hand side result type
 * @tparam ResultTypeRHS Right hand side result type
 * @param[in] lhs Left hand side result information
 * @param[in] rhs Right hand side result information
 * @return @c false if lhs is less than or equal rhs, @c true otherwise.
 */
template <typename ResultTypeLHS, typename ResultTypeRHS>
bool operator>(const ResultInfo<ResultTypeLHS>& lhs, const ResultInfo<ResultTypeRHS>& rhs);

/**
 * Less-than-or-equal operator for two specialized result information objects
 * @tparam ResultTypeLHS Left hand side result type
 * @tparam ResultTypeRHS Right hand side result type
 * @param[in] lhs Left hand side result information
 * @param[in] rhs Right hand side result information
 * @return @c true if lhs is less than or equal rhs, @c false otherwise.
 */
template <typename ResultTypeLHS, typename ResultTypeRHS>
bool operator<=(const ResultInfo<ResultTypeLHS>& lhs, const ResultInfo<ResultTypeRHS>& rhs);

/**
 * Greater-than-or-equal operator for two specialized result information objects
 * @tparam ResultTypeLHS Left hand side result type
 * @tparam ResultTypeRHS Right hand side result type
 * @param[in] lhs Left hand side result information
 * @param[in] rhs Right hand side result information
 * @return @c false if lhs is less than rhs, @c true otherwise.
 */
template <typename ResultTypeLHS, typename ResultTypeRHS>
bool operator>=(const ResultInfo<ResultTypeLHS>& lhs, const ResultInfo<ResultTypeRHS>& rhs);

} // namespace result
} // namespace a_util

/** @cond INTERNAL_DOCUMENTATION */
#include <a_util/result/detail/result_info_impl.h>
/** @endcond*/
#endif // A_UTIL_UTIL_RESULT_RESULT_INFO_DECL_HEADER_INCLUDED
