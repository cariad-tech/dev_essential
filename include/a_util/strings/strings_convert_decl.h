/**
 * @file
 * Public API for @ref strings_convert_decl.h "string conversion" functions
 *
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */

#ifndef A_UTIL_UTIL_STRINGS_STRINGS_CONVERT_DECL_HEADER_INCLUDED
#define A_UTIL_UTIL_STRINGS_STRINGS_CONVERT_DECL_HEADER_INCLUDED

#include <string>

namespace a_util {
namespace strings {
/**
 * Safely convert a string to a boolean value.
 * @param[in] from String to convert. If either @c "true" or @c "false" (case insensitive),
                   evaluates to the corresponding boolean value. If numeric, implicit
                   conversion rules apply.
 * @param[out] to Boolean value converted from the string representation.
 * @retval true Conversion was successful, @c to contains the converted value.
 * @retval false Conversion failed, @c to is left untouched.
 */
bool toBool(const std::string& from, bool& to);

/**
 * Safely convert a string to a numeric value.
 * @param[in] from String to convert. If the potentially convertible numeric value is not in range
                   of @c decltype(to) numeric limits, the conversion fails.
 * @param[out] to Numeric value converted from the string representation.
 * @retval true Conversion was successful, @c to contains the converted value.
 * @retval false Conversion failed, @c to is left untouched.
 */
bool toInt8(const std::string& from, std::int8_t& to);
/// @copydoc toInt8(const std::string& from, std::int8_t& to)
bool toInt16(const std::string& from, std::int16_t& to);
/// @copydoc toInt8(const std::string& from, std::int8_t& to)
bool toInt32(const std::string& from, std::int32_t& to);
/// @copydoc toInt8(const std::string& from, std::int8_t& to)
bool toInt64(const std::string& from, std::int64_t& to);
/// @copydoc toInt8(const std::string& from, std::int8_t& to)
bool toUInt8(const std::string& from, std::uint8_t& to);
/// @copydoc toInt8(const std::string& from, std::int8_t& to)
bool toUInt16(const std::string& from, std::uint16_t& to);
/// @copydoc toInt8(const std::string& from, std::int8_t& to)
bool toUInt32(const std::string& from, std::uint32_t& to);
/// @copydoc toInt8(const std::string& from, std::int8_t& to)
bool toUInt64(const std::string& from, std::uint64_t& to);
/// @copydoc toInt8(const std::string& from, std::int8_t& to)
bool toFloat(const std::string& from, float& to);
/// @copydoc toInt8(const std::string& from, std::int8_t& to)
bool toDouble(const std::string& from, double& to);

/// @copydoc toBool(const std::string& from, bool& to)
bool toBool(const char* from, bool& to);
/// @copydoc toInt8(const std::string& from, std::int8_t& to)
bool toInt8(const char* from, std::int8_t& to);
/// @copydoc toInt8(const std::string& from, std::int8_t& to)
bool toInt16(const char* from, std::int16_t& to);
/// @copydoc toInt8(const std::string& from, std::int8_t& to)
bool toInt32(const char* from, std::int32_t& to);
/// @copydoc toInt8(const std::string& from, std::int8_t& to)
bool toInt64(const char* from, std::int64_t& to);
/// @copydoc toInt8(const std::string& from, std::int8_t& to)
bool toUInt8(const char* from, std::uint8_t& to);
/// @copydoc toInt8(const std::string& from, std::int8_t& to)
bool toUInt16(const char* from, std::uint16_t& to);
/// @copydoc toInt8(const std::string& from, std::int8_t& to)
bool toUInt32(const char* from, std::uint32_t& to);
/// @copydoc toInt8(const std::string& from, std::int8_t& to)
bool toUInt64(const char* from, std::uint64_t& to);
/// @copydoc toInt8(const std::string& from, std::int8_t& to)
bool toFloat(const char* from, float& to);
/// @copydoc toInt8(const std::string& from, std::int8_t& to)
bool toDouble(const char* from, double& to);

/**
 * Convert a string to a boolean value.
 * @param[in] from String to convert. If either @c "true" or @c "false" (case insensitive),
                   evaluates to the corresponding boolean value. If numeric, implicit
                   conversion rules apply.
 * @return Boolean representation of input string.
 */
bool toBool(const std::string& from);

/**
 * Convert a string to a numeric value.
 * @param[in] from String to convert. If the potentially convertible numeric value is not in range
                   of the return type numeric limits, the conversion fails.
 * @return Numeric value, 0 if out of range of return types numeric limits.
 */
std::int8_t toInt8(const std::string& from);
/// @copydoc toInt8(const std::string& from)
std::int16_t toInt16(const std::string& from);
/// @copydoc toInt8(const std::string& from)
std::int32_t toInt32(const std::string& from);
/// @copydoc toInt8(const std::string& from)
std::int64_t toInt64(const std::string& from);
/// @copydoc toInt8(const std::string& from)
std::uint8_t toUInt8(const std::string& from);
/// @copydoc toInt8(const std::string& from)
std::uint16_t toUInt16(const std::string& from);
/// @copydoc toInt8(const std::string& from)
std::uint32_t toUInt32(const std::string& from);
/// @copydoc toInt8(const std::string& from)
std::uint64_t toUInt64(const std::string& from);
/// @copydoc toInt8(const std::string& from)
float toFloat(const std::string& from);
/// @copydoc toInt8(const std::string& from)
double toDouble(const std::string& from);

/// @copydoc toBool(const std::string& from)
bool toBool(const char* from);
/// @copydoc toInt8(const std::string& from)
std::int8_t toInt8(const char* from);
/// @copydoc toInt8(const std::string& from)
std::int16_t toInt16(const char* from);
/// @copydoc toInt8(const std::string& from)
std::int32_t toInt32(const char* from);
/// @copydoc toInt8(const std::string& from)
std::int64_t toInt64(const char* from);
/// @copydoc toInt8(const std::string& from)
std::uint8_t toUInt8(const char* from);
/// @copydoc toInt8(const std::string& from)
std::uint16_t toUInt16(const char* from);
/// @copydoc toInt8(const std::string& from)
std::uint32_t toUInt32(const char* from);
/// @copydoc toInt8(const std::string& from)
std::uint64_t toUInt64(const char* from);
/// @copydoc toInt8(const std::string& from)
float toFloat(const char* from);
/// @copydoc toInt8(const std::string& from)
double toDouble(const char* from);

/**
 * Safe convenience string-to-numeric conversion methods for generic programming
 */
struct ToNumericConverter {
    /**
     * Safely convert a string to a boolean value.
     * @param[in] from String to convert. If either @c "true" or @c "false" (case insensitive),
                       evaluates to the corresponding boolean value. If numeric, implicit
                       conversion rules apply.
     * @param[out] to Numeric value converted from the string representation.
     * @retval true Conversion was successful, @c to contains the converted value.
     * @retval false Conversion failed, @c to is left untouched.
     */
    static bool convert(const char* from, bool& to);
    /**
     * Safely convert a string to a numeric value.
     * @param[in] from String to convert. If the potentially convertible numeric value is not in
                       range of @c decltype(to) numeric limits, the conversion fails.
     * @param[out] to Numeric value converted from the string representation.
     * @retval true Conversion was successful, @c to contains the converted value.
     * @retval false Conversion failed, @c to is left untouched.
     */
    static bool convert(const char* from, std::int8_t& to);
    /// @copydoc convert(const char* from, std::int8_t& to)
    static bool convert(const char* from, std::int16_t& to);
    /// @copydoc convert(const char* from, std::int8_t& to)
    static bool convert(const char* from, std::int32_t& to);
    /// @copydoc convert(const char* from, std::int8_t& to)
    static bool convert(const char* from, std::int64_t& to);
    /// @copydoc convert(const char* from, std::int8_t& to)
    static bool convert(const char* from, std::uint8_t& to);
    /// @copydoc convert(const char* from, std::int8_t& to)
    static bool convert(const char* from, std::uint16_t& to);
    /// @copydoc convert(const char* from, std::int8_t& to)
    static bool convert(const char* from, std::uint32_t& to);
    /// @copydoc convert(const char* from, std::int8_t& to)
    static bool convert(const char* from, std::uint64_t& to);
    /// @copydoc convert(const char* from, std::int8_t& to)
    static bool convert(const char* from, float& to);
    /// @copydoc convert(const char* from, std::int8_t& to)
    static bool convert(const char* from, double& to);
};

/**
 * Safely convert a string to a value of type @c Numeric in a generic manner.
 * @tparam Numeric The numeric type the string gets converted to.
 * @param[in] from String to convert. If the potentially convertible numeric value is not in
                   range of @c decltype(to) numeric limits, the conversion fails.
 * @param[out] to Numeric value converted from the string representation.
 * @retval true Conversion was successful, @c to contains the converted value.
 * @retval false Conversion failed, @c to is left untouched.
 */
template <typename Numeric>
bool toNumeric(const std::string& from, Numeric& to);

/// @copydoc toNumeric(const std::string& from, Numeric& to)
template <typename Numeric>
bool toNumeric(const char* from, Numeric& to);

/**
 * Convert a string to a numeric value in a generic manner.
 * @tparam Numeric The numeric type the string gets converted to.
 * @param[in] from String to convert. If the potentially convertible numeric value is not in range
                   of the return type numeric limits, the conversion fails.
 * @return Numeric value, 0 if out of range of return types numeric limits.
 */
template <typename Numeric>
Numeric toNumeric(const std::string& from);

/// @copydoc toNumeric(const std::string& from)
template <typename Numeric>
Numeric toNumeric(const char* from);

} // namespace strings
} // namespace a_util

/** @cond INTERNAL_DOCUMENTATION */
#include <a_util/strings/detail/strings_convert_impl.h>
/** @endcond */

#endif // A_UTIL_UTIL_STRINGS_STRINGS_CONVERT_DECL_HEADER_INCLUDED
