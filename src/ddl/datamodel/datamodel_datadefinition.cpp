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

#include <a_util/xml.h>
#include <ddl/datamodel/datamodel_datadefinition.h>

#include <exception>
#include <utility>

namespace ddl {
namespace dd {
namespace datamodel {
/*************************************************************************************************************/
// DataDefinition
/*************************************************************************************************************/
DataDefinition::DataDefinition(const dd::Version& ddl_version)
    : ModelSubject<Header>(),
      ModelObserver<Header>(),
      ModelSubject<BaseUnit>(),
      ModelSubject<UnitPrefix>(),
      ModelSubject<Unit>(),
      ModelSubject<DataType>(),
      ModelSubject<EnumType>(),
      ModelSubject<StructType>(),
      ModelSubject<StreamMetaType>(),
      ModelSubject<Stream>(),
      InfoMap(),
      _header(std::make_shared<Header>(ddl_version)),
      _base_units(this, "datamodel::DataDefinition::BaseUnit"),
      _unit_prefixes(this, "datamodel::DataDefinition::UnitPrefixes"),
      _units(this, "datamodel::DataDefinition::Units"),
      _data_types(this, "datamodel::DataDefinition::DataTypes"),
      _enum_types(this, "datamodel::DataDefinition::EnumTypes"),
      _struct_types(this, "datamodel::DataDefinition::StructTypes"),
      _stream_meta_types(this, "datamodel::DataDefinition::StreamMetaTypes"),
      _streams(this, "datamodel::DataDefinition::Streams"),
      _empty_model(true)
{
}

DataDefinition::DataDefinition() : DataDefinition(Version())
{
}

DataDefinition::DataDefinition(const DataDefinition& other)
    : ModelSubject<Header>(),
      ModelObserver<Header>(),
      ModelSubject<BaseUnit>(),
      ModelSubject<UnitPrefix>(),
      ModelSubject<Unit>(),
      ModelSubject<DataType>(),
      ModelSubject<EnumType>(),
      ModelSubject<StructType>(),
      ModelSubject<StreamMetaType>(),
      ModelSubject<Stream>(),
      InfoMap(),
      _header(std::make_shared<Header>(*other._header.get())),
      _base_units(other._base_units),
      _unit_prefixes(other._unit_prefixes),
      _units(other._units),
      _data_types(other._data_types),
      _enum_types(other._enum_types),
      _struct_types(other._struct_types),
      _stream_meta_types(other._stream_meta_types),
      _streams(other._streams)
{
    _base_units._validator = this;
    _unit_prefixes._validator = this;
    _units._validator = this;
    _data_types._validator = this;
    _enum_types._validator = this;
    _struct_types._validator = this;
    _stream_meta_types._validator = this;
    _streams._validator = this;
    _empty_model = other._empty_model;
}

DataDefinition& DataDefinition::operator=(const DataDefinition& other)
{
    _header = std::make_shared<Header>(*other._header.get());
    _base_units = other._base_units;
    _unit_prefixes = other._unit_prefixes;
    _units = other._units;
    _data_types = other._data_types;
    _enum_types = other._enum_types;
    _struct_types = other._struct_types;
    _stream_meta_types = other._stream_meta_types;
    _streams = other._streams;
    _base_units._validator = this;
    _unit_prefixes._validator = this;
    _units._validator = this;
    _data_types._validator = this;
    _enum_types._validator = this;
    _struct_types._validator = this;
    _stream_meta_types._validator = this;
    _streams._validator = this;
    _empty_model = other._empty_model;
    return *this;
}

DataDefinition& DataDefinition::operator=(DataDefinition&& other)
{
    _header = other._header;
    _base_units = std::move(other._base_units);
    _unit_prefixes = std::move(other._unit_prefixes);
    _units = std::move(other._units);
    _data_types = std::move(other._data_types);
    _enum_types = std::move(other._enum_types);
    _struct_types = std::move(other._struct_types);
    _stream_meta_types = std::move(other._stream_meta_types);
    _streams = std::move(other._streams);
    _base_units._validator = this;
    _unit_prefixes._validator = this;
    _units._validator = this;
    _data_types._validator = this;
    _enum_types._validator = this;
    _struct_types._validator = this;
    _stream_meta_types._validator = this;
    _streams._validator = this;
    _empty_model = other._empty_model;
    return *this;
}

DataDefinition::DataDefinition(DataDefinition&& other)
    : _header(other._header),
      _base_units(std::move(other._base_units)),
      _unit_prefixes(std::move(other._unit_prefixes)),
      _units(std::move(other._units)),
      _data_types(std::move(other._data_types)),
      _enum_types(std::move(other._enum_types)),
      _struct_types(std::move(other._struct_types)),
      _stream_meta_types(std::move(other._stream_meta_types)),
      _streams(std::move(other._streams))
{
    _base_units._validator = this;
    _unit_prefixes._validator = this;
    _units._validator = this;
    _data_types._validator = this;
    _enum_types._validator = this;
    _struct_types._validator = this;
    _stream_meta_types._validator = this;
    _streams._validator = this;
    _empty_model = other._empty_model;
}

void DataDefinition::setVersion(const dd::Version& ddl_version)
{
    _empty_model = false;
    _header->setLanguageVersion(ddl_version);
}

Version DataDefinition::getVersion() const
{
    return _header->getLanguageVersion();
}

std::shared_ptr<const Header> DataDefinition::getHeader() const
{
    return _header;
}

std::shared_ptr<Header> DataDefinition::getHeader()
{
    return _header;
}

void DataDefinition::setHeader(const Header& header)
{
    _empty_model = false;
    (*_header).operator=(header);
}

const DataDefinition::BaseUnits& DataDefinition::getBaseUnits() const
{
    return _base_units;
}

DataDefinition::BaseUnits& DataDefinition::getBaseUnits()
{
    return _base_units;
}

const DataDefinition::UnitPrefixes& DataDefinition::getUnitPrefixes() const
{
    return _unit_prefixes;
}

DataDefinition::UnitPrefixes& DataDefinition::getUnitPrefixes()
{
    return _unit_prefixes;
}

const DataDefinition::Units& DataDefinition::getUnits() const
{
    return _units;
}

DataDefinition::Units& DataDefinition::getUnits()
{
    return _units;
}

const DataDefinition::DataTypes& DataDefinition::getDataTypes() const
{
    return _data_types;
}

DataDefinition::DataTypes& DataDefinition::getDataTypes()
{
    return _data_types;
}

const DataDefinition::EnumTypes& DataDefinition::getEnumTypes() const
{
    return _enum_types;
}

DataDefinition::EnumTypes& DataDefinition::getEnumTypes()
{
    return _enum_types;
}

const DataDefinition::StructTypes& DataDefinition::getStructTypes() const
{
    return _struct_types;
}

DataDefinition::StructTypes& DataDefinition::getStructTypes()
{
    return _struct_types;
}

const DataDefinition::StreamMetaTypes& DataDefinition::getStreamMetaTypes() const
{
    return _stream_meta_types;
}

DataDefinition::StreamMetaTypes& DataDefinition::getStreamMetaTypes()
{
    return _stream_meta_types;
}

const DataDefinition::Streams& DataDefinition::getStreams() const
{
    return _streams;
}

DataDefinition::Streams& DataDefinition::getStreams()
{
    return _streams;
}

bool DataDefinition::validateContains(const BaseUnits::access_type& base_unit) const
{
    return containsUnit(base_unit.getName());
}

bool DataDefinition::validateContains(const UnitPrefixes::access_type& unit_prefix) const
{
    return _unit_prefixes.contains(unit_prefix.getName());
}

bool DataDefinition::validateContains(const Units::access_type& unit) const
{
    return containsUnit(unit.getName());
}

bool DataDefinition::validateContains(const DataTypes::access_type& data_type) const
{
    auto name_to_validate = data_type.getName();
    return _data_types.contains(name_to_validate) || _struct_types.contains(name_to_validate) ||
           _enum_types.contains(name_to_validate);
}

bool DataDefinition::validateContains(const EnumTypes::access_type& enum_type) const
{
    auto name_to_validate = enum_type.getName();
    return _data_types.contains(name_to_validate) || _struct_types.contains(name_to_validate) ||
           _enum_types.contains(name_to_validate);
}

bool DataDefinition::validateContains(const StructTypes::access_type& struct_type) const
{
    return containsType(struct_type.getName());
}

bool DataDefinition::validateContains(const StreamMetaTypes::access_type& stream_meta_type) const
{
    auto name_to_validate = stream_meta_type.getName();
    return _struct_types.contains(name_to_validate) ||
           _stream_meta_types.contains(name_to_validate);
}

bool DataDefinition::validateContains(const Streams::access_type& stream) const
{
    return _streams.contains(stream.getName());
}

bool DataDefinition::containsType(const std::string& type_name) const
{
    return _data_types.contains(type_name) || _struct_types.contains(type_name) ||
           _enum_types.contains(type_name) || _stream_meta_types.contains(type_name);
}

TypeOfType DataDefinition::getTypeOfType(const std::string& type_name) const
{
    if (getDataTypes().contains(type_name)) {
        return TypeOfType::data_type;
    }
    else if (getEnumTypes().contains(type_name)) {
        return TypeOfType::enum_type;
    }
    else if (getStructTypes().contains(type_name)) {
        return TypeOfType::struct_type;
    }
    else if (getStreamMetaTypes().contains(type_name)) {
        return TypeOfType::stream_meta_type;
    }
    return TypeOfType::invalid_type;
}

bool DataDefinition::containsUnit(const std::string& unit_name) const
{
    return _base_units.contains(unit_name) || _units.contains(unit_name);
}

TypeOfUnit DataDefinition::getTypeOfUnit(const std::string& unit_name) const
{
    if (getBaseUnits().contains(unit_name)) {
        return TypeOfUnit::base_unit;
    }
    else if (getUnits().contains(unit_name)) {
        return TypeOfUnit::unit;
    }
    return TypeOfUnit::invalid_unit;
}

namespace {
ModelEventCode getModelEventCodeFromMapCode(utility::TypeAccessMapEventCode code)
{
    using namespace utility;
    ModelEventCode code_to_forward = item_changed;
    switch (code) {
    case map_item_added:
        code_to_forward = item_added;
        break;
    case map_item_removed:
        code_to_forward = item_removed;
        break;
    case map_item_changed:
        code_to_forward = item_changed;
        break;
    case map_item_renamed:
        code_to_forward = item_renamed;
        break;
    case map_subitem_added:
        code_to_forward = subitem_added;
        break;
    case map_subitem_removed:
        code_to_forward = subitem_removed;
        break;
    case map_subitem_changed:
        code_to_forward = subitem_changed;
        break;
    case map_subitem_renamed:
        code_to_forward = subitem_renamed;
        break;
    }
    return code_to_forward;
}
} // namespace

void DataDefinition::notifyChangedMapContent(utility::TypeAccessMapEventCode code,
                                             DataTypes::access_type& data_type,
                                             const std::string& additional_info)
{
    _empty_model = false;
    ModelSubject<DataType>::notifyChanged(
        getModelEventCodeFromMapCode(code), data_type, additional_info);
}

void DataDefinition::notifyChangedMapContent(utility::TypeAccessMapEventCode code,
                                             BaseUnits::access_type& base_unit,
                                             const std::string& additional_info)
{
    _empty_model = false;
    ModelSubject<BaseUnit>::notifyChanged(
        getModelEventCodeFromMapCode(code), base_unit, additional_info);
}

void DataDefinition::notifyChangedMapContent(utility::TypeAccessMapEventCode code,
                                             UnitPrefixes::access_type& unit_prefix,
                                             const std::string& additional_info)
{
    _empty_model = false;
    ModelSubject<UnitPrefix>::notifyChanged(
        getModelEventCodeFromMapCode(code), unit_prefix, additional_info);
}

void DataDefinition::notifyChangedMapContent(utility::TypeAccessMapEventCode code,
                                             Units::access_type& unit,
                                             const std::string& additional_info)
{
    _empty_model = false;
    ModelSubject<Unit>::notifyChanged(getModelEventCodeFromMapCode(code), unit, additional_info);
}

void DataDefinition::notifyChangedMapContent(utility::TypeAccessMapEventCode code,
                                             EnumTypes::access_type& enum_type,
                                             const std::string& additional_info)
{
    _empty_model = false;
    ModelSubject<EnumType>::notifyChanged(
        getModelEventCodeFromMapCode(code), enum_type, additional_info);
}

void DataDefinition::notifyChangedMapContent(utility::TypeAccessMapEventCode code,
                                             StructTypes::access_type& struct_type,
                                             const std::string& additional_info)
{
    _empty_model = false;
    ModelSubject<StructType>::notifyChanged(
        getModelEventCodeFromMapCode(code), struct_type, additional_info);
}

void DataDefinition::notifyChangedMapContent(utility::TypeAccessMapEventCode code,
                                             StreamMetaTypes::access_type& stream_meta_type,
                                             const std::string& additional_info)
{
    _empty_model = false;
    ModelSubject<StreamMetaType>::notifyChanged(
        getModelEventCodeFromMapCode(code), stream_meta_type, additional_info);
}

void DataDefinition::notifyChangedMapContent(utility::TypeAccessMapEventCode code,
                                             Streams::access_type& stream,
                                             const std::string& additional_info)
{
    _empty_model = false;
    ModelSubject<Stream>::notifyChanged(
        getModelEventCodeFromMapCode(code), stream, additional_info);
}

void DataDefinition::modelChanged(datamodel::ModelEventCode event_code,
                                  datamodel::Header& changed_subject,
                                  const std::string& additional_info)
{
    _empty_model = false;
    ModelSubject<Header>::notifyChanged(event_code, changed_subject, additional_info);
}

bool DataDefinition::isEmpty() const
{
    return _empty_model;
}

} // namespace datamodel
} // namespace dd
} // namespace ddl
