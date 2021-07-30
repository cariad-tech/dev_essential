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
#include "ddl/datamodel/xml_datamodel.h"
#include "ddl/dd/dd.h"
#include "ddl/dd/ddfile.h"
#include "ddl/dd/ddstructure.h"

#include <gtest/gtest.h>

/**
 * @detail The building up of a DataDefinition object representation.
 * It is very important to keep the model valid while renaming a data type, unit,
 * struct type, enum type which where used in other types.
 */
TEST(TesterOODDL, checkValidationForAutoRenamingsDataType)
{
    using namespace ddl;
    using namespace test_ddl;

    dd::DataDefinition my_dd;
    my_dd.getDataTypes().add({"tBool", "8", "predefined ADTF tBool datatype"});
    my_dd.getDataTypes().add({"tChar", "8", "predefined ADTF tChar datatype"});
    my_dd.getDataTypes().add({"tUInt8", "8", "predefined ADTF tUInt8 datatype"});
    my_dd.getDataTypes().add({"tInt8", "8", "predefined ADTF tInt8 datatype"});
    my_dd.getDataTypes().add({"tUInt16", "16", "predefined ADTF tUInt16 datatype"});
    my_dd.getDataTypes().add({"tInt16", "16", "predefined ADTF tInt16 datatype"});
    my_dd.getDataTypes().add({"tUInt32", "32", "predefined ADTF tUInt32 datatype"});

    my_dd.getEnumTypes().add({"BOOL_ENUM1", "tBool", {{{"t"}, {"true"}}, {{"f"}, {"false"}}}});
    my_dd.getEnumTypes().add({"BOOL_ENUM2", "tBool", {{{"t"}, {"true"}}, {{"f"}, {"false"}}}});
    my_dd.getEnumTypes().add({"BOOL_ENUM3", "tBool", {{{"t"}, {"true"}}, {{"f"}, {"false"}}}});

    // check rename the data type in the enum
    auto enum1_access = my_dd.getEnumTypes().access("BOOL_ENUM1");
    auto enum2_access = my_dd.getEnumTypes().access("BOOL_ENUM2");
    auto enum3_access = my_dd.getEnumTypes().access("BOOL_ENUM3");
    ASSERT_EQ(enum1_access->getDataTypeName(), "tBool");
    ASSERT_EQ(enum2_access->getDataTypeName(), "tBool");
    ASSERT_EQ(enum3_access->getDataTypeName(), "tBool");

    auto bool_type = my_dd.getDataTypes().access("tBool");
    bool_type->setName("bool");

    ASSERT_EQ(enum1_access->getDataTypeName(), "bool");
    ASSERT_EQ(enum2_access->getDataTypeName(), "bool");
    ASSERT_EQ(enum3_access->getDataTypeName(), "bool");

    DDStructure a("A");
    a.addElement<int32_t>("elem1");
    a.addElement("elem2", uint16_data_type());
    a.addElements({DDElement("elem3", uint64_data_type()), DDElement("elem4", float_data_type())});
    a.addElements({{"elem5", double_data_type()},
                   {"elem6", uint16_data_type()},
                   {"elem7", uint64_data_type()}});

    my_dd.add(a.getStructType(), a.getDD());

    // check renaming a enum type and a data type
    // which is used within a struct
    auto uint64_data_type = my_dd.getDataTypes().access("tUInt64");

    auto elem3 = my_dd.getStructTypes().get("A")->getElements().get("elem3");
    auto elem7 = my_dd.getStructTypes().get("A")->getElements().get("elem7");

    ASSERT_EQ(elem3->getTypeName(), "tUInt64");
    ASSERT_EQ(elem7->getTypeName(), "tUInt64");

    uint64_data_type->setName("renamed_uint64_t");

    ASSERT_EQ(elem3->getTypeName(), "renamed_uint64_t");
    ASSERT_EQ(elem7->getTypeName(), "renamed_uint64_t");
}

/**
 * @detail The building up of a DataDefinition object representation.
 * It is very important to keep the model valid while renaming a data type, unit,
 * struct type, enum type which where used in other types.
 */
TEST(TesterOODDL, checkValidationForAutoRenamingsEnumType)
{
    using namespace ddl;
    using namespace test_ddl;

    dd::DataDefinition my_dd;

    DDEnum e1("e1", int8_data_type(), {{"val11", "1"}, {"val12", "2"}, {"val13", "3"}});
    DDStructure a("A");
    a.addElement<int32_t>("elem1");
    a.addElement("elem2", uint16_data_type());
    a.addElements({DDElement("elem3", uint64_data_type()), DDElement("elem4", float_data_type())});
    a.addElements({{"elem5", double_data_type()}, {"elem6", bool_data_type()}, {"elem7", e1}});

    my_dd.add(a.getStructType(), a.getDD());
    // check renaming a enum type and a data type
    // which is used within a struct
    auto e1_enum_type = my_dd.getEnumTypes().access("e1");

    auto elem7 = my_dd.getStructTypes().get("A")->getElements().get("elem7");

    ASSERT_EQ(elem7->getTypeName(), "e1");
    e1_enum_type->setName("renamed_e1");

    ASSERT_EQ(elem7->getTypeName(), "renamed_e1");
}

/**
 * @detail The building up of a DataDefinition object representation.
 * It is very important to keep the model valid while renaming a data type, unit,
 * struct type, enum type which where used in other types.
 */
TEST(TesterOODDL, checkValidationForAutoRenamingsStructType)
{
    using namespace ddl;
    using namespace test_ddl;

    dd::DataDefinition my_ddl;

    DDEnum e1("e1", int8_data_type(), {{"val11", "1"}, {"val12", "2"}, {"val13", "3"}});
    DDStructure a("A");
    a.addElement<int32_t>("elem1");
    a.addElement("elem2", uint16_data_type());
    a.addElements({DDElement("elem3", uint64_data_type()), DDElement("elem4", float_data_type())});
    a.addElements({{"elem5", double_data_type()}, {"elem6", bool_data_type()}, {"elem7", e1}});
    DDStructure b("B");
    b.addElement("nested1", a);
    b.addElement("nested2", a);

    my_ddl.add(b.getStructType(), b.getDD()); // since b is using a both will be added afterwards

    auto nested1 = my_ddl.getStructTypes().get("B")->getElements().get("nested1");
    auto nested2 = my_ddl.getStructTypes().get("B")->getElements().get("nested2");
    // check renaming a struct type
    // which is used within a struct
    ASSERT_EQ(nested1->getTypeName(), "A");
    ASSERT_EQ(nested2->getTypeName(), "A");

    // create a stream using the struct in the type
    my_ddl.getStreams().add({"stream1", "A", "test description 1"});
    // create a stream using the struct in the type and as stream struct
    my_ddl.getStreams().add(
        {"stream2", "A", "test description 2", {{"first", "A", {}}, {"second", "B", 25}}});

    auto stream1 = my_ddl.getStreams().get("stream1");
    auto stream2 = my_ddl.getStreams().get("stream2");
    auto stream2_first = stream2->getStructs().get("first");

    ASSERT_EQ(stream1->getStreamTypeName(), "A");
    ASSERT_EQ(stream2->getStreamTypeName(), "A");
    ASSERT_EQ(stream2_first->getTypeName(), "A");

    // rename it
    auto a_in_mydd = my_ddl.getStructTypes().access("A");
    a_in_mydd->setName("renamed_A");

    ASSERT_EQ(nested1->getTypeName(), "renamed_A");
    ASSERT_EQ(nested2->getTypeName(), "renamed_A");

    ASSERT_EQ(stream1->getStreamTypeName(), "renamed_A");
    ASSERT_EQ(stream2->getStreamTypeName(), "renamed_A");
    ASSERT_EQ(stream2_first->getTypeName(), "renamed_A");
}

/**
 * @detail The building up of a DataDefinition object representation.
 * It is very important to keep the model valid while renaming a data type, unit,
 * struct type, enum type which where used in other types.
 */
TEST(TesterOODDL, checkValidationForAutoRenamingsStreamMetaType)
{
    using namespace ddl;

    dd::DataDefinition my_dd;

    // create stream meta types depending on the "adtf/default"
    my_dd.getStreamMetaTypes().emplace({"test_parent_not_yet_defined", "1", "adtf/default", {}});
    my_dd.getStreamMetaTypes().emplace(
        {"adtf/default", "1", {}, {{"md_struct", "string"}, {"md_definitions", "string"}}});
    my_dd.getStreamMetaTypes().emplace({"test", "1", "adtf/default", {{"test_prop", "tInt32"}}});

    // create stream depending on the "adtf/default"
    my_dd.getStreams().add({"stream1", "adtf/default", "test description"});
    my_dd.getStreams().add({"stream2", "test", "test description"});

    auto test_parent_not_yet_defined =
        my_dd.getStreamMetaTypes().get("test_parent_not_yet_defined");
    auto test = my_dd.getStreamMetaTypes().get("test");

    auto stream1 = my_dd.getStreams().get("stream1");

    ASSERT_EQ(test->getParent(), "adtf/default");
    ASSERT_EQ(test_parent_not_yet_defined->getParent(), "adtf/default");

    ASSERT_EQ(stream1->getStreamTypeName(), "adtf/default");

    // rename it
    auto adtf_default = my_dd.getStreamMetaTypes().access("adtf/default");
    adtf_default->setName("adtf/default/renamed");

    // test if the model is still valid
    ASSERT_EQ(test->getParent(), "adtf/default/renamed");
    ASSERT_EQ(test_parent_not_yet_defined->getParent(), "adtf/default/renamed");
    ASSERT_EQ(stream1->getStreamTypeName(), "adtf/default/renamed");

    my_dd.validate();
    ASSERT_TRUE(my_dd.isValid());
}

/**
 * @detail The building up of a DataDefinition object representation with a recursion and
 * check for detection.
 *
 */
TEST(TesterDDLFile, readDatamodelWithRecursion)
{
    using namespace ddl;
    // the Data Model can be read without exception!
    dd::datamodel::DataDefinition oo_dd_model;
    try {
        oo_dd_model = dd::datamodel::fromXMLFile(TEST_FILES_DIR "/adtf_recursion.description");
    }
    catch (dd::Error& error) {
        std::cout << error.what();
        ASSERT_TRUE(false);
    }

    std::vector<std::pair<std::string, std::string>> tMediaTypeInfo_elements = {
        {"ui32MajorType", "tUInt32"}, {"ui32SubType", "tUInt32"}, {"ui32Flags", "recursiontype2"}};

    std::vector<std::pair<std::string, std::string>> recursiontype2_elements = {
        {"sMediatype", "tMediaTypeInfo"}, {"sWaveFormat", "tWaveFormat"}};

    std::vector<std::pair<std::string, std::string>> recursion_elements = {
        {"subelement", "recursion"}};

    // the datamodel does not care about recursions and validations
    // have a look at the validation infomodel //this will detect a recursion
    ASSERT_TRUE(
        test_ddl::containsStructType(oo_dd_model, "tMediaTypeInfo", tMediaTypeInfo_elements));

    ASSERT_TRUE(
        test_ddl::containsStructType(oo_dd_model, "recursiontype2", recursiontype2_elements));

    ASSERT_TRUE(test_ddl::containsStructType(oo_dd_model, "recursion", recursion_elements));

    // the dd::DataDefinition will check that the data model is invalid
    dd::DataDefinition oo_dd;
    oo_dd.setModel(std::make_shared<dd::datamodel::DataDefinition>(std::move(oo_dd_model)));

    // But the DataDefinition is set to invalid because of the recursion
    ASSERT_FALSE(oo_dd.isValid());

    // And it is completely invalid
    ASSERT_FALSE(oo_dd.isValid(dd::DataDefinition::ValidationLevel::good_enough));

    // there is a message for this validation info
    auto protocol = oo_dd.getValidationProtocol();
    ASSERT_TRUE(protocol.size() > 0) << "the validation protocol is empty!";
    auto recursion_count = 0;
    for (auto& problem: protocol) {
        if (std::string::npos != problem.problem_message.find("recursion")) {
            recursion_count++;
        }
    }

    ASSERT_TRUE(recursion_count > 0) << "the validation protocol must contain the word 'recursion'";

    // Reading an invalid file will throw immediatelly
    bool did_throw = false;
    try {
        dd::DataDefinition oo_new_dd =
            DDFile::fromXMLFile(TEST_FILES_DIR "/adtf_recursion.description");
    }
    catch (dd::Error& error) {
        did_throw = true;
        std::cout << error.what();

        ASSERT_NE(std::string::npos, std::string(error.what()).find("recursion"))
            << "the exception must contain the word 'recursion'";
    }

    ASSERT_TRUE(did_throw) << "The fromXMLFile did not throw!";
}

/**
 * @detail The building up of a DataDefinition object representation.
 * It is very important to keep the model valid while removing a data type, unit,
 * struct type, enum type which where used in other types.
 */
TEST(TesterOODDL, checkValidationForRemovingStructTypes)
{
    using namespace ddl;
    using namespace test_ddl;

    dd::DataDefinition my_ddl;

    DDEnum e1("e1", int8_data_type(), {{"val11", "1"}, {"val12", "2"}, {"val13", "3"}});
    DDStructure a("A");
    a.addElement<int32_t>("elem1");
    a.addElement("elem2", uint16_data_type());
    a.addElements({DDElement("elem3", uint64_data_type()), DDElement("elem4", float_data_type())});
    a.addElements({{"elem5", double_data_type()}, {"elem6", bool_data_type()}, {"elem7", e1}});
    DDStructure b("B");
    b.addElement("nested1", a);
    b.addElement("nested2", a);

    my_ddl.add(b.getStructType(), b.getDD()); // since b is using a both will be added afterwards

    // create a stream using the struct in the type
    my_ddl.getStreams().add({"stream1", "A", "test description 1"});
    // create a stream using the struct in the type and as stream struct
    my_ddl.getStreams().add(
        {"stream2", "A", "test description 2", {{"first", "A", {}}, {"second", "B", 25}}});

    ASSERT_NO_THROW(my_ddl.getStructTypes().remove("A"));

    my_ddl.validate();

    // the struct is missing, so it is marked as false
    ASSERT_FALSE(my_ddl.isValid());

    // check validation protocol
    auto test_validation_string = "The used data_type 'A' is not defined in DataDefinition";
    auto protocol = my_ddl.getValidationProtocol();
    bool found_validation_string_in_protocol = false;
    for (const auto& item: protocol) {
        if (item.problem_message.find_first_of(test_validation_string) != std::string::npos) {
            found_validation_string_in_protocol = true;
        }
    }
    ASSERT_TRUE(found_validation_string_in_protocol)
        << "expected entry:" << test_validation_string << " - not found in validation protocol";
}

/**
 * @detail The building up of a DataDefinition object representation with a recursion.
 * This must be detected!
 *
 */
TEST(TesterDDLFile, detectStreamMetaTypesWithRecursion)
{
    using namespace ddl;

    dd::DataDefinition my_ddl;
    my_ddl.getStreamMetaTypes().add({"smt1", "1", ""});
    my_ddl.getStreamMetaTypes().add({"smt2", "1", "smt1"});

    my_ddl.validate();
    ASSERT_EQ(my_ddl.getStreamMetaTypes().getSize(), 2);
    ASSERT_TRUE(my_ddl.isValid());
    my_ddl.getStreamMetaTypes().access("smt1")->setParent("smt2");
    my_ddl.validate();
    ASSERT_FALSE(my_ddl.isValid());

    bool contains_recursion_keyword = false;
    for (const auto& problem: my_ddl.getValidationProtocol()) {
        if (problem.problem_message.find_first_of("recursion") != std::string::npos) {
            contains_recursion_keyword = true;
        }
    }
    ASSERT_TRUE(contains_recursion_keyword)
        << "The keyword 'recursion' was not found in problems, expecting it";
}
