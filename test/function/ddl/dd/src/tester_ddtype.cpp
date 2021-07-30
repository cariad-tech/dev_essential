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
#include "ddl/codec/pkg_codec.h"
#include "ddl/dd/ddcompare.h"
#include "ddl/dd/ddstructure.h"

#include <gtest/gtest.h>
#include <stdint.h>

struct sub_struct {
    uint16_t value1;
    uint32_t value2;
    uint8_t value3;
};

struct array_struct {
    uint8_t values[10];
};

enum test_enum { a, b, c };

struct test {
    uint32_t value1;
    sub_struct sub;
    array_struct array[5];
    test_enum enumeration;
    bool bool_value;
};

#pragma pack(push, 1)
struct sub_struct_pack1 {
    uint16_t value1;
    uint32_t value2;
    uint8_t value3;
};

struct array_struct_pack1 {
    uint8_t values[10];
};

enum test_enum_pack1 { a_pack1, b_pack1, c_pack1 };

struct test_pack1 {
    uint32_t value1;
    sub_struct_pack1 sub;
    array_struct_pack1 array[5];
    test_enum_pack1 enumeration;
    bool bool_value;
};
#pragma pack(pop)

#pragma pack(push, 2)
struct sub_struct_pack2 {
    uint16_t value1;
    uint32_t value2;
    uint8_t value3;
};
#pragma pack(pop)

#pragma pack(push, 8)
struct sub_struct_pack8 {
    uint16_t value1;
    uint32_t value2;
    uint8_t value3;
};
#pragma pack(pop)

struct test_mixed {
    uint32_t value1;
    int8_t padding_byte;
    sub_struct_pack2 sub2;
    sub_struct_pack8 sub8;
    array_struct_pack1 array[5];
    test_enum_pack1 enumeration;
    bool bool_value;
};

/**
 * @detail The building up of a DataDefinition object representation.
 */
TEST(TesterDDLTypeReflection, createTypeManual)
{
    using namespace ddl;
    auto sub_definition = DDStructure("sub_struct")
                              .addElement<uint16_t>("value1")
                              .addElement<uint32_t>("value2")
                              .addElement<uint8_t>("value3");
    std::cout << "sub_struct size "
              << sub_definition.getDD()
                     .getStructTypeAccess(sub_definition.getStructName())
                     .getStaticStructSize()
              << std::endl;

    auto array_definition = DDStructure("array_struct").addElement<uint8_t>("values", 10);

#if defined(__GNUC__) && ((__GNUC__ == 5) && (__GNUC_MINOR__ == 2))
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
#endif // defined(__GNUC__) && ((__GNUC__ == 5) && (__GNUC_MINOR__ == 2))

    auto enum_definition = DDEnumGenerator<uint32_t>("test_enum")
                               .addElement("a", 0)
                               .addElement("b", 1)
                               .addElement("c", 2);

    auto other_enum_definition =
        DDEnumGenerator<uint32_t>("test_enum_other", {{"a", 0}, {"b", 1}, {"c", 2}});

#if defined(__GNUC__) && ((__GNUC__ == 5) && (__GNUC_MINOR__ == 2))
#pragma GCC diagnostic pop
#endif // defined(__GNUC__) && ((__GNUC__ == 5) && (__GNUC_MINOR__ == 2))

    auto test_definition = DDStructure("test")
                               .addElement<uint32_t>("value1")
                               .addElement("sub", sub_definition)
                               .addElement("array", array_definition, 5)
                               .addElement("enumeration", enum_definition)
                               .addElement<bool>("bool_value");

    CodecFactory decoder_check(test_definition);
    EXPECT_TRUE(isOk(decoder_check.isValid())) << "The created DDStructure is not valid";
    EXPECT_EQ(decoder_check.getStaticBufferSize(), sizeof(test))
        << "The created DDStructure does not have the expected size";
    EXPECT_EQ(test_definition.getDD()
                  .getStructTypeAccess(test_definition.getStructName())
                  .getStaticStructSize(),
              sizeof(test))
        << "The created DDStructure does not have the expected size";
    auto struct_access =
        test_definition.getDD().getStructTypeAccess(test_definition.getStructName());
    EXPECT_TRUE(struct_access) << "The created StructAccess is not valid";
    EXPECT_EQ(struct_access.getStaticStructSize(), sizeof(test))
        << "The created DDStructure does not have the expected size";

    EXPECT_EQ(*struct_access.getStructType().getAlignment(), alignof(test));

    // std::cout << test_definition.getStructDescription();
}

TEST(TesterDDLTypeReflection, createTypeFromStructDefaultAlignment)
{
    using namespace ddl;
    auto sub_definition = DDStructureGenerator<sub_struct>("sub_struct")
                              .addElement("value1", &sub_struct::value1)
                              .addElement("value2", &sub_struct::value2)
                              .addElement("value3", &sub_struct::value3);

    auto array_definition = DDStructureGenerator<array_struct>("array_struct")
                                .addElement("values", &array_struct::values);

    auto enum_definition = DDEnumGenerator<test_enum>("test_enum")
                               .addElement("a", a)
                               .addElement("b", b)
                               .addElement("c", c);

    {
        // this is to check the second CTOR possibility
        auto enum_definition_second =
            DDEnumGenerator<test_enum>("test_enum", {{"a", a}, {"b", b}, {"c", c}});
        EXPECT_TRUE(
            isOk(DDCompare::isEqual(enum_definition.getDD(), enum_definition_second.getDD())));
    }

    auto test_definition = DDStructureGenerator<test>("test")
                               .addElement("value1", &test::value1)
                               .addElement("sub", &test::sub, sub_definition)
                               .addElement("array", &test::array, array_definition)
                               .addElement("enumeration", &test::enumeration, enum_definition)
                               .addElement("bool_value", &test::bool_value);

    CodecFactory decoder_check(test_definition);
    EXPECT_TRUE(isOk(decoder_check.isValid())) << "The created DDStructure is not valid";
    EXPECT_EQ(decoder_check.getStaticBufferSize(), sizeof(test))
        << "The created DDStructure does not have the expected size";
    auto struct_access =
        test_definition.getDD().getStructTypeAccess(test_definition.getStructName());
    EXPECT_TRUE(struct_access) << "The created StructAccess is not valid";
    EXPECT_EQ(struct_access.getStaticStructSize(), sizeof(test))
        << "The created DDStructure does not have the expected size";

    EXPECT_EQ(*struct_access.getStructType().getAlignment(), alignof(test));

    std::cout << test_definition.getStructDescription();
}

TEST(TesterDDLTypeReflection, createTypeFromStructPack1Alignment)
{
    using namespace ddl;
    auto sub_definition = DDStructureGenerator<sub_struct_pack1>("sub_struct")
                              .addElement("value1", &sub_struct_pack1::value1)
                              .addElement("value2", &sub_struct_pack1::value2)
                              .addElement("value3", &sub_struct_pack1::value3);

    auto array_definition = DDStructureGenerator<array_struct_pack1>("array_struct")
                                .addElement("values", &array_struct_pack1::values);

    auto enum_definition = DDEnumGenerator<test_enum_pack1>("test_enum")
                               .addElement("a", a)
                               .addElement("b", b)
                               .addElement("c", c);

    {
        // this is to check the second CTOR possibility
        auto enum_definition_second =
            DDEnumGenerator<test_enum>("test_enum", {{"a", a}, {"b", b}, {"c", c}});
        EXPECT_TRUE(
            isOk(DDCompare::isEqual(enum_definition.getDD(), enum_definition_second.getDD())));
    }

    auto test_definition = DDStructureGenerator<test_pack1>("test")
                               .addElement("value1", &test_pack1::value1)
                               .addElement("sub", &test_pack1::sub, sub_definition)
                               .addElement("array", &test_pack1::array, array_definition)
                               .addElement("enumeration", &test_pack1::enumeration, enum_definition)
                               .addElement("bool_value", &test_pack1::bool_value);

    CodecFactory decoder_check(test_definition);
    EXPECT_TRUE(isOk(decoder_check.isValid())) << "The created DDStructure is not valid";
    EXPECT_EQ(decoder_check.getStaticBufferSize(), sizeof(test_pack1))
        << "The created DDStructure does not have the expected size";
    auto struct_access =
        test_definition.getDD().getStructTypeAccess(test_definition.getStructName());
    EXPECT_TRUE(struct_access) << "The created StructAccess is not valid";
    EXPECT_EQ(struct_access.getStaticStructSize(), sizeof(test_pack1))
        << "The created DDStructure does not have the expected size";

    EXPECT_EQ(*struct_access.getStructType().getAlignment(), alignof(test_pack1));
    for (auto elem: struct_access) {
        EXPECT_EQ(elem.getElement().getAlignment(), 1);
    }

    std::cout << test_definition.getStructDescription();
}

TEST(TesterDDLTypeReflection, createTypeFromStructMixedAlignment)
{
    using namespace ddl;
    auto sub_definition_pack2 = DDStructureGenerator<sub_struct_pack2>("sub_struct_pack2")
                                    .addElement("value1", &sub_struct_pack2::value1)
                                    .addElement("value2", &sub_struct_pack2::value2)
                                    .addElement("value3", &sub_struct_pack2::value3);

    auto sub_definition_pack8 = DDStructureGenerator<sub_struct_pack8>("sub_struct_pack8")
                                    .addElement("value1", &sub_struct_pack8::value1)
                                    .addElement("value2", &sub_struct_pack8::value2)
                                    .addElement("value3", &sub_struct_pack8::value3);

    auto array_definition = DDStructureGenerator<array_struct_pack1>("array_struct")
                                .addElement("values", &array_struct_pack1::values);

    auto enum_definition = DDEnumGenerator<test_enum_pack1>("test_enum")
                               .addElement("a", a_pack1)
                               .addElement("b", b_pack1)
                               .addElement("c", c_pack1);

    {
        // this is to check the second CTOR possibility
        auto enum_definition_second =
            DDEnumGenerator<test_enum>("test_enum", {{"a", a}, {"b", b}, {"c", c}});
        EXPECT_TRUE(
            isOk(DDCompare::isEqual(enum_definition.getDD(), enum_definition_second.getDD())));
    }

    auto test_definition = DDStructureGenerator<test_mixed>("test")
                               .addElement("value1", &test_mixed::value1)
                               .addElement("padding", &test_mixed::padding_byte)
                               .addElement("sub2", &test_mixed::sub2, sub_definition_pack2)
                               .addElement("sub8", &test_mixed::sub8, sub_definition_pack8)
                               .addElement("array", &test_mixed::array, array_definition)
                               .addElement("enumeration", &test_mixed::enumeration, enum_definition)
                               .addElement("bool_value", &test_mixed::bool_value);

    CodecFactory decoder_check(test_definition);
    EXPECT_TRUE(isOk(decoder_check.isValid())) << "The created DDStructure is not valid";
    EXPECT_EQ(decoder_check.getStaticBufferSize(), sizeof(test_mixed))
        << "The created DDStructure does not have the expected size";
    auto struct_access =
        test_definition.getDD().getStructTypeAccess(test_definition.getStructName());
    EXPECT_TRUE(struct_access) << "The created StructAccess is not valid";
    EXPECT_EQ(struct_access.getStaticStructSize(), sizeof(test_mixed))
        << "The created DDStructure does not have the expected size";

    EXPECT_EQ(*struct_access.getStructType().getAlignment(), alignof(test_mixed));

    // expected alignment is always the minimum of struct alignment and type alignment
    size_t expected_alignment = alignof(test_mixed);
    if (expected_alignment > 2) {
        expected_alignment = 2;
    }
    EXPECT_EQ(struct_access.getStructType().getElements().get("sub2")->getAlignment(),
              expected_alignment);
    // expected alignment is always the minimum of struct alignment and type alignment
    expected_alignment = alignof(test_mixed);
    if (expected_alignment > 8) {
        expected_alignment = 8;
    }
    EXPECT_EQ(struct_access.getStructType().getElements().get("sub8")->getAlignment(),
              expected_alignment);

    std::cout << test_definition.getStructDescription();
}

#pragma pack(push, 1)
template <typename T>
struct struct_with_typevalue {
    T value;
};
#pragma pack(pop)

template <typename T>
void test_is_compat_default_and_plain()
{
    ddl::DDStructureGenerator<struct_with_typevalue<T>> test_type_with_reflection("tTestType");
    test_type_with_reflection.addElement("value", &struct_with_typevalue<T>::value);

    ddl::DDStructure test_type_plain("tTestType");
    test_type_plain.addElement("value",
                               ddl::DataType<T>(),
                               1,
                               1); // add with array size 1 and alignment 1 which is expected

    EXPECT_EQ(test_type_with_reflection.getStructType(), test_type_plain.getStructType())
        << test_type_with_reflection.getStructDescription() + "\n is not equal \n" +
               test_type_plain.getStructDescription();
}

TEST(TesterDDLTypeReflection, createStructWithSpecialAlignment)
{
    test_is_compat_default_and_plain<uint32_t>();
    test_is_compat_default_and_plain<int32_t>();
    test_is_compat_default_and_plain<float>();
    test_is_compat_default_and_plain<double>();
    test_is_compat_default_and_plain<uint64_t>();
    test_is_compat_default_and_plain<bool>();
}

struct child1 {
    uint32_t a;
    uint32_t b;
    uint32_t c;
};

struct child2 {
    uint32_t a;
};

struct parent {
    child1 c1;
    child2 c2;
};

TEST(TesterDDLTypeReflection, detectMissingElements)
{
    using namespace ddl;
    auto child1_definition = DDStructureGenerator<child1>("child1")
                                 .addElement("a", &child1::a)
                                 .addElement("b", &child1::b);
    // we deliberately omit the last member
    //        .Add("c", &child1::c);

    auto child2_definition = DDStructureGenerator<child2>("child2").addElement("a", &child2::a);

    auto parent_definition = DDStructureGenerator<parent>("parent")
                                 .addElement("child1", &parent::c1, child1_definition)
                                 .addElement("child2", &parent::c2, child2_definition);

    ddl::CodecFactory codec(parent_definition);
    EXPECT_NE(codec.getStaticBufferSize(), sizeof(parent));

    // this must throw
    ASSERT_ANY_THROW(parent_definition.getStructDescription());
}
