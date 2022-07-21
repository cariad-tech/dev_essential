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

#include "dd_alignment_calculation.h"

#include <a_util/strings.h>
#include <ddl/dd/dd_typeinfomodel.h>
#include <ddl/dd/ddcompare.h>
#include <ddl/dd/ddstring.h>
#include <ddl/dd/ddstructure.h>
#include <ddl/utilities/std_to_string.h>

namespace ddl {

DDStructure::DDStructure(DDStructure&& other)
    : _dd(std::move(other._dd)),
      _initial_alignment(std::move(other._initial_alignment)),
      _struct_type(std::move(other._struct_type))
{
}

DDStructure::DDStructure(const DDStructure& other)
    : _dd(other._dd), _initial_alignment(other._initial_alignment)
{
    _struct_type = _dd.getStructTypes().access(other.getStructName());
}

DDStructure& DDStructure::operator=(DDStructure&& other)
{
    _dd = std::move(other._dd);
    _initial_alignment = std::move(other._initial_alignment);
    _struct_type = std::move(other._struct_type);
    return *this;
}

DDStructure& DDStructure::operator=(const DDStructure& other)
{
    _dd = other._dd;
    _initial_alignment = other._initial_alignment;
    _struct_type = _dd.getStructTypes().access(other.getStructName());
    return *this;
}

DDStructure::DDStructure(const std::string& name,
                         uint32_t struct_version,
                         dd::OptionalSize alignment,
                         const std::string& comment,
                         const dd::Version& ddl_version)
    : _initial_alignment(alignment)
{
    _dd.setVersion(ddl_version);
    _dd.getStructTypes().emplace(
        dd::StructType(name, std::to_string(struct_version), alignment, comment, ddl_version));
    _dd.validate();
    if (!_dd.isValid(dd::ValidationInfo::good_enough)) {
        throw dd::Error(
            "DDStructure::DDStructure",
            {name},
            a_util::strings::join(dd::transformProblemList(_dd.getValidationProtocol()), "\n"));
    }
    _struct_type = _dd.getStructTypes().access(name);
}

DDStructure::DDStructure(const std::string& name,
                         const std::string& xml_string,
                         const dd::Version& ddl_xml_file_version_to_parse)
{
    // this will throw if invalid AND if the struct does not exists
    _dd = DDString::fromXMLString(name, xml_string, ddl_xml_file_version_to_parse);
    _struct_type = _dd.getStructTypes().access(name);
}

std::string DDStructure::getStructDescription() const
{
    return DDString::toXMLString(_dd);
}

std::string DDStructure::getStructName() const
{
    return _struct_type->getName();
}

bool DDStructure::isCompatible(const DDStructure& other) const
{
    return isOk(
        DDCompare::isBinaryEqual(getStructName(), getDD(), other.getStructName(), other.getDD()));
}

bool DDStructure::isEqual(const DDStructure& other) const
{
    return isOk(DDCompare::isEqual(
        getStructType(), getDD(), other.getStructType(), other.getDD(), DDCompare::icf_everything));
}

size_t DDStructure::getSize() const
{
    return _dd.getStructTypeAccess(_struct_type->getName()).getStaticStructSize();
}

size_t DDStructure::getAlignment() const
{
    return _struct_type->getAlignment();
}

namespace {

void addElementBaseType(dd::StructType& struct_type,
                        const std::string& element_name,
                        const std::string& type_name,
                        size_t array_size,
                        size_t type_alignment,
                        const std::string& constant_value = {})
{
    struct_type.getElements().emplace(dd::StructType::Element(
        element_name,
        type_name,
        {type_alignment},
        dd::StructType::SerializedInfo({}, dd::ByteOrderDefault::getPlatformDefault()),
        array_size,
        {},
        {},
        {},
        constant_value));
}

void checkAndSetAlignment(const dd::OptionalSize& initialized_struct_alignment,
                          dd::StructType& struct_type,
                          size_t used_element_alignment)
{
    // we change the alignment only if it did not initalized with a valid one
    if (!(initialized_struct_alignment &&
          dd::AlignmentValidation::isValid(*initialized_struct_alignment))) {
        const auto current_struct_alignment = struct_type.getAlignment();
        auto reset_alignment = false;
        if (current_struct_alignment &&
            dd::AlignmentValidation::isValid(*current_struct_alignment)) {
            if (*current_struct_alignment < used_element_alignment) {
                reset_alignment = true;
            }
        }
        else {
            reset_alignment = true;
        }
        if (reset_alignment) {
            struct_type.setAlignment(used_element_alignment);
        }
    }
}

} // namespace
DDStructure& DDStructure::addElement(const std::string& element_name,
                                     const dd::DataType& data_type,
                                     size_t array_size)
{
    // this will always add the data type given
    _dd.add(data_type, {});
    const auto type_alignment = dd::obtainElementsAlignment(data_type, {});
    addElementBaseType(
        *_struct_type, element_name, data_type.getName(), array_size, type_alignment);
    checkAndSetAlignment(_initial_alignment, *_struct_type, type_alignment);
    return *this;
}

DDStructure& DDStructure::addElement(const std::string& element_name,
                                     const dd::DataType& data_type,
                                     size_t array_size,
                                     size_t alignment)
{
    // this will always add the data type given
    _dd.add(data_type, {});
    addElementBaseType(*_struct_type, element_name, data_type.getName(), array_size, alignment);
    checkAndSetAlignment(_initial_alignment, *_struct_type, alignment);
    return *this;
}

DDStructure& DDStructure::addElement(const std::string& element_name,
                                     const DDDataType& data_type,
                                     size_t array_size)
{
    // this will always add the data type given
    _dd.add(data_type.getDD());
    const auto type_alignment = dd::obtainElementsAlignment(data_type.getDataType(), {});
    addElementBaseType(
        *_struct_type, element_name, data_type.getDataType().getName(), array_size, type_alignment);
    checkAndSetAlignment(_initial_alignment, *_struct_type, type_alignment);
    return *this;
}

DDStructure& DDStructure::addElement(const std::string& element_name,
                                     const DDDataType& data_type,
                                     size_t array_size,
                                     size_t alignment)
{
    // this will always add the data type given
    _dd.add(data_type.getDD());
    addElementBaseType(
        *_struct_type, element_name, data_type.getDataType().getName(), array_size, alignment);
    checkAndSetAlignment(_initial_alignment, *_struct_type, alignment);
    return *this;
}

DDStructure& DDStructure::addElement(const std::string& element_name,
                                     const DDStructure& struct_type,
                                     size_t array_size)
{
    // this will always add the struct type given
    _dd.add(struct_type.getStructType(), struct_type.getDD());
    const auto type_alignment = dd::obtainElementsAlignment(struct_type.getStructType(), {});
    addElementBaseType(
        *_struct_type, element_name, struct_type.getStructName(), array_size, type_alignment);
    checkAndSetAlignment(_initial_alignment, *_struct_type, type_alignment);
    return *this;
}

DDStructure& DDStructure::addElement(const std::string& element_name,
                                     const DDStructure& struct_type,
                                     size_t array_size,
                                     size_t alignment)
{
    // this will always add the struct type given
    _dd.add(struct_type.getStructType(), struct_type.getDD());
    addElementBaseType(
        *_struct_type, element_name, struct_type.getStructName(), array_size, alignment);
    checkAndSetAlignment(_initial_alignment, *_struct_type, alignment);
    return *this;
}

DDStructure& DDStructure::addElement(const std::string& element_name,
                                     const DDEnum& enum_type,
                                     size_t array_size,
                                     const std::string& constant_value)
{
    if (!constant_value.empty()) {
        auto found_constant = enum_type.getEnumType().getElements().get(constant_value);
        if (!found_constant) {
            throw dd::Error("DDStructure::addElement",
                            {element_name, enum_type.getEnumType().getName()},
                            "the value '" + constant_value + "' does not exist in enum type '" +
                                enum_type.getEnumType().getName() + "'");
        }
    }
    _dd.add(enum_type.getDD());
    const auto type_alignment =
        dd::obtainElementsAlignment(enum_type.getEnumType(), enum_type.getDD(), {});
    addElementBaseType(*_struct_type,
                       element_name,
                       enum_type.getEnumType().getName(),
                       array_size,
                       type_alignment,
                       constant_value);
    checkAndSetAlignment(_initial_alignment, *_struct_type, type_alignment);
    return *this;
}

DDStructure& DDStructure::addElement(const std::string& element_name,
                                     const DDEnum& enum_type,
                                     size_t array_size,
                                     size_t alignment,
                                     const std::string& constant_value)
{
    if (!constant_value.empty()) {
        auto found_constant = enum_type.getEnumType().getElements().get(constant_value);
        if (!found_constant) {
            throw dd::Error("DDStructure::addElement",
                            {element_name, enum_type.getEnumType().getName()},
                            "the value '" + constant_value + "' does not exist in enum type '" +
                                enum_type.getEnumType().getName() + "'");
        }
    }
    _dd.add(enum_type.getDD());
    addElementBaseType(*_struct_type,
                       element_name,
                       enum_type.getEnumType().getName(),
                       array_size,
                       alignment,
                       constant_value);
    checkAndSetAlignment(_initial_alignment, *_struct_type, alignment);
    return *this;
}

DDStructure& DDStructure::addElement(const std::string& element_name,
                                     const DDEnum& enum_type,
                                     const std::string& constant_value)
{
    return addElement(element_name, enum_type, 1, constant_value);
}

DDStructure& DDStructure::addElement(const DDElement& element)
{
    _dd.add(element.getDD());
    _struct_type->getElements().add(element.getElement());
    auto elem_name = element.getElement().getName();
    _dd.validate(true);
    if (!_dd.isValid(dd::ValidationInfo::good_enough)) {
        auto error_msg =
            a_util::strings::join(dd::transformProblemList(_dd.getValidationProtocol()), "\n");
        _struct_type->getElements().remove(elem_name);
        throw dd::Error("DDStructure::addElement", {elem_name}, error_msg);
    }
    checkAndSetAlignment(_initial_alignment, *_struct_type, element.getElement().getAlignment());
    return *this;
}

DDStructure& DDStructure::addElements(const std::vector<DDElement>& elements)
{
    for (const auto& current_elem: elements) {
        addElement(current_elem);
    }
    return *this;
}

const dd::DataDefinition& DDStructure::getDD() const
{
    return _dd;
}

const dd::StructType& DDStructure::getStructType() const
{
    return *_struct_type;
}

void DDStructure::popLastElement()
{
    _struct_type->getElements().popBack();
}

} // namespace ddl
