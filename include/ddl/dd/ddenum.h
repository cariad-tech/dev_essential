/**
 * @file
 * OO DataDefinition Enum header
 *
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */

#ifndef DDENUM_H_INCLUDED
#define DDENUM_H_INCLUDED

#include <ddl/dd/dd.h>
#include <ddl/dd/dddatatype.h>
#include <ddl/utilities/std_to_string.h>

namespace ddl {

/**
 * @brief Utility class for a complete valid data definition of one EnumType and its dependencies.
 * This class is for convenience to use it together with ddl::DDStructure .
 * @see ddl::DDStructure.
 */
class DDEnum {
public:
    /**
     * @brief Construct a new DDEnum object
     *
     */
    DDEnum() = delete;
    /**
     * @brief Construct a new DDEnum object
     *
     */
    DDEnum(DDEnum&&);
    /**
     * @brief
     *
     * @return DDEnum&
     */
    DDEnum& operator=(DDEnum&&);
    /**
     * @brief Construct a new DDEnum object
     *
     * @param other the enum type definition to copy
     */
    DDEnum(const DDEnum& other);
    /**
     * @brief copies a DDEnum object
     *
     * @param other the enum type definition to copy
     */
    DDEnum& operator=(const DDEnum& other);
    /**
     * @brief Construct a new DDEnum object
     *
     * @param name name of the enum type.
     * @param data_type base data type to use (see templates @ref ddl::DataType).
     * @param elements vector of element of the enum type.
     * @throw ddl::dd::Error throws if elements with the same name are added.
     */
    DDEnum(const std::string& name,
           const dd::DataType& data_type,
           const std::vector<dd::EnumType::Element>& elements = {});

    /**
     * @brief Construct a new DDEnum object
     *
     * @param name name of the enum type.
     * @param data_type the data type to use (use this if you created a new datatype with units i.e.
     * )
     * @param elements vector of element of the enum type.
     * @throw ddl::dd::Error throws if elements with the same name are added.
     */
    DDEnum(const std::string& name,
           const DDDataType& data_type,
           const std::vector<dd::EnumType::Element>& elements = {});

    /**
     * @brief adds a EnumType element.
     *
     * @param element the elment to add.
     * @throw ddl::dd::Error throws if element with the same name already exists.
     */
    DDEnum& addElement(const dd::EnumType::Element& element);
    /**
     * @brief adds a EnumType element.
     *
     * @param name the name of the element to add.
     * @param value the name of the element (if it is an integer value).
     * @throw ddl::dd::Error throws if element with the same name already exists.
     */
    DDEnum& addElement(const std::string& name, int64_t value);
    /**
     * @brief adds a vector of EnumType elements.
     *
     * @param elements the elments to add.
     * @throw ddl::dd::Error throws if element with the same name already exists.
     */
    DDEnum& addElements(const std::vector<dd::EnumType::Element>& elements);

    /**
     * @brief Get the Enum Type object
     *
     * @return const dd::EnumType&
     */
    const dd::EnumType& getEnumType() const;
    /**
     * @brief return the valid data definition.
     *
     * @return const dd::DataDefinition&
     */
    const dd::DataDefinition& getDD() const;

    /**
     * @brief returns the alignment of the underlying data type.
     *
     * @return the alignemnt of the datatype
     */
    size_t getAlignment() const;

private:
    /// the data definition
    dd::DataDefinition _dd;
    // the enumtype
    std::shared_ptr<dd::EnumType> _enum_type;
};

/**
 * This is used to create the DD representation of an enumeration type.
 *
 * #### Example Usages
 * \code
 * enum MyEnum : uint8_t {eA, eB};
 * auto my_enum_type = DDEnumGenerator<MyEnum>("MyEnum")
 *                        .addElement("eA", eA)
 *                        .addElement("eB", eB);
 * \endcode
 * @tparam EnumDataType The type of the enum .
 * @tparam Enable type trait
 */
template <typename EnumDataType, typename Enable = void>
class DDEnumGenerator : public DDEnum {
public:
    /**
     * @brief CTOR
     *
     * @param name the name of the enum for DataDefinition
     * @param elements the elements of the enumeration
     */
    DDEnumGenerator(const std::string& name,
                    const std::vector<std::pair<std::string, EnumDataType>> elements = {})
        : DDEnum(name, DataType<typename std::underlying_type<EnumDataType>::type>())
    {
        for (const auto& elem: elements) {
            DDEnum::addElement(dd::EnumType::Element(
                elem.first,
                std::to_string(
                    static_cast<typename std::underlying_type<EnumDataType>::type>(elem.second))));
        }
    }

    /**
     * @brief Adds a element for the enumeration with name and value
     *
     * @param name the name of the enumeration value
     * @param value the value for the name out of given type \p EnumDataType
     *
     * @return DDEnumGenerator& a reference of this
     */
    DDEnumGenerator& addElement(const std::string& name, EnumDataType value)
    {
        DDEnum::addElement(dd::EnumType::Element(
            name,
            std::to_string(static_cast<typename std::underlying_type<EnumDataType>::type>(value))));
        return *this;
    }
};

/**
 * This is used to create an enumeration type independently.
 *
 * #### Example Usages
 * \code
 * auto my_enum_type = DDEnumGenerator<uint8_t>("MyEnum")
 *                        .addElement("eA", 0)
 *                        .addElement("eB", 1);
 * \endcode
 * @tparam EnumDataType The type of the enum .
 */
template <typename EnumDataType>
class DDEnumGenerator<EnumDataType,
                      typename std::enable_if<std::is_arithmetic<EnumDataType>::value>::type>
    : public DDEnum {
public:
    /**
     * @brief CTOR
     *
     * @param name the name of the enum for DataDefinition
     * @param elements the elements of the enumeration
     */
    DDEnumGenerator(const std::string& name,
                    const std::vector<std::pair<std::string, EnumDataType>> elements = {})
        : DDEnum(name, DataType<EnumDataType>())
    {
        for (const auto& elem: elements) {
            DDEnum::addElement(dd::EnumType::Element(elem.first, std::to_string(elem.second)));
        }
    }
};

} // namespace ddl

#endif // DDENUM_H_INCLUDED