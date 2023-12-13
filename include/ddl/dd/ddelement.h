/**
 * @file
 * OO DataDefinition Element Header
 *
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */

#ifndef DDELEM_H_INCLUDED
#define DDELEM_H_INCLUDED

#include <ddl/dd/dd.h>
#include <ddl/dd/dddatatype.h>
#include <ddl/dd/ddenum.h>
#include <ddl/dd/ddstructure.h>

#include <string>

namespace ddl {

///@cond nodoc
class DDStructure;
///@endcond

/**
 * @brief Utility class for a complete valid data definition of one StructType::Element and its
 * dependencies. This class is for convenience to use it together with ddl::DDStructure .
 * @see ddl::DDStructure.
 */
class DDElement {
public:
    /**
     * @brief Deserialized definiton for one element.
     */
    using Deserialized = dd::datamodel::StructType::DeserializedInfo;
    /**
     * @brief Serialized definiton for one element.
     */
    using Serialized = dd::datamodel::StructType::SerializedInfo;

    /**
     * @brief Construct a new DDElement object
     *
     */
    DDElement() = default;
    /**
     * @brief Construct a new DDElement object
     *
     */
    DDElement(DDElement&&) = default;
    /**
     * @brief Construct a new DDElement object
     *
     */
    DDElement(const DDElement&) = default;
    /**
     * @brief assignment move operator
     *
     * @return DDElement& reference
     */
    DDElement& operator=(DDElement&&) = default;
    /**
     * @brief assignment copy operator
     *
     * @return DDElement& reference
     */
    DDElement& operator=(const DDElement&) = default;
    /**
     * @brief Destroy the DDElement object
     *
     */
    ~DDElement() = default;

    /**
     * @brief Construct a new DDElement object that uses a POD Type (dd::DataType)
     * Use it like:
     * @code
     * ddl::DDStructure dd_struct("StructType");
     * dd_struct.addElement(ddl::DDElement("element_name", ddl::DataType<uint32_t>()));
     * @endcode
     *
     * @param name Name of the element.
     * @param data_type Data Type of the element. Use @ref dd::DataType template.
     * @param deserialized_info deserialized information (will be set by default to the alignment of
     * data_type).
     * @param serialized_info serialized information of this element (will be set automatically to
     * the next valid bytepos depending on previous elements).
     * @param array_size array size of the element
     * @param description description of the element
     * @param comment comment of the element
     * @param base_unit base_unit of the element to use
     * @param minimum_value minimum value of the element
     * @param maximum_value maximum value of the element
     * @param default_value default value (mind! should fit to the data_type)
     * @param scale scale of the element
     * @param offset offset of the value
     */
    DDElement(const std::string& name,
              const dd::DataType& data_type,
              const Deserialized& deserialized_info = {},
              const Serialized& serialized_info = {},
              const dd::ArraySize& array_size = 1,
              const std::string& description = {},
              const std::string& comment = {},
              const dd::BaseUnit& base_unit = {},
              const std::string& minimum_value = {},
              const std::string& maximum_value = {},
              const std::string& default_value = {},
              const std::string& scale = {},
              const std::string& offset = {});

    /**
     * @brief Construct a new DDElement object that uses a POD Type (dd::DataType)
     * Use it like:
     * @code
     * ddl::DDUnit km("Kilometre", "1", "1", "0",
     *                { ddl::DDRefUnit(ddl::BaseUnit<ddl::unit::Metre>(),
     *                  3, ddl::UnitPrefix<ddl::unit_prefix::kilo>()) });
     * ddl::DDStructure dd_struct("StructType");
     * dd_struct.addElement(ddl::DDElement("element_name", ddl::DataType<uint32_t>(), {}, {}, 1, {},
     * {}, km));
     * @endcode
     *
     * @param name Name of the element.
     * @param data_type Data Type of the element. Use @ref ddl::DataType template.
     * @param deserialized_info deserialized information (will be set by default to the alignment of
     * data_type).
     * @param serialized_info serialized information of this element (will be set automatically to
     * the next valid bytepos depending on previous elements).
     * @param array_size array size of the element
     * @param description description of the element
     * @param comment comment of the element
     * @param unit unit of the element
     * @param minimum_value minimum value of the element
     * @param maximum_value maximum value of the element
     * @param default_value default value (mind! should fit to the data_type)
     * @param scale scale of the element
     * @param offset offset of the value
     */
    DDElement(const std::string& name,
              const dd::DataType& data_type,
              const Deserialized& deserialized_info,
              const Serialized& serialized_info,
              const dd::ArraySize& array_size,
              const std::string& description,
              const std::string& comment,
              const DDUnit& unit,
              const std::string& minimum_value = {},
              const std::string& maximum_value = {},
              const std::string& default_value = {},
              const std::string& scale = {},
              const std::string& offset = {});

    /**
     * @brief Construct a new DDElement object that uses a POD Type (dd::DataType)
     * Use it like:
     * @code
     * ddl::DDDataType custom_type("custom", 1);
     * ddl::DDStructure dd_struct("StructType");
     * dd_struct.addElement(ddl::DDElement("element_name", custom_type));
     * @endcode
     *
     * @param name Name of the element.
     * @param data_type Data Type of the element. Use @ref ddl::DataType template.
     * @param deserialized_info deserialized information (will be set by default to the alignment of
     * data_type).
     * @param serialized_info serialized information of this element (will be set automatically to
     * the next valid bytepos depending on previous elements).
     * @param array_size array size of the element
     * @param description description of the element
     * @param comment comment of the element
     * @param base_unit the base unit name of the element
     * @param minimum_value minimum value of the element
     * @param maximum_value maximum value of the element
     * @param default_value default value (mind! should fit to the data_type)
     * @param scale scale of the element
     * @param offset offset of the value
     */
    DDElement(const std::string& name,
              const DDDataType& data_type,
              const Deserialized& deserialized_info = {},
              const Serialized& serialized_info = {},
              const dd::ArraySize& array_size = 1,
              const std::string& description = {},
              const std::string& comment = {},
              const dd::BaseUnit& base_unit = {},
              const std::string& minimum_value = {},
              const std::string& maximum_value = {},
              const std::string& default_value = {},
              const std::string& scale = {},
              const std::string& offset = {});

    /**
     * @brief Construct a new DDElement object that uses a POD Type (dd::DataType)
     * Use it like:
     * @code
     * ddl::DDDataType custom_type("custom", 1);
     * ddl::DDUnit km("Kilometre", "1", "1", "0",
     *                { ddl::DDRefUnit(ddl::BaseUnit<ddl::unit::Metre>(),
     *                  3, ddl::UnitPrefix<ddl::unit_prefix::kilo>()) });
     * ddl::DDStructure dd_struct("StructType");
     * dd_struct.addElement(ddl::DDElement("element_name", custom_type, {}, {}, 1, {}, {}, km));
     * @endcode
     *
     * @param name Name of the element.
     * @param data_type Data Type of the element. Use @ref ddl::DataType template.
     * @param deserialized_info deserialized information (will be set by default to the alignment of
     * data_type).
     * @param serialized_info serialized information of this element (will be set automatically to
     * the next valid bytepos depending on previous elements).
     * @param array_size array size of the element
     * @param description description of the element
     * @param comment comment of the element
     * @param unit the unit of the element
     * @param minimum_value minimum value of the element
     * @param maximum_value maximum value of the element
     * @param default_value default value (mind! should fit to the data_type)
     * @param scale scale of the element
     * @param offset offset of the value
     */
    DDElement(const std::string& name,
              const DDDataType& data_type,
              const Deserialized& deserialized_info,
              const Serialized& serialized_info,
              const dd::ArraySize& array_size,
              const std::string& description,
              const std::string& comment,
              const DDUnit& unit,
              const std::string& minimum_value = {},
              const std::string& maximum_value = {},
              const std::string& default_value = {},
              const std::string& scale = {},
              const std::string& offset = {});

    /**
     * @brief Construct a new DDElement object that uses a Enum Type (dd::EnumType)
     * Use it like:
     * @code
     * ddl::DDEnum dd_enum("EnumType",
     *                     ddl::DataType<uint32_t>(),
     *                     { {"val1", "1" }, { "val2", "2"} });
     * ddl::DDStructure dd_struct("StructType");
     * dd_struct.addElement(ddl::DDElement("element_enum", dd_enum));
     * @endcode
     *
     * @param name Name of the element.
     * @param enum_type Enum Type of the element. Use @ref ddl::DDEnum class.
     * @param deserialized_info deserialized information (will be set by default to the alignment of
     * data_type).
     * @param serialized_info serialized information of this element (will be set automatically to
     * the next valid bytepos depending on previous elements).
     * @param array_size array size of the element
     * @param description description of the element
     * @param comment comment of the element
     * @param base_unit the base unit of the element
     * @param value constant value name of the element (mind! this constant should exists in given
     * enum_type!)
     * @param minimum_value minimum value of the element
     * @param maximum_value maximum value of the element
     * @param default_value default value (mind! should fit to the enum_types data_type)
     * @param scale scale of the element
     * @param offset offset of the value
     */
    DDElement(const std::string& name,
              const DDEnum& enum_type,
              const Deserialized& deserialized_info = {},
              const Serialized& serialized_info = {},
              const dd::ArraySize& array_size = 1,
              const std::string& description = {},
              const std::string& comment = {},
              const dd::BaseUnit& base_unit = {},
              const std::string& value = {},
              const std::string& minimum_value = {},
              const std::string& maximum_value = {},
              const std::string& default_value = {},
              const std::string& scale = {},
              const std::string& offset = {});

    /**
     * @brief Construct a new DDElement object that uses a Enum Type (dd::EnumType)
     * Use it like:
     * @code
     * ddl::DDUnit km("Kilometre", "1", "1", "0",
     *                { ddl::DDRefUnit(ddl::BaseUnit<ddl::unit::Metre>(),
     *                  3, ddl::UnitPrefix<ddl::unit_prefix::kilo>()) });
     * ddl::DDEnum dd_enum("EnumType",
     *                     ddl::DataType<uint32_t>(),
     *                     { {"val1", "1" }, { "val2", "2"} });
     * ddl::DDStructure dd_struct("StructType");
     * dd_struct.addElement(ddl::DDElement("element_enum", dd_enum{}, {}, 1, {}, {}, km));
     * @endcode
     *
     * @param name Name of the element.
     * @param enum_type Enum Type of the element. Use @ref ddl::DDEnum class.
     * @param deserialized_info deserialized information (will be set by default to the alignment of
     * data_type).
     * @param serialized_info serialized information of this element (will be set automatically to
     * the next valid bytepos depending on previous elements).
     * @param array_size array size of the element
     * @param description description of the element
     * @param comment comment of the element
     * @param unit the unit name of the element
     * @param value constant value name of the element (mind! this constant should exists in given
     * enum_type!)
     * @param minimum_value minimum value of the element
     * @param maximum_value maximum value of the element
     * @param default_value default value (mind! should fit to the enum_types data_type)
     * @param scale scale of the element
     * @param offset offset of the value
     */
    DDElement(const std::string& name,
              const DDEnum& enum_type,
              const Deserialized& deserialized_info,
              const Serialized& serialized_info,
              const dd::ArraySize& array_size,
              const std::string& description,
              const std::string& comment,
              const DDUnit& unit,
              const std::string& value = {},
              const std::string& minimum_value = {},
              const std::string& maximum_value = {},
              const std::string& default_value = {},
              const std::string& scale = {},
              const std::string& offset = {});

    /**
     * @brief Construct a new DDElement object that uses a Enum Type (dd::EnumType)
     * Use it like:
     * @code
     * ddl::DDEnum dd_enum("EnumType", ddl::DataType<uint32_t>(), { {"val1", "1" }, { "val2",
     * "2"} }); ddl::DDStructure dd_struct("StructType");
     * dd_struct.addElement(ddl::DDElement("element_enum", dd_enum));
     * @endcode
     *
     * @param name Name of the element.
     * @param structure_type DDStructure Type of the element. Use @ref ddl::DDStructure class.
     * @param deserialized_info deserialized information (will be set by default to the alignment of
     * data_type).
     * @param serialized_info serialized information of this element (will be set automatically to
     * the next valid bytepos depending on previous elements).
     * @param array_size array size of the element
     * @param description description of the element
     * @param comment comment of the element
     */
    DDElement(const std::string& name,
              const DDStructure& structure_type,
              const Deserialized& deserialized_info = {},
              const Serialized& serialized_info = {},
              const dd::ArraySize& array_size = 1,
              const std::string& description = {},
              const std::string& comment = {});

    /**
     * @brief Get the Element object
     *
     * @return const dd::StructType::Element&
     */
    const dd::StructType::Element& getElement() const;
    /**
     * @brief retrieves a valid DataDefinition for the elements datatypes and units.
     *
     * @return const dd::DataDefinition&
     */
    const dd::DataDefinition& getDD() const;

private:
    // the data definition
    dd::DataDefinition _dd;
    // the element to add to the struct
    dd::StructType::Element _element;
};
} // namespace ddl

#endif // DDELEM_H_INCLUDED
