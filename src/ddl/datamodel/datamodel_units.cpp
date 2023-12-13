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

#include <ddl/datamodel/datamodel_units.h>
#include <ddl/utilities/std_to_string.h>

#include <exception>
#include <string>
#include <utility>

namespace ddl {

namespace dd {

namespace datamodel {

/*************************************************************************************************************/
// UnitBase
/*************************************************************************************************************/
UnitBase::UnitBase(const std::string& name) : _name(name)
{
}

const std::string& UnitBase::getName() const
{
    return _name;
}

void UnitBase::setName(const std::string& name)
{
    _name = name;
}

/*************************************************************************************************************/
// BaseUnit
/*************************************************************************************************************/
BaseUnit::BaseUnit(const std::string& name,
                   const std::string& symbol,
                   const std::string& description)
    : UnitBase(name), _symbol(symbol), _description(description)
{
}
TypeOfUnit BaseUnit::getTypeOfUnit() const
{
    return base_unit;
}

void BaseUnit::setName(const std::string& name)
{
    // Keep old name for reset
    std::string old_name = UnitBase::getName();
    UnitBase::setName(name);
    try {
        utility::TypeAccessMapSubject<BaseUnit>::notifyChanged(
            utility::TypeAccessMapEventCode::map_item_renamed, *this, old_name);
        ModelSubject<BaseUnit>::notifyChanged(item_renamed, *this, old_name);
    }
    catch (const dd::Error& error) {
        // keep consistent .. this exeption does not invalidate the whole model
        BaseUnit::setName(old_name);
        throw error;
    }

    notify("name");
}

const std::string& BaseUnit::getSymbol() const
{
    return _symbol;
}

void BaseUnit::setSymbol(const std::string& symbol)
{
    _symbol = symbol;
    notify("symbol");
}

const std::string& BaseUnit::getDescription() const
{
    return _description;
}

void BaseUnit::setDescription(const std::string& description)
{
    _description = description;
    notify("description");
}

void BaseUnit::notify(const std::string& additional_info)
{
    ModelSubject<BaseUnit>::notifyChanged(item_changed, *this, additional_info);
    utility::TypeAccessMapSubject<BaseUnit>::notifyChanged(
        utility::TypeAccessMapEventCode::map_item_changed, *this, additional_info);
}

bool BaseUnit::operator==(const BaseUnit& other) const
{
    return getName() == other.getName() && getSymbol() == other.getSymbol();
}

bool BaseUnit::operator!=(const BaseUnit& other) const
{
    return !operator==(other);
}
/*************************************************************************************************************/
// UnitPrefix
/*************************************************************************************************************/
UnitPrefix::UnitPrefix(const std::string& name, const std::string& symbol, int32_t power)
    : _name(name), _symbol(symbol), _power(power)
{
}

const std::string& UnitPrefix::getName() const
{
    return _name;
}

void UnitPrefix::setName(const std::string& name)
{
    auto old_name = _name;
    _name = name;
    try {
        utility::TypeAccessMapSubject<UnitPrefix>::notifyChanged(
            utility::TypeAccessMapEventCode::map_item_renamed, *this, old_name);
        ModelSubject<UnitPrefix>::notifyChanged(item_renamed, *this, old_name);
    }
    catch (const dd::Error& error) {
        // keep consistent .. this exeption does not invalidate the whole model
        _name = old_name;
        throw error;
    }

    notify("name");
}

const std::string& UnitPrefix::getSymbol() const
{
    return _symbol;
}

void UnitPrefix::setSymbol(const std::string& symbol)
{
    _symbol = symbol;
    notify("symbol");
}

int32_t UnitPrefix::getPower() const
{
    return _power;
}

void UnitPrefix::setPower(int32_t power)
{
    _power = power;
    notify("power");
}

bool UnitPrefix::operator==(const UnitPrefix& other) const
{
    return getPower() == other.getPower() && getName() == other.getName() &&
           getSymbol() == other.getSymbol();
}

bool UnitPrefix::operator!=(const UnitPrefix& other) const
{
    return !operator==(other);
}

void UnitPrefix::notify(const std::string& additional_info)
{
    ModelSubject<UnitPrefix>::notifyChanged(item_changed, *this, additional_info);
    utility::TypeAccessMapSubject<UnitPrefix>::notifyChanged(
        utility::TypeAccessMapEventCode::map_item_changed, *this, additional_info);
}

/*************************************************************************************************************/
// Unit::RefUnit
/*************************************************************************************************************/
Unit::RefUnit::RefUnit(const std::string& unit_name, int32_t power, const std::string& prefix_name)
    : _unit_name(unit_name), _power(power), _prefix_name(prefix_name)
{
}

bool Unit::RefUnit::operator==(const RefUnit& other) const
{
    return getUnitName() == other.getUnitName() && getPower() == other.getPower() &&
           getPrefixName() == other.getPrefixName();
}

bool Unit::RefUnit::operator!=(const RefUnit& other) const
{
    return !operator==(other);
}

const std::string& Unit::RefUnit::getUnitName() const
{
    return _unit_name;
}

int32_t Unit::RefUnit::getPower() const
{
    return _power;
}

const std::string& Unit::RefUnit::getPrefixName() const
{
    return _prefix_name;
}

/*************************************************************************************************************/
// Unit
/*************************************************************************************************************/
Unit::Unit()
{
}

Unit::Unit(const std::string& name,
           const std::string& numerator,
           const std::string& denominator,
           const std::string& offset,
           const std::vector<RefUnit>& ref_units)
    : UnitBase(name), _numerator(numerator), _denominator(denominator), _offset(offset)
{
    for (auto& ref: ref_units) {
        addRefUnit(ref);
    }
}

namespace {
bool isVectorEqualNoOrder(const std::vector<Unit::RefUnit>& left,
                          const std::vector<Unit::RefUnit>& right)
{
    if (left.size() != right.size()) {
        return false;
    }
    for (const auto& current_left: left) {
        bool left_found_in_right = false;
        for (const auto& current_right: right) {
            if (current_left == current_right) {
                left_found_in_right = true;
                break;
            }
        }
        if (!left_found_in_right) {
            return false;
        }
    }
    return true;
}
} // namespace

bool Unit::operator==(const Unit& other) const
{
    return getName() == other.getName() && getDenominator() == other.getDenominator() &&
           getNumerator() == other.getNumerator() && getOffset() == other.getOffset() &&
           isVectorEqualNoOrder(_ref_units, other._ref_units);
}

bool Unit::operator!=(const Unit& other) const
{
    return !operator==(other);
}

TypeOfUnit Unit::getTypeOfUnit() const
{
    return unit;
}

void Unit::setName(const std::string& name)
{
    // Keep old name for reset
    std::string old_name = UnitBase::getName();
    UnitBase::setName(name);
    try {
        utility::TypeAccessMapSubject<Unit>::notifyChanged(
            utility::TypeAccessMapEventCode::map_item_renamed, *this, old_name);
        ModelSubject<Unit>::notifyChanged(item_renamed, *this, old_name);
    }
    catch (const dd::Error& error) {
        // keep consistent .. this exeption does not invalidate the whole model
        UnitBase::setName(old_name);
        throw error;
    }
    notify(item_changed, utility::TypeAccessMapEventCode::map_item_changed, "name");
}

const std::string& Unit::getNumerator() const
{
    return _numerator;
}

void Unit::setNumerator(const std::string& numerator)
{
    _numerator = numerator;
    notify(item_changed, utility::TypeAccessMapEventCode::map_item_changed, "numerator");
}

const std::string& Unit::getDenominator() const
{
    return _denominator;
}

void Unit::setDenominator(const std::string& denominator)
{
    _denominator = denominator;
    notify(item_changed, utility::TypeAccessMapEventCode::map_item_changed, "denominator");
}

const std::string& Unit::getOffset() const
{
    return _offset;
}

void Unit::setOffset(const std::string& offset)
{
    _offset = offset;
    notify(item_changed, utility::TypeAccessMapEventCode::map_item_changed, "offset");
}

const Unit::RefUnits& Unit::getRefUnits() const
{
    return _ref_units;
}

namespace {
std::vector<Unit::RefUnit>::iterator findRefUnit(std::vector<Unit::RefUnit>& _ref_units,
                                                 const Unit::RefUnit& ref_unit)
{
    // the ref unit id is a combination of unitname and prefixname
    for (auto unit_found = _ref_units.begin(); unit_found != _ref_units.end(); ++unit_found) {
        if ((*unit_found).getPrefixName() == ref_unit.getPrefixName() &&
            (*unit_found).getUnitName() == ref_unit.getUnitName()) {
            return unit_found;
        }
    }
    return _ref_units.end();
}
} // namespace

void Unit::addRefUnit(const RefUnit& ref_unit)
{
    auto found = findRefUnit(_ref_units, ref_unit);
    if (found != _ref_units.end()) {
        throw Error(
            "Unit::addRefUnit",
            {ref_unit.getUnitName(), std::to_string(ref_unit.getPower()), ref_unit.getPrefixName()},
            "RefUnit already defined");
    }
    else {
        _ref_units.push_back(ref_unit);
        notify(subitem_added, utility::TypeAccessMapEventCode::map_subitem_added, "ref_unit");
    }
}

void Unit::removeRefUnit(const RefUnit& ref_unit)
{
    auto found = findRefUnit(_ref_units, ref_unit);
    if (found != _ref_units.end()) {
        _ref_units.erase(found);
        notify(subitem_removed, utility::TypeAccessMapEventCode::map_subitem_removed, "ref_unit");
    }
}

void Unit::notify(ModelEventCode code,
                  utility::TypeAccessMapEventCode map_code,
                  const std::string& additional_info)
{
    ModelSubject<Unit>::notifyChanged(code, *this, additional_info);
    utility::TypeAccessMapSubject<Unit>::notifyChanged(map_code, *this, additional_info);
}

} // namespace datamodel
} // namespace dd
} // namespace ddl
