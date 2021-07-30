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

If it is not possible or desirable to put the notice in a particular file, then
You may include the notice in a location (such as a LICENSE file in a
relevant directory) where a recipient would be likely to look for such a notice.

You may add additional accurate notices of copyright ownership.
@endverbatim
 */

#ifndef A_UTIL_UTIL_RESULT_DETAIL_DESCRIPTION_DECL_HEADER_INCLUDED
#define A_UTIL_UTIL_RESULT_DETAIL_DESCRIPTION_DECL_HEADER_INCLUDED

#include "a_util/base/config.h"
#include "a_util/base/std/types.h"
#include "a_util/result/detail/reference_counted_object_intf.h"

namespace a_util {
namespace base {

#if !A_UTIL_ENABLE_COMPILER_CXX_VARIADIC_TEMPLATES

// forward declaration
template <typename, typename>
struct variadic;

#endif //! A_UTIL_ENABLE_COMPILER_CXX_VARIADIC_TEMPLATES

} // namespace base

namespace result {
namespace detail {

/**
 * @brief Traits type for the result description
 * @tparam DescriptionType The detailed description type which might be used to allocate space for
 *                         detailed error information
 */
template <typename DescriptionType>
struct ResultDescriptionTraits {
    typedef ReferenceCountedObjectInterface<DescriptionType> ReferenceCountedDescriptionType;

    /// The error code bit indicating whether only the error code (1) was set or not (0)
    static const std::uint64_t error_code_bit = (std::uint64_t)1 << 63;
    /// The error code bitmask to mask out the error code bit
    static const std::uint64_t error_code_bitmask = ~error_code_bit;

    /**
     * @brief Check whether the detailed description was allocated
     * @param[in] value To check whether a detailed description was allocated
     * @return @c true if detailed description was allocated (the error code bit is not set),
     *         @c false otherwise
     */
    static bool isDetailedDescriptionSet(const std::uint64_t value);

    /**
     * @brief Check whether only the error code was set
     * @param[in] value Pointer to the description type that is checked for the error code bit.
     * @return @c true if only the error code was set (the error code bit is set),
               @c false otherwise
     */
    static bool isErrorCodeSet(const std::uint64_t value);

private:
    /// Must not be constructed
    ResultDescriptionTraits(); // =delete;
};                             // struct ResultDescriptionTraits

/**
 * @brief 64 Bit error type has max 63 Bit large pointer type member variable
 *
 * [1|...|______________________up_to_63_____________________________]
 * [a| b |                          c                                ]
 * Regions:
 * @li @c a: Error code Bit - Indicates whether only the error code in region b was set
 *           (1 == yes, 0 == no)
 * @li @c b: padding data with unused pointer bits
 * @li @c c: up to 63 Bits - Either only the error code value or the pointer
 *           to the allocated error description (might be nil)
 *
 * @tparam DescriptionType The detailed description type which might be used to allocate space for
 *                         detailed error information.
 */
template <typename DescriptionType>
class ResultDescription {
    typedef ResultDescriptionTraits<DescriptionType> TraitsType; //!< Traits type abbreviation

public:
    typedef ResultDescription<DescriptionType> SelfType; //!< Self type

public:
    ResultDescription();
    ResultDescription(const ResultDescription& other);
    ResultDescription& operator=(const ResultDescription& other);
    ~ResultDescription();

#if A_UTIL_ENABLE_COMPILER_CXX_VARIADIC_TEMPLATES

    /**
     * @brief Allocate an object being able to store detailed error information
     * @details On common operating systems not the entire 64Bits of a pointer are used to address
     *          virtual memory - so there are some spare bits free to be used as user space.
     *          However, to safely mask out the appropriate bits, it is checked whether the pointer
     *          to the allocated detailed description object indeed does not use the user space
     *          bits. If so, the detailed description will not be available and only the error code
     *          is set.
     * @tparam Implementation The concrete implementation type to contain the detailed error
     *                        description. The first constructor argument must be of type @c
     *                        std::int32_t.
     * @tparam Args Constructor arguments for the allocation of type Implementation
     * @param[in] error_code The concrete error code either being used as first constructor
     *                      argument for type @c Implementation or if allocation fails as the
     *                      error code.
     * @param[in] args The constructor arguments for the error description object. These usually
     *                 include information like where did the error occur (line, file, ...) or the
     *                 error string.
     * @return New object of type @c ResultDescription
     */
    template <typename Implementation, typename... Args>
    static SelfType makeResultDescription(std::int32_t error_code, Args&&... args);

#else

    template <typename Implementation, typename T, typename Next>
    static SelfType makeResultDescription(std::int32_t error_code,
                                          const base::variadic<T, Next>& args);

#endif // A_UTIL_ENABLE_COMPILER_CXX_VARIADIC_TEMPLATES

    /**
     * @brief Make the result description object only with an error code
     * @param[in] error_code The error code to set
     * @return New object of type @c ResultDescription
     */
    static SelfType makeResultDescription(std::int32_t error_code);

    /**
     * @brief Get the detailed description if any was allocated
     * @return Pointer to the detailed error description - @c nullptr if none was allocated
     * @note If getErrorCode() and getDetailedDescription() return with indication of no error
     *       code, no error code was set
     */
    DescriptionType const* getDetailedDescription() const;

    /**
     * @brief Get the error code if only the error code was set
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

    ResultDescription(std::uint64_t error_code);
}; // struct ResultDescription

/**
 * @brief Compare two result description objects for equality
 * @tparam T Detailed description type
 * @param[in] lhs Left hand side result description object
 * @param[in] rhs Right hand side result description object
 * @return @c true if both result description objects compare equal, @c false otherwise
 */
template <typename T>
bool operator==(const ResultDescription<T>& lhs, const ResultDescription<T>& rhs);

/**
 * @brief Compare two result description objects for inequality
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

#endif // A_UTIL_UTIL_RESULT_DETAIL_DESCRIPTION_DECL_HEADER_INCLUDED
