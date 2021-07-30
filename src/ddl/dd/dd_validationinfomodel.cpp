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

#include "ddl/dd/dd_validationinfomodel.h"

#include "ddl/dd/dd_predefined_datatypes.h"
#include "ddl/dd/dd_predefined_units.h"
#include "ddl/utilities/std_to_string.h"

namespace ddl {

namespace dd {

constexpr const uint8_t ValidationServiceInfo::INFO_TYPE_ID;
constexpr const uint8_t ValidationInfo::INFO_TYPE_ID;

/**
 * DependencyServiceInfo is only part of the toplevel DataDefinition.
 * The validation model uses these
 */
ValidationServiceInfo::Dependency::Dependency(DependencyType type_of_dependency,
                                              const std::string& from,
                                              const std::string& to)
    : _type_of_dependency(type_of_dependency), _from(from), _to(to)
{
}

namespace {
void addDependencyToMap(const std::string& from,
                        const std::string& to,
                        ValidationServiceInfo::ToFromMap& the_map)
{
    // the map will save wich one is by which items
    // the "to" -> many "used from"
    // but only if it does not exist already
    for (auto& current_from: the_map[to]) {
        if (current_from == from) {
            // is already in there
            return;
        }
    }
    the_map[to].push_back(from);
}

void removeInMapFrom(const std::string& from, ValidationServiceInfo::ToFromMap& the_map)
{
    for (auto current_to = the_map.begin(); current_to != the_map.end(); ++current_to) {
        for (auto it = current_to->second.begin(); it != current_to->second.end(); ++it) {
            if (*it == from) {
                current_to->second.erase(it);
                break;
            }
        }
    }
}

std::vector<std::string> removeInMapTo(const std::string& to,
                                       ValidationServiceInfo::ToFromMap& the_map)
{
    std::vector<std::string> result = {};
    auto it = the_map.find(to);
    if (it != the_map.end()) {
        std::swap(it->second, result);
        the_map.erase(it);
    }
    return std::move(result);
}

void renameInMapFrom(const std::string& from_old,
                     const std::string& from_new,
                     ValidationServiceInfo::ToFromMap& the_map)
{
    for (auto& current_to: the_map) {
        for (auto it = current_to.second.begin(); it != current_to.second.end(); ++it) {
            if (*it == from_old) {
                *it = from_new;
                break;
            }
        }
    }
}

void addDependency(const ValidationServiceInfo::Dependency& dependency,
                   datamodel::DataDefinition& parent_ddl)
{
    auto dependency_service = parent_ddl.getInfo<ValidationServiceInfo>();
    if (dependency_service) {
        dependency_service->addDependency(dependency);
    }
}

} // namespace

/**
 * DependencyServiceInfo is only part of the toplevel DataDefinition.
 * The validation model uses these
 */
void ValidationServiceInfo::addDependency(const Dependency& dependency)
{
    auto& ToFromMap = _dependencies[dependency._type_of_dependency];
    addDependencyToMap(dependency._from, dependency._to, ToFromMap);
}

namespace {

DEF_GETINFO_NON_CONST(ValidationInfo, datamodel::Unit);
DEF_GETINFO_NON_CONST(ValidationInfo, datamodel::DataType);
DEF_GETINFO_NON_CONST(ValidationInfo, datamodel::EnumType);
DEF_GETINFO_NON_CONST(ValidationInfo, datamodel::StructType);
DEF_GETINFO_NON_CONST(ValidationInfo, datamodel::StreamMetaType);
DEF_GETINFO_NON_CONST(ValidationInfo, datamodel::Stream);

template <typename T>
void updateValidationInfo(const std::shared_ptr<T>& type_ref, datamodel::DataDefinition& parent_ddl)
{
    if (type_ref) {
        auto info = getInfoFrom(*type_ref);
        if (info) {
            info->update(*type_ref, parent_ddl);
        }
    }
}
} // namespace

void ValidationServiceInfo::removed(const datamodel::DataType& data_type,
                                    datamodel::DataDefinition& parent_ddl)
{
    removeInMapFrom(data_type.getName(), _dependencies[data_type_to_unit]);
    auto dependencies = removeInMapTo(data_type.getName(), _dependencies[enum_type_to_data_type]);
    // this should invalidate the enum_type if there are values
    for (const auto& current_enum_name: dependencies) {
        const auto enum_type = parent_ddl.getEnumTypes().access(current_enum_name);
        updateValidationInfo<datamodel::EnumType>(enum_type, parent_ddl);
    }
    dependencies = removeInMapTo(data_type.getName(), _dependencies[struct_type_to_data_type]);
    // this should invalidate the struct_types
    for (const auto& current_struct_name: dependencies) {
        const auto struct_type = parent_ddl.getStructTypes().access(current_struct_name);
        updateValidationInfo<datamodel::StructType>(struct_type, parent_ddl);
    }
}

void ValidationServiceInfo::removed(const datamodel::EnumType& enum_type,
                                    datamodel::DataDefinition& parent_ddl)
{
    removeInMapFrom(enum_type.getName(), _dependencies[enum_type_to_data_type]);
    auto dependencies = removeInMapTo(enum_type.getName(), _dependencies[struct_type_to_enum_type]);
    // this should invalidate the struct_types using it
    for (const auto& current_struct_name: dependencies) {
        const auto struct_type = parent_ddl.getStructTypes().access(current_struct_name);
        updateValidationInfo<datamodel::StructType>(struct_type, parent_ddl);
    }
}

void ValidationServiceInfo::removed(const datamodel::StructType& struct_type,
                                    datamodel::DataDefinition& parent_ddl)
{
    removeInMapFrom(struct_type.getName(), _dependencies[struct_type_to_data_type]);
    removeInMapFrom(struct_type.getName(), _dependencies[struct_type_to_enum_type]);
    removeInMapFrom(struct_type.getName(), _dependencies[struct_type_to_struct_type]);
    auto dependencies =
        removeInMapTo(struct_type.getName(), _dependencies[struct_type_to_struct_type]);
    // this should invalidate the struct_types using it
    for (const auto& current_struct_name: dependencies) {
        const auto struct_type_to_access = parent_ddl.getStructTypes().access(current_struct_name);
        updateValidationInfo<datamodel::StructType>(struct_type_to_access, parent_ddl);
    }
    dependencies = removeInMapTo(struct_type.getName(), _dependencies[stream_to_struct_type]);
    // this should invalidate the streams
    for (const auto& current_stream: dependencies) {
        const auto stream = parent_ddl.getStreams().access(current_stream);
        updateValidationInfo<datamodel::Stream>(stream, parent_ddl);
    }
}

void ValidationServiceInfo::removed(const datamodel::StreamMetaType& stream_meta_type,
                                    datamodel::DataDefinition& parent_dd)
{
    removeInMapFrom(stream_meta_type.getName(),
                    _dependencies[stream_meta_type_to_stream_meta_type]);

    auto dependencies = removeInMapTo(stream_meta_type.getName(),
                                      _dependencies[stream_meta_type_to_stream_meta_type]);
    // this should invalidate the struct_types using it
    for (const auto& current_stream_meta_name: dependencies) {
        const auto current_meta_type =
            parent_dd.getStreamMetaTypes().access(current_stream_meta_name);
        updateValidationInfo<datamodel::StreamMetaType>(current_meta_type, parent_dd);
    }
    dependencies =
        removeInMapTo(stream_meta_type.getName(), _dependencies[stream_to_stream_meta_type]);
    // this should invalidate the struct_types using it
    for (const auto& current_stream: dependencies) {
        const auto stream = parent_dd.getStreams().access(current_stream);
        updateValidationInfo<datamodel::Stream>(stream, parent_dd);
    }
}

void ValidationServiceInfo::removed(const datamodel::Stream& stream, datamodel::DataDefinition&)
{
    removeInMapFrom(stream.getName(), _dependencies[stream_to_stream_meta_type]);
    removeInMapFrom(stream.getName(), _dependencies[stream_to_struct_type]);
}

void ValidationServiceInfo::removed(const datamodel::BaseUnit& base_unit,
                                    datamodel::DataDefinition& parent_dd)
{
    // update all units
    auto dependencies = removeInMapTo(base_unit.getName(), _dependencies[unit_to_base_unit]);
    for (const auto& current_unit_name: dependencies) {
        const auto unit = parent_dd.getUnits().access(current_unit_name);
        updateValidationInfo<datamodel::Unit>(unit, parent_dd);
    }
    // update all data types
    dependencies = removeInMapTo(base_unit.getName(), _dependencies[data_type_to_base_unit]);
    for (const auto& current_data_type_name: dependencies) {
        const auto data_type = parent_dd.getDataTypes().access(current_data_type_name);
        updateValidationInfo<datamodel::DataType>(data_type, parent_dd);
    }
    // update all struct types
    dependencies = removeInMapTo(base_unit.getName(), _dependencies[struct_type_to_base_unit]);
    for (const auto& current_struct_type_name: dependencies) {
        const auto struct_type = parent_dd.getStructTypes().access(current_struct_type_name);
        updateValidationInfo<datamodel::StructType>(struct_type, parent_dd);
    }
}

void ValidationServiceInfo::removed(const datamodel::UnitPrefix& unit_prefix,
                                    datamodel::DataDefinition& parent_dd)
{
    // update all units
    auto dependencies = removeInMapTo(unit_prefix.getName(), _dependencies[unit_to_unit_prefix]);
    for (const auto& current_unit_name: dependencies) {
        auto unit = parent_dd.getUnits().access(current_unit_name);
        updateValidationInfo<datamodel::Unit>(unit, parent_dd);
    }
}

void ValidationServiceInfo::removed(const datamodel::Unit& unit,
                                    datamodel::DataDefinition& parent_dd)
{
    removeInMapFrom(unit.getName(), _dependencies[unit_to_base_unit]);
    removeInMapFrom(unit.getName(), _dependencies[unit_to_unit_prefix]);

    // update all data types
    auto dependencies = removeInMapTo(unit.getName(), _dependencies[data_type_to_unit]);
    for (const auto& current_data_type_name: dependencies) {
        auto data_type = parent_dd.getDataTypes().access(current_data_type_name);
        updateValidationInfo<datamodel::DataType>(data_type, parent_dd);
    }
    // update all struct types
    dependencies = removeInMapTo(unit.getName(), _dependencies[struct_type_to_unit]);
    for (const auto& current_struct_type_name: dependencies) {
        auto struct_type = parent_dd.getStructTypes().access(current_struct_type_name);
        updateValidationInfo<datamodel::StructType>(struct_type, parent_dd);
    }
}

void ValidationServiceInfo::renamed(const datamodel::DataType& data_type,
                                    const std::string& old_name,
                                    datamodel::DataDefinition& parent_dd)
{
    renameInMapFrom(old_name, data_type.getName(), _dependencies[data_type_to_unit]);

    // this should invalidate the enum_type if there are values
    auto dependencies = removeInMapTo(old_name, _dependencies[enum_type_to_data_type]);
    for (const auto& current_enum_name: dependencies) {
        auto enum_type = parent_dd.getEnumTypes().access(current_enum_name);
        if (enum_type) {
            if (enum_type->getDataTypeName() == old_name) {
                // this should update the validation and re add the
                enum_type->setDataTypeName(data_type.getName());
            }
        }
    }

    // this should invalidate the struct_type if there are values
    // go through the structs using this
    dependencies = removeInMapTo(old_name, _dependencies[struct_type_to_data_type]);
    // this should invalidate the struct_types
    for (const auto& current_struct_name: dependencies) {
        auto struct_type = parent_dd.getStructTypes().access(current_struct_name);
        // this is just for optimization, we need to to update ValidationInfo only once
        _validation_needed = false;
        if (struct_type) {
            for (auto& elem: struct_type->getElements()) {
                if (elem->getTypeName() == old_name) {
                    elem->setTypeName(data_type.getName());
                }
            }
            // the setType should not invalidate, but this is also needed to readd dependencies
            updateValidationInfo<datamodel::StructType>(struct_type, parent_dd);
        }
        _validation_needed = true;
    }
}

void ValidationServiceInfo::renamed(const datamodel::EnumType& enum_type,
                                    const std::string& old_name,
                                    datamodel::DataDefinition& parent_dd)
{
    renameInMapFrom(old_name, enum_type.getName(), _dependencies[enum_type_to_data_type]);

    auto dependencies = removeInMapTo(old_name, _dependencies[struct_type_to_enum_type]);
    // this should invalidate the struct_types using it
    for (const auto& current_struct_name: dependencies) {
        auto struct_type = parent_dd.getStructTypes().access(current_struct_name);
        // this is just for optimization, we need to to update ValidationInfo only once
        _validation_needed = false;
        if (struct_type) {
            for (auto& elem: struct_type->getElements()) {
                if (elem->getTypeName() == old_name) {
                    elem->setTypeName(enum_type.getName());
                }
            }
            // the setType should not invalidate, but this is also needed to readd dependencies
            updateValidationInfo<datamodel::StructType>(struct_type, parent_dd);
        }
        _validation_needed = true;
    }
}

void ValidationServiceInfo::renamed(const datamodel::StructType& struct_type,
                                    const std::string& old_name,
                                    datamodel::DataDefinition& parent_dd)
{
    renameInMapFrom(old_name, struct_type.getName(), _dependencies[struct_type_to_data_type]);
    renameInMapFrom(old_name, struct_type.getName(), _dependencies[struct_type_to_enum_type]);
    renameInMapFrom(old_name, struct_type.getName(), _dependencies[struct_type_to_struct_type]);

    auto dependencies = removeInMapTo(old_name, _dependencies[struct_type_to_struct_type]);
    // this should invalidate the struct_types using it
    for (const auto& current_struct_name: dependencies) {
        auto struct_type_current = parent_dd.getStructTypes().access(current_struct_name);
        // this is just for optimization, we need to to update ValidationInfo only once
        _validation_needed = false;
        if (struct_type_current) {
            for (auto& elem: struct_type_current->getElements()) {
                if (elem->getTypeName() == old_name) {
                    elem->setTypeName(struct_type.getName());
                }
            }
            // the setType should not invalidate, but this is also needed to readd dependencies
            updateValidationInfo<datamodel::StructType>(struct_type_current, parent_dd);
        }
        _validation_needed = true;
    }

    dependencies = removeInMapTo(old_name, _dependencies[stream_to_struct_type]);
    // this should invalidate the streams
    for (const auto& current_stream_name: dependencies) {
        auto current_stream = parent_dd.getStreams().access(current_stream_name);
        // this is just for optimization, we need to to update ValidationInfo only once
        _validation_needed = false;
        if (current_stream) {
            for (auto& stream_struct: current_stream->getStructs()) {
                if (stream_struct->getTypeName() == old_name) {
                    stream_struct->setTypeName(struct_type.getName());
                }
            }
            if (current_stream->getStreamTypeName() == old_name) {
                current_stream->setStreamTypeName(struct_type.getName());
            }
            // the setType should not invalidate, but this is also needed to readd dependencies
            updateValidationInfo<datamodel::Stream>(current_stream, parent_dd);
        }
        _validation_needed = true;
    }
}

void ValidationServiceInfo::renamed(const datamodel::StreamMetaType& stream_meta_type,
                                    const std::string& old_name,
                                    datamodel::DataDefinition& parent_dd)
{
    renameInMapFrom(
        old_name, stream_meta_type.getName(), _dependencies[stream_meta_type_to_stream_meta_type]);

    auto dependencies =
        removeInMapTo(old_name, _dependencies[stream_meta_type_to_stream_meta_type]);
    // this should invalidate the struct_types using it
    for (const auto& current_stream_meta_type_name: dependencies) {
        auto current_stream_meta_type =
            parent_dd.getStreamMetaTypes().access(current_stream_meta_type_name);
        // this is just for optimization, we need to to update ValidationInfo only once
        _validation_needed = false;
        if (current_stream_meta_type) {
            if (current_stream_meta_type->getParent() == old_name) {
                current_stream_meta_type->setParent(stream_meta_type.getName());
            }
            // the setType should not invalidate, but this is also needed to readd dependencies
            updateValidationInfo<datamodel::StreamMetaType>(current_stream_meta_type, parent_dd);
        }
        _validation_needed = true;
    }

    dependencies = removeInMapTo(old_name, _dependencies[stream_to_stream_meta_type]);
    // this should invalidate the streams using it
    for (const auto& current_stream_name: dependencies) {
        auto current_stream = parent_dd.getStreams().access(current_stream_name);
        // this is just for optimization, we need to to update ValidationInfo only once
        _validation_needed = false;
        if (current_stream) {
            if (current_stream->getStreamTypeName() == old_name) {
                current_stream->setStreamTypeName(stream_meta_type.getName());
            }
            // the setType should not invalidate, but this is also needed to readd dependencies
            updateValidationInfo<datamodel::Stream>(current_stream, parent_dd);
        }
        _validation_needed = true;
    }
}

void ValidationServiceInfo::renamed(const datamodel::Stream& stream,
                                    const std::string& old_name,
                                    datamodel::DataDefinition&)
{
    renameInMapFrom(old_name, stream.getName(), _dependencies[stream_to_stream_meta_type]);
    renameInMapFrom(old_name, stream.getName(), _dependencies[stream_to_struct_type]);
}

void ValidationServiceInfo::renamed(const datamodel::BaseUnit& base_unit,
                                    const std::string& old_name,
                                    datamodel::DataDefinition& parent_dd)
{
    // rename in structs
    auto dependencies = removeInMapTo(old_name, _dependencies[struct_type_to_base_unit]);
    // this should invalidate the struct_types using it
    for (const auto& current_struct_name: dependencies) {
        auto current_struct_type = parent_dd.getStructTypes().access(current_struct_name);
        // this is just for optimization, we need to to update ValidationInfo only once
        _validation_needed = false;
        if (current_struct_type) {
            for (auto& elem: current_struct_type->getElements()) {
                if (elem->getUnitName() == old_name) {
                    elem->setUnitName(base_unit.getName());
                }
            }
            // the setType should not invalidate, but this is also needed to readd dependencies
            updateValidationInfo<datamodel::StructType>(current_struct_type, parent_dd);
        }
        _validation_needed = true;
    }

    // rename in data_types
    dependencies = removeInMapTo(old_name, _dependencies[data_type_to_base_unit]);
    // this should invalidate the struct_types using it
    for (const auto& current_data_type_name: dependencies) {
        auto current_data_type = parent_dd.getDataTypes().access(current_data_type_name);
        // this is just for optimization, we need to to update ValidationInfo only once
        _validation_needed = false;
        if (current_data_type) {
            if (current_data_type->getUnitName() == old_name) {
                current_data_type->setUnitName(base_unit.getName());
            }
            // the setType should not invalidate, but this is also needed to readd dependencies
            updateValidationInfo<datamodel::DataType>(current_data_type, parent_dd);
        }
        _validation_needed = true;
    }

    // rename in refUnits
    dependencies = removeInMapTo(old_name, _dependencies[unit_to_base_unit]);
    // this should invalidate the struct_types using it
    for (const auto& current_unit_name: dependencies) {
        auto current_unit = parent_dd.getUnits().access(current_unit_name);
        // this is just for optimization, we need to to update ValidationInfo only once
        _validation_needed = false;
        if (current_unit) {
            std::vector<datamodel::Unit::RefUnit> found_units;
            for (auto ref_unit: current_unit->getRefUnits()) {
                if (ref_unit.getUnitName() == old_name) {
                    found_units.push_back(ref_unit);
                }
            }
            for (auto& ref_unit_found: found_units) {
                // remove it
                // rename it
                // readd it
                current_unit->removeRefUnit(ref_unit_found);
                current_unit->addRefUnit({base_unit.getName(),
                                          ref_unit_found.getPower(),
                                          ref_unit_found.getPrefixName()});
            }
            // the setType should not invalidate, but this is also needed to readd dependencies
            updateValidationInfo<datamodel::Unit>(current_unit, parent_dd);
        }
        _validation_needed = true;
    }
}

void ValidationServiceInfo::renamed(const datamodel::UnitPrefix& unit_prefix,
                                    const std::string& old_name,
                                    datamodel::DataDefinition& parent_dd)
{
    // rename in refUnits
    auto dependencies = removeInMapTo(old_name, _dependencies[unit_to_unit_prefix]);
    // this should invalidate the struct_types using it
    for (const auto& current_unit_name: dependencies) {
        auto current_unit = parent_dd.getUnits().access(current_unit_name);
        // this is just for optimization, we need to to update ValidationInfo only once
        _validation_needed = false;
        if (current_unit) {
            std::vector<datamodel::Unit::RefUnit> found_units;
            for (auto ref_unit: current_unit->getRefUnits()) {
                if (ref_unit.getPrefixName() == old_name) {
                    found_units.push_back(ref_unit);
                }
            }
            for (auto& ref_unit_found: found_units) {
                // remove it
                // rename it
                // readd it
                current_unit->removeRefUnit(ref_unit_found);
                current_unit->addRefUnit({ref_unit_found.getUnitName(),
                                          ref_unit_found.getPower(),
                                          unit_prefix.getName()});
            }
            // the setType should not invalidate, but this is also needed to readd dependencies
            updateValidationInfo<datamodel::Unit>(current_unit, parent_dd);
        }
        _validation_needed = true;
    }
}

void ValidationServiceInfo::renamed(const datamodel::Unit& unit,
                                    const std::string& old_name,
                                    datamodel::DataDefinition& parent_ddl)
{
    renameInMapFrom(old_name, unit.getName(), _dependencies[unit_to_base_unit]);
    renameInMapFrom(old_name, unit.getName(), _dependencies[unit_to_unit_prefix]);

    // rename in structs
    auto dependencies = removeInMapTo(old_name, _dependencies[struct_type_to_unit]);
    // this should invalidate the struct_types using it
    for (const auto& current_struct_name: dependencies) {
        auto current_struct_type = parent_ddl.getStructTypes().access(current_struct_name);
        // this is just for optimization, we need to to update ValidationInfo only once
        _validation_needed = false;
        if (current_struct_type) {
            for (auto& elem: current_struct_type->getElements()) {
                if (elem->getUnitName() == old_name) {
                    elem->setUnitName(unit.getName());
                }
            }
            // the setType should not invalidate, but this is also needed to readd dependencies
            updateValidationInfo<datamodel::StructType>(current_struct_type, parent_ddl);
        }
        _validation_needed = true;
    }

    // rename in data_types
    dependencies = removeInMapTo(old_name, _dependencies[data_type_to_unit]);
    // this should invalidate the struct_types using it
    for (const auto& current_data_type_name: dependencies) {
        auto current_data_type = parent_ddl.getDataTypes().access(current_data_type_name);
        // this is just for optimization, we need to to update ValidationInfo only once
        _validation_needed = false;
        if (current_data_type) {
            if (current_data_type->getUnitName() == old_name) {
                current_data_type->setUnitName(unit.getName());
            }
            // the setType should not invalidate, but this is also needed to readd dependencies
            updateValidationInfo<datamodel::DataType>(current_data_type, parent_ddl);
        }
        _validation_needed = true;
    }
}

bool ValidationServiceInfo::validationNeeded() const
{
    return _validation_needed;
}

/**
 * Validation Info model will check for validation.
 * Sometimes the TypeInfo must be already set before this validation can be performed.
 */
ValidationInfo::ValidationInfo(datamodel::DataType& data_type, datamodel::DataDefinition& parent_dd)
{
    update(data_type, parent_dd);
}

ValidationInfo::ValidationInfo(datamodel::EnumType& enum_type, datamodel::DataDefinition& parent_dd)
{
    update(enum_type, parent_dd);
}

ValidationInfo::ValidationInfo(datamodel::Stream& stream, datamodel::DataDefinition& parent_dd)
{
    update(stream, parent_dd);
}

ValidationInfo::ValidationInfo(datamodel::Unit& unit, datamodel::DataDefinition& parent_dd)
{
    update(unit, parent_dd);
}

ValidationInfo::~ValidationInfo()
{
    // removeAllDependencies();
}

bool ValidationInfo::isValid(ValidationLevel level) const
{
    if (_valid == invalid) {
        return false;
    }
    else if (_valid >= level) {
        return true;
    }
    else {
        return false;
    }
}

ValidationInfo::ValidationLevel ValidationInfo::getValidationLevel() const
{
    return _valid;
}

const std::vector<ValidationInfo::Problem>& ValidationInfo::getProblems() const
{
    return _validation_problems;
}

void ValidationInfo::addProblem(Problem&& problem)
{
    _validation_problems.emplace_back(problem);
}

void ValidationInfo::addProblem(const Problem& problem)
{
    _validation_problems.emplace_back(problem);
}

namespace {
ValidationInfo::Problem generateProblemMessage(const std::string& typeofmodel,
                                               const std::string& name,
                                               const std::string& problem_message)
{
    return {name, typeofmodel + " ('" + name + "'): " + problem_message};
}

std::shared_ptr<datamodel::BaseUnit> getOrCreatePredefinedBaseUnit(
    const std::string& name, datamodel::DataDefinition& parent_dd)
{
    auto base_unit_found = parent_dd.getBaseUnits().access(name);
    if (!base_unit_found) {
        // if not found mayby it is predefined
        auto predefined = ddl::PredefinedUnits::getInstance().getPredefinedBaseUnit(name);
        if (predefined) {
            parent_dd.getBaseUnits().add(*predefined);
            base_unit_found = parent_dd.getBaseUnits().access(name);
        }
    }
    return base_unit_found;
}
std::shared_ptr<datamodel::UnitPrefix> getOrCreatePredefinedUnitPrefix(
    const std::string& name, datamodel::DataDefinition& parent_dd)
{
    auto unit_prefix_found = parent_dd.getUnitPrefixes().access(name);
    if (!unit_prefix_found) {
        // if not found mayby it is predefined
        auto predefined = ddl::PredefinedUnits::getInstance().getPredefinedUnitPrefix(name);
        if (predefined) {
            parent_dd.getUnitPrefixes().add(*predefined);
            unit_prefix_found = parent_dd.getUnitPrefixes().access(name);
        }
    }
    return unit_prefix_found;
}
} // namespace

void ValidationInfo::update(datamodel::Unit& unit, datamodel::DataDefinition& parent_dd)
{
    _validation_problems.clear();
    // the unit may be dependent to base unit or prefix within its ref units
    ValidationLevel current_validation_level = valid;
    for (auto& ref_unit: unit.getRefUnits()) {
        auto base_unit_name = ref_unit.getUnitName();
        if (!base_unit_name.empty()) {
            addDependency(
                {ValidationServiceInfo::unit_to_base_unit, unit.getName(), base_unit_name},
                parent_dd);
            auto base_unit = getOrCreatePredefinedBaseUnit(base_unit_name, parent_dd);
            // this will invalidate the whole dd ! If a unit is defined it must be valid!
            if (!base_unit) {
                current_validation_level = invalid;
                addProblem(generateProblemMessage("Unit::RefUnit",
                                                  unit.getName(),
                                                  "BaseUnit '" + base_unit_name +
                                                      "' is not defined in DataDefinition"));
            }
        }
        else {
            current_validation_level = invalid;
            addProblem(
                generateProblemMessage("Unit::RefUnit", unit.getName(), "BaseUnit is empty"));
        }
        auto prefix_name = ref_unit.getPrefixName();
        if (!prefix_name.empty()) {
            addDependency({ValidationServiceInfo::unit_to_unit_prefix, unit.getName(), prefix_name},
                          parent_dd);
            auto prefix = getOrCreatePredefinedUnitPrefix(prefix_name, parent_dd);
            if (!prefix) {
                // this will invalidate the whole dd ! If a unit is defined it must be valid!
                current_validation_level = invalid;
                addProblem(generateProblemMessage("Unit::RefUnit",
                                                  unit.getName(),
                                                  "UnitPrefix '" + prefix_name +
                                                      "' is not defined in DataDefinition"));
            }
        }
        else {
            current_validation_level = invalid;
            addProblem(
                generateProblemMessage("Unit::RefUnit", unit.getName(), "UnitPrefix is empty"));
        }
    }
    _valid = current_validation_level;
}

void ValidationInfo::update(datamodel::DataType& data_type, datamodel::DataDefinition& parent_dd)
{
    _validation_problems.clear();
    auto unit_name = data_type.getUnitName();
    ValidationLevel current_validation_level = valid;
    if (!unit_name.empty()) {
        // we do not care about the return value here
        getOrCreatePredefinedBaseUnit(unit_name, parent_dd);
        auto type_of_unit = parent_dd.getTypeOfUnit(unit_name);
        if (type_of_unit == base_unit) {
            addDependency(
                {ValidationServiceInfo::data_type_to_base_unit, data_type.getName(), unit_name},
                parent_dd);
        }
        else if (type_of_unit == unit) {
            auto found_unit = parent_dd.getUnits().access(unit_name);
            auto validation_info = found_unit->getInfo<ValidationInfo>();
            if (validation_info == nullptr) {
                found_unit->setInfo<ValidationInfo>(
                    std::make_shared<ValidationInfo>(*found_unit, parent_dd));
                validation_info = found_unit->getInfo<ValidationInfo>();
            }
            if (validation_info->getValidationLevel() < valid) {
                current_validation_level = good_enough;
                addProblem(
                    generateProblemMessage("DataType",
                                           data_type.getName(),
                                           "The used unit '" + unit_name + "' has a Problem"));
            }
            addDependency(
                {ValidationServiceInfo::data_type_to_unit, data_type.getName(), unit_name},
                parent_dd);
        }
        else {
            addDependency(
                {ValidationServiceInfo::data_type_to_unit, data_type.getName(), unit_name},
                parent_dd);
            addDependency(
                {ValidationServiceInfo::data_type_to_base_unit, data_type.getName(), unit_name},
                parent_dd);
            current_validation_level = good_enough; // good enough for type info
            addProblem(generateProblemMessage("DataType",
                                              data_type.getName(),
                                              "The used unit '" + unit_name + "' is not defined"));
        }
    }
    // other attributes
    if (data_type.getBitSize() == 0) {
        current_validation_level = invalid; // good enough for type info
        addProblem(generateProblemMessage("DataType",
                                          data_type.getName(),
                                          "The bit_size of " + data_type.getName() +
                                              " is invalid (0 is not valid)"));
    }
    if (data_type.getArraySize() && *data_type.getArraySize() == 0) {
        current_validation_level = invalid; // good enough for type info
        addProblem(generateProblemMessage("DataType",
                                          data_type.getName(),
                                          "The array_size of '" + data_type.getName() +
                                              "' is invalid (0 is not valid)"));
    }

    _valid = current_validation_level;
}

namespace {
std::shared_ptr<datamodel::DataType> getOrCreatePredefinedDataType(
    const std::string& name, datamodel::DataDefinition& parent_dd)
{
    auto data_type_found = parent_dd.getDataTypes().access(name);
    if (!data_type_found) {
        // if not found mayby it is predefined
        auto predefined = ddl::PredefinedDataTypes::getInstance().getPredefinedType(name);
        if (predefined) {
            parent_dd.getDataTypes().add(*predefined);
            data_type_found = parent_dd.getDataTypes().access(name);
        }
    }
    return data_type_found;
}
} // namespace

void ValidationInfo::update(datamodel::EnumType& enum_type, datamodel::DataDefinition& parent_ddl)
{
    _validation_problems.clear();
    ValidationLevel current_validation = valid;
    auto data_type_name = enum_type.getDataTypeName();
    if (data_type_name.empty()) {
        current_validation = invalid;
        addProblem(generateProblemMessage(
            "EnumType", enum_type.getName(), "The used data_type is not defined"));
    }
    else {
        addDependency(
            {ValidationServiceInfo::enum_type_to_data_type, enum_type.getName(), data_type_name},
            parent_ddl);
        auto data_type = getOrCreatePredefinedDataType(data_type_name, parent_ddl);
        if (!data_type) {
            current_validation = invalid;
            addProblem(generateProblemMessage("EnumType",
                                              enum_type.getName(),
                                              "The used data_type '" + data_type_name +
                                                  "' is not defined"));
        }
        else {
            auto info = data_type->getInfo<ValidationInfo>();
            if (info) {
                current_validation = data_type->getInfo<ValidationInfo>()->getValidationLevel();
                if (current_validation != valid) {
                    addProblem(generateProblemMessage("EnumType",
                                                      enum_type.getName(),
                                                      "The used data_type '" + data_type_name +
                                                          "' has a problem"));
                }
            }
            else {
                current_validation = good_enough;
            }
        }
    }
    _valid = current_validation;
}

void ValidationInfo::forceRevalidation()
{
    _valid = invalid;
}

namespace {

const ValidationInfo* getOrCreateValidationInfo(const std::string type_name,
                                                TypeOfType& type_of_type,
                                                datamodel::DataDefinition& ddl,
                                                bool with_stream_meta_type)
{
    if (type_name.empty()) {
        // invalid! caller will mark it as invalid!
        return nullptr;
    }

    type_of_type = ddl.getTypeOfType(type_name);
    // we check for invalid here to have a look at the predefined types
    if (!with_stream_meta_type) {
        if (type_of_type == TypeOfType::data_type || type_of_type == TypeOfType::invalid_type) {
            auto dt = getOrCreatePredefinedDataType(type_name, ddl);
            if (dt) {
                // if we maybe found a predefined type we mus set this to data_type
                type_of_type = TypeOfType::data_type;
            }
            else {
                // it is really invalid, so we return nullptr
                return {};
            }
            auto info = dt->getInfo<ValidationInfo>();
            if (info == nullptr) {
                dt->setInfo<ValidationInfo>(std::make_shared<ValidationInfo>(*dt, ddl));
                info = dt->getInfo<ValidationInfo>();
            }
            else {
                if (!info->isValid()) {
                    info->update(*dt, ddl);
                }
            }
            return info;
        }
        else if (type_of_type == TypeOfType::enum_type) {
            auto et = ddl.getEnumTypes().access(type_name);
            auto info = et->getInfo<ValidationInfo>();
            if (info == nullptr) {
                et->setInfo<ValidationInfo>(std::make_shared<ValidationInfo>(*et, ddl));
                info = et->getInfo<ValidationInfo>();
            }
            else {
                if (!info->isValid()) {
                    info->update(*et, ddl);
                }
            }
            return info;
        }
    }
    if (type_of_type == TypeOfType::struct_type) {
        auto st = ddl.getStructTypes().access(type_name);
        auto info = st->getInfo<ValidationInfo>();
        if (info == nullptr) {
            st->setInfo<ValidationInfo>(std::make_shared<ValidationInfo>());
            info = st->getInfo<ValidationInfo>();
            info->update(*st, ddl);
        }
        else {
            if (!info->isValid()) {
                info->update(*st, ddl);
            }
        }
        return info;
    }
    else if (with_stream_meta_type && type_of_type == TypeOfType::stream_meta_type) {
        auto smt = ddl.getStreamMetaTypes().access(type_name);
        auto info = smt->getInfo<ValidationInfo>();
        if (info == nullptr) {
            smt->setInfo<ValidationInfo>(std::make_shared<ValidationInfo>());
            info = smt->getInfo<ValidationInfo>();
            info->update(*smt, ddl);
        }
        else {
            if (!info->isValid()) {
                info->update(*smt, ddl);
            }
        }
        return info;
    }
    else {
        // invalid! caller will mark it as invalid!
        // type does not exist
        return {};
    }
}

ValidationInfo::ValidationLevel updateValidationForElement(
    datamodel::StructType& parent_struct_type,
    datamodel::StructType::Element& element,
    std::vector<ValidationInfo::Problem>& problems,
    datamodel::DataDefinition& parent_dd)
{
    ValidationInfo::ValidationLevel current_validation = ValidationInfo::valid;
    TypeOfType type_returned = {};
    std::string type_name = element.getTypeName();
    // this would detect a recursion
    auto validation_info = getOrCreateValidationInfo(type_name, type_returned, parent_dd, false);
    if (validation_info == nullptr) {
        current_validation = ValidationInfo::invalid;
        if (type_name.empty()) {
            problems.push_back(
                generateProblemMessage("StructType::Element",
                                       parent_struct_type.getName() + "." + element.getName(),
                                       "The used data_type is not set"));
        }
        else {
            problems.push_back(generateProblemMessage(
                "StructType::Element",
                parent_struct_type.getName() + "." + element.getName(),
                "The used data_type '" + type_name + "' is not defined in DataDefinition"));
        }
    }
    else {
        ValidationServiceInfo::DependencyType dep_type = {};
        if (type_returned == data_type) {
            dep_type = ValidationServiceInfo::struct_type_to_data_type;
        }
        else if (type_returned == enum_type) {
            dep_type = ValidationServiceInfo::struct_type_to_enum_type;
        }
        else if (type_returned == struct_type) {
            dep_type = ValidationServiceInfo::struct_type_to_struct_type;
        }
        addDependency({dep_type, parent_struct_type.getName(), type_name}, parent_dd);
        current_validation = validation_info->getValidationLevel();
        if (current_validation != ValidationInfo::valid) {
            problems.push_back(
                generateProblemMessage("StructType::Element",
                                       parent_struct_type.getName() + "." + element.getName(),
                                       "The used data type '" + type_name + "' has a problem"));
        }
    }
    // now we check the unit
    auto unit_name = element.getUnitName();
    if (!unit_name.empty()) {
        // we do not check the return value, this is just to add if it is a base unit
        getOrCreatePredefinedBaseUnit(unit_name, parent_dd);
        auto type_of_unit = parent_dd.getTypeOfUnit(unit_name);
        if (type_of_unit == base_unit) {
            addDependency({ValidationServiceInfo::struct_type_to_base_unit,
                           parent_struct_type.getName(),
                           unit_name},
                          parent_dd);
        }
        else if (type_of_unit == unit) {
            auto found_unit = parent_dd.getUnits().access(unit_name);
            auto unit_validation_info = found_unit->getInfo<ValidationInfo>();
            if (unit_validation_info == nullptr) {
                found_unit->setInfo<ValidationInfo>(
                    std::make_shared<ValidationInfo>(*found_unit, parent_dd));
                unit_validation_info = found_unit->getInfo<ValidationInfo>();
            }
            auto found_level = unit_validation_info->getValidationLevel();
            if (found_level < ValidationInfo::ValidationLevel::valid) {
                if (current_validation == ValidationInfo::ValidationLevel::valid) {
                    current_validation = ValidationInfo::ValidationLevel::good_enough;
                }
                problems.push_back(
                    (generateProblemMessage("StructType::Element",
                                            parent_struct_type.getName() + "." + element.getName(),
                                            "The used unit '" + unit_name + "' has a Problem")));
            }
            addDependency({ValidationServiceInfo::struct_type_to_unit,
                           parent_struct_type.getName(),
                           unit_name},
                          parent_dd);
        }
        else {
            addDependency({ValidationServiceInfo::struct_type_to_unit,
                           parent_struct_type.getName(),
                           unit_name},
                          parent_dd);
            addDependency({ValidationServiceInfo::struct_type_to_base_unit,
                           parent_struct_type.getName(),
                           unit_name},
                          parent_dd);
            if (current_validation == ValidationInfo::ValidationLevel::valid) {
                current_validation = ValidationInfo::ValidationLevel::good_enough;
            }
            problems.push_back(
                (generateProblemMessage("StructType::Element",
                                        parent_struct_type.getName() + "." + element.getName(),
                                        "The used unit '" + unit_name + "' is not defined")));
        }
    }
    // other attributes
    if (element.getArraySize().isDynamicArraySize() &&
        !static_cast<bool>(parent_struct_type.getElements().get(
            element.getArraySize().getArraySizeElementName()))) {
        current_validation = ValidationInfo::ValidationLevel::invalid;
        problems.push_back(generateProblemMessage(
            "StructType::Element",
            parent_struct_type.getName() + "." + element.getName(),
            "The array_size_element_name '" + element.getArraySize().getArraySizeElementName() +
                "' element does not exist!"));
    }
    if (!element.getArraySize().isDynamicArraySize() &&
        element.getArraySize().getArraySizeValue() == 0) {
        current_validation = ValidationInfo::ValidationLevel::invalid;
        problems.push_back(
            generateProblemMessage("StructType::Element",
                                   parent_struct_type.getName() + "." + element.getName(),
                                   "The array_size with 0 is invalid!"));
    }
    if (!element.getValue().empty()) {
        auto enum_type = parent_dd.getEnumTypes().get(element.getTypeName());
        if (enum_type) {
            if (!static_cast<bool>(enum_type->getElements().get(element.getValue()))) {
                current_validation = ValidationInfo::ValidationLevel::invalid;
                problems.push_back(generateProblemMessage(
                    "StructType::Element",
                    parent_struct_type.getName() + "." + element.getName(),
                    "The value is set to '" + element.getValue() +
                        "' but this element is not in enum_type '" + element.getTypeName() + "'!"));
            }
        }
        else {
            current_validation = ValidationInfo::ValidationLevel::invalid;
            problems.push_back(
                generateProblemMessage("StructType::Element",
                                       parent_struct_type.getName() + "." + element.getName(),
                                       "The value is set, but the type is not an enum_type!"));
        }
    }

    if (!dd::AlignmentValidation::isValid(element.getAlignment())) {
        current_validation = ValidationInfo::ValidationLevel::invalid;
        problems.push_back(generateProblemMessage(
            "StructType::Element",
            parent_struct_type.getName() + "." + element.getName(),
            "The alignment with '" + std::to_string(element.getAlignment()) + "' is invalid!"));
    }
    return current_validation;
}

} // namespace

void ValidationInfo::update(datamodel::StructType& struct_type,
                            datamodel::DataDefinition& parent_dd)
{
    if (_currently_on_validation) {
        _valid = ValidationLevel::invalid;
        // recursion found
        addProblem(generateProblemMessage("StructType",
                                          struct_type.getName(),
                                          "Found a recursion! " + struct_type.getName() + " uses " +
                                              struct_type.getName() + " in recursion somehow!"));
        return;
    }

    _currently_on_validation = true;
    _validation_problems.clear();
    ValidationLevel current_validation = valid;
    // for struct type validation we need only to check each element
    for (auto& elem: struct_type.getElements()) {
        auto elem_valididation =
            updateValidationForElement(struct_type, *elem, _validation_problems, parent_dd);
        if (elem_valididation < current_validation) {
            current_validation = elem_valididation;
        }
    }

    if (struct_type.getAlignment() &&
        !dd::AlignmentValidation::isValid(*struct_type.getAlignment())) {
        current_validation = ValidationInfo::ValidationLevel::invalid;
        addProblem(generateProblemMessage(
            "StructType",
            struct_type.getName(),
            "The alignment of '" + std::to_string(*struct_type.getAlignment()) + "' is invalid!"));
    }

    _valid = current_validation;
    _currently_on_validation = false;
}

void ValidationInfo::update(datamodel::StreamMetaType& stream_meta_type,
                            datamodel::DataDefinition& parent_dd)
{
    if (_currently_on_validation) {
        _valid = ValidationLevel::invalid;
        // recursion found
        addProblem(generateProblemMessage("StreamMetaType",
                                          stream_meta_type.getName(),
                                          "Found a recursion! " + stream_meta_type.getName() +
                                              " uses " + stream_meta_type.getName() +
                                              "as parent in recursion somehow!"));
        return;
    }
    _currently_on_validation = true;
    _validation_problems.clear();
    ValidationLevel current_validation = valid;
    auto parent = stream_meta_type.getParent();
    if (!parent.empty()) {
        addDependency({ValidationServiceInfo::stream_meta_type_to_stream_meta_type,
                       stream_meta_type.getName(),
                       parent},
                      parent_dd);
        auto parent_meta_type = parent_dd.getStreamMetaTypes().access(parent);
        if (!parent_meta_type) {
            current_validation = invalid;
            addProblem(generateProblemMessage("StreamMetaType",
                                              stream_meta_type.getName(),
                                              "The parent stream meta type '" + parent +
                                                  "' is not defined"));
        }
        else {
            auto info = parent_meta_type->getInfo<ValidationInfo>();
            if (info == nullptr) {
                // do it like this because of recursion detection!
                parent_meta_type->setInfo<ValidationInfo>(std::make_shared<ValidationInfo>());
                info = parent_meta_type->getInfo<ValidationInfo>();
            }
            if (info) {
                info->update(*parent_meta_type, parent_dd);
                current_validation =
                    parent_meta_type->getInfo<ValidationInfo>()->getValidationLevel();
                if (current_validation != valid) {
                    addProblem(
                        generateProblemMessage("StreamMetaType",
                                               stream_meta_type.getName(),
                                               "The used parent '" + parent + "' has a problem"));
                }
            }
            else {
                current_validation = good_enough;
            }
        }
    }
    _valid = current_validation;
    _currently_on_validation = false;
}

ValidationInfo::ValidationLevel updateValidationForStreamStruct(
    const std::string& stream_name,
    datamodel::Stream::Struct& stream_struct,
    std::vector<ValidationInfo::Problem>& problems,
    datamodel::DataDefinition& parent_dd)
{
    ValidationInfo::ValidationLevel current_validation = ValidationInfo::valid;
    // this would detect a recursion
    ValidationInfo* validation_info = nullptr;
    std::string type_name = stream_struct.getTypeName();
    auto struct_type = parent_dd.getStructTypes().access(type_name);
    if (struct_type) {
        validation_info = struct_type->getInfo<ValidationInfo>();
        if (validation_info) {
            validation_info->update(*struct_type, parent_dd);
        }
        else {
            // do it in this order because of recursion detection
            struct_type->setInfo<ValidationInfo>(std::make_shared<ValidationInfo>());
            validation_info = struct_type->getInfo<ValidationInfo>();
            validation_info->update(*struct_type, parent_dd);
        }
    }
    addDependency({ValidationServiceInfo::stream_to_struct_type, stream_name, type_name},
                  parent_dd);
    if (validation_info == nullptr) {
        current_validation = ValidationInfo::invalid;
        if (type_name.empty()) {
            problems.push_back(generateProblemMessage("Stream::Struct",
                                                      stream_name + "." + stream_struct.getName(),
                                                      "The type is not set"));
        }
        else {
            problems.push_back(generateProblemMessage("Stream::Struct",
                                                      stream_name + "." + stream_struct.getName(),
                                                      "The set type '" + type_name +
                                                          "' is not defined in DataDefinition"));
        }
    }
    else {
        current_validation = validation_info->getValidationLevel();
        if (current_validation != ValidationInfo::valid) {
            problems.push_back(
                generateProblemMessage("Stream::Struct",
                                       stream_name + "." + stream_struct.getName(),
                                       "The set type '" + type_name + "' has a problem"));
        }
    }
    return current_validation;
}

void ValidationInfo::update(datamodel::Stream& stream, datamodel::DataDefinition& parent_dd)
{
    _validation_problems.clear();
    ValidationLevel current_validation = valid;

    auto stream_type_name = stream.getStreamTypeName();
    TypeOfType type_of_stream_type = invalid_type;
    auto stream_type_info =
        getOrCreateValidationInfo(stream_type_name, type_of_stream_type, parent_dd, true);
    if (stream_type_info) {
        if (type_of_stream_type == stream_meta_type) {
            addDependency({ValidationServiceInfo::stream_to_stream_meta_type,
                           stream.getName(),
                           stream_type_name},
                          parent_dd);
        }
        else {
            addDependency(
                {ValidationServiceInfo::stream_to_struct_type, stream.getName(), stream_type_name},
                parent_dd);
        }
        auto stream_struct_type_valididation = stream_type_info->getValidationLevel();
        if (stream_struct_type_valididation < current_validation) {
            current_validation = stream_struct_type_valididation;
            addProblem(
                generateProblemMessage("Stream",
                                       stream.getName(),
                                       "The set type '" + stream_type_name + "' has a problem"));
        }
    }
    else {
        current_validation = ValidationInfo::invalid;
        if (stream_type_name.empty()) {
            addProblem(generateProblemMessage("Stream", stream.getName(), "The type is not set"));
        }
        else {
            addProblem(generateProblemMessage("Stream",
                                              stream.getName(),
                                              "The set type '" + stream_type_name +
                                                  "' is not defined in DataDefinition"));
        }
    }

    // for stream validation need to check the stream structs
    for (auto& stream_struct: stream.getStructs()) {
        auto stream_struct_valididation = updateValidationForStreamStruct(
            stream.getName(), *stream_struct, _validation_problems, parent_dd);
        if (stream_struct_valididation < current_validation) {
            current_validation = stream_struct_valididation;
        }
    }

    // TODO: check the all other attributes
    _valid = current_validation;
}

namespace {
std::vector<std::string> findInMapTo(const std::string& find_to_type,
                                     const ValidationServiceInfo::ToFromMap& the_map)
{
    const auto& found_type_map = the_map.find(find_to_type);
    if (found_type_map != the_map.end()) {
        return found_type_map->second;
    }
    return {};
}
} // namespace

ValidationServiceInfo::InvalidatedTypes ValidationServiceInfo::forceRevalidationOfTypeDependencies(
    const std::string& type_name,
    ddl::dd::TypeOfType type,
    datamodel::DataDefinition& parent_dd) const
{
    ValidationServiceInfo::InvalidatedTypes invalidated_types_to_return;
    std::vector<std::string> invalidated_structs;
    std::vector<std::string> invalidated_enums;
    if (type == struct_type) {
        const auto& to_struct_type_map = _dependencies.find(struct_type_to_struct_type);
        if (to_struct_type_map != _dependencies.end()) {
            invalidated_structs = findInMapTo(type_name, to_struct_type_map->second);
            invalidated_types_to_return._struct_type_names = invalidated_structs;
        }
    }
    else if (type == enum_type) {
        const auto& to_struct_type_map = _dependencies.find(struct_type_to_enum_type);
        if (to_struct_type_map != _dependencies.end()) {
            invalidated_structs = findInMapTo(type_name, to_struct_type_map->second);
            invalidated_types_to_return._struct_type_names = invalidated_structs;
        }
    }
    else if (type == data_type) {
        const auto& to_struct_type_map = _dependencies.find(struct_type_to_data_type);
        if (to_struct_type_map != _dependencies.end()) {
            invalidated_structs = findInMapTo(type_name, to_struct_type_map->second);
            invalidated_types_to_return._struct_type_names = invalidated_structs;
        }
        const auto& to_enum_type_map = _dependencies.find(enum_type_to_data_type);
        if (to_enum_type_map != _dependencies.end()) {
            invalidated_enums = findInMapTo(type_name, to_enum_type_map->second);
            invalidated_types_to_return._enum_type_names = invalidated_enums;
        }
    }

    // set force invalid to the structs
    for (const auto& current: invalidated_structs) {
        auto struct_type = parent_dd.getStructTypes().access(current);
        if (struct_type) {
            auto info = struct_type->getInfo<ValidationInfo>();
            if (info) {
                if (info->isValid()) {
                    info->forceRevalidation();
                    auto invalidated_types = forceRevalidationOfTypeDependencies(
                        struct_type->getName(), struct_type->getTypeOfType(), parent_dd);
                    invalidated_types_to_return._enum_type_names.insert(
                        invalidated_types_to_return._enum_type_names.end(),
                        invalidated_types._enum_type_names.begin(),
                        invalidated_types._enum_type_names.end());
                    invalidated_types_to_return._struct_type_names.insert(
                        invalidated_types_to_return._struct_type_names.end(),
                        invalidated_types._struct_type_names.begin(),
                        invalidated_types._struct_type_names.end());
                }
            }
        }
    }

    // set force invalid to the enums
    for (const auto& current: invalidated_enums) {
        auto enum_type = parent_dd.getEnumTypes().access(current);
        if (enum_type) {
            auto info = enum_type->getInfo<ValidationInfo>();
            if (info) {
                info->forceRevalidation();
                if (info->isValid()) {
                    info->forceRevalidation();
                    auto invalidated_types = forceRevalidationOfTypeDependencies(
                        enum_type->getName(), enum_type->getTypeOfType(), parent_dd);
                    invalidated_types_to_return._enum_type_names.insert(
                        invalidated_types_to_return._enum_type_names.end(),
                        invalidated_types._enum_type_names.begin(),
                        invalidated_types._enum_type_names.end());
                    invalidated_types_to_return._struct_type_names.insert(
                        invalidated_types_to_return._struct_type_names.end(),
                        invalidated_types._struct_type_names.begin(),
                        invalidated_types._struct_type_names.end());
                }
            }
        }
    }
    return invalidated_types_to_return;
}

} // namespace dd
} // namespace ddl
