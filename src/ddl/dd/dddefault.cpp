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

#include <ddl/dd/dd_predefined_datatypes.h>
#include <ddl/dd/dd_predefined_units.h>
#include <ddl/dd/dddefault.h>

namespace ddl {

dd::DataDefinition DDDefault::create(DDDefault::CreateOption create_option)
{
    dd::DataDefinition created_dd;
    // add the datatypes to the ddl depending on the create option
    const auto predefined_types = PredefinedDataTypes::getInstance().getPredefinedTypes();
    for (const auto& predefined_type: predefined_types) {
        if (create_option == CreateOption::use_ddl_types_only) {
            if (predefined_type->getName()[0] == 't') {
                created_dd.getDataTypes().add(*predefined_type.get());
            }
        }
        else if (create_option == CreateOption::use_std_types_only) {
            if (predefined_type->getName()[0] != 't') {
                created_dd.getDataTypes().add(*predefined_type.get());
            }
        }
        else {
            created_dd.getDataTypes().add(*predefined_type.get());
        }
    }
    // add the base units to the ddl
    const auto predefined_base_units = PredefinedUnits::getInstance().getPredefinedBaseUnits();
    for (const auto& predefined_base_unit: predefined_base_units) {
        created_dd.getBaseUnits().add(*predefined_base_unit.get());
    }

    // add the unit prefixes to the ddl
    const auto predefined_unit_prefixes =
        PredefinedUnits::getInstance().getPredefinedUnitPrefixes();
    for (const auto& predefined_unit_prefix: predefined_unit_prefixes) {
        created_dd.getUnitPrefixes().add(*predefined_unit_prefix.get());
    }

    return created_dd;
}

} // namespace ddl
