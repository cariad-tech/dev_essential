/**
 * @file
 * Private implementation for @ref a_util::result::detail::ResultDescription "ResultDescription"
 * type and functions
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

#ifndef A_UTIL_UTIL_RESULT_DETAIL_DESCRIPTION_IMPL_HEADER_INCLUDED
#define A_UTIL_UTIL_RESULT_DETAIL_DESCRIPTION_IMPL_HEADER_INCLUDED

#include "a_util/result/detail/result_description_decl.h"

// This comment keeps result_description_decl.h on top while clang-formatting
#include "a_util/result/detail/error_description.h"
#include "a_util/result/detail/reference_counted_object.h"

#include <new>

namespace a_util {
namespace result {
namespace detail {
template <typename DescriptionIntf>
inline bool ResultDescriptionTraits<DescriptionIntf>::isDetailedDescriptionSet(
    const std::uint64_t value)
{
    // detailed result was allocated...
    return (!isErrorCodeSet(value))              // if the error code bit is not set, ...
           && 0 != (value & error_code_bitmask); //... but other bits
}

template <typename DescriptionIntf>
inline bool ResultDescriptionTraits<DescriptionIntf>::isErrorCodeSet(const std::uint64_t value)
{
    // the error code bit indicates whether only the error code is set
    return error_code_bit == (value & error_code_bit);
}

template <typename DescriptionIntf>
inline ResultDescription<DescriptionIntf>::ResultDescription() : _pointer_to_result_or_error_code(0)
{
}

template <typename DescriptionIntf>
inline ResultDescription<DescriptionIntf>::ResultDescription(const ResultDescription& other)
    : _pointer_to_result_or_error_code(0)
{
    *this = other;
}

template <typename DescriptionIntf>
inline ResultDescription<DescriptionIntf>& ResultDescription<DescriptionIntf>::operator=(
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

template <typename DescriptionIntf>
inline ResultDescription<DescriptionIntf>::~ResultDescription()
{
    if (TraitsType::isDetailedDescriptionSet(_pointer_to_result_or_error_code)) {
        typedef typename TraitsType::ReferenceCountedDescriptionType reference_counted_impl_type;
        reinterpret_cast<reference_counted_impl_type*>(_pointer_to_result_or_error_code)
            ->removeReference();
    }
}

template <typename DescriptionIntf>                   // class template parameter
template <typename DescriptionImpl, typename... Args> // method template parameters
typename ResultDescription<DescriptionIntf>::SelfType // return type
ResultDescription<DescriptionIntf>::makeResultDescription(std::int32_t error_code, Args&&... args)
{
    typedef ReferenceCountedObject<DescriptionIntf, DescriptionImpl> ReferenceCountedErrorType;
    SelfType oDescription;

    // do not use std::nothrow overload, corresponding 'new'-operator can only be overloaded global
    try {
        // first parameter of the detailed description must be of type error code
        oDescription._pointer_to_result_or_error_code = reinterpret_cast<std::uint64_t>(
            new ReferenceCountedErrorType(error_code, std::forward<Args>(args)...));
    }
    catch (const std::bad_alloc&) {
        // operator new() failed to alloc
        oDescription._pointer_to_result_or_error_code = 0;
    }

    if (0 != oDescription._pointer_to_result_or_error_code) {
        typedef
            typename TraitsType::ReferenceCountedDescriptionType ReferenceCountedDescriptionType;
        const ReferenceCountedDescriptionType* const pReferenceCountedObject =
            reinterpret_cast<ReferenceCountedDescriptionType* const>(
                oDescription._pointer_to_result_or_error_code);

        pReferenceCountedObject->addReference();
        // the detailed description is allowed only if the MSB is not set to 1!
        // in reality this should never happen.
        if (TraitsType::isErrorCodeSet(oDescription._pointer_to_result_or_error_code)) {
            // not allowed, only create the error code
            pReferenceCountedObject->removeReference();
            oDescription._pointer_to_result_or_error_code = 0;
        }
        else {
            return oDescription;
        }
    }
    return SelfType::makeResultDescription(error_code);
}

template <typename DescriptionIntf>
template <typename T>
typename ResultDescription<DescriptionIntf>::SelfType ResultDescription<
    DescriptionIntf>::makeResultDescription(const ::a_util::result::ResultInfo<T>&) noexcept
{
    static const UncountedObject<DescriptionIntf, ResultInfoDescription<T>> uncounted_error;

    SelfType result_description;
    result_description._pointer_to_result_or_error_code =
        reinterpret_cast<const std::uint64_t>(&uncounted_error);
    return result_description;
}

template <typename DescriptionIntf>
inline ResultDescription<DescriptionIntf> ResultDescription<DescriptionIntf>::makeResultDescription(
    std::int32_t error_code)
{
    // use the entire pointer for the error code and set the error code bit appropriately
    // the user doesn't want to allocate space by calling this function, so just DON'T!
    return SelfType(error_code);
}

template <typename DescriptionIntf>
inline DescriptionIntf const* ResultDescription<DescriptionIntf>::getDetailedDescription() const
{
    // make sure, detailed description instead of error code is set
    if (TraitsType::isDetailedDescriptionSet(_pointer_to_result_or_error_code)) {
        return &reinterpret_cast<typename TraitsType::ReferenceCountedDescriptionType*>(
                    _pointer_to_result_or_error_code)
                    ->getObject();
    }
    return nullptr;
}

template <typename DescriptionIntf>
inline std::int32_t ResultDescription<DescriptionIntf>::getErrorCode() const
{
    // only return the error code if only the error code was set!
    if (TraitsType::isErrorCodeSet(_pointer_to_result_or_error_code)) {
        return static_cast<std::int32_t>((_pointer_to_result_or_error_code | 0xFFFFFFFF00000000) &
                                         TraitsType::error_code_bitmask);
    }
    return 0;
}

template <typename DescriptionIntf>
inline ResultDescription<DescriptionIntf>::ResultDescription(std::uint64_t error_code)
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
