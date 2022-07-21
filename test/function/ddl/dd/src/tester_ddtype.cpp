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

#include <ddl/codec/pkg_codec.h>
#include <ddl/dd/ddcompare.h>
#include <ddl/dd/ddstring.h>
#include <ddl/dd/ddstructure.h>

#include <gtest/gtest.h>

#include <algorithm>
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

void checkElementDeserializedBytePosAndAlignmentSet(const ddl::DDStructure& structure,
                                                    const std::string& element_name,
                                                    size_t expected_byte_pos,
                                                    size_t expected_alignment)
{
    auto struct_access = structure.getDD().getStructTypeAccess(structure.getStructName());
    auto element = struct_access.getElementByPath(element_name);

    ASSERT_EQ(element.getDeserializedBytePos(), expected_byte_pos);
    ASSERT_EQ(element.getElement().getAlignment(), expected_alignment)
        << "Alignment of element mismatch - " << element_name;
}

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

    codec::CodecFactory decoder_check(test_definition);
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

_MAKE_RESULT(-19, ERR_NOT_SUPPORTED);
TEST(TesterDDLTypeReflection, createUnknownType)
{
    // user defined types are supported!
    using namespace ddl;
    auto test_definition = DDStructure("test").addElement<uint32_t>("value1", 1, "unknown");
    codec::CodecFactory decoder_check(test_definition);
    EXPECT_EQ(a_util::result::SUCCESS, decoder_check.isValid());
}

TEST(TesterDDLTypeReflection, createTypeFromStructDefaultAlignment)
{
    using namespace ddl;
    auto sub_definition = DDStructureGenerator<sub_struct>("sub_struct")
                              .addElement("value1", &sub_struct::value1)
                              .addElement("value2", &sub_struct::value2)
                              .addElement("value3", &sub_struct::value3);

    checkElementDeserializedBytePosAndAlignmentSet(
        sub_definition,
        "value1",
        detail::memberPointerToOffset<sub_struct>(&sub_struct::value1),
        std::min(alignof(decltype(sub_struct::value1)), alignof(sub_struct)));

    checkElementDeserializedBytePosAndAlignmentSet(
        sub_definition,
        "value2",
        detail::memberPointerToOffset<sub_struct>(&sub_struct::value2),
        std::min(alignof(decltype(sub_struct::value2)), alignof(sub_struct)));

    checkElementDeserializedBytePosAndAlignmentSet(
        sub_definition,
        "value3",
        detail::memberPointerToOffset<sub_struct>(&sub_struct::value3),
        std::min(alignof(decltype(sub_struct::value3)), alignof(sub_struct)));

    auto array_definition = DDStructureGenerator<array_struct>("array_struct")
                                .addElement("values", &array_struct::values);

    checkElementDeserializedBytePosAndAlignmentSet(
        array_definition,
        "values",
        detail::memberPointerToOffset<array_struct>(&array_struct::values),
        std::min(alignof(decltype(array_struct::values)), alignof(array_struct)));

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

    codec::CodecFactory decoder_check(test_definition);
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

    auto test_definition = DDStructureGenerator<test_pack1>("test")
                               .addElement("value1", &test_pack1::value1)
                               .addElement("sub", &test_pack1::sub, sub_definition)
                               .addElement("array", &test_pack1::array, array_definition)
                               .addElement("enumeration", &test_pack1::enumeration, enum_definition)
                               .addElement("bool_value", &test_pack1::bool_value);

    codec::CodecFactory decoder_check(test_definition);
    EXPECT_TRUE(isOk(decoder_check.isValid())) << "The created DDStructure is not valid";
    EXPECT_EQ(decoder_check.getStaticBufferSize(), sizeof(test_pack1))
        << "The created DDStructure does not have the expected size";
    auto struct_access =
        test_definition.getDD().getStructTypeAccess(test_definition.getStructName());
    EXPECT_TRUE(struct_access) << "The created StructAccess is not valid";
    EXPECT_EQ(struct_access.getStaticStructSize(), sizeof(test_pack1))
        << "The created DDStructure does not have the expected size";

    EXPECT_EQ(*struct_access.getStructType().getAlignment(), alignof(test_pack1));
    for (auto& elem: struct_access) {
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

    codec::CodecFactory decoder_check(test_definition);
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

/**
 * Detection of missing elements at the end is switched off for padding (by default)
 * and it uses padding to set the positions.
 * The structure is always valid!
 */
TEST(TesterDDLTypeReflection, addWithPaddingByDefault)
{
    using namespace ddl;
    auto child1_definition = DDStructureGenerator<child1>("child1", 1)
                                 .addElement("a", &child1::a)
                                 .addElement("b", &child1::b);

    ASSERT_EQ(child1_definition.getSize(), sizeof(child1));

    auto child2_definition = DDStructureGenerator<child2>("child2").addElement("a", &child2::a);

    ASSERT_EQ(child2_definition.getSize(), sizeof(child2));

    auto parent_definition = DDStructureGenerator<parent>("parent");
    // this will now validate while adding the DDStructure
    ASSERT_NO_THROW(parent_definition.addElement("child1", &parent::c1, child1_definition));
    ASSERT_NO_THROW(parent_definition.addElement("child2", &parent::c2, child2_definition));

    ASSERT_EQ(parent_definition.getSize(), sizeof(parent));
}

/**
 * Detection of missing elements in the middle is switched off for padding (by default)
 * and it uses padding to set the positions.
 *
 */
TEST(TesterDDLTypeReflection, missingWithPaddingInTheMiddle)
{
    using namespace ddl;
    auto child1_definition = DDStructureGenerator<child1>("child1", 1).addElement("b", &child1::b);

    // with padding the definition is always valid and filled with padding
    ASSERT_EQ(child1_definition.getSize(), sizeof(child1));

    auto child2_definition = DDStructureGenerator<child2>("child2").addElement("a", &child2::a);
    ASSERT_EQ(child2_definition.getSize(), sizeof(child2));

    auto parent_definition = DDStructureGenerator<parent>("parent", 1);
    // this will now validate while adding the DDStructure
    ASSERT_NO_THROW(parent_definition.addElement("child1", &parent::c1, child1_definition));

    // we do not add "child2" but we add padding automatically
    ASSERT_EQ(parent_definition.getSize(), sizeof(parent));
}

/**
 * Detection of misordered elements are detected also if padding is switched on.
 */
TEST(TesterDDLTypeReflection, withPaddingStillThrowsIfUnordered)
{
    using namespace ddl;
    auto child1_definition = DDStructureGenerator<child1>("child1", 1).addElement("b", &child1::b);
    ASSERT_ANY_THROW(child1_definition.addElement("a", &child1::a););
}

/**
 * Detection of missing or misordered elements with no padding.
 */
TEST(TesterDDLTypeReflection, detectMissingElementsNoPadding)
{
    using namespace ddl;
    auto child1_definition = DDStructureGenerator<child1, false>("child1")
                                 .addElement("a", &child1::a)
                                 .addElement("b", &child1::b);
    // we deliberately omit the last member
    //        .Add("c", &child1::c);
    ASSERT_NE(child1_definition.getSize(), sizeof(child1));
    // this must throw
    ASSERT_ANY_THROW(child1_definition.getStructType());
    ASSERT_ANY_THROW(child1_definition.getDD());
    ASSERT_ANY_THROW(child1_definition.getStructDescription());

    // this will throw because the definition is not valid while calling getStructType()
    ASSERT_ANY_THROW(ddl::codec::CodecFactory codec_factory(child1_definition));

    auto child1_definition_missorderd = DDStructureGenerator<child1, false>("child1");

    // we try to add one before the other
    ASSERT_ANY_THROW(child1_definition_missorderd.addElement("b", &child1::b););

    child1_definition_missorderd.addElement("a", &child1::a);

    // detection of a element gap when not finding a valid alignment
    ASSERT_ANY_THROW(child1_definition_missorderd.addElement("c", &child1::c););

    ASSERT_NE(child1_definition_missorderd.getSize(), sizeof(child1));

    // we added the elements in the wrong order, it is detected
    ASSERT_ANY_THROW(child1_definition_missorderd.getStructDescription());

    auto child2_definition =
        DDStructureGenerator<child2, false>("child2").addElement("a", &child2::a);

    auto parent_definition = DDStructureGenerator<parent, false>("parent");

    ASSERT_ANY_THROW(parent_definition.addElement("child1", &parent::c1, child1_definition));
    ASSERT_ANY_THROW(parent_definition.addElement("child2", &parent::c2, child2_definition));
}

enum class test_enum_class : uint64_t { a, b, c };
enum class test_enum_class_no_typedefined { a, b, c };

/**
 * Test the enumeration generation with a underlying type and class
 */
TEST(TesterDDLTypeReflection, testForEnumGeneratorWithUnderlyingTypeCTOR)
{
    using namespace ddl;
    // type but with class and concreate type
    auto enum_class_definition = DDEnumGenerator<test_enum_class>(
        "test_enum_class",
        {{"a", test_enum_class::a}, {"b", test_enum_class::b}, {"c", test_enum_class::c}});

    ASSERT_EQ(enum_class_definition.getEnumType().getElements().getSize(), 3);

    // class but no type
    auto enum_no_type_class_definition =
        DDEnumGenerator<test_enum_class_no_typedefined>("test_enum_class_no_typedefined",
                                                        {{"a", test_enum_class_no_typedefined::a},
                                                         {"b", test_enum_class_no_typedefined::b},
                                                         {"c", test_enum_class_no_typedefined::c}});
    ASSERT_EQ(enum_no_type_class_definition.getEnumType().getElements().getSize(), 3);
}

/**
 * Test the enumeration generation with a underlying type and class
 */
TEST(TesterDDLTypeReflection, testForEnumGeneratorWithUnderlyingTypeAddElement)
{
    using namespace ddl;
    // type but with class and concreate type
    auto enum_class_definition = DDEnumGenerator<test_enum_class>("test_enum_class")
                                     .addElement("a", test_enum_class::a)
                                     .addElement("b", test_enum_class::b)
                                     .addElement("c", test_enum_class::c);

    ASSERT_EQ(enum_class_definition.getEnumType().getElements().getSize(), 3);

    // class but no type
    auto enum_no_type_class_definition =
        DDEnumGenerator<test_enum_class_no_typedefined>("test_enum_class_no_typedefined")
            .addElement("a", test_enum_class_no_typedefined::a)
            .addElement("b", test_enum_class_no_typedefined::b)
            .addElement("c", test_enum_class_no_typedefined::c);
    ASSERT_EQ(enum_no_type_class_definition.getEnumType().getElements().getSize(), 3);
}

// we need to disable the "C4324" : structure was padded due to alignment specifier
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4324)
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
#endif
#pragma pack(push, 1)
// expected to mis-compile, and result in something non-portable
// alignment of pack_alignas_conflict_struct is undefined
struct pack_alignas_conflict_struct_pack1_aligned4 {
    uint8_t a;
    alignas(4) uint8_t b;
    uint64_t c; // alignment of member c is packed to 1 in msvc
    alignas(2) uint8_t d;
    uint64_t e; // alignment of member e is packed to 1 in msvc
};
#pragma pack(pop)
#ifdef _MSC_VER
#pragma warning(pop)
#else
//#pragma GCC diagnostic pop
#endif

/**
 * Using a mismatched packed1/aligned4 structure and add all elements.
 * Expecting the right positions!
 */
TEST(TesterDDLTypeReflection, packAlignasConflictPacked1Aligned4)
{
    using namespace ddl;
    // but with padding we can resolve it
    // the alignment of the struct is considered and raised to 4
    // (the alignment of a struct is always the maximum alignment of its members)
    auto aligned_definition = DDStructureGenerator<pack_alignas_conflict_struct_pack1_aligned4>(
                                  "pack_alignas_conflict_struct_pack1_aligned4")
                                  .addElement("a", &pack_alignas_conflict_struct_pack1_aligned4::a)
                                  .addElement("b", &pack_alignas_conflict_struct_pack1_aligned4::b);

    ASSERT_NO_THROW(
        aligned_definition.addElement("c", &pack_alignas_conflict_struct_pack1_aligned4::c););
    ASSERT_NO_THROW(
        aligned_definition.addElement("d", &pack_alignas_conflict_struct_pack1_aligned4::d););
    ASSERT_NO_THROW(
        aligned_definition.addElement("e", &pack_alignas_conflict_struct_pack1_aligned4::e););

    ASSERT_EQ(aligned_definition.getSize(), sizeof(pack_alignas_conflict_struct_pack1_aligned4));
    ASSERT_EQ(aligned_definition.getAlignment(),
              alignof(pack_alignas_conflict_struct_pack1_aligned4));

    ASSERT_NO_THROW(std::cout << ddl::DDString::toXMLString(aligned_definition.getDD()););

    size_t expected_alignment_platform_dependent_4u =
        4u; // in windows the alignof(T) is 4 (raised to 4 because of the member alignas(4) for
            // b!!)
    size_t expected_alignment_platform_dependent_2u =
        2u; // in windows the alignof(T) is 4 (raised to 4 because of the member alignas(4) for
            // b!!)
    // this is for gcc and clang where the pack has a higher "weight" and alignas is ignored!
    if (aligned_definition.getAlignment() == 1) {
        expected_alignment_platform_dependent_4u = 1u;
        expected_alignment_platform_dependent_2u = 1u;
        // in linux gcc the alignas(4) for b is ignored and it will use the pack alignment
    }

    // in MSVC this is pos 0 / alignment 1
    checkElementDeserializedBytePosAndAlignmentSet(
        aligned_definition,
        "a",
        detail::memberPointerToOffset<pack_alignas_conflict_struct_pack1_aligned4>(
            &pack_alignas_conflict_struct_pack1_aligned4::a),
        1u);

    // in MSVC this is pos 4 / alignment 4
    // in GCC this is pos 4 / alignment 1
    checkElementDeserializedBytePosAndAlignmentSet(
        aligned_definition,
        "b",
        detail::memberPointerToOffset<pack_alignas_conflict_struct_pack1_aligned4>(
            &pack_alignas_conflict_struct_pack1_aligned4::b),
        expected_alignment_platform_dependent_4u);

    // in MSVC this is pos 5 / alignment 1
    // in GCC this is pos 5 / alignment 1
    checkElementDeserializedBytePosAndAlignmentSet(
        aligned_definition,
        "c",
        detail::memberPointerToOffset<pack_alignas_conflict_struct_pack1_aligned4>(
            &pack_alignas_conflict_struct_pack1_aligned4::c),
        1u);

    // in MSVC this is pos 14 / alignment 2
    // in GCC this is pos 14 / alignment 1
    checkElementDeserializedBytePosAndAlignmentSet(
        aligned_definition,
        "d",
        detail::memberPointerToOffset<pack_alignas_conflict_struct_pack1_aligned4>(
            &pack_alignas_conflict_struct_pack1_aligned4::d),
        expected_alignment_platform_dependent_2u);

    // in MSVC and GCC this is pos 15 / alignment 1
    checkElementDeserializedBytePosAndAlignmentSet(
        aligned_definition,
        "e",
        detail::memberPointerToOffset<pack_alignas_conflict_struct_pack1_aligned4>(
            &pack_alignas_conflict_struct_pack1_aligned4::e),
        1u);
}

/**
 * Using a mismatched packed1/aligned4 structure and do not add some elements at beginning
 * and the middle.
 *
 * Expecting padding elements and the right positions!
 */
TEST(TesterDDLTypeReflection, packAlignasConflictPacked1Aligned4WithPadding)
{
    using namespace ddl;
    // but with padding we can resolve it
    // the alignment of the struct is considered and raised to 4
    // (the alignment of a struct is always the maximum alignment of its members)
    auto aligned_definition = DDStructureGenerator<pack_alignas_conflict_struct_pack1_aligned4>(
                                  "pack_alignas_conflict_struct_pack1_aligned4")
                                  .addElement("b", &pack_alignas_conflict_struct_pack1_aligned4::b);

    ASSERT_NO_THROW(
        aligned_definition.addElement("c", &pack_alignas_conflict_struct_pack1_aligned4::c););
    ASSERT_NO_THROW(
        aligned_definition.addElement("e", &pack_alignas_conflict_struct_pack1_aligned4::e););

    ASSERT_EQ(aligned_definition.getSize(), sizeof(pack_alignas_conflict_struct_pack1_aligned4));
    ASSERT_EQ(aligned_definition.getAlignment(),
              alignof(pack_alignas_conflict_struct_pack1_aligned4));

    ASSERT_NO_THROW(std::cout << ddl::DDString::toXMLString(aligned_definition.getDD()););

    // in MSVC and GCC this is pos 4 / alignment 1 because of padding
    checkElementDeserializedBytePosAndAlignmentSet(
        aligned_definition,
        "b",
        detail::memberPointerToOffset<pack_alignas_conflict_struct_pack1_aligned4>(
            &pack_alignas_conflict_struct_pack1_aligned4::b),
        1u);

    // in MSVC and GCC this is pos 5 / alignment 1
    checkElementDeserializedBytePosAndAlignmentSet(
        aligned_definition,
        "c",
        detail::memberPointerToOffset<pack_alignas_conflict_struct_pack1_aligned4>(
            &pack_alignas_conflict_struct_pack1_aligned4::c),
        1u);

    // in MSVC and GCC this is pos 15 / alignment 4 because of padding
    checkElementDeserializedBytePosAndAlignmentSet(
        aligned_definition,
        "e",
        detail::memberPointerToOffset<pack_alignas_conflict_struct_pack1_aligned4>(
            &pack_alignas_conflict_struct_pack1_aligned4::e),
        1u);
}

// we need to disable the "C4324" : structure was padded due to alignment specifier
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4324)
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
#endif
#pragma pack(push, 2)
// expected to mis-compile, and result in something non-portable
// alignment of pack_alignas_conflict_struct is undefined
struct pack_alignas_conflict_struct_packed2_aligned4 {
    uint8_t a;
    alignas(4) uint8_t b; // alignment of member c is packed to 2 in msvc
    uint64_t c;
    alignas(2) uint8_t d;
    uint64_t e; // alignment of member e is packed to 2 in msvc
};
#pragma pack(pop)
#ifdef _MSC_VER
#pragma warning(pop)
#else
#pragma GCC diagnostic pop
#endif

/**
 * Using a mismatched packed2/aligned4 structure and add all elements.
 * Expecting the right positions!
 */
TEST(TesterDDLTypeReflection, packAlignasConflictPacked2Aligned4)
{
    using namespace ddl;
    pack_alignas_conflict_struct_packed2_aligned4 layout_struct = {1, 2, 3, 4, 5};
    a_util::maybe_unused(layout_struct);

    size_t size_of_struct = sizeof(layout_struct);
    size_t aligned_value_struct = alignof(pack_alignas_conflict_struct_packed2_aligned4);
    size_t aligned_value_a = (uint8_t*)&layout_struct.a - (uint8_t*)&layout_struct;
    size_t aligned_value_b = (uint8_t*)&layout_struct.b - (uint8_t*)&layout_struct;
    size_t aligned_value_c = (uint8_t*)&layout_struct.c - (uint8_t*)&layout_struct;
    size_t aligned_value_d = (uint8_t*)&layout_struct.d - (uint8_t*)&layout_struct;
    size_t aligned_value_e = (uint8_t*)&layout_struct.e - (uint8_t*)&layout_struct;

    std::cout << "size_of_struct " << size_of_struct << std::endl;
    a_util::maybe_unused(size_of_struct);

    std::cout << "aligned_value_struct " << aligned_value_struct << std::endl;
    a_util::maybe_unused(aligned_value_struct);

    std::cout << "aligned_value_a " << aligned_value_a << std::endl;
    a_util::maybe_unused(aligned_value_a);

    std::cout << "aligned_value_b " << aligned_value_b << std::endl;
    a_util::maybe_unused(aligned_value_b);

    std::cout << "aligned_value_c " << aligned_value_c << std::endl;
    a_util::maybe_unused(aligned_value_c);

    std::cout << "aligned_value_d " << aligned_value_d << std::endl;
    a_util::maybe_unused(aligned_value_d);

    std::cout << "aligned_value_e " << aligned_value_e << std::endl;
    a_util::maybe_unused(aligned_value_e);

    // the alignment of the struct is considered and raised to 4
    // (the alignment of a struct is always the maximum alignment of its members)
    auto aligned_definition =
        DDStructureGenerator<pack_alignas_conflict_struct_packed2_aligned4>(
            "pack_alignas_conflict_struct_pack2_aligned4")
            .addElement("a", &pack_alignas_conflict_struct_packed2_aligned4::a)
            .addElement("b", &pack_alignas_conflict_struct_packed2_aligned4::b);

    ASSERT_NO_THROW(
        aligned_definition.addElement("c", &pack_alignas_conflict_struct_packed2_aligned4::c););
    ASSERT_NO_THROW(
        aligned_definition.addElement("d", &pack_alignas_conflict_struct_packed2_aligned4::d););
    ASSERT_NO_THROW(
        aligned_definition.addElement("e", &pack_alignas_conflict_struct_packed2_aligned4::e););

    ASSERT_EQ(aligned_definition.getSize(), sizeof(pack_alignas_conflict_struct_packed2_aligned4));
    ASSERT_EQ(aligned_definition.getAlignment(),
              alignof(pack_alignas_conflict_struct_packed2_aligned4));

    ASSERT_NO_THROW(std::cout << ddl::DDString::toXMLString(aligned_definition.getDD()););

    auto struct_access =
        aligned_definition.getDD().getStructTypeAccess(aligned_definition.getStructName());

    size_t expected_alignment_platform_dependent_4u =
        4u; // in windows the alignof(T) is 4 (raised to 4 because of the member alignas(4) for
            // b!!)

    // this is for gcc and clang where the pack has a higher "weight" and alignas is ignored!
    if (aligned_definition.getAlignment() == 2) {
        expected_alignment_platform_dependent_4u = 2u;
        // in linux gcc the alignas(4) for b is ignored for the structure so the structure
        // alignment is 2, the padding is different
    }

    // in MSVC this is pos 0 / align 1u
    checkElementDeserializedBytePosAndAlignmentSet(
        aligned_definition,
        "a",
        detail::memberPointerToOffset<pack_alignas_conflict_struct_packed2_aligned4>(
            &pack_alignas_conflict_struct_packed2_aligned4::a),
        1u);

    // in MSVC this is pos 4 / alignment 4
    // in GCC this is pos 4 / alignment 1 ... should be 2 but we can not detect that, 1 also
    // fits
    checkElementDeserializedBytePosAndAlignmentSet(
        aligned_definition,
        "b",
        detail::memberPointerToOffset<pack_alignas_conflict_struct_packed2_aligned4>(
            &pack_alignas_conflict_struct_packed2_aligned4::b),
        expected_alignment_platform_dependent_4u);

    // in MSVC and GCC this is pos 6 / alignment 2
    checkElementDeserializedBytePosAndAlignmentSet(
        aligned_definition,
        "c",
        detail::memberPointerToOffset<pack_alignas_conflict_struct_packed2_aligned4>(
            &pack_alignas_conflict_struct_packed2_aligned4::c),
        2u);

    // in MSVC and GCC this is pos 14 / alignment should be 2 (but we can not detect this)
    checkElementDeserializedBytePosAndAlignmentSet(
        aligned_definition,
        "d",
        detail::memberPointerToOffset<pack_alignas_conflict_struct_packed2_aligned4>(
            &pack_alignas_conflict_struct_packed2_aligned4::d),
        // should be 2u, but is 1u it fits!
        1u);

    // in MSVC this is pos 16 / alignment 4
    // in GCC this is pos 16 / alignment 2
    checkElementDeserializedBytePosAndAlignmentSet(
        aligned_definition,
        "e",
        detail::memberPointerToOffset<pack_alignas_conflict_struct_packed2_aligned4>(
            &pack_alignas_conflict_struct_packed2_aligned4::e),
        expected_alignment_platform_dependent_4u);
}

/**
 * Using a mismatched packed2/aligned4 structure and do not add some elements at beginning
 * and the middle.
 *
 * Expecting padding elements and the right positions!
 */
TEST(TesterDDLTypeReflection, packAlignasConflictPacked2Aligned4WithPadding)
{
    using namespace ddl;
    // the alignment of the struct is considered and raised to 4
    // (the alignment of a struct is always the maximum alignment of its members)
    auto aligned_definition =
        DDStructureGenerator<pack_alignas_conflict_struct_packed2_aligned4>(
            "pack_alignas_conflict_struct_pack2_aligned4")
            .addElement("b", &pack_alignas_conflict_struct_packed2_aligned4::b);

    ASSERT_NO_THROW(
        aligned_definition.addElement("c", &pack_alignas_conflict_struct_packed2_aligned4::c););

    ASSERT_NO_THROW(
        aligned_definition.addElement("e", &pack_alignas_conflict_struct_packed2_aligned4::e););

    ASSERT_EQ(aligned_definition.getSize(), sizeof(pack_alignas_conflict_struct_packed2_aligned4));
    ASSERT_EQ(aligned_definition.getAlignment(),
              alignof(pack_alignas_conflict_struct_packed2_aligned4));

    ASSERT_NO_THROW(std::cout << ddl::DDString::toXMLString(aligned_definition.getDD()););

    auto struct_access =
        aligned_definition.getDD().getStructTypeAccess(aligned_definition.getStructName());

    // in MSVC and GCC this is pos 4 / alignment 1 because of padding
    checkElementDeserializedBytePosAndAlignmentSet(
        aligned_definition,
        "b",
        detail::memberPointerToOffset<pack_alignas_conflict_struct_packed2_aligned4>(
            &pack_alignas_conflict_struct_packed2_aligned4::b),
        1u);

    // in MSVC and GCC this is pos 6 / alignment 2
    checkElementDeserializedBytePosAndAlignmentSet(
        aligned_definition,
        "c",
        detail::memberPointerToOffset<pack_alignas_conflict_struct_packed2_aligned4>(
            &pack_alignas_conflict_struct_packed2_aligned4::c),
        2u);

    // in MSVC this is pos 16 / alignment 4 we should use a padding element here, but it fits
    // also with alignment in GCC this is pos 16 / alignment 1 because of padding
    size_t expected_alignment_platform_dependent =
        4u; // in windows the alignof(T) is 4 (raised to 4 because of the member alignas(4) for
            // b!!)
    // this is for gcc and clang where the pack has a higher "weight" and alignas is ignored!
    if (aligned_definition.getAlignment() == 2) {
        expected_alignment_platform_dependent = 1u;
        // in linux gcc the alignas(4) for b is ignored for the structure so padding is added!!
        // and we expect 1u after padding
    }
    checkElementDeserializedBytePosAndAlignmentSet(
        aligned_definition,
        "e",
        detail::memberPointerToOffset<pack_alignas_conflict_struct_packed2_aligned4>(
            &pack_alignas_conflict_struct_packed2_aligned4::e),
        expected_alignment_platform_dependent);
}

enum class EnumForValue : uint8_t { one = 1, two = 2, three = 3 };

struct SubStructWithInfo {
    EnumForValue value_1;
    uint64_t value_2;
};

struct StructWithInfos {
    SubStructWithInfo sub_struct;
    uint64_t value;
};

static constexpr const char* const expected_dd_generator_with_info = R"(<?xml version="1.0"?>
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
    <enums>
        <enum name="EnumForValue" type="tUInt8">
            <element name="one" value="1" />
            <element name="two" value="2" />
            <element name="three" value="3" />
        </enum>
    </enums>
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
            <element name="value_1" type="EnumForValue" description="test_description" unit="Ampere" comment="test_comment" arraysize="1" value="two" min="-12" max="42" default="3" scale="0.5" offset="10">
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
</ddl:ddl>
)";

/**
 * @detail Check the resulting DataDefinition by setting additional info on struct and elements.
 */
TEST(TesterDDLTypeReflection, addAdditionalInfo)
{
    using namespace ddl;

    DDEnumGenerator<EnumForValue> enum_for_value("EnumForValue");
    enum_for_value.addElement("one", EnumForValue::one);
    enum_for_value.addElement("two", EnumForValue::two);
    enum_for_value.addElement("three", EnumForValue::three);

    DDStructureGenerator<SubStructWithInfo> my_sub_struct("SubStructWithInfo");
    my_sub_struct.setStructInfo("this is a comment");

    my_sub_struct.addElement("value_1", &SubStructWithInfo::value_1, enum_for_value);
    my_sub_struct.setElementInfo(
        "value_1", "test_description", "test_comment", "two", "-12", "42", "3", "0.5", "10");
    my_sub_struct.setElementUnit("value_1", BaseUnit<unit::Ampere>());

    my_sub_struct.addElement("value_2", &SubStructWithInfo::value_2);
    my_sub_struct.setElementInfo(
        "value_2", "test_description", "test_comment", {}, "-12", "42", "3", "0.5", "10");

    DDUnit unit("unit_to_set",
                "1",
                "2",
                "3",
                {DDRefUnit(BaseUnit<unit::Metre>(), 1, UnitPrefix<unit_prefix::kilo>()),
                 DDRefUnit(BaseUnit<unit::Mole>(), 32, UnitPrefix<unit_prefix::hecto>())});
    my_sub_struct.setElementUnit("value_2", unit);

    DDStructureGenerator<StructWithInfos> my_struct("StructWithInfos");
    my_sub_struct.setStructInfo("this is another comment");
    my_struct.addElement("sub_struct", &StructWithInfos::sub_struct, my_sub_struct);

    my_struct.addElement("value", &StructWithInfos::value);
    my_struct.setElementInfo(
        "value", "test_description", "test_comment", {}, "-12", "42", "3", "0.5", "10");

    my_struct.setElementUnit("value", unit);

    auto result = my_struct.getStructDescription();

    dd::DataDefinition expected_dd = DDString::fromXMLString(expected_dd_generator_with_info);
    ASSERT_EQ(a_util::result::SUCCESS, DDCompare::isEqual(expected_dd, my_struct.getDD()));
}

/**
 * @detail Check if setting additional infos to non existing elements and uninitialized struct
 * throws.
 */
TEST(TesterDDLTypeReflection, addAdditionalInfoNegative)
{
    using namespace ddl;
    DDUnit unit("unit_to_set",
                "1",
                "2",
                "3",
                {DDRefUnit(BaseUnit<unit::Metre>(), 1, UnitPrefix<unit_prefix::kilo>()),
                 DDRefUnit(BaseUnit<unit::Mole>(), 32, UnitPrefix<unit_prefix::hecto>())});

    DDStructureGenerator<StructWithInfos> my_sub_struct("test");
    my_sub_struct.addElement("value_exists", &StructWithInfos::value);

    ASSERT_THROW(my_sub_struct.setElementInfo("value_not_exists",
                                              "test_description",
                                              "test_comment",
                                              "123",
                                              "-12",
                                              "42",
                                              "3",
                                              "0.5",
                                              "10"),
                 dd::Error);
    ASSERT_THROW(my_sub_struct.setElementUnit("value_not_exists", BaseUnit<unit::Ampere>()),
                 dd::Error);
    ASSERT_THROW(my_sub_struct.setElementUnit("value_not_exists", unit), dd::Error);

    // try to set value if the type is not an enum type
    ASSERT_THROW(my_sub_struct.setElementInfo("value_exists", {}, {}, "123"), dd::Error);
}
