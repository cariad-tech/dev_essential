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

#include <ddl/dd/dd.h>
#include <ddl/dd/dd_predefined_datatypes.h>
#include <ddl/dd/dd_typeinfomodel.h>
#include <ddl/dd/dd_validationinfomodel.h>

#include <memory>

namespace ddl {

namespace dd {

DataDefinition::DataDefinition() : DataDefinition(Version::getLatestVersion())
{
}

DataDefinition::DataDefinition(Version ddl_version)
{
    setModel(std::make_shared<datamodel::DataDefinition>(ddl_version));
}

DataDefinition::DataDefinition(const std::shared_ptr<datamodel::DataDefinition>& datamodel)
{
    setModel(datamodel);
}

DataDefinition::DataDefinition(const DataDefinition& other)
{
    setModel(std::make_shared<datamodel::DataDefinition>(*other.getModel()));
    _last_known_ddl_version = other._last_known_ddl_version;
}

DataDefinition::DataDefinition(DataDefinition&& other)
{
    detachFromModel();
    _datamodel = other._datamodel;
    _last_known_ddl_version = other._last_known_ddl_version;
    attachToModel();
}

DataDefinition& DataDefinition::operator=(const DataDefinition& other)
{
    setModel(std::make_shared<datamodel::DataDefinition>(*other.getModel()));
    _last_known_ddl_version = other._last_known_ddl_version;
    return *this;
}

DataDefinition& DataDefinition::operator=(DataDefinition&& other)
{
    detachFromModel();
    _datamodel = other._datamodel;
    _last_known_ddl_version = other._last_known_ddl_version;
    attachToModel();
    return *this;
}

DataDefinition::~DataDefinition()
{
    detachFromModel();
}

void DataDefinition::setModel(const std::shared_ptr<datamodel::DataDefinition>& datamodel)
{
    detachFromModel();
    _datamodel = datamodel;
    attachToModel();
    if (_datamodel) {
        auto info = _datamodel->getInfo<ValidationServiceInfo>();
        if (info == nullptr) {
            // we need to create our validation service
            // the validation service is for loose coupling for renaming and validation info
            _datamodel->setInfo<ValidationServiceInfo>(std::make_shared<ValidationServiceInfo>());
        }
        _last_known_ddl_version = _datamodel->getVersion();
        validate();
        calculatePositions("", invalid_type, false);
    }
}

std::shared_ptr<const datamodel::DataDefinition> DataDefinition::getModel() const
{
    return _datamodel;
}

void DataDefinition::attachToModel()
{
    if (_datamodel) {
        static_cast<datamodel::ModelSubject<datamodel::Header>*>(_datamodel.get())
            ->attachObserver(this);
        static_cast<datamodel::ModelSubject<datamodel::BaseUnit>*>(_datamodel.get())
            ->attachObserver(this);
        static_cast<datamodel::ModelSubject<datamodel::UnitPrefix>*>(_datamodel.get())
            ->attachObserver(this);
        static_cast<datamodel::ModelSubject<datamodel::Unit>*>(_datamodel.get())
            ->attachObserver(this);
        static_cast<datamodel::ModelSubject<datamodel::DataType>*>(_datamodel.get())
            ->attachObserver(this);
        static_cast<datamodel::ModelSubject<datamodel::EnumType>*>(_datamodel.get())
            ->attachObserver(this);
        static_cast<datamodel::ModelSubject<datamodel::StructType>*>(_datamodel.get())
            ->attachObserver(this);
        static_cast<datamodel::ModelSubject<datamodel::StreamMetaType>*>(_datamodel.get())
            ->attachObserver(this);
        static_cast<datamodel::ModelSubject<datamodel::Stream>*>(_datamodel.get())
            ->attachObserver(this);
    }
}
void DataDefinition::detachFromModel()
{
    if (_datamodel) {
        static_cast<datamodel::ModelSubject<datamodel::Header>*>(_datamodel.get())
            ->detachObserver(this);
        static_cast<datamodel::ModelSubject<datamodel::BaseUnit>*>(_datamodel.get())
            ->detachObserver(this);
        static_cast<datamodel::ModelSubject<datamodel::UnitPrefix>*>(_datamodel.get())
            ->detachObserver(this);
        static_cast<datamodel::ModelSubject<datamodel::Unit>*>(_datamodel.get())
            ->detachObserver(this);
        static_cast<datamodel::ModelSubject<datamodel::DataType>*>(_datamodel.get())
            ->detachObserver(this);
        static_cast<datamodel::ModelSubject<datamodel::EnumType>*>(_datamodel.get())
            ->detachObserver(this);
        static_cast<datamodel::ModelSubject<datamodel::StructType>*>(_datamodel.get())
            ->detachObserver(this);
        static_cast<datamodel::ModelSubject<datamodel::StreamMetaType>*>(_datamodel.get())
            ->detachObserver(this);
        static_cast<datamodel::ModelSubject<datamodel::Stream>*>(_datamodel.get())
            ->detachObserver(this);
    }
}

void DataDefinition::setVersion(const dd::Version& ddl_version)
{
    auto old_version = getVersion();
    // we set the change only if the version is different to prevent recalculation via model event
    if (old_version != ddl_version) {
        getHeader().setLanguageVersion(ddl_version);
    }
}

Version DataDefinition::getVersion() const
{
    return getHeader().getLanguageVersion();
}

void DataDefinition::setHeader(const Header& header)
{
    auto old_version = getVersion();
    getHeader().operator=(header);
    if (old_version != getHeader().getLanguageVersion()
        // this is to check if th version is already updated by event
        && _last_known_ddl_version != getHeader().getLanguageVersion()) {
        calculatePositions("", invalid_type, true);
    }
}

Header& DataDefinition::getHeader()
{
    return *_datamodel->getHeader();
}

const Header& DataDefinition::getHeader() const
{
    return *_datamodel->getHeader();
}

const DataDefinition::BaseUnits& DataDefinition::getBaseUnits() const
{
    return _datamodel->getBaseUnits();
}

DataDefinition::BaseUnits& DataDefinition::getBaseUnits()
{
    return _datamodel->getBaseUnits();
}

const DataDefinition::UnitPrefixes& DataDefinition::getUnitPrefixes() const
{
    return _datamodel->getUnitPrefixes();
}

DataDefinition::UnitPrefixes& DataDefinition::getUnitPrefixes()
{
    return _datamodel->getUnitPrefixes();
}

const DataDefinition::Units& DataDefinition::getUnits() const
{
    return _datamodel->getUnits();
}

DataDefinition::Units& DataDefinition::getUnits()
{
    return _datamodel->getUnits();
}

const DataDefinition::DataTypes& DataDefinition::getDataTypes() const
{
    return _datamodel->getDataTypes();
}

DataDefinition::DataTypes& DataDefinition::getDataTypes()
{
    return _datamodel->getDataTypes();
}

const DataDefinition::EnumTypes& DataDefinition::getEnumTypes() const
{
    return _datamodel->getEnumTypes();
}

DataDefinition::EnumTypes& DataDefinition::getEnumTypes()
{
    return _datamodel->getEnumTypes();
}

const DataDefinition::StructTypes& DataDefinition::getStructTypes() const
{
    return _datamodel->getStructTypes();
}

DataDefinition::StructTypes& DataDefinition::getStructTypes()
{
    return _datamodel->getStructTypes();
}

StructTypeAccess DataDefinition::getStructTypeAccess(const std::string& type_name) const
{
    auto struct_type = getStructTypes().get(type_name);
    if (struct_type) {
        return struct_type;
    }
    else {
        return nullptr;
    }
}

bool DataDefinition::containsType(const std::string& type_name) const
{
    return _datamodel->containsType(type_name);
}

TypeOfType DataDefinition::getTypeOfType(const std::string& type_name) const
{
    return _datamodel->getTypeOfType(type_name);
}

bool DataDefinition::containsUnit(const std::string& unit_name) const
{
    return _datamodel->containsUnit(unit_name);
}

TypeOfUnit DataDefinition::getTypeOfUnit(const std::string& unit_name) const
{
    return _datamodel->getTypeOfUnit(unit_name);
}

bool DataDefinition::isValid(ValidationLevel level) const
{
    ValidationLevel current_level = ValidationLevel::dont_know;
    if (_datamodel) {
        if (_datamodel->isEmpty()) {
            current_level = ValidationLevel::valid;
        }
        else {
            auto info = _datamodel->getInfo<ValidationServiceInfo>();
            if (info) {
                current_level = info->getValidationLevel();
            }
        }
    }

    if (level == ValidationLevel::invalid) {
        return false;
    }
    else if (current_level == ValidationLevel::invalid) {
        return false;
    }
    else {
        return (current_level >= level);
    }
}

const DataDefinition::StreamMetaTypes& DataDefinition::getStreamMetaTypes() const
{
    return _datamodel->getStreamMetaTypes();
}

DataDefinition::StreamMetaTypes& DataDefinition::getStreamMetaTypes()
{
    return _datamodel->getStreamMetaTypes();
}

const DataDefinition::Streams& DataDefinition::getStreams() const
{
    return _datamodel->getStreams();
}

DataDefinition::Streams& DataDefinition::getStreams()
{
    return _datamodel->getStreams();
}

namespace {

template <typename T>
void getOrCreateValidationLevelFor(std::shared_ptr<T>& type_val,
                                   datamodel::DataDefinition& parent_ddl,
                                   bool force_update)
{
    auto info = type_val->template getInfo<ValidationInfo>();
    if (info == nullptr) {
        // for recursion detection we need to create it first, then update!
        type_val->template setInfo<ValidationInfo>(std::make_shared<ValidationInfo>());
        info = type_val->template getInfo<ValidationInfo>();
        info->update(*type_val, parent_ddl);
    }
    else if (force_update || info->getValidationLevel() < ValidationInfo::ValidationLevel::valid) {
        info->forceRevalidation();
        info->update(*type_val, parent_ddl);
    }
}
} // namespace

void DataDefinition::validate(bool force_revalidation)
{
    if (_datamodel->isEmpty()) {
        return;
    }

    // validate data types
    for (auto& ref_types: getUnits()) {
        getOrCreateValidationLevelFor<Unit>(ref_types.second, *_datamodel, force_revalidation);
    }
    // validate data types
    for (auto& ref_types: getDataTypes()) {
        getOrCreateValidationLevelFor<DataType>(ref_types.second, *_datamodel, force_revalidation);
    }
    // validate enum types
    for (auto& ref_types: getEnumTypes()) {
        getOrCreateValidationLevelFor<EnumType>(ref_types.second, *_datamodel, force_revalidation);
    }
    // validate struct types
    for (auto& ref_types: getStructTypes()) {
        getOrCreateValidationLevelFor<StructType>(
            ref_types.second, *_datamodel, force_revalidation);
    }
    // validate stream meta types
    for (auto& ref_types: getStreamMetaTypes()) {
        getOrCreateValidationLevelFor<StreamMetaType>(
            ref_types.second, *_datamodel, force_revalidation);
    }

    // validate streams
    for (auto& ref_types: getStreams()) {
        getOrCreateValidationLevelFor<Stream>(ref_types.second, *_datamodel, force_revalidation);
    }
}

std::vector<ValidationInfo::Problem> DataDefinition::getValidationProtocol() const
{
    if (_datamodel) {
        auto info_service = _datamodel->getInfo<ValidationServiceInfo>();
        if (info_service) {
            return info_service->getProblems();
        }
    }
    return {};
}

void DataDefinition::calculatePositions(const std::string& type_name,
                                        TypeOfType type_of_type,
                                        bool force_recalculation)
{
    if (_datamodel->isEmpty()) {
        return;
    }
    bool calc_dt = (type_of_type == TypeOfType::data_type);
    bool calc_et = (type_of_type == TypeOfType::enum_type);
    bool calc_st = (type_of_type == TypeOfType::struct_type);
    if (type_of_type == TypeOfType::invalid_type) {
        calc_dt = calc_et = calc_st = true;
    }
    if (type_name.empty()) {
        // all datatypes
        if (calc_dt) {
            for (const auto& ref_types: getDataTypes()) {
                auto type_info = ref_types.second->getInfo<TypeInfo>();
                if (type_info == nullptr) {
                    // we need that order because of possible recursions!
                    ref_types.second->setInfo(std::make_shared<TypeInfo>());
                    type_info = ref_types.second->getInfo<TypeInfo>();
                    type_info->update(*(ref_types.second), *_datamodel);
                }
                else {
                    if (force_recalculation) {
                        type_info->update(*(ref_types.second), *_datamodel);
                    }
                }
            }
        }
        // all enum types
        if (calc_et) {
            for (const auto& ref_types: getEnumTypes()) {
                auto type_info = ref_types.second->getInfo<TypeInfo>();
                if (type_info == nullptr) {
                    // we need that order because of possible recursions!
                    ref_types.second->setInfo(std::make_shared<TypeInfo>());
                    type_info = ref_types.second->getInfo<TypeInfo>();
                    type_info->update(*(ref_types.second), *_datamodel);
                }
                else {
                    if (force_recalculation) {
                        type_info->update(*(ref_types.second), *_datamodel);
                    }
                }
            }
        }
        // calculate ALL structs
        if (calc_st) {
            for (const auto& ref_types: getStructTypes()) {
                auto type_info = ref_types.second->getInfo<TypeInfo>();
                if (type_info == nullptr) {
                    // we need that order because of possible recursions!
                    ref_types.second->setInfo(std::make_shared<TypeInfo>());
                    type_info = ref_types.second->getInfo<TypeInfo>();
                    type_info->update(
                        *(ref_types.second), *_datamodel, TypeInfo::UpdateType::force_all);
                }
                else {
                    type_info->update(*(ref_types.second),
                                      *_datamodel,
                                      force_recalculation ? TypeInfo::UpdateType::force_all :
                                                            TypeInfo::UpdateType::only_changed);
                }
            }
        }
    }
    else {
        const auto struct_type = getStructTypes().access(type_name);
        const auto enum_type = getEnumTypes().access(type_name);
        if (struct_type) {
            auto type_info = struct_type->getInfo<TypeInfo>();
            if (type_info == nullptr) {
                // we need that order because of possible recursion!
                struct_type->setInfo(std::make_shared<TypeInfo>());
                type_info = struct_type->getInfo<TypeInfo>();
                type_info->update(*(struct_type), *_datamodel, TypeInfo::UpdateType::force_all);
            }
            else {
                type_info->update(*(struct_type),
                                  *_datamodel,
                                  force_recalculation ? TypeInfo::UpdateType::force_all :
                                                        TypeInfo::UpdateType::only_changed);
            }
        }
        else if (enum_type) {
            auto type_info = enum_type->getInfo<TypeInfo>();
            if (type_info == nullptr) {
                // we need that order because of possible recursion!
                enum_type->setInfo(std::make_shared<TypeInfo>());
                type_info = enum_type->getInfo<TypeInfo>();
                type_info->update(*(enum_type), *_datamodel);
            }
            else {
                type_info->update(*(enum_type), *_datamodel);
            }
        }
        else {
            throw dd::Error("dd::DataDefinition::recalculatePositions",
                            {type_name},
                            "The type does not exists");
        }
    }
}

void DataDefinition::modelChanged(datamodel::ModelEventCode,
                                  datamodel::Header& changed_subject,
                                  const std::string&)
{
    // we need to check versioned changes here
    if (_last_known_ddl_version != changed_subject.getLanguageVersion()) {
        calculatePositions({}, TypeOfType::struct_type, true);
        _last_known_ddl_version = changed_subject.getLanguageVersion();
    }
}

namespace {
// remove all validations
template <typename T>
void removeAllValidations(T& item, datamodel::DataDefinition& parent_dd)
{
    auto info = item.template getInfo<ValidationInfo>();
    if (info) {
        info->removeProblems(parent_dd);
    }
}
} // namespace

void DataDefinition::modelChanged(datamodel::ModelEventCode event_code,
                                  datamodel::BaseUnit& changed_subject,
                                  const std::string& additional_info)
{
    if (event_code == datamodel::ModelEventCode::item_renamed) {
        _datamodel->getInfo<ValidationServiceInfo>()->renamed(
            changed_subject, additional_info, *_datamodel);
    }
    else if (event_code == datamodel::ModelEventCode::item_removed) {
        _datamodel->getInfo<ValidationServiceInfo>()->removed(changed_subject, *_datamodel);
        removeAllValidations(changed_subject, *_datamodel);
    }
}

void DataDefinition::modelChanged(datamodel::ModelEventCode event_code,
                                  datamodel::UnitPrefix& changed_subject,
                                  const std::string& additional_info)
{
    if (event_code == datamodel::ModelEventCode::item_renamed) {
        _datamodel->getInfo<ValidationServiceInfo>()->renamed(
            changed_subject, additional_info, *_datamodel);
    }
    else if (event_code == datamodel::ModelEventCode::item_removed) {
        _datamodel->getInfo<ValidationServiceInfo>()->removed(changed_subject, *_datamodel);
        removeAllValidations(changed_subject, *_datamodel);
    }
}

void DataDefinition::modelChanged(datamodel::ModelEventCode event_code,
                                  datamodel::Unit& changed_subject,
                                  const std::string& additional_info)
{
    if (event_code == datamodel::ModelEventCode::item_added) {
        changed_subject.setInfo<ValidationInfo>(
            std::make_shared<ValidationInfo>(changed_subject, *_datamodel));
    }
    else if (event_code == datamodel::ModelEventCode::subitem_added ||
             event_code == datamodel::ModelEventCode::subitem_removed) {
        changed_subject.getInfo<ValidationInfo>()->update(changed_subject, *_datamodel);
    }
    else if (event_code == datamodel::ModelEventCode::subitem_changed) {
        if (additional_info == "prefix_name" || additional_info == "unit_name") {
            auto valid_info_service = _datamodel->getInfo<ValidationServiceInfo>();
            auto valid_info = changed_subject.getInfo<ValidationInfo>();
            // switch off if this change comes from the Validation service while renaming
            // the validation service will take care of this by itself
            if (valid_info_service->validationNeeded()) {
                valid_info->forceRevalidation();
                valid_info->update(changed_subject, *_datamodel);
            }
        }
    }
    else if (event_code == datamodel::ModelEventCode::item_renamed) {
        _datamodel->getInfo<ValidationServiceInfo>()->renamed(
            changed_subject, additional_info, *_datamodel);
    }
    else if (event_code == datamodel::ModelEventCode::item_removed) {
        _datamodel->getInfo<ValidationServiceInfo>()->removed(changed_subject, *_datamodel);
        removeAllValidations(changed_subject, *_datamodel);
    }
}

namespace {
void updateInfoAllStructs(const std::vector<std::string>& struct_names,
                          bool with_type_info,
                          datamodel::DataDefinition& parent_dd)
{
    for (const auto& current_dependency_type: struct_names) {
        auto struct_type = parent_dd.getStructTypes().access(current_dependency_type);
        if (struct_type) {
            auto struct_info = struct_type->getInfo<ValidationInfo>();
            struct_info->update(*struct_type, parent_dd);
            if (with_type_info) {
                if (struct_info->isValid(ValidationInfo::ValidationLevel::good_enough)) {
                    struct_type->getInfo<TypeInfo>()->update(
                        *struct_type, parent_dd, TypeInfo::UpdateType::force_all);
                }
            }
        }
    }
}

void updateInfoAllEnums(const std::vector<std::string>& enum_names,
                        datamodel::DataDefinition& parent_dd)
{
    for (const auto& current_dependency_type: enum_names) {
        auto enum_type = parent_dd.getEnumTypes().access(current_dependency_type);
        if (enum_type) {
            auto enum_type_info = enum_type->getInfo<ValidationInfo>();
            enum_type_info->update(*enum_type, parent_dd);
            if (enum_type_info->isValid(ValidationInfo::ValidationLevel::good_enough)) {
                enum_type->getInfo<TypeInfo>()->update(*enum_type, parent_dd);
            }
        }
    }
}
} // namespace

void DataDefinition::modelChanged(datamodel::ModelEventCode event_code,
                                  datamodel::DataType& changed_subject,
                                  const std::string& additional_info)
{
    if (event_code == datamodel::ModelEventCode::item_added) {
        // check if a valid typeinfo was copied
        auto existing_type_info = changed_subject.getInfo<TypeInfo>();
        if (!existing_type_info) {
            changed_subject.setInfo<TypeInfo>(
                std::make_shared<TypeInfo>(changed_subject, *_datamodel));
        }
        changed_subject.setInfo<ValidationInfo>(
            std::make_shared<ValidationInfo>(changed_subject, *_datamodel));
        _datamodel->getInfo<ValidationServiceInfo>()->renamed(
            changed_subject, changed_subject.getName(), *_datamodel);
    }
    else if (event_code == datamodel::ModelEventCode::item_changed) {
        // almost every change on a type will invalidate the type info (except description, but we
        // do not check that here
        auto valid_info_service = _datamodel->getInfo<ValidationServiceInfo>();
        auto valid_info = changed_subject.getInfo<ValidationInfo>();
        if (additional_info == "unit_name") {
            // switch off if this change comes from the Validation service while renaming
            // the validation service will take care of this by itself
            if (valid_info_service->validationNeeded()) {
                valid_info->forceRevalidation();
                valid_info->update(changed_subject, *_datamodel);
            }
        }
        else {
            if (valid_info_service->validationNeeded()) {
                valid_info->forceRevalidation();
                valid_info->update(changed_subject, *_datamodel);
                changed_subject.getInfo<TypeInfo>()->update(changed_subject, *_datamodel);
                ValidationServiceInfo::InvalidatedTypes dependencies;
                valid_info_service->forceRevalidationOfTypeDependencies(
                    changed_subject.getName(),
                    changed_subject.getTypeOfType(),
                    *_datamodel,
                    dependencies);
                updateInfoAllStructs(dependencies._struct_type_names, true, *_datamodel);
                updateInfoAllEnums(dependencies._enum_type_names, *_datamodel);
            }
        }
    }
    else if (event_code == datamodel::ModelEventCode::item_renamed) {
        _datamodel->getInfo<ValidationServiceInfo>()->renamed(
            changed_subject, additional_info, *_datamodel);
    }
    else if (event_code == datamodel::ModelEventCode::item_removed) {
        _datamodel->getInfo<ValidationServiceInfo>()->removed(changed_subject, *_datamodel);
        removeAllValidations(changed_subject, *_datamodel);
    }
}

void DataDefinition::modelChanged(datamodel::ModelEventCode event_code,
                                  datamodel::EnumType& changed_subject,
                                  const std::string& additional_info)
{
    if (event_code == datamodel::ModelEventCode::item_added) {
        // check if a valid typeinfo was copied
        auto existing_type_info = changed_subject.getInfo<TypeInfo>();
        if (!existing_type_info) {
            changed_subject.setInfo<TypeInfo>(
                std::make_shared<TypeInfo>(changed_subject, *_datamodel));
        }
        changed_subject.setInfo<ValidationInfo>(
            std::make_shared<ValidationInfo>(changed_subject, *_datamodel));
        _datamodel->getInfo<ValidationServiceInfo>()->renamed(
            changed_subject, changed_subject.getName(), *_datamodel);
    }
    else if (event_code == datamodel::ModelEventCode::item_changed) {
        // validation and typeinfo depends only on the data_type_name, nothing more!
        bool is_relevant_change = (additional_info == "data_type_name");
        if (is_relevant_change) {
            auto valid_info_service = _datamodel->getInfo<ValidationServiceInfo>();
            // switch off if this change comes from the Validation service while renaming
            // the validation service will take care of this by itself
            if (valid_info_service->validationNeeded()) {
                // we need to invalidate everything before to force revalidation
                ValidationServiceInfo::InvalidatedTypes dependencies;
                valid_info_service->forceRevalidationOfTypeDependencies(
                    changed_subject.getName(),
                    changed_subject.getTypeOfType(),
                    *_datamodel,
                    dependencies);
                auto info = changed_subject.getInfo<ValidationInfo>();
                info->forceRevalidation();
                info->update(changed_subject, *_datamodel);
                updateInfoAllStructs(dependencies._struct_type_names, true, *_datamodel);
                if (info->isValid(ValidationInfo::ValidationLevel::good_enough)) {
                    changed_subject.getInfo<TypeInfo>()->update(changed_subject, *_datamodel);
                }
            }
        }
    }
    else if (event_code == datamodel::ModelEventCode::item_renamed) {
        _datamodel->getInfo<ValidationServiceInfo>()->renamed(
            changed_subject, additional_info, *_datamodel);
    }
    else if (event_code == datamodel::ModelEventCode::item_removed) {
        _datamodel->getInfo<ValidationServiceInfo>()->removed(changed_subject, *_datamodel);
        removeAllValidations(changed_subject, *_datamodel);
    }
    // we do not react on subitem changes for type info
}

void DataDefinition::modelChanged(datamodel::ModelEventCode event_code,
                                  datamodel::StructType& changed_subject,
                                  const std::string& additional_info)
{
    switch (event_code) {
    case datamodel::ModelEventCode::item_added: {
        // struct may have recursion, thats why we have no CTOR on that, first we set it, then we
        // update it
        changed_subject.setInfo<ValidationInfo>(std::make_shared<ValidationInfo>());
        changed_subject.getInfo<ValidationInfo>()->update(changed_subject, *_datamodel);
        // struct may have recursion, thats why we have no CTOR on that, first we set it, then we
        // update it
        // check if a valid typeinfo was copied
        auto existing_type_info = changed_subject.getInfo<TypeInfo>();
        if (!existing_type_info) {
            changed_subject.setInfo<TypeInfo>(std::make_shared<TypeInfo>());
            changed_subject.getInfo<TypeInfo>()->update(changed_subject, *_datamodel);
        }
        else {
            // if this was copied we ned onl to fix the references
            // if this does not work the implementation will revalidate all
            existing_type_info->update(
                changed_subject, *_datamodel, TypeInfo::UpdateType::only_reference);
        }
        _datamodel->getInfo<ValidationServiceInfo>()->renamed(
            changed_subject, changed_subject.getName(), *_datamodel);
    } break;
    case datamodel::ModelEventCode::item_removed:
        _datamodel->getInfo<ValidationServiceInfo>()->removed(changed_subject, *_datamodel);
        removeAllValidations(changed_subject, *_datamodel);
        break;
    case datamodel::ModelEventCode::subitem_changed: {
        auto valid_info_service = _datamodel->getInfo<ValidationServiceInfo>();
        // switch off if this change comes from the Validation service while renaming
        // the validation service will take care of this by itself
        if (valid_info_service->validationNeeded()) {
            // We need to force revalidation because of recursion detection
            ValidationServiceInfo::InvalidatedTypes dependencies;
            valid_info_service->forceRevalidationOfTypeDependencies(changed_subject.getName(),
                                                                    changed_subject.getTypeOfType(),
                                                                    *_datamodel,
                                                                    dependencies);
            auto info = changed_subject.getInfo<ValidationInfo>();
            info->forceRevalidation();
            info->update(changed_subject, *_datamodel);
            // calculate position after validation
            const bool update_type_info =
                additional_info == "type_name" || additional_info == "alignment" ||
                additional_info == "byte_pos" || additional_info == "num_bits" ||
                additional_info == "array_size";
            if (update_type_info) {
                changed_subject.getInfo<TypeInfo>()->update(
                    changed_subject, *_datamodel, TypeInfo::UpdateType::force_all);
            }
            updateInfoAllStructs(dependencies._struct_type_names, update_type_info, *_datamodel);
        }

    } break;
    case datamodel::ModelEventCode::item_changed: {
        const bool relevant_change =
            ("language_version" == additional_info) || ("alignment" == additional_info);
        if (relevant_change) {
            auto valid_info_service = _datamodel->getInfo<ValidationServiceInfo>();
            // force revalidation of all depencencies
            // no check for validation needed because of language_version and alignment is no
            // renaming thing
            ValidationServiceInfo::InvalidatedTypes dependencies;
            valid_info_service->forceRevalidationOfTypeDependencies(changed_subject.getName(),
                                                                    changed_subject.getTypeOfType(),
                                                                    *_datamodel,
                                                                    dependencies);
            auto info = changed_subject.getInfo<ValidationInfo>();
            info->forceRevalidation();
            info->update(changed_subject, *_datamodel);
            changed_subject.getInfo<TypeInfo>()->update(
                changed_subject, *_datamodel, TypeInfo::UpdateType::force_all);
            updateInfoAllStructs(dependencies._struct_type_names, true, *_datamodel);
        }
    } break;
    case datamodel::ModelEventCode::subitem_removed:
    case datamodel::ModelEventCode::subitem_popped:
        // this is the case if one element is added, this must be optimized in runtime !! (used for
        // DDStructure!)
        {
            const auto update_type = (event_code == datamodel::ModelEventCode::subitem_popped) ?
                                         TypeInfo::UpdateType::only_last :
                                         TypeInfo::UpdateType::force_all;
            changed_subject.getInfo<TypeInfo>()->update(changed_subject, *_datamodel, update_type);
            if (update_type == TypeInfo::UpdateType::force_all) {
                auto valid_info = changed_subject.getInfo<ValidationInfo>();
                valid_info->forceRevalidation();
                valid_info->update(changed_subject, *_datamodel);
            }
        }
        break;
    case datamodel::ModelEventCode::item_renamed:
        _datamodel->getInfo<ValidationServiceInfo>()->renamed(
            changed_subject, additional_info, *_datamodel);
        break;
    case datamodel::ModelEventCode::subitem_inserted:
    case datamodel::ModelEventCode::subitem_added:
        // this is the case if one element is added, this must be optimized in runtime !! (used for
        // DDStructure!)
        {
            auto valid_info = changed_subject.getInfo<ValidationInfo>();
            const ValidationInfo::UpdateType valid_update_type =
                (event_code == datamodel::ModelEventCode::subitem_added) ?
                    ValidationInfo::UpdateType::only_last :
                    ValidationInfo::UpdateType::all;
            const TypeInfo::UpdateType type_update_type =
                (event_code == datamodel::ModelEventCode::subitem_added) ?
                    TypeInfo::UpdateType::only_last :
                    TypeInfo::UpdateType::only_changed;
            valid_info->forceRevalidation();
            valid_info->update(changed_subject, *_datamodel, valid_update_type);
            changed_subject.getInfo<TypeInfo>()->update(
                changed_subject, *_datamodel, type_update_type);
        }
        break;
    case datamodel::ModelEventCode::subitem_renamed:
        break;
    }
}

void DataDefinition::modelChanged(datamodel::ModelEventCode event_code,
                                  datamodel::StreamMetaType& changed_subject,
                                  const std::string& additional_info)
{
    if (event_code == datamodel::ModelEventCode::item_added) {
        changed_subject.setInfo<ValidationInfo>(std::make_shared<ValidationInfo>());
        changed_subject.getInfo<ValidationInfo>()->update(changed_subject, *_datamodel);
        _datamodel->getInfo<ValidationServiceInfo>()->renamed(
            changed_subject, changed_subject.getName(), *_datamodel);
    }
    else if (event_code == datamodel::ModelEventCode::item_changed) {
        // validation and typeinfo depends only on the data_type_name, nothing more!
        bool is_relevant_change = (additional_info == "parent");
        if (is_relevant_change) {
            auto valid_info_service = _datamodel->getInfo<ValidationServiceInfo>();
            // switch off if this change comes from the Validation service while renaming
            // the validation service will take care of this by itself
            if (valid_info_service->validationNeeded()) {
                auto info = changed_subject.getInfo<ValidationInfo>();
                info->forceRevalidation();
                info->update(changed_subject, *_datamodel);
            }
        }
    }
    else if (event_code == datamodel::ModelEventCode::item_renamed) {
        _datamodel->getInfo<ValidationServiceInfo>()->renamed(
            changed_subject, additional_info, *_datamodel);
    }
    else if (event_code == datamodel::ModelEventCode::item_removed) {
        _datamodel->getInfo<ValidationServiceInfo>()->removed(changed_subject, *_datamodel);
        removeAllValidations(changed_subject, *_datamodel);
    }
    // we do not react on subitem changes for type info
}

void DataDefinition::modelChanged(datamodel::ModelEventCode event_code,
                                  datamodel::Stream& changed_subject,
                                  const std::string& additional_info)
{
    if (event_code == datamodel::ModelEventCode::item_added) {
        changed_subject.setInfo<ValidationInfo>(
            std::make_shared<ValidationInfo>(changed_subject, *_datamodel));
    }
    else if (event_code == datamodel::ModelEventCode::item_changed) {
        // validation and typeinfo depends only on the data_type_name, nothing more!
        bool is_relevant_change = (additional_info == "stream_type_name");
        if (is_relevant_change) {
            auto valid_info_service = _datamodel->getInfo<ValidationServiceInfo>();
            // switch off if this change comes from the Validation service while renaming
            // the validation service will take care of this by itself
            if (valid_info_service->validationNeeded()) {
                // validation and typeinfo depends only on the data_type_name, nothing more!
                auto info = changed_subject.getInfo<ValidationInfo>();
                info->forceRevalidation();
                info->update(changed_subject, *_datamodel);
            }
        }
    }
    else if (event_code == datamodel::ModelEventCode::item_renamed) {
        _datamodel->getInfo<ValidationServiceInfo>()->renamed(
            changed_subject, additional_info, *_datamodel);
    }
    else if (event_code == datamodel::ModelEventCode::item_removed) {
        _datamodel->getInfo<ValidationServiceInfo>()->removed(changed_subject, *_datamodel);
        removeAllValidations(changed_subject, *_datamodel);
    }
    // we do not react on subitem changes for type info
}

/******************************************************************************************
 * adding things
 ******************************************************************************************/
void DataDefinition::add(const BaseUnit& base_unit, const DataDefinition&)
{
    getBaseUnits().add(base_unit);
}

void DataDefinition::add(const UnitPrefix& unit_prefix, const DataDefinition&)
{
    getUnitPrefixes().add(unit_prefix);
}

void DataDefinition::add(const Unit& unit, const DataDefinition& source_ddl)
{
    for (const auto& ref_unit: unit.getRefUnits()) {
        auto base_unit = source_ddl.getBaseUnits().get(ref_unit.getUnitName());
        if (base_unit) {
            getBaseUnits().add(*base_unit);
        }
        auto prefix = source_ddl.getUnitPrefixes().get(ref_unit.getPrefixName());
        if (prefix) {
            getUnitPrefixes().add(*prefix);
        }
    }
    getUnits().add(unit);
}

void DataDefinition::add(const DataType& data_type, const DataDefinition& source_dd)
{
    const auto& unit_name = data_type.getUnitName();
    if (!unit_name.empty()) {
        if (!addUnitByName(unit_name, source_dd, *this)) {
            throw dd::Error("DataDefinition::add",
                            {data_type.getName()},
                            "datatype defines an unit with " + unit_name +
                                " but can not find in source DD");
        }
    }
    getDataTypes().add(data_type);
}

void DataDefinition::add(const EnumType& enum_type, const DataDefinition& source_ddl)
{
    auto data_type = source_ddl.getDataTypes().get(enum_type.getDataTypeName());
    // we hope the type is already defined if it is not in source, but we figure out
    if (data_type) {
        add(*data_type, source_ddl);
    }
    else {
        data_type =
            PredefinedDataTypes::getInstance().getPredefinedType(enum_type.getDataTypeName());
        if (data_type) {
            add(*data_type, source_ddl);
        }
        else {
            throw dd::Error("DataDefinition::add",
                            {enum_type.getName()},
                            "enumtype defines a datatype with " + enum_type.getDataTypeName() +
                                " but can not find in source DD and is not a predefined type");
        }
    }
    getEnumTypes().add(enum_type);
}

void DataDefinition::add(const StructType& struct_type, const DataDefinition& source_ddl)
{
    if (_recursion_detection_struct.size() > 0) {
        if (_recursion_detection_struct.find(struct_type.getName()) !=
            _recursion_detection_struct.end()) {
            _recursion_detection_struct.clear();
            throw dd::Error("DataDefinition::add",
                            {struct_type.getName()},
                            "recursion detected for StructType :" + struct_type.getName());
        }
    }
    try {
        _recursion_detection_struct[struct_type.getName()] = struct_type.getName();
        for (auto current_elem = struct_type.getElements().cbegin();
             current_elem != struct_type.getElements().cend();
             ++current_elem) {
            const auto& unit_name = (*current_elem)->getUnitName();
            if (!unit_name.empty()) {
                addUnitByName(unit_name, source_ddl, *this);
            }
            const auto& type_name_to_add = (*current_elem)->getTypeName();
            addTypeByName(type_name_to_add, source_ddl, *this);
        }
        getStructTypes().add(struct_type);
    }
    catch (const dd::Error& oErr) {
        _recursion_detection_struct.clear();
        throw oErr;
    }
    _recursion_detection_struct.erase(struct_type.getName());
}

void DataDefinition::add(const StreamMetaType& stream_meta_type, const DataDefinition& source_ddl)
{
    if (_recursion_detection_stream_meta_types.size() > 0) {
        if (_recursion_detection_stream_meta_types.find(stream_meta_type.getName()) !=
            _recursion_detection_stream_meta_types.end()) {
            _recursion_detection_stream_meta_types.clear();
            throw dd::Error("DataDefinition::add",
                            {stream_meta_type.getName()},
                            "recursion detected for StreamMetaType :" + stream_meta_type.getName());
        }
    }
    try {
        _recursion_detection_stream_meta_types[stream_meta_type.getName()] =
            stream_meta_type.getName();
        auto parent = stream_meta_type.getParent();
        if (!parent.empty()) {
            auto meta_type_parent = source_ddl.getStreamMetaTypes().get(parent);
            if (meta_type_parent) {
                add(*meta_type_parent, source_ddl);
            }
        }

        getStreamMetaTypes().add(stream_meta_type);
    }
    catch (const dd::Error& oErr) {
        _recursion_detection_stream_meta_types.clear();
        // rethrow
        throw oErr;
    }

    _recursion_detection_stream_meta_types.erase(stream_meta_type.getName());
}

void DataDefinition::add(const Stream& stream, const DataDefinition& source_ddl)
{
    auto stream_type = stream.getStreamTypeName();
    if (!stream_type.empty()) {
        if (!addStreamTypeByName(stream_type, source_ddl, *this)) {
        }
    }
    for (auto stream_struct = stream.getStructs().cbegin();
         stream_struct != stream.getStructs().cend();
         ++stream_struct) {
        auto stream_struct_type_name = (*stream_struct)->getTypeName();
        auto struct_type = source_ddl.getStructTypes().get(stream_struct_type_name);
        if (struct_type) {
            add(*struct_type, source_ddl);
        }
    }
    getStreams().add(stream);
}

void DataDefinition::add(const DataDefinition& source_ddl)
{
    for (const auto& base_unit: source_ddl.getBaseUnits()) {
        add(*base_unit.second, source_ddl);
    }
    for (const auto& unit_pref: source_ddl.getUnitPrefixes()) {
        add(*unit_pref.second, source_ddl);
    }
    for (const auto& unit: source_ddl.getUnits()) {
        add(*unit.second, source_ddl);
    }
    for (const auto& data_type: source_ddl.getDataTypes()) {
        add(*data_type.second, source_ddl);
    }
    for (const auto& enum_type: source_ddl.getEnumTypes()) {
        add(*enum_type.second, source_ddl);
    }
    for (const auto& struct_type: source_ddl.getStructTypes()) {
        add(*struct_type.second, source_ddl);
    }
    for (const auto& stream_meta_type: source_ddl.getStreamMetaTypes()) {
        add(*stream_meta_type.second, source_ddl);
    }
    for (const auto& stream: source_ddl.getStreams()) {
        add(*stream.second, source_ddl);
    }
}

bool addTypeByName(const std::string& type_name,
                   const DataDefinition& source_dd,
                   DataDefinition& destination_dd)
{
    bool added = false;
    auto type_of_type = source_dd.getTypeOfType(type_name);
    switch (type_of_type) {
    case TypeOfType::invalid_type: {
        // we find out while validating but try a predefined
        auto data_type_pre = PredefinedDataTypes::getInstance().getPredefinedType(type_name);
        if (data_type_pre) {
            destination_dd.add(*data_type_pre, source_dd);
            added = true;
        }
    } break;
    case TypeOfType::data_type:
        destination_dd.add(*source_dd.getDataTypes().get(type_name), source_dd);
        added = true;
        break;
    case TypeOfType::enum_type:
        destination_dd.add(*source_dd.getEnumTypes().get(type_name), source_dd);
        added = true;
        break;
    case TypeOfType::struct_type:
        destination_dd.add(*source_dd.getStructTypes().get(type_name), source_dd);
        added = true;
        break;
    case TypeOfType::stream_meta_type:
        break;
    }
    return added;
}

bool addStreamTypeByName(const std::string& type_name,
                         const DataDefinition& source_dd,
                         DataDefinition& destination_dd)
{
    bool added = false;
    auto type_of_type = source_dd.getTypeOfType(type_name);
    switch (type_of_type) {
    case TypeOfType::invalid_type:
        // we find out while validating
        break;
    case TypeOfType::data_type:
        break;
    case TypeOfType::enum_type:
        break;
    case TypeOfType::struct_type:
        destination_dd.add(*source_dd.getStructTypes().get(type_name), source_dd);
        added = true;
        break;
    case TypeOfType::stream_meta_type:
        destination_dd.add(*source_dd.getStreamMetaTypes().get(type_name), source_dd);
        added = true;
        break;
    }
    return added;
}

bool addUnitByName(const std::string& unit_name,
                   const DataDefinition& source_dd,
                   DataDefinition& destination_dd)
{
    bool added = false;
    auto type_of_unit = source_dd.getTypeOfUnit(unit_name);
    switch (type_of_unit) {
    case TypeOfUnit::invalid_unit:
        // we find out while validating
        break;
    case TypeOfUnit::base_unit:
        destination_dd.add(*source_dd.getBaseUnits().get(unit_name), source_dd);
        added = true;
        break;
    case TypeOfUnit::unit:
        destination_dd.add(*source_dd.getUnits().get(unit_name), source_dd);
        added = true;
        break;
    }
    return added;
}

std::vector<std::string> transformValidationProblemList(
    const std::vector<ValidationInfo::Problem>& problems)
{
    return dd::transformProblemList(problems);
}

} // namespace dd
} // namespace ddl
