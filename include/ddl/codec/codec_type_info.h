/**
 * @file
 * Implementation of the CodecTypeInfoo and CodecConstantInfo.
 *
 * @copyright
 * @verbatim
Copyright @ 2022 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */

#ifndef DDL_CODEC_TYPE_INFO_CLASS_HEADER
#define DDL_CODEC_TYPE_INFO_CLASS_HEADER

#include <ddl/codec/legacy/struct_element.h>

#include <vector>

namespace ddl {
namespace codec {

/**
 * @brief The element type of the value.
 *
 */
enum class ElementType {
    cet_empty = a_util::variant::VariantType::VT_Empty,   //!< Variant type is empty
    cet_bool = a_util::variant::VariantType::VT_Bool,     //!< Variant type is bool
    cet_int8 = a_util::variant::VariantType::VT_Int8,     //!< Variant type is std::int8_t
    cet_uint8 = a_util::variant::VariantType::VT_UInt8,   //!< Variant type is std::uint8_t
    cet_int16 = a_util::variant::VariantType::VT_Int16,   //!< Variant type is std::int16_t
    cet_uint16 = a_util::variant::VariantType::VT_UInt16, //!< Variant type is std::uint16_t
    cet_int32 = a_util::variant::VariantType::VT_Int32,   //!< Variant type is std::int32_t
    cet_uint32 = a_util::variant::VariantType::VT_UInt32, //!< Variant type is std::uint32_t
    cet_int64 = a_util::variant::VariantType::VT_Int64,   //!< Variant type is std::int64_t
    cet_uint64 = a_util::variant::VariantType::VT_UInt64, //!< Variant type is std::uint64_t
    cet_float = a_util::variant::VariantType::VT_Float,   //!< Variant type is float
    cet_double = a_util::variant::VariantType::VT_Double, //!< Variant type is double
    cet_sub_codec = cet_double + 1,   //!< type marks a subcodec/substructure with children
    cet_user_type = cet_sub_codec + 1 //!< type marks a user defined type
};

/**
 * @brief This is the legacy Access Type for resolving enum names.
 */
using AccessEnumType = ddl::AccessEnumType;

// forward declaration
class StructAccess;

/**
 * @brief Description of the elements Type
 * \li in case of "POD": getType() and getTypeName() are set
 * \li in case of "Enum": getType(), getTypeName() and the getEnumElementNames() are set
 * \li in case of "Struct": getType() and getTypeName() are set
 */
class CodecTypeInfo {
public:
    /**
     * @brief Get the Type
     * @return ElementType
     */
    ElementType getType() const;
    /**
     * @brief Get the typename
     * @remark The pointer of the return value is valid as long as the codec factory created this
     * CodecTypeInfo.
     * @return const char*
     */
    const char* getTypeName() const;
    /**
     * @brief Determines the value is an enum or not.
     * @remark the type (@ref getType) is set to the enums base type defines in ddl.
     *
     * @return true is an enum type
     * @return false in not an enum type
     */
    bool isEnum() const;
    /**
     * @brief Get the Enum Element Names
     *
     * @return std::vector<std::string>
     */
    std::vector<std::string> getEnumElementNames() const;
    /**
     * @brief Determines if the enum element name exists
     *
     * @param name The name of the enum element
     * @return true the name exists
     * @return false the name does not exist
     */
    bool hasEnumElementName(const std::string& name) const;
    /**
     * Returns the current enum elements value of the given enum element name converted to type T if
     * supported.
     * @param[in] name The name of the enum element value to retrieve.
     * @tparam T the type to retrieve the value in
     * @return The value in converted to type T
     * @throw throws std::runtime_error if not found.
     */
    a_util::variant::Variant getEnumElementValue(const std::string& name) const;
    /**
     * Returns the enum elements name for the given value if exists.
     * @param[in] value The value retrieve the for the value of type T.
     * @remark The pointer of the return value is valid as long as the codec factory created this
     * CodecTypeInfo.
     * @return const char* Returns the enum elements name
     * @throw throws std::runtime_error if not found.
     */
    const char* getEnumElementName(const a_util::variant::Variant& value) const;
    /**
     * @brief Legacy Only: Get the Legacy Access Enum Type object
     * @return const AccessEnumType*
     */
    const AccessEnumType* getLegacyAccessEnumType() const;

private:
    friend class StructAccess;
    CodecTypeInfo(const std::string& type_name, ElementType _element_type);
    CodecTypeInfo(const std::string& type_name,
                  ElementType _element_type,
                  AccessEnumType enum_access_type);

private:
    ElementType _element_type;
    std::string _type_name;
    AccessEnumType _enum_access_type;
    bool _is_enum;
};

/**
 * @brief The constant info of a element
 */
class CodecConstantInfo {
public:
    /**
     * @brief Determines if the element has a constant value or not
     *
     * @return true is constant
     * @return false is not contant
     */
    bool isConstant() const;
    /**
     * @brief Get the name of the constant value
     * @remark The pointer of the return value is valid as long as the codec factory created this
     * CodecConstantInfo.
     * @return const char*
     */
    const char* getConstantName() const;
    /**
     * Returns the constant value in type T.
     * @return T Returns the the constant value
     * @throw throws std::runtime_error if not found.
     */
    a_util::variant::Variant getConstantValue() const;

private:
    friend class StructAccess;
    CodecConstantInfo(const char* constant_name, const a_util::variant::Variant* constant);

private:
    const char* _constant_name;
    const a_util::variant::Variant* _constant;
};

/**
 * @brief The default value info of a element
 */
class CodecDefaultValueInfo {
public:
    /**
     * @brief Determines if the element has a constant value or not
     *
     * @return true is constant
     * @return false is not contant
     */
    bool hasDefaultValue() const;
    /**
     * Returns the default value in type T.
     * @return a_util::variant::Variant Returns the the default value as variant
     * @throw throws std::runtime_error if not found.
     */
    a_util::variant::Variant getDefaultValue() const;

private:
    friend class StructAccess;
    CodecDefaultValueInfo(const a_util::variant::Variant& default_value);

private:
    const a_util::variant::Variant _default_value;
};

} // namespace codec
} // namespace ddl

#endif // DDL_CODEC_TYPE_INFO_CLASS_HEADER
