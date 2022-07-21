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

#include <a_util/xml.h>
#include <ddl/dd/ddcompare.h>
#include <ddl/dd/ddstring.h>

#include <gtest/gtest.h>

#include <cstdio>

/**
 * @detail The building up of a DataDefinition object representation.
 */
TEST(TesterDDCompare, TestisBinaryEqual)
{
    using namespace ddl;
    // isBinaryEqual
    {
        ASSERT_EQ(
            a_util::result::SUCCESS,
            DDCompare::isBinaryEqual(
                "adtf.type.video", DDL_TEST_STRING, "adtf.type.video", DDL_TEST_STRING, true));
        ASSERT_EQ(
            a_util::result::SUCCESS,
            DDCompare::isBinaryEqual(
                "adtf.type.video", DDL_TEST_STRING, "adtf.type.video", DDL_TEST_STRING, false));

        ASSERT_NE(
            a_util::result::SUCCESS,
            DDCompare::isBinaryEqual(
                "adtf.type.video", DDL_TEST_STRING, "adtf.type.video", DDL_TEST_STRING_DIFF, true));
        ASSERT_NE(a_util::result::SUCCESS,
                  DDCompare::isBinaryEqual("adtf.type.video",
                                           DDL_TEST_STRING,
                                           "adtf.type.video",
                                           DDL_TEST_STRING_DIFF,
                                           false));

        ASSERT_EQ(a_util::result::SUCCESS,
                  DDCompare::isBinaryEqual("adtf.type.video",
                                           DDL_TEST_STRING_SUBSET,
                                           "adtf.type.video",
                                           DDL_TEST_STRING,
                                           true));
        ASSERT_NE(a_util::result::SUCCESS,
                  DDCompare::isBinaryEqual("adtf.type.video",
                                           DDL_TEST_STRING,
                                           "adtf.type.video",
                                           DDL_TEST_STRING_SUBSET,
                                           true));
        ASSERT_NE(a_util::result::SUCCESS,
                  DDCompare::isBinaryEqual("adtf.type.video",
                                           DDL_TEST_STRING_SUBSET,
                                           "adtf.type.video",
                                           DDL_TEST_STRING,
                                           false));

        ASSERT_EQ(a_util::result::SUCCESS,
                  DDCompare::isBinaryEqual("adtf.type.video",
                                           DDL_TEST_STRING_DYNAMIC,
                                           "adtf.type.video",
                                           DDL_TEST_STRING_DYNAMIC,
                                           false));
        ASSERT_EQ(a_util::result::SUCCESS,
                  DDCompare::isBinaryEqual("adtf.type.video",
                                           DDL_TEST_STRING_DYNAMIC,
                                           "adtf.type.video",
                                           DDL_TEST_STRING_DYNAMIC_NAME_DIFF,
                                           false));
        ASSERT_NE(a_util::result::SUCCESS,
                  DDCompare::isBinaryEqual("adtf.type.video",
                                           DDL_TEST_STRING_DYNAMIC,
                                           "adtf.type.video",
                                           DDL_TEST_STRING_DYNAMIC_BIN_DIFF,
                                           false));
        ASSERT_NE(a_util::result::SUCCESS,
                  DDCompare::isBinaryEqual("adtf.type.video",
                                           DDL_TEST_STRING_DYNAMIC,
                                           "adtf.type.video",
                                           DDL_TEST_STRING_DYNAMIC_BIN_DIFF_2,
                                           false));

        ASSERT_EQ(a_util::result::SUCCESS,
                  DDCompare::isBinaryEqual("tTest1",
                                           DDL_TEST_STRING_LAYOUT_TEST,
                                           "tTest2",
                                           DDL_TEST_STRING_LAYOUT_TEST,
                                           false));
    }
}

/**
 * @detail The building up of a DataDefinition object representation.
 */
TEST(TesterDDCompare, TestisEqualTypes)
{
    using namespace ddl;
    // isEqual (types)
    {
        auto result = DDCompare::isEqualType("adtf.type.video",
                                             DDL_TEST_STRING,
                                             "adtf.type.video",
                                             DDL_TEST_STRING,
                                             DDCompare::icf_memory | DDCompare::icf_serialized |
                                                 DDCompare::icf_names | DDCompare::icf_versions);
        ASSERT_EQ(a_util::result::SUCCESS, result) << result.getDescription();

        ASSERT_NE(a_util::result::SUCCESS,
                  DDCompare::isEqualType("adtf.type.video",
                                         DDL_TEST_STRING,
                                         "adtf.type.video",
                                         DDL_TEST_STRING_DIFF,
                                         DDCompare::icf_memory));

        ASSERT_NE(a_util::result::SUCCESS,
                  DDCompare::isEqualType("adtf.type.video",
                                         DDL_TEST_STRING,
                                         "adtf.type.video",
                                         DDL_TEST_STRING_DIFF_NAMES,
                                         DDCompare::icf_memory | DDCompare::icf_names));
        ASSERT_EQ(a_util::result::SUCCESS,
                  DDCompare::isEqualType("adtf.type.video",
                                         DDL_TEST_STRING,
                                         "adtf.type.video",
                                         DDL_TEST_STRING_DIFF_NAMES,
                                         DDCompare::icf_memory));

        ASSERT_NE(a_util::result::SUCCESS,
                  DDCompare::isEqualType("tTest1",
                                         DDL_TEST_STRING_LAYOUT_TEST,
                                         "tTest2",
                                         DDL_TEST_STRING_LAYOUT_TEST,
                                         DDCompare::icf_memory));

        ASSERT_NE(a_util::result::SUCCESS,
                  DDCompare::isEqualType("adtf.type.video",
                                         DDL_TEST_STRING,
                                         "adtf.type.video",
                                         DDL_TEST_STRING_DIFF_VERSION,
                                         DDCompare::icf_memory | DDCompare::icf_versions));
        ASSERT_EQ(a_util::result::SUCCESS,
                  DDCompare::isEqualType("adtf.type.video",
                                         DDL_TEST_STRING,
                                         "adtf.type.video",
                                         DDL_TEST_STRING_DIFF_VERSION,
                                         DDCompare::icf_memory));

        ASSERT_NE(a_util::result::SUCCESS,
                  DDCompare::isEqualType("adtf.type.video",
                                         DDL_TEST_STRING,
                                         "adtf.type.video",
                                         DDL_TEST_STRING_DIFF_ARRAY,
                                         DDCompare::icf_memory));

        // dynamic stuff
        ASSERT_EQ(a_util::result::SUCCESS,
                  DDCompare::isEqualType("adtf.type.video",
                                         DDL_TEST_STRING_DYNAMIC,
                                         "adtf.type.video",
                                         DDL_TEST_STRING_DYNAMIC,
                                         DDCompare::icf_memory));
        ASSERT_EQ(a_util::result::SUCCESS,
                  DDCompare::isEqualType("adtf.type.video",
                                         DDL_TEST_STRING_DYNAMIC,
                                         "adtf.type.video",
                                         DDL_TEST_STRING_DYNAMIC_NAME_DIFF,
                                         DDCompare::icf_memory));
        ASSERT_NE(a_util::result::SUCCESS,
                  DDCompare::isEqualType("adtf.type.video",
                                         DDL_TEST_STRING_DYNAMIC,
                                         "adtf.type.video",
                                         DDL_TEST_STRING_DYNAMIC_NAME_DIFF,
                                         DDCompare::icf_memory | DDCompare::icf_names));

        ASSERT_NE(a_util::result::SUCCESS,
                  DDCompare::isEqualType("adtf.type.video",
                                         DDL_TEST_STRING_DYNAMIC,
                                         "adtf.type.video",
                                         DDL_TEST_STRING_DYNAMIC_BIN_DIFF,
                                         DDCompare::icf_memory));

        ASSERT_NE(a_util::result::SUCCESS,
                  DDCompare::isEqualType("adtf.type.video",
                                         DDL_TEST_STRING_DYNAMIC,
                                         "adtf.type.video",
                                         DDL_TEST_STRING_DYNAMIC_BIN_DIFF_2,
                                         DDCompare::icf_memory));

        ASSERT_EQ(a_util::result::SUCCESS,
                  DDCompare::isEqualType("tTest1",
                                         DDL_TEST_STRING_DYNAMIC_DIFF_ARRAY_SPEC,
                                         "tTest1",
                                         DDL_TEST_STRING_DYNAMIC_DIFF_ARRAY_SPEC,
                                         DDCompare::icf_memory));
        ASSERT_NE(a_util::result::SUCCESS,
                  DDCompare::isEqualType("tTest1",
                                         DDL_TEST_STRING_DYNAMIC_DIFF_ARRAY_SPEC,
                                         "tTest2",
                                         DDL_TEST_STRING_DYNAMIC_DIFF_ARRAY_SPEC,
                                         DDCompare::icf_memory));

        // subset
        ASSERT_EQ(a_util::result::SUCCESS,
                  DDCompare::isEqualType("adtf.type.video",
                                         DDL_TEST_STRING_SUBSET,
                                         "adtf.type.video",
                                         DDL_TEST_STRING,
                                         DDCompare::icf_memory | DDCompare::icf_subset));
        ASSERT_NE(a_util::result::SUCCESS,
                  DDCompare::isEqualType("adtf.type.video",
                                         DDL_TEST_STRING,
                                         "adtf.type.video",
                                         DDL_TEST_STRING_SUBSET,
                                         DDCompare::icf_memory | DDCompare::icf_subset));
        ASSERT_NE(a_util::result::SUCCESS,
                  DDCompare::isEqualType("adtf.type.video",
                                         DDL_TEST_STRING_INVALID_SUBSET,
                                         "adtf.type.video",
                                         DDL_TEST_STRING,
                                         DDCompare::icf_memory | DDCompare::icf_subset));
    }
}

/**
 * @detail The building up of a DataDefinition object representation.
 */
TEST(TesterDDCompare, TestisEqualDescriptions)
{
    using namespace ddl;
    // isEqual (descriptions)
    {
        ASSERT_EQ(a_util::result::SUCCESS,
                  DDCompare::isEqual(DDL_TEST_STRING, DDL_TEST_STRING, DDCompare::dcf_everything));

        // header
        {
            a_util::xml::DOM oDom;
            ASSERT_TRUE(oDom.fromString(DDL_TEST_STRING_HEADER));
            a_util::xml::DOMElement oNode;
            ASSERT_TRUE(oDom.findNode("//header/language_version", oNode));
            oNode.setData("123");
            ASSERT_NE(a_util::result::SUCCESS,
                      DDCompare::isEqual(DDL_TEST_STRING_HEADER,
                                         oDom.toString().c_str(),
                                         DDCompare::dcf_header | DDCompare::dcf_descriptions |
                                             DDCompare::dcf_versions));

            ASSERT_EQ(a_util::result::SUCCESS,
                      DDCompare::isEqual(DDL_TEST_STRING_HEADER,
                                         oDom.toString().c_str(),
                                         DDCompare::dcf_header | DDCompare::dcf_descriptions));

            ASSERT_TRUE(oDom.findNode("//header/description", oNode));
            oNode.setData("blah blah");
            ASSERT_NE(a_util::result::SUCCESS,
                      DDCompare::isEqual(DDL_TEST_STRING_HEADER,
                                         oDom.toString().c_str(),
                                         DDCompare::dcf_header | DDCompare::dcf_descriptions));
            ASSERT_EQ(a_util::result::SUCCESS,
                      DDCompare::isEqual(
                          DDL_TEST_STRING_HEADER, oDom.toString().c_str(), DDCompare::dcf_header));

            ASSERT_TRUE(oDom.findNode("//header/date_creation", oNode));
            std::string strOldData = oNode.getData().c_str();
            oNode.setData("20140213");
            ASSERT_NE(a_util::result::SUCCESS,
                      DDCompare::isEqual(
                          DDL_TEST_STRING_HEADER, oDom.toString().c_str(), DDCompare::dcf_header));
            oNode.setData(strOldData);

            ASSERT_TRUE(oDom.findNode("//header/ext_declaration[@key='AnyKey']", oNode));
            oNode.setAttribute("value", "haha");

            ASSERT_NE(a_util::result::SUCCESS,
                      DDCompare::isEqual(
                          DDL_TEST_STRING_HEADER, oDom.toString().c_str(), DDCompare::dcf_header));

            ASSERT_NE(a_util::result::SUCCESS,
                      DDCompare::isEqual(
                          DDL_TEST_STRING_HEADER, oDom.toString().c_str(), DDCompare::dcf_header));

            ASSERT_EQ(a_util::result::SUCCESS,
                      DDCompare::isEqual(DDL_TEST_STRING_HEADER,
                                         oDom.toString().c_str(),
                                         DDCompare::dcf_all & ~DDCompare::dcf_header));
        }

        // datatypes
        {
            a_util::xml::DOM oDom;
            ASSERT_TRUE(oDom.fromString(DDL_TEST_STRING));
            a_util::xml::DOMElement oNode;
            ASSERT_TRUE(oDom.findNode("//datatype[@name='tFloat64']", oNode));
            oNode.setAttribute("description", "foo");
            ASSERT_NE(a_util::result::SUCCESS,
                      DDCompare::isEqual(DDL_TEST_STRING,
                                         oDom.toString().c_str(),
                                         DDCompare::dcf_data_types | DDCompare::dcf_descriptions));
            ASSERT_EQ(a_util::result::SUCCESS,
                      DDCompare::isEqual(
                          DDL_TEST_STRING, oDom.toString().c_str(), DDCompare::dcf_data_types));

            oNode.setAttribute("min", "2000");
            ASSERT_EQ(a_util::result::SUCCESS,
                      DDCompare::isEqual(
                          DDL_TEST_STRING, oDom.toString().c_str(), DDCompare::dcf_data_types));

            ASSERT_NE(a_util::result::SUCCESS,
                      DDCompare::isEqual(DDL_TEST_STRING,
                                         oDom.toString().c_str(),
                                         DDCompare::dcf_data_types |
                                             DDCompare::dcf_visualization_attributes));

            oNode.setAttribute("size", "16");
            ASSERT_NE(a_util::result::SUCCESS,
                      DDCompare::isEqual(
                          DDL_TEST_STRING, oDom.toString().c_str(), DDCompare::dcf_data_types));

            ASSERT_NE(a_util::result::SUCCESS,
                      DDCompare::isEqual(
                          DDL_TEST_STRING, oDom.toString().c_str(), DDCompare::dcf_data_types));

            ASSERT_EQ(a_util::result::SUCCESS,
                      DDCompare::isEqual(DDL_TEST_STRING,
                                         oDom.toString().c_str(),
                                         DDCompare::dcf_all & ~DDCompare::dcf_data_types));
        }

        // structs
        {
            a_util::xml::DOM oDom;
            ASSERT_TRUE(oDom.fromString(DDL_TEST_STRING));
            a_util::xml::DOMElement oNode;
            ASSERT_TRUE(oDom.findNode("//struct[@name='tBitmapFormat']", oNode));
            oNode.setAttribute("comment", "foo");
            ASSERT_NE(a_util::result::SUCCESS,
                      DDCompare::isEqual(DDL_TEST_STRING,
                                         oDom.toString().c_str(),
                                         DDCompare::dcf_structs | DDCompare::dcf_comments |
                                             DDCompare::dcf_versions));
            ASSERT_EQ(a_util::result::SUCCESS,
                      DDCompare::isEqual(DDL_TEST_STRING,
                                         oDom.toString().c_str(),
                                         DDCompare::dcf_structs | DDCompare::dcf_versions));

            oNode.setAttribute("version", "123");
            ASSERT_NE(a_util::result::SUCCESS,
                      DDCompare::isEqual(DDL_TEST_STRING,
                                         oDom.toString().c_str(),
                                         DDCompare::dcf_structs | DDCompare::dcf_versions));
            ASSERT_EQ(a_util::result::SUCCESS,
                      DDCompare::isEqual(
                          DDL_TEST_STRING, oDom.toString().c_str(), DDCompare::dcf_structs));

            std::string strOldAlignment = oNode.getAttribute("alignment").c_str();
            oNode.setAttribute("alignment", "16");
            ASSERT_NE(a_util::result::SUCCESS,
                      DDCompare::isEqual(
                          DDL_TEST_STRING, oDom.toString().c_str(), DDCompare::dcf_structs));

            oNode.setAttribute("alignment", strOldAlignment);

            ASSERT_TRUE(oNode.findNode(".//element[@name='nPixelFormat']", oNode));

            oNode.setAttribute("scale", "1000");
            ASSERT_EQ(a_util::result::SUCCESS,
                      DDCompare::isEqual(
                          DDL_TEST_STRING, oDom.toString().c_str(), DDCompare::dcf_structs));
            ASSERT_NE(a_util::result::SUCCESS,
                      DDCompare::isEqual(DDL_TEST_STRING,
                                         oDom.toString().c_str(),
                                         DDCompare::dcf_structs |
                                             DDCompare::dcf_visualization_attributes));

            oNode.setAttribute("name", "sepp");
            ASSERT_NE(a_util::result::SUCCESS,
                      DDCompare::isEqual(
                          DDL_TEST_STRING, oDom.toString().c_str(), DDCompare::dcf_structs));

            ASSERT_EQ(a_util::result::SUCCESS,
                      DDCompare::isEqual(DDL_TEST_STRING,
                                         oDom.toString().c_str(),
                                         DDCompare::dcf_structs | DDCompare::dcf_no_recursion));

            ASSERT_EQ(a_util::result::SUCCESS,
                      DDCompare::isEqual(DDL_TEST_STRING,
                                         oDom.toString().c_str(),
                                         DDCompare::dcf_all & ~DDCompare::dcf_structs &
                                             ~DDCompare::dcf_streams));
        }

        // streams
        {
            a_util::xml::DOM oDom;
            ASSERT_TRUE(oDom.fromString(DDL_TEST_STRING));
            a_util::xml::DOMElement oNode;
            ASSERT_TRUE(oDom.findNode("//stream[@name='can_stream']", oNode));
            oNode.setAttribute("description", "foo");
            ASSERT_NE(a_util::result::SUCCESS,
                      DDCompare::isEqual(DDL_TEST_STRING,
                                         oDom.toString().c_str(),
                                         DDCompare::dcf_streams | DDCompare::dcf_descriptions));
            ASSERT_EQ(a_util::result::SUCCESS,
                      DDCompare::isEqual(
                          DDL_TEST_STRING, oDom.toString().c_str(), DDCompare::dcf_streams));

            std::string strOldType = oNode.getAttribute("type").c_str();
            oNode.setAttribute("type", "tCanMessageExt");
            ASSERT_NE(a_util::result::SUCCESS,
                      DDCompare::isEqual(
                          DDL_TEST_STRING, oDom.toString().c_str(), DDCompare::dcf_streams));
            oNode.setAttribute("type", strOldType);

            a_util::xml::DOMElement oStructNode;
            ASSERT_TRUE(oNode.findNode(".//struct[@type='tCanMessage']", oStructNode));

            oStructNode.setAttribute("bytepos", "500");
            ASSERT_NE(a_util::result::SUCCESS,
                      DDCompare::isEqual(
                          DDL_TEST_STRING, oDom.toString().c_str(), DDCompare::dcf_streams));

            ASSERT_EQ(a_util::result::SUCCESS,
                      DDCompare::isEqual(DDL_TEST_STRING,
                                         oDom.toString().c_str(),
                                         DDCompare::dcf_streams | DDCompare::dcf_no_recursion));

            ASSERT_NE(a_util::result::SUCCESS,
                      DDCompare::isEqual(
                          DDL_TEST_STRING, oDom.toString().c_str(), DDCompare::dcf_streams));

            ASSERT_EQ(a_util::result::SUCCESS,
                      DDCompare::isEqual(DDL_TEST_STRING,
                                         oDom.toString().c_str(),
                                         DDCompare::dcf_all & ~DDCompare::dcf_streams));

            // check differences in type as well
            ASSERT_TRUE(oDom.fromString(DDL_TEST_STRING));
            ASSERT_TRUE(oDom.findNode("//struct[@name='adtf.core.media_type']", oNode));
            oNode.setAttribute("alignment", "8");
            ASSERT_NE(a_util::result::SUCCESS,
                      DDCompare::isEqual(
                          DDL_TEST_STRING, oDom.toString().c_str(), DDCompare::dcf_streams));
        }

        // base units
        {
            ASSERT_EQ(a_util::result::SUCCESS,
                      DDCompare::isEqual(
                          DDL_TEST_STRING_UNITS, DDL_TEST_STRING_UNITS, DDCompare::dcf_all));

            a_util::xml::DOM oDom;
            ASSERT_TRUE(oDom.fromString(DDL_TEST_STRING_UNITS));
            a_util::xml::DOMElement oNode;
            ASSERT_TRUE(oDom.findNode("//baseunit[@name='Candela']", oNode));
            oNode.setAttribute("description", "foo");
            ASSERT_NE(a_util::result::SUCCESS,
                      DDCompare::isEqual(DDL_TEST_STRING_UNITS,
                                         oDom.toString().c_str(),
                                         DDCompare::dcf_base_units | DDCompare::dcf_descriptions));
            ASSERT_EQ(a_util::result::SUCCESS,
                      DDCompare::isEqual(DDL_TEST_STRING_UNITS,
                                         oDom.toString().c_str(),
                                         DDCompare::dcf_base_units));

            oNode.setAttribute("name", "foo_unit");
            ASSERT_NE(a_util::result::SUCCESS,
                      DDCompare::isEqual(DDL_TEST_STRING_UNITS,
                                         oDom.toString().c_str(),
                                         DDCompare::dcf_base_units));

            ASSERT_NE(a_util::result::SUCCESS,
                      DDCompare::isEqual(DDL_TEST_STRING_UNITS,
                                         oDom.toString().c_str(),
                                         DDCompare::dcf_base_units));

            ASSERT_EQ(a_util::result::SUCCESS,
                      DDCompare::isEqual(DDL_TEST_STRING_UNITS,
                                         oDom.toString().c_str(),
                                         DDCompare::dcf_all & ~DDCompare::dcf_base_units));
        }

        // prefix
        {
            a_util::xml::DOM oDom;
            ASSERT_TRUE(oDom.fromString(DDL_TEST_STRING_UNITS));
            a_util::xml::DOMElement oNode;
            ASSERT_TRUE(oDom.findNode("//prefixes[@name='deca']", oNode));

            std::string strOldName = oNode.getAttribute("name").c_str();
            oNode.setAttribute("name", "deca2");
            ASSERT_NE(a_util::result::SUCCESS,
                      DDCompare::isEqual(
                          DDL_TEST_STRING_UNITS, oDom.toString().c_str(), DDCompare::dcf_prefixes));
            oNode.setAttribute("name", strOldName);

            std::string strOldSymbol = oNode.getAttribute("symbol").c_str();
            oNode.setAttribute("symbol", "XF");

            ASSERT_NE(a_util::result::SUCCESS,
                      DDCompare::isEqual(
                          DDL_TEST_STRING_UNITS, oDom.toString().c_str(), DDCompare::dcf_prefixes));
            ASSERT_NE(a_util::result::SUCCESS,
                      DDCompare::isEqual(DDL_TEST_STRING_UNITS,
                                         oDom.toString().c_str(),
                                         DDCompare::dcf_units)); // a unit that uses the prefix

            oNode.setAttribute("symbol", strOldSymbol);
            ASSERT_EQ(a_util::result::SUCCESS,
                      DDCompare::isEqual(
                          DDL_TEST_STRING_UNITS, oDom.toString().c_str(), DDCompare::dcf_all));
        }

        // units
        {
            a_util::xml::DOM oDom;
            ASSERT_TRUE(oDom.fromString(DDL_TEST_STRING_UNITS));
            a_util::xml::DOMElement oNode;
            ASSERT_TRUE(oDom.findNode("//unit[@name='bar_unit2']", oNode));

            std::string strOldName = oNode.getAttribute("name").c_str();
            oNode.setAttribute("name", "foo_unit");
            ASSERT_NE(a_util::result::SUCCESS,
                      DDCompare::isEqual(
                          DDL_TEST_STRING_UNITS, oDom.toString().c_str(), DDCompare::dcf_units));
            oNode.setAttribute("name", strOldName);

            a_util::xml::DOMElement oNumerator;
            ASSERT_TRUE(oNode.findNode(".//numerator", oNumerator));

            std::string strOldNumerator = oNumerator.getData().c_str();
            oNumerator.setData("200");
            ASSERT_NE(a_util::result::SUCCESS,
                      DDCompare::isEqual(
                          DDL_TEST_STRING_UNITS, oDom.toString().c_str(), DDCompare::dcf_units));
            oNumerator.setData(strOldNumerator);

            a_util::xml::DOMElement oRef;
            ASSERT_TRUE(oNode.findNode(".//refUnit", oRef));
            oRef.setAttribute("power", "100");
            ASSERT_NE(a_util::result::SUCCESS,
                      DDCompare::isEqual(
                          DDL_TEST_STRING_UNITS, oDom.toString().c_str(), DDCompare::dcf_units));

            ASSERT_EQ(a_util::result::SUCCESS,
                      DDCompare::isEqual(DDL_TEST_STRING_UNITS,
                                         oDom.toString().c_str(),
                                         DDCompare::dcf_units | DDCompare::dcf_no_recursion));

            oRef.setAttribute("name", "Kelvin");
            ASSERT_NE(a_util::result::SUCCESS,
                      DDCompare::isEqual(
                          DDL_TEST_STRING_UNITS, oDom.toString().c_str(), DDCompare::dcf_units));

            ASSERT_EQ(a_util::result::SUCCESS,
                      DDCompare::isEqual(DDL_TEST_STRING_UNITS,
                                         oDom.toString().c_str(),
                                         DDCompare::dcf_units | DDCompare::dcf_no_recursion));

            ASSERT_NE(a_util::result::SUCCESS,
                      DDCompare::isEqual(
                          DDL_TEST_STRING_UNITS, oDom.toString().c_str(), DDCompare::dcf_units));

            ASSERT_EQ(a_util::result::SUCCESS,
                      DDCompare::isEqual(DDL_TEST_STRING_UNITS,
                                         oDom.toString().c_str(),
                                         DDCompare::dcf_all & ~DDCompare::dcf_units));
        }

        // enums
        {
            ASSERT_EQ(a_util::result::SUCCESS,
                      DDCompare::isEqual(
                          DDL_TEST_STRING_ENUMS, DDL_TEST_STRING_ENUMS, DDCompare::dcf_all));

            a_util::xml::DOM oDom;
            ASSERT_TRUE(oDom.fromString(DDL_TEST_STRING_ENUMS));
            a_util::xml::DOMElement oNode;
            ASSERT_TRUE(oDom.findNode("//enum[@name='tValueDefinitions2']", oNode));

            std::string strOldType = oNode.getAttribute("type").c_str();
            oNode.setAttribute("type", "tFloat64");
            ASSERT_NE(a_util::result::SUCCESS,
                      DDCompare::isEqual(
                          DDL_TEST_STRING_ENUMS, oDom.toString().c_str(), DDCompare::dcf_enums));
            oNode.setAttribute("type", strOldType);

            a_util::xml::DOMElement oElement;
            ASSERT_TRUE(oNode.findNode(".//element[@name='ELEMENT_TWO2']", oElement));
            oElement.setAttribute("value", "321");

            ASSERT_NE(a_util::result::SUCCESS,
                      DDCompare::isEqual(
                          DDL_TEST_STRING_ENUMS, oDom.toString().c_str(), DDCompare::dcf_enums));

            ASSERT_EQ(
                a_util::result::SUCCESS,
                DDCompare::isEqual(DDL_TEST_STRING_ENUMS,
                                   oDom.toString().c_str(),
                                   DDCompare::dcf_enums | DDCompare::dcf_no_enum_values_check));
            ASSERT_EQ(
                a_util::result::SUCCESS,
                DDCompare::isEqual(DDL_TEST_STRING_ENUMS,
                                   oDom.toString().c_str(),
                                   DDCompare::dcf_enums | DDCompare::dcf_no_enum_values_check));
            ASSERT_NE(a_util::result::SUCCESS,
                      DDCompare::isEqual(
                          DDL_TEST_STRING_ENUMS, oDom.toString().c_str(), DDCompare::dcf_enums));

            ASSERT_EQ(a_util::result::SUCCESS,
                      DDCompare::isEqual(DDL_TEST_STRING_ENUMS,
                                         oDom.toString().c_str(),
                                         DDCompare::dcf_all & ~DDCompare::dcf_enums));
        }
    }
}

/**
 * @detail The building up of a DataDefinition object representation.
 */
TEST(TesterDDCompare, TestisEqualDescriptionsWithStringMethods)
{
    using namespace ddl;
    {
        // Item based string methods

        {
            // enums
            ASSERT_EQ(a_util::result::SUCCESS,
                      DDCompare::isEqualType("tValueDefinitions2",
                                             DDL_TEST_STRING_ENUMS,
                                             "tValueDefinitions2",
                                             DDL_TEST_STRING_ENUMS));

            a_util::xml::DOM oDom;
            ASSERT_TRUE(oDom.fromString(DDL_TEST_STRING_ENUMS));
            a_util::xml::DOMElement oNode;
            ASSERT_TRUE(oDom.findNode("//enum[@name='tValueDefinitions2']", oNode));
            oNode.setAttribute("type", "tBool");
            ASSERT_NE(a_util::result::SUCCESS,
                      DDCompare::isEqualType("tValueDefinitions2",
                                             DDL_TEST_STRING_ENUMS,
                                             "tValueDefinitions2",
                                             oDom.toString().c_str(),
                                             DDCompare::icf_none));
        }

        {
            // prefixes
            ASSERT_EQ(a_util::result::SUCCESS,
                      DDCompare::isEqualPrefix(
                          "deca", DDL_TEST_STRING_UNITS, "deca", DDL_TEST_STRING_UNITS));

            a_util::xml::DOM oDom;
            ASSERT_TRUE(oDom.fromString(DDL_TEST_STRING_UNITS));
            a_util::xml::DOMElement oNode;
            ASSERT_TRUE(oDom.findNode("//prefixes[@name='deca']", oNode));
            oNode.setAttribute("symbol", "XF");
            ASSERT_NE(a_util::result::SUCCESS,
                      DDCompare::isEqualPrefix("deca",
                                               DDL_TEST_STRING_UNITS,
                                               "deca",
                                               oDom.toString().c_str(),
                                               DDCompare::icf_none));
        }

        {
            // units
            ASSERT_EQ(a_util::result::SUCCESS,
                      DDCompare::isEqualUnit(
                          "bar_unit2", DDL_TEST_STRING_UNITS, "bar_unit2", DDL_TEST_STRING_UNITS));

            a_util::xml::DOM oDom;
            ASSERT_TRUE(oDom.fromString(DDL_TEST_STRING_UNITS));
            a_util::xml::DOMElement oNode;
            ASSERT_TRUE(oDom.findNode("//unit[@name='bar_unit2']", oNode));
            a_util::xml::DOMElement oNumerator;
            ASSERT_TRUE(oNode.findNode(".//numerator", oNumerator));
            oNumerator.setData("200");

            ASSERT_NE(a_util::result::SUCCESS,
                      DDCompare::isEqualUnit("bar_unit2",
                                             DDL_TEST_STRING_UNITS,
                                             "bar_unit2",
                                             oDom.toString().c_str(),
                                             DDCompare::icf_none));
        }

        {
            // structs
            ASSERT_EQ(a_util::result::SUCCESS,
                      DDCompare::isEqualType(
                          "tBitmapFormat", DDL_TEST_STRING, "tBitmapFormat", DDL_TEST_STRING));

            a_util::xml::DOM oDom;
            ASSERT_TRUE(oDom.fromString(DDL_TEST_STRING));
            a_util::xml::DOMElement oNode;
            ASSERT_TRUE(oDom.findNode("//struct[@name='tBitmapFormat']", oNode));
            oNode.setAttribute("alignment", "2");

            ASSERT_NE(a_util::result::SUCCESS,
                      DDCompare::isEqualType("tBitmapFormat",
                                             DDL_TEST_STRING,
                                             "tBitmapFormat",
                                             oDom.toString().c_str(),
                                             DDCompare::icf_memory));
        }

        {
            // datatypes
            ASSERT_EQ(a_util::result::SUCCESS,
                      DDCompare::isEqualType("tUInt8", DDL_TEST_STRING, "tUInt8", DDL_TEST_STRING));

            a_util::xml::DOM oDom;
            ASSERT_TRUE(oDom.fromString(DDL_TEST_STRING));
            a_util::xml::DOMElement oNode;
            ASSERT_TRUE(oDom.findNode("//datatype[@name='tUInt8']", oNode));
            oNode.setAttribute("size", "9");

            ASSERT_NE(a_util::result::SUCCESS,
                      DDCompare::isEqualType("tUInt8",
                                             DDL_TEST_STRING,
                                             "tUInt8",
                                             oDom.toString().c_str(),
                                             DDCompare::icf_memory));

            oNode.setAttribute("size", "8");
            ASSERT_EQ(a_util::result::SUCCESS,
                      DDCompare::isEqualType("tUInt8",
                                             DDL_TEST_STRING,
                                             "tUInt8",
                                             oDom.toString().c_str(),
                                             DDCompare::icf_memory));

            oNode.setAttribute("max", "10"); // check attribute set only on one of the two
            std::string strHelper = oDom.toString().c_str();
            ASSERT_EQ(a_util::result::SUCCESS,
                      DDCompare::isEqualType("tUInt8",
                                             DDL_TEST_STRING,
                                             "tUInt8",
                                             strHelper,
                                             DDCompare::icf_names | DDCompare::icf_memory));
            ASSERT_NE(a_util::result::SUCCESS,
                      DDCompare::isEqualType("tUInt8",
                                             DDL_TEST_STRING,
                                             "tUInt8",
                                             strHelper,
                                             DDCompare::icf_names |
                                                 DDCompare::icf_visualizations_attributes));

            // check set on both but different
            oNode.setAttribute("max", "11");
            ASSERT_EQ(a_util::result::SUCCESS,
                      DDCompare::isEqualType("tUInt8",
                                             oDom.toString().c_str(),
                                             "tUInt8",
                                             strHelper,
                                             DDCompare::icf_names | DDCompare::icf_memory));
            ASSERT_NE(a_util::result::SUCCESS,
                      DDCompare::isEqualType("tUInt8",
                                             oDom.toString().c_str(),
                                             "tUInt8",
                                             strHelper,
                                             DDCompare::icf_names |
                                                 DDCompare::icf_visualizations_attributes));
        }

        {
            // streams
            ASSERT_EQ(a_util::result::SUCCESS,
                      DDCompare::isEqualStream(
                          "video_stream", DDL_TEST_STRING, "video_stream", DDL_TEST_STRING));

            a_util::xml::DOM oDom;
            ASSERT_TRUE(oDom.fromString(DDL_TEST_STRING));
            a_util::xml::DOMElement oNode;
            ASSERT_TRUE(oDom.findNode("//stream[@name='video_stream']", oNode));
            oNode.setAttribute("type", "adtf.type.audio");

            ASSERT_NE(a_util::result::SUCCESS,
                      DDCompare::isEqualStream("video_stream",
                                               DDL_TEST_STRING,
                                               "video_stream",
                                               oDom.toString().c_str(),
                                               DDCompare::icf_memory));

            // check recursion from stream to struct to unit to prefix.
            ASSERT_TRUE(oDom.fromString(DDL_TEST_STRING_STREAM_TO_PREFIX));
            ASSERT_EQ(a_util::result::SUCCESS,
                      DDCompare::isEqualStream("a",
                                               DDL_TEST_STRING_STREAM_TO_PREFIX,
                                               "a",
                                               oDom.toString().c_str(),
                                               DDCompare::icf_units));
            ASSERT_TRUE(oDom.findNode("//prefixes[@name='TestPrefix']", oNode));
            oNode.setAttribute("symbol", "p2");
            ASSERT_NE(a_util::result::SUCCESS,
                      DDCompare::isEqualStream("a",
                                               DDL_TEST_STRING_STREAM_TO_PREFIX,
                                               "a",
                                               oDom.toString().c_str(),
                                               DDCompare::icf_units));

            // check recursion from stream to struct to unit to baseunit.
            ASSERT_TRUE(oDom.fromString(DDL_TEST_STRING_STREAM_TO_PREFIX));
            ASSERT_TRUE(oDom.findNode("//baseunit[@name='nou']", oNode));
            oNode.setAttribute("symbol", "1");
            ASSERT_NE(a_util::result::SUCCESS,
                      DDCompare::isEqualStream("a",
                                               DDL_TEST_STRING_STREAM_TO_PREFIX,
                                               "a",
                                               oDom.toString().c_str(),
                                               DDCompare::icf_units));
        }

        {
            auto imported_dd1 = DDString::fromXMLString(DDL_TEST_STRING_UNITS);
            const auto& pRefUnit1 = imported_dd1.getUnits().get("bar_unit")->getRefUnits().front();
            ASSERT_EQ(a_util::result::SUCCESS,
                      DDCompare::isEqual(
                          pRefUnit1, imported_dd1, pRefUnit1, imported_dd1, DDCompare::icf_units));

            a_util::xml::DOM oDom;
            ASSERT_TRUE(oDom.fromString(DDL_TEST_STRING_UNITS));
            a_util::xml::DOMElement oNode;
            ASSERT_TRUE(oDom.findNode("//refUnit", oNode));
            oNode.setAttribute("prefix", "giga");

            auto imported_dd2 = DDString::fromXMLString(oDom.toString().c_str());

            const auto& pRefUnit2 = imported_dd2.getUnits().get("bar_unit")->getRefUnits().front();
            ASSERT_NE(a_util::result::SUCCESS,
                      DDCompare::isEqual(
                          pRefUnit1, imported_dd2, pRefUnit2, imported_dd2, DDCompare::icf_none));
            ASSERT_NE(a_util::result::SUCCESS,
                      DDCompare::isEqual(pRefUnit1,
                                         imported_dd2,
                                         pRefUnit2,
                                         imported_dd2,
                                         DDCompare::icf_no_recursion | DDCompare::icf_names));
        }
    }
}
