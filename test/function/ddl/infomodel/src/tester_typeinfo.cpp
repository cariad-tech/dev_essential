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

#include "../../_common/test_oo_ddl.h"

#include <ddl/dd/dd.h>
#include <ddl/dd/dddatatype.h>
#include <ddl/dd/dddefault.h>
#include <ddl/dd/ddstring.h>
#include <ddl/dd/ddstructure.h>
#include <ddl/utilities/std_to_string.h>

#define DEV_ESSENTIAL_DISABLE_DEPRECATED_WARNINGS
#include <ddl/dd/dd_typeinfomodel.h>

#include <gtest/gtest.h>

#include <chrono>
#include <iostream>

void dump_deserialized_positions(const ddl::dd::StructTypeAccess& struct_type)
{
    std::cout << "Positions for struct: " << struct_type.getStructType().getName() << std::endl;
    for (const auto& elem_access: struct_type) {
        const auto& elem_ref = elem_access.getElement();
        std::string name_to_dump = "    " + elem_ref.getName();
        if (!elem_access.isDynamic() && !elem_access.isAfterDynamic()) {
            name_to_dump += " \t (static      )";
        }
        else {
            if (elem_access.isDynamic()) {
                name_to_dump += " \t (dynamic      )";
            }
            if (elem_access.isAfterDynamic()) {
                name_to_dump += " \t (after dynamic)";
            }
        }
        std::cout << name_to_dump << ": aligned(" << elem_ref.getAlignment() << ")"
                  << " bytepos " << elem_access.getDeserializedBytePos() << " bytesize "
                  << elem_access.getDeserializedByteSize() << std::endl;
    }
}

namespace static_struct {
struct tChildStruct {
    int8_t nValueDummy;
    int32_t nValue[3];
    int8_t nAfter;
};

struct tTest {
    tChildStruct sChild[2];
};

static constexpr const char* const strTestDesc =
    R"(<?xml version="1.0" encoding="iso-8859-1" standalone="no"?>
    <structs>
        <struct alignment="4" name="child_struct" version="2">
            <element alignment="1" arraysize="1" byteorder="BE" bytepos="0" name="value_dummy" type="tInt8"/>
            <element alignment="4" arraysize="3" byteorder="BE" bytepos="1" name="value" type="tInt32"/>
            <element alignment="1" arraysize="1" byteorder="BE" bytepos="13" name="after" type="tInt8"/>
        </struct>"
        <struct alignment="4" name="test" version="2">
            <element alignment="4" arraysize="2" byteorder="LE" bytepos="0" name="child" type="child_struct"/>
        </struct>"
    </structs>)";

const tTest sTestData = {{
    {0x01, {0x02, 0x03, 0x04}, 0x05},
    {0x06, {0x07, 0x08, 0x09}, 0x0A},
}};

} // namespace static_struct

/**
 * @detail The building up of a DataDefinition object representation.
 * It is very important to keep the model valid while renaming a data type, unit,
 * struct type, enum type which where used in different
 */
TEST(TesterOODDL, checkTypeInfoSizesFromString)
{
    using namespace ddl;

    auto my_dd = DDString::fromXMLString(static_struct::strTestDesc);
    my_dd.calculatePositions();

    auto struct_access_child_struct = my_dd.getStructTypeAccess("child_struct");
    auto struct_access_test = my_dd.getStructTypeAccess("test");

    ASSERT_EQ(struct_access_child_struct.getStaticStructSize(),
              sizeof(static_struct::tChildStruct));
    EXPECT_EQ(struct_access_test.getStaticStructSize(), sizeof(static_struct::tTest));

    dump_deserialized_positions(struct_access_test);
}

namespace all_types {
#ifdef WIN32
#define ALIGNED_ELEM(__align, __def) __declspec(align(__align)) __def;
#else
#define ALIGNED_ELEM(__align, __def) __def __attribute__((aligned(__align)));
#endif

struct tMain {
    ALIGNED_ELEM(1, bool bBool)
    ALIGNED_ELEM(1, int8_t nInt8)
    ALIGNED_ELEM(1, uint8_t nUInt8)
    ALIGNED_ELEM(2, int16_t nInt16)
    ALIGNED_ELEM(2, uint16_t nUInt16)
    ALIGNED_ELEM(4, int32_t nInt32)
    ALIGNED_ELEM(4, uint32_t nUInt32)
    ALIGNED_ELEM(8, int64_t nInt64)
    ALIGNED_ELEM(8, uint64_t nUInt64)
    ALIGNED_ELEM(4, float fFloat32)
    ALIGNED_ELEM(8, double fFloat64)
    ALIGNED_ELEM(1, char nChar)
};

static constexpr const char* const strTestDesc =
    R"(<?xml version="1.0" encoding="iso-8859-1" standalone="no"?>
       <struct alignment="8" name="main" version="2">
            <element alignment="1" arraysize="1" byteorder="BE" bitpos="1" bytepos="0" name="bBool" type="tBool"/>
            <element alignment="1" arraysize="1" byteorder="BE" bitpos="1" bytepos="1" name="nInt8" type="tInt8"/>
            <element alignment="1" arraysize="1" byteorder="BE" bitpos="1" bytepos="2" name="nUInt8" type="tUInt8"/>
            <element alignment="2" arraysize="1" byteorder="BE" bitpos="1" bytepos="3" name="nInt16" type="tInt16"/>
            <element alignment="2" arraysize="1" byteorder="BE" bitpos="1" bytepos="5" name="nUInt16" type="tUInt16"/>
            <element alignment="4" arraysize="1" byteorder="BE" bitpos="1" bytepos="7" name="nInt32" type="tInt32"/>
            <element alignment="4" arraysize="1" byteorder="BE" bitpos="1" bytepos="11" name="nUInt32" type="tUInt32"/>
            <element alignment="8" arraysize="1" byteorder="BE" bitpos="1" bytepos="15" name="nInt64" type="tInt64"/>
            <element alignment="8" arraysize="1" byteorder="BE" bitpos="1" bytepos="23" name="nUInt64" type="tUInt64"/>
            <element alignment="4" arraysize="1" byteorder="BE" bitpos="1" bytepos="31" name="fFloat32" type="tFloat32"/>
            <element alignment="8" arraysize="1" byteorder="BE" bitpos="1" bytepos="35" name="fFloat64" type="tFloat64"/>
            <element alignment="1" arraysize="1" byteorder="BE" bitpos="1" bytepos="43" name="nChar" type="tChar"/>
    </struct>)";

const tMain sTestData = {true, 1, 2, 3, 4, 5, 6, 7, 8, (float)3.1415, 2.7182, 'x'};

} // namespace all_types

/**
 * @detail We build a struct_type within the ddl by using DDString.
 * After creating, the type info positions are calculated automatically and have expected size
 * values set.
 */
TEST(TesterOODDL, checkTypeInfoSizesAllTypes)
{
    using namespace ddl;

    auto my_dd = DDString::fromXMLString("main", all_types::strTestDesc);
    auto struct_access_main = my_dd.getStructTypeAccess("main");
    ASSERT_TRUE(struct_access_main) << "struct access 'main' not found";
    EXPECT_EQ(struct_access_main.getStaticStructSize(), sizeof(all_types::tMain));

    dump_deserialized_positions(struct_access_main);
}

/**
 * @detail We build a struct_type within the ddl by using DDString.
 * After creating, the serialized positions are calculated automatically and have bytepos values
 * set.
 */
TEST(TesterOODDL, checkAutoSerializedPosCalculation)
{
    using namespace ddl;
    dd::StructType struct_type("my_struct", "1", 8);
    struct_type.getElements().add(
        {"elem1", "tUInt32", {}, dd::StructType::SerializedInfo({}, dd::ByteOrder::e_le)}); // pos 0
    struct_type.getElements().add(
        {"elem2",
         "tFloat64",
         {},
         dd::StructType::SerializedInfo({}, dd::ByteOrder::e_le)}); // pos 4
    struct_type.getElements().add(
        {"elem3", "tUInt8", {}, dd::StructType::SerializedInfo({}, dd::ByteOrder::e_le)}); // pos 12
    struct_type.getElements().add(
        {"elem4",
         "tFloat64",
         {},
         dd::StructType::SerializedInfo({}, dd::ByteOrder::e_le)}); // pos 13
    struct_type.getElements().add(
        {"elem5", "tUInt8", {}, dd::StructType::SerializedInfo({}, dd::ByteOrder::e_le)}); // pos 21

    dd::DataDefinition dd;
    // calculation is done only within the DataDefinition!!
    // while adding the calculation is done automatically if the Byte pos is not set!
    dd.getStructTypes().add(struct_type);

    // the struct_type is copied ! So the calculated positions are only WITHIN the DD
    auto added_struct_type = dd.getStructTypes().get("my_struct");

    auto elem_to_check = added_struct_type->getElements().get("elem1");
    EXPECT_EQ(*elem_to_check->getBytePos(), 0);
    elem_to_check = added_struct_type->getElements().get("elem2");
    EXPECT_EQ(*elem_to_check->getBytePos(), 4);
    elem_to_check = added_struct_type->getElements().get("elem3");
    EXPECT_EQ(*elem_to_check->getBytePos(), 12);
    elem_to_check = added_struct_type->getElements().get("elem4");
    EXPECT_EQ(*elem_to_check->getBytePos(), 13);
    elem_to_check = added_struct_type->getElements().get("elem5");
    EXPECT_EQ(*elem_to_check->getBytePos(), 21);
}

static constexpr const char* const strTestDescDyn =
    R"(<?xml version="1.0" encoding="iso-8859-1" standalone="no"?>
    <struct alignment="8" name="main" version="2">
        <element alignment="1" arraysize="1" byteorder="BE" bitpos="1" bytepos="0" name="bBool" type="tBool"/>
        <element alignment="1" arraysize="1" byteorder="BE" bitpos="1" bytepos="1" name="nInt8" type="tInt8"/>
        <element alignment="1" arraysize="1" byteorder="BE" bitpos="1" bytepos="2" name="nUInt8" type="tUInt8"/>
        <element alignment="2" arraysize="1" byteorder="BE" bitpos="1" bytepos="3" name="nInt16" type="tInt16"/>
        <element alignment="2" arraysize="1" byteorder="BE" bitpos="1" bytepos="5" name="nUInt16" type="tUInt16"/>
        <element alignment="4" arraysize="1" byteorder="BE" bitpos="1" bytepos="7" name="nInt32" type="tInt32"/>
        <element alignment="4" arraysize="1" byteorder="BE" bitpos="1" bytepos="11" name="nUInt32" type="tUInt32"/>
        <element alignment="8" arraysize="1" byteorder="BE" bitpos="1" bytepos="15" name="nInt64" type="tInt64"/>
        <element alignment="8" arraysize="1" byteorder="BE" bitpos="1" bytepos="23" name="nUInt64" type="tUInt64"/>
        <element alignment="4" arraysize="1" byteorder="BE" bitpos="1" bytepos="31" name="fFloat32" type="tFloat32"/>
        <element alignment="8" arraysize="1" byteorder="BE" bitpos="1" bytepos="35" name="fFloat64" type="tFloat64"/>
        <element alignment="1" arraysize="nInt64" byteorder="BE" bitpos="1" bytepos="43" name="nCharArray" type="tChar"/>
   </struct>)";

/**
 * @detail We build a struct_type within the ddl by using DDString.
 * After creating, struct with dynamic content are marked as that.
 */
TEST(TesterOODDL, checkDynamicDetection)
{
    using namespace ddl;
    auto dyn_dd = DDString::fromXMLString(strTestDescDyn);
    bool detect_dynamic = false;
    for (auto struct_type: dyn_dd.getStructTypes()) {
        auto type_info = struct_type.second->getInfo<dd::TypeInfo>();
        if (type_info) {
            if (type_info->isDynamic()) {
                detect_dynamic = true;
                break;
            }
        }
    }
    ASSERT_TRUE(detect_dynamic);

    // check for false
    auto not_dyn_dd = DDString::fromXMLString(all_types::strTestDesc);
    detect_dynamic = false;
    for (auto struct_type: not_dyn_dd.getStructTypes()) {
        auto type_info = struct_type.second->getInfo<dd::TypeInfo>();
        if (type_info) {
            if (type_info->isDynamic()) {
                detect_dynamic = true;
                break;
            }
        }
    }
    ASSERT_FALSE(detect_dynamic);
}

/**
 * @detail We build a data definition. The positions are calculated automatically.
 * if changing a alignment, datatype the TypeInfo must be updated also for the dependent types.
 */
TEST(TesterOODDL, checkTypeCalculations)
{
    using namespace test_ddl;
    // prepare
    ddl::dd::DataDefinition my_dd;
    my_dd.getDataTypes().add(uint8_data_type());
    my_dd.getDataTypes().add(uint16_data_type());

    my_dd.getEnumTypes().add({"e1", "tUInt8"});

    ddl::DDStructure s1("s1", 1, 1);
    s1.addElement("elem1_in_s1", uint8_data_type());

    ddl::DDStructure s2("s2", 1, 1);
    s2.addElement("elem1_in_s2", s1);

    my_dd.add(s2.getDD());

    // check precondition
    auto uint8_type = my_dd.getDataTypes().access("tUInt8");
    auto uint16_type = my_dd.getDataTypes().access("tUInt16");
    auto e1_type = my_dd.getEnumTypes().access("e1");
    auto s1_type = my_dd.getStructTypes().access("s1");
    auto s2_type = my_dd.getStructTypes().access("s2");

    ASSERT_TRUE(uint8_type);
    ASSERT_TRUE(uint16_type);
    ASSERT_TRUE(e1_type);
    ASSERT_TRUE(s1_type);
    ASSERT_TRUE(s2_type);

    auto uint8_type_info = uint8_type->getInfo<ddl::dd::TypeInfo>();
    auto uint16_type_info = uint16_type->getInfo<ddl::dd::TypeInfo>();
    auto e1_type_info = e1_type->getInfo<ddl::dd::TypeInfo>();
    auto s1_type_info = s1_type->getInfo<ddl::dd::TypeInfo>();
    auto s2_type_info = s2_type->getInfo<ddl::dd::TypeInfo>();

    ASSERT_TRUE(uint8_type_info);
    ASSERT_TRUE(uint16_type_info);
    ASSERT_TRUE(e1_type_info);
    ASSERT_TRUE(s1_type_info);
    ASSERT_TRUE(s2_type_info);

    ASSERT_EQ(uint8_type_info->getTypeBitSize(), 8);
    ASSERT_EQ(uint16_type_info->getTypeBitSize(), 16);
    ASSERT_EQ(e1_type_info->getTypeBitSize(), 8);
    ASSERT_EQ(s1_type_info->getTypeBitSize(), 8);
    ASSERT_EQ(s2_type_info->getTypeBitSize(), 8);

    // change the datatype and check the typeinfo
    uint8_type->setBitSize(32);

    // check the sizes
    ASSERT_EQ(uint8_type_info->getTypeBitSize(), 32);
    ASSERT_EQ(uint16_type_info->getTypeBitSize(), 16);
    ASSERT_EQ(e1_type_info->getTypeBitSize(), 32);
    ASSERT_EQ(s1_type_info->getTypeBitSize(), 32);
    ASSERT_EQ(s2_type_info->getTypeBitSize(), 32);

    // change the type of the s1 element
    s1_type->getElements().access("elem1_in_s1")->setTypeName("tUInt16");

    ASSERT_EQ(uint8_type_info->getTypeBitSize(), 32);
    ASSERT_EQ(uint16_type_info->getTypeBitSize(), 16);
    ASSERT_EQ(e1_type_info->getTypeBitSize(), 32);
    ASSERT_EQ(s1_type_info->getTypeBitSize(), 16);
    ASSERT_EQ(s2_type_info->getTypeBitSize(), 16);

    ASSERT_EQ(s2_type_info->getTypeAlignedByteSize(), 2);
    ASSERT_EQ(s2_type_info->getTypeUnalignedByteSize(), 2);

    // change alignment
    s1_type->setAlignment(64);

    ASSERT_EQ(s2_type_info->getTypeAlignedByteSize(), 64);
    ASSERT_EQ(s2_type_info->getTypeUnalignedByteSize(), 64);

    // force a recursion
    // this will be invalid and so not result in a endless loop
    ASSERT_TRUE(my_dd.isValid());
    s1_type->getElements().access("elem1_in_s1")->setTypeName("s2");

    ASSERT_FALSE(my_dd.isValid());
    auto validation_protcol = my_dd.getValidationProtocol();
    bool contains_recursion_keyword = false;
    for (const auto& problem: validation_protcol) {
        if (problem.problem_message.find("recursion") != std::string::npos) {
            contains_recursion_keyword = true;
        }
    }
    ASSERT_TRUE(contains_recursion_keyword)
        << "The keyword 'recursion' was not found in problems, expecting it";

    // repair recursion
    s1_type->getElements().access("elem1_in_s1")->setTypeName("tUInt16");

    // its valid again
    ASSERT_TRUE(my_dd.isValid());
}

/**
 * @detail When adding a element to an existing StructType at the end, the calculation will only
 *         calculate the last position.
 */
TEST(TesterOODDL, checkTypeCalculationPerformance)
{
    using namespace ddl;
    DataDefinition my_ddl = DDDefault::create();

    auto& struct_types = my_ddl.getStructTypes();
    struct_types.add(dd::StructType("test_type"));
    auto test_type = struct_types.access("test_type");
    auto& test_elements = test_type->getElements();
    std::vector<std::chrono::microseconds> measured_durations;
    size_t test_count = 50000;
    for (size_t elem_count = 0; elem_count < test_count; ++elem_count) {
        auto begin_of_measurement = std::chrono::high_resolution_clock::now();
        test_elements.add(
            dd::StructType::Element("elem_" + std::to_string(elem_count), "tInt32", {2}, {}));
        auto duration_of_measurement =
            std::chrono::high_resolution_clock::now() - begin_of_measurement;
        measured_durations.push_back(
            std::chrono::duration_cast<std::chrono::microseconds>(duration_of_measurement));
    }

    // detect complexity
    // should be O(1) not above !
    // its only a guess: the max duration should not be greater than 3 times of min duration
    dd::utility::Optional<std::chrono::microseconds> min_duration;
    dd::utility::Optional<int64_t> average_duration;
    for (auto current_duration: measured_durations) {
        if (min_duration) {
            if (current_duration < *min_duration) {
                min_duration = current_duration;
            }
        }
        else {
            min_duration = current_duration;
        }
        if (average_duration) {
            *average_duration = *average_duration + current_duration.count();
        }
        else {
            average_duration = current_duration.count();
        }
    }
    *average_duration = *average_duration / test_count;
    // this is more a guess!
    // should be 2, but the times are to small, that there might be many deviations
    constexpr double factor = 3;
    std::cout << "average: " << *average_duration << " - min : " << (*min_duration).count()
              << " - min * " << factor << " : " << ((*min_duration).count() * factor) << std::endl;
    // the times are so
    ASSERT_LT(*average_duration, static_cast<int64_t>((*min_duration).count() * factor));
}

/**
 * @detail When adding a element to an existing DDStructure at the end, the calculation will only
 *         calculate the last position again, and wont update all.
 */
TEST(TesterOODDL, checkTypeCalculationPerformanceWithStructure)
{
    using namespace ddl;
    ddl::DDStructure my_structure("test_type");
    std::vector<std::chrono::microseconds> measured_durations;
    size_t test_count = 10000;
    for (size_t elem_count = 0; elem_count < test_count; ++elem_count) {
        auto begin_of_measurement = std::chrono::high_resolution_clock::now();
        my_structure.addElement(
            DDElement("elem_" + std::to_string(elem_count), DataType<int32_t>()));
        auto duration_of_measurement =
            std::chrono::high_resolution_clock::now() - begin_of_measurement;
        measured_durations.push_back(
            std::chrono::duration_cast<std::chrono::microseconds>(duration_of_measurement));
    }

    // detect complexity
    // should be O(1) not above !
    // its only a guess: the max duration should not be greater than 3 times of min duration
    dd::utility::Optional<std::chrono::microseconds> min_duration;
    dd::utility::Optional<int64_t> average_duration;
    for (auto current_duration: measured_durations) {
        if (min_duration) {
            if (current_duration < *min_duration) {
                min_duration = current_duration;
            }
        }
        else {
            min_duration = current_duration;
        }
        if (average_duration) {
            *average_duration = *average_duration + current_duration.count();
        }
        else {
            average_duration = current_duration.count();
        }
    }
    *average_duration = *average_duration / test_count;

    // should be 2, but the times are to small, that there might be many deviations
    // this is more a guess!
    constexpr double factor = 3;
    std::cout << "average: " << *average_duration << " - min : " << (*min_duration).count()
              << " - min * " << factor << " : " << ((*min_duration).count() * factor) << std::endl;
    ASSERT_LT(*average_duration, static_cast<int64_t>((*min_duration).count() * factor));
}

namespace reference_test {
static constexpr const char* const strTestDesc =
    R"(<?xml version="1.0" encoding="iso-8859-1" standalone="no"?>
<ddl>
    <enums>
        <enum name="test_enum" type="tInt16">
            <element name="CONSTANT_ONE" value="42"/>
            <element name="CONSTANT_TWO" value="10"/>
        </enum>
    </enums>
    <structs>
        <struct alignment="4" name="child_struct" version="2">
            <element alignment="1" arraysize="1" byteorder="BE" bytepos="0" name="value_dummy" type="tInt8"/>
            <element alignment="4" arraysize="3" byteorder="BE" bytepos="1" name="value" type="tInt32"/>
            <element alignment="1" arraysize="1" byteorder="BE" bytepos="13" name="after" type="tInt8"/>
        </struct>"
        <struct alignment="4" name="test" version="2">
            <element alignment="4" arraysize="2" byteorder="LE" bytepos="0" name="child" type="child_struct"/>
        </struct>"
    </structs>
</ddl>)";

template <typename T>
void renameTypes(T& types, const std::string& prefix, size_t& counter)
{
    std::vector<std::string> names;
    names.reserve(types.getSize());
    // we need to collect the names, because a renaming invalidates the iterator!
    for (auto& current: types) {
        names.push_back(current.second->getName());
    }
    for (const auto& name: names) {
        auto current_type = types.access(name);
        current_type->setName(prefix + std::to_string(counter));
        ++counter;
    }
}
} // namespace reference_test

/**
 * @detail When adding a StructType to the DDL
 *         the ElementTypeInfo will be reset to the right references
 */
TEST(TesterOODDL, checkAddingTypesWithCorrectReferences)
{
    ddl::dd::DataDefinition new_dd;
    {
        auto old_dd = ddl::DDString::fromXMLString(reference_test::strTestDesc);
        new_dd.add(*old_dd.getStructTypes().get("test"), old_dd);

        // now we rename all types in the "old dd"
        size_t counter = 0;
        reference_test::renameTypes(old_dd.getDataTypes(), "renamed_old_", counter);
        reference_test::renameTypes(old_dd.getEnumTypes(), "renamed_old_", counter);
        reference_test::renameTypes(old_dd.getStructTypes(), "renamed_old_", counter);
    }

    for (const auto& current_type: new_dd.getStructTypes()) {
        const auto& name_of_type = current_type.second->getName();
        ASSERT_EQ(name_of_type.find("renamed_old_"), std::string::npos);

        auto struct_access = new_dd.getStructTypeAccess(name_of_type);
        for (const auto& element_access: struct_access) {
            const auto data_type = element_access.getDataType();
            const auto enum_type = element_access.getEnumType();
            const auto struct_type = element_access.getStructType();
            ASSERT_TRUE(data_type || enum_type || struct_type);
            if (data_type) {
                EXPECT_EQ(data_type->getName().find("renamed_old_"), std::string::npos);
            }
            if (enum_type) {
                EXPECT_EQ(enum_type->getName().find("renamed_old_"), std::string::npos);
            }
            if (struct_type) {
                EXPECT_EQ(struct_type->getName().find("renamed_old_"), std::string::npos);
            }
        }
    }
}