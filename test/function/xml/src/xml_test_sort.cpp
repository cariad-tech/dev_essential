/**
 * @file
 * XML test_sort implementation
 *
 * Copyright @ 2022 VW Group. All rights reserved.
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
 */

#include <a_util/filesystem.h>
#include <a_util/xml.h>

#include <gtest/gtest.h>

namespace {

using namespace a_util;

void sortNodes(xml::DOMElement& root, const SortingOrder order)
{
    size_t nodes;
    // sort all nodes below header by node name
    EXPECT_TRUE(root.sortNodes("header/*", order));
    // sort all nodes named header/ext_declaration by attribute key
    EXPECT_EQ(root.sortNodes("header/ext_declaration", "key", order), 3);
    // sort all nodes below units by node name
    EXPECT_TRUE(root.sortNodes("units/*", order));
    // sort all nodes named units/baseunit by attribute name
    EXPECT_EQ(root.sortNodes("units/baseunit", "name", order), 4);
    // sort all nodes named units/prefixes by attribute name
    EXPECT_EQ(root.sortNodes("units/prefixes", "name", order), 4);
    // sort all nodes named units/unit by attribute name
    EXPECT_EQ(nodes = root.sortNodes("units/unit", "name", order), 2);
    // sort all nodes below all units/unit by node name
    EXPECT_TRUE(root.sortNodes("units/unit[*]/*", order, nodes));
    // sort all nodes refUnit of all units/unit by attribute name
    // expect false, because not all nodes unit contain nodes refUnit
    EXPECT_FALSE(root.sortNodes("units/unit[*]/refUnit", "name", order, nodes));
    // sort all nodes named datatypes/datatype by attribute type
    EXPECT_EQ(root.sortNodes("datatypes/datatype", "type", order), 4);
    // sort all nodes named enums/enum by attribute name
    EXPECT_EQ(nodes = root.sortNodes("enums/enum", "name", order), 2);
    // sort all nodes element of all enums/enum by attribute name
    EXPECT_TRUE(root.sortNodes("enums/enum[*]/element", "name", order, nodes));
    // sort all nodes named structs/struct by attribute name
    EXPECT_EQ(root.sortNodes("structs/struct", "name", order), 2);
    // sort all nodes named streammetatypes/streammetatype by attribute name
    EXPECT_EQ(nodes = root.sortNodes("streammetatypes/streammetatype", "name", order), 3);
    // sort all nodes property of all streammetatypes/streammetatype by attribute name
    // expect false, because not all nodes streammetatype contain nodes property
    EXPECT_FALSE(
        root.sortNodes("streammetatypes/streammetatype[*]/property", "name", order, nodes));
    // sort all nodes named streams/stream by attribute name
    EXPECT_EQ(root.sortNodes("streams/stream", "name", order), 2);
}

TEST(xml_test_sort, TestSortAscending)
{
    xml::DOM dom;
    xml::DOMElement root;

    ASSERT_TRUE(dom.load(TEST_FILES_DIR "/test_sort.xml"));
    ASSERT_FALSE((root = dom.getRoot()).isNull());

    sortNodes(root, SortingOrder::ascending);
    ASSERT_TRUE(dom.save(TEST_FILES_WRITE_DIR "/test_sort_ascending.xml"));

    EXPECT_EQ(0,
              filesystem::compareFiles(TEST_FILES_DIR "/test_sort_ascending.xml",
                                       TEST_FILES_WRITE_DIR "/test_sort_ascending.xml"));
}

TEST(xml_test_sort, TestSortDescending)
{
    xml::DOM dom;
    xml::DOMElement root;

    ASSERT_TRUE(dom.load(TEST_FILES_DIR "/test_sort.xml"));
    ASSERT_FALSE((root = dom.getRoot()).isNull());

    sortNodes(root, SortingOrder::descending);
    ASSERT_TRUE(dom.save(TEST_FILES_WRITE_DIR "/test_sort_descending.xml"));

    EXPECT_EQ(0,
              filesystem::compareFiles(TEST_FILES_DIR "/test_sort_descending.xml",
                                       TEST_FILES_WRITE_DIR "/test_sort_descending.xml"));
}

} // namespace