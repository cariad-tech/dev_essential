/**
 * @file
 * OO DataDefinition Validation Info Model
 *
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

    This Source Code Form is subject to the terms of the Mozilla
    Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

If it is not possible or desirable to put the notice in a particular file, then
You may include the notice in a location (such as a LICENSE file in a
relevant directory) where a recipient would be likely to look for such a notice.

You may add additional accurate notices of copyright ownership.
@endverbatim
 */
#ifndef DD_VALIDATIONINFOMODEL_H_INCLUDED
#define DD_VALIDATIONINFOMODEL_H_INCLUDED

#include "ddl/datamodel/datamodel_datadefinition.h"
#include "ddl/dd/dd_error.h"
#include "ddl/dd/dd_infomodel_type.h"

#include <memory>
#include <unordered_map>

namespace ddl {

namespace dd {

/**
 * @brief Validation Service Info used only as one global instance at the dd::DataDefinition.
 * It keeps the model consistent while renaming types, unit, enum values, streammetatypes while
 * using a loose coupling.
 */
class ValidationServiceInfo : public datamodel::Info<ValidationServiceInfo> {
public:
    /// definiton of info type to use the @ref datamodel::Info
    static constexpr const uint8_t INFO_TYPE_ID = dd::InfoType::validation_service_info;

    /**
     * @brief dependency type for loose coupling
     *
     */
    enum DependencyType : uint8_t {
        /// dependency unit depends on a unit prefix
        unit_to_unit_prefix,
        /// dependency unit depends on a base unit
        unit_to_base_unit,
        /// dependency data type depends on a unit
        data_type_to_unit,
        /// dependency data type depends on a base unit
        data_type_to_base_unit,
        /// dependency enum type depends on a data type
        enum_type_to_data_type,
        /// dependency struct type depends on a data type
        struct_type_to_data_type,
        /// dependency struct type depends on a enum type
        struct_type_to_enum_type,
        /// dependency struct type depends on a struct type
        struct_type_to_struct_type,
        /// dependency struct type depends on a unit, if one element belongs to it
        struct_type_to_unit,
        /// dependency struct type depends on a base unit, if one element belongs to it
        struct_type_to_base_unit,
        /// dependency stream_meta_type depends on a stream_meta_type
        stream_meta_type_to_stream_meta_type,
        /// dependency stream depends on a stream_meta_type
        stream_to_stream_meta_type,
        /// dependency stream depends on a struct_type
        stream_to_struct_type
    };

    /**
     * @brief dependency
     *
     */
    struct Dependency {
    public:
        /**
         * @brief no default CTOR
         *
         */
        Dependency() = delete;
        /**
         * @brief CTOR
         *
         * @param type_of_dependency type of depency
         * @param from item name that depends on the \p to
         * @param to item name of the item depending to.
         */
        Dependency(DependencyType type_of_dependency,
                   const std::string& from,
                   const std::string& to);
        /**
         * @brief DTOR
         *
         */
        ~Dependency() = default;

        /**
         * @brief Dependency type
         *
         */
        DependencyType _type_of_dependency;
        /**
         * @brief the "from" item name. Item name that depends on the \p _to
         *
         */
        std::string _from;
        /**
         * @brief the "to" item name. Item name of the item depending to.
         *
         */
        std::string _to;
    };

    /**
     * @brief add the dependency to the depemndency model
     *
     * @param dependency to add.
     */
    void addDependency(const Dependency& dependency);
    /**
     * @brief A base unit was removed.
     *
     * @param base_unit the base unit that is removed
     * @param parent_dd the parent DD
     */
    void removed(const datamodel::BaseUnit& base_unit, datamodel::DataDefinition& parent_dd);
    /**
     * @brief A unit prefix was removed.
     *
     * @param unit_prefix the unit prefix that is removed
     * @param parent_dd the parent DD
     */
    void removed(const datamodel::UnitPrefix& unit_prefix, datamodel::DataDefinition& parent_dd);
    /**
     * @brief A unit was removed.
     * This will invalidate all dependent struct_types and data_types using the unit!
     *
     * @param unit the unit that is removed
     * @param parent_dd the parent DD
     */
    void removed(const datamodel::Unit& unit, datamodel::DataDefinition& parent_dd);
    /**
     * @brief A data_type was removed.
     * This will invalidate all dependent structs using the data_type!
     *
     * @param data_type the data_type that is removed
     * @param parent_dd the parent DD
     */
    void removed(const datamodel::DataType& data_type, datamodel::DataDefinition& parent_dd);
    /**
     * @brief A enum_type was removed.
     * This will invalidate all dependent structs using the enum_type!
     *
     * @param enum_type the enum_type that is removed
     * @param parent_dd the parent DD
     */
    void removed(const datamodel::EnumType& enum_type, datamodel::DataDefinition& parent_dd);
    /**
     * @brief A struct_type was removed.
     * This will invalidate all dependent structs using the struct_type!
     *
     * @param struct_type the struct_type that is removed
     * @param parent_dd the parent DD
     */
    void removed(const datamodel::StructType& struct_type, datamodel::DataDefinition& parent_dd);
    /**
     * @brief A stream_meta_type was removed.
     * This will invalidate all dependent streams and other stream_meta_type using the
     * stream_meta_type!
     *
     * @param stream_meta_type the stream_meta_type that is removed
     * @param parent_dd the parent DD
     */
    void removed(const datamodel::StreamMetaType& stream_meta_type,
                 datamodel::DataDefinition& parent_dd);
    /**
     * @brief A stream was removed.
     *
     * @param stream the stream that is removed
     * @param parent_dd the parent DD
     */
    void removed(const datamodel::Stream& stream, datamodel::DataDefinition& parent_dd);

    /**
     * @brief Update all dependency to the renamed base_unit.
     *
     * @param base_unit the base_unit renamed
     * @param old_name the old name of the base_unit
     * @param parent_dd the parent DD
     */
    void renamed(const datamodel::BaseUnit& base_unit,
                 const std::string& old_name,
                 datamodel::DataDefinition& parent_dd);
    /**
     * @brief Update all dependency to the renamed unit_prefix.
     *
     * @param unit_prefix the unit_prefix renamed
     * @param old_name the old name of the unit_prefix
     * @param parent_dd the parent DD
     */
    void renamed(const datamodel::UnitPrefix& unit_prefix,
                 const std::string& old_name,
                 datamodel::DataDefinition& parent_dd);
    /**
     * @brief Update all dependency to the renamed unit.
     *
     * @param unit the unit renamed
     * @param old_name the old name of the unit
     * @param parent_dd the parent DD
     */
    void renamed(const datamodel::Unit& unit,
                 const std::string& old_name,
                 datamodel::DataDefinition& parent_dd);
    /**
     * @brief Update all dependency to the renamed data_type.
     *
     * @param data_type the data_type renamed
     * @param old_name the old name of the data_type
     * @param parent_dd the parent DD
     */
    void renamed(const datamodel::DataType& data_type,
                 const std::string& old_name,
                 datamodel::DataDefinition& parent_dd);
    /**
     * @brief Update all dependency to the renamed enum_type.
     *
     * @param enum_type the enum_type renamed
     * @param old_name the old name of the enum_type
     * @param parent_dd the parent DD
     */
    void renamed(const datamodel::EnumType& enum_type,
                 const std::string& old_name,
                 datamodel::DataDefinition& parent_dd);
    /**
     * @brief Update all dependency to the renamed struct_type.
     *
     * @param struct_type the struct_type renamed
     * @param old_name the old name of the struct_type
     * @param parent_dd the parent DD
     */
    void renamed(const datamodel::StructType& struct_type,
                 const std::string& old_name,
                 datamodel::DataDefinition& parent_dd);
    /**
     * @brief Update all dependency to the renamed base_stream_meta_typeunit.
     *
     * @param stream_meta_type the stream_meta_type renamed
     * @param old_name the old name of the stream_meta_type
     * @param parent_dd the parent DD
     */
    void renamed(const datamodel::StreamMetaType& stream_meta_type,
                 const std::string& old_name,
                 datamodel::DataDefinition& parent_dd);
    /**
     * @brief Update all dependency to the renamed stream.
     *
     * @param stream the stream renamed
     * @param old_name the old name of the stream
     * @param parent_dd the parent DD
     */
    void renamed(const datamodel::Stream& stream,
                 const std::string& old_name,
                 datamodel::DataDefinition& parent_dd);

    /**
     * @brief this will internally prevent multiple revalidations
     *
     * @return true a validation is needed
     * @return false a validation is not needed
     */
    bool validationNeeded() const;
    /**
     * @brief  dependencies are stored in ToFrom maps (not in FromTo maps) to raise performance.
     *
     */
    typedef std::unordered_map<std::string, std::vector<std::string>> ToFromMap;

    /**
     * @brief Return structure for @ref forceRevalidationOfTypeDependencies call.
     *        Contains the type names for types that have been invalidated.
     */
    struct InvalidatedTypes {
        /**
         * Collection of enum type names that are invalidated.
         */
        std::vector<std::string> _enum_type_names;
        /**
         * Collection of struct type names that are invalidated.
         */
        std::vector<std::string> _struct_type_names;
    };
    /**
     * Collect all dependent types to the given one and sets them invalid
     * @param type_name the type_name looking for dependencies
     * @param type the type of the given \p type_name
     * @param parent_dd the dd where to set forceRevalidation
     * @return return all typenames depending on the given type
     */
    InvalidatedTypes forceRevalidationOfTypeDependencies(
        const std::string& type_name,
        ddl::dd::TypeOfType type,
        datamodel::DataDefinition& parent_dd) const;

private:
    /**
     * @brief dependencies are stored in ToFrom maps (not in FromTo maps)
     */

#if defined(__GNUC__) && ((__GNUC__ == 5) && (__GNUC_MINOR__ == 2))
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
#endif // defined(__GNUC__) && ((__GNUC__ == 5) && (__GNUC_MINOR__ == 2))

    std::unordered_map<uint8_t, ToFromMap> _dependencies;

#if defined(__GNUC__) && ((__GNUC__ == 5) && (__GNUC_MINOR__ == 2))
#pragma GCC diagnostic pop
#endif // defined(__GNUC__) && ((__GNUC__ == 5) && (__GNUC_MINOR__ == 2))

    bool _validation_needed = true;
};

/**
 * Validation Info model will check for validation.
 * Sometimes the TypeInfo must be already set before this validation can be performed.
 */
class ValidationInfo : public datamodel::Info<ValidationInfo> {
public:
    /// the type info id to use the @ref datamodel::Info template.
    static constexpr const uint8_t INFO_TYPE_ID = dd::InfoType::validation_info;

    /**
     * @brief default CTOR
     *
     */
    ValidationInfo() = default;
    /**
     * @brief CTOR and immediatelly update a Validation Info for units
     *
     * @param unit the unit to update the validation info
     * @param parent_dd the parent DD to retrieve dependencies
     */
    ValidationInfo(datamodel::Unit& unit, datamodel::DataDefinition& parent_dd);
    /**
     * @brief CTOR and immediatelly update a Validation Info for data_types
     *
     * @param data_type the data_type to update the validation info
     * @param parent_dd the parent DD to retrieve dependencies
     */
    ValidationInfo(datamodel::DataType& data_type, datamodel::DataDefinition& parent_dd);
    /**
     * @brief CTOR and immediatelly update a Validation Info for enum_type
     *
     * @param enum_type the enum_type to update the validation info
     * @param parent_dd the parent DD to retrieve dependencies
     */
    ValidationInfo(datamodel::EnumType& enum_type, datamodel::DataDefinition& parent_dd);
    /**
     * @brief CTOR and immediatelly update a Validation Info for streams
     *
     * @param stream the stream to update the validation info
     * @param parent_dd the parent DD to retrieve dependencies
     */
    ValidationInfo(datamodel::Stream& stream, datamodel::DataDefinition& parent_dd);
    /**
     * @brief DTOR
     *
     */
    virtual ~ValidationInfo();

    /**
     * @brief Validation level
     *
     */
    enum ValidationLevel : uint8_t {
        /**
         * @brief The validation is not known. This means it is not calculated yet. Use i.e. @ref
         * ddl::dd::DataDefinition::validate
         *
         */
        dont_know,
        /**
         * @brief the validation level is invalid, dependencies can not be obtained or the
         * dependency "to" is invalid (struct to struct i.e.).
         *
         */
        invalid,
        /**
         * @brief Structs and elements can be "good_enough" for the TypeInfo.
         * This means the Coder and Decoder can calculate the sizes and the all access Functions can
         * obtain values. Usually this should be the valid state. The problem is, that there are
         * DataDefinition out there (in the users world) using i.e. units, but does not define the
         * unit within that DD. At this point we do not want to break that code! So good enough
         * means: Not well defined in some cases, but good enough for calculate byte positions and
         * @ref TypeInfo.
         *
         * Only structs, enums and datatypes may have a validationlevel of "good_enough" if there
         * are any other DD Objects used in a @ref DataDefinition and they are "invalid", the DD
         * will also set to "invalid"!
         */
        good_enough,
        /**
         * @brief every thing is well defined.
         */
        valid
    };

    /**
     * @brief Validation Problem to report
     *
     */
    using Problem = ddl::dd::Problem;

    /**
     * @brief checks if the validation level is reached
     *
     * @param level can only be "good_enough" or "valid"
     * @return true the given \p level is reached
     * @return false the given \p level is NOT reached
     */
    bool isValid(ValidationLevel level = valid) const;

    /**
     * @brief sets the Info to invalid to force revalidation
     *
     */
    void forceRevalidation();
    /**
     * @brief Get the Validation Level
     *
     * @return ValidationLevel
     */
    ValidationLevel getValidationLevel() const;

    /**
     * @brief Get the Validation Problems (only set if below "valid")
     *
     * @return const std::vector<Problem>&
     */
    const std::vector<Problem>& getProblems() const;
    /**
     * @brief adds a problem
     *
     * @param problem the problem to add
     */
    void addProblem(Problem&& problem);
    /**
     * @brief adds a problem
     *
     * @param problem the propblem to add
     */
    void addProblem(const Problem& problem);

    /**
     * @brief Update and revalidate the unit.
     *
     * @param unit the unit to validate
     * @param parent_dd the parent DD
     */
    void update(datamodel::Unit& unit, datamodel::DataDefinition& parent_dd);
    /**
     * @brief Update and revalidate the data_type.
     *
     * @param data_type the unit to validate
     * @param parent_dd the parent DD
     */
    void update(datamodel::DataType& data_type, datamodel::DataDefinition& parent_dd);
    /**
     * @brief Update and revalidate the enum_type.
     *
     * @param enum_type the unit to validate
     * @param parent_dd the parent DD
     */
    void update(datamodel::EnumType& enum_type, datamodel::DataDefinition& parent_dd);
    /**
     * @brief Update and revalidate the struct_type.
     *
     * @param struct_type the struct_type to validate
     * @param parent_dd the parent DD
     */
    void update(datamodel::StructType& struct_type, datamodel::DataDefinition& parent_dd);
    /**
     * @brief Update and revalidate the stream_meta_type.
     *
     * @param stream_meta_type the stream_meta_type to validate
     * @param parent_dd the parent DD
     */
    void update(datamodel::StreamMetaType& stream_meta_type, datamodel::DataDefinition& parent_dd);
    /**
     * @brief Update and revalidate the stream.
     *
     * @param stream the stream to validate
     * @param parent_dd the parent DD
     */
    void update(datamodel::Stream& stream, datamodel::DataDefinition& parent_dd);

private:
    ValidationLevel _valid = ValidationLevel::invalid;
    bool _currently_on_validation = false; // to prevent recursive validation calls if somebody
                                           // defines this type as a part of the type
    std::vector<Problem> _validation_problems;
};

} // namespace dd
} // namespace ddl

#endif // DD_VALIDATIONINFOMODEL_H_INCLUDED
