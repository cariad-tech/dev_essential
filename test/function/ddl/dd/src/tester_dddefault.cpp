/**
 * @file
 * Implementation of the tester for the DataDefinition representation.
 *
 * Copyright @ 2021 VW Group. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "./../../_common/test_oo_ddl.h"

#include <a_util/result.h>
#include <ddl/dd/dd_predefined_units.h>
#include <ddl/dd/dddefault.h>
#include <ddl/dd/ddfile.h>

#include <gtest/gtest.h>

#include <cstdio>

void checkForDDLTypes(const ddl::dd::DataDefinition& dd)
{
    // test if all expected ddl datatypes are the
    test_ddl::containsDataType(dd, ddl::DataType<bool>("tBool"));
    test_ddl::containsDataType(dd, ddl::DataType<char>("tChar"));
    test_ddl::containsDataType(dd, ddl::DataType<uint8_t>("tUInt8"));
    test_ddl::containsDataType(dd, ddl::DataType<int8_t>("tInt8"));
    test_ddl::containsDataType(dd, ddl::DataType<uint16_t>("tUInt16"));
    test_ddl::containsDataType(dd, ddl::DataType<int16_t>("tInt16"));
    test_ddl::containsDataType(dd, ddl::DataType<uint32_t>("tUInt32"));
    test_ddl::containsDataType(dd, ddl::DataType<int32_t>("tInt32"));
    test_ddl::containsDataType(dd, ddl::DataType<uint64_t>("tUInt64"));
    test_ddl::containsDataType(dd, ddl::DataType<int64_t>("tInt64"));
    test_ddl::containsDataType(dd, ddl::DataType<float>("tFloat32"));
    test_ddl::containsDataType(dd, ddl::DataType<double>("tFloat64"));

    // test if all expected ddl datatypes are the
    // we check the same ( but use only default names!)
    test_ddl::containsDataType(dd, ddl::DataType<bool>());
    test_ddl::containsDataType(dd, ddl::DataType<char>());
    test_ddl::containsDataType(dd, ddl::DataType<uint8_t>());
    test_ddl::containsDataType(dd, ddl::DataType<int8_t>());
    test_ddl::containsDataType(dd, ddl::DataType<uint16_t>());
    test_ddl::containsDataType(dd, ddl::DataType<int16_t>());
    test_ddl::containsDataType(dd, ddl::DataType<uint32_t>());
    test_ddl::containsDataType(dd, ddl::DataType<int32_t>());
    test_ddl::containsDataType(dd, ddl::DataType<uint64_t>());
    test_ddl::containsDataType(dd, ddl::DataType<int64_t>());
    test_ddl::containsDataType(dd, ddl::DataType<float>());
    test_ddl::containsDataType(dd, ddl::DataType<double>());
}

void checkForStdTypes(const ddl::dd::DataDefinition& dd)
{
    using namespace ddl;

    // test if all expected std datatypes are the
    test_ddl::containsDataType(dd, DataType<bool>("bool"));
    test_ddl::containsDataType(dd, DataType<char>("char"));
    test_ddl::containsDataType(dd, DataType<uint8_t>("uint8_t"));
    test_ddl::containsDataType(dd, DataType<int8_t>("int8_t"));
    test_ddl::containsDataType(dd, DataType<uint16_t>("uint16_t"));
    test_ddl::containsDataType(dd, DataType<int16_t>("int16_t"));
    test_ddl::containsDataType(dd, DataType<uint32_t>("uint32_t"));
    test_ddl::containsDataType(dd, DataType<int32_t>("int32_t"));
    test_ddl::containsDataType(dd, DataType<uint64_t>("uint64_t"));
    test_ddl::containsDataType(dd, DataType<int64_t>("int64_t"));
    test_ddl::containsDataType(dd, DataType<float>("float"));
    test_ddl::containsDataType(dd, DataType<double>("double"));
}

void checkForBaseUnitsAndPrefixes(const ddl::dd::DataDefinition& dd)
{
    using namespace ddl;
    EXPECT_EQ(dd.getBaseUnits().getSize(), 11U);
    EXPECT_EQ(dd.getUnitPrefixes().getSize(), 20U);

    test_ddl::containsBaseUnit(dd, BaseUnit<unit::Metre>());
    test_ddl::containsBaseUnit(dd, BaseUnit<unit::Kilogram>());
    test_ddl::containsBaseUnit(dd, BaseUnit<unit::Second>());
    test_ddl::containsBaseUnit(dd, BaseUnit<unit::Ampere>());
    test_ddl::containsBaseUnit(dd, BaseUnit<unit::Kelvin>());
    test_ddl::containsBaseUnit(dd, BaseUnit<unit::Mole>());
    test_ddl::containsBaseUnit(dd, BaseUnit<unit::Candela>());
    test_ddl::containsBaseUnit(dd, BaseUnit<unit::Degree>());
    test_ddl::containsBaseUnit(dd, BaseUnit<unit::Radiant>());
    test_ddl::containsBaseUnit(dd, BaseUnit<unit::Unitless>());
    test_ddl::containsBaseUnit(dd, BaseUnit<unit::Nou>());

    test_ddl::containsUnitPrefix(dd, UnitPrefix<unit_prefix::yotta>());
    test_ddl::containsUnitPrefix(dd, UnitPrefix<unit_prefix::zetta>());
    test_ddl::containsUnitPrefix(dd, UnitPrefix<unit_prefix::exa>());
    test_ddl::containsUnitPrefix(dd, UnitPrefix<unit_prefix::peta>());
    test_ddl::containsUnitPrefix(dd, UnitPrefix<unit_prefix::tera>());
    test_ddl::containsUnitPrefix(dd, UnitPrefix<unit_prefix::giga>());
    test_ddl::containsUnitPrefix(dd, UnitPrefix<unit_prefix::mega>());
    test_ddl::containsUnitPrefix(dd, UnitPrefix<unit_prefix::kilo>());
    test_ddl::containsUnitPrefix(dd, UnitPrefix<unit_prefix::hecto>());
    test_ddl::containsUnitPrefix(dd, UnitPrefix<unit_prefix::deca>());
    test_ddl::containsUnitPrefix(dd, UnitPrefix<unit_prefix::deci>());
    test_ddl::containsUnitPrefix(dd, UnitPrefix<unit_prefix::centi>());
    test_ddl::containsUnitPrefix(dd, UnitPrefix<unit_prefix::milli>());
    test_ddl::containsUnitPrefix(dd, UnitPrefix<unit_prefix::micro>());
    test_ddl::containsUnitPrefix(dd, UnitPrefix<unit_prefix::nano>());
    test_ddl::containsUnitPrefix(dd, UnitPrefix<unit_prefix::pico>());
    test_ddl::containsUnitPrefix(dd, UnitPrefix<unit_prefix::femto>());
    test_ddl::containsUnitPrefix(dd, UnitPrefix<unit_prefix::atto>());
    test_ddl::containsUnitPrefix(dd, UnitPrefix<unit_prefix::zepto>());
    test_ddl::containsUnitPrefix(dd, UnitPrefix<unit_prefix::yocto>());
}

/**
 * @detail Create a data definition with all default datatypes and units
 */
TEST(TesterDDDefault, createAllPredefinedTypes)
{
    using namespace ddl::dd;
    DataDefinition created_dd;

    ASSERT_NO_THROW(created_dd = ddl::DDDefault::create());

    EXPECT_EQ(created_dd.getDataTypes().getSize(), 24U);

    checkForDDLTypes(created_dd);
    checkForStdTypes(created_dd);

    EXPECT_EQ(created_dd.getStructTypes().getSize(), 0U);
    EXPECT_EQ(created_dd.getStreamMetaTypes().getSize(), 0U);
    EXPECT_EQ(created_dd.getEnumTypes().getSize(), 0U);
    EXPECT_EQ(created_dd.getStreams().getSize(), 0U);

    checkForBaseUnitsAndPrefixes(created_dd);

    EXPECT_EQ(created_dd.getUnits().getSize(), 0U);
}

/**
 * @detail Create a data definition with all default datatypes and units, check the content of a
 *         written file if also all expected xml tags are set like expected.
 */
TEST(TesterDDDefault, createAllPredefinedTypesAndCheckFileExpected)
{
    using namespace ddl::dd;
    DataDefinition created_dd;

    ASSERT_NO_THROW(created_dd = ddl::DDDefault::create());

    std::string default_file_created = TEST_FILES_WRITE_DIR "default_created.description";
    std::string default_file_expected = TEST_FILES_DIR "default_expected.description";

    std::remove(default_file_created.c_str());
    ASSERT_NO_THROW(ddl::DDFile::toXMLFile(created_dd, default_file_created);)
        << "Write default file failed.";

    ASSERT_EQ(a_util::result::SUCCESS,
              test_ddl::CompOutput(default_file_created, default_file_expected));
}

/**
 * @detail Create a data definition with only std datatypes and units
 */
TEST(TesterDDDefault, createStdPredefinedTypes)
{
    using namespace ddl::dd;
    DataDefinition created_dd;

    ASSERT_NO_THROW(created_dd =
                        ddl::DDDefault::create(ddl::DDDefault::CreateOption::use_std_types_only));

    EXPECT_EQ(created_dd.getDataTypes().getSize(), 12U);

    checkForStdTypes(created_dd);

    EXPECT_EQ(created_dd.getStructTypes().getSize(), 0U);
    EXPECT_EQ(created_dd.getStreamMetaTypes().getSize(), 0U);
    EXPECT_EQ(created_dd.getEnumTypes().getSize(), 0U);
    EXPECT_EQ(created_dd.getStreams().getSize(), 0U);

    checkForBaseUnitsAndPrefixes(created_dd);

    EXPECT_EQ(created_dd.getUnits().getSize(), 0U);
}

/**
 * @detail Create a data definition with only ddl datatypes and units
 */
TEST(TesterDDDefault, createDDLPredefinedTypes)
{
    using namespace ddl::dd;
    DataDefinition created_dd;

    ASSERT_NO_THROW(created_dd =
                        ddl::DDDefault::create(ddl::DDDefault::CreateOption::use_ddl_types_only));

    EXPECT_EQ(created_dd.getDataTypes().getSize(), 12U);

    checkForDDLTypes(created_dd);

    EXPECT_EQ(created_dd.getStructTypes().getSize(), 0U);
    EXPECT_EQ(created_dd.getStreamMetaTypes().getSize(), 0U);
    EXPECT_EQ(created_dd.getEnumTypes().getSize(), 0U);
    EXPECT_EQ(created_dd.getStreams().getSize(), 0U);

    checkForBaseUnitsAndPrefixes(created_dd);

    EXPECT_EQ(created_dd.getUnits().getSize(), 0U);
}
