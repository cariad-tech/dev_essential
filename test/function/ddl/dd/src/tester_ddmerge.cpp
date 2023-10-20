/**
 * @file
 * Implementation of the tester for the DataDefinition and its merging functionality.
 *
 * Copyright @ 2022 VW Group. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "./../../_common/test_oo_ddl.h"

#include <ddl/dd/ddstructure.h>

#include <gtest/gtest.h>

#include <cstdio>
#include <functional>

class DDMergeTestFixture : public ::testing::Test {
protected:
    using DataType = ddl::dd::datamodel::DataType;
    using EnumType = ddl::dd::datamodel::EnumType;
    using StructType = ddl::dd::datamodel::StructType;
    using Stream = ddl::dd::datamodel::Stream;
    using StreamMetaType = ddl::dd::datamodel::StreamMetaType;

protected:
    static ddl::dd::DataDefinition createTestDD()
    {
        using namespace test_ddl;
        using ddl::BaseUnit;
        using ddl::DDEnum;
        using ddl::DDStructure;

        DDStructure initial_sub_struct("sub_struct");
        const DDEnum e1("E1", int32_data_type(), {{"val1", "1"}, {"val2", "2"}});

        initial_sub_struct.addElement("elem1", int64_data_type());
        initial_sub_struct.setElementUnit("elem1", BaseUnit<ddl::unit::Ampere>());
        initial_sub_struct.addElement("elem2", e1, "val2");
        initial_sub_struct.addElement("elem3", float_data_type());

        DDStructure initial_struct("test_struct");
        const DDEnum e2("E2", int32_data_type(), {{"valE21", "1"}, {"valE22", "2"}});

        initial_struct.addElement("elem1", double_data_type());
        initial_struct.setElementUnit("elem1", BaseUnit<ddl::unit::Metre>());
        initial_struct.addElement("elem2", e2, "valE21");
        initial_struct.addElement("elem3", initial_sub_struct);

        return initial_struct.getDD();
    }

    static ddl::dd::DataDefinition createTestDDWithMetaTypes()
    {
        auto created_dd = createTestDD();
        created_dd.getStreamMetaTypes().add(
            StreamMetaType("other_parent", "1", {}, {{"other_parent_test", "value"}}));
        created_dd.getStreamMetaTypes().add(
            StreamMetaType("parent", "1", {}, {{"parent_test", "value"}}));
        created_dd.getStreamMetaTypes().add(
            StreamMetaType("meta_type", "1", {}, {{"test1", "value1"}, {"test2", "value2"}}));
        return created_dd;
    }

    static ddl::dd::DataDefinition createTestDDWithStreams()
    {
        auto created_dd = createTestDDWithMetaTypes();
        created_dd.getStreams().add(
            Stream("test_stream",
                   "meta_type",
                   "descr",
                   {{"pos1", "test_struct", 0}, {"pos2", "test_struct", 2}}));
        return created_dd;
    }

    static void modifyAndMergeDataType(const std::function<void(DataType&)>& modify_function)
    {
        auto first = createTestDD();
        auto second = createTestDD();

        auto second_test_dt = second.getDataTypes().access("tInt64");
        modify_function(*second_test_dt);
        first.add(second);
    }

    static void modifyAndMergeEnum(const std::function<void(EnumType&)>& modify_function)
    {
        auto first = createTestDD();
        auto second = createTestDD();

        auto second_test_enum = second.getEnumTypes().access("E2");
        modify_function(*second_test_enum);
        first.add(second);
    }

    static void modifyAndMergeStruct(const std::function<void(StructType&)>& modify_function)
    {
        auto first = createTestDD();
        auto second = createTestDD();

        auto second_test_struct = second.getStructTypes().access("test_struct");
        modify_function(*second_test_struct);
        first.add(second);
    }

    static void modifyAndMergeStream(const std::function<void(Stream&)>& modify_function)
    {
        auto first = createTestDDWithStreams();
        auto second = createTestDDWithStreams();

        auto second_test_stream = second.getStreams().access("test_stream");
        modify_function(*second_test_stream);
        first.add(second);
    }

    static void modifyAndMergeStreamMetaType(
        const std::function<void(StreamMetaType&)>& modify_function)
    {
        auto first = createTestDDWithMetaTypes();
        auto second = createTestDDWithMetaTypes();

        auto second_test_meta_type = second.getStreamMetaTypes().access("meta_type");
        modify_function(*second_test_meta_type);
        first.add(second);
    }
};

/**
 * @detail Create the same DD 2 times and merge
 */
TEST_F(DDMergeTestFixture, MergeCompatible)
{
    auto first = createTestDD();
    const auto second = createTestDD();

    ASSERT_NO_THROW(first.add(second));
}

/**
 * @detail Create the same DD 2 times, manipulate 1 of its datatype attributes
 *         and merge. Expect exception.
 */
TEST_F(DDMergeTestFixture, TryMergingIncompatibleDatatypes)
{
    ASSERT_THROW(modifyAndMergeDataType([](DataType& type) { type.setBitSize(12); }),
                 ddl::dd::Error);
    ASSERT_THROW(modifyAndMergeDataType([](DataType& type) { type.setArraySize(4); }),
                 ddl::dd::Error);
    ASSERT_THROW(modifyAndMergeDataType([](DataType& type) { type.setUnitName("123"); }),
                 ddl::dd::Error);
}

/**
 * @detail Create the same DD 2 times, manipulate 1 datatype but at position that are not relevant
 * for merging. Everything is fine.
 */
TEST_F(DDMergeTestFixture, MergingDataTypesAndAcceptDifferences)
{
    ASSERT_NO_THROW(
        modifyAndMergeDataType([](DataType& type) { type.setDescription("different 123"); }));
    ASSERT_NO_THROW(modifyAndMergeDataType([](DataType& type) { type.setMin("123"); }));
    ASSERT_NO_THROW(modifyAndMergeDataType([](DataType& type) { type.setMax("123"); }));
}

/**
 * @detail Create the same DD 2 times, manipulate 1 of it at the enum attributes
 *         and merge. Expect exception.
 */
TEST_F(DDMergeTestFixture, TryMergingIncompatibleEnums)
{
    ASSERT_THROW(modifyAndMergeEnum([](EnumType& type) { type.setDataTypeName("int64_t"); }),
                 ddl::dd::Error);

    ASSERT_THROW(modifyAndMergeEnum([](EnumType& type) {
                     type.getElements().access("valE21")->setName("otherValName");
                 }),
                 ddl::dd::Error);

    ASSERT_THROW(modifyAndMergeEnum(
                     [](EnumType& type) { type.getElements().access("valE21")->setValue("25"); }),
                 ddl::dd::Error);

    ASSERT_THROW(modifyAndMergeEnum([](EnumType& type) { type.getElements().remove("valE22"); }),
                 ddl::dd::Error);
}

/**
 * @detail Create the same DD 2 times, manipulate 1 of it at the struct attributes
 *         and merge. Expect exception.
 */
TEST_F(DDMergeTestFixture, TryMergingIncompatibleStructsByStructAttributes)
{
    ASSERT_THROW(modifyAndMergeStruct([](StructType& type) { type.setAlignment(4); }),
                 ddl::dd::Error);

    ASSERT_THROW(modifyAndMergeStruct([](StructType& type) { type.setVersion("4"); }),
                 ddl::dd::Error);

    ASSERT_THROW(modifyAndMergeStruct(
                     [](StructType& type) { type.setLanguageVersion(ddl::dd::Version(2, 0)); }),
                 ddl::dd::Error);
}

/**
 * @detail Create the same DD 2 times, manipulate 1 of it at element values
 *         and merge. Expect exception.
 */
TEST_F(DDMergeTestFixture, TryMergingIncompatibleStructsByElements)
{
    ASSERT_THROW(modifyAndMergeStruct([](StructType& type) {
                     type.getElements().access("elem1")->setName("elem_manipulated");
                 }),
                 ddl::dd::Error);

    ASSERT_THROW(modifyAndMergeStruct(
                     [](StructType& type) { type.getElements().access("elem1")->setAlignment(4); }),
                 ddl::dd::Error);

    ASSERT_THROW(modifyAndMergeStruct(
                     [](StructType& type) { type.getElements().access("elem1")->setArraySize(4); }),
                 ddl::dd::Error);

    ASSERT_THROW(modifyAndMergeStruct(
                     [](StructType& type) { type.getElements().access("elem1")->setBitPos(7); }),
                 ddl::dd::Error);

    ASSERT_THROW(modifyAndMergeStruct(
                     [](StructType& type) { type.getElements().access("elem1")->setBytePos(5); }),
                 ddl::dd::Error);

    ASSERT_THROW(modifyAndMergeStruct([](StructType& type) {
                     type.getElements().access("elem1")->setTypeName("uint8_t");
                 }),
                 ddl::dd::Error);

    ASSERT_THROW(modifyAndMergeStruct([](StructType& type) {
                     type.getElements().access("elem1")->setOffset("123");
                 }),
                 ddl::dd::Error);

    ASSERT_THROW(modifyAndMergeStruct(
                     [](StructType& type) { type.getElements().access("elem1")->setMin("123"); }),
                 ddl::dd::Error);

    ASSERT_THROW(modifyAndMergeStruct(
                     [](StructType& type) { type.getElements().access("elem1")->setMax("123"); }),
                 ddl::dd::Error);

    ASSERT_THROW(modifyAndMergeStruct(
                     [](StructType& type) { type.getElements().access("elem1")->setScale("123"); }),
                 ddl::dd::Error);

    ASSERT_THROW(modifyAndMergeStruct([](StructType& type) {
                     type.getElements().access("elem1")->setUnitName("Ampere");
                 }),
                 ddl::dd::Error);

    ASSERT_THROW(modifyAndMergeStruct(
                     [](StructType& type) { type.getElements().access("elem1")->setValue("123"); }),
                 ddl::dd::Error);
}

/**
 * @detail Create the same DD 2 times, manipulate 1 but at positions that are not relevant for
 * merging and merge. Everything is fine.
 */
TEST_F(DDMergeTestFixture, MergingStructsAndAcceptDifferences)
{
    ASSERT_NO_THROW(modifyAndMergeStruct(
        [](StructType& type) { type.getElements().access("elem1")->setComment("123"); }));

    ASSERT_NO_THROW(modifyAndMergeStruct(
        [](StructType& type) { type.getElements().access("elem1")->setDescription("123"); }));

    ASSERT_NO_THROW(modifyAndMergeStruct([](StructType& type) { type.setComment("123"); }));
}

/**
 * @detail Create the same DD with stream metatype 2 times and merge
 */
TEST_F(DDMergeTestFixture, MergeCompatibleWithStreamMetaTypes)
{
    auto first = createTestDDWithMetaTypes();
    const auto second = createTestDDWithMetaTypes();

    ASSERT_NO_THROW(first.add(second));
}

/**
 * @detail Create the same DD 2 times, manipulate 1 of its streammetatype attributes
 *         and merge. Expect exception.
 */
TEST_F(DDMergeTestFixture, TryMergingIncompatibleStreamMetaTypes)
{
    ASSERT_THROW(
        modifyAndMergeStreamMetaType([](StreamMetaType& type) { type.setVersion("123456"); }),
        ddl::dd::Error);

    ASSERT_THROW(
        modifyAndMergeStreamMetaType([](StreamMetaType& type) { type.setParent("other_parent"); }),
        ddl::dd::Error);

    ASSERT_THROW(modifyAndMergeStreamMetaType([](StreamMetaType& type) {
                     type.getProperties().access("test1")->setValue("different");
                 }),
                 ddl::dd::Error);

    ASSERT_THROW(modifyAndMergeStreamMetaType(
                     [](StreamMetaType& type) { type.getProperties().remove("test1"); }),
                 ddl::dd::Error);
}

/**
 * @detail Create the same DD with streams 2 times and merge.
 */
TEST_F(DDMergeTestFixture, MergeCompatibleWithStreams)
{
    auto first = createTestDDWithStreams();
    const auto second = createTestDDWithStreams();

    ASSERT_NO_THROW(first.add(second));
}

/**
 * @detail Create the same DD 2 times, manipulate 1 of its stream attributes
 *         and merge. Expect exception.
 */
TEST_F(DDMergeTestFixture, TryMergingIncompatibleStreams)
{
    ASSERT_THROW(modifyAndMergeStream([](Stream& type) { type.setStreamTypeName("other_parent"); }),
                 ddl::dd::Error);

    ASSERT_THROW(
        modifyAndMergeStream([](Stream& type) { type.getStructs().access("pos2")->setBytePos(4); }),
        ddl::dd::Error);

    ASSERT_THROW(modifyAndMergeStream(
                     [](Stream& type) { type.getStructs().access("pos2")->setName("different"); }),
                 ddl::dd::Error);

    ASSERT_THROW(modifyAndMergeStream([](Stream& type) {
                     type.getStructs().access("pos2")->setTypeName("sub_struct");
                 }),
                 ddl::dd::Error);
}

/**
 * @detail Create the same DD 2 times, manipulate 1 stream but at positions that are not relevant
 * for merging and merge. Everything is fine.
 */
TEST_F(DDMergeTestFixture, MergingStreamsAndAcceptDifferences)
{
    ASSERT_NO_THROW(
        modifyAndMergeStream([](Stream& type) { type.setDescription("different description"); }));
}
