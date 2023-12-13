/**
 * @file
 * OO DataDefinition Redesign
 *
 * Copyright @ 2021 VW Group. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "dd_offset_calculation.h"

#include <ddl/dd/dd_typeinfomodel.h>
namespace ddl {

namespace dd {

size_t calculateAlignedSize(size_t unaligned_byte_size, size_t alignment)
{
    if (alignment == 0)
        alignment = 1;
    auto rest_bytes = unaligned_byte_size % alignment;
    if (rest_bytes > 0) {
        return unaligned_byte_size + (alignment - rest_bytes);
    }
    else {
        return unaligned_byte_size;
    }
}

OptionalSize getSerializedHighestBit(const datamodel::StructType::Element& current)
{
    auto size_info = current.getInfo<ElementTypeInfo>();
    if (size_info == nullptr) {
        throw Error("getSerializedHighestBit",
                    {current.getName()},
                    "Something went wrong calculating Elements Pos");
    }
    if (size_info->isDynamic() || size_info->isAfterDynamic()) {
        return {};
    }

    size_t used_bit_pos = 0;
    if (current.getBitPos()) {
        used_bit_pos = *current.getBitPos();
    }
    return (*current.getBytePos() * 8 + used_bit_pos + size_info->getSerializedBitSize());
}

size_t calculateSerializedBitSize(size_t array_size, size_t type_bit_size, OptionalSize numbits)
{
    size_t array_size_to_use = array_size;
    if (array_size_to_use == 0) {
        array_size_to_use = 1;
    }
    if (numbits) {
        if (type_bit_size < *numbits) {
            // numbits is not allowed to be greater than the the type used (but also 1 - 7)
            return type_bit_size;
        }
        return *numbits;
    }
    else {
        return type_bit_size * array_size_to_use;
    }
}

namespace {

const TypeInfo* getOrCreateTypeInfo(const datamodel::StructType::Element& current,
                                    datamodel::DataDefinition& ddl,
                                    datamodel::ElementType& elem_type_return)
{
    elem_type_return = {};
    const auto& type_name = current.getTypeName();
    if (type_name.empty()) {
        // invalid! caller will mark it as invalid!
        return nullptr;
    }

    elem_type_return._type_of_type = ddl.getTypeOfType(type_name);
    if (elem_type_return._type_of_type == TypeOfType::data_type) {
        elem_type_return._data_type = ddl.getDataTypes().access(type_name);
        auto info = elem_type_return._data_type->getInfo<TypeInfo>();
        if (info == nullptr) {
            elem_type_return._data_type->setInfo<TypeInfo>(
                std::make_shared<TypeInfo>(*elem_type_return._data_type, ddl));
            info = elem_type_return._data_type->getInfo<TypeInfo>();
        }
        return info;
    }
    else if (elem_type_return._type_of_type == TypeOfType::enum_type) {
        elem_type_return._enum_type = ddl.getEnumTypes().access(type_name);
        auto info = elem_type_return._enum_type->getInfo<TypeInfo>();
        if (info == nullptr) {
            elem_type_return._enum_type->setInfo<TypeInfo>(
                std::make_shared<TypeInfo>(*elem_type_return._enum_type, ddl));
            info = elem_type_return._enum_type->getInfo<TypeInfo>();
        }
        // we set also the data type here!
        elem_type_return._data_type =
            ddl.getDataTypes().access(elem_type_return._enum_type->getDataTypeName());
        return info;
    }
    else if (elem_type_return._type_of_type == TypeOfType::struct_type) {
        elem_type_return._struct_type = ddl.getStructTypes().access(type_name);
        auto info = elem_type_return._struct_type->getInfo<TypeInfo>();
        if (info == nullptr) {
            // we need that order because of possible recursions
            elem_type_return._struct_type->setInfo<TypeInfo>(std::make_shared<TypeInfo>());
            info = elem_type_return._struct_type->getInfo<TypeInfo>();
            info->update(*elem_type_return._struct_type, ddl);
        }
        return info;
    }
    else {
        // invalid! caller will mark it as invalid!
        // type does not exist
        return nullptr;
    }
}
} // namespace

CalculatedSerializedPos calculateSerializedPosSize(
    const datamodel::StructType::Element& current,
    const std::shared_ptr<datamodel::StructType::Element>& previous,
    datamodel::DataDefinition& ddl,
    datamodel::ElementType& elem_type_return)
{
    CalculatedSerializedPos result;
    // if previous element exists and this is not the first element we need to get the info for it
    ElementTypeInfo* previous_info = nullptr;
    if (previous) {
        previous_info = previous->getInfo<ElementTypeInfo>();
        if (previous_info == nullptr) {
            throw Error("calculateSerializedPosSize",
                        {current.getName(), previous->getName()},
                        "Something went wrong calculating Elements Pos");
        }
    }

    // we need to discover the typeinfo of the used type
    auto type_info = getOrCreateTypeInfo(current, ddl, elem_type_return);
    if (type_info == nullptr) {
        result._valid = false;
        return result;
    }

    result._serialized_type_bit_size = type_info->getTypeBitSize();

    // calculate or get the serialized size
    // find out if this element is dynamic
    if (current.getArraySize().isDynamicArraySize() || type_info->isDynamic()) {
        result._is_dynamic = true;
    }

    // the serialized size can only be calculated now if this is not a dynamic array or type
    if (!result._is_dynamic) {
        // the serialized bitsize can be always calculated
        result._serialized_bit_size =
            calculateSerializedBitSize(current.getArraySize().getArraySizeValue(),
                                       type_info->getTypeBitSize(),
                                       current.getNumBits());
    }

    // get the serialized pos
    // the pos can only be valid if this is not after dynamic
    if (previous) {
        result._is_after_dynamic = previous_info->isDynamic() || previous_info->isAfterDynamic();
    }

    if (result._is_after_dynamic) {
        result._serialized_byte_pos = {};
        return result;
    }

    // the pos was set by the user
    if (current.getBytePos()) {
        // leave it alone! User set bytepos already
        result._serialized_byte_pos = *current.getBytePos();
        result._serialized_bit_pos = *current.getBitPos();
        result._reset_allowed = false;
        return result;
    }
    else {
        if (result._is_after_dynamic) {
            result._reset_allowed = false;
        }
    }

    // we fully calculate the pos
    if (!previous) {
        // if this is the first we start with 0
        result._serialized_byte_pos = 0;
        return result;
    }
    else {
        auto prev_bit_size_to_use = previous_info->getSerializedBitSize();
        auto prev_byte_pos = previous_info->getSerializedBytePos();
        auto prev_bit_pos = previous->getBitPos();
        auto my_bit_pos = current.getBitPos();

        if (prev_byte_pos) {
            // if both have a bit pos set, we calculate that this will be set a bit field somehow
            if (prev_bit_pos && my_bit_pos) {
                auto absolute_bit_pos = (*prev_byte_pos * 8) + *prev_bit_pos + prev_bit_size_to_use;
                result._serialized_bit_pos = absolute_bit_pos % 8;
                result._serialized_byte_pos = absolute_bit_pos / 8;
            }
            // otherwise we move it to the next byte
            else {
                auto absolute_bit_pos = (*prev_byte_pos * 8) + *prev_bit_pos + prev_bit_size_to_use;
                result._serialized_byte_pos =
                    (((absolute_bit_pos % 8) > 0) ? (absolute_bit_pos / 8) + 1 :
                                                    (absolute_bit_pos / 8));
            }
        }
        else {
            result._valid = false;
            throw Error("StructType::Element::calculateSerializedPosSize",
                        "previous 'bytepos' of " + previous->getName() + " is invalid");
        }
        return result;
    }
}

CalculatedDeserializedPos calculateDeserializedPosSize(
    const datamodel::StructType::Element& current,
    const std::shared_ptr<datamodel::StructType::Element>& previous,
    const Version& struct_ddl_version,
    datamodel::DataDefinition& ddl,
    datamodel::ElementType& elem_type_return)
{
    Version file_ddl_version = ddl.getVersion();
    CalculatedDeserializedPos result;
    // if previous element exists and this is not the first element we need to get the info for it
    ElementTypeInfo* previous_info = nullptr;
    if (previous) {
        previous_info = previous->getInfo<ElementTypeInfo>();
        if (previous_info == nullptr) {
            throw Error("calculateDeserializedPosSize",
                        {current.getName(), previous->getName()},
                        "Something went wrong calculating Elements Pos");
        }
    }

    // we need to discover the typeinfo of the used type
    auto type_info = getOrCreateTypeInfo(current, ddl, elem_type_return);
    if (type_info == nullptr) {
        result._valid = false;
        return result;
    }

    // align the pos
    // use the elements alignment if set!
    size_t alignment_of_the_element = current.getAlignment();
    if (alignment_of_the_element == Alignment::e_invalid) {
        // if not set use the types alignment
        alignment_of_the_element = type_info->getTypeAlignment();
    }
    if (alignment_of_the_element == 0)
        alignment_of_the_element = 1;

    // calculate or get the serialized size
    // find out if this element is dynamic, it is if it has a dynamic content or the type itself is
    // dynamic
    if (current.getArraySize().isDynamicArraySize() || type_info->isDynamic()) {
        result._is_dynamic = true;
    }

    // type size can already set now also if it is dyn
    result._deserialized_type_byte_size = type_info->getTypeByteSize();
    result._deserialized_type_aligned_byte_size = type_info->getTypeAlignedByteSize();

    // get the serialized pos
    // the pos can only be valid if this is not after dynamic
    if (previous) {
        result._is_after_dynamic = previous_info->isDynamic() || previous_info->isAfterDynamic();
    }

    if (result._is_after_dynamic) {
        result._deserialized_byte_pos = {};
    }
    else {
        // position
        if (!previous) {
            result._deserialized_byte_pos = 0;
        }
        else {
            size_t prev_deserialized_pos = previous_info->getDeserializedBytePos();
            size_t prev_deserialized_size = previous_info->getDeserializedByteSize();
            // get the next pos
            // the byte size was influenceed by the types alignment
            size_t current_pos = prev_deserialized_pos + prev_deserialized_size;
            // move pos to the alignment to use
            // we do NOT consider the struct alignment here
            size_t rest_bytes = (current_pos % alignment_of_the_element);
            if (rest_bytes != 0) {
                rest_bytes = alignment_of_the_element - rest_bytes;
                current_pos += rest_bytes;
            }
            result._deserialized_byte_pos = current_pos;
        }
    }

    if (result._is_dynamic) {
        // my size can not be calculated now
        result._deserialized_byte_size = {};
    }

    // my size depending on the struct/fileversion
    // is both versions not set we use the newest
    Version relevant_version =
        (struct_ddl_version != Version(0, 0)) ? struct_ddl_version : file_ddl_version;
    if (relevant_version == Version(0, 0)) {
        // We take the newest if not set
        relevant_version = dd::Version::getLatestVersion();
    }

    size_t array_size_used = current.getArraySize().getArraySizeValue();
    if (array_size_used == 0) {
        array_size_used = 1;
    }

    if (relevant_version >= Version(3, 0)) {
        // if array and version is 3.0 and higher -> then we use the aligned size for all array
        // elements
        size_t calculated_byte_size = array_size_used * type_info->getTypeAlignedByteSize();
        result._deserialized_byte_size = calculated_byte_size;
    }
    else {
        // if array and version is lower then 3.0 -> we use the aligned size for n-1 array elements
        size_t calculated_byte_size = 1 * type_info->getTypeByteSize() +
                                      (array_size_used - 1) * type_info->getTypeAlignedByteSize();
        result._deserialized_byte_size = calculated_byte_size;
    }
    return result;
}

} // namespace dd
} // namespace ddl
