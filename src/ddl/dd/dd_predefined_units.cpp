/**
 * @file
 * Predefined Units Implementation
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

#include <ddl/dd/dd_predefined_units.h>

// for older gcc we need to define the value within at least one object
#define DDL_BASE_UNIT_OBJ_DEFINITION(_classname_)                                                  \
    namespace unit {                                                                               \
    constexpr const char* const _classname_::_name;                                                \
    constexpr const char* const _classname_::_symbol;                                              \
    constexpr const char* const _classname_::_description;                                         \
    }                                                                                              \
    static_assert(true, "Simply to make sure a closing semicolon ';' is used for the macro")

// for older gcc we need to define the value within at least one object
#define DDL_UNIT_PREFIX_OBJ_DEFINITION(_classname_)                                                \
    namespace unit_prefix {                                                                        \
    constexpr const char* const _classname_::_name;                                                \
    constexpr const char* const _classname_::_symbol;                                              \
    constexpr const int32_t _classname_::_power;                                                   \
    }                                                                                              \
    static_assert(true, "Simply to make sure a closing semicolon ';' is used for the macro")

namespace ddl {

DDL_BASE_UNIT_OBJ_DEFINITION(Metre);
DDL_BASE_UNIT_OBJ_DEFINITION(Kilogram);
DDL_BASE_UNIT_OBJ_DEFINITION(Second);
DDL_BASE_UNIT_OBJ_DEFINITION(Ampere);
DDL_BASE_UNIT_OBJ_DEFINITION(Kelvin);
DDL_BASE_UNIT_OBJ_DEFINITION(Mole);
DDL_BASE_UNIT_OBJ_DEFINITION(Candela);
DDL_BASE_UNIT_OBJ_DEFINITION(Degree);
DDL_BASE_UNIT_OBJ_DEFINITION(Radiant);
DDL_BASE_UNIT_OBJ_DEFINITION(Unitless);
DDL_BASE_UNIT_OBJ_DEFINITION(Nou);

DDL_UNIT_PREFIX_OBJ_DEFINITION(yotta);
DDL_UNIT_PREFIX_OBJ_DEFINITION(zetta);
DDL_UNIT_PREFIX_OBJ_DEFINITION(exa);
DDL_UNIT_PREFIX_OBJ_DEFINITION(peta);
DDL_UNIT_PREFIX_OBJ_DEFINITION(tera);
DDL_UNIT_PREFIX_OBJ_DEFINITION(giga);
DDL_UNIT_PREFIX_OBJ_DEFINITION(mega);
DDL_UNIT_PREFIX_OBJ_DEFINITION(kilo);
DDL_UNIT_PREFIX_OBJ_DEFINITION(hecto);
DDL_UNIT_PREFIX_OBJ_DEFINITION(deca);
DDL_UNIT_PREFIX_OBJ_DEFINITION(deci);
DDL_UNIT_PREFIX_OBJ_DEFINITION(centi);
DDL_UNIT_PREFIX_OBJ_DEFINITION(milli);
DDL_UNIT_PREFIX_OBJ_DEFINITION(micro);
DDL_UNIT_PREFIX_OBJ_DEFINITION(nano);
DDL_UNIT_PREFIX_OBJ_DEFINITION(pico);
DDL_UNIT_PREFIX_OBJ_DEFINITION(femto);
DDL_UNIT_PREFIX_OBJ_DEFINITION(atto);
DDL_UNIT_PREFIX_OBJ_DEFINITION(zepto);
DDL_UNIT_PREFIX_OBJ_DEFINITION(yocto);

PredefinedUnits::PredefinedUnits()
    : _defined_base_units{{unit::Metre::_name, std::make_shared<BaseUnit<unit::Metre>>()},
                          {unit::Kilogram::_name, std::make_shared<BaseUnit<unit::Kilogram>>()},
                          {unit::Second::_name, std::make_shared<BaseUnit<unit::Second>>()},
                          {unit::Ampere::_name, std::make_shared<BaseUnit<unit::Ampere>>()},
                          {unit::Kelvin::_name, std::make_shared<BaseUnit<unit::Kelvin>>()},
                          {unit::Mole::_name, std::make_shared<BaseUnit<unit::Mole>>()},
                          {unit::Candela::_name, std::make_shared<BaseUnit<unit::Candela>>()},
                          {unit::Degree::_name, std::make_shared<BaseUnit<unit::Degree>>()},
                          {unit::Radiant::_name, std::make_shared<BaseUnit<unit::Radiant>>()},
                          {unit::Unitless::_name, std::make_shared<BaseUnit<unit::Unitless>>()},
                          {unit::Nou::_name, std::make_shared<BaseUnit<unit::Nou>>()}},
      _defined_unit_prefixes{
          {unit_prefix::yotta::_name, std::make_shared<UnitPrefix<unit_prefix::yotta>>()},
          {unit_prefix::zetta::_name, std::make_shared<UnitPrefix<unit_prefix::zetta>>()},
          {unit_prefix::exa::_name, std::make_shared<UnitPrefix<unit_prefix::exa>>()},
          {unit_prefix::peta::_name, std::make_shared<UnitPrefix<unit_prefix::peta>>()},
          {unit_prefix::tera::_name, std::make_shared<UnitPrefix<unit_prefix::tera>>()},
          {unit_prefix::giga::_name, std::make_shared<UnitPrefix<unit_prefix::giga>>()},
          {unit_prefix::mega::_name, std::make_shared<UnitPrefix<unit_prefix::mega>>()},
          {unit_prefix::kilo::_name, std::make_shared<UnitPrefix<unit_prefix::kilo>>()},
          {unit_prefix::hecto::_name, std::make_shared<UnitPrefix<unit_prefix::hecto>>()},
          {unit_prefix::deca::_name, std::make_shared<UnitPrefix<unit_prefix::deca>>()},
          {unit_prefix::deci::_name, std::make_shared<UnitPrefix<unit_prefix::deci>>()},
          {unit_prefix::centi::_name, std::make_shared<UnitPrefix<unit_prefix::centi>>()},
          {unit_prefix::milli::_name, std::make_shared<UnitPrefix<unit_prefix::milli>>()},
          {unit_prefix::micro::_name, std::make_shared<UnitPrefix<unit_prefix::micro>>()},
          {unit_prefix::nano::_name, std::make_shared<UnitPrefix<unit_prefix::nano>>()},
          {unit_prefix::pico::_name, std::make_shared<UnitPrefix<unit_prefix::pico>>()},
          {unit_prefix::femto::_name, std::make_shared<UnitPrefix<unit_prefix::femto>>()},
          {unit_prefix::atto::_name, std::make_shared<UnitPrefix<unit_prefix::atto>>()},
          {unit_prefix::zepto::_name, std::make_shared<UnitPrefix<unit_prefix::zepto>>()},
          {unit_prefix::yocto::_name, std::make_shared<UnitPrefix<unit_prefix::yocto>>()}}
{
}

PredefinedUnits::~PredefinedUnits()
{
}

const PredefinedUnits& PredefinedUnits::getInstance()
{
    static PredefinedUnits units;
    return units;
}

std::shared_ptr<dd::datamodel::BaseUnit> PredefinedUnits::getPredefinedBaseUnit(
    const std::string& name) const
{
    auto found = _defined_base_units.find(name);
    if (found != _defined_base_units.end()) {
        return found->second;
    }
    return {};
}

std::shared_ptr<dd::datamodel::UnitPrefix> PredefinedUnits::getPredefinedUnitPrefix(
    const std::string& name) const
{
    auto found = _defined_unit_prefixes.find(name);
    if (found != _defined_unit_prefixes.end()) {
        return found->second;
    }
    return {};
}

std::vector<std::shared_ptr<dd::datamodel::BaseUnit>> PredefinedUnits::getPredefinedBaseUnits()
    const
{
    std::vector<std::shared_ptr<dd::datamodel::BaseUnit>> base_units;
    for (const auto& defined_base_unit: _defined_base_units) {
        base_units.push_back(defined_base_unit.second);
    }
    return base_units;
}

std::vector<std::shared_ptr<dd::datamodel::UnitPrefix>> PredefinedUnits::getPredefinedUnitPrefixes()
    const
{
    std::vector<std::shared_ptr<dd::datamodel::UnitPrefix>> unit_prefixes;
    for (const auto& defined_unit_prefix: _defined_unit_prefixes) {
        unit_prefixes.push_back(defined_unit_prefix.second);
    }
    return unit_prefixes;
}

} // namespace ddl
