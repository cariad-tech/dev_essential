/**
 * @file
 * OO DataDefinition Redesign
 *
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */

#ifndef DD_DATA_MODEL_H_INCLUDED
#define DD_DATA_MODEL_H_INCLUDED

#include <ddl/datamodel/datamodel_base.h>
#include <ddl/datamodel/datamodel_header.h>
#include <ddl/datamodel/datamodel_streams.h>
#include <ddl/datamodel/datamodel_types.h>
#include <ddl/datamodel/datamodel_units.h>
#include <ddl/datamodel/infomodel_base.h>
#include <ddl/dd/dd_common_types.h>
#include <ddl/utilities/dd_access_map.h>

#include <string>
#include <vector>

namespace ddl {

namespace dd {

namespace datamodel {

/**
 * @brief DataDefinition Datamodel
 * This datamodel is observable for any change of:
 * \li the header - ModelSubject<Header>
 * \li the baseunits - ModelSubject<BaseUnit>
 * \li the prefixes - ModelSubject<UnitPrefix>
 * \li the units - ModelSubject<Unit>
 * \li the datatypes - ModelSubject<DataType>
 * \li the enums - ModelSubject<EnumType>
 * \li the structs - ModelSubject<StructType>
 * \li the streammetatypes - ModelSubject<StreamMetaType>
 * \li the Stream - ModelSubject<Stream>
 *
 * Use the @ref ModelObserver template to observe the specific change. \n Example:
 * If you observe the changes on a StructType:
 * \li see the events observable: @ref ModelEventCode
 * \li you will get events for the structs (items)
 * \li you will get events for the elements too (subitems)
 */
class DataDefinition : public ModelSubject<Header>,
                       private ModelObserver<Header>,
                       public ModelSubject<BaseUnit>,
                       public ModelSubject<UnitPrefix>,
                       public ModelSubject<Unit>,
                       public ModelSubject<DataType>,
                       public ModelSubject<EnumType>,
                       public ModelSubject<StructType>,
                       public ModelSubject<StreamMetaType>,
                       public ModelSubject<Stream>,
                       public InfoMap {
public:
    /**
     * @brief CTOR. Creates a datamodel with the current version (@ref ddl::dd::Version)
     *
     */
    DataDefinition();
    /**
     * @brief CTOR. Creates a datamodel with the given version (@ref ddl::dd::Version)
     *
     * @param ddl_version the version to use.
     */
    DataDefinition(const dd::Version& ddl_version);

    /**
     * @brief move CTOR
     *
     */
    DataDefinition(DataDefinition&&);
    /**
     * @brief move assignment operator
     *
     * @return DataDefinition&
     */
    DataDefinition& operator=(DataDefinition&&);

    /**
     * @brief CTOR. Copies the datamodel DD.
     *
     * @param other the other DD to copy.
     */
    DataDefinition(const DataDefinition& other);
    /**
     * @brief resets the current content an copies the \p other
     *
     * @param other the other to copy from
     * @return DataDefinition&
     */
    DataDefinition& operator=(const DataDefinition& other);

    virtual ~DataDefinition() = default;

public:
    /// container class for all BaseUnits
    using BaseUnits = utility::TypeAccessMap<BaseUnit, DataDefinition>;
    /// container class for all UnitPrefixes
    using UnitPrefixes = utility::TypeAccessMap<UnitPrefix, DataDefinition>;
    /// container class for all Units
    using Units = utility::TypeAccessMap<Unit, DataDefinition>;
    /// container class for all DataTypes
    using DataTypes = utility::TypeAccessMap<DataType, DataDefinition>;
    /// container class for all EnumTypes
    using EnumTypes = utility::TypeAccessMap<EnumType, DataDefinition>;
    /// container class for all StructTypes
    using StructTypes = utility::TypeAccessMap<StructType, DataDefinition>;
    /// container class for all StreamMetaTypes
    using StreamMetaTypes = utility::TypeAccessMap<StreamMetaType, DataDefinition>;
    /// container class for all Streams
    using Streams = utility::TypeAccessMap<Stream, DataDefinition>;
    /// @cond nodoc
    friend BaseUnits;
    friend UnitPrefixes;
    friend Units;
    friend DataTypes;
    friend EnumTypes;
    friend StructTypes;
    friend StreamMetaTypes;
    friend Streams;
    /// @endcond

    /**
     * @brief Set the DDL Version (in the header!)
     *
     * @param ddl_version the version to set
     * @remark This change is observable on the ModelObserver<Header>!
     */
    void setVersion(const dd::Version& ddl_version);
    /**
     * @brief Get the DDL Version
     *
     * @return Version
     */
    Version getVersion() const;

    /**
     * @brief Get the Header object
     *
     * @return std::shared_ptr<const Header>
     */
    std::shared_ptr<const Header> getHeader() const;
    /**
     * @brief Get the Header object
     *
     * @return std::shared_ptr<Header>
     */
    std::shared_ptr<Header> getHeader();
    /**
     * @brief Set the Header object
     *
     * @param header the header to set
     */
    void setHeader(const Header& header);

    /**
     * @brief Get the Base Units object
     *
     * @return const BaseUnits&
     */
    const BaseUnits& getBaseUnits() const;
    /**
     * @brief Get the Base Units object
     *
     * @return BaseUnits&
     */
    BaseUnits& getBaseUnits();

    /**
     * @brief Get the Unit Prefixes object
     *
     * @return const UnitPrefixes&
     */
    const UnitPrefixes& getUnitPrefixes() const;
    /**
     * @brief Get the Unit Prefixes object
     *
     * @return UnitPrefixes&
     */
    UnitPrefixes& getUnitPrefixes();

    /**
     * @brief Get the Units object
     *
     * @return const Units&
     */
    const Units& getUnits() const;
    /**
     * @brief Get the Units object
     *
     * @return Units&
     */
    Units& getUnits();

    /**
     * @brief Get the Data Types object
     *
     * @return const DataTypes&
     */
    const DataTypes& getDataTypes() const;
    /**
     * @brief Get the Data Types object
     *
     * @return DataTypes&
     */
    DataTypes& getDataTypes();

    /**
     * @brief Get the Enum Types object
     *
     * @return const EnumTypes&
     */
    const EnumTypes& getEnumTypes() const;
    /**
     * @brief Get the Enum Types object
     *
     * @return EnumTypes&
     */
    EnumTypes& getEnumTypes();

    /**
     * @brief Get the Struct Types object
     *
     * @return const StructTypes&
     */
    const StructTypes& getStructTypes() const;
    /**
     * @brief Get the Struct Types object
     *
     * @return StructTypes&
     */
    StructTypes& getStructTypes();

    /**
     * @brief Get the Stream Meta Types object
     *
     * @return const StreamMetaTypes&
     */
    const StreamMetaTypes& getStreamMetaTypes() const;
    /**
     * @brief Get the Stream Meta Types object
     *
     * @return StreamMetaTypes&
     */
    StreamMetaTypes& getStreamMetaTypes();

    /**
     * @brief Get the Streams object
     *
     * @return const Streams&
     */
    const Streams& getStreams() const;
    /**
     * @brief Get the Streams object
     *
     * @return Streams&
     */
    Streams& getStreams();

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
     * indicates the model as empty or not.
     * @return true/false
     */
    bool isEmpty() const;

private:
    bool validateContains(const BaseUnits::access_type& base_unit) const;
    bool validateContains(const UnitPrefixes::access_type& unit_prefix) const;
    bool validateContains(const Units::access_type& unit) const;
    bool validateContains(const DataTypes::access_type& data_type) const;
    bool validateContains(const EnumTypes::access_type& enum_type) const;
    bool validateContains(const StructTypes::access_type& struct_type) const;
    bool validateContains(const StreamMetaTypes::access_type& stream_meta_type) const;
    bool validateContains(const Streams::access_type& stream) const;

    void notifyChangedMapContent(utility::TypeAccessMapEventCode code,
                                 BaseUnits::access_type& base_unit,
                                 const std::string& additional_info);
    void notifyChangedMapContent(utility::TypeAccessMapEventCode code,
                                 UnitPrefixes::access_type& unit_prefix,
                                 const std::string& additional_info);
    void notifyChangedMapContent(utility::TypeAccessMapEventCode code,
                                 Units::access_type& unit,
                                 const std::string& additional_info);
    void notifyChangedMapContent(utility::TypeAccessMapEventCode code,
                                 DataTypes::access_type& data_type,
                                 const std::string& additional_info);
    void notifyChangedMapContent(utility::TypeAccessMapEventCode code,
                                 EnumTypes::access_type& enum_type,
                                 const std::string& additional_info);
    void notifyChangedMapContent(utility::TypeAccessMapEventCode code,
                                 StructTypes::access_type& struct_type,
                                 const std::string& additional_info);
    void notifyChangedMapContent(utility::TypeAccessMapEventCode code,
                                 StreamMetaTypes::access_type& struct_type,
                                 const std::string& additional_info);
    void notifyChangedMapContent(utility::TypeAccessMapEventCode code,
                                 Streams::access_type& stream,
                                 const std::string& additional_info);

    void modelChanged(datamodel::ModelEventCode event_code,
                      datamodel::Header& changed_subject,
                      const std::string& additional_info) override;

    std::shared_ptr<Header> _header;
    BaseUnits _base_units;
    UnitPrefixes _unit_prefixes;
    Units _units;
    DataTypes _data_types;
    EnumTypes _enum_types;
    StructTypes _struct_types;
    StreamMetaTypes _stream_meta_types;
    Streams _streams;
    bool _empty_model;
};

} // namespace datamodel
} // namespace dd
} // namespace ddl

#endif // DD_DATA_MODEL_H_INCLUDED
