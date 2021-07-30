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

If it is not possible or desirable to put the notice in a particular file, then
You may include the notice in a location (such as a LICENSE file in a
relevant directory) where a recipient would be likely to look for such a notice.

You may add additional accurate notices of copyright ownership.
@endverbatim
 */

#ifndef DD_PREDEFINED_UNITS_H_INCLUDED
#define DD_PREDEFINED_UNITS_H_INCLUDED

#include "ddl/datamodel/datamodel_units.h"

#include <memory>
#include <string>
#include <type_traits>
#include <unordered_map>

namespace ddl {

/**
 * @brief Generator template to create baseunits.
 *
 * @tparam T the type for a base unit following a special concept
 */
template <typename T>
struct BaseUnit : public dd::datamodel::BaseUnit {
    BaseUnit() : dd::datamodel::BaseUnit(T::_name, T::_symbol, T::_description)
    {
    }
};

/**
 * @brief Generator template to create UnitPrefix.
 *
 * @tparam T the type for a unit prefix following a special concept
 */
template <typename T>
struct UnitPrefix : public dd::datamodel::UnitPrefix {
    UnitPrefix() : dd::datamodel::UnitPrefix(T::_name, T::_symbol, T::_power)
    {
    }
};

/**
 * @brief definiton of the unit namespace
 * @see ddl::PredefinedUnits
 */
namespace unit {
}
/**
 * @brief definiton of the unit prefixes namespace
 * @see ddl::PredefinedUnits
 */
namespace unit_prefix {
}
} // namespace ddl

/**
 * @brief Base unit template definiton
 * @param _classname_ the classname for the base unit
 * @param _baseunitname_ the name for the base unit
 * @param _baseunitsymbol_ the symbol for the base unit
 * @param _baseunitdescription_ the description for the base unit
 * @see ddl::PredefinedUnits.
 */
#define DDL_BASE_UNIT_DEFINTION(                                                                   \
    _classname_, _baseunitname_, _baseunitsymbol_, _baseunitdescription_)                          \
    namespace unit {                                                                               \
    struct _classname_ {                                                                           \
        constexpr static const char* const _name = _baseunitname_;                                 \
        constexpr static const char* const _symbol = _baseunitsymbol_;                             \
        constexpr static const char* const _description = _baseunitdescription_;                   \
    };                                                                                             \
    }                                                                                              \
    static_assert(true, "Simply to make sure a closing semicolon ';' is used for the macro")

/**
 * @brief Unit prefix template definiton
 * @param _classname_ the classname for the unit prefix
 * @param _unitprefixname_ the prefix name for the unit prefix
 * @param _unitprefixsymbol_ the symbol for the unit prefix
 * @param _unitprefixpower_ the power for the unit prefix
 * @see ddl::PredefinedUnits
 */
#define DDL_UNIT_PREFIX_DEFINTION(                                                                 \
    _classname_, _unitprefixname_, _unitprefixsymbol_, _unitprefixpower_)                          \
    namespace unit_prefix {                                                                        \
    struct _classname_ {                                                                           \
        constexpr static const char* const _name = _unitprefixname_;                               \
        constexpr static const char* const _symbol = _unitprefixsymbol_;                           \
        constexpr static const int32_t _power = _unitprefixpower_;                                 \
    };                                                                                             \
    }                                                                                              \
    static_assert(true, "Simply to make sure a closing semicolon ';' is used for the macro")

namespace ddl {
/// @cond nodoc
DDL_BASE_UNIT_DEFINTION(Metre, "Metre", "m", "Fundamental unit for length");
DDL_BASE_UNIT_DEFINTION(Kilogram, "Kilogram", "kg", "Fundamental unit for mass");
DDL_BASE_UNIT_DEFINTION(Second, "Second", "s", "Fundamental unit for time");
DDL_BASE_UNIT_DEFINTION(Ampere, "Ampere", "A", "Fundamental unit for electric current");
DDL_BASE_UNIT_DEFINTION(Kelvin, "Kelvin", "K", "Fundamental unit for temperature");
DDL_BASE_UNIT_DEFINTION(Mole, "Mole", "mol", "Fundamental unit for amount of substance");
DDL_BASE_UNIT_DEFINTION(Candela, "Candela", "cd", "Fundamental unit for luminous intensity");
DDL_BASE_UNIT_DEFINTION(Degree, "Degree", "deg", "Non-SI standard unit for angle");
DDL_BASE_UNIT_DEFINTION(Radiant, "Radiant", "rad", "Non-SI standard unit for angle");
DDL_BASE_UNIT_DEFINTION(Unitless, "Unitless", "", "No SI, but needed for own unit definitions");
DDL_BASE_UNIT_DEFINTION(Nou, "nou", "", "No SI, but needed for no unit definitions");

DDL_UNIT_PREFIX_DEFINTION(yotta, "yotta", "Y", 24);
DDL_UNIT_PREFIX_DEFINTION(zetta, "zetta", "Z", 21);
DDL_UNIT_PREFIX_DEFINTION(exa, "exa", "E", 18);
DDL_UNIT_PREFIX_DEFINTION(peta, "peta", "P", 15);
DDL_UNIT_PREFIX_DEFINTION(tera, "tera", "T", 12);
DDL_UNIT_PREFIX_DEFINTION(giga, "giga", "G", 9);
DDL_UNIT_PREFIX_DEFINTION(mega, "mega", "M", 6);
DDL_UNIT_PREFIX_DEFINTION(kilo, "kilo", "k", 3);
DDL_UNIT_PREFIX_DEFINTION(hecto, "hecto", "h", 2);
DDL_UNIT_PREFIX_DEFINTION(deca, "deca", "da", 1);
DDL_UNIT_PREFIX_DEFINTION(deci, "deci", "d", -1);
DDL_UNIT_PREFIX_DEFINTION(centi, "centi", "c", -2);
DDL_UNIT_PREFIX_DEFINTION(milli, "mili", "m", -3);
DDL_UNIT_PREFIX_DEFINTION(micro, "micro", "u", -6);
DDL_UNIT_PREFIX_DEFINTION(nano, "nano", "n", -9);
DDL_UNIT_PREFIX_DEFINTION(pico, "pico", "p", -12);
DDL_UNIT_PREFIX_DEFINTION(femto, "femto", "f", -15);
DDL_UNIT_PREFIX_DEFINTION(atto, "atto", "a", -18);
DDL_UNIT_PREFIX_DEFINTION(zepto, "zepto", "z", -21);
DDL_UNIT_PREFIX_DEFINTION(yocto, "yocto", "y", -24);
/// @endcond nodoc

/**
 * @brief Static class for predefined units
 *
 * Following predefined base units can be used:
 * \li unit::Metre::_name - "Metre" - BaseUnit<unit::Metre>>()
 * \li unit::Kilogram::_name - "Kilogram" - BaseUnit<unit::Kilogram>>()
 * \li unit::Second::_name - "Second" - BaseUnit<unit::Second>>()
 * \li unit::Ampere::_name - "Ampere" - BaseUnit<unit::Ampere>>()
 * \li unit::Kelvin::_name - "Kelvin" - BaseUnit<unit::Kelvin>>()
 * \li unit::Mole::_name - "Mole" - BaseUnit<unit::Mole>>()
 * \li unit::Candela::_name - "Candela" - BaseUnit<unit::Candela>>()
 * \li unit::Degree::_name - "Degree" - BaseUnit<unit::Degree>>()
 * \li unit::Radiant::_name - "Radiant" - BaseUnit<unit::Radiant>>()
 * \li unit::Unitless::_name - "Unitless" - BaseUnit<unit::Unitless>>()
 * \li unit::Nou::_name - "nou" - BaseUnit<unit::Nou>>()
 *
 *
 * Following predefined unit prefixes can be used:
 * \li unit_prefix::yotta::_name - "yotta" - UnitPrefix<unit_prefix::yotta>>()
 * \li unit_prefix::zetta::_name - "zetta" - UnitPrefix<unit_prefix::zetta>>()
 * \li unit_prefix::exa::_name - "exa" - UnitPrefix<unit_prefix::exa>>()
 * \li unit_prefix::peta::_name - "peta" - UnitPrefix<unit_prefix::peta>>()
 * \li unit_prefix::tera::_name - "yotta" - UnitPrefix<unit_prefix::tera>>()
 * \li unit_prefix::giga::_name - "giga" - UnitPrefix<unit_prefix::giga>>()
 * \li unit_prefix::mega::_name - "mega" - UnitPrefix<unit_prefix::mega>>()
 * \li unit_prefix::kilo::_name - "kilo" - UnitPrefix<unit_prefix::kilo>>()
 * \li unit_prefix::hecto::_name - "hecto" - UnitPrefix<unit_prefix::hecto>>()
 * \li unit_prefix::deca::_name - "deca" - UnitPrefix<unit_prefix::deca>>()
 * \li unit_prefix::deci::_name - "deci" - UnitPrefix<unit_prefix::deci>>()
 * \li unit_prefix::centi::_name - "centi" - UnitPrefix<unit_prefix::centi>>()
 * \li unit_prefix::milli::_name - "milli" - UnitPrefix<unit_prefix::milli>>()
 * \li unit_prefix::micro::_name - "micro" - UnitPrefix<unit_prefix::micro>>()
 * \li unit_prefix::nano::_name - "nano" - UnitPrefix<unit_prefix::nano>>()
 * \li unit_prefix::micro::_name - "micro" - UnitPrefix<unit_prefix::micro>>()
 * \li unit_prefix::pico::_name - "pico" - UnitPrefix<unit_prefix::pico>>()
 * \li unit_prefix::femto::_name - "femto" - UnitPrefix<unit_prefix::femto>>()
 * \li unit_prefix::atto::_name - "atto" - UnitPrefix<unit_prefix::atto>>()
 * \li unit_prefix::zepto::_name - "zepto" - UnitPrefix<unit_prefix::zepto>>()
 * \li unit_prefix::yocto::_name - "yocto" - UnitPrefix<unit_prefix::yocto>>()
 *
 */
class PredefinedUnits {
private:
    /**
     * @brief Construct a new Predefined Units object
     *
     */
    PredefinedUnits()
    {
        _defined_base_units[unit::Metre::_name] = std::make_shared<BaseUnit<unit::Metre>>();
        _defined_base_units[unit::Kilogram::_name] = std::make_shared<BaseUnit<unit::Kilogram>>();
        _defined_base_units[unit::Second::_name] = std::make_shared<BaseUnit<unit::Second>>();
        _defined_base_units[unit::Ampere::_name] = std::make_shared<BaseUnit<unit::Ampere>>();
        _defined_base_units[unit::Kelvin::_name] = std::make_shared<BaseUnit<unit::Kelvin>>();
        _defined_base_units[unit::Mole::_name] = std::make_shared<BaseUnit<unit::Mole>>();
        _defined_base_units[unit::Candela::_name] = std::make_shared<BaseUnit<unit::Candela>>();
        _defined_base_units[unit::Degree::_name] = std::make_shared<BaseUnit<unit::Degree>>();
        _defined_base_units[unit::Radiant::_name] = std::make_shared<BaseUnit<unit::Radiant>>();
        _defined_base_units[unit::Unitless::_name] = std::make_shared<BaseUnit<unit::Unitless>>();
        _defined_base_units[unit::Nou::_name] = std::make_shared<BaseUnit<unit::Nou>>();

        _defined_unit_prefixes[unit_prefix::yotta::_name] =
            std::make_shared<UnitPrefix<unit_prefix::yotta>>();
        _defined_unit_prefixes[unit_prefix::zetta::_name] =
            std::make_shared<UnitPrefix<unit_prefix::zetta>>();
        _defined_unit_prefixes[unit_prefix::exa::_name] =
            std::make_shared<UnitPrefix<unit_prefix::exa>>();
        _defined_unit_prefixes[unit_prefix::peta::_name] =
            std::make_shared<UnitPrefix<unit_prefix::peta>>();
        _defined_unit_prefixes[unit_prefix::tera::_name] =
            std::make_shared<UnitPrefix<unit_prefix::tera>>();
        _defined_unit_prefixes[unit_prefix::giga::_name] =
            std::make_shared<UnitPrefix<unit_prefix::giga>>();
        _defined_unit_prefixes[unit_prefix::mega::_name] =
            std::make_shared<UnitPrefix<unit_prefix::mega>>();
        _defined_unit_prefixes[unit_prefix::kilo::_name] =
            std::make_shared<UnitPrefix<unit_prefix::kilo>>();
        _defined_unit_prefixes[unit_prefix::hecto::_name] =
            std::make_shared<UnitPrefix<unit_prefix::hecto>>();
        _defined_unit_prefixes[unit_prefix::deca::_name] =
            std::make_shared<UnitPrefix<unit_prefix::deca>>();
        _defined_unit_prefixes[unit_prefix::deci::_name] =
            std::make_shared<UnitPrefix<unit_prefix::deci>>();
        _defined_unit_prefixes[unit_prefix::centi::_name] =
            std::make_shared<UnitPrefix<unit_prefix::centi>>();
        _defined_unit_prefixes[unit_prefix::milli::_name] =
            std::make_shared<UnitPrefix<unit_prefix::milli>>();
        _defined_unit_prefixes[unit_prefix::micro::_name] =
            std::make_shared<UnitPrefix<unit_prefix::micro>>();
        _defined_unit_prefixes[unit_prefix::nano::_name] =
            std::make_shared<UnitPrefix<unit_prefix::nano>>();
        _defined_unit_prefixes[unit_prefix::pico::_name] =
            std::make_shared<UnitPrefix<unit_prefix::pico>>();
        _defined_unit_prefixes[unit_prefix::femto::_name] =
            std::make_shared<UnitPrefix<unit_prefix::femto>>();
        _defined_unit_prefixes[unit_prefix::atto::_name] =
            std::make_shared<UnitPrefix<unit_prefix::atto>>();
        _defined_unit_prefixes[unit_prefix::zepto::_name] =
            std::make_shared<UnitPrefix<unit_prefix::zepto>>();
        _defined_unit_prefixes[unit_prefix::yocto::_name] =
            std::make_shared<UnitPrefix<unit_prefix::yocto>>();
    }

public:
    /**
     * @brief Get the Instance object
     *
     * @return PredefinedUnits&
     */
    static PredefinedUnits& getInstance()
    {
        static PredefinedUnits units;
        return units;
    }
    /**
     * @brief Get the Predefined Base Unit by name
     *
     * @param name the name of the predefined base unit.
     * @return std::shared_ptr<dd::datamodel::BaseUnit>
     */
    std::shared_ptr<dd::datamodel::BaseUnit> getPredefinedBaseUnit(const std::string& name)
    {
        auto found = _defined_base_units.find(name);
        if (found != _defined_base_units.end()) {
            return found->second;
        }
        return {};
    }
    /**
     * @brief Get the Predefined Unit Prefix by name
     *
     * @param name the name of the unit prefix
     * @return std::shared_ptr<dd::datamodel::UnitPrefix>
     */
    std::shared_ptr<dd::datamodel::UnitPrefix> getPredefinedUnitPrefix(const std::string& name)
    {
        auto found = _defined_unit_prefixes.find(name);
        if (found != _defined_unit_prefixes.end()) {
            return found->second;
        }
        return {};
    }

private:
    std::unordered_map<std::string, std::shared_ptr<dd::datamodel::BaseUnit>> _defined_base_units;
    std::unordered_map<std::string, std::shared_ptr<dd::datamodel::UnitPrefix>>
        _defined_unit_prefixes;
};

} // namespace ddl

#endif // DD_PREDEFINED_UNITS_H_INCLUDED