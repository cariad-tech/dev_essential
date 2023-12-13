/**
 * @file
 * Implementation of a static test library to force the static variable initialization fiasko
 *
 * Copyright @ 2022 VW Group. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include "static_ddl_version_test_lib.h"

const static auto sub_definition_from_library_1 =
    ddl::DDStructureGenerator<static_sub_struct>("static_sub_struct")
        .addElement("value1", &static_sub_struct::value1)
        .addElement("value2", &static_sub_struct::value2)
        .addElement("value3", &static_sub_struct::value3);

const static auto sub_definition_from_library_2 = ddl::DDStructure("static_sub_struct")
                                                      .addElement<uint16_t>("value1")
                                                      .addElement<uint32_t>("value2")
                                                      .addElement<uint8_t>("value3");

const static auto dd_default_version_inst = ddl::dd::DataDefinition();
const static auto dd_30_version_inst = ddl::dd::DataDefinition(ddl::dd::Version(3, 0));

const ddl::DDStructureGenerator<static_sub_struct>& getTestStructure1()
{
    return sub_definition_from_library_1;
}

const ddl::DDStructure& getTestStructure2()
{
    return sub_definition_from_library_2;
}

const ddl::DataDefinition& getDefaultTestDD()
{
    return dd_default_version_inst;
}

const ddl::DataDefinition& getTestDD30()
{
    return dd_30_version_inst;
}