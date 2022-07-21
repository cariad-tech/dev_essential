/**
 * @file
 * Private implementation for @ref a_util::result::ResultInfo "ResultInfo" type and functions
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

#ifndef A_UTIL_UTIL_RESULT_DETAIL_RESULT_INFO_IMPL_HEADER_INCLUDED
#define A_UTIL_UTIL_RESULT_DETAIL_RESULT_INFO_IMPL_HEADER_INCLUDED

#include <a_util/base/types.h>
#include <a_util/result/result_info_decl.h>

namespace a_util {
namespace result {
template <typename ResultType>
inline bool isOk(const ResultType& result)
{ // zero initialization
    return (ResultType() == result);
}

template <typename ResultType>
inline bool isFailed(const ResultType& result)
{
    return !(isOk(result));
}

template <typename T>
inline const char* ResultInfo<T>::getLabel()
{
    return T::getLabel();
}

template <typename T>
inline std::int32_t ResultInfo<T>::getCode()
{
    return T::getCode();
}

inline std::int32_t ResultInfo<void>::getCode()
{
    return 0;
}

inline const char* ResultInfo<void>::getLabel()
{
    return "SUCCESS";
}

template <typename ResultTypeLHS, typename ResultTypeRHS>
inline bool operator==(const ResultInfo<ResultTypeLHS>& lhs, const ResultInfo<ResultTypeRHS>& rhs)
{
    a_util::maybe_unused(lhs);
    a_util::maybe_unused(rhs);
    return (lhs.getCode() == rhs.getCode()) && (lhs.getLabel() == rhs.getLabel());
}

template <typename ResultTypeLHS, typename ResultTypeRHS>
inline bool operator!=(const ResultInfo<ResultTypeLHS>& lhs, const ResultInfo<ResultTypeRHS>& rhs)
{
    return !operator==(lhs, rhs);
}

template <typename ResultTypeLHS, typename ResultTypeRHS>
inline bool operator<(const ResultInfo<ResultTypeLHS>& lhs, const ResultInfo<ResultTypeRHS>& rhs)
{
    return lhs.getCode() < rhs.getCode();
}

template <typename ResultTypeLHS, typename ResultTypeRHS>
inline bool operator>(const ResultInfo<ResultTypeLHS>& lhs, const ResultInfo<ResultTypeRHS>& rhs)
{
    return operator<(rhs, lhs);
}

template <typename ResultTypeLHS, typename ResultTypeRHS>
inline bool operator<=(const ResultInfo<ResultTypeLHS>& lhs, const ResultInfo<ResultTypeRHS>& rhs)
{
    return !operator>(lhs, rhs);
}

template <typename ResultTypeLHS, typename ResultTypeRHS>
inline bool operator>=(const ResultInfo<ResultTypeLHS>& lhs, const ResultInfo<ResultTypeRHS>& rhs)
{
    return !operator<(lhs, rhs);
}

} // namespace result
} // namespace a_util

#endif // A_UTIL_UTIL_RESULT_DETAIL_RESULT_INFO_IMPL_HEADER_INCLUDED
