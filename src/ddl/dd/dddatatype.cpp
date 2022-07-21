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

#include <a_util/strings.h>
#include <ddl/dd/dddatatype.h>

namespace ddl {

DDDataType::DDDataType(const DDDataType& other) : _dd(other._dd)
{
    _data_type = _dd.getDataTypes().access(other.getDataType().getName());
}

DDDataType& DDDataType::operator=(const DDDataType& other)
{
    _dd = other._dd;
    _data_type = _dd.getDataTypes().access(other.getDataType().getName());
    return *this;
}

DDDataType::DDDataType(DDDataType&& other)
    : _dd(std::move(other._dd)), _data_type(std::move(other._data_type))
{
}

DDDataType& DDDataType::operator=(DDDataType&& other)
{
    _dd = std::move(other._dd);
    _data_type = std::move(other._data_type);
    return *this;
}

DDDataType::DDDataType(const std::string& name,
                       size_t bit_size,
                       const std::string& description,
                       dd::OptionalSize array_size,
                       const std::string& minimum_value,
                       const std::string& maximum_value,
                       dd::OptionalSize default_type_alignment)
{
    _dd.getDataTypes().emplace(dd::DataType(name,
                                            bit_size,
                                            description,
                                            array_size,
                                            {},
                                            minimum_value,
                                            maximum_value,
                                            default_type_alignment));
    _dd.validate();
    if (!_dd.isValid(dd::ValidationInfo::good_enough)) {
        throw dd::Error(
            "DDDataType::DDDataType",
            {name},
            a_util::strings::join(dd::transformProblemList(_dd.getValidationProtocol()), "\n"));
    }
    _data_type = _dd.getDataTypes().access(name);
}

DDDataType::DDDataType(const std::string& name,
                       size_t bit_size,
                       const std::string& description,
                       dd::OptionalSize array_size,
                       const std::string& minimum_value,
                       const std::string& maximum_value,
                       dd::OptionalSize default_type_alignment,
                       const DDUnit& unit)
{
    _dd.add(unit.getDD());
    _dd.getUnits().add(unit.getUnit());
    _dd.getDataTypes().emplace(dd::DataType(name,
                                            bit_size,
                                            description,
                                            array_size,
                                            {},
                                            minimum_value,
                                            maximum_value,
                                            default_type_alignment));
    _dd.validate();
    if (!_dd.isValid(dd::ValidationInfo::good_enough)) {
        throw dd::Error(
            "DDDataType::DDDataType",
            {name},
            a_util::strings::join(dd::transformProblemList(_dd.getValidationProtocol()), "\n"));
    }
    _data_type = _dd.getDataTypes().access(name);
}

DDDataType::DDDataType(const std::string& name,
                       size_t bit_size,
                       const std::string& description,
                       dd::OptionalSize array_size,
                       const std::string& minimum_value,
                       const std::string& maximum_value,
                       dd::OptionalSize default_type_alignment,
                       const dd::BaseUnit& base_unit)
{
    _dd.getBaseUnits().add(base_unit);
    _dd.getDataTypes().emplace(dd::DataType(name,
                                            bit_size,
                                            description,
                                            array_size,
                                            {},
                                            minimum_value,
                                            maximum_value,
                                            default_type_alignment));
    _dd.validate();
    if (!_dd.isValid(dd::ValidationInfo::good_enough)) {
        throw dd::Error(
            "DDDataType::DDDataType",
            {name},
            a_util::strings::join(dd::transformProblemList(_dd.getValidationProtocol()), "\n"));
    }
    _data_type = _dd.getDataTypes().access(name);
}

const dd::DataType& DDDataType::getDataType() const
{
    return *_data_type;
}

const dd::DataDefinition& DDDataType::getDD() const
{
    return _dd;
}

} // namespace ddl
