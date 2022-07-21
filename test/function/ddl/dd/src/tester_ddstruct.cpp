/**
 * @file
 * Implementation of the tester for the DataDefinition representation.
 *
 * Copyright @ 2021 VW Group. All rights reserved.
 *
 *     This Source Code Form is subject to the terms of the Mozilla
 *     Public License, v. 2.0. If a copy of the MPL was not distributed
 *     with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * If it is not possible or desirable to put the notice in a particular file, then
 * You may include the notice in a location (such as a LICENSE file in a
 * relevant directory) where a recipient would be likely to look for such a notice.
 *
 * You may add additional accurate notices of copyright ownership.
 *
 */

#include "./../../_common/test_oo_ddl.h"

#include <ddl/dd/ddcompare.h>
#include <ddl/dd/ddstring.h>
#include <ddl/dd/ddstructure.h>

#include <gtest/gtest.h>

/**
 * @detail The building up of a DataDefinition object representation.
 */
TEST(cTesterDDLStruct, createDDLStruct)
{
    using namespace ddl;
    using namespace test_ddl;

    DDEnum e1("e1", int8_data_type(), {{"val11", "1"}, {"val12", "2"}, {"val13", "3"}});
    DDStructure a("A");
    a.addElement<int32_t>("elem1");
    a.addElement("elem2", uint16_data_type());
    a.addElements({DDElement("elem3", uint64_data_type()), DDElement("elem4", float_data_type())});
    a.addElements({{"elem5", double_data_type()}, {"elem6", bool_data_type()}, {"elem7", e1}});
    a.addElement("elem8", uint16_data_type("uint16_t"));

    // following DataTypes must exists
    ASSERT_TRUE(test_ddl::containsDataType(a.getDD(), uint16_data_type()));
    ASSERT_TRUE(test_ddl::containsDataType(a.getDD(), int32_data_type()));
    ASSERT_TRUE(test_ddl::containsDataType(a.getDD(), uint64_data_type()));
    ASSERT_TRUE(test_ddl::containsDataType(a.getDD(), float_data_type()));
    ASSERT_TRUE(test_ddl::containsDataType(a.getDD(), double_data_type()));
    ASSERT_TRUE(test_ddl::containsDataType(a.getDD(), bool_data_type()));
    ASSERT_TRUE(test_ddl::containsDataType(a.getDD(), uint16_data_type("uint16_t")));

    // from enum 1
    ASSERT_TRUE(test_ddl::containsDataType(a.getDD(), int8_data_type()));
    ASSERT_TRUE(test_ddl::containsEnumType(a.getDD(), e1.getEnumType()));

    // find struct with a simple test
    std::vector<std::pair<std::string, std::string>> a_elements = {{"elem1", "tInt32"},
                                                                   {"elem2", "tUInt16"},
                                                                   {"elem3", "tUInt64"},
                                                                   {"elem4", "tFloat32"},
                                                                   {"elem5", "tFloat64"},
                                                                   {"elem6", "tBool"},
                                                                   {"elem7", "e1"},
                                                                   {"elem8", "uint16_t"}};
    ASSERT_TRUE(test_ddl::containsStructType(*a.getDD().getModel(), "A", a_elements));

    DDStructure b("B");
    b.addElement("nested1", a);
    b.addElement("nested2", a);

    DDEnum e2("e2", int64_data_type(), {{"val21", "1"}, {"val22", "2"}, {"val23", "3"}});
    b.addElement("enum_elem", e2, "val23");

    std::vector<std::pair<std::string, std::string>> b_elements = {
        {"nested1", "A"}, {"nested2", "A"}, {"enum_elem", "e2"}};

    // following DataTypes must exists from the A struct
    ASSERT_TRUE(test_ddl::containsDataType(b.getDD(), uint16_data_type()));
    ASSERT_TRUE(test_ddl::containsDataType(b.getDD(), int32_data_type()));
    ASSERT_TRUE(test_ddl::containsDataType(b.getDD(), uint64_data_type()));
    ASSERT_TRUE(test_ddl::containsDataType(b.getDD(), float_data_type()));
    ASSERT_TRUE(test_ddl::containsDataType(b.getDD(), double_data_type()));
    ASSERT_TRUE(test_ddl::containsDataType(b.getDD(), bool_data_type()));
    ASSERT_TRUE(test_ddl::containsDataType(b.getDD(), uint16_data_type("uint16_t")));

    // from the enums out of A is added to B
    ASSERT_TRUE(test_ddl::containsDataType(b.getDD(), int64_data_type()));
    ASSERT_TRUE(test_ddl::containsEnumType(b.getDD(), e1.getEnumType()));

    // the emum e2 was use, this is added and its data type
    ASSERT_TRUE(test_ddl::containsDataType(b.getDD(), int8_data_type()));
    ASSERT_TRUE(test_ddl::containsEnumType(b.getDD(), e2.getEnumType()));

    // struct description A must be in there
    ASSERT_TRUE(test_ddl::containsStructType(*b.getDD().getModel(), "A", a_elements));

    // struct description B must be in there
    ASSERT_TRUE(test_ddl::containsStructType(*b.getDD().getModel(), "B", b_elements));

    // the Struct Description A is fully the same then the A added in B
    ASSERT_EQ(*(b.getDD().getStructTypes().get("A")), (a.getStructType()));

    DDStructure c("C");
    c.addElement(DDElement("nested1", a));
    // the Struct Description A is fully the same then the A added in C via element adder
    ASSERT_EQ(*(c.getDD().getStructTypes().get("A")), (a.getStructType()));
}

/**
 * @detail The building up of a DataDefinition object representation.
 */
TEST(cTesterDDLStructure, createDDLStructureWithUnitsInDataType)
{
    using namespace ddl;
    using namespace test_ddl;

    DDDataType type1(
        "type1_baseunit", 32, "description", 1, "min", "max", {}, BaseUnit<unit::Metre>());
    DDUnit unit2("unit_for_type2_unit",
                 "1",
                 "2",
                 "3",
                 {DDRefUnit(BaseUnit<unit::Ampere>(), 1, UnitPrefix<unit_prefix::atto>()),
                  DDRefUnit(BaseUnit<unit::Candela>(), 32, UnitPrefix<unit_prefix::centi>())});

    DDUnit unit3("unit_for_type3_unit",
                 "1",
                 "2",
                 "3",
                 {DDRefUnit(BaseUnit<unit::Degree>(), 1, UnitPrefix<unit_prefix::deca>()),
                  DDRefUnit(BaseUnit<unit::Kelvin>(), 32, UnitPrefix<unit_prefix::deci>())});
    DDDataType type2("type2_unit", 32, "description", 1, "min", "max", {}, unit2);
    DDDataType type3("type3_unit", 32, "description", 1, "min", "max", {}, unit3);

    DDEnum e1("e1", type2, {{"val11", "1"}, {"val12", "2"}, {"val13", "3"}});

    DDStructure a("A");
    a.addElement("elem1", type1);
    a.addElement("elem2", e1);
    a.addElements({DDElement("elem3", type3)});
    a.addElement("elem8", uint16_data_type("tUInt16"));

    // the type1 is used ... so the base unit must exist
    ASSERT_TRUE(test_ddl::containsDataType(a.getDD(), type1.getDataType()));
    ASSERT_TRUE(test_ddl::containsBaseUnit(a.getDD(), ddl::BaseUnit<unit::Metre>()));

    // type 2 is used in enum which is used in structure so ... so the base unit and the unit and
    // the prefixes must exist
    ASSERT_TRUE(test_ddl::containsDataType(a.getDD(), type2.getDataType()));
    ASSERT_TRUE(test_ddl::containsBaseUnit(a.getDD(), ddl::BaseUnit<unit::Ampere>()));
    ASSERT_TRUE(test_ddl::containsBaseUnit(a.getDD(), ddl::BaseUnit<unit::Candela>()));
    ASSERT_TRUE(test_ddl::containsUnitPrefix(a.getDD(), UnitPrefix<unit_prefix::atto>()));
    ASSERT_TRUE(test_ddl::containsUnitPrefix(a.getDD(), UnitPrefix<unit_prefix::centi>()));

    // type 3 is used in structure via Element ... so the base unit and the unit and
    // the prefixes must exist
    ASSERT_TRUE(test_ddl::containsDataType(a.getDD(), type3.getDataType()));
    ASSERT_TRUE(test_ddl::containsBaseUnit(a.getDD(), ddl::BaseUnit<unit::Degree>()));
    ASSERT_TRUE(test_ddl::containsBaseUnit(a.getDD(), ddl::BaseUnit<unit::Kelvin>()));
    ASSERT_TRUE(test_ddl::containsUnitPrefix(a.getDD(), UnitPrefix<unit_prefix::deca>()));
    ASSERT_TRUE(test_ddl::containsUnitPrefix(a.getDD(), UnitPrefix<unit_prefix::deci>()));
}

/**
 * @detail The building up of a DataDefinition with units in elements
 */
TEST(cTesterDDLStructure, createDDLStructureWithUnitsInElements)
{
    using namespace ddl;
    using namespace test_ddl;
    DDDataType custom_type("custom", 1);

    DDEnum e1("e1", int8_data_type(), {{"val11", "1"}, {"val12", "2"}, {"val13", "3"}});

    { // check for BaseUnits
        DDStructure a("A");
        a.addElement(
            DDElement("elem1", int8_data_type(), {}, {}, 1, {}, {}, BaseUnit<unit::Metre>()));
        a.addElement(DDElement("elem2", e1, {}, {}, 1, {}, {}, BaseUnit<unit::Ampere>()));
        a.addElement(DDElement("elem3", custom_type, {}, {}, 1, {}, {}, BaseUnit<unit::Kelvin>()));

        ASSERT_TRUE(test_ddl::containsBaseUnit(a.getDD(), ddl::BaseUnit<unit::Metre>()));
        ASSERT_TRUE(test_ddl::containsBaseUnit(a.getDD(), ddl::BaseUnit<unit::Ampere>()));
        ASSERT_TRUE(test_ddl::containsBaseUnit(a.getDD(), ddl::BaseUnit<unit::Kelvin>()));
    }

    DDUnit unit1("unit_for_type1_unit",
                 "1",
                 "2",
                 "3",
                 {DDRefUnit(BaseUnit<unit::Metre>(), 1, UnitPrefix<unit_prefix::kilo>()),
                  DDRefUnit(BaseUnit<unit::Mole>(), 32, UnitPrefix<unit_prefix::hecto>())});
    DDUnit unit2("unit_for_type2_unit",
                 "1",
                 "2",
                 "3",
                 {DDRefUnit(BaseUnit<unit::Ampere>(), 1, UnitPrefix<unit_prefix::atto>()),
                  DDRefUnit(BaseUnit<unit::Candela>(), 32, UnitPrefix<unit_prefix::centi>())});

    DDUnit unit3("unit_for_type3_unit",
                 "1",
                 "2",
                 "3",
                 {DDRefUnit(BaseUnit<unit::Degree>(), 1, UnitPrefix<unit_prefix::deca>()),
                  DDRefUnit(BaseUnit<unit::Kelvin>(), 32, UnitPrefix<unit_prefix::deci>())});

    { // check for Units
        DDStructure a("A");
        a.addElement(DDElement("elem1", int8_data_type(), {}, {}, 1, {}, {}, unit1));
        a.addElement(DDElement("elem2", e1, {}, {}, 1, {}, {}, unit2));
        a.addElement(DDElement("elem3", custom_type, {}, {}, 1, {}, {}, unit3));

        ASSERT_TRUE(test_ddl::containsBaseUnit(a.getDD(), ddl::BaseUnit<unit::Metre>()));
        ASSERT_TRUE(test_ddl::containsBaseUnit(a.getDD(), ddl::BaseUnit<unit::Mole>()));
        ASSERT_TRUE(test_ddl::containsBaseUnit(a.getDD(), ddl::BaseUnit<unit::Ampere>()));
        ASSERT_TRUE(test_ddl::containsBaseUnit(a.getDD(), ddl::BaseUnit<unit::Candela>()));
        ASSERT_TRUE(test_ddl::containsBaseUnit(a.getDD(), ddl::BaseUnit<unit::Degree>()));
        ASSERT_TRUE(test_ddl::containsBaseUnit(a.getDD(), ddl::BaseUnit<unit::Kelvin>()));

        ASSERT_TRUE(test_ddl::containsUnitPrefix(a.getDD(), UnitPrefix<unit_prefix::kilo>()));
        ASSERT_TRUE(test_ddl::containsUnitPrefix(a.getDD(), UnitPrefix<unit_prefix::hecto>()));
        ASSERT_TRUE(test_ddl::containsUnitPrefix(a.getDD(), UnitPrefix<unit_prefix::atto>()));
        ASSERT_TRUE(test_ddl::containsUnitPrefix(a.getDD(), UnitPrefix<unit_prefix::centi>()));
        ASSERT_TRUE(test_ddl::containsUnitPrefix(a.getDD(), UnitPrefix<unit_prefix::deca>()));
        ASSERT_TRUE(test_ddl::containsUnitPrefix(a.getDD(), UnitPrefix<unit_prefix::deci>()));

        ASSERT_TRUE(test_ddl::containsUnit(a.getDD(), unit1.getUnit()));
        ASSERT_TRUE(test_ddl::containsUnit(a.getDD(), unit2.getUnit()));
        ASSERT_TRUE(test_ddl::containsUnit(a.getDD(), unit3.getUnit()));
    }
}

/**
 * @detail Check if it is really not possible to create a invalid structure.
 */
TEST(cTesterDDLStructure, checkValidationOfStruct)
{
    // DDStructure will never create a DD under validation level good_enough!
    // check CTOR!
    using namespace ddl;

    EXPECT_THROW({ DDStructure a("test", 1, 5); }, dd::Error)
        << "the CTOR of DDLStructure should throw here (invalid alignment), but did not!";
    // check CTOR again
    EXPECT_THROW({ DDStructure a("test", "<struct name=\"test\" alignment=\"6\"/>"); }, dd::Error)
        << "the CTOR of DDLStructure should throw here (invalid alignment), but did not";
    // check CTOR again
    EXPECT_THROW({ DDStructure a("test", "<struct name=\"othername\" alignment=\"8\"/>"); },
                 dd::Error)
        << "the CTOR of DDLStructure should throw here (given ddl has no 'test'), but did not";
}

/**
 * @detail Check if it is really not possible to create a invalid structure.
 */
TEST(cTesterDDLStructure, checkValidationOfDataType)
{
    // DDDataType will never create a DD under validation level good_enough!
    // check CTOR!
    using namespace ddl;
    {
        try {
            DDDataType a("test", 0, {}, 0);
            FAIL() << "the CTOR of DDDataType should throw here (invalid array size and bit_size), "
                      "but did not!";
        }
        catch (const dd::Error& err) {
            EXPECT_NE(std::string(err.what()).find("array_size"), std::string::npos);
            EXPECT_NE(std::string(err.what()).find("bit_size"), std::string::npos);
        }
    }

    // check CTOR! with unit
    {
        const std::vector<DDRefUnit> ref_units = {
            DDRefUnit(BaseUnit<unit::Metre>(), 2, UnitPrefix<unit_prefix::atto>())};
        const DDUnit dd_unit("test", "1", "2", "3", ref_units);
        try {
            DDDataType a("test", 0, {}, 0, {}, {}, {}, dd_unit);
            FAIL() << "the CTOR of DDDataType should throw here (invalid array size and bit_size), "
                      "but did not!";
        }
        catch (const dd::Error& err) {
            EXPECT_NE(std::string(err.what()).find("array_size"), std::string::npos);
            EXPECT_NE(std::string(err.what()).find("bit_size"), std::string::npos);
        }
    }
    // check CTOR! with base unit
    {
        try {
            DDDataType a("test", 0, {}, 0, {}, {}, {}, BaseUnit<unit::Metre>());
            FAIL() << "the CTOR of DDDataType should throw here (invalid array size and bit_size), "
                      "but did not!";
        }
        catch (const dd::Error& err) {
            EXPECT_NE(std::string(err.what()).find("array_size"), std::string::npos);
            EXPECT_NE(std::string(err.what()).find("bit_size"), std::string::npos);
        }
    }
}

/**
 * @detail Check if it is really not possible to create a invalid structure.
 */
TEST(cTesterDDLStructure, checkValidationOfAddElement)
{
    // DDStructure will never create a DD under validation level good_enough!
    // check addElement!
    // DDElement itself can not be checked, because arraysize="other_element_name" will fail,
    // DDElement is only a helper class here! Important addElement will always be valid, the invalid
    // element will be remove, so it is not possible to catch and work with an invalid Structure
    // then!
    using namespace ddl;
    using namespace test_ddl;

    // check for invalid enum element used for constant value
    {
        DDStructure my_struct("A");
        try {
            DDEnum e1("E1", int32_data_type(), {{"val1", "1"}, {"val2", "2"}});
            my_struct.addElement("elem1", e1, "val3");
            FAIL() << "the addElement of DDStructure should throw here (invalid enum element "
                      "used), but did not!";
        }
        catch (const dd::Error& err) {
            EXPECT_NE(std::string(err.what()).find("val3"), std::string::npos);
            EXPECT_FALSE(my_struct.getStructType().getElements().contains("elem1"));
        }
    }
    // check if invalid array size was use
    {
        DDStructure my_struct("A");
        try {
            my_struct.addElement(DDElement("elem1", int32_data_type(), {}, {}, 0));
            FAIL() << "the addElement of DDStructure should throw here (invalid array_size used), "
                      "but did not!";
        }
        catch (const dd::Error& err) {
            EXPECT_NE(std::string(err.what()).find("array_size"), std::string::npos);
            EXPECT_FALSE(my_struct.getStructType().getElements().contains("elem1"));
        }
    }
    // check if invalid alignemt was used
    {
        DDStructure my_struct("A");
        try {
            my_struct.addElement(DDElement("elem1", int32_data_type(), {5}));
            FAIL() << "the addElement of DDStructure should throw here (invalid alignment used), "
                      "but did not!";
        }
        catch (const dd::Error& err) {
            EXPECT_NE(std::string(err.what()).find("alignment"), std::string::npos);
            EXPECT_FALSE(my_struct.getStructType().getElements().contains("elem1"));
        }
    }
    // check if invalid array_size_element_name is valid used
    {
        DDStructure my_struct("A");
        try {
            my_struct.addElement(DDElement("elem1", int32_data_type(), {8}, {}, 1));
            my_struct.addElement(DDElement("elem2", int32_data_type(), {8}, {}, {"test_throw"}));
            FAIL() << "the addElement of DDStructure should throw here (invalid "
                      "array_size_element_name used), but did not!";
        }
        catch (const dd::Error& err) {
            EXPECT_NE(std::string(err.what()).find("test_throw"), std::string::npos);
            EXPECT_NE(std::string(err.what()).find("array_size_element_name"), std::string::npos);
            EXPECT_FALSE(my_struct.getStructType().getElements().contains("elem2"));
        }
    }
}

/**
 * @detail Check if already exists
 */
TEST(cTesterDDLStructure, addAlreadyExisting)
{
    ddl::DDStructure my_struct("A");
    my_struct.addElement<uint8_t>("value");

    ddl::DDStructure my_A_again_struct("A", 1, 8);
    my_A_again_struct.addElement<uint8_t>("value");

    ddl::dd::DataDefinition definition = my_struct.getDD();
    EXPECT_ANY_THROW(definition.add(my_A_again_struct.getDD()));

    ddl::DDStructure my_A_again_struct_other_element("A");
    my_A_again_struct_other_element.addElement<uint16_t>("value");
    definition = my_struct.getDD();
    EXPECT_ANY_THROW(definition.add(my_A_again_struct_other_element.getDD()));
}

static constexpr const char* const expected_dd_with_info = R"(<?xml version="1.0"?>
<ddl:ddl xmlns:ddl="ddl">
    <header>
        <language_version>4.01</language_version>
        <author></author>
        <date_creation></date_creation>
        <date_change></date_change>
        <description></description>
    </header>
    <units>
        <baseunit name="Ampere" symbol="A" description="Fundamental unit for electric current" />
        <baseunit name="Metre" symbol="m" description="Fundamental unit for length" />
        <baseunit name="Mole" symbol="mol" description="Fundamental unit for amount of substance" />
        <prefixes name="kilo" power="3" symbol="k" />
        <prefixes name="hecto" power="2" symbol="h" />
        <unit name="unit_to_set">
            <numerator>1</numerator>
            <denominator>2</denominator>
            <offset>3</offset>
            <refUnit name="Metre" power="1" prefix="kilo" />
            <refUnit name="Mole" power="32" prefix="hecto" />
        </unit>
    </units>
    <datatypes>
        <datatype name="tUInt64" size="64" description="Predefined DataType for tUInt64" arraysize="1" min="0" max="18446744073709551615" />
        <datatype name="tUInt8" size="8" description="Predefined DataType for tUInt8" arraysize="1" min="0" max="255" />
    </datatypes>
    <enums />
    <structs>
        <struct name="StructWithInfos" version="1" alignment="8" ddlversion="4.01">
            <element name="sub_struct" type="SubStructWithInfo" arraysize="1">
                <serialized bytepos="0" byteorder="LE" />
                <deserialized alignment="8" />
            </element>
            <element name="value" type="tUInt64" description="test_description" unit="unit_to_set" comment="test_comment" arraysize="1" min="-12" max="42" default="3" scale="0.5" offset="10">
                <serialized bytepos="9" byteorder="LE" />
                <deserialized alignment="8" />
            </element>
        </struct>
        <struct name="SubStructWithInfo" version="1" alignment="8" comment="this is another comment" ddlversion="4.01">
            <element name="value_1" type="tUInt8" description="test_description" unit="Ampere" comment="test_comment" arraysize="1" min="-12" max="42" default="3" scale="0.5" offset="10">
                <serialized bytepos="0" byteorder="LE" />
                <deserialized alignment="1" />
            </element>
            <element name="value_2" type="tUInt64" description="test_description" unit="unit_to_set" comment="test_comment" arraysize="1" min="-12" max="42" default="3" scale="0.5" offset="10">
                <serialized bytepos="1" byteorder="LE" />
                <deserialized alignment="8" />
            </element>
        </struct>
    </structs>
    <streammetatypes />
    <streams />
</ddl:ddl>)";

/**
 * @detail Check the resulting DataDefinition by setting additional info on struct and elements.
 */
TEST(cTesterDDLStructure, addAdditionalInfo)
{
    using namespace ddl;
    DDStructure my_sub_struct("SubStructWithInfo");
    my_sub_struct.setStructInfo("this is a comment");

    my_sub_struct.addElement<uint8_t>("value_1");
    my_sub_struct.setElementInfo(
        "value_1", "test_description", "test_comment", {}, "-12", "42", "3", "0.5", "10");
    my_sub_struct.setElementUnit("value_1", BaseUnit<unit::Ampere>());

    my_sub_struct.addElement<uint64_t>("value_2");
    my_sub_struct.setElementInfo(
        "value_2", "test_description", "test_comment", {}, "-12", "42", "3", "0.5", "10");

    DDUnit unit("unit_to_set",
                "1",
                "2",
                "3",
                {DDRefUnit(BaseUnit<unit::Metre>(), 1, UnitPrefix<unit_prefix::kilo>()),
                 DDRefUnit(BaseUnit<unit::Mole>(), 32, UnitPrefix<unit_prefix::hecto>())});
    my_sub_struct.setElementUnit("value_2", unit);

    DDStructure my_struct("StructWithInfos");
    my_sub_struct.setStructInfo("this is another comment");
    my_struct.addElement("sub_struct", my_sub_struct);

    my_struct.addElement<uint64_t>("value");
    my_struct.setElementInfo(
        "value", "test_description", "test_comment", {}, "-12", "42", "3", "0.5", "10");

    my_struct.setElementUnit("value", unit);

    dd::DataDefinition expected_dd = DDString::fromXMLString(expected_dd_with_info);
    ASSERT_EQ(a_util::result::SUCCESS, DDCompare::isEqual(expected_dd, my_struct.getDD()));
}

/**
 * @detail Check if setting additional infos to non existing elements and uninitialized struct
 * throws.
 */
TEST(cTesterDDLStructure, addAdditionalInfoNegative)
{
    using namespace ddl;
    DDUnit unit("unit_to_set",
                "1",
                "2",
                "3",
                {DDRefUnit(BaseUnit<unit::Metre>(), 1, UnitPrefix<unit_prefix::kilo>()),
                 DDRefUnit(BaseUnit<unit::Mole>(), 32, UnitPrefix<unit_prefix::hecto>())});

    DDStructure my_sub_struct = DDStructure("test");
    ASSERT_THROW(
        my_sub_struct.setElementInfo(
            "value_1", "test_description", "test_comment", {}, "-12", "42", "3", "0.5", "10"),
        dd::Error);
    ASSERT_THROW(my_sub_struct.setElementUnit("value_1", BaseUnit<unit::Ampere>()), dd::Error);
    ASSERT_THROW(my_sub_struct.setElementUnit("value_1", unit), dd::Error);
}
