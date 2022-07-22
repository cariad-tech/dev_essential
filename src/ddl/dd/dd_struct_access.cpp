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

#include <a_util/strings.h>
#include <ddl/dd/dd_struct_access.h>
#include <ddl/dd/dd_typeinfomodel.h>

namespace ddl {

namespace dd {

StructElementAccess::StructElementAccess(std::nullptr_t)
{
}

StructElementAccess::StructElementAccess(
    const std::shared_ptr<const datamodel::StructType::Element>& element,
    OptionalSize serialized_struct_bit_offset,
    OptionalSize deserialized_struct_byte_offset,
    OptionalSize array_pos)
    : _element(element),
      _element_type_info(element->getInfo<ElementTypeInfo>()),
      _serialized_struct_bit_offset(serialized_struct_bit_offset),
      _deserialized_struct_byte_offset(deserialized_struct_byte_offset),
      _array_pos(array_pos)
{
}

StructElementAccess::operator bool() const
{
    return static_cast<bool>(_element) && _element_type_info != nullptr;
}

TypeOfType StructElementAccess::getTypeOfType() const
{
    return _element_type_info->getTypeOfType();
}

std::shared_ptr<const datamodel::StructType> StructElementAccess::getStructType() const
{
    return _element_type_info->getStructType();
}

std::shared_ptr<const datamodel::EnumType> StructElementAccess::getEnumType() const
{
    return _element_type_info->getEnumType();
}

std::shared_ptr<const datamodel::DataType> StructElementAccess::getDataType() const
{
    return _element_type_info->getDataType();
}

StructTypeAccess StructElementAccess::getStructTypeAccess(size_t array_pos) const
{
    if (array_pos == (size_t)-1) {
        return {_element_type_info->getStructType(), 0, 0};
    }
    else {
        return {_element_type_info->getStructType(),
                *_element_type_info->getSerializedBitOffset(array_pos) +
                    *_serialized_struct_bit_offset,
                *_element_type_info->getDeserializedBytePos(array_pos) +
                    *_deserialized_struct_byte_offset};
    }
}

const datamodel::StructType::Element& StructElementAccess::getElement() const
{
    return *_element;
}

bool StructElementAccess::isDynamic() const
{
    return _element_type_info->isDynamic();
}

bool StructElementAccess::isAfterDynamic() const
{
    return _element_type_info->isAfterDynamic();
}

size_t StructElementAccess::getDeserializedBytePos(size_t array_size) const
{
    auto byte_pos_within_the_struct = _element_type_info->getDeserializedBytePos(array_size);
    if (_deserialized_struct_byte_offset && byte_pos_within_the_struct) {
        return *_deserialized_struct_byte_offset + *byte_pos_within_the_struct;
    }
    return 0;
}

size_t StructElementAccess::getDeserializedByteSize() const
{
    return _element_type_info->getDeserializedByteSize();
}

size_t StructElementAccess::getDeserializedTypeByteSize() const
{
    return _element_type_info->getDeserializedTypeByteSize();
}

size_t StructElementAccess::getSerializedBytePos(size_t array_size) const
{
    auto byte_pos_within_the_struct = _element_type_info->getSerializedBytePos(array_size);
    if (_serialized_struct_bit_offset && byte_pos_within_the_struct) {
        return *_serialized_struct_bit_offset / 8 + *byte_pos_within_the_struct;
    }
    return 0;
}

size_t StructElementAccess::getSerializedBitOffset(size_t array_size) const
{
    auto bit_pos_within_the_struct = _element_type_info->getSerializedBitOffset(array_size);
    if (_serialized_struct_bit_offset && bit_pos_within_the_struct) {
        return *bit_pos_within_the_struct + *_serialized_struct_bit_offset;
    }
    return 0;
}

size_t StructElementAccess::getSerializedBitSize() const
{
    return _element_type_info->getSerializedBitSize();
}

size_t StructElementAccess::getSerializedTypeBitSize() const
{
    return _element_type_info->getSerializedTypeBitSize();
}

OptionalSize StructElementAccess::getArrayPos() const
{
    return _array_pos;
}

/********************************************************************************************************************
 * StructElementAccessIterator iterator
 ********************************************************************************************************************/

StructElementAccessIterator::StructElementAccessIterator(
    datamodel::StructType::Elements::const_iterator base_it,
    datamodel::StructType::Elements::const_iterator end_it,
    OptionalSize struct_ser,
    OptionalSize struct_deser)
    : _base_it(base_it),
      _end_it(end_it),
      _struct_ser(struct_ser),
      _struct_deser(struct_deser),
      _ref(nullptr)
{
    if (base_it != end_it) {
        _ref = {*base_it, struct_ser, struct_deser, 0};
    }
}

StructElementAccessIterator::self_type StructElementAccessIterator::operator++()
{
    self_type incre = *this;
    _base_it++;
    if (_base_it != _end_it) {
        _ref = {*_base_it, _struct_ser, _struct_deser, 0};
    }
    return incre;
}
StructElementAccessIterator::self_type StructElementAccessIterator::operator++(int)
{
    _base_it++;
    return *this;
}
StructElementAccessIterator::const_reference StructElementAccessIterator::operator*() const
{
    return _ref;
}
StructElementAccessIterator::const_pointer StructElementAccessIterator::operator->() const
{
    return &_ref;
}
bool StructElementAccessIterator::operator==(const self_type& other) const
{
    return _base_it == other._base_it;
}

bool StructElementAccessIterator::operator!=(const self_type& other) const
{
    return _base_it != other._base_it;
}

/********************************************************************************************************************
 * StructTypeAccess
 ********************************************************************************************************************/
StructTypeAccess::StructTypeAccess(std::nullptr_t)
{
}

StructTypeAccess::StructTypeAccess(const std::shared_ptr<const datamodel::StructType>& struct_type)
    : _struct_type(struct_type),
      _type_info(static_cast<bool>(struct_type) ? struct_type->getInfo<TypeInfo>() : nullptr),
      _serialized_struct_as_element_offset(0),
      _deserialized_struct_as_element_offset(0)
{
}

StructTypeAccess::StructTypeAccess(const std::shared_ptr<const datamodel::StructType>& struct_type,
                                   OptionalSize serialized_struct_offset,
                                   OptionalSize deserialized_struct_offset)
    : _struct_type(struct_type),
      _type_info(static_cast<bool>(struct_type) ? struct_type->getInfo<TypeInfo>() : nullptr),
      _serialized_struct_as_element_offset(serialized_struct_offset),
      _deserialized_struct_as_element_offset(deserialized_struct_offset)
{
}

StructTypeAccess::operator bool() const
{
    return static_cast<bool>(_struct_type) && _type_info != nullptr;
}

namespace {

void findArrayPos(std::string& elem_name, OptionalSize& array_pos_found)
{
    auto right_bracket_pos = elem_name.rfind("]");
    if (right_bracket_pos != std::string::npos) {
        auto left_bracket_pos = elem_name.rfind("[");
        if (left_bracket_pos != std::string::npos && left_bracket_pos < right_bracket_pos) {
            array_pos_found = a_util::strings::toUInt64(
                elem_name.substr(left_bracket_pos + 1, right_bracket_pos - left_bracket_pos - 1));
            elem_name = elem_name.substr(0, left_bracket_pos);
        }
    }
}

const std::shared_ptr<const datamodel::StructType::Element> findElement(
    const datamodel::StructType& struct_type,
    const std::vector<std::string>& element_names,
    OptionalSize& current_serialized_struct_offset,
    OptionalSize& current_deserialized_struct_offset,
    OptionalSize& current_array_pos)
{
    if (element_names.size() > 0) {
        OptionalSize array_pos_found = {};
        auto first_name_it = element_names.begin();
        auto first_name = *first_name_it;
        // split if we have a array
        findArrayPos(first_name, array_pos_found);
        auto elem = struct_type.getElements().get(first_name);

        if (elem) {
            if (element_names.size() == 1) {
                // we found it
                current_array_pos = array_pos_found;
                return elem;
            }
            else {
                auto elem_info = elem->getInfo<ElementTypeInfo>();
                if (elem_info != nullptr) {
                    auto sub_struct = elem_info->getStructType();
                    if (sub_struct) {
                        auto byte_pos_ser = elem_info->getSerializedBytePos();
                        if (array_pos_found) {
                            byte_pos_ser = elem_info->getSerializedBytePos(*array_pos_found);
                        }
                        if (byte_pos_ser) {
                            *current_serialized_struct_offset += *byte_pos_ser;
                        }
                        auto byte_pos_deser = elem_info->getDeserializedBytePos();
                        if (array_pos_found) {
                            byte_pos_deser = elem_info->getDeserializedBytePos(*array_pos_found);
                        }
                        if (byte_pos_deser) {
                            *current_deserialized_struct_offset += *byte_pos_deser;
                            if (array_pos_found && *array_pos_found > 1) {
                                *current_deserialized_struct_offset +=
                                    (*array_pos_found - 1) * *elem_info->getDeserializedByteSize();
                            }
                        }

                        return findElement(
                            *sub_struct,
                            std::vector<std::string>(++first_name_it, element_names.end()),
                            current_serialized_struct_offset,
                            current_deserialized_struct_offset,
                            current_array_pos);
                    }
                }
            }
        }
    }
    // wrong path, element not found
    return {};
}
} // namespace

StructElementAccess StructTypeAccess::getElementByPath(const std::string name_path) const
{
    auto splitted_list_of_elements = a_util::strings::split(name_path, ".");
    if (splitted_list_of_elements.size() == 0 &&
        name_path.find_first_of("/") != std::string::npos) {
        splitted_list_of_elements = a_util::strings::split(name_path, "/");
    }

    OptionalSize current_serialized_struct_offset = _serialized_struct_as_element_offset;
    OptionalSize current_deserialized_struct_offset = _deserialized_struct_as_element_offset;
    OptionalSize array_pos = {};

    auto found_elem = findElement(*_struct_type,
                                  splitted_list_of_elements,
                                  current_serialized_struct_offset,
                                  current_deserialized_struct_offset,
                                  array_pos);
    if (found_elem) {
        return {found_elem,
                current_serialized_struct_offset,
                current_deserialized_struct_offset,
                array_pos};
    }
    else {
        return nullptr;
    }
}

size_t StructTypeAccess::getStaticStructSize() const
{
    if (*this) {
        return _type_info->getTypeByteSize();
    }
    else {
        return 0;
    }
}

size_t StructTypeAccess::getStaticSerializedBitSize() const
{
    if (*this) {
        return _type_info->getTypeBitSize();
    }
    else {
        return 0;
    }
}

size_t StructTypeAccess::getStaticUnalignedStructSize() const
{
    if (*this) {
        return _type_info->getTypeUnalignedByteSize();
    }
    else {
        return 0;
    }
}

const dd::datamodel::StructType& StructTypeAccess::getStructType() const
{
    return *_struct_type;
}

StructTypeAccess::const_iterator StructTypeAccess::cbegin() const
{
    if (*this) {
        return {_struct_type->getElements().cbegin(),
                _struct_type->getElements().cend(),
                _serialized_struct_as_element_offset,
                _deserialized_struct_as_element_offset};
    }
    return {{}, {}, {}, {}};
}

StructTypeAccess::const_iterator StructTypeAccess::begin() const
{
    return cbegin();
}

StructTypeAccess::const_iterator StructTypeAccess::cend() const
{
    if (*this) {
        return {_struct_type->getElements().cend(),
                _struct_type->getElements().cend(),
                _serialized_struct_as_element_offset,
                _deserialized_struct_as_element_offset};
    }
    return {{}, {}, {}, {}};
}

StructTypeAccess::const_iterator StructTypeAccess::end() const
{
    return cend();
}

} // namespace dd
} // namespace ddl
