/**
 * @file
 * Declarations for helper structs and functions for detailed result descriptions
 *
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */

#ifndef A_UTIL_UTIL_RESULT_DETAIL_DESCRIPTION_IMPL_HEADER_INCLUDED
#define A_UTIL_UTIL_RESULT_DETAIL_DESCRIPTION_IMPL_HEADER_INCLUDED

#include <a_util/base/nullptr_fallback.h>
#include <a_util/base/static_assert_fallback.h>
#include <a_util/base/std/types.h>
#include <a_util/result/detail/reference_counted_object_intf.h>
#include <a_util/result/detail/result_description_decl.h>

namespace a_util {
namespace result {
namespace detail {

template <typename DescriptionType>
inline bool ResultDescriptionTraits<DescriptionType>::isDetailedDescriptionSet(
    const std::uint64_t value)
{
    // detailed result was allocated...
    return (!isErrorCodeSet(value))              // if the error code bit is not set, ...
           && 0 != (value & error_code_bitmask); //... but other bits
}

template <typename DescriptionType>
inline bool ResultDescriptionTraits<DescriptionType>::isErrorCodeSet(const std::uint64_t value)
{
    // the error code bit indicates whether only the error code is set
    return error_code_bit == (value & error_code_bit);
}

template <typename DescriptionType>
inline ResultDescription<DescriptionType>::ResultDescription() : _pointer_to_result_or_error_code(0)
{
}

template <typename DescriptionType>
inline ResultDescription<DescriptionType>::ResultDescription(const ResultDescription& other)
    : _pointer_to_result_or_error_code(0)
{
    *this = other;
}

template <typename DescriptionType>
inline ResultDescription<DescriptionType>& ResultDescription<DescriptionType>::operator=(
    const ResultDescription& other)
{
    typedef typename TraitsType::ReferenceCountedDescriptionType ReferenceCountedType;

    if (&other != this) {
        if (TraitsType::isDetailedDescriptionSet(_pointer_to_result_or_error_code)) {
            reinterpret_cast<ReferenceCountedType*>(_pointer_to_result_or_error_code)
                ->removeReference();
        }

        _pointer_to_result_or_error_code = other._pointer_to_result_or_error_code;
        if (TraitsType::isDetailedDescriptionSet(_pointer_to_result_or_error_code)) {
            reinterpret_cast<ReferenceCountedType*>(_pointer_to_result_or_error_code)
                ->addReference();
        }
    }
    return *this;
}

template <typename DescriptionType>
inline ResultDescription<DescriptionType>::~ResultDescription()
{
    if (TraitsType::isDetailedDescriptionSet(_pointer_to_result_or_error_code)) {
        typedef typename TraitsType::ReferenceCountedDescriptionType reference_counted_impl_type;
        reinterpret_cast<reference_counted_impl_type*>(_pointer_to_result_or_error_code)
            ->removeReference();
    }
}

template <typename DescriptionType>
inline ResultDescription<DescriptionType> ResultDescription<DescriptionType>::makeResultDescription(
    std::int32_t error_code)
{
    // use the entire pointer for the error code and set the error code bit appropriately
    // the user doesn't want to allocate space by calling this function, so just DON'T!
    return SelfType(error_code);
}

template <typename DescriptionType>
inline DescriptionType const* ResultDescription<DescriptionType>::getDetailedDescription() const
{
    // make sure, detailed description instead of error code is set
    if (TraitsType::isDetailedDescriptionSet(_pointer_to_result_or_error_code)) {
        return &reinterpret_cast<typename TraitsType::ReferenceCountedDescriptionType*>(
                    _pointer_to_result_or_error_code)
                    ->getObject();
    }
    return nullptr;
}

template <typename DescriptionType>
inline std::int32_t ResultDescription<DescriptionType>::getErrorCode() const
{
    // only return the error code if only the error code was set!
    if (TraitsType::isErrorCodeSet(_pointer_to_result_or_error_code)) {
        return static_cast<std::int32_t>((_pointer_to_result_or_error_code | 0xFFFFFFFF00000000) &
                                         TraitsType::error_code_bitmask);
    }
    return 0;
}

template <typename DescriptionType>
inline ResultDescription<DescriptionType>::ResultDescription(std::uint64_t error_code)
    : // highly optimized code to bypass the cast on negative error codes...
      _pointer_to_result_or_error_code(((error_code & 0xFFFFFFFF) | TraitsType::error_code_bit))
{
}

template <typename T>
inline bool operator==(const ResultDescription<T>& lhs, const ResultDescription<T>& rhs)
{
    // we put the tests inside this function so it will be available for all specializations
    static_assert(sizeof(std::uintptr_t) <= 8,
                  "ResultDescription pointers must always have a size of 8 Byte or less!");
    static_assert(sizeof(ResultDescription<T>) == 8, "Description type unsupported size!");
    //    static_assert(std::is_standard_layout<ResultDescription<T>>::value,
    //                        "Description type is not of standard layout!");

    return lhs.getErrorCode() == rhs.getErrorCode() &&
           lhs.getDetailedDescription() == rhs.getDetailedDescription();
}

template <typename T>
inline bool operator!=(const ResultDescription<T>& lhs, const ResultDescription<T>& rhs)
{
    return !(lhs == rhs);
}

} // namespace detail
} // namespace result
} // namespace a_util

#endif // A_UTIL_UTIL_RESULT_DETAIL_DESCRIPTION_IMPL_HEADER_INCLUDED
