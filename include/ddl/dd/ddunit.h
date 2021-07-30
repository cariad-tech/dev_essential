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

#ifndef DDLUNIT_H_INCLUDED
#define DDLUNIT_H_INCLUDED

#include "ddl/dd/dd.h"
#include "ddl/dd/dd_predefined_units.h"

#include <memory>
#include <string>

namespace ddl {

/**
 * @brief The DDRefunit class holding a reference to its complete DD (references to baseunit and
 * prefix)
 *
 */
class DDRefUnit {
public:
    /**
     * @brief no default CTOR
     *
     */
    DDRefUnit() = delete;
    /**
     * @brief copy CTOR
     *
     */
    DDRefUnit(const DDRefUnit&) = default;
    /**
     * @brief move CTOR
     *
     */
    DDRefUnit(DDRefUnit&&) = default;
    /**
     * @brief copy assignment operator
     *
     * @return DDRefUnit&
     */
    DDRefUnit& operator=(const DDRefUnit&) = default;
    /**
     * @brief move assignment operator
     *
     * @return DDRefUnit&
     */
    DDRefUnit& operator=(DDRefUnit&&) = default;

    /**
     * @brief CTOR
     *
     * @param base_unit the base_unit
     * @param power the power of the refunit
     * @param unit_prefix the prefix
     */
    DDRefUnit(const dd::BaseUnit& base_unit, int32_t power, const dd::UnitPrefix& unit_prefix);
    /**
     * @brief Get the Ref Unit object
     *
     * @return const dd::Unit::RefUnit&
     */
    const dd::Unit::RefUnit& getRefUnit() const;
    /**
     * @brief Get the data definition
     *
     * @return const dd::DataDefinition&
     */
    const dd::DataDefinition& getDD() const;

private:
    dd::Unit::RefUnit _ref_unit;
    dd::DataDefinition _dd;
};

/**
 * @brief The unit class holding a reference to its complete DD (references to baseunits and
 * prefixes)
 *
 */
class DDUnit {
public:
    /**
     * @brief no default CTOR
     *
     */
    DDUnit() = delete;
    /**
     * @brief copy CTOR
     *
     */
    DDUnit(const DDUnit&) = default;
    /**
     * @brief move CTOR
     *
     */
    DDUnit(DDUnit&&) = default;
    /**
     * @brief copy assignment operator
     *
     * @return DDUnit&
     */
    DDUnit& operator=(const DDUnit&) = default;
    /**
     * @brief move assignment operator
     *
     * @return DDUnit&
     */
    DDUnit& operator=(DDUnit&&) = default;

    /**
     * @brief CTOR
     *
     * @param name the name of the unit
     * @param numerator the numerator for the unit
     * @param denominator the denominator for the unit
     * @param offset the offset for the unit
     * @param ref_units vector of refunits
     * @throws dd::Error this will throw if this list contains a base_unit/prefix pair twice
     */
    DDUnit(const std::string& name,
           const std::string& numerator,
           const std::string& denominator,
           const std::string& offset,
           const std::vector<DDRefUnit>& ref_units = {});

    /**
     * @brief adding a refunit
     *
     * @param ref_unit the refunit to add
     * @throws dd::Error this will throw if this refUnits already contains a base_unit/prefix pair
     */
    void addRefUnit(const DDRefUnit& ref_unit);

    /**
     * @brief Get the Unit object
     *
     * @return const dd::Unit&
     */
    const dd::Unit& getUnit() const;
    /**
     * @brief Get the data definiton
     *
     * @return const dd::DataDefinition&
     */
    const dd::DataDefinition& getDD() const;

private:
    std::shared_ptr<dd::Unit> _unit;
    dd::DataDefinition _dd;
};

} // namespace ddl

#endif // DDLUNIT_H_INCLUDED