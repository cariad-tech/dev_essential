/**
 * @file
 * Class and function templates extending std type traits functionality
 *
 * @copyright
 * @verbatim
Copyright @ 2022 VW Group. All rights reserved.

    This Source Code Form is subject to the terms of the Mozilla
    Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

If it is not possible or desirable to put the notice in a particular file, then
You may include the notice in a location (such as a LICENSE file in a
relevant directory) where a recipient would be likely to look for such a notice.

You may add additional accurate notices of copyright ownership.
@endverbatim
 */

#ifndef A_UTIL_UTIL_BASE_TYPE_TRAITS_H
#define A_UTIL_UTIL_BASE_TYPE_TRAITS_H

#include <type_traits>

#if defined(__GNUC__) && (__GNUC__ == 5) && defined(__QNX__)
#pragma GCC diagnostic ignored                                                                     \
    "-Wattributes" // standard type attributes are ignored when used in templates
#endif

#if (defined(__GNUC__) && (__GNUC__ < 7)) || (defined(_MSC_VER) && (_MSC_VER < 1920))
namespace std {
/**
 * @brief This is a C++ 17 void type convenient class
 *
 * @tparam Any type
 */
template <typename...>
using void_t = void;
} // namespace std
#endif // (defined(__GNUC__) && (__GNUC__ < 7)) || (defined(_MSC_VER) && (_MSC_VER < 1920))

#if (defined(__GNUC__) && (__GNUC__ < 7))
namespace std {
/**
 * C++17 backport of std::is_enum_v
 */
template <typename T>
constexpr bool is_enum_v = is_enum<T>::value;
} // namespace std
#endif // (defined(__GNUC__) && (__GNUC__ < 7))

namespace a_util {
/**
 * @addtogroup a_util_core
 * @{
 */

/**
 * @brief Retrieves the underlying type if @p T is an enum, otherwise the type itself.
 *
 * @tparam T The type tor retrieve the underlying type from
 * @tparam is_enum value to determine if @p T is an enum
 */
template <typename T, bool = std::is_enum_v<T>>
struct underlying_type_or_type {
    /**
     * @brief The type of the underlying type retrieved
     */
    using type = T;
};

/**
 * @brief Retrieves the underlying type of the enum type @p T
 *
 * @tparam T the enum type
 */
template <typename T>
struct underlying_type_or_type<T, true> {
    /**
     * @brief The type of the underlying type retrieved
     */
    using type = std::underlying_type_t<T>;
};

/**
 * @brief Retrieves the underlying type if @p T is an enum, otherwise the type itself.
 *
 * @tparam T The type tor retrieve the underlying type from
 * @tparam is_enum value to determine if @p T is an enum
 */
template <class T, bool IsEnum = std::is_enum_v<T>>
using underlying_type_or_type_t = typename underlying_type_or_type<T, IsEnum>::type;

namespace detail {

// taken from https://stackoverflow.com/a/71563534
template <class T, class U, class = void>
struct is_explicitly_convertible_to_impl : std::false_type {
};

// taken from https://stackoverflow.com/a/71563534
template <class T, class U>
struct is_explicitly_convertible_to_impl<T,
                                         U,
                                         std::void_t<decltype(static_cast<U>(std::declval<T>()))>>
    : std::true_type {
};

} // namespace detail

// taken from https://stackoverflow.com/a/71563534
template <class T, class U>
struct is_explicitly_convertible_to : detail::is_explicitly_convertible_to_impl<T, U> {
};

/**
 * @brief Checks if there is an explicitly defined conversion from @p U to @p T
 * @details taken from https://stackoverflow.com/a/71563534
 * @tparam T The to type
 * @tparam U The from type
 */
template <class T, class U>
constexpr bool is_explicitly_convertible_to_v = is_explicitly_convertible_to<T, U>::value;

} // namespace a_util
/** @} */

#if defined(__GNUC__) && (__GNUC__ == 5) && defined(__QNX__)
#pragma GCC diagnostic warning                                                                     \
    "-Wattributes" // standard type attributes are ignored when used in templates
#endif

#endif /* A_UTIL_UTIL_BASE_TYPE_TRAITS_H */
