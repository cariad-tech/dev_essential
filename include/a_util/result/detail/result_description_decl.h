/**
 * @file
 * Private API for @ref a_util::result::detail::ResultDescription "ResultDescription" type and
 * functions
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

#ifndef A_UTIL_UTIL_RESULT_DETAIL_DESCRIPTION_DECL_HEADER_INCLUDED
#define A_UTIL_UTIL_RESULT_DETAIL_DESCRIPTION_DECL_HEADER_INCLUDED

#include <a_util/result/detail/reference_counted_object_intf.h>
#include <a_util/result/result_info_decl.h>

#include <cstdint>

namespace a_util {
namespace result {
namespace detail {
/**
 * Traits type for the result description
 * @tparam DescriptionIntf The interface used to retrieve the error information
 */
template <typename DescriptionIntf /*e.g. a_util::result::detail::IErrorDescription*/>
struct ResultDescriptionTraits final {
    /// Reference counted error description type
    typedef ReferenceCountedObjectInterface<DescriptionIntf> ReferenceCountedDescriptionType;
    /// The error code bit indicating whether only the error code (1) was set or not (0)
    static const std::uint64_t error_code_bit = (std::uint64_t)1 << 63;
    /// The error code bitmask to mask out the error code bit
    static const std::uint64_t error_code_bitmask = ~error_code_bit;
    /// The bitmask to serialize the error code to its internal representation
    static const std::uint64_t error_code_serialize_bitmask = 0x00000000FFFFFFFF;
    /// The bitmask to deserialize the internal error representation to its original value
    static const std::uint64_t error_code_deserialize_bitmask = 0xFFFFFFFF00000000;

    /// Must not be constructed/copied/moved
    ResultDescriptionTraits() = delete;
    ResultDescriptionTraits(const ResultDescriptionTraits&) = delete;
    ResultDescriptionTraits& operator=(const ResultDescriptionTraits&) = delete;
    ResultDescriptionTraits(ResultDescriptionTraits&&) = delete;
    ResultDescriptionTraits& operator=(ResultDescriptionTraits&&) = delete;

    /**
     * Check whether the detailed description was allocated
     * @param[in] value To check whether a detailed description was allocated
     * @return @c true if detailed description was allocated (the error code bit is not set),
     *         @c false otherwise
     */
    static bool isDetailedDescriptionSet(std::uint64_t value);

    /**
     * Check whether only the error code was set
     * @param[in] value Pointer to the description type that is checked for the error code bit.
     * @return @c true if only the error code was set (the error code bit is set),
               @c false otherwise
     */
    static bool isErrorCodeSet(std::uint64_t value);

    /**
     * Get the internal error code representation from the external error code
     * @param[in] error_code The external error code
     * @return Internal error code representation of @c error_code
     */
    constexpr static std::uint64_t toInternalErrorCode(std::int32_t error_code) noexcept;

    /**
     * Get the external error code from the internal error code representation
     * @param[in] error_code The internal error code representation.
     * @return External error code
     */
    constexpr static std::int32_t toExternalErrorCode(std::uint64_t error_code) noexcept;

    /**
     * Get the internal error pointer representation from the internal error code representation
     * @param[in] error_code The internal error code representation
     * @return Pointer to the internal error description interface
     */
    constexpr static const ReferenceCountedDescriptionType* toInternalErrorPointer(
        std::uint64_t error_code) noexcept;

private:
    /// To implement various static checks
    ~ResultDescriptionTraits();
}; // struct ResultDescriptionTraits

/// Wrapper for either a pointer to a detailed description object or simply a numeric error code.
/**
 * 64 Bit error type has max 63 Bit large pointer type member variable
 * @verbatim
 [1|...|______________________up_to_63_____________________________]
 [a| b |                          c                                ]
   @endverbatim
 * Regions:
 * @li @c a: Error code Bit - Indicates whether only the error code in region b was set
 *           (1 == yes, 0 == no)
 * @li @c b: padding data with unused pointer bits
 * @li @c c: up to 63 Bits - Either only the error code value or the pointer
 *           to the allocated error description (might be nil)
 *
 * @tparam DescriptionIntf The interface used to retrieve the error information
 */
template <typename DescriptionIntf /*e.g. a_util::result::detail::IErrorDescription*/>
class ResultDescription {
    typedef ResultDescriptionTraits<DescriptionIntf> TraitsType; //!< Traits type abbreviation

public:
    typedef ResultDescription<DescriptionIntf> SelfType; //!< Self type

public:
    /// CTOR
    ResultDescription();
    /**
     * Copy constructor
     * @param[in] other Other object to copy from
     */
    ResultDescription(const ResultDescription& other);
    /**
     * Copy assignment operator
     * @param[in] other Other object to copy from
     * @return @c *this
     */
    ResultDescription& operator=(const ResultDescription& other);
    /**
     * Move CTOR
     * @param[in,out] other Move-from object *this is created from
     * @pre @c other is left in default-initialized state
     */
    ResultDescription(ResultDescription&& other);
    /**
     * Move assignment operator
     * @param[in,out] other Move-from object *this is assigned from
     * @pre @c other is left in default-initialized state
     */
    ResultDescription& operator=(ResultDescription&& other);

    /// DTOR
    ~ResultDescription();

    /**
     * Allocate an object being able to store detailed error information
     * @details On common operating systems not the entire 64Bits of a pointer are used to address
     *          virtual memory - so there are some spare bits free to be used as user space.
     *          However, to safely mask out the appropriate bits, it is checked whether the pointer
     *          to the allocated detailed description object indeed does not use the user space
     *          bits. If so, the detailed description will not be available and only the error code
     *          is set.
     * @tparam DescriptionImpl The concrete implementation type to contain the detailed error
     *                         description. The first constructor argument must be of type @c
     *                         std::int32_t.
     * @tparam Args Constructor arguments for the allocation of type Implementation
     * @param[in] error_code The concrete error code either being used as first constructor
     *                      argument for type @c Implementation or if allocation fails as the
     *                      error code.
     * @param[in] args The constructor arguments for the error description object. These usually
     *                 include information like where did the error occur (line, file, ...) or the
     *                 error string.
     * @return Object of type @c ResultDescription containing heap allocated error information.
     */
    template <typename DescriptionImpl, typename... Args>
    static SelfType makeResultDescription(std::int32_t error_code, Args&&... args);

    /**
     * Creates a static error object referencing error code and error label information of type @c T
     *
     * @tparam T Type for template parameter of @ref a_util::result::ResultInfo<T>
     * @param[in] result_info Unused, provided only for type deduction.
     * @return Object of type @c ResultDescription containing static error code and error label
     *         information.
     */
    template <typename T>
    static SelfType makeResultDescription(
        const ::a_util::result::ResultInfo<T>& result_info) noexcept;

    /**
     * Make the result description object only with an error code
     * @param[in] error_code The error code to set
     * @return New object of type @c ResultDescription
     */
    static SelfType makeResultDescription(std::int32_t error_code);

    /**
     * Get the detailed description if any was allocated
     * @return Pointer to the detailed error description - @c nullptr if none was allocated
     * @note If getErrorCode() and getDetailedDescription() return with indication of no error
     *       code, no error code was set
     */
    DescriptionIntf const* getDetailedDescription() const;

    /**
     * Get the error code if only the error code was set
     * @return The error code
     * @note If getErrorCode() and getDetailedDescription() return with indication of no error
     *       code, no error code was set
     */
    std::int32_t getErrorCode() const;

private:
    // On all systems, a 64Bit variable for storage
    // the most significant bits are used for user flags
    // size of pointer part is system dependent, lower bits
    std::uint64_t _pointer_to_result_or_error_code;

    /**
     * Construct with simple error code, no memory is allocated.
     * @param[in] error_code The error code
     */
    constexpr ResultDescription(std::int32_t error_code) noexcept;

    /**
     * Construct with detailed error description
     * @param[in] error_object Reference to error object getting cast to internal error variable
     * @post @c error_object reference counter is incremented
     */
    constexpr ResultDescription(
        const ReferenceCountedObjectInterface<DescriptionIntf>& error_object) noexcept;
}; // struct ResultDescription

/**
 * Compare two result description objects for equality
 * @tparam T Detailed description type
 * @param[in] lhs Left hand side result description object
 * @param[in] rhs Right hand side result description object
 * @return @c true if both result description objects compare equal, @c false otherwise
 */
template <typename T>
bool operator==(const ResultDescription<T>& lhs, const ResultDescription<T>& rhs);

/**
 * Compare two result description objects for inequality
 * @tparam T Detailed description type
 * @param[in] lhs Left hand side result description object
 * @param[in] rhs Right hand side result description object
 * @return @c true if both result description objects compare not equal, @c false otherwise
 */
template <typename T>
bool operator!=(const ResultDescription<T>& lhs, const ResultDescription<T>& rhs);

} // namespace detail
} // namespace result
} // namespace a_util

/** @cond INTERNAL_DOCUMENTATION */
#include <a_util/result/detail/result_description_impl.h>
/** @endcond */

#endif // A_UTIL_UTIL_RESULT_DETAIL_DESCRIPTION_DECL_HEADER_INCLUDED
