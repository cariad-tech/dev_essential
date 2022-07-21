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

#include <ddl/datamodel/datamodel_units.h>

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
#define DDL_BASE_UNIT_DEFINITION(                                                                  \
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
#define DDL_UNIT_PREFIX_DEFINITION(                                                                \
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
DDL_BASE_UNIT_DEFINITION(Metre, "Metre", "m", "Fundamental unit for length");
DDL_BASE_UNIT_DEFINITION(Kilogram, "Kilogram", "kg", "Fundamental unit for mass");
DDL_BASE_UNIT_DEFINITION(Second, "Second", "s", "Fundamental unit for time");
DDL_BASE_UNIT_DEFINITION(Ampere, "Ampere", "A", "Fundamental unit for electric current");
DDL_BASE_UNIT_DEFINITION(Kelvin, "Kelvin", "K", "Fundamental unit for temperature");
DDL_BASE_UNIT_DEFINITION(Mole, "Mole", "mol", "Fundamental unit for amount of substance");
DDL_BASE_UNIT_DEFINITION(Candela, "Candela", "cd", "Fundamental unit for luminous intensity");
DDL_BASE_UNIT_DEFINITION(Degree, "Degree", "deg", "Non-SI standard unit for angle");
DDL_BASE_UNIT_DEFINITION(Radiant, "Radiant", "rad", "Non-SI standard unit for angle");
DDL_BASE_UNIT_DEFINITION(Unitless, "Unitless", "", "No SI, but needed for own unit definitions");
DDL_BASE_UNIT_DEFINITION(Nou, "nou", "", "No SI, but needed for no unit definitions");

DDL_UNIT_PREFIX_DEFINITION(yotta, "yotta", "Y", 24);
DDL_UNIT_PREFIX_DEFINITION(zetta, "zetta", "Z", 21);
DDL_UNIT_PREFIX_DEFINITION(exa, "exa", "E", 18);
DDL_UNIT_PREFIX_DEFINITION(peta, "peta", "P", 15);
DDL_UNIT_PREFIX_DEFINITION(tera, "tera", "T", 12);
DDL_UNIT_PREFIX_DEFINITION(giga, "giga", "G", 9);
DDL_UNIT_PREFIX_DEFINITION(mega, "mega", "M", 6);
DDL_UNIT_PREFIX_DEFINITION(kilo, "kilo", "k", 3);
DDL_UNIT_PREFIX_DEFINITION(hecto, "hecto", "h", 2);
DDL_UNIT_PREFIX_DEFINITION(deca, "deca", "da", 1);
DDL_UNIT_PREFIX_DEFINITION(deci, "deci", "d", -1);
DDL_UNIT_PREFIX_DEFINITION(centi, "centi", "c", -2);
DDL_UNIT_PREFIX_DEFINITION(milli, "mili", "m", -3);
DDL_UNIT_PREFIX_DEFINITION(micro, "micro", "u", -6);
DDL_UNIT_PREFIX_DEFINITION(nano, "nano", "n", -9);
DDL_UNIT_PREFIX_DEFINITION(pico, "pico", "p", -12);
DDL_UNIT_PREFIX_DEFINITION(femto, "femto", "f", -15);
DDL_UNIT_PREFIX_DEFINITION(atto, "atto", "a", -18);
DDL_UNIT_PREFIX_DEFINITION(zepto, "zepto", "z", -21);
DDL_UNIT_PREFIX_DEFINITION(yocto, "yocto", "y", -24);
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
    PredefinedUnits();
    /**
     * @brief Destructor
     *
     */
    ~PredefinedUnits();

public:
    /**
     * @brief Get the Instance object
     *
     * @return PredefinedUnits&
     */
    static const PredefinedUnits& getInstance();
    /**
     * @brief Get the Predefined Base Unit by name
     *
     * @param name the name of the predefined base unit.
     * @return std::shared_ptr<dd::datamodel::BaseUnit>
     */
    std::shared_ptr<dd::datamodel::BaseUnit> getPredefinedBaseUnit(const std::string& name) const;
    /**
     * @brief Get the Predefined Unit Prefix by name
     *
     * @param name the name of the unit prefix
     * @return std::shared_ptr<dd::datamodel::UnitPrefix>
     */
    std::shared_ptr<dd::datamodel::UnitPrefix> getPredefinedUnitPrefix(
        const std::string& name) const;

    /**
     * @brief Gets a vector of all the predefined base units.
     *
     * @return std::vector<std::shared_ptr<dd::datamodel::BaseUnit>> The predefined base units.
     */
    std::vector<std::shared_ptr<dd::datamodel::BaseUnit>> getPredefinedBaseUnits() const;
    /**
     * @brief Gets a vector of all the predefined unit prefixes types.
     *
     * @return std::vector<std::shared_ptr<dd::datamodel::UnitPrefix>> The predefined unit prefixes.
     */
    std::vector<std::shared_ptr<dd::datamodel::UnitPrefix>> getPredefinedUnitPrefixes() const;

private:
    const std::unordered_map<std::string, std::shared_ptr<dd::datamodel::BaseUnit>>
        _defined_base_units;
    const std::unordered_map<std::string, std::shared_ptr<dd::datamodel::UnitPrefix>>
        _defined_unit_prefixes;
};

} // namespace ddl

#endif // DD_PREDEFINED_UNITS_H_INCLUDED
