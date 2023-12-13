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

#include <ddl/dd/dd_predefined_datatypes.h>
#include <ddl/dd/dd_predefined_units.h>
#include <ddl/dd/dd_validationinfomodel.h>
#include <ddl/utilities/std_to_string.h>

#include <algorithm>

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
    the_map[to].insert(from);
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

std::unordered_set<std::string> removeInMapTo(const std::string& old_to_name,
                                              const std::string& new_to_name,
                                              ValidationServiceInfo::ToFromMap& the_map)
{
    if (old_to_name != new_to_name) {
        std::unordered_set<std::string> result = {};
        auto it = the_map.find(old_to_name);
        if (it != the_map.end()) {
            std::swap(it->second, result);
            the_map.erase(it);
        }
        return result;
    }
    else {
        auto it = the_map.find(old_to_name);
        if (it != the_map.end()) {
            return it->second;
        }
        return {};
    }
}

void renameInMapFrom(const std::string& from_old,
                     const std::string& from_new,
                     ValidationServiceInfo::ToFromMap& the_map)
{
    if (from_old != from_new) {
        for (auto& current_to: the_map) {
            auto& from_set = current_to.second;
            from_set.erase(from_old);
            from_set.insert(from_new);
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

template <typename T>
void updateValidationInfo(const std::shared_ptr<T>& type_ref, datamodel::DataDefinition& parent_ddl)
{
    if (type_ref) {
        auto info = type_ref->template getInfo<ValidationInfo>();
        if (info) {
            info->forceRevalidation();
            info->update(*type_ref, parent_ddl);
        }
    }
}
} // namespace

void ValidationServiceInfo::removed(const datamodel::DataType& data_type,
                                    datamodel::DataDefinition& parent_ddl)
{
    removeInMapFrom(data_type.getName(), _dependencies[data_type_to_unit]);
    auto dependencies =
        removeInMapTo(data_type.getName(), "", _dependencies[enum_type_to_data_type]);
    // this should invalidate the enum_type if there are values
    for (const auto& current_enum_name: dependencies) {
        const auto enum_type = parent_ddl.getEnumTypes().access(current_enum_name);
        updateValidationInfo<datamodel::EnumType>(enum_type, parent_ddl);
    }
    dependencies = removeInMapTo(data_type.getName(), "", _dependencies[struct_type_to_data_type]);
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
    auto dependencies =
        removeInMapTo(enum_type.getName(), "", _dependencies[struct_type_to_enum_type]);
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
        removeInMapTo(struct_type.getName(), "", _dependencies[struct_type_to_struct_type]);
    // this should invalidate the struct_types using it
    for (const auto& current_struct_name: dependencies) {
        const auto struct_type_to_access = parent_ddl.getStructTypes().access(current_struct_name);
        updateValidationInfo<datamodel::StructType>(struct_type_to_access, parent_ddl);
    }
    dependencies = removeInMapTo(struct_type.getName(), "", _dependencies[stream_to_struct_type]);
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

    auto dependencies = removeInMapTo(
        stream_meta_type.getName(), "", _dependencies[stream_meta_type_to_stream_meta_type]);
    // this should invalidate the struct_types using it
    for (const auto& current_stream_meta_name: dependencies) {
        const auto current_meta_type =
            parent_dd.getStreamMetaTypes().access(current_stream_meta_name);
        updateValidationInfo<datamodel::StreamMetaType>(current_meta_type, parent_dd);
    }
    dependencies =
        removeInMapTo(stream_meta_type.getName(), "", _dependencies[stream_to_stream_meta_type]);
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
    auto dependencies = removeInMapTo(base_unit.getName(), "", _dependencies[unit_to_base_unit]);
    for (const auto& current_unit_name: dependencies) {
        const auto unit = parent_dd.getUnits().access(current_unit_name);
        updateValidationInfo<datamodel::Unit>(unit, parent_dd);
    }
    // update all data types
    dependencies = removeInMapTo(base_unit.getName(), "", _dependencies[data_type_to_base_unit]);
    for (const auto& current_data_type_name: dependencies) {
        const auto data_type = parent_dd.getDataTypes().access(current_data_type_name);
        updateValidationInfo<datamodel::DataType>(data_type, parent_dd);
    }
    // update all struct types
    dependencies = removeInMapTo(base_unit.getName(), "", _dependencies[struct_type_to_base_unit]);
    for (const auto& current_struct_type_name: dependencies) {
        const auto struct_type = parent_dd.getStructTypes().access(current_struct_type_name);
        updateValidationInfo<datamodel::StructType>(struct_type, parent_dd);
    }
}

void ValidationServiceInfo::removed(const datamodel::UnitPrefix& unit_prefix,
                                    datamodel::DataDefinition& parent_dd)
{
    // update all units
    auto dependencies =
        removeInMapTo(unit_prefix.getName(), "", _dependencies[unit_to_unit_prefix]);
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
    auto dependencies = removeInMapTo(unit.getName(), "", _dependencies[data_type_to_unit]);
    for (const auto& current_data_type_name: dependencies) {
        auto data_type = parent_dd.getDataTypes().access(current_data_type_name);
        updateValidationInfo<datamodel::DataType>(data_type, parent_dd);
    }
    // update all struct types
    dependencies = removeInMapTo(unit.getName(), "", _dependencies[struct_type_to_unit]);
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
    auto dependencies =
        removeInMapTo(old_name, data_type.getName(), _dependencies[enum_type_to_data_type]);
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
    dependencies =
        removeInMapTo(old_name, data_type.getName(), _dependencies[struct_type_to_data_type]);
    // this should invalidate the struct_types
    for (const auto& current_struct_name: dependencies) {
        auto struct_type = parent_dd.getStructTypes().access(current_struct_name);
        // this is just for optimization, we need to to update ValidationInfo only once
        _validation_needed = false;
        if (struct_type) {
            // rename it
            if (data_type.getName() != old_name) {
                for (auto& elem: struct_type->getElements()) {
                    if (elem->getTypeName() == old_name) {
                        elem->setTypeName(data_type.getName());
                    }
                }
            }
        }
        // the setType should not invalidate, but this is also needed to readd dependencies
        updateValidationInfo<datamodel::StructType>(struct_type, parent_dd);
        _validation_needed = true;
    }
}

void ValidationServiceInfo::renamed(const datamodel::EnumType& enum_type,
                                    const std::string& old_name,
                                    datamodel::DataDefinition& parent_dd)
{
    renameInMapFrom(old_name, enum_type.getName(), _dependencies[enum_type_to_data_type]);

    auto dependencies =
        removeInMapTo(old_name, enum_type.getName(), _dependencies[struct_type_to_enum_type]);
    // this should invalidate the struct_types using it
    for (const auto& current_struct_name: dependencies) {
        auto struct_type = parent_dd.getStructTypes().access(current_struct_name);
        // this is just for optimization, we need to to update ValidationInfo only once
        _validation_needed = false;
        if (struct_type) {
            // rename it
            if (enum_type.getName() != old_name) {
                for (auto& elem: struct_type->getElements()) {
                    if (elem->getTypeName() == old_name) {
                        elem->setTypeName(enum_type.getName());
                    }
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

    auto dependencies =
        removeInMapTo(old_name, struct_type.getName(), _dependencies[struct_type_to_struct_type]);
    // this should invalidate the struct_types using it
    for (const auto& current_struct_name: dependencies) {
        auto struct_type_current = parent_dd.getStructTypes().access(current_struct_name);
        // this is just for optimization, we need to to update ValidationInfo only once
        _validation_needed = false;
        if (struct_type_current) {
            // rename it
            if (struct_type.getName() != old_name) {
                for (auto& elem: struct_type_current->getElements()) {
                    if (elem->getTypeName() == old_name) {
                        elem->setTypeName(struct_type.getName());
                    }
                }
            }
            // the setType should not invalidate, but this is also needed to readd dependencies
            updateValidationInfo<datamodel::StructType>(struct_type_current, parent_dd);
        }
        _validation_needed = true;
    }

    dependencies =
        removeInMapTo(old_name, struct_type.getName(), _dependencies[stream_to_struct_type]);
    // this should invalidate the streams
    for (const auto& current_stream_name: dependencies) {
        auto current_stream = parent_dd.getStreams().access(current_stream_name);
        // this is just for optimization, we need to to update ValidationInfo only once
        _validation_needed = false;
        if (current_stream) {
            if (struct_type.getName() != old_name) {
                for (auto& stream_struct: current_stream->getStructs()) {
                    if (stream_struct->getTypeName() == old_name) {
                        stream_struct->setTypeName(struct_type.getName());
                    }
                }
                if (current_stream->getStreamTypeName() == old_name) {
                    current_stream->setStreamTypeName(struct_type.getName());
                }
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

    auto dependencies = removeInMapTo(
        old_name, stream_meta_type.getName(), _dependencies[stream_meta_type_to_stream_meta_type]);
    // this should invalidate the struct_types using it
    for (const auto& current_stream_meta_type_name: dependencies) {
        auto current_stream_meta_type =
            parent_dd.getStreamMetaTypes().access(current_stream_meta_type_name);
        // this is just for optimization, we need to to update ValidationInfo only once
        _validation_needed = false;
        if (current_stream_meta_type) {
            if (current_stream_meta_type->getParent() == old_name) {
                if (stream_meta_type.getName() != old_name) {
                    // we do not reset the name here, because this is usually call on adding a type
                    // which was used before and we update the validation information only
                    current_stream_meta_type->setParent(stream_meta_type.getName());
                }
            }
            // the setType should not invalidate, but this is also needed to readd dependencies
            updateValidationInfo<datamodel::StreamMetaType>(current_stream_meta_type, parent_dd);
        }
        _validation_needed = true;
    }

    dependencies = removeInMapTo(
        old_name, stream_meta_type.getName(), _dependencies[stream_to_stream_meta_type]);
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
    auto dependencies =
        removeInMapTo(old_name, base_unit.getName(), _dependencies[struct_type_to_base_unit]);
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
    dependencies =
        removeInMapTo(old_name, base_unit.getName(), _dependencies[data_type_to_base_unit]);
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
    dependencies = removeInMapTo(old_name, base_unit.getName(), _dependencies[unit_to_base_unit]);
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
    auto dependencies =
        removeInMapTo(old_name, unit_prefix.getName(), _dependencies[unit_to_unit_prefix]);
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
    auto dependencies = removeInMapTo(old_name, unit.getName(), _dependencies[struct_type_to_unit]);
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
    dependencies = removeInMapTo(old_name, unit.getName(), _dependencies[data_type_to_unit]);
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

ValidationServiceInfo::ValidationProblem::ValidationProblem(ValidationLevel level,
                                                            const ddl::dd::Problem& problem)
    : _level(level), _problem(problem)
{
}

ValidationServiceInfo::ValidationProblem::ValidationProblem(const ddl::dd::Problem& problem)
    : ValidationProblem(ValidationLevel::invalid, problem)
{
}

ValidationLevel ValidationServiceInfo::ValidationProblem::getLevel() const
{
    return _level;
}

ddl::dd::Problem ValidationServiceInfo::ValidationProblem::getProblem() const
{
    return _problem;
}

void ValidationServiceInfo::removeProblem(ValidationProblemId id, ValidationLevel level)
{
    if (level < ValidationLevel::valid) {
        auto found_level_entry = _validation_problems.find(static_cast<uint8_t>(level));
        if (found_level_entry != _validation_problems.end()) {
            found_level_entry->second.erase(id);
        }
        if (found_level_entry->second.empty()) {
            _validation_problems.erase(found_level_entry);
        }
    }
}

void ValidationServiceInfo::updateProblem(ValidationProblemId id,
                                          const std::shared_ptr<const ValidationProblem>& problem)
{
    if (problem->getLevel() < ValidationLevel::valid) {
        _validation_problems[static_cast<uint8_t>(problem->getLevel())][id] = problem;
    }
}

ValidationLevel ValidationServiceInfo::getValidationLevel() const
{
    if (_validation_problems.empty()) {
        return ValidationLevel::valid;
    }
    else {
        return static_cast<ValidationLevel>(_validation_problems.rbegin()->first);
    }
}

std::vector<Problem> ValidationServiceInfo::getProblems() const
{
    std::vector<Problem> problems;
    size_t pre_size = 0;
    for (const auto& current_level: _validation_problems) {
        pre_size += current_level.second.size();
    }
    problems.reserve(pre_size);
    for (const auto& current_level: _validation_problems) {
        for (const auto& current_problem: current_level.second) {
            problems.push_back(current_problem.second->getProblem());
        }
    }
    return problems;
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
    const auto current_level = getValidationLevel();
    if (current_level == ValidationLevel::invalid) {
        return false;
    }
    else if (current_level >= level) {
        return true;
    }
    else {
        return false;
    }
}

ValidationInfo::ValidationLevel ValidationInfo::getValidationLevel() const
{
    if (!_is_validated) {
        return ValidationLevel::dont_know;
    }
    if (_validation_problems.empty()) {
        return ValidationLevel::valid;
    }
    return static_cast<ValidationInfo::ValidationLevel>(_validation_problems.rbegin()->first);
}

std::vector<ValidationInfo::Problem> ValidationInfo::getProblems() const
{
    std::vector<ValidationInfo::Problem> problems;
    size_t pre_size = 0;
    for (const auto& current_level: _validation_problems) {
        pre_size += current_level.second.size();
    }
    problems.reserve(pre_size);
    for (const auto& current_level: _validation_problems) {
        for (const auto& current_problem: current_level.second) {
            problems.push_back(current_problem.second->getProblem());
        }
    }
    return problems;
}

void ValidationInfo::addProblem(ValidationServiceInfo::ValidationProblem&& problem)
{
    auto problem_to_add =
        std::make_shared<ValidationServiceInfo::ValidationProblem>(std::move(problem));
    addProblem(problem_to_add);
}

void ValidationInfo::addProblem(const ValidationServiceInfo::ValidationProblem& problem)
{
    auto problem_to_add = std::make_shared<ValidationServiceInfo::ValidationProblem>(problem);
    addProblem(problem_to_add);
}

void ValidationInfo::addProblem(
    const std::shared_ptr<ValidationServiceInfo::ValidationProblem>& problem)
{
    if (problem->getLevel() < static_cast<ddl::dd::ValidationLevel>(ValidationLevel::valid)) {
        auto id = static_cast<ValidationServiceInfo::ValidationProblemId>(problem.get());
        _validation_problems[static_cast<uint8_t>(problem->getLevel())][id] = problem;
        if (_current_validation_service) {
            _current_validation_service->updateProblem(id, problem);
        }
    }
}

/**
 * Remove all problems
 */
void ValidationInfo::removeProblems(datamodel::DataDefinition& parent_dd)
{
    _current_validation_service = parent_dd.getInfo<ValidationServiceInfo>();
    if (_current_validation_service) {
        for (const auto& current_level: _validation_problems) {
            for (const auto& current_problem: current_level.second) {
                _current_validation_service->removeProblem(current_problem.first,
                                                           current_problem.second->getLevel());
            }
        }
    }
    _validation_problems.clear();
}

namespace {
ValidationInfo::Problem generateProblemMessage(const std::string& typeofmodel,
                                               const std::string& name,
                                               const std::string& problem_message)
{
    return {name, typeofmodel + " ('" + name + "'): " + problem_message};
}

/**
 * This function will retrieve the base unit with the @p name from the @p parent_dd.
 * If not found it will have a look at the (static) predefined base units and copy it to
 * @p parent_dd if found there.
 * @remark: This is to automatically add base units from previously incomplete defined data
 * definitions, where the predefined types were always added as not deletable references!
 */
std::shared_ptr<datamodel::BaseUnit> getOrCreateBaseUnit(const std::string& name,
                                                         datamodel::DataDefinition& parent_dd)
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

/**
 * This function will retrieve the unit prefix with the @p name from the @p parent_dd.
 * If not found it will have a look at the (static) predefined unit prefixes and copy it to
 * @p parent_dd if found there.
 * @remark: This is to automatically add unit prefixes from previously incomplete defined data
 * definitions, where the predefined where always there as a not deletable reference!
 */
std::shared_ptr<datamodel::UnitPrefix> getOrCreateUnitPrefix(const std::string& name,
                                                             datamodel::DataDefinition& parent_dd)
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
    if (!_is_validated) {
        removeProblems(parent_dd);
        // the unit may be dependent to base unit or prefix within its ref units
        for (auto& ref_unit: unit.getRefUnits()) {
            auto base_unit_name = ref_unit.getUnitName();
            if (!base_unit_name.empty()) {
                addDependency(
                    {ValidationServiceInfo::unit_to_base_unit, unit.getName(), base_unit_name},
                    parent_dd);
                auto base_unit = getOrCreateBaseUnit(base_unit_name, parent_dd);
                // this will invalidate the whole dd ! If a unit is defined it must be valid!
                if (!base_unit) {
                    addProblem(generateProblemMessage("Unit::RefUnit",
                                                      unit.getName(),
                                                      "BaseUnit '" + base_unit_name +
                                                          "' is not defined in DataDefinition"));
                }
            }
            else {
                addProblem(
                    generateProblemMessage("Unit::RefUnit", unit.getName(), "BaseUnit is empty"));
            }
            auto prefix_name = ref_unit.getPrefixName();
            if (!prefix_name.empty()) {
                addDependency(
                    {ValidationServiceInfo::unit_to_unit_prefix, unit.getName(), prefix_name},
                    parent_dd);
                auto prefix = getOrCreateUnitPrefix(prefix_name, parent_dd);
                if (!prefix) {
                    // this will invalidate the whole dd ! If a unit is defined it must be valid!
                    addProblem(generateProblemMessage("Unit::RefUnit",
                                                      unit.getName(),
                                                      "UnitPrefix '" + prefix_name +
                                                          "' is not defined in DataDefinition"));
                }
            }
            else {
                addProblem(
                    generateProblemMessage("Unit::RefUnit", unit.getName(), "UnitPrefix is empty"));
            }
        }
        _is_validated = true;
    }
}

void ValidationInfo::update(datamodel::DataType& data_type, datamodel::DataDefinition& parent_dd)
{
    if (!_is_validated) {
        removeProblems(parent_dd);
        const auto& unit_name = data_type.getUnitName();
        if (!unit_name.empty()) {
            // we do not care about the return value here
            getOrCreateBaseUnit(unit_name, parent_dd);
            const auto type_of_unit = parent_dd.getTypeOfUnit(unit_name);
            if (type_of_unit == base_unit) {
                addDependency(
                    {ValidationServiceInfo::data_type_to_base_unit, data_type.getName(), unit_name},
                    parent_dd);
            }
            else if (type_of_unit == unit) {
                const auto found_unit = parent_dd.getUnits().access(unit_name);
                auto validation_info = found_unit->getInfo<ValidationInfo>();
                if (validation_info == nullptr) {
                    found_unit->setInfo<ValidationInfo>(
                        std::make_shared<ValidationInfo>(*found_unit, parent_dd));
                    validation_info = found_unit->getInfo<ValidationInfo>();
                }
                if (validation_info->getValidationLevel() < ValidationLevel::valid) {
                    addProblem({ddl::dd::ValidationLevel::good_enough,
                                generateProblemMessage("DataType",
                                                       data_type.getName(),
                                                       "The used unit '" + unit_name +
                                                           "' has a Problem")});
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
                addProblem(
                    {ddl::dd::ValidationLevel::good_enough,
                     generateProblemMessage("DataType",
                                            data_type.getName(),
                                            "The used unit '" + unit_name + "' is not defined")});
            }
        }
        // other attributes
        if (data_type.getBitSize() == 0) {
            addProblem(generateProblemMessage("DataType",
                                              data_type.getName(),
                                              "The bit_size of " + data_type.getName() +
                                                  " is invalid (0 is not valid)"));
        }
        if (data_type.getArraySize() && *data_type.getArraySize() == 0) {
            addProblem(generateProblemMessage("DataType",
                                              data_type.getName(),
                                              "The array_size of '" + data_type.getName() +
                                                  "' is invalid (0 is not valid)"));
        }
        _is_validated = true;
    }
}

namespace {
/**
 * This function will retrieve the datatype with the @p name from the @p parent_dd.
 * If not found it will have a look at the (static) predefined datatype and copy it to
 * @p parent_dd if found there.
 * @remark: This is to automatically add datatypes from previously incomplete defined data
 * definitions!
 */
std::shared_ptr<datamodel::DataType> getOrCreateDataType(const std::string& name,
                                                         datamodel::DataDefinition& parent_dd)
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

void ValidationInfo::update(datamodel::EnumType& enum_type, datamodel::DataDefinition& parent_dd)
{
    if (!_is_validated) {
        removeProblems(parent_dd);
        const auto& data_type_name = enum_type.getDataTypeName();
        if (data_type_name.empty()) {
            addProblem(generateProblemMessage(
                "EnumType", enum_type.getName(), "The used data_type is not defined"));
        }
        else {
            addDependency({ValidationServiceInfo::enum_type_to_data_type,
                           enum_type.getName(),
                           data_type_name},
                          parent_dd);
            const auto data_type = getOrCreateDataType(data_type_name, parent_dd);
            if (!data_type) {
                addProblem(generateProblemMessage("EnumType",
                                                  enum_type.getName(),
                                                  "The used data_type '" + data_type_name +
                                                      "' is not defined"));
            }
            else {
                const auto info = data_type->getInfo<ValidationInfo>();
                if (info) {
                    if (data_type->getInfo<ValidationInfo>()->getValidationLevel() !=
                        ValidationLevel::valid) {
                        addProblem(generateProblemMessage("EnumType",
                                                          enum_type.getName(),
                                                          "The used data_type '" + data_type_name +
                                                              "' has a problem"));
                    }
                }
                else {
                    addProblem({ddl::dd::ValidationLevel::good_enough,
                                generateProblemMessage("EnumType",
                                                       enum_type.getName(),
                                                       "The used data_type '" + data_type_name +
                                                           "' has no validation state")});
                }
            }
        }
        _is_validated = true;
    }
}

void ValidationInfo::forceRevalidation()
{
    _is_validated = false;
}

namespace {

const ValidationInfo* getOrCreateValidationInfo(const std::string& type_name,
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
            auto dt = getOrCreateDataType(type_name, ddl);
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

void updateValidationForElement(datamodel::StructType& parent_struct_type,
                                datamodel::StructType::Element& element,
                                datamodel::DataDefinition& parent_dd)
{
    ValidationInfo* validation_info = parent_struct_type.getInfo<ValidationInfo>();
    if (!validation_info) {
        return;
    }
    TypeOfType type_returned = {};
    const auto& type_name = element.getTypeName();
    // this would detect a recursion
    auto validation_info_type =
        getOrCreateValidationInfo(type_name, type_returned, parent_dd, false);
    if (validation_info_type == nullptr) {
        if (type_name.empty()) {
            validation_info->addProblem(
                generateProblemMessage("StructType::Element",
                                       parent_struct_type.getName() + "." + element.getName(),
                                       "The used data_type is not set"));
        }
        else {
            validation_info->addProblem(generateProblemMessage(
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
        const auto current_validation = validation_info_type->getValidationLevel();
        if (current_validation != ValidationInfo::ValidationLevel::valid) {
            validation_info->addProblem(
                {static_cast<ddl::dd::ValidationLevel>(current_validation),
                 generateProblemMessage("StructType::Element",
                                        parent_struct_type.getName() + "." + element.getName(),
                                        "The used data type '" + type_name + "' has a problem")});
        }
    }
    // now we check the unit
    const auto& unit_name = element.getUnitName();
    if (!unit_name.empty()) {
        // we do not check the return value, this is just to add if it is a base unit
        getOrCreateBaseUnit(unit_name, parent_dd);
        const auto type_of_unit = parent_dd.getTypeOfUnit(unit_name);
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
                validation_info->addProblem(
                    {ValidationLevel::good_enough,
                     (generateProblemMessage("StructType::Element",
                                             parent_struct_type.getName() + "." + element.getName(),
                                             "The used unit '" + unit_name + "' has a Problem"))});
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
            validation_info->addProblem(
                {ValidationLevel::good_enough,
                 (generateProblemMessage("StructType::Element",
                                         parent_struct_type.getName() + "." + element.getName(),
                                         "The used unit '" + unit_name + "' is not defined"))});
        }
    }
    // other attributes
    if (element.getArraySize().isDynamicArraySize() &&
        !static_cast<bool>(parent_struct_type.getElements().get(
            element.getArraySize().getArraySizeElementName()))) {
        validation_info->addProblem(generateProblemMessage(
            "StructType::Element",
            parent_struct_type.getName() + "." + element.getName(),
            "The array_size_element_name '" + element.getArraySize().getArraySizeElementName() +
                "' element does not exist!"));
    }
    if (!element.getArraySize().isDynamicArraySize() &&
        element.getArraySize().getArraySizeValue() == 0) {
        validation_info->addProblem(
            generateProblemMessage("StructType::Element",
                                   parent_struct_type.getName() + "." + element.getName(),
                                   "The array_size with 0 is invalid!"));
    }
    if (!element.getValue().empty()) {
        const auto enum_type = parent_dd.getEnumTypes().get(element.getTypeName());
        if (enum_type) {
            if (!static_cast<bool>(enum_type->getElements().get(element.getValue()))) {
                validation_info->addProblem(generateProblemMessage(
                    "StructType::Element",
                    parent_struct_type.getName() + "." + element.getName(),
                    "The value is set to '" + element.getValue() +
                        "' but this element is not in enum_type '" + element.getTypeName() + "'!"));
            }
        }
        else {
            validation_info->addProblem(
                generateProblemMessage("StructType::Element",
                                       parent_struct_type.getName() + "." + element.getName(),
                                       "The value is set, but the type is not an enum_type!"));
        }
    }

    if (!dd::AlignmentValidation::isValid(element.getAlignment())) {
        validation_info->addProblem(generateProblemMessage(
            "StructType::Element",
            parent_struct_type.getName() + "." + element.getName(),
            "The alignment with '" + std::to_string(element.getAlignment()) + "' is invalid!"));
    }
}

} // namespace

void ValidationInfo::update(datamodel::StructType& struct_type,
                            datamodel::DataDefinition& parent_dd,
                            UpdateType update_type)
{
    if (!_is_validated) {
        if (_currently_on_validation) {
            // recursion found
            addProblem(generateProblemMessage("StructType",
                                              struct_type.getName(),
                                              "Found a recursion! " + struct_type.getName() +
                                                  " uses " + struct_type.getName() +
                                                  " in recursion somehow!"));
            _is_validated = true;
            return;
        }

        _currently_on_validation = true;
        // this is a shortcut for validation on a single added element at the end of the structure
        if (update_type == UpdateType::only_last) {
            auto& elems = struct_type.getElements();
            auto added_elem = elems.begin();
            if (elems.getSize() > 1) {
                std::advance(added_elem, elems.getSize() - 1);
            }
            updateValidationForElement(struct_type, *(*added_elem), parent_dd);
        }
        else {
            removeProblems(parent_dd);
            // for struct type validation we need only to check each element
            for (auto& elem: struct_type.getElements()) {
                updateValidationForElement(struct_type, *elem, parent_dd);
            }

            if (struct_type.getAlignment() &&
                !dd::AlignmentValidation::isValid(*struct_type.getAlignment())) {
                addProblem(generateProblemMessage("StructType",
                                                  struct_type.getName(),
                                                  "The alignment of '" +
                                                      std::to_string(*struct_type.getAlignment()) +
                                                      "' is invalid!"));
            }
        }
        _currently_on_validation = false;
        _is_validated = true;
    }
}

void ValidationInfo::update(datamodel::StreamMetaType& stream_meta_type,
                            datamodel::DataDefinition& parent_dd)
{
    if (!_is_validated) {
        if (_currently_on_validation) {
            // recursion found
            addProblem(generateProblemMessage("StreamMetaType",
                                              stream_meta_type.getName(),
                                              "Found a recursion! " + stream_meta_type.getName() +
                                                  " uses " + stream_meta_type.getName() +
                                                  "as parent in recursion somehow!"));
            _is_validated = true;
            return;
        }
        _currently_on_validation = true;
        removeProblems(parent_dd);
        const auto& parent = stream_meta_type.getParent();
        if (!parent.empty()) {
            addDependency({ValidationServiceInfo::stream_meta_type_to_stream_meta_type,
                           stream_meta_type.getName(),
                           parent},
                          parent_dd);
            auto parent_meta_type = parent_dd.getStreamMetaTypes().access(parent);
            if (!parent_meta_type) {
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
                    info->update(*parent_meta_type, parent_dd);
                }
                else {
                    info->forceRevalidation();
                    info->update(*parent_meta_type, parent_dd);
                }
                if (info) {
                    const auto current_validation =
                        parent_meta_type->getInfo<ValidationInfo>()->getValidationLevel();
                    if (current_validation != ValidationLevel::valid) {
                        addProblem({ddl::dd::ValidationLevel::good_enough,
                                    generateProblemMessage("StreamMetaType",
                                                           stream_meta_type.getName(),
                                                           "The used parent '" + parent +
                                                               "' has a problem")});
                    }
                }
                else {
                    addProblem({ddl::dd::ValidationLevel::good_enough,
                                generateProblemMessage("StreamMetaType",
                                                       stream_meta_type.getName(),
                                                       "The used parent '" + parent +
                                                           "' has no validation information")});
                }
            }
        }
        _currently_on_validation = false;
        _is_validated = true;
    }
}

void updateValidationForStreamStruct(datamodel::Stream& stream,
                                     datamodel::Stream::Struct& stream_struct,
                                     datamodel::DataDefinition& parent_dd)
{
    // this would detect a recursion
    auto validation_info_stream = stream.getInfo<ValidationInfo>();
    if (!validation_info_stream) {
        return;
    }
    ValidationInfo* validation_info_stream_struct = nullptr;
    const auto& type_name = stream_struct.getTypeName();
    auto struct_type = parent_dd.getStructTypes().access(type_name);
    if (struct_type) {
        validation_info_stream_struct = struct_type->getInfo<ValidationInfo>();
        if (validation_info_stream_struct) {
            validation_info_stream_struct->update(*struct_type, parent_dd);
        }
        else {
            // do it in this order because of recursion detection
            struct_type->setInfo<ValidationInfo>(std::make_shared<ValidationInfo>());
            validation_info_stream_struct = struct_type->getInfo<ValidationInfo>();
            validation_info_stream_struct->update(*struct_type, parent_dd);
        }
    }
    addDependency({ValidationServiceInfo::stream_to_struct_type, stream.getName(), type_name},
                  parent_dd);
    if (validation_info_stream_struct == nullptr) {
        if (type_name.empty()) {
            validation_info_stream->addProblem(
                generateProblemMessage("Stream::Struct",
                                       stream.getName() + "." + stream_struct.getName(),
                                       "The type is not set"));
        }
        else {
            validation_info_stream->addProblem(generateProblemMessage(
                "Stream::Struct",
                stream.getName() + "." + stream_struct.getName(),
                "The set type '" + type_name + "' is not defined in DataDefinition"));
        }
    }
    else {
        const auto current_validation = validation_info_stream_struct->getValidationLevel();
        if (current_validation != ValidationInfo::ValidationLevel::valid) {
            validation_info_stream->addProblem(
                {static_cast<ddl::dd::ValidationLevel>(current_validation),
                 generateProblemMessage("Stream::Struct",
                                        stream.getName() + "." + stream_struct.getName(),
                                        "The set type '" + type_name + "' has a problem")});
        }
    }
}

void ValidationInfo::update(datamodel::Stream& stream, datamodel::DataDefinition& parent_dd)
{
    if (!_is_validated) {
        removeProblems(parent_dd);

        const auto& stream_type_name = stream.getStreamTypeName();
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
                addDependency({ValidationServiceInfo::stream_to_struct_type,
                               stream.getName(),
                               stream_type_name},
                              parent_dd);
            }
            const auto stream_struct_type_valididation = stream_type_info->getValidationLevel();
            if (stream_struct_type_valididation != ValidationLevel::valid) {
                addProblem({static_cast<ddl::dd::ValidationLevel>(stream_struct_type_valididation),
                            generateProblemMessage("Stream",
                                                   stream.getName(),
                                                   "The set type '" + stream_type_name +
                                                       "' has a problem")});
            }
        }
        else {
            if (stream_type_name.empty()) {
                addProblem(
                    generateProblemMessage("Stream", stream.getName(), "The type is not set"));
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
            updateValidationForStreamStruct(stream, *stream_struct, parent_dd);
        }
        _is_validated = true;
    }
}

namespace {
const std::unordered_set<std::string>* findInMapTo(const std::string& find_to_type,
                                                   const ValidationServiceInfo::ToFromMap& the_map)
{
    const auto& found_type_map = the_map.find(find_to_type);
    if (found_type_map != the_map.end()) {
        return &found_type_map->second;
    }
    return nullptr;
}
} // namespace

void ValidationServiceInfo::forceRevalidationOfTypeDependencies(
    const std::string& type_name,
    ddl::dd::TypeOfType type,
    datamodel::DataDefinition& parent_dd,
    ValidationServiceInfo::InvalidatedTypes& invalidated_types_to_return) const
{
    const std::unordered_set<std::string>* invalidated_structs = nullptr;
    const std::unordered_set<std::string>* invalidated_enums = nullptr;
    if (type == struct_type) {
        const auto& to_struct_type_map = _dependencies.find(struct_type_to_struct_type);
        if (to_struct_type_map != _dependencies.end()) {
            invalidated_structs = findInMapTo(type_name, to_struct_type_map->second);
        }
    }
    else if (type == enum_type) {
        const auto& to_struct_type_map = _dependencies.find(struct_type_to_enum_type);
        if (to_struct_type_map != _dependencies.end()) {
            invalidated_structs = findInMapTo(type_name, to_struct_type_map->second);
        }
    }
    else if (type == data_type) {
        const auto& to_struct_type_map = _dependencies.find(struct_type_to_data_type);
        if (to_struct_type_map != _dependencies.end()) {
            invalidated_structs = findInMapTo(type_name, to_struct_type_map->second);
        }
        const auto& to_enum_type_map = _dependencies.find(enum_type_to_data_type);
        if (to_enum_type_map != _dependencies.end()) {
            invalidated_enums = findInMapTo(type_name, to_enum_type_map->second);
        }
    }

    if (invalidated_structs != nullptr) {
        // set force invalid to the structs
        for (const auto& current: *invalidated_structs) {
            auto struct_type = parent_dd.getStructTypes().access(current);
            if (struct_type) {
                auto info = struct_type->getInfo<ValidationInfo>();
                // if struct type already added find it and detect recursuin with that
                if (std::find(invalidated_types_to_return._struct_type_names.begin(),
                              invalidated_types_to_return._struct_type_names.end(),
                              struct_type->getName()) ==
                    invalidated_types_to_return._struct_type_names.end()) {
                    // we insert the value here to ensure recursion detection
                    invalidated_types_to_return._struct_type_names.push_back(current);
                    if (info) {
                        info->forceRevalidation();
                        forceRevalidationOfTypeDependencies(struct_type->getName(),
                                                            struct_type->getTypeOfType(),
                                                            parent_dd,
                                                            invalidated_types_to_return);
                    }
                }
            }
        }
    }

    // set force invalid to the enums
    if (invalidated_enums != nullptr) {
        for (const auto& current: *invalidated_enums) {
            auto enum_type = parent_dd.getEnumTypes().access(current);
            if (enum_type) {
                auto info = enum_type->getInfo<ValidationInfo>();
                invalidated_types_to_return._enum_type_names.push_back(current);
                if (info) {
                    info->forceRevalidation();
                    forceRevalidationOfTypeDependencies(enum_type->getName(),
                                                        enum_type->getTypeOfType(),
                                                        parent_dd,
                                                        invalidated_types_to_return);
                }
            }
        }
    }
}

} // namespace dd
} // namespace ddl
