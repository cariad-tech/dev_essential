/**
 * @file
 * OO DataDefinition DataTypes header for template based DataType usages.
 *
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */

#ifndef DD_DATATYPE_H_INCLUDED
#define DD_DATATYPE_H_INCLUDED

#include <ddl/dd/dd.h>
#include <ddl/dd/dd_common_types.h>
#include <ddl/dd/dd_predefined_datatypes.h>
#include <ddl/dd/ddunit.h>

#include <string>

namespace ddl {
/**
 * @brief A DataType class holding the DD for a Datatype with dependencies. (i.e. Units!)
 *
 */
class DDDataType {
public:
    /**
     * @brief no default CTOR
     *
     */
    DDDataType() = delete;
    /**
     * @brief copy CTOR
     *
     */
    DDDataType(const DDDataType&);
    /**
     * @brief move CTOR
     *
     */
    DDDataType(DDDataType&&);
    /**
     * @brief copy assignment operator
     *
     * @return DDDataType&
     */
    DDDataType& operator=(const DDDataType&);
    /**
     * @brief move assignment operator
     *
     * @return DDDataType&
     */
    DDDataType& operator=(DDDataType&&);

    /**
     * @brief CTOR
     *
     * @param name the name of the POD Type
     * @param bit_size the bit size of the type
     * @param description the description of the type
     * @param array_size the array size of the type (please use only 1! codec API does not support
     * greater than 1 here!)
     * @param minimum_value the minimum value (for information only)
     * @param maximum_value the maximum value (for information only)
     * @param default_type_alignment Type alignment (this is optional, because it is not part of the
     *                               DD, but important for automatic calculation of member
     *                               positions).
     */
    DDDataType(const std::string& name,
               size_t bit_size,
               const std::string& description = {},
               dd::OptionalSize array_size = {},
               const std::string& minimum_value = {},
               const std::string& maximum_value = {},
               dd::OptionalSize default_type_alignment = {});
    /**
     * @brief CTOR
     *
     * @param name the name of the POD Type
     * @param bit_size the bit size of the type
     * @param description the description of the type
     * @param array_size the array size of the type (please use only 1! codec API does not support
     *                    greater than 1 here!)
     * @param minimum_value the minimum value (for information only)
     * @param maximum_value the maximum value (for information only)
     * @param unit the unit to use for this type
     * @param default_type_alignment Type alignment (this is optional, because it is not part of the
     *                               DD, but important for automatic calculation of member
     *                               positions)
     */
    DDDataType(const std::string& name,
               size_t bit_size,
               const std::string& description,
               dd::OptionalSize array_size,
               const std::string& minimum_value,
               const std::string& maximum_value,
               dd::OptionalSize default_type_alignment,
               const DDUnit& unit);
    /**
     * @brief CTOR
     *
     * @param name the name of the POD Type
     * @param bit_size the bit size of the type
     * @param description the description of the type
     * @param array_size the array size of the type (please use only 1! codec API does not support
     * greater than 1 here!)
     * @param minimum_value the minimum value (for information only)
     * @param maximum_value the maximum value (for information only)
     * @param base_unit the base_unit to use for this type
     * @param default_type_alignment Type alignment (this is optional, because it is not part of the
     *                               DD, but important for automatic calculation of member
     *                               positions)
     */
    DDDataType(const std::string& name,
               size_t bit_size,
               const std::string& description,
               dd::OptionalSize array_size,
               const std::string& minimum_value,
               const std::string& maximum_value,
               dd::OptionalSize default_type_alignment,
               const dd::BaseUnit& base_unit);
    /**
     * @brief Get the Data Type object
     *
     * @return const dd::DataType&
     */
    const dd::DataType& getDataType() const;
    /**
     * @brief Get the DataDefinition
     *
     * @return const dd::DataDefinition&
     */
    const dd::DataDefinition& getDD() const;

private:
    /**
     * @brief the type
     *
     */
    std::shared_ptr<dd::DataType> _data_type;
    /**
     * @brief the data definition
     *
     */
    dd::DataDefinition _dd;
};

} // namespace ddl

#endif // DD_DATATYPE_H_INCLUDED
