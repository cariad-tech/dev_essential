/**
 * @file
 * OO DataDefinition Redesign
 *
 * Copyright @ 2021 VW Group. All rights reserved.
 *
 *     This Source Code Form is subject to the terms of the Mozilla
 *     Public License, v. 2.0. If a copy of the MPL was not distributed
 *     with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * If it is not possible or desirable to put the notice in a particular file, then
 * You may include the notice in a location (such as a LICENSE file in a
 * relevant directory) where a recipient would be likely to look for such a notice.
 *
 * You may add additional accurate notices of copyright ownership.
 */

#include "dd_offset_calculation.h"

#include <ddl/dd/dd_predefined_datatypes.h>
#include <ddl/dd/dd_typeinfomodel.h>

#include <iterator>

namespace ddl {
namespace dd {

constexpr const uint8_t TypeInfo::INFO_TYPE_ID;
constexpr const uint8_t ElementTypeInfo::INFO_TYPE_ID;

TypeInfo::TypeInfo(const datamodel::DataType& data_type, datamodel::DataDefinition& parent_ddl)
{
    update(data_type, parent_ddl);
}

TypeInfo::TypeInfo(const datamodel::EnumType& enum_type, datamodel::DataDefinition& parent_ddl)
{
    update(enum_type, parent_ddl);
}

size_t TypeInfo::getTypeBitSize() const
{
    return _type_bit_size;
}

size_t TypeInfo::getTypeAlignment() const
{
    return _type_alignment;
}

size_t TypeInfo::getTypeByteSize() const
{
    return _type_byte_size;
}

size_t TypeInfo::getTypeAlignedByteSize() const
{
    return _type_aligned_byte_size;
}

size_t TypeInfo::getTypeUnalignedByteSize() const
{
    return _type_unaligned_byte_size;
}

bool TypeInfo::isDynamic() const
{
    return _is_dynamic;
}

bool TypeInfo::isValid() const
{
    return _is_valid;
}

void TypeInfo::update(const dd::datamodel::DataType& data_type, datamodel::DataDefinition&)
{
    _type_bit_size = data_type.getBitSize();
    _type_byte_size = (_type_bit_size % 8) == 0 ? _type_bit_size / 8 : _type_bit_size / 8 + 1;
    auto default_alignment = data_type.getDefaultAlignment();
    if (!default_alignment) {
        default_alignment =
            ddl::PredefinedDataTypes::getInstance().getDefaultAlignment(data_type.getName());
    }
    if (!default_alignment) {
        _type_alignment = _type_byte_size;
    }
    else {
        _type_alignment = *default_alignment;
    }
    _type_unaligned_byte_size = _type_byte_size;
    _type_aligned_byte_size = calculateAlignedSize(_type_byte_size, _type_alignment);
    _is_dynamic = false;
    _is_valid = true;
}

void TypeInfo::update(const datamodel::EnumType& enum_type, datamodel::DataDefinition& parent_dd)
{
    auto data_type = parent_dd.getDataTypes().get(enum_type.getDataTypeName());
    if (data_type) {
        // the enum type has the same type info then
        update(*data_type, parent_dd);
    }
    else {
        // the type is invalid, but this was already checked by the ValidationInfo
        _type_bit_size = 0;
        _type_byte_size = 0;
        _type_alignment = 0;
        _type_aligned_byte_size = 0;
        _type_unaligned_byte_size = 0;
        _is_dynamic = false;
        _is_valid = false;
    }
}

namespace {

void createOrUpdateElementInfo(
    datamodel::StructType::Element& element_type,
    const std::shared_ptr<datamodel::StructType::Element>& previous_element,
    const Version& struct_ddl_version,
    datamodel::DataDefinition& parent_dd)
{
    auto elem_info = element_type.getInfo<ElementTypeInfo>();
    if (elem_info == nullptr) {
        element_type.setInfo<ElementTypeInfo>(std::make_shared<ElementTypeInfo>());
        elem_info = element_type.getInfo<ElementTypeInfo>();
    }
    elem_info->update(element_type, previous_element, struct_ddl_version, parent_dd);
}

} // namespace

void TypeInfo::update(datamodel::StructType& struct_type,
                      datamodel::DataDefinition& parent_dd,
                      UpdateType update_type)
{
    if (_already_discovering) {
        _is_valid = false;
        throw Error("TypeInfo::update", {struct_type.getName()}, "Recursive use of this type!");
    }
    _already_discovering = true;

    // this is a special case, we need to access the element and create elements info, it cant be
    // const then
    bool updated_previous_element = false;
    std::shared_ptr<datamodel::StructType::Element> previous_element = {};
    size_t discovered_type_bit_size = 0;
    size_t discovered_last_static_byte_offset = 0;

    // prepare loop for all or only the last
    auto& elements = struct_type.getElements();
    auto current_it = elements.begin();
    if (update_type == UpdateType::only_last) {
        if (elements.getSize() > 2) {
            std::advance(current_it, elements.getSize() - 2);
            previous_element = *current_it;
            std::advance(current_it, 1);
        }
    }
    else {
        updated_previous_element = (update_type == UpdateType::force_all);
        _is_valid = true;
        _is_dynamic = false; // this will be reset within the element loop
    }

    for (; current_it != elements.end(); ++current_it) {
        auto current_element = *current_it;
        auto current_elem_info = current_element->getInfo<ElementTypeInfo>();
        if (current_elem_info != nullptr && !updated_previous_element) {
            // go ahead to the next
        }
        else {
            // if something happend in the middle we need to recalculate all after this
            updated_previous_element = true;
            createOrUpdateElementInfo(
                *current_element, previous_element, struct_type.getLanguageVersion(), parent_dd);
            current_elem_info = current_element->getInfo<ElementTypeInfo>();
        }
        if (current_elem_info->isDynamic()) {
            // means it has a dynamic type OR is an dynamic array
            _is_dynamic = true;
        }
        else {
            if (!current_elem_info->isAfterDynamic()) {
                // last static known size is before any dynamic content
                discovered_last_static_byte_offset =
                    *(current_elem_info->getDeserializedBytePos()) +
                    *(current_elem_info->getDeserializedByteSize());
            }
        }
        if (!current_elem_info->isValid()) {
            // means it has a dynamic type OR is an dynamic array
            _is_valid = false;
        }
        size_t current_serialized_highest_bit = getSerializedHighestBit(*current_element);
        if (current_serialized_highest_bit > discovered_type_bit_size) {
            discovered_type_bit_size = current_serialized_highest_bit;
        }
        previous_element = current_element;
    }

    _type_bit_size = discovered_type_bit_size; // this is the serialized thing
    _type_alignment = struct_type.getAlignment();

    // previous element should be the
    if (_is_dynamic) {
        _type_byte_size = discovered_last_static_byte_offset;
    }
    else {
        if (previous_element) {
            // the last element has the size (bytepos + its size)
            _type_byte_size = discovered_last_static_byte_offset;
            _type_aligned_byte_size = calculateAlignedSize(_type_byte_size, _type_alignment);
            Version relevant_version = struct_type.getLanguageVersion();
            if (relevant_version == Version::ddl_version_notset) {
                relevant_version = parent_dd.getVersion();
                if (relevant_version == Version::ddl_version_notset) {
                    relevant_version = Version::ddl_version_current;
                }
            }
            _type_unaligned_byte_size = _type_byte_size;
            if (relevant_version >= Version::ddl_version_30) {
                // since 3.0 there is no difference between aligned and non aligned
                _type_byte_size = _type_aligned_byte_size;
            }
        }
        else {
            // there is no element
            _type_byte_size = 0;
        }
    }

    _already_discovering = false;
}

/**
 * Element Size Info is inportant to get the TypeInfo for Structs
 */

OptionalSize ElementTypeInfo::getDeserializedBytePos(size_t array_pos) const
{
    if (_deserialized_byte_pos && array_pos > 0) {
        return *_deserialized_byte_pos + array_pos * getDeserializedTypeAlignedByteSize();
    }
    return _deserialized_byte_pos;
}

OptionalSize ElementTypeInfo::getDeserializedByteSize() const
{
    return _deserialized_byte_size;
}

OptionalSize ElementTypeInfo::getSerializedBytePos(size_t array_pos) const
{
    if (_serialized_byte_pos && array_pos > 0) {
        size_t bit_offset = array_pos * getSerializedTypeBitSize();
        auto rest_bits = bit_offset % 8;
        if (rest_bits == 0) {
            return *_serialized_byte_pos + bit_offset / 8;
        }
        else {
            return *_serialized_byte_pos + bit_offset / 8 + 1;
        }
    }
    return _serialized_byte_pos;
}

OptionalSize ElementTypeInfo::getSerializedBitOffset(size_t array_pos) const
{
    if (_serialized_absolute_bit_offset && array_pos > 0) {
        return *_serialized_absolute_bit_offset + (array_pos * getSerializedTypeBitSize());
    }
    return _serialized_absolute_bit_offset;
}

size_t ElementTypeInfo::getSerializedBitSize() const
{
    return _serialized_bit_size;
}

bool ElementTypeInfo::isDynamic() const
{
    return _is_dynamic;
}

bool ElementTypeInfo::isAfterDynamic() const
{
    return _is_after_dynamic;
}

bool ElementTypeInfo::isValid() const
{
    return _is_valid;
}

size_t ElementTypeInfo::getDeserializedTypeByteSize() const
{
    return _deserialized_type_byte_size;
}

size_t ElementTypeInfo::getDeserializedTypeAlignedByteSize() const
{
    return _deserialized_type_aligned_byte_size;
}

size_t ElementTypeInfo::getSerializedTypeBitSize() const
{
    return _serialized_type_bit_size;
}

void ElementTypeInfo::update(
    datamodel::StructType::Element& element_type,
    const std::shared_ptr<datamodel::StructType::Element>& previous_element,
    const Version& struct_ddl_version,
    datamodel::DataDefinition& parent_dd)
{
    auto serialized_pos =
        calculateSerializedPosSize(element_type, previous_element, parent_dd, _element_type);
    if (serialized_pos._reset_allowed) {
        element_type.setBytePos(serialized_pos._serialized_byte_pos, true);
        element_type.setBitPos(serialized_pos._serialized_bit_pos, true);
    }
    _serialized_byte_pos = serialized_pos._serialized_byte_pos;
    _serialized_absolute_bit_offset =
        (*serialized_pos._serialized_byte_pos * 8) + *serialized_pos._serialized_bit_pos;
    _serialized_bit_size = serialized_pos._serialized_bit_size;
    _serialized_type_bit_size = serialized_pos._serialized_type_bit_size;

    auto deserialized_pos = calculateDeserializedPosSize(
        element_type, previous_element, struct_ddl_version, parent_dd, _element_type);

    _deserialized_byte_pos = deserialized_pos._deserialized_byte_pos;
    _deserialized_byte_size = deserialized_pos._deserialized_byte_size;
    _deserialized_type_byte_size = deserialized_pos._deserialized_type_byte_size;
    _deserialized_type_aligned_byte_size = deserialized_pos._deserialized_type_aligned_byte_size;

    _is_after_dynamic = deserialized_pos._is_after_dynamic;
    _is_dynamic = deserialized_pos._is_dynamic;

    _is_valid = deserialized_pos._valid && serialized_pos._valid;
}

std::shared_ptr<const datamodel::StructType> ElementTypeInfo::getStructType() const
{
    return _element_type._struct_type;
}

std::shared_ptr<const datamodel::EnumType> ElementTypeInfo::getEnumType() const
{
    return _element_type._enum_type;
}

std::shared_ptr<const datamodel::DataType> ElementTypeInfo::getDataType() const
{
    return _element_type._data_type;
}

TypeOfType ElementTypeInfo::getTypeOfType() const
{
    return _element_type._type_of_type;
}

} // namespace dd
} // namespace ddl
