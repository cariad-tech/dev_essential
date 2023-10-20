/**
 * @file
 * OO DataDefinition - Data Definition
 *
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */

#ifndef DD_DD_H_INCLUDED
#define DD_DD_H_INCLUDED

#include <ddl/datamodel/datamodel_datadefinition.h>
#include <ddl/dd/dd_common_types.h>
#include <ddl/dd/dd_infomodel_type.h>
#include <ddl/dd/dd_struct_access.h>

#include <string>

namespace ddl {
namespace dd {
/**
 * Reuse of datamodel header class - @ref ddl::dd::datamodel::Header.
 */
using Header = datamodel::Header;
/**
 * Reuse of datamodel base unit class - @ref ddl::dd::datamodel::BaseUnit.
 */
using BaseUnit = datamodel::BaseUnit;
/**
 * Reuse of datamodel unit prefix class - @ref ddl::dd::datamodel::UnitPrefix.
 */
using UnitPrefix = datamodel::UnitPrefix;
/**
 * Reuse of datamodel unit class - @ref ddl::dd::datamodel::Unit.
 */
using Unit = datamodel::Unit;
/**
 * Reuse of datamodel datatype class - @ref ddl::dd::datamodel::DataType.
 */
using DataType = datamodel::DataType;
/**
 * Reuse of datamodel enumtype class - @ref ddl::dd::datamodel::EnumType.
 */
using EnumType = datamodel::EnumType;
/**
 * Reuse of datamodel structtype class - @ref ddl::dd::datamodel::StructType.
 */
using StructType = datamodel::StructType;
/**
 * Reuse of datamodel streammetatype class - @ref ddl::dd::datamodel::StreamMetaType.
 */
using StreamMetaType = datamodel::StreamMetaType;
/**
 * Reuse of datamodel stream class - @ref ddl::dd::datamodel::Stream.
 */
using Stream = datamodel::Stream;

/**
 * @brief The Data Definiton class uses the validation model to keep
 * a Data Definition datamodel (@ref ddl::dd::datamodel::DataDefinition) valid or inform about
 * inconsitence: \n See: \li @ref validate \li @ref isValid \li @ref getValidationProtocol
 *
 * Additionally,
 * it provides access to a address, offset and type information via the typeinfo model:
 * \li calculatePositions
 * \li getStructTypeAccess
 *
 * There are predefined objects that can be used without adding it explicitly: see @ref
 * ddl::PredefinedDataTypes.
 *
 * There are predefined unit objects that can be used immediatelly: see @ref ddl::PredefinedUnits.
 */
class DataDefinition final : datamodel::ModelObserver<Header>,
                             datamodel::ModelObserver<BaseUnit>,
                             datamodel::ModelObserver<UnitPrefix>,
                             datamodel::ModelObserver<Unit>,
                             datamodel::ModelObserver<DataType>,
                             datamodel::ModelObserver<EnumType>,
                             datamodel::ModelObserver<StructType>,
                             datamodel::ModelObserver<StreamMetaType>,
                             datamodel::ModelObserver<Stream> {
public:
    /**
     * @brief Construct a new DataDefinition object with a new datamodel::DataDefinition.
     * The used language version of the new datamodel is @ref ddl::dd::Version::ddl_version_current
     */
    DataDefinition();
    /**
     * @brief Construct a new DataDefinition object with a new datamodel::DataDefinition.
     * @param ddl_version The used language version of the new datamodel.
     */
    DataDefinition(Version ddl_version);
    /**
     * @brief Construct a new DataDefinition object and \b copies the datamodel.
     *
     * @param other The other data definition to copy.
     */
    DataDefinition(const DataDefinition& other);
    /**
     * @brief Construct a new DataDefinition object and take the datamodel of the other.
     *
     * @param other The other data definition to move the model from.
     */
    DataDefinition(DataDefinition&& other);
    /**
     * @brief Assigns the DataDefinition object and \b copies the datamodel.
     *
     * @param other The other data definition to copy from.
     * @return the resulting data definition.
     */
    DataDefinition& operator=(const DataDefinition& other);
    /**
     * @brief Assigns the DataDefinition object and \b moves the datamodel.
     *
     * @param other The other data definition to move from.
     * @return the resulting data definition.
     */
    DataDefinition& operator=(DataDefinition&& other);
    /**
     * @brief Construct a new DataDefinition object and set the datamodel to the given one.
     *
     * @param datamodel the datamodel to validate and observe.
     */
    DataDefinition(const std::shared_ptr<datamodel::DataDefinition>& datamodel);
    /**
     * @brief Destroy the DataDefinition object
     *
     */
    virtual ~DataDefinition();

    /**
     * @brief Sets and references the datamodel object, that is to validate and observe.
     *
     * @param datamodel the datamodel to validate and observe.
     */
    void setModel(const std::shared_ptr<datamodel::DataDefinition>& datamodel);
    /**
     * @brief Gets the datamodel reference.
     *
     * @return std::shared_ptr<const datamodel::DataDefinition> the datamodel reference
     */
    std::shared_ptr<const datamodel::DataDefinition> getModel() const;

public:
    /**
     * @brief Reuse of datamodel base unit container - @ref
     * ddl::dd::datamodel::DataDefinition::BaseUnits.
     */
    using BaseUnits = datamodel::DataDefinition::BaseUnits;
    /**
     * @brief Reuse of datamodel unit prefixes container - @ref
     * ddl::dd::datamodel::DataDefinition::BaseUnits.
     */
    using UnitPrefixes = datamodel::DataDefinition::UnitPrefixes;
    /**
     * @brief Reuse of datamodel units container - @ref ddl::dd::datamodel::DataDefinition::Units.
     */
    using Units = datamodel::DataDefinition::Units;
    /**
     * @brief Reuse of datamodel datatypes container - @ref
     * ddl::dd::datamodel::DataDefinition::DataTypes.
     */
    using DataTypes = datamodel::DataDefinition::DataTypes;
    /**
     * @brief Reuse of datamodel enumtypes container - @ref
     * ddl::dd::datamodel::DataDefinition::EnumTypes.
     */
    using EnumTypes = datamodel::DataDefinition::EnumTypes;
    /**
     * @brief Reuse of datamodel structs container - @ref
     * ddl::dd::datamodel::DataDefinition::StructTypes.
     */
    using StructTypes = datamodel::DataDefinition::StructTypes;
    /**
     * @brief Reuse of datamodel streammetatypes container - @ref
     * ddl::dd::datamodel::DataDefinition::StreamMetaTypes.
     */
    using StreamMetaTypes = datamodel::DataDefinition::StreamMetaTypes;
    /**
     * @brief Reuse of datamodel streams container - @ref
     * ddl::dd::datamodel::DataDefinition::Streams.
     */
    using Streams = datamodel::DataDefinition::Streams;

    /**
     * @brief Set the language version of the containing datamodel.
     * @remark This will recalculate the structure sizes if necessary, because the alignment of
     * structs are version dependent.
     *
     * @param ddl_version the lanuage version to set.
     */
    void setVersion(const Version& ddl_version);
    /**
     * @brief Get the current language version set in datamodel.
     *
     * @return Version - the current language version.
     */
    Version getVersion() const;

    /**
     * @brief Resets the DataDefinition Header of the datamodel
     *
     * @param header The header to set.
     * @remark  This will recalculate the structure sizes if necessary, because the alignment of
     * structs are version dependent.
     */
    void setHeader(const Header& header);
    /**
     * @brief Gets the Header of the datamodel
     *
     * @return const Header& the header of the datamodel.
     */
    const Header& getHeader() const;
    /**
     * @brief Gets the Header of the datamodel
     *
     * @return Header& the header of the datamodel.
     */
    Header& getHeader();

    /**
     * @brief Get the Base Units of the datamodel.
     *
     * @return const BaseUnits& - base units of the datamodel.
     */
    const BaseUnits& getBaseUnits() const;
    /**
     * @brief Get the Base Units of the datamodel.
     *
     * @return BaseUnits& - base units of the datamodel.
     */
    BaseUnits& getBaseUnits();

    /**
     * @brief Get the Unit Prefixes of the datamodel.
     *
     * @return const UnitPrefixes& - prefixes of the datamodel.
     */
    const UnitPrefixes& getUnitPrefixes() const;
    /**
     * @brief Get the Unit Prefixes of the datamodel.
     *
     * @return UnitPrefixes& - the prefixes of the datamodel.
     */
    UnitPrefixes& getUnitPrefixes();

    /**
     * @brief Get the Units of the datamodel.
     *
     * @return const Units& - the units of the datamodel.
     */
    const Units& getUnits() const;
    /**
     * @brief Get the Units of the datamodel.
     *
     * @return Units& - the units of the datamodel.
     */
    Units& getUnits();

    /**
     * @brief Get the Data Types of the datamodel.
     *
     * @return const DataTypes& - the datatypes of the datamodel.
     */
    const DataTypes& getDataTypes() const;
    /**
     * @brief Get the Data Types of the datamodel.
     *
     * @return DataTypes& - the datatypes of the datamodel.
     */
    DataTypes& getDataTypes();

    /**
     * @brief Get the Enum Types of the datamodel.
     *
     * @return const EnumTypes& - the enumtypes of the datamodel.
     */
    const EnumTypes& getEnumTypes() const;
    /**
     * @brief Get the Enum Types of the datamodel.
     *
     * @return EnumTypes& - the enumtypes of the datamodel.
     */
    EnumTypes& getEnumTypes();

    /**
     * @brief Get the Struct Types of the datamodel.
     *
     * @return const StructTypes& - the structtypes of the datamodel.
     */
    const StructTypes& getStructTypes() const;
    /**
     * @brief Get the Struct Types of the datamodel.
     *
     * @return StructTypes& - the structtypes of the datamodel.
     */
    StructTypes& getStructTypes();

    /**
     * @brief Retrieve information if the datamodel contains a type with the given name.
     * \n Following types are considered: DataType, StructType, EnumType, StreamMetaType.
     *
     * @param type_name the name to check
     * @return true a type with the given name exists.
     * @return false a type with the given name does not exist.
     */
    bool containsType(const std::string& type_name) const;
    /**
     * @brief Retrieve information if the datamodel contains a type with the given name and return
     * the type of it. \n Following types are considered: DataType, StructType, EnumType,
     * StreamMetaType.
     *
     * @param type_name the name to check
     * @retval TypeOfType::invalid_type A type with the given name does not exists.
     * @retval TypeOfType::data_type A type with the given name is a DataType.
     * @retval TypeOfType::enum_type A type with the given name is a EnumType.
     * @retval TypeOfType::struct_type A type with the given name is a StructType.
     * @retval TypeOfType::streammetatype_type A type with the given name is a StreamMetaType.
     */
    TypeOfType getTypeOfType(const std::string& type_name) const;

    /**
     * @brief Retrieve information if the datamodel contains a unit with the given name.
     * \n Following units are considered: BaseUnit, Unit.
     *
     * @param unit_name the name to check
     * @return true a unit with the given name exists.
     * @return false a unit with the given name does not exist.
     */
    bool containsUnit(const std::string& unit_name) const;
    /**
     * @brief Retrieve information if the datamodel contains a unit with the given name and return
     * the type of it. \n Following units are considered: BaseUnit, Unit.
     *
     * @param unit_name the name to check
     * @retval TypeOfUnit::invalid_unit A unit with the given name does not exists.
     * @retval TypeOfUnit::unit A unit with the given name is a Unit.
     * @retval TypeOfUnit::base_unit A unit with the given name is a BaseUnit.
     */
    TypeOfUnit getTypeOfUnit(const std::string& unit_name) const;

    /**
     * @brief Get the Streammeta Types of the datamodel.
     *
     * @return StreamMetaTypes& - the streammeta types of the datamodel.
     */
    const StreamMetaTypes& getStreamMetaTypes() const;
    /**
     * @brief Get the Streammeta Types of the datamodel.
     *
     * @return const StreamMetaTypes& - the streammeta types of the datamodel.
     */
    StreamMetaTypes& getStreamMetaTypes();

    /**
     * @brief Get the Streams of the datamodel.
     *
     * @return const Streams& - the streams of the datamodel.
     */
    const Streams& getStreams() const;
    /**
     * @brief Get the Streams of the datamodel.
     *
     * @return Streams& - the streams of the datamodel.
     */
    Streams& getStreams();

    /**
     * @brief Obtains if the validation level has reached at least the given \p level.
     * @see @ref ValidationServiceInfo, @ref ValidationInfo.
     *
     * @param level the level to check
     * @return true the level is at least \p level
     * @return false the level is below \p level
     */
    bool isValid(ValidationLevel level = ValidationLevel::valid) const;
    /**
     * @brief Calculate the validation level of all DataDefinition objects that does not have a
     * validation level yet or is below ValidationLevel::valid
     * @see @ref ValidationServiceInfo, @ref ValidationInfo.
     *
     * @param force_revalidation if a validation level was already calculated to
     * ValidationLevel::valid this will force a recalculation!
     */
    void validate(bool force_revalidation = false);
    /**
     * @brief Gets a collection of all problems obtained while validating the DataDefinition
     * Objects-
     * @see @ref ValidationServiceInfo, @ref ValidationInfo.
     *
     * @return std::vector<ValidationInfo::Problem> the collection of problems
     * @remarks to print or flatten the protocol consider @ref
     * ddl::dd::transformValidationProblemList.
     */
    std::vector<ddl::dd::Problem> getValidationProtocol() const;

    /**
     * @brief calculates the element sizes and positions of all structs, enumtypes and datatypes or
     * dedicated for the one given.
     * @see @ref TypeInfo, @ref ElementTypeInfo.
     *
     * @param type_name the type name to calculate the size and type info for. if empty recalculate
     * all, if necessary.
     * @param type_of_type if type name is empty, this parameter can choose the types to
     * recalculate.
     * @param force_recalculation if set to true this will force the recalculation.
     */
    void calculatePositions(const std::string& type_name = {},
                            TypeOfType type_of_type = TypeOfType::invalid_type,
                            bool force_recalculation = false);
    /**
     * @brief Get the Struct Type Access, where to enter the type and calculated element position
     * information.
     * @see @ref TypeInfo, @ref ElementTypeInfo, @ref StructTypeAccess, @ref StructElementAccess.
     *
     * @param type_name the struct type name to retrieve the structtype access for.
     * @return const StructTypeAccess
     */
    StructTypeAccess getStructTypeAccess(const std::string& type_name) const;

    // merge/add
    /**
     * @brief Merges or add the given base unit. If there are dependencies, they will be retrieved
     * from \p source_dd. If there does already exist one base unit with the same name, the base
     * unit must be equal (@ref ddl::dd::datamodel::BaseUnit).
     *
     * @param base_unit the base unit to add.
     * @param source_dd dependencies will be obtained from this DataDefinition reference.
     * @throws ddl::dd::Error throws if base unit already exists and is not equal to the given one.
     */
    void add(const BaseUnit& base_unit, const DataDefinition& source_dd);
    /**
     * @brief Merges or add the given unit prefix. If there are dependencies, they will be retrieved
     * from \p source_dd. If there does already exist one unit prefix with the same name, the prefix
     * must be equal (@ref ddl::dd::datamodel::UnitPrefix).
     *
     * @param unit_prefix the unit prefix to add.
     * @param source_dd dependencies will be obtained from this DataDefinition reference.
     * @throws ddl::dd::Error throws if a unit prefix already exists and is not equal to the given
     * one.
     */
    void add(const UnitPrefix& unit_prefix, const DataDefinition& source_dd);
    /**
     * @brief Merges or add the given unit. If there are dependencies, they will be retrieved from
     * \p source_dd. If there does already exist one unit with the same name, the unit must be equal
     * (@ref ddl::dd::datamodel::Unit).
     *
     * @param unit the unit to add.
     * @param source_dd dependencies will be obtained from this DataDefinition reference (base units
     * and unit prefixes)
     * @throws ddl::dd::Error throws if a unit already exists and is not equal to the given one.
     * Will also check dependencies.
     */
    void add(const Unit& unit, const DataDefinition& source_dd);
    /**
     * @brief Merges or add the given datatype. If there are dependencies, they will be retrieved
     * from \p source_dd. If there does already exist one datatype with the same name, the datatype
     * must be equal (@ref ddl::dd::datamodel::DataType).
     *
     * @param data_type the data_type to add.
     * @param source_dd dependencies will be obtained from this DataDefinition reference (base units
     * and units)
     * @throws ddl::dd::Error throws if a datatype already exists and is not equal to the given one.
     * Will also check dependencies.
     */
    void add(const DataType& data_type, const DataDefinition& source_dd);
    /**
     * @brief Merges or add the given enumtype. If there are dependencies, they will be retrieved
     * from \p source_dd. If there does already exist one enumtype with the same name, the enumtype
     * must be equal (@ref ddl::dd::datamodel::EnumType).
     *
     * @param enum_type the enumtype to add.
     * @param source_dd dependencies will be obtained from this DataDefinition reference (datatype)
     * @throws ddl::dd::Error throws if a datatype already exists and is not equal to the given one.
     * Will also check dependencies.
     */
    void add(const EnumType& enum_type, const DataDefinition& source_dd);
    /**
     * @brief Merges or add the given structtype. If there are dependencies, they will be retrieved
     * from \p source_dd. If there does already exist one structtype with the same name, the
     * structtype must be equal (@ref ddl::dd::datamodel::StructType).
     *
     * @param struct_type the structtype to add.
     * @param source_dd dependencies will be obtained from this DataDefinition reference (datatype,
     * other structtype, enumtype, units)
     * @throws ddl::dd::Error throws if a structype already exists and is not equal to the given
     * one. Will also check dependencies.
     */
    void add(const StructType& struct_type, const DataDefinition& source_dd);
    /**
     * @brief Merges or add the given streammetatype. If there are dependencies, they will be
     * retrieved from \p source_dd. If there does already exist one streammetatype with the same
     * name, the streammetatype must be equal (@ref ddl::dd::datamodel::StreamMetaType).
     *
     * @param stream_meta_type the streammetatype to add.
     * @param source_dd dependencies will be obtained from this DataDefinition reference (other
     * streammetatype)
     * @throws ddl::dd::Error throws if a streammetatype already exists and is not equal to the
     * given one. Will also check dependencies.
     */
    void add(const StreamMetaType& stream_meta_type, const DataDefinition& source_dd);
    /**
     * @brief Merges or add the given stream. If there are dependencies, they will be retrieved from
     * \p source_dd. If there does already exist one stream with the same name, the stream must be
     * equal (@ref ddl::dd::datamodel::Stream).
     *
     * @param stream the stream to add.
     * @param source_dd dependencies will be obtained from this DataDefinition reference
     * (streammetatype, structtype)
     * @throws ddl::dd::Error throws if a Stream already exists and is not equal to the given one.
     * Will also check dependencies.
     */
    void add(const Stream& stream, const DataDefinition& source_dd);
    /**
     * @brief Merges or add the DataDefinition objects of the given \p source_dd.
     *
     * @param source_dd the data definition to add.
     * @throws ddl::dd::Error throws if a DataDefinition object to add already exists and it is not
     * equal to the given one.
     * @see all other \c add methods.
     */
    void add(const DataDefinition& source_dd);

private:
    std::shared_ptr<datamodel::DataDefinition> _datamodel;
    void attachToModel();
    void detachFromModel();
    void modelChanged(datamodel::ModelEventCode event_code,
                      datamodel::Header& changed_subject,
                      const std::string& additional_info) override;
    void modelChanged(datamodel::ModelEventCode event_code,
                      datamodel::BaseUnit& changed_subject,
                      const std::string& additional_info) override;
    void modelChanged(datamodel::ModelEventCode event_code,
                      datamodel::UnitPrefix& changed_subject,
                      const std::string& additional_info) override;
    void modelChanged(datamodel::ModelEventCode event_code,
                      datamodel::Unit& changed_subject,
                      const std::string& additional_info) override;
    void modelChanged(datamodel::ModelEventCode event_code,
                      datamodel::DataType& changed_subject,
                      const std::string& additional_info) override;
    void modelChanged(datamodel::ModelEventCode event_code,
                      datamodel::EnumType& changed_subject,
                      const std::string& additional_info) override;
    void modelChanged(datamodel::ModelEventCode event_code,
                      datamodel::StructType& changed_subject,
                      const std::string& additional_info) override;
    void modelChanged(datamodel::ModelEventCode event_code,
                      datamodel::StreamMetaType& changed_subject,
                      const std::string& additional_info) override;
    void modelChanged(datamodel::ModelEventCode event_code,
                      datamodel::Stream& changed_subject,
                      const std::string& additional_info) override;

    Version _last_known_ddl_version;
    // this member variable must stay there for ABI compatibility!
    // NOLINTNEXTLINE(clang-diagnostic-unused-private-field)
    ValidationLevel _dummy_validation_level = ValidationLevel::dont_know;
    std::unordered_map<std::string, std::string> _recursion_detection_stream_meta_types;
    std::unordered_map<std::string, std::string> _recursion_detection_struct;
};

/**
 * @brief Helper function which merges or adds the type with the name of \p type_name. If there are
 * dependencies, they will be retrieved from \p source_dd. \n following types are considered: \li
 * DataType \li EnumType \li StructType
 *
 * @remark This functionality was designed to support adding types that are part of the elements
 * within a StructType.
 *
 * @param type_name the typename of the type to add.
 * @param source_dd dependencies will be obtained from this DataDefinition reference (other
 *                  streammetatype)
 * @param destination_dd where to add or merge
 * @throws ddl::dd::Error throws if the type already exists and is not equal to the given one.
 * Will also check dependencies.
 * @retval true a type was found and added
 * @retval false a type was NOT found and so NOT added
 */
bool addTypeByName(const std::string& type_name,
                   const DataDefinition& source_dd,
                   DataDefinition& destination_dd);
/**
 * @brief Merges or add the streamtype with the name of \p stream_type_name. If there are
 * dependencies, they will be retrieved from \p source_dd. \n following types are considered:
 * \li StructType
 * \li StreamMetaType
 *
 * @remark This functionality was designed to support adding types that are part of the streams
 * (Stream).
 *
 * @param stream_type_name the typename of the type to add.
 * @param source_dd dependencies will be obtained from this DataDefinition reference
 *                  (streammetatype, structtype, etc.)
 * @param destination_dd where to add or merge
 * @throws ddl::dd::Error throws if the type already exists and is not equal to the given one.
 * Will also check dependencies.
 * @retval true a type was found and added
 * @retval false a type was NOT found and so NOT added
 */
bool addStreamTypeByName(const std::string& stream_type_name,
                         const DataDefinition& source_dd,
                         DataDefinition& destination_dd);
/**
 * @brief Merges or add the unit with the name of \p unit_name. If there are dependencies, they
 * will be retrieved from \p source_dd. \n following types are considered: \li BaseUnit \li Unit
 *
 * @param unit_name the unitname of the unit to add.
 * @param source_dd dependencies will be obtained from this DataDefinition reference (baseunit,
 *                  unitprefix)
 * @param destination_dd where to add or merge
 * @throws ddl::dd::Error throws if the unit already exists and is not equal to the given one.
 * Will also check dependencies.
 * @retval true a unit was found and added
 * @retval false a unit was NOT found and so NOT added
 */
bool addUnitByName(const std::string& unit_name,
                   const DataDefinition& source_dd,
                   DataDefinition& destination_dd);

/**
 * @brief transforms the validation protocol of problems to a simple vrctor of strings.
 * \n Simple usage:
 * @code
 * //...
 *
 * using namespace ddl;
 * dd::DataDefinition my_dd;
 * //... adding ... loading ... using
 * std::cout <<
 * a_util::strings::join(dd::transformValidationProblemList(my_dd.getValidationProtocol()),"\n");
 *
 * @endcode
 *
 * @param problems the problems to tranform.
 * @return std::vector<std::string>
 */
std::vector<std::string> transformValidationProblemList(
    const std::vector<ddl::dd::Problem>& problems);

/**
 * @brief Alias Name for DataDefinition, formally known as DataDescription
 *
 */
using DataDescription = dd::DataDefinition;

} // namespace dd
using dd::DataDefinition;
using dd::DataDescription;
} // namespace ddl

#endif // DD_DD_H_INCLUDED