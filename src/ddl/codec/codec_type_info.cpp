/**
 * @file
 * Implementation of Codec Type Info class.
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

#include <ddl/codec/codec_type_info.h>

#include <stdexcept>

namespace ddl {
namespace codec {

ElementType CodecTypeInfo::getType() const
{
    return _element_type;
}

const char* CodecTypeInfo::getTypeName() const
{
    return _type_name.c_str();
}
bool CodecTypeInfo::isEnum() const
{
    return _is_enum;
}

std::vector<std::string> CodecTypeInfo::getEnumElementNames() const
{
    if (isEnum()) {
        std::vector<std::string> element_names;
        element_names.reserve(_enum_access_type.size());
        for (const auto& enum_type: _enum_access_type) {
            element_names.push_back(enum_type.first);
        }
        return element_names;
    }
    throw std::runtime_error("The type " + _type_name + " is not an enum");
}

bool CodecTypeInfo::hasEnumElementName(const std::string& name) const
{
    return _enum_access_type.cend() != _enum_access_type.find(name);
}

a_util::variant::Variant CodecTypeInfo::getEnumElementValue(const std::string& name) const
{
    if (_is_enum) {
        const auto found = _enum_access_type.find(name);
        if (found != _enum_access_type.cend()) {
            return found->second;
        }
        throw std::runtime_error("Can not find " + name + " in type " + _type_name);
    }
    throw std::runtime_error("The type " + _type_name + " is not an enum");
}

#define GET_ENUM_CASE(__type)                                                                      \
    case a_util::variant::VariantType::VT_##__type: {                                              \
        const auto xValue = value.as##__type();                                                    \
        const auto xValueFound = current_value.as##__type();                                       \
        if (xValue == xValueFound) {                                                               \
            return enum_element.first.c_str();                                                     \
        }                                                                                          \
        break;                                                                                     \
    }

const char* CodecTypeInfo::getEnumElementName(const a_util::variant::Variant& value) const
{
    if (_is_enum) {
        for (const auto& enum_element: _enum_access_type) {
            const a_util::variant::Variant& current_value = enum_element.second;
            if (value.getType() == current_value.getType()) {
                if (current_value == value) {
                    return enum_element.first.c_str();
                }
            }
            else {
                switch (value.getType()) {
                    GET_ENUM_CASE(Bool)
                    GET_ENUM_CASE(Int8)
                    GET_ENUM_CASE(UInt8)
                    GET_ENUM_CASE(Int16)
                    GET_ENUM_CASE(UInt16)
                    GET_ENUM_CASE(Int32)
                    GET_ENUM_CASE(UInt32)
                    GET_ENUM_CASE(Int64)
                    GET_ENUM_CASE(UInt64)
                    GET_ENUM_CASE(Float)
                    GET_ENUM_CASE(Double)
                default:
                    break;
                }
            }
        }
        throw std::runtime_error("Can not find value " + value.asString() + " in type " +
                                 _type_name);
    }
    throw std::runtime_error("The type " + _type_name + " is not an enum");
}

CodecTypeInfo::CodecTypeInfo(const std::string& type_name, ElementType element_type)
    : _element_type(element_type), _type_name(type_name), _is_enum(false)
{
}

CodecTypeInfo::CodecTypeInfo(const std::string& type_name,
                             ElementType element_type,
                             AccessEnumType enum_access_type)
    : _element_type(element_type),
      _type_name(type_name),
      _enum_access_type(std::move(enum_access_type)),
      _is_enum(true)
{
}

const AccessEnumType* CodecTypeInfo::getLegacyAccessEnumType() const
{
    return isEnum() ? &_enum_access_type : nullptr;
}

/**************************************************************************/

/**************************************************************************/

bool CodecConstantInfo::isConstant() const
{
    return _constant != nullptr;
}

const char* CodecConstantInfo::getConstantName() const
{
    return _constant ? _constant_name : nullptr;
}

namespace {
template <typename T>
T getConstValue(const a_util::variant::Variant* value)
{
    if (value) {
        return *value;
    }
    throw std::runtime_error("The value is not a constant");
}
} // namespace

a_util::variant::Variant CodecConstantInfo::getConstantValue() const
{
    if (_constant) {
        return *_constant;
    }
    throw std::runtime_error("The value is not a constant");
}

CodecConstantInfo::CodecConstantInfo(const char* constant_name,
                                     const a_util::variant::Variant* constant)
    : _constant_name(constant_name), _constant(constant)
{
}

bool CodecDefaultValueInfo::hasDefaultValue() const
{
    return _default_value.getType() != a_util::variant::VariantType::VT_Empty;
}

a_util::variant::Variant CodecDefaultValueInfo::getDefaultValue() const
{
    return _default_value;
}

CodecDefaultValueInfo::CodecDefaultValueInfo(const a_util::variant::Variant& default_value)
    : _default_value(default_value)
{
}

} // namespace codec
} // namespace ddl
