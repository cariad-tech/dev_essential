/**
 * @file
 * Header of a static test library to force the static variable initialization fiasko
 *
 * Copyright @ 2022 VW Group. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef TEST_STATIC_DDL_VERSION_TEST_LIB_HEADER
#define TEST_STATIC_DDL_VERSION_TEST_LIB_HEADER

#include <ddl/dd/ddstructure.h>

struct static_sub_struct {
    uint16_t value1;
    uint32_t value2;
    uint8_t value3;
};

const ddl::DDStructureGenerator<static_sub_struct>& getTestStructure1();
const ddl::DDStructure& getTestStructure2();

const ddl::DataDefinition& getDefaultTestDD();
const ddl::DataDefinition& getTestDD30();

#endif // TEST_STATIC_DDL_VERSION_TEST_LIB_HEADER