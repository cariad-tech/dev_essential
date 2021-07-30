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

#include "ddl/dd/ddelement.h"

#include "dd_alignment_calculation.h"
#include "ddl/dd/dd_typeinfomodel.h"

namespace ddl {

namespace detail {
DEF_GETINFO(dd::TypeInfo, dd::DataType);
DEF_GETINFO(dd::TypeInfo, dd::EnumType);
DEF_GETINFO(dd::TypeInfo, dd::StructType);

template <typename TYPE>
void resetAlignmentByTypeAlignment(const TYPE& ddl_type, dd::StructType::Element& element)
{
    if (element.getAlignment() == dd::Alignment::e_invalid) {
        element.setAlignment(dd::obtainElementsAlignment(ddl_type, element.getAlignment()));
    }
}
template <typename TYPE>
void resetAlignmentByTypeAlignment(const TYPE& ddl_type,
                                   dd::StructType::Element& element,
                                   const dd::DataDescription& source_data_defintion)
{
    if (element.getAlignment() == dd::Alignment::e_invalid) {
        element.setAlignment(
            dd::obtainElementsAlignment(ddl_type, source_data_defintion, element.getAlignment()));
    }
}
} // namespace detail

DDElement::DDElement(const std::string& name,
                     const dd::DataType& data_type,
                     const DDElement::Deserialized& deserialized_info,
                     const DDElement::Serialized& serialized_info,
                     const dd::ArraySize& array_size,
                     const std::string& description,
                     const std::string& comment,
                     const dd::BaseUnit& base_unit,
                     const std::string& minimum_value,
                     const std::string& maximum_value,
                     const std::string& default_value,
                     const std::string& scale,
                     const std::string& offset)
    : _element(name,
               data_type.getName(),
               deserialized_info,
               serialized_info,
               array_size,
               description,
               comment,
               base_unit.getName(),
               "",
               minimum_value,
               maximum_value,
               default_value,
               scale,
               offset)
{
    detail::resetAlignmentByTypeAlignment(data_type, _element);
    if (!base_unit.getName().empty()) {
        _dd.getBaseUnits().add(base_unit);
    }
    _dd.getDataTypes().add(data_type);
}

DDElement::DDElement(const std::string& name,
                     const dd::DataType& data_type,
                     const Deserialized& deserialized_info,
                     const Serialized& serialized_info,
                     const dd::ArraySize& array_size,
                     const std::string& description,
                     const std::string& comment,
                     const DDUnit& unit,
                     const std::string& minimum_value,
                     const std::string& maximum_value,
                     const std::string& default_value,
                     const std::string& scale,
                     const std::string& offset)
    : _element(name,
               data_type.getName(),
               deserialized_info,
               serialized_info,
               array_size,
               description,
               comment,
               unit.getUnit().getName(),
               "",
               minimum_value,
               maximum_value,
               default_value,
               scale,
               offset)
{
    detail::resetAlignmentByTypeAlignment(data_type, _element);
    // we add the definition of that unit
    _dd.add(unit.getDD());
    _dd.getDataTypes().add(data_type);
}

DDElement::DDElement(const std::string& name,
                     const DDDataType& data_type,
                     const DDElement::Deserialized& deserialized_info,
                     const DDElement::Serialized& serialized_info,
                     const dd::ArraySize& array_size,
                     const std::string& description,
                     const std::string& comment,
                     const dd::BaseUnit& base_unit,
                     const std::string& minimum_value,
                     const std::string& maximum_value,
                     const std::string& default_value,
                     const std::string& scale,
                     const std::string& offset)
    : _element(name,
               data_type.getDataType().getName(),
               deserialized_info,
               serialized_info,
               array_size,
               description,
               comment,
               base_unit.getName(),
               "",
               minimum_value,
               maximum_value,
               default_value,
               scale,
               offset)
{
    detail::resetAlignmentByTypeAlignment(data_type.getDataType(), _element);
    if (!base_unit.getName().empty()) {
        _dd.getBaseUnits().add(base_unit);
    }
    _dd.add(data_type.getDD());
}

DDElement::DDElement(const std::string& name,
                     const DDDataType& data_type,
                     const Deserialized& deserialized_info,
                     const Serialized& serialized_info,
                     const dd::ArraySize& array_size,
                     const std::string& description,
                     const std::string& comment,
                     const DDUnit& unit,
                     const std::string& minimum_value,
                     const std::string& maximum_value,
                     const std::string& default_value,
                     const std::string& scale,
                     const std::string& offset)
    : _element(name,
               data_type.getDataType().getName(),
               deserialized_info,
               serialized_info,
               array_size,
               description,
               comment,
               unit.getUnit().getName(),
               "",
               minimum_value,
               maximum_value,
               default_value,
               scale,
               offset)
{
    detail::resetAlignmentByTypeAlignment(data_type.getDataType(), _element);
    // we add the definition of that unit
    _dd.add(unit.getDD());
    _dd.add(data_type.getDD());
}

DDElement::DDElement(const std::string& name,
                     const DDEnum& enum_type,
                     const DDElement::Deserialized& deserialized_info,
                     const DDElement::Serialized& serialized_info,
                     const dd::ArraySize& array_size,
                     const std::string& description,
                     const std::string& comment,
                     const dd::BaseUnit& base_unit,
                     const std::string& value,
                     const std::string& minimum_value,
                     const std::string& maximum_value,
                     const std::string& default_value,
                     const std::string& scale,
                     const std::string& offset)
    : _element(name,
               enum_type.getEnumType().getName(),
               deserialized_info,
               serialized_info,
               array_size,
               description,
               comment,
               base_unit.getName(),
               value,
               minimum_value,
               maximum_value,
               default_value,
               scale,
               offset)
{
    detail::resetAlignmentByTypeAlignment(enum_type.getEnumType(), _element, enum_type.getDD());
    if (!base_unit.getName().empty()) {
        _dd.getBaseUnits().add(base_unit);
    }
    // check if value is part of the enum
    if (!value.empty()) {
        auto elem_of_enum = enum_type.getEnumType().getElements().get(value);
        if (!elem_of_enum) {
            throw dd::Error("DDElement::DDElement",
                            {name, enum_type.getEnumType().getName()},
                            "the value '" + value + "' does not exist in enum type '" +
                                enum_type.getEnumType().getName() + "'");
        }
    }
    _dd.add(enum_type.getEnumType(), enum_type.getDD());
}

DDElement::DDElement(const std::string& name,
                     const DDEnum& enum_type,
                     const DDElement::Deserialized& deserialized_info,
                     const DDElement::Serialized& serialized_info,
                     const dd::ArraySize& array_size,
                     const std::string& description,
                     const std::string& comment,
                     const DDUnit& unit,
                     const std::string& value,
                     const std::string& minimum_value,
                     const std::string& maximum_value,
                     const std::string& default_value,
                     const std::string& scale,
                     const std::string& offset)
    : _element(name,
               enum_type.getEnumType().getName(),
               deserialized_info,
               serialized_info,
               array_size,
               description,
               comment,
               unit.getUnit().getName(),
               value,
               minimum_value,
               maximum_value,
               default_value,
               scale,
               offset)
{
    detail::resetAlignmentByTypeAlignment(enum_type.getEnumType(), _element, enum_type.getDD());
    _dd.add(unit.getDD());
    // check if value is part of the enum
    if (!value.empty()) {
        auto elem_of_enum = enum_type.getEnumType().getElements().get(value);
        if (!elem_of_enum) {
            throw dd::Error("DDElement::DDElement",
                            {name, enum_type.getEnumType().getName()},
                            "the value '" + value + "' does not exist in enum type '" +
                                enum_type.getEnumType().getName() + "'");
        }
    }
    _dd.add(enum_type.getEnumType(), enum_type.getDD());
}

DDElement::DDElement(const std::string& name,
                     const DDStructure& structure_type,
                     const Deserialized& deserialized_info,
                     const Serialized& serialized_info,
                     const dd::ArraySize& array_size,
                     const std::string& description,
                     const std::string& comment)
    : _element(name,
               structure_type.getStructType().getName(),
               deserialized_info,
               serialized_info,
               array_size,
               description,
               comment)
{
    detail::resetAlignmentByTypeAlignment(structure_type.getStructType(), _element);
    _dd.add(structure_type.getStructType(), structure_type.getDD());
}

const dd::StructType::Element& DDElement::getElement() const
{
    return _element;
}

const dd::DataDefinition& DDElement::getDD() const
{
    return _dd;
}

} // namespace ddl
