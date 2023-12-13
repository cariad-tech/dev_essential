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
@endverbatim
 */

#ifndef A_UTIL_UTIL_RESULT_DETAIL_DESCRIPTION_IMPL_HEADER_INCLUDED
#define A_UTIL_UTIL_RESULT_DETAIL_DESCRIPTION_IMPL_HEADER_INCLUDED

#include <a_util/result/detail/result_description_decl.h>

// This comment keeps result_description_decl.h on top while clang-formatting
#include <a_util/result/detail/error_description.h>
#include <a_util/result/detail/reference_counted_object.h>

#include <cassert>
#include <new>

namespace a_util {
namespace result {
namespace detail {
template <typename DescriptionIntf>
inline bool ResultDescriptionTraits<DescriptionIntf>::isDetailedDescriptionSet(
    const std::uint64_t value)
{
    // detailed result was allocated...
    return (!isErrorCodeSet(value)) &&        // if the error code bit is not set, ...
           0 != (value & error_code_bitmask); //... but other bits
}

template <typename DescriptionIntf>
inline bool ResultDescriptionTraits<DescriptionIntf>::isErrorCodeSet(const std::uint64_t value)
{
    // the error code bit indicates whether only the error code is set
    return error_code_bit == (value & error_code_bit);
}

template <typename DescriptionIntf>
inline constexpr std::uint64_t ResultDescriptionTraits<DescriptionIntf>::toInternalErrorCode(
    const std::int32_t error_code) noexcept
{
    // highly optimized code to bypass the cast on negative error codes...
    return (
        // fit the error code to the first 32 bits
        (static_cast<std::uint64_t>(error_code) & error_code_serialize_bitmask) |
        // set the error code bit to the most significant bit
        error_code_bit);
}

template <typename DescriptionIntf>
inline constexpr std::int32_t ResultDescriptionTraits<DescriptionIntf>::toExternalErrorCode(
    const std::uint64_t error_code) noexcept
{
    return static_cast<std::int32_t>(
        // get rid of the error code bit
        error_code_bitmask &
        // convert error code to its original value
        (error_code | error_code_deserialize_bitmask));
}

template <typename DescriptionIntf>
inline constexpr const ReferenceCountedObjectInterface<DescriptionIntf>* ResultDescriptionTraits<
    DescriptionIntf>::toInternalErrorPointer(const std::uint64_t error_code) noexcept
{
    return reinterpret_cast<const ReferenceCountedDescriptionType*>(error_code);
}

template <typename DescriptionIntf>
inline ResultDescriptionTraits<DescriptionIntf>::~ResultDescriptionTraits()
{
    // check whether the internal representation remains binary compatible
    // the calculation might be changed
    using namespace std;
    constexpr auto minus_one_representation = toInternalErrorCode(-1);
    constexpr auto plus_one_representation = toInternalErrorCode(1);

#if defined(__GNUC__) && ((__GNUC__ == 5) && (__GNUC_MINOR__ == 2))
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
#endif // defined(__GNUC__) && ((__GNUC__ == 5) && (__GNUC_MINOR__ == 2))
    constexpr auto min_val_representation =
        toInternalErrorCode((numeric_limits<std::int32_t>::min)());
    constexpr auto max_val_representation =
        toInternalErrorCode((numeric_limits<std::int32_t>::max)());
#if defined(__GNUC__) && ((__GNUC__ == 5) && (__GNUC_MINOR__ == 2))
#pragma GCC diagnostic pop
#endif // defined(__GNUC__) && ((__GNUC__ == 5) && (__GNUC_MINOR__ == 2))

    // but these checks must not be changed!
    // this is the original error code specification
    // these checks exist to make sure refactoring the code does not change the original behavior
    static_assert(0x8000000000000000 == error_code_bit, "error_code_bit must not be changed!");
    static_assert(~0x8000000000000000 == error_code_bitmask,
                  "error_code_bitmask must not be changed!");
    static_assert(0xFFFFFFFF00000000 == error_code_deserialize_bitmask,
                  "error_code_deserialize_bitmask must not be changed!");
    static_assert(0xFFFFFFFF == error_code_serialize_bitmask,
                  "error_code_serialize_bitmask must not be changed!");
    static_assert(0x80000000ffffffff == minus_one_representation,
                  "Calculation of internal error code representation must not be changed!");
    static_assert(0x8000000000000001 == plus_one_representation,
                  "Calculation of internal error code representation must not be changed!");
    static_assert(0x8000000080000000 == min_val_representation,
                  "Calculation of internal error code representation must not be changed!");
    static_assert(0x800000007fffffff == max_val_representation,
                  "Calculation of internal error code representation must not be changed!");
}

template <typename DescriptionIntf>
inline ResultDescription<DescriptionIntf>::ResultDescription() : _pointer_to_result_or_error_code{}
{
}

template <typename DescriptionIntf>
inline ResultDescription<DescriptionIntf>::ResultDescription(const ResultDescription& other)
    : _pointer_to_result_or_error_code{}
{
    *this = other;
}

template <typename DescriptionIntf>
inline ResultDescription<DescriptionIntf>& ResultDescription<DescriptionIntf>::operator=(
    const ResultDescription& other)
{
    if (&other != this) {
        if (TraitsType::isDetailedDescriptionSet(_pointer_to_result_or_error_code)) {
            TraitsType::toInternalErrorPointer(_pointer_to_result_or_error_code)->removeReference();
        }

        _pointer_to_result_or_error_code = other._pointer_to_result_or_error_code;
        if (TraitsType::isDetailedDescriptionSet(_pointer_to_result_or_error_code)) {
            TraitsType::toInternalErrorPointer(_pointer_to_result_or_error_code)->addReference();
        }
    }
    return *this;
}

template <typename DescriptionIntf>
inline ResultDescription<DescriptionIntf>::ResultDescription(ResultDescription&& other)
    : _pointer_to_result_or_error_code{}
{
    std::swap(other._pointer_to_result_or_error_code, _pointer_to_result_or_error_code);
}

template <typename DescriptionIntf>
inline ResultDescription<DescriptionIntf>& ResultDescription<DescriptionIntf>::operator=(
    ResultDescription&& other)
{
    if (&other != this) {
        if (TraitsType::isDetailedDescriptionSet(_pointer_to_result_or_error_code)) {
            TraitsType::toInternalErrorPointer(_pointer_to_result_or_error_code)->removeReference();
        }

        _pointer_to_result_or_error_code = {};
        std::swap(other._pointer_to_result_or_error_code, _pointer_to_result_or_error_code);
    }
    return *this;
}

template <typename DescriptionIntf>
inline ResultDescription<DescriptionIntf>::~ResultDescription()
{
    if (TraitsType::isDetailedDescriptionSet(_pointer_to_result_or_error_code)) {
        TraitsType::toInternalErrorPointer(_pointer_to_result_or_error_code)->removeReference();
    }
    // DTOR is always compiled, so put the test here
    static_assert(sizeof(std::uintptr_t) <= 8,
                  "ResultDescription pointers must always have a size of 8 Byte or less!");
}

template <typename DescriptionIntf>                   // class template parameter
template <typename DescriptionImpl, typename... Args> // method template parameters
typename ResultDescription<DescriptionIntf>::SelfType // return type
ResultDescription<DescriptionIntf>::makeResultDescription(std::int32_t error_code, Args&&... args)
{
    // do not use std::nothrow overload, corresponding 'new'-operator can only be overloaded global
    try {
        typedef ReferenceCountedObject<DescriptionIntf, DescriptionImpl> ReferenceCountedErrorType;
        // first parameter of the detailed description must be of type error code
        const auto reference_counted_error_object =
            new ReferenceCountedErrorType(error_code, std::forward<Args>(args)...);
        SelfType result_description(*reference_counted_error_object);
        if (TraitsType::isErrorCodeSet(result_description._pointer_to_result_or_error_code)) {
            // not allowed, only create the error code (no assert in production code, only in debug)
            reference_counted_error_object->removeReference();
            assert((false) && "The MSB of the pointer to the heap allocated memory is set to 1, "
                              "but it is reserved for the error code bit. This is inplausible and "
                              "must not happen.");
        }
        else {
            return result_description;
        }
    }
    catch (const std::bad_alloc&) {
        // operator new() failed to alloc
    }

    return SelfType::makeResultDescription(error_code);
}

template <typename DescriptionIntf>
template <typename T>
typename ResultDescription<DescriptionIntf>::SelfType ResultDescription<
    DescriptionIntf>::makeResultDescription(const ::a_util::result::ResultInfo<T>&) noexcept
{
    static const UncountedObject<DescriptionIntf, ResultInfoDescription<T>> uncounted_error;
    SelfType result_description(uncounted_error);

    // The address of the pointer must not occupy the MSB of the internal error, because the
    // MSB is used to set the error code bit. In reality this should never happen.
    if (!TraitsType::isErrorCodeSet(result_description._pointer_to_result_or_error_code)) {
        return result_description;
    }

    return SelfType::makeResultDescription(::a_util::result::ResultInfo<T>::getCode());
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
        return &TraitsType::toInternalErrorPointer(_pointer_to_result_or_error_code)->getObject();
    }
    return nullptr;
}

template <typename DescriptionIntf>
inline std::int32_t ResultDescription<DescriptionIntf>::getErrorCode() const
{
    // only return the error code if only the error code was set!
    if (TraitsType::isErrorCodeSet(_pointer_to_result_or_error_code)) {
        return TraitsType::toExternalErrorCode(_pointer_to_result_or_error_code);
    }
    return 0;
}

template <typename DescriptionIntf>
constexpr inline ResultDescription<DescriptionIntf>::ResultDescription(
    std::int32_t error_code) noexcept
    : _pointer_to_result_or_error_code(TraitsType::toInternalErrorCode(error_code))
{
}

template <typename DescriptionIntf>
constexpr inline ResultDescription<DescriptionIntf>::ResultDescription(
    const ReferenceCountedObjectInterface<DescriptionIntf>& error_object) noexcept
    : _pointer_to_result_or_error_code(reinterpret_cast<std::uintptr_t>(&error_object))
{
    static_assert(sizeof(std::uintptr_t) == sizeof(decltype(&error_object)),
                  "ResultDescription pointers must match the size of std::uintptr_t!");
    assert((!TraitsType::isErrorCodeSet(_pointer_to_result_or_error_code)) &&
           "The cast of the pointer to the internal error code set the MSB to 1, but it is "
           "reserved for the error code bit. This is inplausible and must not happen.");
    error_object.addReference();
}

template <typename T>
inline bool operator==(const ResultDescription<T>& lhs, const ResultDescription<T>& rhs)
{
    // we put the tests inside this function so it will be available for all specializations
    static_assert(sizeof(ResultDescription<T>) == 8, "Description type unsupported size!");

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
