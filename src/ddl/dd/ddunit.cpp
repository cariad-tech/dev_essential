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

#include <ddl/dd/ddunit.h>

namespace ddl {

DDRefUnit::DDRefUnit(const dd::BaseUnit& base_unit,
                     int32_t power,
                     const dd::UnitPrefix& unit_prefix)
    : _ref_unit(base_unit.getName(), power, unit_prefix.getName())
{
    _dd.getBaseUnits().add(base_unit);
    _dd.getUnitPrefixes().add(unit_prefix);
}

const dd::Unit::RefUnit& DDRefUnit::getRefUnit() const
{
    return _ref_unit;
}

const dd::DataDefinition& DDRefUnit::getDD() const
{
    return _dd;
}

DDUnit::DDUnit(const DDUnit& other) : _dd(other._dd)
{
    _unit = _dd.getUnits().access(other.getUnit().getName());
}

DDUnit::DDUnit(DDUnit&& other) : _unit(std::move(other._unit)), _dd(std::move(other._dd))
{
}

DDUnit& DDUnit::operator=(const DDUnit& other)
{
    _dd = other._dd;
    _unit = _dd.getUnits().access(other.getUnit().getName());
    return *this;
}

DDUnit& DDUnit::operator=(DDUnit&& other)
{
    _dd = std::move(other._dd);
    _unit = std::move(other._unit);
    return *this;
}

DDUnit::DDUnit(const std::string& name,
               const std::string& numerator,
               const std::string& denominator,
               const std::string& offset,
               const std::vector<DDRefUnit>& ref_units)
{
    _dd.getUnits().add(dd::Unit(name, numerator, denominator, offset));
    _unit = _dd.getUnits().access(name);
    for (const auto& curent_ref: ref_units) {
        addRefUnit(curent_ref);
    }
}

void DDUnit::addRefUnit(const DDRefUnit& ref_unit)
{
    _dd.getBaseUnits().add(
        *ref_unit.getDD().getBaseUnits().get(ref_unit.getRefUnit().getUnitName()));
    _dd.getUnitPrefixes().add(
        *ref_unit.getDD().getUnitPrefixes().get(ref_unit.getRefUnit().getPrefixName()));
    _unit->addRefUnit(ref_unit.getRefUnit());
}

const dd::Unit& DDUnit::getUnit() const
{
    return *_unit;
}

const dd::DataDefinition& DDUnit::getDD() const
{
    return _dd;
}

} // namespace ddl
