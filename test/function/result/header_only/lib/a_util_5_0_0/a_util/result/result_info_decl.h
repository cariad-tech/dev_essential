/**
 * @file
 * Result information types and macros usable to create objects of type @c a_util::result::Result
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

#include <a_util/base/std/types.h>

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
            return _no;                                                                            \
        }                                                                                          \
        static const char* getLabel()                                                              \
        {                                                                                          \
            return #_label;                                                                        \
        }                                                                                          \
    };                                                                                             \
    const ::a_util::result::ResultInfo<ResultType_##_label> _label = ResultType_##_label();        \
    }

namespace a_util {
namespace result {

template <typename ResultType>
bool isOk(const ResultType& result);

template <typename ResultType>
bool isFailed(const ResultType& result);

/**
 * @brief Basic result information template, used by @c a_util::result::Result
 * @tparam T Type required to implement static methods @c getLabel() and @c getCode()
 */
template <typename T>
struct ResultInfo {
    static std::int32_t getCode();
    static const char* getLabel();
};

/**
 * @brief Implements basic success result information for usage with @c a_util::result::Result
 */
template <>
struct ResultInfo<void> {
    static std::int32_t getCode();
    static const char* getLabel();
};

namespace {
/// Instance of basic success type
const ResultInfo<void> SUCCESS = ResultInfo<void>();
} // namespace

/**
 * @brief Compare two specialized result information objects for equality
 * @tparam ResultTypeLHS Left hand side result type
 * @tparam ResultTypeRHS Right hand side result type
 * @param[in] lhs Left hand side result information
 * @param[in] rhs Right hand side result information
 * @return @c true if rhs and lhs are equal, @c false otherwise.
 */
template <typename ResultTypeLHS, typename ResultTypeRHS>
bool operator==(const ResultInfo<ResultTypeLHS>& lhs, const ResultInfo<ResultTypeRHS>& rhs);

/**
 * @brief Compare two specialized result information objects for inequality
 * @tparam ResultTypeLHS Left hand side result type
 * @tparam ResultTypeRHS Right hand side result type
 * @param[in] lhs Left hand side result information
 * @param[in] rhs Right hand side result information
 * @return @c false if rhs and lhs are equal, @c true otherwise.
 */
template <typename ResultTypeLHS, typename ResultTypeRHS>
bool operator!=(const ResultInfo<ResultTypeLHS>& lhs, const ResultInfo<ResultTypeRHS>& rhs);

/**
 * @brief Less-than operator for two specialized result information objects
 * @tparam ResultTypeLHS Left hand side result type
 * @tparam ResultTypeRHS Right hand side result type
 * @param[in] lhs Left hand side result information
 * @param[in] rhs Right hand side result information
 * @return @c true if lhs is less than rhs, @c false otherwise.
 */
template <typename ResultTypeLHS, typename ResultTypeRHS>
bool operator<(const ResultInfo<ResultTypeLHS>& lhs, const ResultInfo<ResultTypeRHS>& rhs);

/**
 * @brief Greater-than operator for two specialized result information objects
 * @tparam ResultTypeLHS Left hand side result type
 * @tparam ResultTypeRHS Right hand side result type
 * @param[in] lhs Left hand side result information
 * @param[in] rhs Right hand side result information
 * @return @c false if lhs is less than or equal rhs, @c true otherwise.
 */
template <typename ResultTypeLHS, typename ResultTypeRHS>
bool operator>(const ResultInfo<ResultTypeLHS>& lhs, const ResultInfo<ResultTypeRHS>& rhs);

/**
 * @brief Less-than-or-equal operator for two specialized result information objects
 * @tparam ResultTypeLHS Left hand side result type
 * @tparam ResultTypeRHS Right hand side result type
 * @param[in] lhs Left hand side result information
 * @param[in] rhs Right hand side result information
 * @return @c true if lhs is less than or equal rhs, @c false otherwise.
 */
template <typename ResultTypeLHS, typename ResultTypeRHS>
bool operator<=(const ResultInfo<ResultTypeLHS>& lhs, const ResultInfo<ResultTypeRHS>& rhs);

/**
 * @brief Greater-than-or-equal operator for two specialized result information objects
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

#endif // A_UTIL_UTIL_RESULT_RESULT_INFO_DECL_HEADER_INCLUDED
