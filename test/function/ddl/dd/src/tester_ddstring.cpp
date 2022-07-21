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
#include "ddl_definitions.h"

#include <ddl/dd/ddstring.h>

#include <gtest/gtest.h>

#include <thread>

/**
 * @detail The building up of a DataDefinition object.
 * Read string and build up the object representation
 * @req_id ACORE-3904
 */
TEST(TesterDDString, createDDStringCTOR)
{
    using namespace ddl;

    ASSERT_NO_THROW(auto my_dd = DDString::fromXMLString(DDL_TEST_STRING);
                    ASSERT_EQ(true, my_dd.isValid())
                    << "Invalid DataDefinition object retrieved.";);
}

/**
 * @detail The building up of a DataDefinition object.
 * Read string and build up the object representation
 * @req_id ACORE-3904
 */
TEST(TesterDDString, createDDStringCTORNoFileVersion)
{
    using namespace ddl;

    ddl::dd::DataDefinition my_dd;
    ASSERT_NO_THROW(my_dd = DDString::fromXMLString(DDL_TEST_STRING_DIFF);
                    ASSERT_EQ(true, my_dd.isValid())
                    << "Invalid DataDefinition object retrieved.";);
    EXPECT_EQ(my_dd.getVersion(), ddl::dd::Version::ddl_version_current);
}

/**
 * @detail Check if it is really not possible to create a invalid DDString.
 */
TEST(TesterDDLString, checkValidationOfStringCTOR)
{
    // DDSTring will never create a DD under validation level good_enough!
    // check CTOR again
    using namespace ddl;
    EXPECT_THROW(
        {
            auto test_dd = DDString::fromXMLString("<struct name=\"test\" alignment=\"6\"/>");
            // this code should never be reached, but we need some access, that the compiler
            // does not optimize!
            EXPECT_FALSE(test_dd.isValid());
        },
        dd::Error)
        << "the CTOR of DDLString should throw here (invalid alignment), but did not";

    // check CTOR again

    EXPECT_THROW(
        {
            auto test_dd =
                DDString::fromXMLString("test", "<struct name=\"othername\" alignment=\"8\"/>");
            // this code should never be reached, but we need some access, that the compiler does
            // not optimize!
            EXPECT_FALSE(test_dd.isValid());
        },
        dd::Error)
        << "the CTOR of DDLString should throw here (given ddl has no 'test'), but did not";
}

/**
 * @detail Check if it is really not possible to create a invalid DDString.
 */
TEST(TesterDDLString, checkValidationOfStringRead)
{
    // DDString will never create a DD under validation level good_enough!
    // check CTOR again
    using namespace ddl;

    EXPECT_THROW(
        {
            auto test_dd = DDString::fromXMLString("<struct name=\"test\" alignment=\"6\"/>");
            // this code should never be reached, but we need some access, that the compiler does
            // not optimize!
            EXPECT_FALSE(test_dd.isValid());
        },
        dd::Error)
        << "the CTOR of DDLString should throw here (invalid alignment), but did not";

    // check CTOR again
    EXPECT_THROW(
        {
            auto test_dd =
                DDString::fromXMLString("othername", "<struct name=\"test\" alignment=\"8\"/>");
            // this code should never be reached, but we need some access, that the compiler does
            // not optimize!
            EXPECT_FALSE(test_dd.isValid());
        },
        dd::Error)
        << "the CTOR of DDLString should throw here (invalid struct name), but did not";
}

/**
 * @detail Check if it is really not possible to create a invalid DDString.
 */
TEST(TesterDDLString, checkValidationOfStringInvalidUnit)
{
    // DDSTring will never create a DD under validation level good_enough!
    // a not existing unit is good enough
    // check CTOR again
    using namespace ddl;
    {
        std::vector<dd::Problem> problems;
        DataDefinition my_dd;
        bool it_throwed = false;
        try {
            my_dd = DDString::fromXMLString(DDL_TEST_STRING_UNIT_USED_BUT_NOT_PRESENT);
        }
        catch (dd::Error& err) {
            it_throwed = true;
            problems = err.problems();
        }
        EXPECT_FALSE(it_throwed)
            << "the CTOR of DDLString should not throw here (invalid unit used), but did!";
        EXPECT_TRUE(test_ddl::containsASpecificProblem(
            my_dd.getValidationProtocol(),
            "The used unit 'my_unit_which_does_not_exist' is not defined"));
    }
}

/**
 * @detail Check if it is really not possible to create a invalid DDString.
 */
TEST(TesterDDLString, checkInvalidWithMissingStructAlignmentIsAccepted)
{
    using namespace ddl;
    const std::string test_desc = "<structs>\
        <struct name=\"from_string\" version=\"1\" ddlversion=\"4.0\">\
                <element name=\"value1\" type=\"tUInt32\" arraysize=\"1\">\
                        <deserialized alignment=\"1\"/>\
                        <serialized bytepos=\"0\" byteorder=\"LE\"/>\
                </element>\
        </struct>\
        </structs>";

    // usually we accept it withaut a set alignment
    ASSERT_NO_THROW(auto my_dd = DDString::fromXMLString(test_desc););
}

const char* simple_struct_desc = "<struct alignment=\"4\" name=\"tSimple\" version=\"2\">"
                                 "<element name=\"fValue\" type=\"tFloat32\" arraysize=\"1\">"
                                 "<deserialized alignment=\"4\"/>"
                                 "<serialized byteorder=\"LE\" bytepos=\"0\"/>"
                                 "</element>"
                                 "</struct>";

/**
 * @detail Check if implementation of creation is thread-safe and reentrant.
 */
TEST(TesterDDLString, checkConcurrentCreationOfSimpleString)
{
    using namespace ddl;

    std::vector<std::thread> threads;

    for (size_t current = 0; current < 2000; current++) {
        threads.push_back(std::thread([&current] { // create a stream type with media description
            auto millisec_sleep_time = current % 20;
            std::this_thread::sleep_for(std::chrono::milliseconds(millisec_sleep_time));
            // usually we accept it withaut a set alignment
            ASSERT_NO_THROW(auto my_dd = DDString::fromXMLString("tSimple", simple_struct_desc));
        }));
    }

    for (auto& oCurrentThread: threads) {
        oCurrentThread.join();
    }
}
