/**
 * @file
 * Implementation of CodecFactory
 *
 * Copyright @ 2022 VW Group. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "codec_elements.h"

#include <a_util/result/error_def.h>
#include <ddl/dd/dd_typeinfomodel.h>
#include <ddl/legacy_error_macros.h>
#include <ddl/utilities/std_to_string.h>

namespace ddl {
namespace codec {

// define all needed error types and values locally
_MAKE_RESULT(-37, ERR_NOT_INITIALIZED);
_MAKE_RESULT(-38, ERR_FAILED);

constexpr size_t invalid_codec_index_pos = static_cast<size_t>(-1);
constexpr size_t byte_size_in_bits = 8;

namespace {

constexpr size_t bitToBytes(const size_t size) noexcept
{
    return (size % byte_size_in_bits) ? (size / byte_size_in_bits + 1) : (size / byte_size_in_bits);
}

constexpr size_t calculateAlignedSizeOrPos(size_t unaligned_byte_size, size_t alignment) noexcept
{
    if (alignment == 0) {
        alignment = 1;
    }
    const auto remaining_bytes = unaligned_byte_size % alignment;
    return (remaining_bytes > 0) ? (unaligned_byte_size + alignment - remaining_bytes) :
                                   unaligned_byte_size;
}

TypeSize getTypeSizes(const dd::TypeInfo& info) noexcept
{
    return TypeSize{
        // the type sizes needs to be recalculated if the type has dynamic size
        info.getTypeBitSize(),           // serialized_bits
        info.getTypeUnalignedByteSize(), // deserialized_bytes_unaligned
        info.getTypeAlignedByteSize(),   // deserialized_bytes_aligned
        info.getTypeByteSize(),          // deserialized_bytes
        info.getTypeAlignment(),         // type_alignment
        dd::Version(0, 0),               // type_ddl_version
        info.isDynamic()                 // is_dynamic
    };
}

void calculateDeserializedTypeSize(const ElementLayoutBase& base_layout_last,
                                   TypeSize& type_sizes,
                                   const dd::Version& file_dd_version)
{
    const auto& relevant_version = [&type_sizes, &file_dd_version]() {
        if (type_sizes.type_ddl_version != dd::Version(0, 0)) {
            return type_sizes.type_ddl_version;
        }
        else if (file_dd_version != dd::Version(0, 0)) {
            return file_dd_version;
        }
        else {
            return dd::Version::getLatestVersion();
        }
    }();

    // we must calculate the struct_size after resolving by using the last element
    // but the ddl version is relevant
    type_sizes.deserialized_bytes_unaligned =
        (base_layout_last.deserialized.bit_offset / byte_size_in_bits) +
        (base_layout_last.deserialized.bit_size / byte_size_in_bits);
    type_sizes.deserialized_bytes_aligned = calculateAlignedSizeOrPos(
        type_sizes.deserialized_bytes_unaligned, type_sizes.type_alignment);
    if (relevant_version >= dd::Version(3, 0)) {
        // since 3.0 there is no difference between aligned and non aligned
        type_sizes.deserialized_bytes = type_sizes.deserialized_bytes_aligned;
    }
    else {
        type_sizes.deserialized_bytes = type_sizes.deserialized_bytes_unaligned;
    }
}
} // namespace

/************************************************************************************
 *
 */
void ChildElementAccess::SubElements::add(std::shared_ptr<ChildElementAccess> element_access)
{
    if (getSize() != 0) {
        operator[](getSize() - 1)->setNextElement(element_access);
    }
    element_access->setElementIndex(getSize());
    _codec_elements_vector.push_back(element_access);
    _codec_elements_map[element_access->getBaseName()] = std::move(element_access);
}

void ChildElementAccess::SubElements::addStatic(
    const std::shared_ptr<ChildElementAccess>& element_access)
{
    add(element_access);
    _static_count++;
}

void ChildElementAccess::SubElements::addDynamic(
    const std::shared_ptr<ChildElementAccess>& element_access)
{
    add(element_access);
}

size_t ChildElementAccess::SubElements::getSize() const
{
    return _codec_elements_vector.size();
}

size_t ChildElementAccess::SubElements::getStaticCount() const
{
    return _static_count;
}

size_t ChildElementAccess::SubElements::getDynamicCount() const
{
    return getSize() - getStaticCount();
}

const ChildElementAccess& ChildElementAccess::SubElements::operator[](
    const CodecIndex::ElementIndex& index) const
{
    return operator[](index.getIndex());
}

const ChildElementAccess& ChildElementAccess::SubElements::operator[](
    const NamedCodecIndex::NamedElementIndex& index) const
{
    return operator[](index.getName());
}

std::shared_ptr<ChildElementAccess> ChildElementAccess::SubElements::operator[](size_t index)
{
    if (index != invalid_codec_index_pos && index < getSize()) {
        return _codec_elements_vector[index];
    }
    throw std::runtime_error("Element not found : " + std::to_string(index));
}

const ChildElementAccess& ChildElementAccess::SubElements::operator[](size_t index) const
{
    if (index != invalid_codec_index_pos && index < getSize()) {
        return *_codec_elements_vector[index];
    }
    throw std::runtime_error("Element not found : " + std::to_string(index));
}

const ChildElementAccess& ChildElementAccess::SubElements::operator[](const std::string& name) const
{
    const auto& found = _codec_elements_map.find(name);
    if (found != _codec_elements_map.end()) {
        return *found->second;
    }
    throw std::runtime_error("Element not found : " + name);
}

ChildElementAccess::SubElements::SubElements(const ChildElementAccess::SubElements& other)
{
    this->operator=(other);
}

ChildElementAccess::SubElements& ChildElementAccess::SubElements::operator=(
    const ChildElementAccess::SubElements& other)
{
    for (size_t index = 0; index < other.getSize(); ++index) {
        if (index < other._static_count) {
            // the statics we do not need to copy, so we only reference it
            auto copied_static_element = std::make_shared<StaticCodecElementAccess>(
                *static_cast<const StaticCodecElementAccess*>(&other[index]));
            addStatic(copied_static_element);
        }
        else {
            // the dynamics must be copied
            auto copied_dynamic_element = std::make_shared<DynamicCodecElementAccess>(
                *static_cast<const DynamicCodecElementAccess*>(&other[index]));
            addDynamic(copied_dynamic_element);
        }
    }
    return *this;
}

/************************************************************************************
 * Base Elements
 */
void ChildElementAccess::initializeStatic(const ddl::dd::StructElementAccess& element_access,
                                          const ddl::dd::StructTypeAccess& struct_type_access,
                                          StructAccess& struct_access)
{
    // if an element access is set this is a member of the struct
    if (element_access) {
        _base_name = element_access.getElement().getName();

        // fill out every thing known
        // for StaticCodecElementAccess everything is known
        _static_layout_base.serialized.bit_offset = element_access.getSerializedBitOffset(0);
        _static_layout_base.deserialized.bit_offset =
            element_access.getDeserializedBytePos(0) * byte_size_in_bits;

        // the whole elements size (all array elements are meant)
        _static_layout_base.serialized.bit_size = element_access.getSerializedBitSize();
        _static_layout_base.deserialized.bit_size =
            element_access.getDeserializedByteSize() * byte_size_in_bits;

        _static_layout_base.serialized.type_bit_size = element_access.getSerializedTypeBitSize();
        if (_static_layout_base.serialized.bit_size <
            _static_layout_base.serialized.type_bit_size) {
            _static_layout_base.serialized.type_bit_size_used =
                _static_layout_base.serialized.bit_size;
        }
        else {
            _static_layout_base.serialized.type_bit_size_used =
                _static_layout_base.serialized.type_bit_size;
        }

        // byte order
        _static_layout_base.byte_order = element_access.getElement().getByteOrder();

        // array size
        _static_layout_base.array_size =
            element_access.getElement().getArraySize().getArraySizeValue();
        if (_static_layout_base.array_size == 0) {
            if (element_access.getElement().getArraySize().getArraySizeElementName().empty()) {
                _static_layout_base.array_size = 1;
            }
        }

        _element_alignment = element_access.getElement().getAlignment();
        if (_element_alignment == 0 || _element_alignment == ddl::dd::Alignment::e_invalid) {
            _element_alignment = 1;
        }
    }
    // if no element access is set this is the "main" instance of the structure (top level)
    else {
        // byte positions of first element in the array
        _static_layout_base.serialized.bit_offset = 0;
        _static_layout_base.deserialized.bit_offset = 0;

        // the whole elements size (all array elements are meant)
        _static_layout_base.serialized.bit_size = struct_type_access.getStaticSerializedBitSize();
        _static_layout_base.deserialized.bit_size =
            struct_type_access.getStaticStructSize() * byte_size_in_bits;

        // the type bit size is the same like above
        _static_layout_base.serialized.type_bit_size =
            struct_type_access.getStaticSerializedBitSize();
        if (_static_layout_base.serialized.bit_size <
            _static_layout_base.serialized.type_bit_size) {
            _static_layout_base.serialized.type_bit_size_used =
                _static_layout_base.serialized.bit_size;
        }
        else {
            _static_layout_base.serialized.type_bit_size_used =
                _static_layout_base.serialized.type_bit_size;
        }
        _static_layout_base.deserialized.type_bit_size =
            struct_type_access.getStaticStructSize() * byte_size_in_bits;

        // array size for root element is always 1
        _static_layout_base.array_size = 1;
    }
    // if a struct type access is set this is a struct and we need to go thru the sub elements
    if (struct_type_access) {
        const auto& struct_type = struct_type_access.getStructType();
        _static_layout_base.type_info = struct_access.getOrCreateCodecTypeInfo(struct_type);
        // we want an empty constant info!
        _static_layout_base.constant_info = struct_access.findConstantInfo(std::string());
        // we want an empty default value info!
        _static_layout_base.default_value_info =
            struct_access.getOrCreateDefaultValueInfo(ElementType::cet_empty, std::string());

        // the type sizes i need to calculate the next offset if needed
        auto type_info = struct_type.getInfo<const dd::TypeInfo>();
        _static_type_size = getTypeSizes(*type_info);
        _static_type_size.type_ddl_version = struct_type.getLanguageVersion();
        if (_element_alignment == ddl::dd::Alignment::e_invalid) {
            _element_alignment = _static_type_size.type_alignment;
            if (_element_alignment == 0 || _element_alignment == ddl::dd::Alignment::e_invalid) {
                _element_alignment = 1;
            }
        }
        _static_layout_base.deserialized.type_bit_size =
            _static_type_size.deserialized_bytes * byte_size_in_bits;
        _static_layout_base.deserialized.type_bit_size_aligned =
            _static_type_size.deserialized_bytes_aligned * byte_size_in_bits;

        for (const auto& current_element_access: struct_type_access) {
            addSubElement(current_element_access, struct_access);
        }
        _array_leaf_count = _static_layout_base.array_size * _leaf_count;
    }
    // if no struct type access is set this is a leaf with a datatype or enumtype
    // it has no sub elements anymore
    else {
        // retrieve the typeinfo
        auto data_type = element_access.getDataType();
        auto enum_type = element_access.getEnumType();
        if (enum_type) {
            _static_layout_base.type_info = struct_access.getOrCreateCodecTypeInfo(*enum_type);
            _static_layout_base.constant_info =
                struct_access.findConstantInfo(element_access.getElement().getValue());
            if (_static_layout_base.constant_info->isConstant()) {
                // if already constant there is no default
                _static_layout_base.default_value_info = struct_access.getOrCreateDefaultValueInfo(
                    ElementType::cet_empty, std::string());
            }
            else {
                _static_layout_base.default_value_info = struct_access.getOrCreateDefaultValueInfo(
                    _static_layout_base.type_info->getType(),
                    element_access.getElement().getDefault());
            }
        }
        else {
            _static_layout_base.type_info = struct_access.getOrCreateCodecTypeInfo(*data_type);
            // we want an empty constant info!
            _static_layout_base.constant_info = struct_access.findConstantInfo(std::string());
            // set default info if any default set
            _static_layout_base.default_value_info = struct_access.getOrCreateDefaultValueInfo(
                _static_layout_base.type_info->getType(), element_access.getElement().getDefault());
        }

        // the type sizes i need to calculate the next offset if needed
        auto type_info = element_access.getDataType()->getInfo<dd::TypeInfo>();
        _static_type_size = getTypeSizes(*type_info);

        _static_layout_base.deserialized.type_bit_size =
            _static_type_size.deserialized_bytes * byte_size_in_bits;
        _static_layout_base.deserialized.type_bit_size_aligned =
            _static_type_size.deserialized_bytes_aligned * byte_size_in_bits;

        _leaf_count = 1;
        _array_leaf_count = _static_layout_base.array_size;
        // at least one array element will be added, also for dynamics
        if (_array_leaf_count == 0) {
            _array_leaf_count = 1;
        }
    }
}

void ChildElementAccess::addSubElement(const ddl::dd::StructElementAccess& sub_element_access,
                                       StructAccess& struct_access)
{
    const auto add_to_dynamics = hasDynamicElements() || sub_element_access.isDynamic();
    auto sub_codec_element_access = [&]() -> std::shared_ptr<ChildElementAccess> {
        if (add_to_dynamics) {
            return std::make_shared<DynamicCodecElementAccess>(sub_element_access, struct_access);
        }
        else {
            return std::make_shared<StaticCodecElementAccess>(sub_element_access, struct_access);
        }
    }();

    add_to_dynamics ? _sub_elements.addDynamic(sub_codec_element_access) :
                      _sub_elements.addStatic(sub_codec_element_access);

    if (sub_codec_element_access->hasStaticPosition()) {
        sub_codec_element_access->setBeginLeafIndex(_leaf_count);
        _leaf_count += sub_codec_element_access->getArrayLeafCount();
    }
}

bool ChildElementAccess::hasDynamicElements() const
{
    return _sub_elements.getDynamicCount() > 0;
}

namespace {
void addOffsets(const ElementLayoutBase& layout_base, size_t array_pos, ElementLayout& elem_layout)
{
    elem_layout.deserialized.bit_offset +=
        layout_base.deserialized.bit_offset +
        array_pos * layout_base.deserialized.type_bit_size_aligned;
    elem_layout.serialized.bit_offset +=
        layout_base.serialized.bit_offset + array_pos * layout_base.serialized.type_bit_size;
}

std::string createArrayName(const std::string& element_name, size_t array_pos, size_t array_size)
{
    if (array_size > 1) {
        return element_name + "[" + std::to_string(array_pos) + "]";
    }
    else {
        return element_name;
    }
}

void addBaseLayout(const ElementLayoutBase& layout_base,
                   size_t array_pos,
                   const std::string& element_name,
                   ElementLayout& elem_layout,
                   bool known_position,
                   size_t child_element_count)
{
    // check if within the arraysize
    if (array_pos < layout_base.array_size ||
        (array_pos == 0 && layout_base.array_size == 0 && known_position)) {
        // we want the size of only one element
        elem_layout.deserialized.type_bit_size = layout_base.deserialized.type_bit_size;
        elem_layout.deserialized.type_bit_size_aligned =
            layout_base.deserialized.type_bit_size_aligned;
        elem_layout.serialized.type_bit_size = layout_base.serialized.type_bit_size;
        elem_layout.serialized.type_bit_size_used = layout_base.serialized.type_bit_size_used;

        // we want the whole element size
        elem_layout.deserialized.bit_size = layout_base.deserialized.bit_size;
        elem_layout.serialized.bit_size = layout_base.serialized.bit_size;

        // the base is always the elements pos within its parent struct
        addOffsets(layout_base, array_pos, elem_layout);
        elem_layout.byte_order = layout_base.byte_order;
        elem_layout.array_pos = array_pos;
        elem_layout.type_info = layout_base.type_info;
        elem_layout.constant_info = layout_base.constant_info;
        elem_layout.default_value_info = layout_base.default_value_info;
        elem_layout.array_size = layout_base.array_size;
        elem_layout.child_element_count = child_element_count;
    }
    else {
        throw std::runtime_error(
            "array pos '" + std::to_string(array_pos) + "' is out of range for " +
            createArrayName(element_name, layout_base.array_size, layout_base.array_size) +
            ". It might be an dynamic value.");
    }
}

void addBaseLayout(const ElementLayoutBase& layout_base,
                   size_t array_pos,
                   const std::string& element_name,
                   ElementLayout& elem_layout,
                   std::string& full_element_name,
                   bool known_position,
                   size_t child_element_count)
{
    addBaseLayout(
        layout_base, array_pos, element_name, elem_layout, known_position, child_element_count);
    if (!full_element_name.empty()) {
        full_element_name += ".";
    }
    size_t array_size_to_use = layout_base.array_size;
    if (known_position && array_size_to_use == 0) {
        array_size_to_use = 2;
    }
    full_element_name += createArrayName(element_name, array_pos, array_size_to_use);
}
} // namespace

std::string ChildElementAccess::getName(size_t array_pos) const
{
    size_t used_size = _static_layout_base.array_size;
    if (used_size == 0 && array_pos == 0 && hasStaticPosition()) {
        used_size = 2;
    }
    return createArrayName(getBaseName(), array_pos, used_size);
}

std::string ChildElementAccess::getName(const CodecIndex& codec_index, size_t pos) const
{
    if (pos < codec_index.getSize()) {
        const auto& elem_index = codec_index[pos];
        if (pos == codec_index.getSize() - 1) {
            return operator[](elem_index).getName(elem_index.getArrayPos());
        }
        return operator[](elem_index).getName(codec_index, ++pos);
    }
    throw std::runtime_error("Invalid Index " + toString(codec_index));
}

const std::string& ChildElementAccess::getBaseName() const
{
    return _base_name;
}

const std::string& ChildElementAccess::getBaseName(const CodecIndex& codec_index, size_t pos) const
{
    if (pos < codec_index.getSize()) {
        const auto& elem_index = codec_index[pos];
        if (pos == codec_index.getSize() - 1) {
            return operator[](elem_index).getBaseName();
        }
        return operator[](elem_index).getBaseName(codec_index, ++pos);
    }
    throw std::runtime_error("Invalid Index " + toString(codec_index));
}

void ChildElementAccess::setElementIndex(size_t index)
{
    _element_index = index;
}

size_t ChildElementAccess::getElementIndex() const
{
    return _element_index;
}

void ChildElementAccess::setBeginLeafIndex(size_t leaf_index)
{
    _begin_leaf_index = leaf_index;
}

size_t ChildElementAccess::getBeginLeafIndex() const
{
    return _begin_leaf_index;
}

bool ChildElementAccess::isLeaf() const
{
    return _static_layout_base.type_info->getType() != ElementType::cet_sub_codec;
}

size_t ChildElementAccess::getLeafCount() const
{
    return _leaf_count;
}

size_t ChildElementAccess::getArrayLeafCount() const
{
    return _array_leaf_count;
}

size_t ChildElementAccess::getSubElementCount(bool only_static) const
{
    if (only_static) {
        return _sub_elements.getStaticCount();
    }
    else {
        return _sub_elements.getSize();
    }
}

size_t ChildElementAccess::getSubElementCount(const CodecIndex& codec_index,
                                              size_t pos,
                                              bool only_static) const
{
    if (pos < codec_index.getSize()) {
        const auto& elem_index = codec_index[pos];
        if (pos == codec_index.getSize() - 1) {
            return operator[](elem_index).getSubElementCount(only_static);
        }
        return operator[](elem_index).getSubElementCount(codec_index, ++pos, only_static);
    }
    throw std::runtime_error("Invalid Index " + toString(codec_index));
}

const ElementLayoutBase& ChildElementAccess::getLayoutBase() const
{
    return _static_layout_base;
}

const TypeSize& ChildElementAccess::getTypeSize() const
{
    return _static_type_size;
}

const ChildElementAccess& ChildElementAccess::operator[](
    const CodecIndex::ElementIndex& element_index) const
{
    return _sub_elements[element_index];
}

void ChildElementAccess::findCodecLayout(const CodecIndex& index,
                                         size_t index_pos,
                                         ElementLayout& elem_layout,
                                         bool only_static) const
{
    if (index_pos != invalid_codec_index_pos && index_pos < index.getSize()) {
        const auto& elem_index = index[index_pos];
        const auto& found_codec_elem = _sub_elements[elem_index];
        // this is the last element index, we retrieve the pos
        const auto& layout_base = found_codec_elem.getLayoutBase();
        if (index_pos == index.getSize() - 1) {
            // We retrieve the array pos
            addBaseLayout(found_codec_elem.getLayoutBase(),
                          elem_index.getArrayPos(),
                          found_codec_elem.getBaseName(),
                          elem_layout,
                          found_codec_elem.hasStaticPosition(),
                          found_codec_elem.getSubElementCount(only_static));
            return;
        }
        else {
            found_codec_elem.findCodecLayout(index, ++index_pos, elem_layout, only_static);
            // We add the offset after we added the offset of the sub
            addOffsets(layout_base, elem_index.getArrayPos(), elem_layout);
            return;
        }
    }
    // not found
    throw std::runtime_error("Invalid Index " + toString(index));
}

void ChildElementAccess::findCodecLayout(const CodecIndex& index,
                                         size_t index_pos,
                                         ElementLayout& elem_layout,
                                         std::string& full_element_name,
                                         bool only_static) const

{
    if (index_pos != invalid_codec_index_pos && index_pos < index.getSize()) {
        const auto& elem_index = index[index_pos];
        auto found_codec_elem = _sub_elements[elem_index];
        const auto& layout_base = found_codec_elem.getLayoutBase();
        if (index_pos == index.getSize() - 1) {
            // We retrieve the array pos
            addBaseLayout(layout_base,
                          elem_index.getArrayPos(),
                          found_codec_elem.getBaseName(),
                          elem_layout,
                          full_element_name,
                          found_codec_elem.hasStaticPosition(),
                          found_codec_elem.getSubElementCount(only_static));
            return;
        }
        else {
            if (!full_element_name.empty()) {
                full_element_name += ".";
            }
            full_element_name += found_codec_elem.getName(elem_index.getArrayPos());
            found_codec_elem.findCodecLayout(
                index, ++index_pos, elem_layout, full_element_name, only_static);
            // We add the offsets and the name of this element go further
            addOffsets(layout_base, elem_index.getArrayPos(), elem_layout);
            return;
        }
    }
    // not found
    throw std::runtime_error("Invalid Index " + toString(index));
}

void ChildElementAccess::findCodecLayoutByName(const NamedCodecIndex& named_index,
                                               size_t index_pos,
                                               ElementLayout& elem_layout,
                                               CodecIndex& find_index,
                                               bool only_static) const
{
    if (index_pos != invalid_codec_index_pos && index_pos < named_index.getSize()) {
        const auto& elem_index = named_index[index_pos];
        auto found_codec_elem = _sub_elements[elem_index];
        const auto& layout_base = found_codec_elem.getLayoutBase();
        if (index_pos == named_index.getSize() - 1) {
            // We retrieve the array pos
            find_index.addElementIndex(
                {found_codec_elem.getElementIndex(), elem_index.getArrayPos()});
            addBaseLayout(layout_base,
                          elem_index.getArrayPos(),
                          found_codec_elem.getBaseName(),
                          elem_layout,
                          found_codec_elem.hasStaticPosition(),
                          found_codec_elem.getSubElementCount(only_static));
            return;
        }
        else {
            find_index.addElementIndex(
                {found_codec_elem.getElementIndex(), elem_index.getArrayPos()});
            found_codec_elem.findCodecLayoutByName(
                named_index, ++index_pos, elem_layout, find_index, only_static);
            // We add the offsets and the name of this element go further
            addOffsets(layout_base, elem_index.getArrayPos(), elem_layout);
            return;
        }
    }
    // not found
    throw std::runtime_error("Invalid Index " + toString(named_index));
}

const ChildElementAccess* ChildElementAccess::binaryLeafSearch(size_t leaf_index,
                                                               size_t begin_search_index,
                                                               size_t end_search_index) const
{
    if (end_search_index >= begin_search_index) {
        size_t mid_search_index = begin_search_index;
        if (end_search_index > begin_search_index) {
            mid_search_index = begin_search_index + (end_search_index - 1 / 2);
        }
        while (!_sub_elements[mid_search_index].hasStaticPosition() &&
               mid_search_index > begin_search_index) {
            mid_search_index--;
        }
        const auto& current_sub_element = _sub_elements[mid_search_index];
        if (!current_sub_element.hasStaticPosition()) {
            return nullptr;
        }
        size_t begin_leaf_index = current_sub_element.getBeginLeafIndex();
        size_t end_leaf_index =
            current_sub_element.getBeginLeafIndex() + current_sub_element.getArrayLeafCount();
        if (leaf_index >= begin_leaf_index && leaf_index < end_leaf_index) {
            return &current_sub_element;
        }
        else if (leaf_index < begin_leaf_index) {
            return binaryLeafSearch(leaf_index, begin_search_index, mid_search_index - 1);
        }
        else {
            return binaryLeafSearch(leaf_index, mid_search_index + 1, end_search_index);
        }
    }
    return nullptr;
}

bool ChildElementAccess::findCodecLayoutByLeafIndex(size_t leaf_index,
                                                    ElementLayout& elem_layout,
                                                    CodecIndex& index,
                                                    bool only_static) const
{
    const auto sub_element = binaryLeafSearch(leaf_index, 0, _sub_elements.getSize() - 1);
    if (sub_element) {
        // decrease with the leaf count before
        auto current_leaf_index = leaf_index - sub_element->getBeginLeafIndex();
        const auto& base_layout = sub_element->getLayoutBase();
        size_t array_pos = 0;
        if (base_layout.array_size > 1 && sub_element->getLeafCount() > 0) {
            array_pos = current_leaf_index / sub_element->getLeafCount();
            current_leaf_index -= array_pos * sub_element->getLeafCount();
        }
        if (sub_element->isLeaf()) {
            index.addElementIndex({sub_element->getElementIndex(), array_pos});
            addBaseLayout(base_layout,
                          array_pos,
                          sub_element->getBaseName(),
                          elem_layout,
                          sub_element->hasStaticPosition(),
                          sub_element->getSubElementCount(only_static));
            return true;
        }
        else {
            index.addElementIndex({sub_element->getElementIndex(), array_pos});
            if (sub_element->findCodecLayoutByLeafIndex(
                    current_leaf_index, elem_layout, index, only_static)) {
                addOffsets(base_layout, array_pos, elem_layout);
                return true;
            }
            else {
                return false;
            }
        }
    }
    return false;
}

void ChildElementAccess::setNextElement(const std::shared_ptr<ChildElementAccess>& next_element)
{
    _next_element = next_element;
}

size_t ChildElementAccess::resolveArraySize(const NamedCodecIndex&,
                                            size_t,
                                            const ArraySizeResolverFunction&)
{
    // nothing to resolve for non dynamics
    return _static_layout_base.array_size;
}

bool ChildElementAccess::hasStaticPosition() const
{
    return true;
}

void ChildElementAccess::resolveSizes(const dd::Version&)
{
    // for statics we do not need to resolve sizes, they are known
}

void ChildElementAccess::resolveOffsets(const ChildElementAccess&)
{
    // for statics we do not need to resolve offsets, they are known
}

size_t ChildElementAccess::getElementAlignment() const
{
    return _element_alignment;
}

void ChildElementAccess::resolveDynamics(const NamedCodecIndex& parent_index,
                                         const ArraySizeResolverFunction& array_resolver,
                                         const dd::Version& dd_file_version)
{
    // reset the leaf count for resolving
    _leaf_count = 0;
    // we only resolve for the dynamic elements
    for (size_t current_index_pos = 0; current_index_pos < _sub_elements.getSize();
         ++current_index_pos) {
        const auto& sub_element_access = _sub_elements[current_index_pos];

        // this will calculate the size of the element if the used type is not dynamic
        sub_element_access->resolveArraySize(parent_index, parent_index.getSize(), array_resolver);

        if (!sub_element_access->isLeaf()) {
            sub_element_access->resolveDynamics(
                NamedCodecIndex(parent_index, {sub_element_access->getBaseName(), 0}),
                array_resolver,
                dd_file_version);
        }

        sub_element_access->resolveSizes(dd_file_version);
        sub_element_access->setBeginLeafIndex(_leaf_count);
        _leaf_count += sub_element_access->getArrayLeafCount();
        resolveSizes(dd_file_version);
    }
    _array_leaf_count = _leaf_count * _static_layout_base.array_size;
}

void ChildElementAccess::resolveDynamicSize(const dd::Version& file_dd_version)
{
    if (_sub_elements.getSize() > 0) {
        // Deserialized
        const auto last_element = _sub_elements[_sub_elements.getSize() - 1];
        const auto& base_layout_last = last_element->getLayoutBase();
        calculateDeserializedTypeSize(base_layout_last, _static_type_size, file_dd_version);
        // serialized is the highest pos of the elements
        for (size_t index = 0; index < _sub_elements.getSize(); ++index) {
            const auto current_element = _sub_elements[index];
            const auto& base_layout_current = current_element->getLayoutBase();
            // serialized size in bits
            auto serialized_bits =
                base_layout_current.serialized.bit_offset + base_layout_current.serialized.bit_size;
            if (serialized_bits > _static_type_size.serialized_bits) {
                _static_type_size.serialized_bits = serialized_bits;
            }
        }
    }
}

namespace {
a_util::result::Result isLayoutBinaryEqual(const ElementLayout& layout_left,
                                           const ElementLayout& layout_right)
{
    if (layout_left.type_info->getType() != layout_right.type_info->getType()) {
        RETURN_ERROR_DESCRIPTION(ERR_FAILED,
                                 "Type %s is different to type %s",
                                 layout_left.type_info->getTypeName(),
                                 layout_left.type_info->getTypeName());
    }
    if (layout_left.deserialized.bit_offset != layout_right.deserialized.bit_offset) {
        RETURN_ERROR_DESCRIPTION(ERR_FAILED, "deserialized bit offset is different");
    }
    if (layout_left.deserialized.type_bit_size_aligned !=
        layout_right.deserialized.type_bit_size_aligned) {
        RETURN_ERROR_DESCRIPTION(ERR_FAILED, "deserialized aligned size is different");
    }
    if (layout_left.deserialized.type_bit_size != layout_right.deserialized.type_bit_size) {
        RETURN_ERROR_DESCRIPTION(ERR_FAILED, "deserialized type size is different");
    }
    return a_util::result::SUCCESS;
}

std::vector<std::pair<ElementLayout, CodecIndex>> getAllStaticLayouts(
    const ChildElementAccess& child_element, bool checking_dynamics)
{
    std::vector<std::pair<ElementLayout, CodecIndex>> result_vector;
    auto leaf_count = child_element.getArrayLeafCount();
    // we have a look into the it with a recommended array size of 1
    if (checking_dynamics && leaf_count == 0) {
        leaf_count = child_element.getLeafCount();
    }
    for (size_t leaf_index = 0; leaf_index < leaf_count; ++leaf_index) {
        ElementLayout layout;
        CodecIndex index;
        if (child_element.findCodecLayoutByLeafIndex(leaf_index, layout, index, true)) {
            result_vector.push_back({layout, index});
        }
    }
    return result_vector;
}

bool isLayoutBinarySubset(const ElementLayout& layout_left, const ElementLayout& layout_right)
{
    // padding bytes are defined as an array of uint8_t elements
    if (layout_left.type_info->getType() != layout_right.type_info->getType()) {
        if (layout_right.type_info->getType() == ElementType::cet_uint8) {
            // this might be a padding byte
            // we go ahead within the padding bytes until we find the current position left
            if (layout_left.deserialized.bit_offset != layout_right.deserialized.bit_offset) {
                return false;
            }
            else {
                return true;
            }
        }
        else {
            return false;
        }
    }
    if (layout_left.deserialized.bit_offset != layout_right.deserialized.bit_offset) {
        return false;
    }
    if (layout_left.deserialized.type_bit_size != layout_right.deserialized.type_bit_size) {
        return false;
    }
    return true;
}

size_t findEqualLeafLayout(const ElementLayout& layout_left,
                           size_t begin_right_leaf_index,
                           const std::vector<std::pair<ElementLayout, CodecIndex>>& layouts_right)
{
    for (; begin_right_leaf_index < layouts_right.size(); ++begin_right_leaf_index) {
        if (isLayoutBinarySubset(layout_left, layouts_right[begin_right_leaf_index].first)) {
            return begin_right_leaf_index;
        }
    }
    return static_cast<size_t>(-1);
}
} // namespace

a_util::result::Result ChildElementAccess::isBinaryEqual(const ChildElementAccess& other,
                                                         bool checking_dynamics) const
{
    // check for static leaf count equality first to fasten up
    if (other.getArrayLeafCount() != getArrayLeafCount()) {
        RETURN_ERROR_DESCRIPTION(ERR_FAILED,
                                 "The structs '%s' and '%s' have different amounts of elements.",
                                 getLayoutBase().type_info->getTypeName(),
                                 other.getLayoutBase().type_info->getTypeName());
    }
    else {
        // we need to retrieve the whole layout of each element
        // the binary equality does not need the same names, but the same layout for each defined
        // leaf element independent on the structures
        auto layouts_left = getAllStaticLayouts(*this, checking_dynamics);
        auto layouts_right = getAllStaticLayouts(other, checking_dynamics);
        if (layouts_left.size() != layouts_right.size()) {
            RETURN_ERROR_DESCRIPTION(
                ERR_FAILED,
                "The structs '%s' and '%s' have different amounts of elements.",
                getLayoutBase().type_info->getTypeName(),
                other.getLayoutBase().type_info->getTypeName());
        }
        size_t current_leaf_index = 0;
        for (const auto& current_value: layouts_left) {
            auto result =
                isLayoutBinaryEqual(current_value.first, layouts_right[current_leaf_index].first);
            if (!result) {
                RETURN_ERROR_DESCRIPTION(
                    result.getErrorCode(),
                    "%s in element '%s' and element '%s'",
                    result.getDescription(),
                    getName(current_value.second, 0).c_str(),
                    other.getName(layouts_right[current_leaf_index].second, 0).c_str());
            }
            ++current_leaf_index;
        }
        // check for dynamic stuff
        // there we only check if the subitems are defined with binary equalality
        auto dynamic_element_count_left = _sub_elements.getDynamicCount();
        if (dynamic_element_count_left > 0) {
            auto dynamic_element_count_right = other._sub_elements.getDynamicCount();
            if (dynamic_element_count_right != dynamic_element_count_left) {
                RETURN_ERROR_DESCRIPTION(
                    ERR_FAILED,
                    "The dynamic content differs in dynamic element count for %s and %s",
                    getBaseName().c_str(),
                    other.getBaseName().c_str());
            }
            size_t index_left = _sub_elements.getStaticCount();
            size_t index_right = other._sub_elements.getStaticCount();
            for (; index_left < _sub_elements.getSize(); ++index_left) {
                RETURN_IF_FAILED(_sub_elements[index_left].isBinaryEqual(
                    other._sub_elements[index_right], true));
                ++index_right;
            }
        }
        return a_util::result::SUCCESS;
    }
}

a_util::result::Result ChildElementAccess::isBinarySubset(const ChildElementAccess& other,
                                                          bool checking_dynamics) const
{
    // we need to retrieve the whole layout of each element
    // the binary equality does not need the same names, but the same layout for each defined
    // leaf element independent on the structures
    auto layouts_left = getAllStaticLayouts(*this, checking_dynamics);
    auto layouts_right = getAllStaticLayouts(other, checking_dynamics);

    size_t current_leaf_index_right = 0;
    // try to find a leaf for each left non padding element
    for (const auto& current_value: layouts_left) {
        if (current_value.first.type_info->getType() != ElementType::cet_uint8) {
            current_leaf_index_right =
                findEqualLeafLayout(current_value.first, current_leaf_index_right, layouts_right);
            if (current_leaf_index_right == static_cast<size_t>(-1)) {
                RETURN_ERROR_DESCRIPTION(
                    ERR_FAILED,
                    "Can not find a corrensponding layout element for %s of type %s in type %s",
                    getBaseName(current_value.second, 0).c_str(),
                    getLayoutBase().type_info->getTypeName(),
                    other.getLayoutBase().type_info->getTypeName());
            }
            ++current_leaf_index_right;
        }
    }
    // check for dynamic stuff
    // there we only check if the subitems are defined with binary equalality
    auto dynamic_element_count_left = _sub_elements.getDynamicCount();
    if (dynamic_element_count_left > 0) {
        auto dynamic_element_count_right = other._sub_elements.getDynamicCount();
        if (dynamic_element_count_right != dynamic_element_count_left) {
            RETURN_ERROR_DESCRIPTION(
                ERR_FAILED,
                "The dynamic content differs in dynamic element count for %s and %s",
                getBaseName().c_str(),
                other.getBaseName().c_str());
        }
        size_t index_left = _sub_elements.getStaticCount();
        size_t index_right = other._sub_elements.getStaticCount();
        for (; index_left < _sub_elements.getSize(); ++index_left) {
            RETURN_IF_FAILED(
                _sub_elements[index_left].isBinarySubset(other._sub_elements[index_right], true));
            ++index_right;
        }
    }
    return a_util::result::SUCCESS;
}

/************************************************************************************
 * Static Elements
 */
StaticCodecElementAccess::StaticCodecElementAccess(
    const ddl::dd::StructElementAccess& element_access, StructAccess& struct_access)
{
    //-1 because this retrieves the offset of the StructTypeAccess based to 0
    initializeStatic(
        element_access, element_access.getStructTypeAccess(static_cast<size_t>(-1)), struct_access);
}

/************************************************************************************
 * Dynamic Elements
 */

DynamicCodecElementAccess::DynamicCodecElementAccess(
    const ddl::dd::StructElementAccess& element_access, StructAccess& struct_access)
    : ChildElementAccess(),
      _is_after_dynamic(element_access.isAfterDynamic()),
      _is_dynamic(element_access.isDynamic())
{
    //-1 will retrieve the offsets of sub elements based to 0
    initializeStatic(
        element_access, element_access.getStructTypeAccess(static_cast<size_t>(-1)), struct_access);
    initializeDynamic(element_access);
}

void DynamicCodecElementAccess::initializeDynamic(
    const ddl::dd::StructElementAccess& element_access)
{
    // we keep the element access to calculate later
    if (_is_dynamic) {
        _array_element_name = element_access.getElement().getArraySize().getArraySizeElementName();
    }
}

const std::string& DynamicCodecElementAccess::getArrayElementName() const
{
    return _array_element_name;
}

size_t DynamicCodecElementAccess::resolveArraySize(const NamedCodecIndex& current_index,
                                                   size_t index_pos,
                                                   const ArraySizeResolverFunction& array_resolver)
{
    if (_is_dynamic && !_array_element_name.empty()) {
        // we resolve it only for array pos 0

        NamedCodecIndex array_size_index(current_index, index_pos, _array_element_name);
        auto resolved_array_size = array_resolver(array_size_index);

        if (_static_layout_base.array_size != 0 &&
            resolved_array_size != _static_layout_base.array_size) {
            throw std::runtime_error(std::to_string(resolved_array_size) +
                                     "!=" + std::to_string(_static_layout_base.array_size) +
                                     "If using dynamic type in arrays, each dynamic "
                                     "type must have the same size!");
        }
        _static_layout_base.array_size = resolved_array_size;
    }

    // for static array size the current index is not relevant
    // this is dynamic because of "isAfterDynamic"
    return _static_layout_base.array_size;
}

void DynamicCodecElementAccess::resolveSizes(const dd::Version& file_dd_version)
{
    if (_is_dynamic) {
        if (_static_type_size.is_dynamic) {
            resolveDynamicSize(file_dd_version);
            // update type sizes in layout base
            _static_layout_base.serialized.type_bit_size = _static_type_size.serialized_bits;
            _static_layout_base.deserialized.type_bit_size =
                _static_type_size.deserialized_bytes * byte_size_in_bits;
            _static_layout_base.deserialized.type_bit_size_aligned =
                _static_type_size.deserialized_bytes_aligned * byte_size_in_bits;
        }

        _static_layout_base.serialized.bit_size =
            _static_layout_base.array_size * _static_layout_base.serialized.type_bit_size;
        if (_static_layout_base.serialized.bit_size <
            _static_layout_base.serialized.type_bit_size) {
            _static_layout_base.serialized.type_bit_size_used =
                _static_layout_base.serialized.bit_size;
        }
        else {
            _static_layout_base.serialized.type_bit_size_used =
                _static_layout_base.serialized.type_bit_size;
        }

        size_t byte_size =
            _static_type_size.deserialized_bytes_aligned * (_static_layout_base.array_size - 1) +
            _static_type_size.deserialized_bytes;

        _static_layout_base.deserialized.bit_size = byte_size * byte_size_in_bits;
        _array_leaf_count = _leaf_count * _static_layout_base.array_size;
    }

    // after resolving sizes we can resolve the next offset
    if (_is_dynamic || _is_after_dynamic) {
        if (_next_element) {
            _next_element->resolveOffsets(*this);
        }
    }
}

void DynamicCodecElementAccess::resolveOffsets(const ChildElementAccess& previous_element)
{
    if (_is_after_dynamic) {
        const auto& previous_base_layout = previous_element.getLayoutBase();

        _static_layout_base.serialized.bit_offset =
            previous_base_layout.serialized.bit_offset + previous_base_layout.serialized.bit_size;

        size_t unaligned_pos = (previous_base_layout.deserialized.bit_offset / byte_size_in_bits) +
                               (previous_base_layout.deserialized.bit_size / byte_size_in_bits);
        unaligned_pos = calculateAlignedSizeOrPos(unaligned_pos, getElementAlignment());
        _static_layout_base.deserialized.bit_offset = unaligned_pos * byte_size_in_bits;
    }
}

bool DynamicCodecElementAccess::hasStaticPosition() const
{
    if (_is_after_dynamic) {
        // if resolved the static position is now set
        return _static_layout_base.serialized.bit_offset != 0 &&
               _static_layout_base.deserialized.bit_offset != 0;
    }
    return true;
}

/************************************************************************************
 * Main Codec
 */
StructAccess::StructAccess()
    : _init_result(ERR_NOT_INITIALIZED),
      _all_types(std::make_shared<CodecTypes>()),
      _all_constants(std::make_shared<CodecConstants>()),
      _empty_constant(new CodecConstantInfo(nullptr, nullptr)),
      _all_default_values(std::make_shared<CodecDefaultValues>()),
      _empty_default(new CodecDefaultValueInfo(a_util::variant::Variant()))
{
}

StructAccess::StructAccess(const ddl::dd::StructTypeAccess& struct_type_access,
                           ddl::dd::Version dd_version)
    : _is_dynamic(struct_type_access.isDynamic()),
      _init_result(ERR_NOT_INITIALIZED),
      _dd_version(dd_version),
      _all_types(std::make_shared<CodecTypes>()),
      _all_constants(std::make_shared<CodecConstants>()),
      _empty_constant(new CodecConstantInfo(nullptr, nullptr)),
      _all_default_values(std::make_shared<CodecDefaultValues>()),
      _empty_default(new CodecDefaultValueInfo(a_util::variant::Variant()))
{
    try {
        _single_codec_access_element.initializeStatic(nullptr, struct_type_access, *this);
        _static_struct_size = _single_codec_access_element.getTypeSize();
        _init_result = a_util::result::SUCCESS;
    }
    catch (std::exception& ex) {
        _init_result = a_util::result::Result(
            ERR_NOT_INITIALIZED, ex.what(), __LINE__, __FILE__, __FUNCTION__);
    }

    if (!_is_dynamic) {
        // dynamic and static is the same
        _dynamic_struct_size = _static_struct_size;
    }
}

a_util::result::Result StructAccess::getInitResult() const
{
    return _init_result;
}

size_t StructAccess::getStaticBufferSize(DataRepresentation representation) const
{
    return representation == DataRepresentation::deserialized ?
               _static_struct_size.deserialized_bytes :
               bitToBytes(_static_struct_size.serialized_bits);
}

size_t StructAccess::getBufferSize(DataRepresentation representation) const
{
    return representation == DataRepresentation::deserialized ?
               _dynamic_struct_size.deserialized_bytes :
               bitToBytes(_dynamic_struct_size.serialized_bits);
}

ElementLayout StructAccess::getCodecElementLayout(const CodecIndex& index) const
{
    if (index.hasLayout()) {
        return index.getLayout();
    }
    ElementLayout layout;
    _single_codec_access_element.findCodecLayout(index, 0, layout, !_resolved_dynamics);
    return layout;
}

ElementLayout StructAccess::getCodecElementLayout(const CodecIndex& index,
                                                  std::string& fullname) const
{
    ElementLayout layout;
    _single_codec_access_element.findCodecLayout(index, 0, layout, fullname, !_resolved_dynamics);
    return layout;
}

CodecIndex StructAccess::resolve(size_t leaf_index) const
{
    CodecIndex index;
    ElementLayout layout;
    if (_single_codec_access_element.findCodecLayoutByLeafIndex(
            leaf_index, layout, index, !_resolved_dynamics)) {
        index.setLayout(layout);
        return index;
    }
    else {
        throw std::runtime_error("Index " + std::to_string(leaf_index) + " not found");
    }
}

void StructAccess::resolve(CodecIndex& index, bool force_reset) const
{
    if (index.hasLayout() && !force_reset) {
        // this is a shortcut for optimization
        // If the codec index does not fit for this struct access it might be the same behaviour
        // like before with the leaf_index (if you give an size_t leaf_index) into the wrong
        // instance
        return;
    }
    ElementLayout layout;
    _single_codec_access_element.findCodecLayout(index, 0, layout, !_resolved_dynamics);
    index.setLayout(layout);
    return;
}

CodecIndex StructAccess::resolve(const NamedCodecIndex& named_index) const
{
    ElementLayout layout;
    CodecIndex find_index;
    if (named_index.getSize() == 0) {
        find_index = CodecIndex(0);
        _single_codec_access_element.findCodecLayout(find_index, 0, layout, !_resolved_dynamics);
        // we want the whole element size when getting or setting a memcopy
        // we manipulate the size (otherwise we need to add an if-statement within the
        // ElementsAccessor ... performance!)
        layout.deserialized.type_bit_size =
            getBufferSize(DataRepresentation::deserialized) * byte_size_in_bits;
    }
    else {
        _single_codec_access_element.findCodecLayoutByName(
            named_index, 0, layout, find_index, !_resolved_dynamics);
        if (layout.array_size > 1 && layout.array_pos == 0 &&
            !named_index.lastResolvedWithArrayPos()) {
            // we want the whole element size when getting or setting a memcopy
            // we manipulate the size (otherwise we need to add an if-statement within the
            // ElementsAccessor ... performance!)
            layout.deserialized.type_bit_size = layout.deserialized.bit_size;
        }
    }

    find_index.setLayout(layout);
    return find_index;
}

void StructAccess::resolveDynamic(ArraySizeResolverFunction array_resolver)
{
    _resolved_dynamics = true;
    try {
        if (_init_result) {
            _single_codec_access_element.resolveDynamics(
                NamedCodecIndex(), array_resolver, _dd_version);
            resolveDynamicStructSize();
        }
    }
    catch (const std::runtime_error& ex) {
        _init_result = a_util::result::Result(
            ERR_NOT_INITIALIZED, ex.what(), __LINE__, __FILE__, __FUNCTION__);
    }
}

void StructAccess::resolveDynamicStructSize()
{
    _single_codec_access_element.resolveDynamicSize(_dd_version);
    _dynamic_struct_size = _single_codec_access_element.getTypeSize();
}

std::shared_ptr<StructAccess> StructAccess::makeResolvedCodecAccess() const
{
    return std::make_shared<StructAccess>(*this);
}

bool StructAccess::hasEnums() const
{
    return _has_enums;
}

size_t StructAccess::getElementChildCount(const CodecIndex& codec_index, bool only_static) const
{
    if (codec_index.getSize() == 0) {
        return _single_codec_access_element.getSubElementCount(only_static);
    }
    return _single_codec_access_element.getSubElementCount(codec_index, 0, only_static);
}

size_t StructAccess::getLeafIndexCount() const
{
    // single_codec_access_element has always array size 1
    return _single_codec_access_element.getArrayLeafCount();
}

std::string StructAccess::getName(const CodecIndex& codec_index) const
{
    return _single_codec_access_element.getName(codec_index, 0);
}

const std::string& StructAccess::getBaseName(const CodecIndex& codec_index) const
{
    return _single_codec_access_element.getBaseName(codec_index, 0);
}

bool StructAccess::isDynamic() const
{
    return _is_dynamic;
}

namespace {
/************************************************************************************
 * Supported POD types
 */
class SupportedTypes {
private:
    SupportedTypes()
        : _supported_types{
              {"tBool", ElementType::cet_bool},      {"bool", ElementType::cet_bool},
              {"tChar", ElementType::cet_int8},      {"char", ElementType::cet_int8},
              {"tInt8", ElementType::cet_int8},      {"int8_t", ElementType::cet_int8},
              {"tInt16", ElementType::cet_int16},    {"int16_t", ElementType::cet_int16},
              {"tInt32", ElementType::cet_int32},    {"int32_t", ElementType::cet_int32},
              {"tInt64", ElementType::cet_int64},    {"int64_t", ElementType::cet_int64},
              {"tUInt8", ElementType::cet_uint8},    {"uint8_t", ElementType::cet_uint8},
              {"tUInt16", ElementType::cet_uint16},  {"uint16_t", ElementType::cet_uint16},
              {"tUInt32", ElementType::cet_uint32},  {"uint32_t", ElementType::cet_uint32},
              {"tUInt64", ElementType::cet_uint64},  {"uint64_t", ElementType::cet_uint64},
              {"tFloat32", ElementType::cet_float},  {"float", ElementType::cet_float},
              {"tFloat64", ElementType::cet_double}, {"double", ElementType::cet_double}}
    {
    }

public:
    static const SupportedTypes& getInst()
    {
        static const SupportedTypes s_types;
        return s_types;
    }
    ElementType getType(const std::string& data_type_name) const
    {
        const auto& found = _supported_types.find(data_type_name);
        if (found != _supported_types.end()) {
            return found->second;
        }
        else {
            return ElementType::cet_user_type;
        }
    }

private:
    const std::unordered_map<std::string, ElementType> _supported_types;
};
} // namespace

const CodecTypeInfo* StructAccess::getOrCreateCodecTypeInfo(const dd::DataType& data_type)
{
    const auto found = _all_types->find(data_type.getName());
    if (found != _all_types->end()) {
        return found->second.get();
    }
    else {
        auto& new_type = (*_all_types)[data_type.getName()];
        new_type.reset(new CodecTypeInfo(data_type.getName(),
                                         SupportedTypes::getInst().getType(data_type.getName())));
        return new_type.get();
    }
}

#define TRANSFORM_TO_TYPE(__type)                                                                  \
    case a_util::variant::VariantType::VT_##__type: {                                              \
        variant_value = variant_value.as##__type();                                                \
        break;                                                                                     \
    }

const CodecTypeInfo* StructAccess::getOrCreateCodecTypeInfo(const dd::EnumType& enum_type)
{
    const auto found = _all_types->find(enum_type.getName());
    if (found != _all_types->end()) {
        return found->second.get();
    }
    else {
        auto supported_element_type =
            SupportedTypes::getInst().getType(enum_type.getDataTypeName());
        AccessEnumType access_enum_type;
        for (const auto& ref_value: enum_type.getElements()) {
            a_util::variant::Variant variant_value = (ref_value.second->getValue().c_str());
            switch (static_cast<a_util::variant::VariantType>(supported_element_type)) {
                TRANSFORM_TO_TYPE(Bool)
                TRANSFORM_TO_TYPE(Int8)
                TRANSFORM_TO_TYPE(UInt8)
                TRANSFORM_TO_TYPE(Int16)
                TRANSFORM_TO_TYPE(UInt16)
                TRANSFORM_TO_TYPE(Int32)
                TRANSFORM_TO_TYPE(UInt32)
                TRANSFORM_TO_TYPE(Int64)
                TRANSFORM_TO_TYPE(UInt64)
                TRANSFORM_TO_TYPE(Float)
                TRANSFORM_TO_TYPE(Double)
            default:
                break;
            };
            access_enum_type.insert(std::make_pair(ref_value.second->getName(), variant_value));
        }
        auto& new_type = (*_all_types)[enum_type.getName()];
        new_type.reset(
            new CodecTypeInfo(enum_type.getName(), supported_element_type, access_enum_type));
        _has_enums = true;
        return new_type.get();
    }
}

const CodecTypeInfo* StructAccess::getOrCreateCodecTypeInfo(const dd::StructType& struct_type)
{
    const auto found = _all_types->find(struct_type.getName());
    if (found != _all_types->end()) {
        return found->second.get();
    }
    else {
        auto& new_type = (*_all_types)[struct_type.getName()];
        new_type.reset(new CodecTypeInfo(struct_type.getName(), ElementType::cet_sub_codec));
        return new_type.get();
    }
}

const CodecConstantInfo* StructAccess::findConstantInfo(const std::string& constant_name)
{
    if (!constant_name.empty()) {
        const auto found_constant_info = _all_constants->find(constant_name);
        if (found_constant_info != _all_constants->end()) {
            return found_constant_info->second.get();
        }
        for (const auto& current_type: *_all_types) {
            const auto found_value = current_type.second->_enum_access_type.find(constant_name);
            if (found_value != current_type.second->_enum_access_type.end()) {
                auto& new_constant = (*_all_constants)[constant_name];
                new_constant.reset(
                    new CodecConstantInfo(found_value->first.c_str(), &found_value->second));
                return new_constant.get();
            }
        }
    }
    return _empty_constant.get();
}

const CodecDefaultValueInfo* StructAccess::getOrCreateDefaultValueInfo(
    ElementType type_of_element, const std::string& default_value)
{
    if (type_of_element == ElementType::cet_empty || default_value.empty()) {
        return _empty_default.get();
    }
    else {
        const auto found_default_value_info = _all_default_values->find(default_value);
        if (found_default_value_info != _all_default_values->end()) {
            return found_default_value_info->second.get();
        }
        else {
            auto& new_default_value = (*_all_default_values)[default_value];
            new_default_value.reset(new CodecDefaultValueInfo(default_value.c_str()));
            return new_default_value.get();
        }
    }
}

a_util::result::Result StructAccess::isBinaryEqual(const StructAccess& other) const
{
    // check for static leaf count equality first to fasten up
    if (other.getLeafIndexCount() != getLeafIndexCount()) {
        RETURN_ERROR_DESCRIPTION(
            ERR_FAILED,
            "The structs '%s' and '%s' have different amounts of elements.",
            _single_codec_access_element.getLayoutBase().type_info->getTypeName(),
            other._single_codec_access_element.getLayoutBase().type_info->getTypeName());
    }
    else {
        return _single_codec_access_element.isBinaryEqual(other._single_codec_access_element,
                                                          false);
    }
}

a_util::result::Result StructAccess::isBinarySubset(const StructAccess& other) const
{
    // we do not check the leaf count, because of the possible padding bytes
    return _single_codec_access_element.isBinarySubset(other._single_codec_access_element, false);
}

} // namespace codec
} // namespace ddl
