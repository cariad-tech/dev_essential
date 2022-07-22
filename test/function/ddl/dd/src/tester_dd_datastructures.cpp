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

#include <ddl/dd/dd.h>

#include <gtest/gtest.h>

#include <stdint.h>

using namespace ddl::dd;

/*****************************************************************************************
 * Test list
 *****************************************************************************************/

struct TestValidatorList;
using TestElementList = utility::TypeAccessList<StructType::Element, TestValidatorList>;

struct TestValidatorList : utility::TypeAccessListObserver<StructType::Element> {
    void clearEvents()
    {
        _change_events.clear();
    }

    bool validateContains(const StructType::Element& elem)
    {
        if (_list_to_check) {
            return _list_to_check->contains(elem.getName());
        }
        return false;
    }

    bool notifyChangedListContent(utility::TypeAccessListEventCode code,
                                  StructType::Element&,
                                  const std::string& additional_info)
    {
        _change_events.push_back({code, additional_info});
        return true;
    }

    void modelChanged(utility::TypeAccessListEventCode code,
                      StructType::Element&,
                      const std::string& additional_info) override
    {
        _change_events.push_back({code, additional_info});
    }

    void setListToCheck(TestElementList* list_to_check)
    {
        _list_to_check = list_to_check;
    }
    const TestElementList::container_named_type* getNamedItemList() const
    {
        return &_container;
    }

    TestElementList::container_named_type* getNamedItemList()
    {
        return &_container;
    }

    std::vector<std::pair<utility::TypeAccessListEventCode, std::string>> _change_events;
    TestElementList* _list_to_check = {};
    TestElementList::container_named_type _container;
};

/**
 * @detail Create an element list add remoce get access elements, check if events are sent
 */
TEST(TesterUtilityDataStructureList, createAccessRemove)
{
    TestElementList elems_copy(nullptr, "test_list_copy_out_of_context");
    {
        TestValidatorList test_validator;
        TestElementList elems(&test_validator, "test_list");
        test_validator.setListToCheck(&elems);

        elems.add(StructType::Element("elem1", "some_type", {}, {}));
        elems.add(StructType::Element("elem3", "some_type", {}, {}));
        elems.insert(1, StructType::Element("elem2", "some_type", {}, {}));
        elems.emplace(StructType::Element("elem4", "some_type", {}, {}));
        elems.emplace(StructType::Element("elem5", "some_type", {}, {}));

        EXPECT_EQ(elems.getSize(), 5);
        ASSERT_EQ(test_validator._change_events.size(), 5);

        EXPECT_EQ(test_validator._change_events[0],
                  decltype(test_validator._change_events)::value_type(
                      {utility::TypeAccessListEventCode::list_item_added, "elem1"}));
        EXPECT_EQ(test_validator._change_events[1],
                  decltype(test_validator._change_events)::value_type(
                      {utility::TypeAccessListEventCode::list_item_added, "elem3"}));
        EXPECT_EQ(test_validator._change_events[2],
                  decltype(test_validator._change_events)::value_type(
                      {utility::TypeAccessListEventCode::list_item_inserted, "elem2"}));
        EXPECT_EQ(test_validator._change_events[3],
                  decltype(test_validator._change_events)::value_type(
                      {utility::TypeAccessListEventCode::list_item_added, "elem4"}));
        EXPECT_EQ(test_validator._change_events[4],
                  decltype(test_validator._change_events)::value_type(
                      {utility::TypeAccessListEventCode::list_item_added, "elem5"}));

        auto check_elem5 = elems.get("elem5");
        EXPECT_TRUE(check_elem5);

        auto check_elem5_access = elems.access("elem5");
        EXPECT_TRUE(check_elem5_access);
        check_elem5_access->setName("elem6");

        // check the events
        ASSERT_EQ(test_validator._change_events.size(), 7);
        EXPECT_EQ(test_validator._change_events[5],
                  decltype(test_validator._change_events)::value_type(
                      {utility::TypeAccessListEventCode::list_item_renamed, "elem5"}));
        EXPECT_EQ(test_validator._change_events[6],
                  decltype(test_validator._change_events)::value_type(
                      {utility::TypeAccessListEventCode::list_item_changed, "name"}));

        // remove an item
        elems.remove("elem6");
        auto check_elem5_access_after_remove = elems.access("elem6");
        EXPECT_FALSE(check_elem5_access_after_remove);

        // check the events
        EXPECT_EQ(test_validator._change_events.size(), 8);
        EXPECT_EQ(test_validator._change_events[7],
                  decltype(test_validator._change_events)::value_type(
                      {utility::TypeAccessListEventCode::list_item_removed, "elem6"}));

        // but the other is still accessable!
        EXPECT_TRUE(check_elem5_access);

        // but no event is given to the list
        check_elem5_access->setName("back_to_elem5");
        EXPECT_EQ(test_validator._change_events.size(), 8);

        elems_copy = elems;
    }

    // we copied everything
    EXPECT_EQ(elems_copy.getSize(), 4);
    auto check_elem1_access = elems_copy.access("elem1");
    EXPECT_TRUE(check_elem1_access);
    check_elem1_access->setName("new_name_elem1");

    // check contains
    EXPECT_TRUE(elems_copy.contains("new_name_elem1"));
    EXPECT_TRUE(elems_copy.contains("elem2"));
    EXPECT_TRUE(elems_copy.contains("elem3"));
    EXPECT_TRUE(elems_copy.contains("elem4"));

    // we insert in the mid
    elems_copy.insert(2, StructType::Element("elem_pos2", "some_type", {}, {}));
    EXPECT_EQ(*elems_copy.getPosOf("elem_pos2"), 2);
    EXPECT_EQ(*elems_copy.getPosOf("elem3"), 3);
    EXPECT_EQ(*elems_copy.getPosOf("elem4"), 4);
    EXPECT_EQ(elems_copy.getSize(), 5);

    // we insert at the end
    elems_copy.insert(5, StructType::Element("elem_pos5", "some_type", {}, {}));
    EXPECT_EQ(*elems_copy.getPosOf("elem_pos5"), 5);
    EXPECT_EQ(*elems_copy.getPosOf("elem3"), 3);
    EXPECT_EQ(*elems_copy.getPosOf("elem4"), 4);
    EXPECT_EQ(elems_copy.getSize(), 6);

    // we insert in front
    elems_copy.insert(0, StructType::Element("elem_pos0", "some_type", {}, {}));
    EXPECT_EQ(*elems_copy.getPosOf("elem_pos0"), 0);
    EXPECT_EQ(*elems_copy.getPosOf("elem3"), 4);
    EXPECT_EQ(*elems_copy.getPosOf("elem4"), 5);
    EXPECT_EQ(*elems_copy.getPosOf("elem_pos5"), 6);
    EXPECT_EQ(elems_copy.getSize(), 7);
}

/*****************************************************************************************
 * Test map
 *****************************************************************************************/

struct TestValidatorMap : utility::TypeAccessMapObserver<EnumType> {
    void clearEvents()
    {
        _change_events.clear();
    }
    void setReturnVal(bool return_val)
    {
        _current_return_val = return_val;
    }
    bool validateContains(const EnumType&)
    {
        return _current_return_val;
    }

    bool notifyChangedMapContent(utility::TypeAccessMapEventCode code,
                                 EnumType&,
                                 const std::string& additional_info)
    {
        _change_events.push_back({code, additional_info});
        return true;
    }

    void modelChanged(utility::TypeAccessMapEventCode code,
                      EnumType&,
                      const std::string& additional_info) override
    {
        _change_events.push_back({code, additional_info});
    }

    bool _current_return_val = false;
    std::vector<std::pair<utility::TypeAccessMapEventCode, std::string>> _change_events;
};

using TestElementMap = utility::TypeAccessMap<EnumType, TestValidatorMap>;

/**
 * @detail The building up of a DataDefinition object representation.
 */
TEST(TesterUtilityDataStructureMap, createAccessRemoveInMap)
{
    TestElementMap enums_copy(nullptr, "test_map_copy_out_of_context");
    {
        TestValidatorMap test_validator;
        TestElementMap enums(&test_validator, "test_map");

        enums.add(EnumType("enum1", "some_type", {{"test_val1", "1"}, {"test_val2", "2"}}));
        enums.add(EnumType("enum2", "some_type", {{"test_val1", "1"}, {"test_val2", "2"}}));
        enums.add(EnumType("enum3", "some_type", {{"test_val1", "1"}, {"test_val2", "2"}}));
        enums.emplace(EnumType("enum4", "some_type", {{"test_val1", "1"}, {"test_val2", "2"}}));
        enums.emplace(EnumType("enum5", "some_type", {{"test_val1", "1"}, {"test_val2", "2"}}));

        EXPECT_EQ(enums.getSize(), 5);
        ASSERT_EQ(test_validator._change_events.size(), 5);

        EXPECT_EQ(test_validator._change_events[0],
                  decltype(test_validator._change_events)::value_type(
                      {utility::TypeAccessMapEventCode::map_item_added, "enum1"}));
        EXPECT_EQ(test_validator._change_events[1],
                  decltype(test_validator._change_events)::value_type(
                      {utility::TypeAccessMapEventCode::map_item_added, "enum2"}));
        EXPECT_EQ(test_validator._change_events[2],
                  decltype(test_validator._change_events)::value_type(
                      {utility::TypeAccessMapEventCode::map_item_added, "enum3"}));
        EXPECT_EQ(test_validator._change_events[3],
                  decltype(test_validator._change_events)::value_type(
                      {utility::TypeAccessMapEventCode::map_item_added, "enum4"}));
        EXPECT_EQ(test_validator._change_events[4],
                  decltype(test_validator._change_events)::value_type(
                      {utility::TypeAccessMapEventCode::map_item_added, "enum5"}));

        auto check_enum5 = enums.get("enum5");
        EXPECT_TRUE(check_enum5);

        auto check_enum5_access = enums.access("enum5");
        EXPECT_TRUE(check_enum5_access);
        check_enum5_access->setName("new_name_enum5");

        // check the events
        ASSERT_EQ(test_validator._change_events.size(), 7);
        EXPECT_EQ(test_validator._change_events[5],
                  decltype(test_validator._change_events)::value_type(
                      {utility::TypeAccessMapEventCode::map_item_renamed, "enum5"}));
        EXPECT_EQ(test_validator._change_events[6],
                  decltype(test_validator._change_events)::value_type(
                      {utility::TypeAccessMapEventCode::map_item_changed, "name"}));

        // remove and item
        enums.remove("new_name_enum5");
        auto check_enum5_access_after_remove = enums.access("new_name_enum5");
        EXPECT_FALSE(check_enum5_access_after_remove);

        // check the events
        EXPECT_EQ(test_validator._change_events.size(), 8);
        EXPECT_EQ(test_validator._change_events[7],
                  decltype(test_validator._change_events)::value_type(
                      {utility::TypeAccessMapEventCode::map_item_removed, "new_name_enum5"}));

        // but the other is still accessable!
        EXPECT_TRUE(check_enum5_access);

        // but no event is given to the list
        check_enum5_access->setName("back_to_enum5");
        EXPECT_EQ(test_validator._change_events.size(), 8);

        enums_copy = enums;
    }

    // we copied everything
    EXPECT_EQ(enums_copy.getSize(), 4);
    auto check_enum1_access = enums_copy.access("enum1");
    EXPECT_TRUE(check_enum1_access);
    check_enum1_access->setName("new_name_enum1");

    {
        EnumType copy_of_enum = *check_enum1_access;
        copy_of_enum.setName("test_name");
    }

    // check contains
    EXPECT_TRUE(enums_copy.contains("new_name_enum1"));
    EXPECT_TRUE(enums_copy.contains("enum2"));
    EXPECT_TRUE(enums_copy.contains("enum3"));
    EXPECT_TRUE(enums_copy.contains("enum4"));
}
/**
 * @detail Check if the observer are moved and copied right
 */
TEST(TesterUtilityDataStructureMap, observerMove)
{
    using namespace ddl;
    dd::datamodel::DataDefinition data_model_copy;
    dd::datamodel::DataDefinition data_model_move;
    {
        dd::datamodel::DataDefinition data_model;
        data_model.getDataTypes().add({"type1", 1});
        data_model.getDataTypes().add({"type2", 2});
        data_model.getDataTypes().emplace({"type3", 2});

        data_model.getStructTypes().add({"structtype1", "1"});
        auto struct_type = data_model.getStructTypes().access("structtype1");
        struct_type->getElements().add({"elem1", "type1", {}, {}});
        struct_type->getElements().emplace({"elem2", "type2", {}, {}});

        data_model.getStructTypes().add({"structtype2", "2"});
        data_model.getStructTypes().add({"structtype3", "2"});

        data_model_copy = data_model;
        data_model_move = std::move(data_model);
    }

    auto type1_copy = data_model_copy.getDataTypes().access("type1");
    ASSERT_TRUE(type1_copy);
    type1_copy->setName("type1_changed");
    auto type1_changed_copy = data_model_copy.getDataTypes().access("type1_changed");
    ASSERT_TRUE(type1_changed_copy);

    auto struct_type_copy = data_model_copy.getStructTypes().access("structtype1");
    ASSERT_TRUE(struct_type_copy);
    auto struct_type_elem_copy = struct_type_copy->getElements().access("elem1");
    ASSERT_TRUE(struct_type_elem_copy);
    struct_type_elem_copy->setName("elem1_changed");
    auto struct_type_elem_changed_copy = struct_type_copy->getElements().access("elem1_changed");
    ASSERT_TRUE(struct_type_elem_changed_copy);

    auto type1_move = data_model_move.getDataTypes().access("type1");
    ASSERT_TRUE(type1_move);
    type1_move->setName("type1_changed");
    auto type1_changed_move = data_model_move.getDataTypes().access("type1_changed");
    ASSERT_TRUE(type1_changed_move);

    auto struct_type_move = data_model_move.getStructTypes().access("structtype1");
    ASSERT_TRUE(struct_type_move);
    auto struct_type_elem_move = struct_type_move->getElements().access("elem1");
    ASSERT_TRUE(struct_type_elem_move);
    struct_type_elem_move->setName("elem1_changed");
    auto struct_type_elem_changed_move = struct_type_move->getElements().access("elem1_changed");
    ASSERT_TRUE(struct_type_elem_changed_move);
}
