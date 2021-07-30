/**
 * @file
 * DataDefinition Unit definitions
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

#ifndef DD_DATA_MODEL_UNITS_H_INCLUDED
#define DD_DATA_MODEL_UNITS_H_INCLUDED

#include "ddl/datamodel/datamodel_base.h"
#include "ddl/datamodel/infomodel_base.h"
#include "ddl/dd/dd_common_types.h"
#include "ddl/utilities/dd_access_list.h"
#include "ddl/utilities/dd_access_map.h"
#include "ddl/utilities/dd_access_observer.h"

#include <string>
#include <vector>

namespace ddl {

namespace dd {

namespace datamodel {
/**
 * @brief UnitBase is the base class for Unit or BaseUnit
 *
 */
class UnitBase {
public:
    /**
     * @brief default CTOR
     *
     */
    UnitBase() = default;
    /**
     * @brief copy CTOR
     *
     */
    UnitBase(const UnitBase&) = default;
    /**
     * @brief move CTOR
     *
     */
    UnitBase(UnitBase&&) = default;
    /**
     * @brief copy assignment operator
     *
     * @return UnitBase&
     */
    UnitBase& operator=(const UnitBase&) = default;
    /**
     * @brief move assignment operator
     *
     * @return UnitBase&
     */
    UnitBase& operator=(UnitBase&&) = default;
    /**
     * @brief CTOR
     *
     * @param name the name of the unit
     */
    UnitBase(const std::string& name);
    /**
     * @brief DTOR
     *
     */
    virtual ~UnitBase() = default;
    /**
     * @brief Get the Type Of Unit
     *
     * @return TypeOfUnit
     */
    virtual TypeOfUnit getTypeOfUnit() const = 0;
    /**
     * @brief Set the Name
     *
     * @param name the name of the unit
     */
    virtual void setName(const std::string& name);
    /**
     * @brief Get the Name
     *
     * @return const std::string&
     */
    const std::string& getName() const;

protected:
    /// the name
    std::string _name;
};

/**
 * @brief BaseUnit
 *
 */
class BaseUnit : public UnitBase,
                 public ModelSubject<BaseUnit>,
                 public utility::TypeAccessMapSubject<BaseUnit>,
                 public InfoMap {
public:
    /**
     * @brief CTOR
     *
     */
    BaseUnit(){};
    /**
     * @brief copy CTOR
     *
     */
    BaseUnit(const BaseUnit&) = default;
    /**
     * @brief move CTOR
     *
     */
    BaseUnit(BaseUnit&&) = default;
    /**
     * @brief copy assignment operator
     *
     * @return BaseUnit&
     */
    BaseUnit& operator=(const BaseUnit&) = default;
    /**
     * @brief move assignment operator
     *
     * @return BaseUnit&
     */
    BaseUnit& operator=(BaseUnit&&) = default;
    /**
     * @brief CTOR
     *
     * @param name name of the base unit
     * @param symbol symbol of the baseunit
     * @param description description of the baseunit
     */
    BaseUnit(const std::string& name,
             const std::string& symbol,
             const std::string& description = {});
    /**
     * @brief DTOR
     *
     */
    virtual ~BaseUnit() = default;

    TypeOfUnit getTypeOfUnit() const override;
    /**
     * @brief Set the Name
     *
     * @param name the name to set
     * @remark This will notify the change.
     * @throws dd::Error If the name already exists in the container this unit prefix is part of it
     * will throw.
     */
    void setName(const std::string& name) override;

    /**
     * @brief Get the Symbol
     *
     * @return const std::string&
     */
    const std::string& getSymbol() const;
    /**
     * @brief Set the Symbol
     *
     * @param symbol the symbol to set
     * @remark This will notify the change.
     */
    void setSymbol(const std::string& symbol);

    /**
     * @brief Get the Description
     *
     * @return const std::string&
     */
    const std::string& getDescription() const;
    /**
     * @brief Set the Description
     *
     * @param description the description to set
     * @remark This will notify the change.
     */
    void setDescription(const std::string& description);

    /**
     * @brief equality operator.
     *
     * @param other the other baseunit to compare this baseunit to.
     * @return true  the baseunit are equal.
     * @return false the baseunit are not equal.
     */
    bool operator==(const BaseUnit& other) const;
    /**
     * @brief non equality operator.
     *
     * @param other the other baseunit to compare this baseunit to.
     * @return false the baseunit are equal.
     * @return true the baseunit are not equal.
     */
    bool operator!=(const BaseUnit& other) const;

private:
    void notify(const std::string& additional_info);
    std::string _symbol = {};
    std::string _description = {};
};

/**
 * @brief Unit Prefix - datamodel pefixes
 *
 */
class UnitPrefix : public ModelSubject<UnitPrefix>,
                   public utility::TypeAccessMapSubject<UnitPrefix>,
                   public InfoMap {
public:
    /**
     * @brief default CTOR
     *
     */
    UnitPrefix(){};
    /**
     * @brief copy CTOR
     *
     */
    UnitPrefix(const UnitPrefix&) = default;
    /**
     * @brief move CTOR
     *
     */
    UnitPrefix(UnitPrefix&&) = default;
    /**
     * @brief copy assignment operator
     *
     * @return UnitPrefix&
     */
    UnitPrefix& operator=(const UnitPrefix&) = default;
    /**
     * @brief move assignment operator
     *
     * @return UnitPrefix&
     */
    UnitPrefix& operator=(UnitPrefix&&) = default;
    /**
     * @brief CTOR
     *
     * @param name name of the unit prefix
     * @param symbol symbol of the unit prefix
     * @param power power of the unit prefix
     */
    UnitPrefix(const std::string& name, const std::string& symbol, int32_t power);
    /**
     * @brief DTOR
     *
     */
    virtual ~UnitPrefix() = default;

    /**
     * @brief Get the Name
     *
     * @return const std::string&
     */
    const std::string& getName() const;
    /**
     * @brief Set the Name
     *
     * @param name the name to set
     * @remark This will notify the change.
     * @throws dd::Error If the name already exists in the container this unit prefix is part of it
     * will throw.
     */
    void setName(const std::string& name);

    /**
     * @brief Get the Symbol
     *
     * @return const std::string&
     */
    const std::string& getSymbol() const;
    /**
     * @brief Set the Symbol
     *
     * @param symbol the symbol to set
     * @remark This will notify the change.
     */
    void setSymbol(const std::string& symbol);

    /**
     * @brief Get the Power
     *
     * @return int32_t
     */
    int32_t getPower() const;
    /**
     * @brief Set the Power
     *
     * @param power the power to set
     * @remark This will notify the change.
     */
    void setPower(int32_t power);
    /**
     * @brief equality operator.
     *
     * @param other the other unitprefix to compare this unitprefix to.
     * @return true  the unitprefixes are equal.
     * @return false the unitprefixes are not equal.
     */
    bool operator==(const UnitPrefix& other) const;
    /**
     * @brief non equality operator.
     *
     * @param other the other unitprefix to compare this unitprefix to.
     * @return false  the unitprefixes are equal.
     * @return true the unitprefixes are not equal.
     */
    bool operator!=(const UnitPrefix& other) const;

private:
    void notify(const std::string& additional_info);
    std::string _name = {};
    std::string _symbol = {};
    int32_t _power = {};
};

/**
 * @brief BaseUnit
 *
 */
class Unit : public UnitBase,
             public ModelSubject<Unit>,
             public utility::TypeAccessMapSubject<Unit>,
             public InfoMap {
public:
    /**
     * @brief CTOR
     *
     */
    Unit();
    /**
     * @brief copy CTOR
     *
     */
    Unit(const Unit&) = default;
    /**
     * @brief copy assignment operator
     *
     * @return Unit&
     */
    Unit& operator=(const Unit&) = default;
    /**
     * @brief move CTOR
     *
     */
    Unit(Unit&&) = default;
    /**
     * @brief move assignment operator
     *
     * @return Unit&
     */
    Unit& operator=(Unit&&) = default;
    /**
     * @brief DTOR
     *
     */
    virtual ~Unit() = default;

    /**
     * @brief datamodel for the refUnit
     *
     */
    class RefUnit {
    public:
        /**
         * @brief default CTOR
         *
         */
        RefUnit(){};
        /**
         * @brief copy CTOR
         *
         */
        RefUnit(const RefUnit&) = default;
        /**
         * @brief move CTOR
         *
         */
        RefUnit(RefUnit&&) = default;
        /**
         * @brief copy assignment operator
         *
         * @return RefUnit&
         */
        RefUnit& operator=(const RefUnit&) = default;
        /**
         * @brief move assignment operator
         *
         * @return RefUnit&
         */
        RefUnit& operator=(RefUnit&&) = default;
        /**
         * @brief CTOR
         *
         * @param unit_name the refunit name (must be a base unit!)
         * @param power the power of the refunit
         * @param prefix_name the prefix name (must be a unit prefix)
         */
        RefUnit(const std::string& unit_name, int32_t power, const std::string& prefix_name);
        /**
         * @brief equality operator.
         *
         * @param other the other refunit to compare this refunit to.
         * @return true  the refunit are equal.
         * @return false the refunit are not equal.
         */
        bool operator==(const RefUnit& other) const;
        /**
         * @brief non equality operator.
         *
         * @param other the other refunit to compare this refunit to.
         * @return false  the refunit are equal.
         * @return true the refunit are not equal.
         */
        bool operator!=(const RefUnit& other) const;

        /**
         * @brief Get the Unit Name
         *
         * @return const std::string&
         */
        const std::string& getUnitName() const;
        /**
         * @brief Get the Power
         *
         * @return int32_t
         */
        int32_t getPower() const;
        /**
         * @brief Get the Prefix Name
         *
         * @return const std::string&
         */
        const std::string& getPrefixName() const;

    private:
        std::string _unit_name = {};
        int32_t _power = {};
        std::string _prefix_name = {};
    };
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
    Unit(const std::string& name,
         const std::string& numerator,
         const std::string& denominator,
         const std::string& offset,
         const std::vector<RefUnit>& ref_units = {});
    /**
     * @brief equality operator.
     *
     * @param other the other unit to compare this unit to.
     * @return true  the unit are equal (and all its refunits)
     * @return false the unit are not equal.
     */
    bool operator==(const Unit& other) const;
    /**
     * @brief non equality operator.
     *
     * @param other the other unit to compare this unit to.
     * @return false  the unit are equal (and all its refunits)
     * @return true the unit are not equal.
     */
    bool operator!=(const Unit& other) const;

    /**
     * @brief Get the Type Of Unit
     *
     * @return TypeOfUnit
     */
    TypeOfUnit getTypeOfUnit() const override;
    /**
     * @brief Set the Name
     *
     * @param name the name to set
     * @remark This will notify the change.
     * @throws dd::Error If the name already exists in the container this unit is part of it will
     * throw.
     */
    void setName(const std::string& name) override;
    /**
     * @brief Get the Numerator
     *
     * @return const std::string&
     */
    const std::string& getNumerator() const;
    /**
     * @brief Set the Numerator object
     *
     * @param numerator the numerator to set
     * @remark This will notify the change.
     */
    void setNumerator(const std::string& numerator);

    /**
     * @brief Get the Denominator
     *
     * @return const std::string&
     */
    const std::string& getDenominator() const;
    /**
     * @brief Set the Denominator
     *
     * @param denominator the denominator to set
     * @remark This will notify the change.
     */
    void setDenominator(const std::string& denominator);

    /**
     * @brief Get the Offset
     *
     * @return const std::string&
     */
    const std::string& getOffset() const;
    /**
     * @brief Set the Offset
     *
     * @param offset the offset to set
     * @remark This will notify the change.
     */
    void setOffset(const std::string& offset);

public:
    /**
     * @brief definiton of the container for the RefUnits
     *
     */
    using RefUnits = std::vector<RefUnit>;
    /**
     * @brief return the refUnits container
     *
     * @return const RefUnits&
     */
    const RefUnits& getRefUnits() const;

    /**
     * @brief adding a refunit
     *
     * @param ref_unit the refunit to add
     * @throws dd::Error this will throw if this refUnits already contains a base_unit/prefix pair
     */
    void addRefUnit(const RefUnit& ref_unit);
    /**
     * @brief remove the refunit
     *
     * @param ref_unit the refunit to remove
     */
    void removeRefUnit(const RefUnit& ref_unit);

private:
    void notify(ModelEventCode code,
                utility::TypeAccessMapEventCode map_code,
                const std::string& additional_info);

private:
    std::string _numerator = {};
    std::string _denominator = {};
    std::string _offset = {};
    RefUnits _ref_units;
};

} // namespace datamodel
} // namespace dd
} // namespace ddl

#endif // DD_DATA_MODEL_UNITS_H_INCLUDED
