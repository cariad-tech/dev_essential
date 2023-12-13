/**
 * @file
 * Public API for @ref a_util::variant::Variant "Variant" class and functions
 *
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */

#ifndef _A_UTILS_UTIL_VARIANT_VARIANT_HEADER_INCLUDED_
#define _A_UTILS_UTIL_VARIANT_VARIANT_HEADER_INCLUDED_

#include <memory>
#include <string>

namespace a_util {
namespace variant {
/// Enumeration of supported Variant types
typedef enum {
    VT_Empty = 0,           //!< Variant type is empty
    VT_Bool = 1,            //!< Variant type is bool
    VT_Int8 = 2,            //!< Variant type is std::int8_t
    VT_UInt8 = 3,           //!< Variant type is std::uint8_t
    VT_Int16 = 4,           //!< Variant type is std::int16_t
    VT_UInt16 = 5,          //!< Variant type is std::uint16_t
    VT_Int32 = 6,           //!< Variant type is std::int32_t
    VT_UInt32 = 7,          //!< Variant type is std::uint32_t
    VT_Int64 = 8,           //!< Variant type is std::int64_t
    VT_UInt64 = 9,          //!< Variant type is std::uint64_t
    VT_Float = 10,          //!< Variant type is float
    VT_Double = 11,         //!< Variant type is double
    VT_String = 12,         //!< Variant type is const char*
    VT_Float32 = VT_Float,  //!< Variant type is float
    VT_Float64 = VT_Double, //!< Variant type is double
} VariantType;

/**
 * Container type, able to store any primitive data type (and arrays thereof)
 */
class Variant {
public:
    /// CTOR: @ref VT_Empty
    Variant();
    /**
     * CTOR: @ref VT_Bool
     * @param[in] value Value of the variant
     */
    Variant(bool value);
    /**
     * CTOR: @ref VT_Int8
     * @param[in] value Value of the variant
     */
    Variant(std::int8_t value);
    /**
     * CTOR: @ref VT_UInt8
     * @param[in] value Value of the variant
     */
    Variant(std::uint8_t value);
    /**
     * CTOR: @ref VT_Int16
     * @param[in] value Value of the variant
     */
    Variant(std::int16_t value);
    /**
     * CTOR: @ref VT_UInt16
     * @param[in] value Value of the variant
     */
    Variant(std::uint16_t value);
    /**
     * CTOR: @ref VT_Int32
     * @param[in] value Value of the variant
     */
    Variant(std::int32_t value);
    /**
     * CTOR: @ref VT_UInt32
     * @param[in] value Value of the variant
     */
    Variant(std::uint32_t value);
    /**
     * CTOR: @ref VT_Int64
     * @param[in] value Value of the variant
     */
    Variant(std::int64_t value);
    /**
     * CTOR: @ref VT_UInt64
     * @param[in] value Value of the variant
     */
    Variant(std::uint64_t value);
    /**
     * CTOR: @ref VT_Float32
     * @param[in] value Value of the variant
     */
    Variant(float value);
    /**
     * CTOR: @ref VT_Float64
     * @param[in] value Value of the variant
     */
    Variant(double value);
    /**
     * CTOR: @ref VT_String
     * @param[in] value Value of the variant
     */
    Variant(const char* value);

    /**
     * Copy CTOR
     * @param[in] other Other variant object to copy construct *this from
     */
    Variant(const Variant& other);

    /**
     * Assignment operator
     * @param[in] other Other object to assign *this from
     * @return *this
     */
    Variant& operator=(const Variant& other);

    /// Non-virtual DTOR
    ~Variant();

    /**
     * Move CTOR
     * @param[in,out] other Moved-from object *this is constructed from. Left in a valid but
     *                      unspecified state. Should not be used after this move ctor returns
     */
    Variant(Variant&& other) noexcept;

    /**
     * Move assignment
     * @param[in,out] other Moved-from object *this is assigned from. Left in a valid but
     *                      unspecified state. Should not be used after this assignment operator
     *                      returns.
     * @retval *this
     */
    Variant& operator=(Variant&& other) noexcept;

    /// Returns the current underlying data type of the instance
    VariantType getType() const;

    /**
     * Get the current array size of the instance
     * @note For @ref VT_String the array size is always 0
     */
    std::size_t getArraySize() const;

    /// Returns whether the instance stores an array of any kind
    bool isArray() const;

    /// Returns whether the instance is in the empty state (@ref VT_Empty)
    bool isEmpty() const;

    /// Resets the instance to @ref VT_Empty
    void reset();

    /**
     * Resets the instance to @ref VT_Bool
     * @param[in] value Value of the variant
     */
    void reset(bool value);
    /**
     * Resets the instance to @ref VT_Int8
     * @param[in] value Value of the variant
     */
    void reset(std::int8_t value);
    /**
     * Resets the instance to @ref VT_UInt8
     * @param[in] value Value of the variant
     */
    void reset(std::uint8_t value);
    /**
     * Resets the instance to @ref VT_Int16
     * @param[in] value Value of the variant
     */
    void reset(std::int16_t value);
    /**
     * Resets the instance to @ref VT_UInt16
     * @param[in] value Value of the variant
     */
    void reset(std::uint16_t value);
    /**
     * Resets the instance to @ref VT_Int32
     * @param[in] value Value of the variant
     */
    void reset(std::int32_t value);
    /**
     * Resets the instance to @ref VT_UInt32
     * @param[in] value Value of the variant
     */
    void reset(std::uint32_t value);
    /**
     * Resets the instance to @ref VT_Int64
     * @param[in] value Value of the variant
     */
    void reset(std::int64_t value);
    /**
     * Resets the instance to @ref VT_UInt64
     * @param[in] value Value of the variant
     */
    void reset(std::uint64_t value);
    /**
     * Resets the instance to @ref VT_Float
     * @param[in] value Value of the variant
     */
    void reset(float value);
    /**
     * Resets the instance to @ref VT_Double
     * @param[in] value Value of the variant
     */
    void reset(double value);
    /**
     * Resets the instance to @ref VT_String
     * @param[in] value Value of the variant
     */
    void reset(const char* value);

    /**
     * Resets the instance to an array of @ref VT_Bool
     * @param[in] array_storage Pointer to begin of the array
     * @param[in] array_size Size of the array @c array_storage points to
     * @throw std::invalid_argument if @c either array_storage is nullptr or @c array_size is zero
     */
    void reset(const bool* array_storage, std::size_t array_size);
    /**
     * Resets the instance to an array of @ref VT_Int8
     * @param[in] array_storage Pointer to begin of the array
     * @param[in] array_size Size of the array @c array_storage points to
     * @throw std::invalid_argument if @c either array_storage is nullptr or @c array_size is zero
     */
    void reset(const std::int8_t* array_storage, std::size_t array_size);
    /**
     * Resets the instance to an array of @ref VT_UInt8
     * @param[in] array_storage Pointer to begin of the array
     * @param[in] array_size Size of the array @c array_storage points to
     * @throw std::invalid_argument if @c either array_storage is nullptr or @c array_size is zero
     */
    void reset(const std::uint8_t* array_storage, std::size_t array_size);
    /**
     * Resets the instance to an array of @ref VT_Int16
     * @param[in] array_storage Pointer to begin of the array
     * @param[in] array_size Size of the array @c array_storage points to
     * @throw std::invalid_argument if @c either array_storage is nullptr or @c array_size is zero
     */
    void reset(const std::int16_t* array_storage, std::size_t array_size);
    /**
     * Resets the instance to an array of @ref VT_UInt16
     * @param[in] array_storage Pointer to begin of the array
     * @param[in] array_size Size of the array @c array_storage points to
     * @throw std::invalid_argument if @c either array_storage is nullptr or @c array_size is zero
     */
    void reset(const std::uint16_t* array_storage, std::size_t array_size);
    /**
     * Resets the instance to an array of @ref VT_Int32
     * @param[in] array_storage Pointer to begin of the array
     * @param[in] array_size Size of the array @c array_storage points to
     * @throw std::invalid_argument if @c either array_storage is nullptr or @c array_size is zero
     */
    void reset(const std::int32_t* array_storage, std::size_t array_size);
    /**
     * Resets the instance to an array of @ref VT_UInt32
     * @param[in] array_storage Pointer to begin of the array
     * @param[in] array_size Size of the array @c array_storage points to
     * @throw std::invalid_argument if @c either array_storage is nullptr or @c array_size is zero
     */
    void reset(const std::uint32_t* array_storage, std::size_t array_size);
    /**
     * Resets the instance to an array of @ref VT_Int64
     * @param[in] array_storage Pointer to begin of the array
     * @param[in] array_size Size of the array @c array_storage points to
     * @throw std::invalid_argument if @c either array_storage is nullptr or @c array_size is zero
     */
    void reset(const std::int64_t* array_storage, std::size_t array_size);
    /**
     * Resets the instance to an array of @ref VT_UInt64
     * @param[in] array_storage Pointer to begin of the array
     * @param[in] array_size Size of the array @c array_storage points to
     * @throw std::invalid_argument if @c either array_storage is nullptr or @c array_size is zero
     */
    void reset(const std::uint64_t* array_storage, std::size_t array_size);
    /**
     * Resets the instance to an array of @ref VT_Float
     * @param[in] array_storage Pointer to begin of the array
     * @param[in] array_size Size of the array @c array_storage points to
     * @throw std::invalid_argument if @c either array_storage is nullptr or @c array_size is zero
     */
    void reset(const float* array_storage, std::size_t array_size);
    /**
     * Resets the instance to an array of @ref VT_Double
     * @param[in] array_storage Pointer to begin of the array
     * @param[in] array_size Size of the array @c array_storage points to
     * @throw std::invalid_argument if @c either array_storage is nullptr or @c array_size is zero
     */
    void reset(const double* array_storage, std::size_t array_size);

    /**
     * Get current value or alternatively from an array index of the variants value
     * @param[in] array_index Index of the value to retrieve from the array (default: 0).
     *                        Must be zero, if variant is not an array.
     * @return The value
     * @throw std::runtime_error If variant type is different to the requested type
     * @throw std::out_of_range If @c array_index is out of range or the variant is not an array
     */
    bool getBool(std::size_t array_index = 0) const;
    /// @copydoc getBool()
    std::int8_t getInt8(std::size_t array_index = 0) const;
    /// @copydoc getBool()
    std::uint8_t getUInt8(std::size_t array_index = 0) const;
    /// @copydoc getBool()
    std::int16_t getInt16(std::size_t array_index = 0) const;
    /// @copydoc getBool()
    std::uint16_t getUInt16(std::size_t array_index = 0) const;
    /// @copydoc getBool()
    std::int32_t getInt32(std::size_t array_index = 0) const;
    /// @copydoc getBool()
    std::uint32_t getUInt32(std::size_t array_index = 0) const;
    /// @copydoc getBool()
    std::int64_t getInt64(std::size_t array_index = 0) const;
    /// @copydoc getBool()
    std::uint64_t getUInt64(std::size_t array_index = 0) const;
    /// @copydoc getBool()
    float getFloat(std::size_t array_index = 0) const;
    /// @copydoc getBool()
    double getDouble(std::size_t array_index = 0) const;
    /// @copydoc getBool()
    float getFloat32(std::size_t array_index = 0) const;
    /// @copydoc getBool()
    double getFloat64(std::size_t array_index = 0) const;
    /**
     * Get current string value
     * @return The value
     * @throw std::runtime_error If variant type is different to the requested type
     */
    const char* getString() const;

    /**
     * Convert the value of the variant
     * @return The value after conversion to the new type
     * @throw std::runtime_error If variant is either @ref VT_Empty or the type is unknown
     */
    bool asBool() const;
    /// @copydoc asBool()
    std::int8_t asInt8() const;
    /// @copydoc asBool()
    std::uint8_t asUInt8() const;
    /// @copydoc asBool()
    std::int16_t asInt16() const;
    /// @copydoc asBool()
    std::uint16_t asUInt16() const;
    /// @copydoc asBool()
    std::int32_t asInt32() const;
    /// @copydoc asBool()
    std::uint32_t asUInt32() const;
    /// @copydoc asBool()
    std::int64_t asInt64() const;
    /// @copydoc asBool()
    std::uint64_t asUInt64() const;
    /// @copydoc asBool()
    float asFloat() const;
    /// @copydoc asBool()
    double asDouble() const;
    /// @copydoc asBool()
    std::string asString() const;

    /**
     * Explicit conversion of the value of the variant to @ref VT_Bool
     * @return The value after conversion to the new type
     * @throw std::runtime_error If variant is either @ref VT_Empty or the type is unknown
     */
    operator bool() const;
    /**
     * Explicit conversion of the value of the variant to @ref VT_Int8
     * @return The value after conversion to the new type
     * @throw std::runtime_error If variant is either @ref VT_Empty or the type is unknown
     */
    operator std::int8_t() const;
    /**
     * Explicit conversion of the value of the variant to @ref VT_UInt8
     * @return The value after conversion to the new type
     * @throw std::runtime_error If variant is either @ref VT_Empty or the type is unknown
     */
    operator std::uint8_t() const;
    /**
     * Explicit conversion of the value of the variant to @ref VT_Int16
     * @return The value after conversion to the new type
     * @throw std::runtime_error If variant is either @ref VT_Empty or the type is unknown
     */
    operator std::int16_t() const;
    /**
     * Explicit conversion of the value of the variant to @ref VT_UInt16
     * @return The value after conversion to the new type
     * @throw std::runtime_error If variant is either @ref VT_Empty or the type is unknown
     */
    operator std::uint16_t() const;
    /**
     * Explicit conversion of the value of the variant to @ref VT_Int32
     * @return The value after conversion to the new type
     * @throw std::runtime_error If variant is either @ref VT_Empty or the type is unknown
     */
    operator std::int32_t() const;
    /**
     * Explicit conversion of the value of the variant to @ref VT_UInt32
     * @return The value after conversion to the new type
     * @throw std::runtime_error If variant is either @ref VT_Empty or the type is unknown
     */
    operator std::uint32_t() const;
    /**
     * Explicit conversion of the value of the variant to @ref VT_Int64
     * @return The value after conversion to the new type
     * @throw std::runtime_error If variant is either @ref VT_Empty or the type is unknown
     */
    operator std::int64_t() const;
    /**
     * Explicit conversion of the value of the variant to @ref VT_UInt64
     * @return The value after conversion to the new type
     * @throw std::runtime_error If variant is either @ref VT_Empty or the type is unknown
     */
    operator std::uint64_t() const;
    /**
     * Explicit conversion of the value of the variant to @ref VT_Float
     * @return The value after conversion to the new type
     * @throw std::runtime_error If variant is either @ref VT_Empty or the type is unknown
     */
    operator float() const;
    /**
     * Explicit conversion of the value of the variant to @ref VT_Double
     * @return The value after conversion to the new type
     * @throw std::runtime_error If variant is either @ref VT_Empty or the type is unknown
     */
    operator double() const;
    /**
     * Explicit conversion of the value of the variant to @ref VT_String
     * @return The value after conversion to the new type
     * @throw std::runtime_error If variant is either @ref VT_Empty or the type is unknown
     */
    operator std::string() const;

    /**
     * Assignment operator for implicit type conversion to @ref VT_Bool
     * @param[in] value Other value to assign *this from
     * @return *this
     */
    Variant& operator=(bool value);
    /**
     * Assignment operator for implicit type conversion to @ref VT_Int8
     * @param[in] value Other value to assign *this from
     * @return *this
     */
    Variant& operator=(std::int8_t value);
    /**
     * Assignment operator for implicit type conversion to @ref VT_UInt8
     * @param[in] value Other value to assign *this from
     * @return *this
     */
    Variant& operator=(std::uint8_t value);
    /**
     * Assignment operator for implicit type conversion to @ref VT_Int16
     * @param[in] value Other value to assign *this from
     * @return *this
     */
    Variant& operator=(std::int16_t value);
    /**
     * Assignment operator for implicit type conversion to @ref VT_UInt16
     * @param[in] value Other value to assign *this from
     * @return *this
     */
    Variant& operator=(std::uint16_t value);
    /**
     * Assignment operator for implicit type conversion to @ref VT_Int32
     * @param[in] value Other value to assign *this from
     * @return *this
     */
    Variant& operator=(std::int32_t value);
    /**
     * Assignment operator for implicit type conversion to @ref VT_UInt32
     * @param[in] value Other value to assign *this from
     * @return *this
     */
    Variant& operator=(std::uint32_t value);
    /**
     * Assignment operator for implicit type conversion to @ref VT_Int64
     * @param[in] value Other value to assign *this from
     * @return *this
     */
    Variant& operator=(std::int64_t value);
    /**
     * Assignment operator for implicit type conversion to @ref VT_UInt64
     * @param[in] value Other value to assign *this from
     * @return *this
     */
    Variant& operator=(std::uint64_t value);
    /**
     * Assignment operator for implicit type conversion to @ref VT_Float
     * @param[in] value Other value to assign *this from
     * @return *this
     */
    Variant& operator=(float value);
    /**
     * Assignment operator for implicit type conversion to @ref VT_Double
     * @param[in] value Other value to assign *this from
     * @return *this
     */
    Variant& operator=(double value);
    /**
     * Assignment operator for implicit type conversion to @ref VT_String
     * @param[in] value Other value to assign *this from
     * @return *this
     */
    Variant& operator=(const char* value);

    /**
     * Compare for equality
     * @param[in] lhs Left hand side operand
     * @param[in] rhs Right hand side operand
     * @return @c true if both are equal, @c false otherwise
     */
    friend bool operator==(const Variant& lhs, const Variant& rhs);

private:
    class Implementation;
    std::unique_ptr<Implementation> _impl;
};

/**
 * Compare for inequality
 * @param[in] lhs Left hand side operand
 * @param[in] rhs Right hand side operand
 * @return @c true if both are not equal, @c false otherwise
 */
bool operator!=(const Variant& lhs, const Variant& rhs);

} // namespace variant
} // namespace a_util

#endif // _A_UTILS_UTIL_VARIANT_VARIANT_HEADER_INCLUDED_
