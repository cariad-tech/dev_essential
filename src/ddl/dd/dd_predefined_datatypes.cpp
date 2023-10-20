/**
 * @file
 * OO DataDefinition DataTypes implementation the predefined types
 *
 * Copyright @ 2021 VW Group. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <ddl/dd/dd_predefined_datatypes.h>

namespace ddl {

PredefinedDataTypes::PredefinedDataTypes()
    : _defined_types{
          {"bool", {std::make_shared<DataType<bool>>("bool"), {"tBool"}}},
          {"tBool", {std::make_shared<DataType<bool>>("tBool"), {"bool"}}},
          {"char", {std::make_shared<DataType<char>>("char"), {"tChar"}}},
          {"tChar", {std::make_shared<DataType<char>>("tChar"), {"char"}}},
          {"uint8_t", {std::make_shared<DataType<std::uint8_t>>("uint8_t"), {"tUInt8"}}},
          {"int8_t", {std::make_shared<DataType<std::int8_t>>("int8_t"), {"tInt8"}}},
          {"tUInt8", {std::make_shared<DataType<std::uint8_t>>("tUInt8"), {"uint8_t"}}},
          {"tInt8", {std::make_shared<DataType<std::int8_t>>("tInt8"), {"int8_t"}}},
          {"uint16_t", {std::make_shared<DataType<std::uint16_t>>("uint16_t"), {"tUInt16"}}},
          {"int16_t", {std::make_shared<DataType<std::int16_t>>("int16_t"), {"tInt16"}}},
          {"tUInt16", {std::make_shared<DataType<std::uint16_t>>("tUInt16"), {"uint16_t"}}},
          {"tInt16", {std::make_shared<DataType<std::int16_t>>("tInt16"), {"int16_t"}}},
          {"uint32_t", {std::make_shared<DataType<std::uint32_t>>("uint32_t"), {"tUInt32"}}},
          {"int32_t", {std::make_shared<DataType<std::int32_t>>("int32_t"), {"tInt32"}}},
          {"tUInt32", {std::make_shared<DataType<std::uint32_t>>("tUInt32"), {"uint32_t"}}},
          {"tInt32", {std::make_shared<DataType<std::int32_t>>("tInt32"), {"int32_t"}}},
          {"uint64_t", {std::make_shared<DataType<std::uint64_t>>("uint64_t"), {"tUInt64"}}},
          {"int64_t", {std::make_shared<DataType<std::int64_t>>("int64_t"), {"tInt64"}}},
          {"tUInt64", {std::make_shared<DataType<std::uint64_t>>("tUInt64"), {"uint64_t"}}},
          {"tInt64", {std::make_shared<DataType<std::int64_t>>("tInt64"), {"int64_t"}}},
          {"float", {std::make_shared<DataType<float>>("float"), {"tFloat32"}}},
          {"tFloat32", {std::make_shared<DataType<float>>("tFloat32"), {"float"}}},
          {"double", {std::make_shared<DataType<double>>("double"), {"tFloat64"}}},
          {"tFloat64", {std::make_shared<DataType<double>>("tFloat64"), {"double"}}}}
{
}

PredefinedDataTypes::~PredefinedDataTypes()
{
}

const PredefinedDataTypes& PredefinedDataTypes::getInstance()
{
    static const PredefinedDataTypes types;
    return types;
}

std::shared_ptr<dd::datamodel::DataType> PredefinedDataTypes::getPredefinedType(
    const std::string& name) const
{
    auto found = _defined_types.find(name);
    if (found != _defined_types.end()) {
        return found->second._type;
    }
    return {};
}

dd::OptionalSize PredefinedDataTypes::getDefaultAlignment(const std::string& name) const
{
    auto found = _defined_types.find(name);
    if (found != _defined_types.end()) {
        return found->second._type->getDefaultAlignment();
    }
    return {};
}

std::vector<std::string> PredefinedDataTypes::getAliasTypes(const std::string& name) const
{
    auto found = _defined_types.find(name);
    if (found != _defined_types.end()) {
        return found->second._aliases;
    }
    return {};
}

std::vector<std::shared_ptr<dd::datamodel::DataType>> PredefinedDataTypes::getPredefinedTypes()
    const
{
    std::vector<std::shared_ptr<dd::datamodel::DataType>> data_types;
    for (const auto& defined_type: _defined_types) {
        data_types.push_back(defined_type.second._type);
    }
    return data_types;
}

} // namespace ddl
