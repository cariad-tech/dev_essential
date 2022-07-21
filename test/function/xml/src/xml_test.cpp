/**
 * @file
 * XML test implementation
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
 */

#include <a_util/xml.h>

#include <gtest/gtest.h>

const static std::string test_xml = "<?xml version=\"1.0\"?>\n"
                                    "<root attr=\"value\">\n"
                                    "    <blub>bla</blub>\n"
                                    "</root>\n";

// Test all basic DOM methods
TEST(xml_test, TestDOM)
{
    using a_util::xml::DOM;

    // CTOR
    DOM dom;
    EXPECT_EQ(dom.getRoot().getName(), "");

    // fromString
    EXPECT_FALSE(dom.fromString(""));
    EXPECT_EQ(dom.getLastError(), "No document element found");
    EXPECT_TRUE(dom.fromString(test_xml));

    EXPECT_EQ(dom.getRoot().getName(), "root");
    EXPECT_TRUE(dom.getRoot().getChildren().size() == 1);
    EXPECT_EQ(dom.getRoot().getAttribute("attr"), "value");
    EXPECT_EQ("blub", dom.getRoot().getChild("blub").getName());
    EXPECT_EQ(dom.getRoot().getChild("blub").getName(), "blub");
    EXPECT_EQ(dom.getRoot().getChild("blub").getData(), "bla");

    // toString
    EXPECT_EQ(dom.toString(), test_xml);

    // CCTOR
    DOM dom_copy = dom;
    EXPECT_EQ(dom.toString(), dom_copy.toString());
    EXPECT_EQ(dom, dom_copy);

    // Assignment
    EXPECT_TRUE(dom_copy.getRoot().setName("root2"));
    EXPECT_NE(dom, dom_copy);
    dom = dom_copy;
    EXPECT_EQ(dom.getRoot().getName(), "root2");
    EXPECT_EQ(dom, dom_copy);

    // reset
    dom.reset();
    EXPECT_EQ(dom.getRoot().getName(), "");
    EXPECT_TRUE(dom.getRoot().getChildren().empty());
    EXPECT_EQ(dom_copy.getRoot().getName(), "root2");

    // load/save
    EXPECT_FALSE(dom.save(""));
    EXPECT_EQ(dom.getLastError(), "Failed to save dom to file");
    EXPECT_TRUE(dom.fromString(test_xml));
    EXPECT_TRUE(dom.save(TEST_FILES_WRITE_DIR "/test.xml"));
    dom.reset();
    EXPECT_FALSE(dom.load(""));
    EXPECT_EQ(dom.getLastError(), "File was not found");
    EXPECT_TRUE(dom.load(TEST_FILES_WRITE_DIR "/test.xml"));
    EXPECT_EQ(dom.toString(), test_xml);
}

// Test creation of a DOM using the provided methods
TEST(xml_test, TestDOMCreate)
{
    using a_util::xml::DOM;
    using a_util::xml::DOMElement;

    DOM dom;
    DOMElement root = dom.getRoot();

    // fill dom
    EXPECT_TRUE(root.setName("Root"));

    // createChild
    DOMElement child02 = root.createChild("Child02");

    // createChildBefore
    DOMElement child01 = root.createChildBefore("Child01", child02);
    EXPECT_TRUE(child01.setAttribute("attr", "value"));

    DOMElement child11 = child01.createChild("Child11");
    EXPECT_TRUE(child11.setData("data"));

    DOMElement child12 = child01.createChild("Child12");

    DOMElement child21 = child12.createChild("Child21");
    EXPECT_TRUE(child21.setAttribute("attr1", "value1"));
    EXPECT_TRUE(child21.setAttribute("attr2", "value2"));

    // dom now looks like this:
    // Root
    //   Child01 attr=value
    //     Child11
    //       data
    //     Child12
    //       Child21 attr1=value1 attr2=value2
    //   Child02

    // getParent & getChild
    EXPECT_EQ(root.getName(), "Root");
    EXPECT_EQ(root.getChild("Child01"), child01);
    EXPECT_EQ(child01.getParent(), root);
    EXPECT_EQ(child01.getAttribute("attr"), "value");
    EXPECT_EQ(child01.getChild("Child11"), child11);
    EXPECT_EQ(child11.getParent(), child01);
    EXPECT_EQ(child11.getData(), "data");
    EXPECT_EQ(child01.getChild("Child12"), child12);
    EXPECT_EQ(child12.getParent(), child01);
    EXPECT_EQ(child12.getChild("Child21"), child21);
    EXPECT_EQ(child21.getParent(), child12);
    EXPECT_EQ(child21.getAttribute("attr1"), "value1");
    EXPECT_EQ(child21.getAttribute("attr2"), "value2");
    EXPECT_EQ(root.getChild("Child02"), child02);
    EXPECT_EQ(child02.getParent(), root);
    EXPECT_TRUE(child02.getChildren().empty());

    // removeChild
    EXPECT_FALSE(child01.removeChild(""));
    EXPECT_TRUE(child01.removeChild("Child12"));
    EXPECT_EQ(DOMElement(), child01.getChild("Child12"));
    EXPECT_EQ(child01.getChildren().size(), 1);

    EXPECT_TRUE(root.removeChild("Child01"));
    EXPECT_EQ(DOMElement(), root.getChild("Child01"));
    EXPECT_EQ(root.getChildren().size(), 1);
    EXPECT_EQ(root.getChild("Child02"), child02);
}

// Test all basic methods of DOMElement
TEST(xml_test, TestDOMElement)
{
    using a_util::xml::DOM;
    using a_util::xml::DOMElement;

    DOM dom;
    EXPECT_TRUE(dom.fromString(test_xml));
    DOMElement root = dom.getRoot();

    // getName/setName
    EXPECT_EQ(root.getName(), "root");
    EXPECT_TRUE(root.setName("Root"));
    EXPECT_EQ(root.getName(), "Root");

    // getDocument
    EXPECT_EQ(root.getDocument(), dom);
    EXPECT_EQ(root.getChild("blub").getDocument(), dom);

    // getParent
    EXPECT_EQ(root.getChild("blub").getParent(), root);

    // getPath
    EXPECT_EQ(root.getPath(), "/Root");
    EXPECT_EQ(root.getChild("blub").getPath(), "/Root/blub");

    // getData/setData
    EXPECT_EQ(root.getChild("blub").getData(), "bla");
    EXPECT_TRUE(root.getChild("blub").setData("blablubb"));
    EXPECT_EQ(root.getChild("blub").getData(), "blablubb");

    // setData has no effect if the node is of the wrong type
    EXPECT_EQ(root.getData(), "");
    EXPECT_FALSE(root.setData("blablubb"));
    EXPECT_EQ(root.getData(), "");

    // hasAttribute, getAttributes, getAttribute, setAttribute, eraseAttribute
    EXPECT_TRUE(root.hasAttribute("attr"));
    EXPECT_EQ(root.getAttribute("attr"), "value");
    EXPECT_EQ(root.getAttribute("nonexisting", "bla"), "bla");
    EXPECT_FALSE(root.setAttribute("", ""));
    EXPECT_TRUE(root.setAttribute("attr", "new_value"));
    EXPECT_EQ(root.getAttribute("attr"), "new_value");
    EXPECT_FALSE(root.eraseAttribute(""));
    EXPECT_TRUE(root.eraseAttribute("attr"));
    EXPECT_FALSE(root.hasAttribute("attr"));
}

// Test XPath queries
TEST(xml_test, TestQueries)
{
    using a_util::xml::DOM;
    using a_util::xml::DOMElement;
    using a_util::xml::DOMElementList;

    // findNode, findNodes
    {
        DOM dom;
        EXPECT_TRUE(dom.load(TEST_FILES_DIR "/test.xml"));

        DOMElementList node_list;
        DOMElement element;

        EXPECT_TRUE(dom.findNode("level_1/level_2[@l1id='3']/trueunique", element));
        EXPECT_TRUE(dom.findNodes("level_1/level_2[@l1id='3']/trueunique", node_list));
        EXPECT_TRUE(node_list.size() == 1);
        node_list.clear();

        EXPECT_FALSE(dom.findNode("level_1/level_2[@l1id='2']/trueunique", element));
        EXPECT_FALSE(dom.findNodes("level_1/level_2[@l1id='2']/trueunique", node_list));
        EXPECT_EQ(DOMElement(), element);
        EXPECT_TRUE(node_list.size() == 0);

        ASSERT_TRUE(dom.findNode("level_1[@id='1']/level_2[@id='2']", element));
        EXPECT_TRUE(dom.findNodes("level_1[@id='1']/level_2[@id='2']", node_list));
        EXPECT_EQ(element.getAttribute("name"), "beta");
        EXPECT_TRUE(node_list.size() == 1);

        ASSERT_TRUE(dom.findNode("level_1/level_2[@id='2'][@name='beta']", element));
        EXPECT_TRUE(dom.findNodes("level_1/level_2[@id='2'][@l1id='1']", node_list));
        EXPECT_EQ(element.getAttribute("name"), "beta");
        EXPECT_TRUE(node_list.size() == 1);

        EXPECT_TRUE(dom.findNode("level_1/*[@l1id='1']", element));
        EXPECT_TRUE(dom.findNodes("level_1/*[@l1id='1']", node_list));
        EXPECT_TRUE(3 == node_list.size());

        EXPECT_FALSE(dom.findNode("level_1/*[@l1id='7']", element));
        EXPECT_FALSE(dom.findNodes("level_1/*[@l1id='7']", node_list));
        EXPECT_TRUE(0 == node_list.size());
        EXPECT_EQ(DOMElement(), element);

        ASSERT_TRUE(dom.findNode("level_1/*[@id='3'][@l1id='1']", element));
        EXPECT_TRUE(dom.findNodes("level_1/*[@id='3'][@l1id='1']", node_list));
        EXPECT_TRUE(1 == node_list.size());
        EXPECT_EQ(element.getAttribute("name"), "gamma");

        EXPECT_FALSE(dom.findNode("level_1/*[@id='3'][@l1id='7']", element));
        EXPECT_FALSE(dom.findNodes("level_1/*[@id='3'][@l1id='7']", node_list));
        EXPECT_TRUE(0 == node_list.size());
        EXPECT_EQ(DOMElement(), element);

        ASSERT_TRUE(dom.findNode("*", element));
        EXPECT_TRUE(dom.findNodes("*", node_list));
        EXPECT_TRUE(3 == node_list.size());
        EXPECT_EQ(element.getAttribute("id"), "1");

        ASSERT_TRUE(dom.findNode("level_1[@id='3']/*/unique", element));
        EXPECT_TRUE(dom.findNodes("level_1[@id='3']/*/unique", node_list));
        EXPECT_TRUE(3 == node_list.size());
        EXPECT_EQ(element.getAttribute("description"), "unique_3");

        ASSERT_TRUE(dom.findNode("level_1[@id='3']/*", element));
        EXPECT_TRUE(dom.findNodes("level_1[@id='3']/*", node_list));
        EXPECT_TRUE(3 == node_list.size());
        EXPECT_EQ(element.getAttribute("id"), "1");

        ASSERT_TRUE(dom.findNode("/root/*/*/*", element));
        EXPECT_TRUE(dom.findNodes("/root/*/*/*", node_list));
        EXPECT_TRUE(10 == node_list.size());
        EXPECT_EQ(element.getAttribute("description"), "unique");

        ASSERT_TRUE(dom.findNode("/root/*/level_2[@id='2']/*", element));
        EXPECT_TRUE(dom.findNodes("/root/*/level_2[@id='2']/*", node_list));
        EXPECT_TRUE(3 == node_list.size());
        EXPECT_EQ(element.getAttribute("description"), "unique");
    }
    {
        DOM dom;
        DOMElementList node_list;
        EXPECT_TRUE(dom.load(TEST_FILES_DIR "/test.xml"));
        DOMElement element;
        DOMElement root_element = dom.getRoot();

        EXPECT_TRUE(root_element.findNode("level_1/level_2[@l1id='3']/trueunique", element));
        EXPECT_TRUE(root_element.findNodes("level_1/level_2[@l1id='3']/trueunique", node_list));
        EXPECT_TRUE(node_list.size() == 1);
        node_list.clear();

        EXPECT_FALSE(root_element.findNode("level_1/level_2[@l1id='2']/trueunique", element));
        EXPECT_FALSE(root_element.findNodes("level_1/level_2[@l1id='2']/trueunique", node_list));
        EXPECT_EQ(DOMElement(), element);
        EXPECT_TRUE(node_list.size() == 0);

        ASSERT_TRUE(root_element.findNode("level_1[@id='1']/level_2[@id='2']", element));
        EXPECT_TRUE(root_element.findNodes("level_1[@id='1']/level_2[@id='2']", node_list));
        EXPECT_EQ(element.getAttribute("name"), "beta");
        EXPECT_TRUE(node_list.size() == 1);

        ASSERT_TRUE(root_element.findNode("level_1/level_2[@id='2'][@name='beta']", element));
        EXPECT_TRUE(root_element.findNodes("level_1/level_2[@id='2'][@l1id='1']", node_list));
        EXPECT_EQ(element.getAttribute("name"), "beta");
        EXPECT_TRUE(node_list.size() == 1);

        EXPECT_TRUE(root_element.findNode("level_1/*[@l1id='1']", element));
        EXPECT_TRUE(root_element.findNodes("level_1/*[@l1id='1']", node_list));
        EXPECT_TRUE(3 == node_list.size());

        EXPECT_FALSE(root_element.findNode("level_1/*[@l1id='7']", element));
        EXPECT_FALSE(root_element.findNodes("level_1/*[@l1id='7']", node_list));
        EXPECT_TRUE(0 == node_list.size());
        EXPECT_EQ(DOMElement(), element);

        ASSERT_TRUE(root_element.findNode("level_1/*[@id='3'][@l1id='1']", element));
        EXPECT_TRUE(root_element.findNodes("level_1/*[@id='3'][@l1id='1']", node_list));
        EXPECT_TRUE(1 == node_list.size());
        EXPECT_EQ(element.getAttribute("name"), "gamma");

        EXPECT_FALSE(root_element.findNode("level_1/*[@id='3'][@l1id='7']", element));
        EXPECT_FALSE(root_element.findNodes("level_1/*[@id='3'][@l1id='7']", node_list));
        EXPECT_TRUE(0 == node_list.size());
        EXPECT_EQ(DOMElement(), element);
    }
    {
        DOM dom;
        DOMElementList node_list;
        EXPECT_TRUE(dom.load(TEST_FILES_DIR "/test.xml"));
        DOMElement element;
        DOMElement root_element;
        EXPECT_TRUE(dom.findNode("/root/level_1/level_2[@id='2'][@l1id='2']", root_element));

        EXPECT_TRUE(root_element.findNode("/root/level_1/level_2[@l1id='3']/trueunique", element));
        EXPECT_TRUE(
            root_element.findNodes("/root/level_1/level_2[@l1id='3']/trueunique", node_list));
        EXPECT_TRUE(node_list.size() == 1);
        node_list.clear();

        EXPECT_FALSE(root_element.findNode("/root/level_1/level_2[@l1id='2']/trueunique", element));
        EXPECT_FALSE(
            root_element.findNodes("/root/level_1/level_2[@l1id='2']/trueunique", node_list));
        EXPECT_EQ(DOMElement(), element);
        EXPECT_TRUE(node_list.size() == 0);

        ASSERT_TRUE(root_element.findNode("/root/level_1[@id='1']/level_2[@id='2']", element));
        EXPECT_TRUE(root_element.findNodes("/root/level_1[@id='1']/level_2[@id='2']", node_list));
        EXPECT_EQ(element.getAttribute("name"), "beta");
        EXPECT_TRUE(node_list.size() == 1);

        ASSERT_TRUE(root_element.findNode("/root/level_1/level_2[@id='2'][@name='beta']", element));
        EXPECT_TRUE(root_element.findNodes("/root/level_1/level_2[@id='2'][@l1id='1']", node_list));
        EXPECT_EQ(element.getAttribute("name"), "beta");
        EXPECT_TRUE(node_list.size() == 1);

        EXPECT_TRUE(root_element.findNode("/root/level_1/*[@l1id='1']", element));
        EXPECT_TRUE(root_element.findNodes("/root/level_1/*[@l1id='1']", node_list));
        EXPECT_TRUE(3 == node_list.size());

        EXPECT_FALSE(root_element.findNode("/root/level_1/*[@l1id='7']", element));
        EXPECT_FALSE(root_element.findNodes("/root/level_1/*[@l1id='7']", node_list));
        EXPECT_TRUE(0 == node_list.size());
        EXPECT_EQ(DOMElement(), element);

        ASSERT_TRUE(root_element.findNode("/root/level_1/*[@id='3'][@l1id='1']", element));
        EXPECT_TRUE(root_element.findNodes("/root/level_1/*[@id='3'][@l1id='1']", node_list));
        EXPECT_TRUE(1 == node_list.size());
        EXPECT_EQ(element.getAttribute("name"), "gamma");

        EXPECT_FALSE(root_element.findNode("/root/level_1/*[@id='3'][@l1id='7']", element));
        EXPECT_FALSE(root_element.findNodes("/root/level_1/*[@id='3'][@l1id='7']", node_list));
        EXPECT_TRUE(0 == node_list.size());
        EXPECT_EQ(DOMElement(), element);
    }
    {
        using a_util::xml::DOMAttributes;

        // recursive tests
        DOM dom;
        DOMElementList node_list;
        EXPECT_TRUE(dom.load(TEST_FILES_DIR "/test.xml"));
        DOMElement dom_element;

        ASSERT_TRUE(dom.findNode("//level_1", dom_element));
        EXPECT_TRUE(dom.findNodes("//level_1", node_list));
        EXPECT_TRUE(3 == node_list.size());
        EXPECT_EQ(dom_element.getAttribute("id"), "1");

        ASSERT_TRUE(dom.findNode("//level_2", dom_element));
        EXPECT_TRUE(dom.findNodes("//level_2", node_list));
        EXPECT_TRUE(9 == node_list.size());
        EXPECT_EQ(dom_element.getAttribute("id"), "1");

        DOMAttributes level2_first_node_expected_attributes;
        level2_first_node_expected_attributes["id"] = "1";
        level2_first_node_expected_attributes["level"] = "2";
        level2_first_node_expected_attributes["l1id"] = "1";
        level2_first_node_expected_attributes["name"] = "alpha";
        EXPECT_EQ(dom_element.getAttributes(), level2_first_node_expected_attributes);

        ASSERT_TRUE(dom.findNode("/root//level_2/unique[@description='unique_2']", dom_element));
        EXPECT_TRUE(dom.findNodes("/root//level_2/unique[@description='unique_2']", node_list));
        EXPECT_TRUE(3 == node_list.size());
        EXPECT_EQ(dom_element.getAttribute("description"), "unique_2");

        DOMElement node;
        ASSERT_TRUE(dom.findNode("level_1", node));
        ASSERT_TRUE(node.findNode(".//unique", dom_element));
        EXPECT_TRUE(node.findNodes(".//unique", node_list));
        EXPECT_TRUE(3 == node_list.size());
        EXPECT_EQ(dom_element.getAttribute("description"), "unique");

        ASSERT_TRUE(dom.findNode("//root", dom_element));
        EXPECT_TRUE(dom.findNodes("//root", node_list));
        EXPECT_TRUE(1 == node_list.size());
        EXPECT_EQ(dom_element.getName(), "root");

        ASSERT_TRUE(dom.findNode("//*", dom_element));
        EXPECT_TRUE(dom.findNodes("//*", node_list));

        EXPECT_TRUE(23 == node_list.size());
        EXPECT_EQ(dom_element.getName(), "root");

        ASSERT_TRUE(dom.findNode(".//*", dom_element));
        EXPECT_TRUE(dom.findNodes(".//*", node_list));
        EXPECT_TRUE(22 == node_list.size());
        EXPECT_EQ(dom_element.getName(), "level_1");

        ASSERT_TRUE(dom.findNode("/*//*", dom_element));
        EXPECT_TRUE(dom.findNodes("/*//*", node_list));
        EXPECT_TRUE(22 == node_list.size());
        EXPECT_EQ(dom_element.getName(), "level_1");
    }
    {
        // logical expressions on attributes
        DOM dom;
        DOMElementList node_list;
        EXPECT_TRUE(dom.load(TEST_FILES_DIR "/test.xml"));
        DOMElement dom_element;

        EXPECT_TRUE(dom.findNode("/root/*/*[@id='2' and @name='epsilon']/unique", dom_element));
        EXPECT_TRUE(dom.findNodes("/root/*/*[@id='2' and @name='epsilon']/unique", node_list));
        EXPECT_TRUE(1 == node_list.size());
        ASSERT_TRUE(dom.findNode("/root/*/*[@id='2' and @name='epsilon']/unique", dom_element));
        EXPECT_TRUE(dom.findNodes("/root/*/*[@id='2' and @name='epsilon']/unique", node_list));
        EXPECT_TRUE(1 == node_list.size());
        EXPECT_EQ(dom_element.getAttribute("description"), "unique_2");

        EXPECT_TRUE(dom.findNode("/root/*/*[@id='3' or @name='epsilon']/unique", dom_element));
        EXPECT_TRUE(dom.findNodes("/root/*/*[@id='3' or @name='epsilon']/unique", node_list));
        EXPECT_TRUE(4 == node_list.size());
        ASSERT_TRUE(dom.findNode("/root/*/*[@id='3' or @name='epsilon']/unique", dom_element));
        EXPECT_TRUE(dom.findNodes("/root/*/*[@id='3' or @name='epsilon']/unique", node_list));
        EXPECT_TRUE(4 == node_list.size());
        EXPECT_EQ(dom_element.getAttribute("description"), "unique");
    }

    {
        // search attribute name
        DOM dom;
        DOMElementList node_list;
        EXPECT_TRUE(dom.load(TEST_FILES_DIR "/test.xml"));
        DOMElement dom_element;

        EXPECT_TRUE(dom.findNode(".//*[@id and @level and @l1id and @name]", dom_element));
        EXPECT_TRUE(dom.findNodes(".//*[@id and @level and @l1id and @name]", node_list));
        EXPECT_TRUE(9 == node_list.size());

        EXPECT_TRUE(dom.findNode(".//*[@id]", dom_element));
        EXPECT_TRUE(dom.findNodes(".//*[@id]", node_list));
        EXPECT_TRUE(12 == node_list.size());

        EXPECT_TRUE(dom.findNode(".//*[@description]", dom_element));
        EXPECT_TRUE(dom.findNodes(".//*[@description]", node_list));
        EXPECT_TRUE(9 == node_list.size());

        EXPECT_TRUE(dom.findNode(".//*[@id='3' and @l1id and @level and @name]", dom_element));
        EXPECT_TRUE(dom.findNodes(".//*[@id='3' and @l1id and @level and @name]", node_list));
        EXPECT_TRUE(3 == node_list.size());
    }
}

TEST(xml_test, TestEmptyDOM)
{
    using a_util::xml::DOM;

    EXPECT_TRUE(DOM().isNull());

    { // not empty after loading from file
        DOM dom;
        ASSERT_TRUE(dom.load(TEST_FILES_DIR "/test.xml"));
        EXPECT_FALSE(dom.isNull());
        dom.reset();
        EXPECT_TRUE(dom.isNull());
    }

    { // not empty after initializing from string
        DOM dom;
        ASSERT_TRUE(dom.fromString(test_xml));
        EXPECT_FALSE(dom.isNull());
        dom.reset();
        EXPECT_TRUE(dom.isNull());
    }

    { // not empty after setting elements
        DOM dom;
        dom.getRoot().setName("Root");
        EXPECT_FALSE(dom.isNull());
        dom.reset();
        EXPECT_TRUE(dom.isNull());
    }
}

TEST(xml_test, TestEmptyDOMElement)
{
    using a_util::xml::DOM;
    using a_util::xml::DOMElement;

    EXPECT_TRUE(DOMElement().isNull());

    { // not empty if queried from dom
        DOM dom;
        ASSERT_TRUE(dom.load(TEST_FILES_DIR "/test.xml"));
        DOMElement element = dom.getRoot();
        EXPECT_FALSE(element.isNull());

        // but empty if reassigned
        element = DOMElement();
        EXPECT_TRUE(element.isNull());
    }

    { // empty after initializing with name (not related to any DOM)
        DOMElement element;
        EXPECT_FALSE(element.setName("testName"));
        EXPECT_TRUE(element.isNull());
    }

    { // empty after initializing with data (not related to any DOM)
        DOMElement element;
        EXPECT_FALSE(element.setData("testData"));
        EXPECT_TRUE(element.isNull());
    }

    { // empty after initializing with attribute (not related to any DOM)
        DOMElement element;
        EXPECT_FALSE(element.setAttribute("testAttribute", "testValue"));
        EXPECT_TRUE(element.isNull());
    }
}
