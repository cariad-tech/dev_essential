/**
 * @file
 * Public API for @ref strings_numeric_traits_decl.h "string numeric trait" functions
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

#ifndef A_UTIL_UTIL_STRINGS_STRINGS_NUMERIC_TRAITS_DECL_HEADER_INCLUDED
#define A_UTIL_UTIL_STRINGS_STRINGS_NUMERIC_TRAITS_DECL_HEADER_INCLUDED

#include <string> //std::string

namespace a_util {
namespace strings {
/**
 * Check whether a string can be converted to a boolean type.
 * @param[in] str The string to check.
 * @return @c true if the string can be converted, @c false otherwise.
 */
bool isBool(const std::string& str);
/**
 * Check whether a string can be converted to a value of the given numeric type.
 * @param[in] str The string to check.
 * @return @c true if the string can be converted, @c false otherwise.
 */
bool isInt8(const std::string& str);
/// @copydoc isInt8(const std::string& str);
bool isInt16(const std::string& str);
/// @copydoc isInt8(const std::string& str);
bool isInt32(const std::string& str);
/// @copydoc isInt8(const std::string& str);
bool isInt64(const std::string& str);
/// @copydoc isInt8(const std::string& str);
bool isUInt8(const std::string& str);
/// @copydoc isInt8(const std::string& str);
bool isUInt16(const std::string& str);
/// @copydoc isInt8(const std::string& str);
bool isUInt32(const std::string& str);
/// @copydoc isInt8(const std::string& str);
bool isUInt64(const std::string& str);
/// @copydoc isInt8(const std::string& str);
bool isFloat(const std::string& str);
/// @copydoc isInt8(const std::string& str);
bool isDouble(const std::string& str);

/// @copydoc isBool(const std::string& str)
bool isBool(const char* str);
/// @copydoc isInt8(const std::string& str)
bool isInt8(const char* str);
/// @copydoc isInt8(const std::string& str)
bool isInt16(const char* str);
/// @copydoc isInt8(const std::string& str)
bool isInt32(const char* str);
/// @copydoc isInt8(const std::string& str)
bool isInt64(const char* str);
/// @copydoc isInt8(const std::string& str)
bool isUInt8(const char* str);
/// @copydoc isInt8(const std::string& str)
bool isUInt16(const char* str);
/// @copydoc isInt8(const std::string& str)
bool isUInt32(const char* str);
/// @copydoc isInt8(const std::string& str)
bool isUInt64(const char* str);
/// @copydoc isInt8(const std::string& str)
bool isFloat(const char* str);
/// @copydoc isInt8(const std::string& str)
bool isDouble(const char* str);

/**
 * Convenience methods to validate string-to-numeric conversion for generic programming
 */
struct IsNumericConvertible {
    /**
     * Check whether a conversion of string @c from to a boolean type is possible
     * @note The second parameter is only for automatic overloading resolution,
     *       its value is ignored.
     * @param[in] from String to validate the conversion on.
     * @return @c true if conversion is possible, false otherwise.
     */
    static bool check(const char* from, const bool&);
    /**
     * Check whether a conversion of string @c from to the type of the second parameter is possible
     * @note The second parameter is only for automatic overloading resolution,
     *       its value is ignored.If the potentially convertible numeric value is not in
             range of the second parameters numeric limits, the check fails.
     * @param[in] from String to validate the conversion on.
     * @return @c true if conversion is possible, false otherwise.
     */
    static bool check(const char* from, const std::int8_t&);
    /// @copydoc check(const char* from, const std::int8_t&)
    static bool check(const char* from, const std::int16_t&);
    /// @copydoc check(const char* from, const std::int8_t&)
    static bool check(const char* from, const std::int32_t&);
    /// @copydoc check(const char* from, const std::int8_t&)
    static bool check(const char* from, const std::int64_t&);
    /// @copydoc check(const char* from, const std::int8_t&)
    static bool check(const char* from, const std::uint8_t&);
    /// @copydoc check(const char* from, const std::int8_t&)
    static bool check(const char* from, const std::uint16_t&);
    /// @copydoc check(const char* from, const std::int8_t&)
    static bool check(const char* from, const std::uint32_t&);
    /// @copydoc check(const char* from, const std::int8_t&)
    static bool check(const char* from, const std::uint64_t&);
    /// @copydoc check(const char* from, const std::int8_t&)
    static bool check(const char* from, const float&);
    /// @copydoc check(const char* from, const std::int8_t&)
    static bool check(const char* from, const double&);
};

/**
 * Check whether a string is convertible to a value of type @c Numeric for generic programming.
 * @tparam Numeric The numeric type.
 * @param[in] str The string to check.
 * @return @c true if valid numeric value, @c false otherwise.
 */
template <typename Numeric>
bool isNumeric(const std::string& str);

/// @copydoc isNumeric(const std::string& str)
template <typename Numeric>
bool isNumeric(const char* str);

} // namespace strings
} // namespace a_util

/** @cond INTERNAL_DOCUMENTATION */
#include <a_util/strings/detail/strings_numeric_traits_impl.h>
/** @endcond */

#endif // A_UTIL_UTIL_STRINGS_STRINGS_NUMERIC_TRAITS_DECL_HEADER_INCLUDED
