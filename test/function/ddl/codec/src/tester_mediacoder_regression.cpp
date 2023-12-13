/**
 * @file
 * Description is missing.
 *
 * Copyright @ 2021 VW Group. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "../../_common/adtf_compat.h"

#include <a_util/filesystem.h>
#include <ddl/codec/codec_factory.h>
#include <ddl/serialization/serialization.h>

#include <gtest/gtest.h>

#define PLATFORM_BYTEORDER __get_platform_byteorder()

/**
 * This function retrieves the platform dependent byte order.
 * @return @ref platform_not_supported, @ref plattform_little_endian_8 or @ref
 * platform_big_endian_8.
 */
static inline unsigned char __get_platform_byteorder()
{
    std::uint32_t ui32Value = 0x01020304;
    if (((unsigned char*)&ui32Value)[0] == 0x04 && ((unsigned char*)&ui32Value)[2] == 0x02) {
        return ddl::dd::ByteOrder::plattform_little_endian_8;
    }
    else if (((unsigned char*)&ui32Value)[0] == 0x01 && ((unsigned char*)&ui32Value)[2] == 0x03) {
        return ddl::dd::ByteOrder::platform_big_endian_8;
    }
    else {
        return ddl::dd::ByteOrder::platform_not_supported;
    }
}

using namespace ddl;

/**
 * @detail Test that initializes a media coder by using Create and setValue
 * @req_id ACORE-1265
 */
TEST(cTesterMediaCoder, TestParseStructureSimple)
{
    std::string strToParse = "<description> <structs>";
    strToParse.append(ADTF_MEDIA_DESC_MEDIA_TYPEINFO);
    strToParse.append("</structs> </description>");
    codec::CodecFactory oFactory("tMediaTypeInfo", strToParse.c_str());

    tMediaTypeInfo sInfo;

    codec::Codec oCodec = oFactory.makeCodecFor(&sInfo, sizeof(sInfo));

    const uint32_t ui32MajorType = 1;
    const uint32_t ui32SubType = 2;
    const uint32_t ui32Flags = 3;

    ASSERT_NO_THROW(oCodec.getElement("ui32MajorType").setVariantValue(ui32MajorType));
    ASSERT_NO_THROW(oCodec.getElement("ui32SubType").setValue(ui32SubType));
    ASSERT_NO_THROW(oCodec.getElement("ui32Flags").setValue(ui32Flags));

    ASSERT_EQ(sInfo.ui32Flags, 3u);
    ASSERT_EQ(sInfo.ui32SubType, 2u);
    ASSERT_EQ(sInfo.ui32MajorType, 1u);
}

/**
 * @detail Test that initializes a media coder by using Create and setValue with more complex values
 * @req_id ACORE-1265
 */
TEST(cTesterMediaCoder, TestParseNotSoVerySimple)
{
    std::string strToParse = "<description> <structs>";
    strToParse.append(ADTF_MEDIA_DESC_MEDIA_TYPEINFO);
    strToParse.append("<struct name=\"tMyType\">");
    strToParse.append("<element type=\"tMediaTypeInfo\" name=\"value1\" bytepos=\"0\" "
                      "arraysize=\"1\" byteorder=\"LE\" alignment=\"1\"/>");
    strToParse.append("<element type=\"tMediaTypeInfo\" name=\"value2\" bytepos=\"12\" "
                      "arraysize=\"1\" byteorder=\"LE\" alignment=\"1\"/>");
    strToParse.append("</struct>");
    strToParse.append("</structs> </description>");

    codec::CodecFactory oFactory("tMyType", strToParse.c_str());

    typedef struct {
        tMediaTypeInfo sInfo1;
        tMediaTypeInfo sInfo2;
    } tMyType;
    tMyType sMyType;

    codec::Codec oCodec = oFactory.makeCodecFor(&sMyType, sizeof(sMyType));

    const uint32_t ui32MajorType = 1;
    const uint32_t ui32SubType = 2;
    const uint32_t ui32Flags = 3;

    ASSERT_NO_THROW(oCodec.getElement("value1.ui32MajorType").setValue(ui32MajorType));
    ASSERT_NO_THROW(oCodec.getElement("value1.ui32SubType").setValue(ui32SubType));
    ASSERT_NO_THROW(oCodec.getElement("value1.ui32Flags").setValue(ui32Flags));

    ASSERT_NO_THROW(oCodec.getElement("value2.ui32MajorType").setValue(ui32MajorType));
    ASSERT_NO_THROW(oCodec.getElement("value2.ui32SubType").setValue(ui32SubType));
    ASSERT_NO_THROW(oCodec.getElement("value2.ui32Flags").setValue(ui32Flags));

    ASSERT_EQ(sMyType.sInfo1.ui32Flags, 3u);
    ASSERT_EQ(sMyType.sInfo1.ui32SubType, 2u);
    ASSERT_EQ(sMyType.sInfo1.ui32MajorType, 1u);

    ASSERT_EQ(sMyType.sInfo2.ui32Flags, 3u);
    ASSERT_EQ(sMyType.sInfo2.ui32SubType, 2u);
    ASSERT_EQ(sMyType.sInfo2.ui32MajorType, 1u);
}

/**
 * @detail Test that initializes a media coder by using Create and setValue with a union in the
 * values
 * @req_id ACORE-1265
 */
TEST(cTesterMediaCoder, TestParseALittleBitDifficult)
{
    std::string strToParse = "<description> <structs>";
    strToParse.append(ADTF_MEDIA_DESC_MEDIA_TYPEINFO);
    strToParse.append("<struct name=\"tMyType\">");
    strToParse.append("<element type=\"tMediaTypeInfo\" name=\"value1\" bytepos=\"0\" "
                      "arraysize=\"1\" byteorder=\"LE\" alignment=\"1\"/>");
    strToParse.append("<element type=\"tMediaTypeInfo\" name=\"value2\" bytepos=\"4\" "
                      "arraysize=\"1\" byteorder=\"LE\" alignment=\"1\"/>");
    strToParse.append("</struct>");
    strToParse.append("</structs> </description>");

    codec::CodecFactory oFactory("tMyType", strToParse.c_str());

    typedef struct {
        tMediaTypeInfo sInfo1;
        tMediaTypeInfo sInfo2;
    } tMyType;
    tMyType sMyType;

    codec::Codec oCodec = oFactory.makeCodecFor(&sMyType, sizeof(sMyType));

    const uint32_t ui32MajorType = 1;
    const uint32_t ui32SubType = 2;
    const uint32_t ui32Flags = 3;

    ASSERT_NO_THROW(oCodec.getElement("value1.ui32MajorType").setValue(ui32MajorType));

    ASSERT_NO_THROW(oCodec.getElement("value1.ui32SubType").setValue(ui32SubType));

    ASSERT_NO_THROW(oCodec.getElement("value1.ui32Flags").setValue(ui32Flags));

    ASSERT_NO_THROW(
        oCodec.getElement("value2").setRawValue(&sMyType.sInfo1, sizeof(sMyType.sInfo1)));

    ASSERT_EQ(sMyType.sInfo1.ui32MajorType, 1u);
    ASSERT_EQ(sMyType.sInfo1.ui32SubType, 2u);
    ASSERT_EQ(sMyType.sInfo1.ui32Flags, 3u);

    ASSERT_EQ(sMyType.sInfo2.ui32MajorType, 1u);
    ASSERT_EQ(sMyType.sInfo2.ui32SubType, 2u);
    ASSERT_EQ(sMyType.sInfo2.ui32Flags, 3u);
}

/**
 * @detail The parsing of data types will be tested with very different possibilities to add a type.
 * A media coder is initialized using Create with dom string
 * @req_id ACORE-1265 ACORE-2641
 */
TEST(cTesterMediaCoder, TestParseOnlyDataTypes)
{
    {
        std::string strDataTypeToParse = "<datatype name=\"mytype1\" size=\"8\" >";
        strDataTypeToParse.append("</datatype>");
        codec::CodecFactory oFactory("tMyType", strDataTypeToParse.c_str());
        ASSERT_NE(a_util::result::SUCCESS, oFactory.isValid());
        ASSERT_EQ(oFactory.getStaticElementCount(), 0U);
    }

    {
        std::string strDataTypeToParse = "<datatypes><datatype name=\"mytype1\" size=\"8\" >";
        strDataTypeToParse.append("</datatype></datatypes>");

        codec::CodecFactory oFactory("tMyType", strDataTypeToParse.c_str());
        ASSERT_NE(a_util::result::SUCCESS, oFactory.isValid());
        ASSERT_EQ(oFactory.getStaticElementCount(), 0U);
    }

    {
        // error but nothing happen while datatype parsing
        std::string strDataTypeToParse = "<test><datatypes><datatype name=\"mytype1\" size=\"8\" >";
        strDataTypeToParse.append("</datatype></datatypes></test>");
        codec::CodecFactory oFactory("tMyType", strDataTypeToParse.c_str());
        ASSERT_NE(a_util::result::SUCCESS, oFactory.isValid());
        ASSERT_EQ(oFactory.getStaticElementCount(), 0U);
    }

    {
        // size is not valid
        std::string strDataTypeToParse =
            "<test><datatypes><datatype name=\"mytype1\" size=\"-1\" >";
        strDataTypeToParse.append("</datatype></datatypes></test>");
        codec::CodecFactory oFactory("tMyType", strDataTypeToParse.c_str());
        ASSERT_NE(a_util::result::SUCCESS, oFactory.isValid());
        ASSERT_EQ(oFactory.getStaticElementCount(), 0U);
    }
}

/**
 * @detail The parsing of struct will be tested with very different possibilities to add a struct
 * A media coder is initialized using Create with dom string
 * @req_id ACORE-1265 ACORE-2641
 */
TEST(cTesterMediaCoder, TestParseOnlyStruct)
{
    {
        // with no element
        std::string strStructToParse = "<struct name=\"mystruct\" alignment=\"8\" version=\"1\">";
        strStructToParse.append("</struct>");
        codec::CodecFactory oFactory("mystruct", strStructToParse.c_str());
        ASSERT_EQ(a_util::result::SUCCESS, oFactory.isValid());
        ASSERT_EQ(oFactory.getStaticElementCount(), 0U);
    }

    {
        // with no element and forgotten alignment
        std::string strStructToParse = "<struct name=\"mystruct\" version=\"1\">";
        strStructToParse.append("</struct>");
        codec::CodecFactory oFactory("mystruct", strStructToParse.c_str());
        ASSERT_EQ(a_util::result::SUCCESS, oFactory.isValid());
        ASSERT_EQ(oFactory.getStaticElementCount(), 0U);
    }

    {
        // test if root is the struct (it will work)
        std::string strStructToParse = std::string("<struct name=\"tMyType\" version=\"1\">");
        strStructToParse.append("<element type=\"tUInt8\" name=\"value1\" bytepos=\"0\" "
                                "arraysize=\"1\" byteorder=\"LE\" alignment=\"1\"/>");
        strStructToParse.append("<element type=\"tInt64\" name=\"value2\" bytepos=\"1\" "
                                "arraysize=\"1\" byteorder=\"LE\" alignment=\"1\"/>");
        strStructToParse.append("</struct>");
        codec::CodecFactory oFactory("tMyType", strStructToParse.c_str());
        ASSERT_EQ(oFactory.getStaticElementCount(), 2U);
    }

    {
        // test if root is structs and then the struct is in it (it will work)
        std::string strStructToParse =
            std::string("<structs><struct name=\"tMyType\" version=\"1\">");
        strStructToParse.append("<element type=\"tUInt8\" name=\"value1\" bytepos=\"0\" "
                                "arraysize=\"1\" byteorder=\"LE\" alignment=\"1\"/>");
        strStructToParse.append("<element type=\"tInt64\" name=\"value2\" bytepos=\"1\" "
                                "arraysize=\"1\" byteorder=\"LE\" alignment=\"1\"/>");
        strStructToParse.append("</struct></structs>");
        codec::CodecFactory oFactory("tMyType", strStructToParse.c_str());
        ASSERT_EQ(oFactory.getStaticElementCount(), 2U);
    }

    {
        // test if root is something else is the root and then the struct is in it (it will work)
        std::string strStructToParse =
            std::string("<test><structs><struct name=\"tMyType\" version=\"1\">");
        strStructToParse.append("<element type=\"tUInt8\" name=\"value1\" bytepos=\"0\" "
                                "arraysize=\"1\" byteorder=\"LE\" alignment=\"1\"/>");
        strStructToParse.append("<element type=\"tInt64\" name=\"value2\" bytepos=\"1\" "
                                "arraysize=\"1\" byteorder=\"LE\" alignment=\"1\"/>");
        strStructToParse.append("</struct></structs></test>");
        codec::CodecFactory oFactory("tMyType", strStructToParse.c_str());
        ASSERT_EQ(oFactory.getStaticElementCount(), 2U);
    }

    {
        // test if root is something else and there is ea forgotten struct
        std::string strStructToParse = std::string("<test><struct name=\"tMyType\" version=\"1\">");
        strStructToParse.append("<element type=\"tUInt8\" name=\"value1\" bytepos=\"0\" "
                                "arraysize=\"1\" byteorder=\"LE\" alignment=\"1\"/>");
        strStructToParse.append("<element type=\"tInt64\" name=\"value2\" bytepos=\"1\" "
                                "arraysize=\"1\" byteorder=\"LE\" alignment=\"1\"/>");
        strStructToParse.append("</struct></test>");
        codec::CodecFactory oFactory("tMyType", strStructToParse.c_str());
        ASSERT_NE(a_util::result::SUCCESS, oFactory.isValid());
        ASSERT_EQ(oFactory.getStaticElementCount(), 0U);
    }
}

#pragma pack(push, 1)
// Test struct for DataDefinition version < 3.0
typedef struct {
    uint16_t firstvalue; // aligned at position 0 (1 byte aligned)
    uint8_t padding[6];
    uint8_t secondvalue; // aligned at position 8 (8 byte aligned)
} tAlignedStruct_v20;
// Test struct for DataDefinition version >= 3.0
typedef struct {
    uint16_t firstvalue; // aligned at position 0 (1 byte aligned)
    uint8_t padding1[6];
    uint8_t secondvalue; // aligned at position 8 (8 byte aligned)
    uint8_t padding2[7]; // size of struct is 16 (for 8byte alignment)
} tAlignedStruct_v30;
#pragma pack(pop)

/**
 * Template method for filling an aligned struct
 * Use this to fill tAlignedStruct_v20, tAlignedStruct_v30 and so on with number 1 and 2.
 * @return A struct filled with value 1 and 2.
 */
template <class S>
S FillAlignedStruct()
{
    S sAlignedStruct;
    a_util::memory::set(&sAlignedStruct, sizeof(sAlignedStruct), 0, sizeof(sAlignedStruct));
    sAlignedStruct.firstvalue = 1;
    sAlignedStruct.secondvalue = 2;
    return sAlignedStruct;
}

/**
 * Template method for checking a struct if it has the same content as a struct filled with
 * FillAlignedStruct(). Use this with tAlignedStruct_v20, tAlignedStruct_v30 and so on.
 * @param [in] sAlignedStruct  The struct to be checked.
 * @param [in] pReference  a pointer to the original memory block used with oCoder.Begin.
 */
template <class S>
void CheckAlignedStruct(const S* sAlignedStruct, const S* sReference)
{
    // A bit redundant checking, but better save than sorry.
    ASSERT_EQ(sAlignedStruct->firstvalue, sReference->firstvalue);
    ASSERT_EQ(sAlignedStruct->secondvalue, sReference->secondvalue);
    ASSERT_EQ(a_util::memory::compare(sAlignedStruct, sizeof(S), sReference, sizeof(S)), 0);
}

/**
 * Checks if the decoded values from the media coder are the same as filled with
 * FillAlignedStruct().
 * @param [in] pCoder  The coder to use. Has to be fully initialized.
 * @param [in] pReference  a pointer to the original memory block used with oCoder.Begin.
 * @param [in] strPrefix  a prefix to used if the the member is within an array.
 */
template <class S>
void CheckAlignedStructElementCoderAccess(codec::Codec& oCodec,
                                          const S* pReference,
                                          const char* strPrefix = "")
{
    auto ui16Value = static_cast<uint16_t>(-1);
    auto ui8Value = static_cast<uint8_t>(-1);
    ASSERT_NE(ui16Value, pReference->firstvalue);
    ASSERT_NE(ui8Value, pReference->secondvalue);

    std::string strFmt = a_util::strings::format("%sfirstvalue", strPrefix);
    ASSERT_NO_THROW(oCodec.getElement(strFmt).getRawValue((void*)&ui16Value, 0));
    ASSERT_EQ(ui16Value, pReference->firstvalue);
    strFmt = a_util::strings::format("%ssecondvalue", strPrefix);
    ASSERT_NO_THROW(oCodec.getElement(strFmt).getRawValue((void*)&ui8Value, 0));
    ASSERT_EQ(ui8Value, pReference->secondvalue);

    // Negative test: nonexisting element must throw
    ASSERT_ANY_THROW(
        oCodec.getElement(a_util::strings::format("%sbla", strPrefix)).getRawValue(&ui16Value, 0));
}

/**
 * Template method for checking if if a whole struct can be accessed in a coder.
 * Use this with tAlignedStruct_v20, tAlignedStruct_v30 and so on.
 * @param [in] pCoder  The coder to use. Has to be fully initialized.
 * @param [in] pReference  a pointer to the original memory block used with oCoder.Begin.
 */
template <class S>
void CheckAlignedStructCoderAccess(codec::Codec& oCodec, const S* pReference, bool bRefCheck)
{
    S* pValCheck = new S();
    a_util::memory::set((void*)pValCheck, sizeof(S), 0, sizeof(S));
    // access via Empty String is not possible in new Codec!
    // so we need to copy it by ourself
    ASSERT_NO_THROW(oCodec.getElement("").getRawValue(pValCheck, sizeof(S)));
    CheckAlignedStruct<S>(pValCheck, pReference);

    // Ref check
    delete pValCheck;

    pValCheck = static_cast<S*>(oCodec.getElement("").getAddress());
    ASSERT_NE(pValCheck, nullptr);
    CheckAlignedStruct(pValCheck, pReference);

    if (bRefCheck) {
        ASSERT_EQ((void*)pValCheck, (void*)pReference);
    }
}

std::string g_strAlignedStructStructToParse = "";

std::string strDDLv20Header = "<adtf:ddl xmlns:adtf=\"adtf\">"
                              "<header>"
                              "<language_version>2.00</language_version>"
                              "<author>dev_essential team</author>"
                              "<date_creation>20130703</date_creation>"
                              "<date_change />"
                              "<description>ADTF Common Description File</description>"
                              "</header>";
std::string strDDLv30Header = "<adtf:ddl xmlns:adtf=\"adtf\">"
                              "<header>"
                              "<language_version>3.00</language_version>"
                              "<author>dev_essential team</author>"
                              "<date_creation>20130703</date_creation>"
                              "<date_change />"
                              "<description>ADTF Common Description File</description>"
                              "</header>";

/**
 * @detail The Structure will be represented without error in the memory
 * This test initializes a media coder using create with dom string
 *             and access the members that are aligned
 * Passed if:  The function will resolve the right byte position to
 *             access the elements of the structure
 * @req_id ACORE-1265 ACORE-2641 ACORE-6148
 */
TEST(cTesterMediaCoder, TestAlignmentSimpleAlsoVersion30)
{
    std::string strStruct = "<structs>"
                            "<struct name=\"tAlignedStruct\" alignment=\"8\" version=\"1\">"
                            "    <element name=\"firstvalue\" type=\"tUInt16\" alignment=\"1\" "
                            "arraysize=\"1\" byteorder=\"BE\" bytepos=\"0\"/>"
                            "    <element name=\"secondvalue\" type=\"tUInt8\" alignment=\"8\" "
                            "arraysize=\"1\" byteorder=\"LE\" bytepos=\"2\"/>"
                            "</struct>"
                            "</structs>";

    std::string strDDLv20 = strDDLv20Header + strStruct + "</adtf:ddl>";
    std::string strDDLv30 = strDDLv30Header + strStruct + "</adtf:ddl>";

    codec::CodecFactory oFactory_v20("tAlignedStruct", strDDLv20.c_str());
    codec::CodecFactory oFactory_v30("tAlignedStruct", strDDLv30.c_str());

    tAlignedStruct_v20 sVal20 = FillAlignedStruct<tAlignedStruct_v20>();
    tAlignedStruct_v30 sVal30 = FillAlignedStruct<tAlignedStruct_v30>();

    codec::Codec oCodec_v20 = oFactory_v20.makeCodecFor(&sVal20, sizeof(sVal20));
    codec::Codec oCodec_v30 = oFactory_v30.makeCodecFor(&sVal30, sizeof(sVal30));

    CheckAlignedStructElementCoderAccess(oCodec_v20, &sVal20);
    CheckAlignedStructElementCoderAccess(oCodec_v30, &sVal30);

    CheckAlignedStructCoderAccess(oCodec_v20, &sVal20, true);
    CheckAlignedStructCoderAccess(oCodec_v30, &sVal30, true);
}

#pragma pack(push, 1)
// Test struct for DataDefinition version < 3.0
typedef struct {
    tAlignedStruct_v20 s0; // 9
    uint8_t padding0[7];   // 16
    tAlignedStruct_v20 s1; // 25
    uint8_t padding1[7];   // 32
    tAlignedStruct_v20 s2; // 41
    uint8_t padding2[7];   // 48
    tAlignedStruct_v20 s3; // 57
    uint8_t padding3[7];   // 64
    tAlignedStruct_v20 s4; // 73
} tAlignedStructArray_v20;
// Test struct for DataDefinition version >= 3.0
typedef struct {
    tAlignedStruct_v30 s0;
    tAlignedStruct_v30 s1;
    tAlignedStruct_v30 s2;
    tAlignedStruct_v30 s3;
    tAlignedStruct_v30 s4;
} tAlignedStructArray_v30;
#pragma pack(pop)

/**
 * Template method for filling an aligned struct array
 * Use this to fill tAlignedStructArray_v20, tAlignedStructArray_v30 and so on with values 1 to 10.
 * @return A struct filled with values 1 to 10.
 */
template <class S>
S FillAlignedStructArray()
{
    S sAlignedStructArray;
    a_util::memory::set(&sAlignedStructArray, sizeof(S), 0, sizeof(S));
    sAlignedStructArray.s0.firstvalue = 1;
    sAlignedStructArray.s0.secondvalue = 2;
    sAlignedStructArray.s1.firstvalue = 3;
    sAlignedStructArray.s1.secondvalue = 4;
    sAlignedStructArray.s2.firstvalue = 5;
    sAlignedStructArray.s2.secondvalue = 6;
    sAlignedStructArray.s3.firstvalue = 7;
    sAlignedStructArray.s3.secondvalue = 8;
    sAlignedStructArray.s4.firstvalue = 9;
    sAlignedStructArray.s4.secondvalue = 10;
    return sAlignedStructArray;
}

/**
 * Template method for checking a struct if it has the same content as a struct filled with
 * FillAlignedStruct(). Use this with tAlignedStructArray_v20, tAlignedStructArray_v30 and so on.
 * @param [in] sAlignedStructArray  The struct to be checked.
 */
template <class A, class S>
void CheckAlignedStructArray(const A* pAlignedStructArray, const A* pReference)
{
    // A bit redundant checking, but better save than sorry.
    CheckAlignedStruct<S>(&(pAlignedStructArray->s0), &(pReference->s0));
    CheckAlignedStruct<S>(&(pAlignedStructArray->s1), &(pReference->s1));
    CheckAlignedStruct<S>(&(pAlignedStructArray->s2), &(pReference->s2));
    CheckAlignedStruct<S>(&(pAlignedStructArray->s3), &(pReference->s3));
    CheckAlignedStruct<S>(&(pAlignedStructArray->s4), &(pReference->s4));
    ASSERT_EQ(a_util::memory::compare(pAlignedStructArray, sizeof(A), pReference, sizeof(A)), 0);
}

/**
 * Template method for checking if a whole struct can be accessed with a coder.
 * Use this with tAlignedStructArray_v20, tAlignedStructArray_v30 and so on.
 * @param [in] pCoder  The coder to use. Has to be fully initialized.
 * @param [in] pReference  a pointer to the original memory block used with oCoder.Begin.
 */
template <class A, class S>
void CheckAlignedStructArrayCoderAccess(codec::Codec& oCodec, const A* pReference, bool bRefCheck)
{
    ASSERT_EQ(oCodec.getBufferSize(), sizeof(A));

    A* pValCheck = new A();
    a_util::memory::set((void*)pValCheck, sizeof(A), 0, sizeof(A));
    // access via Empty String
    a_util::memory::set((void*)pValCheck, sizeof(A), 0, sizeof(A));
    ASSERT_NO_THROW(oCodec.getElement("").getRawValue(pValCheck, sizeof(A)));
    CheckAlignedStructArray<A, S>(pValCheck, pReference);

    // access via whole array element
    a_util::memory::set((void*)pValCheck, sizeof(A), 0, sizeof(A));
    ASSERT_NO_THROW(oCodec.getElement("array").getRawValue(pValCheck, sizeof(A)));
    CheckAlignedStructArray<A, S>(pValCheck, pReference);

    // Ref check
    delete pValCheck;
    pValCheck = nullptr;

    ASSERT_NO_THROW(pValCheck = static_cast<A*>(oCodec.getElement("").getAddress()));
    ASSERT_NE(pValCheck, nullptr);
    CheckAlignedStructArray<A, S>(pValCheck, pReference);
    if (bRefCheck) {
        ASSERT_EQ((void*)pValCheck, (void*)pReference);
    }
    // Ref check via first element
    pValCheck = NULL;
    ASSERT_NO_THROW(pValCheck = static_cast<A*>(oCodec.getElement("array").getAddress()));
    ASSERT_NE(pValCheck, nullptr);
    CheckAlignedStructArray<A, S>(pValCheck, pReference);
    if (bRefCheck) {
        ASSERT_EQ((void*)pValCheck, (void*)pReference);
    }
}

/**
 * Template method for checking if single elements of and array and their children can be accessed
 * with a coder. Use this with tAlignedStructArray_v20, tAlignedStructArray_v30 and so on.
 * @param [in] pCoder  The coder to use. Has to be fully initialized.
 * @param [in] pReference  a pointer to the original memory block used with oCoder.Begin.
 */

template <class A, class S>
void CheckAlignedStructArrayCoderArrayAccess(codec::Codec& oCodec, const A* pReference)
{
    S* pTestAlignedElement = new S();
    a_util::memory::set((void*)pTestAlignedElement, sizeof(S), 0, sizeof(S));
    ASSERT_NO_THROW(oCodec.getElement("array[0]").getRawValue(pTestAlignedElement, sizeof(S)));
    CheckAlignedStruct(pTestAlignedElement, &pReference->s0);

    a_util::memory::set((void*)pTestAlignedElement, sizeof(S), 0, sizeof(S));
    ASSERT_NO_THROW(oCodec.getElement("array[1]").getRawValue(pTestAlignedElement, sizeof(S)));
    CheckAlignedStruct(pTestAlignedElement, &pReference->s1);

    a_util::memory::set((void*)pTestAlignedElement, sizeof(S), 0, sizeof(S));
    ASSERT_NO_THROW(oCodec.getElement("array[2]").getRawValue(pTestAlignedElement, sizeof(S)));
    CheckAlignedStruct(pTestAlignedElement, &pReference->s2);

    a_util::memory::set((void*)pTestAlignedElement, sizeof(S), 0, sizeof(S));
    ASSERT_NO_THROW(oCodec.getElement("array[3]").getRawValue(pTestAlignedElement, sizeof(S)));
    CheckAlignedStruct(pTestAlignedElement, &pReference->s3);

    a_util::memory::set((void*)pTestAlignedElement, sizeof(S), 0, sizeof(S));
    ASSERT_NO_THROW(oCodec.getElement("array[4]").getRawValue(pTestAlignedElement, sizeof(S)));
    CheckAlignedStruct(pTestAlignedElement, &pReference->s4);

    CheckAlignedStructElementCoderAccess(oCodec, &pReference->s0, "array[0].");
    CheckAlignedStructElementCoderAccess(oCodec, &pReference->s1, "array[1].");
    CheckAlignedStructElementCoderAccess(oCodec, &pReference->s2, "array[2].");
    CheckAlignedStructElementCoderAccess(oCodec, &pReference->s3, "array[3].");
    CheckAlignedStructElementCoderAccess(oCodec, &pReference->s4, "array[4].");

    // Negative test: nonexisting array element must not be found
    ASSERT_ANY_THROW(oCodec.getElement("array[5]").getRawValue(pTestAlignedElement, sizeof(S)));
}

std::string g_strAlignedStructArrayStructToParse = "";

/**
 * @detail The Structure will be represented without error in the memory
 * This test initializes a media coder using create with dom string and
 *             access the members of an array that are aligned
 * Passed if:  The function will resolve the right byte position to access the
 *             elements of the of the structure and array
 * @req_id ACORE-1265 ACORE-2641 ACORE-6148
 */
TEST(cTesterMediaCoder, TestAlignmentArray)
{
    std::string strStruct = "<structs>"
                            "<struct name=\"tAlignedStruct\" alignment=\"8\" version=\"1\">"
                            "    <element name=\"firstvalue\" type=\"tUInt16\" alignment=\"1\" "
                            "arraysize=\"1\" byteorder=\"BE\" bytepos=\"0\"/>"
                            "    <element name=\"secondvalue\" type=\"tUInt8\" alignment=\"8\" "
                            "arraysize=\"1\" byteorder=\"LE\" bytepos=\"2\"/>"
                            "</struct>"
                            "<struct name=\"tAlignedStructArray\" alignment=\"8\" version=\"1\">"
                            "    <element name=\"array\" type=\"tAlignedStruct\" alignment=\"4\" "
                            "arraysize=\"5\" byteorder=\"LE\" bytepos=\"20\"/>"
                            "</struct>"
                            "</structs>";

    std::string strDDLv20 = strDDLv20Header + strStruct + "</adtf:ddl>";
    std::string strDDLv30 = strDDLv30Header + strStruct + "</adtf:ddl>";

    // this is the description for the above Array structure with aligned access

    tAlignedStructArray_v20 sArray_v20 = FillAlignedStructArray<tAlignedStructArray_v20>();
    tAlignedStructArray_v30 sArray_v30 = FillAlignedStructArray<tAlignedStructArray_v30>();

    codec::CodecFactory oFactory_v20("tAlignedStructArray", strDDLv20.c_str());
    codec::CodecFactory oFactory_v30("tAlignedStructArray", strDDLv30.c_str());
    codec::Codec oCodec_v20 = oFactory_v20.makeCodecFor(&sArray_v20, sizeof(sArray_v20));
    codec::Codec oCodec_v30 = oFactory_v30.makeCodecFor(&sArray_v30, sizeof(sArray_v30));

    CheckAlignedStructArrayCoderAccess<tAlignedStructArray_v20, tAlignedStruct_v20>(
        oCodec_v20, &sArray_v20, true);
    CheckAlignedStructArrayCoderAccess<tAlignedStructArray_v30, tAlignedStruct_v30>(
        oCodec_v30, &sArray_v30, true);

    CheckAlignedStructArrayCoderArrayAccess<tAlignedStructArray_v20, tAlignedStruct_v20>(
        oCodec_v20, &sArray_v20);
    CheckAlignedStructArrayCoderArrayAccess<tAlignedStructArray_v30, tAlignedStruct_v30>(
        oCodec_v30, &sArray_v30);
}

#pragma pack(push, 1)
typedef struct {
    uint16_t firstvalue;
    uint8_t secondvalue;
} tSerializedStruct;
typedef struct {
    uint8_t gap[20];
    tSerializedStruct sValues[5];
} tSerializedArray;
#pragma pack(pop)

/**
 * @detail The Structure will be serialized and deserialized with the description
 * This test initializes a media coder using create with dom string and serializes
 *             and deserializes the content given. Test if the values are equal after
 *             deserialization.
 * Passed if:  The function will set the right byte and bit position in the stream given.
 * @req_id ACORE-1265 ACORE-2641 ACORE-6148
 */
TEST(cTesterMediaCoder, TestCoderSerializer)
{
    std::string strStruct = "<structs>"
                            "<struct name=\"tAlignedStruct\" alignment=\"8\" version=\"1\">"
                            "    <element name=\"firstvalue\" type=\"tUInt16\" alignment=\"1\" "
                            "arraysize=\"1\" byteorder=\"BE\" bytepos=\"0\"/>"
                            "    <element name=\"secondvalue\" type=\"tUInt8\" alignment=\"8\" "
                            "arraysize=\"1\" byteorder=\"LE\" bytepos=\"2\"/>"
                            "</struct>"
                            "<struct name=\"tAlignedStructArray\" alignment=\"8\" version=\"1\">"
                            "    <element name=\"array\" type=\"tAlignedStruct\" alignment=\"4\" "
                            "arraysize=\"5\" byteorder=\"LE\" bytepos=\"20\"/>"
                            "</struct>"
                            "</structs>";

    std::string strDDLv20 = strDDLv20Header + strStruct + "</adtf:ddl>";
    std::string strDDLv30 = strDDLv30Header + strStruct + "</adtf:ddl>";

    // this is the description for the above Array structure with aligned access

    tAlignedStructArray_v20 sArray_v20 = FillAlignedStructArray<tAlignedStructArray_v20>();
    tAlignedStructArray_v30 sArray_v30 = FillAlignedStructArray<tAlignedStructArray_v30>();

    codec::CodecFactory oFactory_v20("tAlignedStructArray", strDDLv20.c_str());
    codec::CodecFactory oFactory_v30("tAlignedStructArray", strDDLv30.c_str());
    codec::Codec oCodec_v20 = oFactory_v20.makeCodecFor(&sArray_v20, sizeof(sArray_v20));
    codec::Codec oCodec_v30 = oFactory_v30.makeCodecFor(&sArray_v30, sizeof(sArray_v30));

    a_util::memory::MemoryBuffer oBuffer_v20;
    a_util::memory::MemoryBuffer oBuffer_v30;

    ASSERT_EQ(a_util::result::SUCCESS, codec::transformToBuffer(oCodec_v20, oBuffer_v20, true));
    ASSERT_EQ(a_util::result::SUCCESS, codec::transformToBuffer(oCodec_v30, oBuffer_v30, true));

    tSerializedArray* pSerArray_v20 = (tSerializedArray*)oBuffer_v20.getPtr();
    tSerializedArray* pSerArray_v30 = (tSerializedArray*)oBuffer_v30.getPtr();

    // Check if serialization has the same result with both versions.
    ASSERT_EQ(a_util::memory::compare(
                  pSerArray_v20, sizeof(tSerializedArray), pSerArray_v30, sizeof(tSerializedArray)),
              0);
    // Since both memory blocks are equal, only one has to be verified.
    ASSERT_EQ(pSerArray_v20->sValues[0].secondvalue, 2);
    ASSERT_EQ(pSerArray_v20->sValues[1].secondvalue, 4);
    ASSERT_EQ(pSerArray_v20->sValues[2].secondvalue, 6);
    ASSERT_EQ(pSerArray_v20->sValues[3].secondvalue, 8);
    ASSERT_EQ(pSerArray_v20->sValues[4].secondvalue, 10);

    if (PLATFORM_BYTEORDER == ddl::dd::ByteOrder::plattform_little_endian_8) {
        ASSERT_EQ(pSerArray_v20->sValues[0].firstvalue, 256);
        ASSERT_EQ(pSerArray_v20->sValues[1].firstvalue, 768);
        ASSERT_EQ(pSerArray_v20->sValues[2].firstvalue, 1280);
        ASSERT_EQ(pSerArray_v20->sValues[3].firstvalue, 1792);
        ASSERT_EQ(pSerArray_v20->sValues[4].firstvalue, 2304);
    }
    else {
        ASSERT_EQ(pSerArray_v20->sValues[0].firstvalue, 1);
        ASSERT_EQ(pSerArray_v20->sValues[1].firstvalue, 3);
        ASSERT_EQ(pSerArray_v20->sValues[2].firstvalue, 5);
        ASSERT_EQ(pSerArray_v20->sValues[3].firstvalue, 7);
        ASSERT_EQ(pSerArray_v20->sValues[4].firstvalue, 9);
    }

    codec::Decoder oSerDecoder_v20 =
        oFactory_v20.makeDecoderFor(oBuffer_v20.getPtr(), oBuffer_v20.getSize(), serialized);
    codec::Decoder oSerDecoder_v30 =
        oFactory_v30.makeDecoderFor(oBuffer_v30.getPtr(), oBuffer_v30.getSize(), serialized);

    tAlignedStructArray_v20 sArrayget_v20;
    tAlignedStructArray_v30 sArrayget_v30;
    a_util::memory::set(&sArrayget_v20, sizeof(sArrayget_v20), 0, sizeof(sArrayget_v20));
    a_util::memory::set(&sArrayget_v30, sizeof(sArrayget_v30), 0, sizeof(sArrayget_v30));
    oCodec_v20 = oSerDecoder_v20.makeCodecFor(&sArrayget_v20, sizeof(sArrayget_v20), deserialized);
    oCodec_v30 = oSerDecoder_v30.makeCodecFor(&sArrayget_v30, sizeof(sArrayget_v30), deserialized);

    ASSERT_EQ(a_util::result::SUCCESS, codec::transform(oSerDecoder_v20, oCodec_v20));
    ASSERT_EQ(a_util::result::SUCCESS, codec::transform(oSerDecoder_v30, oCodec_v30));

    // test if the content is still the same
    CheckAlignedStructArrayCoderAccess<tAlignedStructArray_v20, tAlignedStruct_v20>(
        oCodec_v20, &sArray_v20, false);
    CheckAlignedStructArrayCoderAccess<tAlignedStructArray_v30, tAlignedStruct_v30>(
        oCodec_v30, &sArray_v30, false);

    // test if accessing of the whole struct still works
    CheckAlignedStructArrayCoderAccess<tAlignedStructArray_v20, tAlignedStruct_v20>(
        oCodec_v20, &sArrayget_v20, true);
    CheckAlignedStructArrayCoderAccess<tAlignedStructArray_v30, tAlignedStruct_v30>(
        oCodec_v30, &sArrayget_v30, true);

    // test if array access still works
    CheckAlignedStructArrayCoderArrayAccess<tAlignedStructArray_v20, tAlignedStruct_v20>(
        oCodec_v20, &sArrayget_v20);
    CheckAlignedStructArrayCoderArrayAccess<tAlignedStructArray_v30, tAlignedStruct_v30>(
        oCodec_v30, &sArrayget_v30);
}

/**
 * @detail The CAN Message will be serialized right.
 * This test initializes a media coder using the definition of ADTF CAN Message
 * Passed if:  The function will resolve the right byte position to access the
 *              elements of the of the CAN structure
 * @req_id ACORE-1265 ACORE-2641
 */
TEST(cTesterMediaCoder, TestCoderSerializerCanMessage)
{
    tCanMessage sMessageToCompare;
    tCanMessage sMessageToset;

    codec::CodecFactory oFactory(ADTF_MEDIA_DESC_CANMESSAGE,
                                 ADTF_MEDIA_DESC_CANMESSAGE_DESCRIPTION);

    for (std::uint8_t nIdx = 1; nIdx < 200; nIdx++) {
        a_util::memory::set(&sMessageToset, sizeof(sMessageToset), 0, sizeof(sMessageToset));
        a_util::memory::set(
            &sMessageToCompare, sizeof(sMessageToCompare), 0, sizeof(sMessageToCompare));
        codec::Codec oCodec = oFactory.makeCodecFor(&sMessageToset, sizeof(sMessageToset));

        sMessageToCompare.ui16Id = nIdx;
        ASSERT_NO_THROW(oCodec.getElement("ui16Id").setRawValue(&sMessageToCompare.ui16Id, 0));

        sMessageToCompare.ui8Channel = nIdx % 5;
        ASSERT_NO_THROW(
            oCodec.getElement("ui8Channel").setRawValue(&sMessageToCompare.ui8Channel, 0));

        sMessageToCompare.ui8Length = nIdx % 7 + 1;
        ASSERT_NO_THROW(
            oCodec.getElement("ui8Length").setRawValue(&sMessageToCompare.ui8Length, 0));

        for (std::uint8_t nPayload = 0; nPayload < sMessageToCompare.ui8Length; nPayload++) {
            ASSERT_LT(nPayload, 8);
            sMessageToCompare.pui8Data[nPayload] = nPayload;
            ASSERT_NO_THROW(oCodec.getElement(a_util::strings::format("pui8Data[%d]", nPayload))
                                .setRawValue(&sMessageToCompare.pui8Data[nPayload], 0));
        }
        ASSERT_EQ(
            a_util::memory::compare(
                &sMessageToset, sizeof(sMessageToset), &sMessageToCompare, sizeof(sMessageToset)),
            0);

        a_util::memory::MemoryBuffer oWriteBuffer;
        ASSERT_EQ(a_util::result::SUCCESS, codec::transformToBuffer(oCodec, oWriteBuffer));

        if (PLATFORM_BYTEORDER == ddl::dd::ByteOrder::plattform_little_endian_8) {
            const tCanMessage* pCanMessageSerialized = (const tCanMessage*)oWriteBuffer.getPtr();
            ASSERT_EQ(a_util::memory::compare(pCanMessageSerialized,
                                              sizeof(sMessageToCompare),
                                              &sMessageToCompare,
                                              sizeof(sMessageToCompare)),
                      0);
        }
        else {
            const tCanMessage* pCanMessageSerialized = (const tCanMessage*)oWriteBuffer.getPtr();
            ASSERT_NE(a_util::memory::compare(pCanMessageSerialized,
                                              sizeof(sMessageToCompare),
                                              &sMessageToCompare,
                                              sizeof(sMessageToCompare)),
                      0);
            ASSERT_EQ(pCanMessageSerialized->ui8Channel, sMessageToCompare.ui8Channel);
            ASSERT_EQ(pCanMessageSerialized->ui8Length, sMessageToCompare.ui8Length);
            ASSERT_EQ(a_util::memory::compare(
                          pCanMessageSerialized->pui8Data, 8, sMessageToCompare.pui8Data, 8),
                      0);
            uint16_t ui16Id = pCanMessageSerialized->ui16Id;
            ui16Id = a_util::memory::swapEndianess(ui16Id);
            ASSERT_EQ(sMessageToCompare.ui16Id, ui16Id);
        }

        codec::Codec oSerCodec =
            oFactory.makeCodecFor(oWriteBuffer.getPtr(), oWriteBuffer.getSize(), serialized);
        a_util::memory::MemoryBuffer oReadBuffer;
        ASSERT_EQ(a_util::result::SUCCESS, codec::transformToBuffer(oSerCodec, oReadBuffer));
        ASSERT_EQ(a_util::memory::compare(oReadBuffer.getPtr(),
                                          sizeof(sMessageToCompare),
                                          &sMessageToCompare,
                                          sizeof(sMessageToCompare)),
                  0);
    }
}

/**
 * @detail Media Coder Serialization for one bit values.
 * This test serializes bit described values and check the serialized data buffer.
 * @req_id ACORE-1265 ACORE-2641
 */
TEST(cTesterMediaCoder, TestCoderForEverySerializationPathOneBit)
{
    typedef struct {
        uint8_t ui8Value[10];
    } tSerializedStructure10;

    std::string strStructure = "<struct type=\"tTest\" alignment=\"8\" version=\"1\">";
    strStructure.append(
        "<element name=\"OneBitOnByteBorderInBeginning\" arraysize=\"1\" type=\"tUInt16\" "
        "alignment=\"4\" bytepos=\"2\" bitpos=\"0\" numbits=\"1\" byteorder=\"LE\" />");
    strStructure.append(
        "<element name=\"OneBitNotOnByteBorderInBeginning1\" arraysize=\"1\" type=\"tUInt8\" "
        "alignment=\"2\" bytepos=\"3\" bitpos=\"1\" numbits=\"1\" byteorder=\"LE\" />");
    strStructure.append(
        "<element name=\"OneBitNotOnByteBorderInBeginning2\" arraysize=\"1\" type=\"tUInt8\" "
        "alignment=\"2\" bytepos=\"4\" bitpos=\"2\" numbits=\"1\" byteorder=\"LE\" />");
    strStructure.append(
        "<element name=\"OneBitNotOnByteBorderInBeginning3\" arraysize=\"1\" type=\"tUInt8\" "
        "alignment=\"2\" bytepos=\"5\" bitpos=\"3\" numbits=\"1\" byteorder=\"LE\" />");
    strStructure.append(
        "<element name=\"OneBitNotOnByteBorderInBeginning4\" arraysize=\"1\" type=\"tUInt8\" "
        "alignment=\"1\" bytepos=\"6\" bitpos=\"4\" numbits=\"1\" byteorder=\"LE\" />");
    strStructure.append(
        "<element name=\"OneBitNotOnByteBorderInBeginning5\" arraysize=\"1\" type=\"tUInt8\" "
        "alignment=\"2\" bytepos=\"7\" bitpos=\"5\" numbits=\"1\" byteorder=\"LE\" />");
    strStructure.append(
        "<element name=\"OneBitNotOnByteBorderInBeginning6\" arraysize=\"1\" type=\"tUInt8\" "
        "alignment=\"8\" bytepos=\"8\" bitpos=\"6\" numbits=\"1\" byteorder=\"LE\" />");
    strStructure.append(
        "<element name=\"OneBitNotOnByteBorderInBeginning7\" arraysize=\"1\" type=\"tUInt8\" "
        "alignment=\"2\" bytepos=\"9\" bitpos=\"7\" numbits=\"1\" byteorder=\"LE\" />");
    strStructure.append("</struct>");

    codec::CodecFactory oFactory("tTest", strStructure.c_str());
    size_t nSize = oFactory.getStaticBufferSize();
    a_util::memory::MemoryBuffer oBuffer(nSize);
    codec::Codec oCodec = oFactory.makeCodecFor(oBuffer.getPtr(), oBuffer.getSize());
    uint16_t ui16Value = 1; // one bit used
    uint8_t ui8Value = 1;   // one bit used
    ASSERT_NO_THROW(oCodec.getElement("OneBitOnByteBorderInBeginning").setRawValue(&ui16Value, 0));
    ASSERT_NO_THROW(
        oCodec.getElement("OneBitNotOnByteBorderInBeginning1").setRawValue(&ui8Value, 0));
    ASSERT_NO_THROW(
        oCodec.getElement("OneBitNotOnByteBorderInBeginning2").setRawValue(&ui8Value, 0));
    ASSERT_NO_THROW(
        oCodec.getElement("OneBitNotOnByteBorderInBeginning3").setRawValue(&ui8Value, 0));
    ASSERT_NO_THROW(
        oCodec.getElement("OneBitNotOnByteBorderInBeginning4").setRawValue(&ui8Value, 0));
    ASSERT_NO_THROW(
        oCodec.getElement("OneBitNotOnByteBorderInBeginning5").setRawValue(&ui8Value, 0));
    ASSERT_NO_THROW(
        oCodec.getElement("OneBitNotOnByteBorderInBeginning6").setRawValue(&ui8Value, 0));
    ASSERT_NO_THROW(
        oCodec.getElement("OneBitNotOnByteBorderInBeginning7").setRawValue(&ui8Value, 0));

    a_util::memory::MemoryBuffer oWriteBuffer;
    ASSERT_EQ(a_util::result::SUCCESS, codec::transformToBuffer(oCodec, oWriteBuffer, true));

    const tSerializedStructure10* psSerStructure =
        (const tSerializedStructure10*)oWriteBuffer.getPtr();

    ASSERT_EQ(psSerStructure->ui8Value[0], 0);
    ASSERT_EQ(psSerStructure->ui8Value[1], 0);
    ASSERT_EQ(psSerStructure->ui8Value[2], 1);
    ASSERT_EQ(psSerStructure->ui8Value[3], 2);
    ASSERT_EQ(psSerStructure->ui8Value[4], 4);
    ASSERT_EQ(psSerStructure->ui8Value[5], 8);
    ASSERT_EQ(psSerStructure->ui8Value[6], 16);
    ASSERT_EQ(psSerStructure->ui8Value[7], 32);
    ASSERT_EQ(psSerStructure->ui8Value[8], 64);
    ASSERT_EQ(psSerStructure->ui8Value[9], 128);
}

/**
 * @detail Media Coder Serialization for two bit values.
 * This test serializes bits as described and check the serialized data buffer.
 * @req_id ACORE-1265 ACORE-2641
 */
TEST(cTesterMediaCoder, TestCoderForEverySerializationPathTwoBits)
{
    typedef struct {
        uint8_t ui8Value[11];
    } tSerializedStructure11;

    std::string strStructure = "<struct type=\"tTest\" alignment=\"8\" version=\"2\">";
    strStructure.append(
        "<element name=\"TwoBitOnByteBorderInBeginning\" arraysize=\"1\" type=\"tUInt16\" "
        "alignment=\"4\" bytepos=\"2\" bitpos=\"0\" numbits=\"2\" byteorder=\"LE\" />");
    strStructure.append(
        "<element name=\"TwoBitNotOnByteBorderInBeginning3\" arraysize=\"1\" type=\"tUInt8\" "
        "alignment=\"2\" bytepos=\"3\" bitpos=\"1\" numbits=\"2\" byteorder=\"LE\" />");
    strStructure.append(
        "<element name=\"TwoBitNotOnByteBorderInBeginning4\" arraysize=\"1\" type=\"tUInt8\" "
        "alignment=\"2\" bytepos=\"4\" bitpos=\"2\" numbits=\"2\" byteorder=\"LE\" />");
    strStructure.append(
        "<element name=\"TwoBitNotOnByteBorderInBeginning5\" arraysize=\"1\" type=\"tUInt8\" "
        "alignment=\"2\" bytepos=\"5\" bitpos=\"3\" numbits=\"2\" byteorder=\"LE\" />");
    strStructure.append(
        "<element name=\"TwoBitNotOnByteBorderInBeginning6\" arraysize=\"1\" type=\"tUInt8\" "
        "alignment=\"1\" bytepos=\"6\" bitpos=\"4\" numbits=\"2\" byteorder=\"LE\" />");
    strStructure.append(
        "<element name=\"TwoBitNotOnByteBorderInBeginning7\" arraysize=\"1\" type=\"tUInt8\" "
        "alignment=\"2\" bytepos=\"7\" bitpos=\"5\" numbits=\"2\" byteorder=\"LE\" />");
    strStructure.append(
        "<element name=\"TwoBitNotOnByteBorderInBeginning8\" arraysize=\"1\" type=\"tUInt8\" "
        "alignment=\"8\" bytepos=\"8\" bitpos=\"6\" numbits=\"2\" byteorder=\"LE\" />");
    strStructure.append(
        "<element name=\"TwoBitNotOnByteBorderInBeginning9\" arraysize=\"1\" type=\"tUInt8\" "
        "alignment=\"2\" bytepos=\"9\" bitpos=\"7\" numbits=\"2\" byteorder=\"LE\" />");
    strStructure.append("</struct>");

    codec::CodecFactory oFactory("tTest", strStructure.c_str());
    size_t nSize = oFactory.getStaticBufferSize();
    a_util::memory::MemoryBuffer oBuffer(nSize);
    codec::Codec oCodec = oFactory.makeCodecFor(oBuffer.getPtr(), oBuffer.getSize());
    uint16_t ui16Value = 1; // one bit used
    uint8_t ui8Value = 1;   // one bit used
    ASSERT_NO_THROW(oCodec.getElement("TwoBitOnByteBorderInBeginning").setRawValue(&ui16Value, 0));
    ASSERT_NO_THROW(
        oCodec.getElement("TwoBitNotOnByteBorderInBeginning3").setRawValue(&ui8Value, 0));
    ASSERT_NO_THROW(
        oCodec.getElement("TwoBitNotOnByteBorderInBeginning4").setRawValue(&ui8Value, 0));
    ASSERT_NO_THROW(
        oCodec.getElement("TwoBitNotOnByteBorderInBeginning5").setRawValue(&ui8Value, 0));
    ASSERT_NO_THROW(
        oCodec.getElement("TwoBitNotOnByteBorderInBeginning6").setRawValue(&ui8Value, 0));
    ASSERT_NO_THROW(
        oCodec.getElement("TwoBitNotOnByteBorderInBeginning7").setRawValue(&ui8Value, 0));
    ASSERT_NO_THROW(
        oCodec.getElement("TwoBitNotOnByteBorderInBeginning8").setRawValue(&ui8Value, 0));
    ASSERT_NO_THROW(
        oCodec.getElement("TwoBitNotOnByteBorderInBeginning9").setRawValue(&ui8Value, 0));

    a_util::memory::MemoryBuffer oWriteBuffer;
    ASSERT_EQ(a_util::result::SUCCESS, codec::transformToBuffer(oCodec, oWriteBuffer, true));

    const tSerializedStructure11* psSerStructure =
        (const tSerializedStructure11*)oWriteBuffer.getPtr();

    ASSERT_EQ(psSerStructure->ui8Value[0], 0);
    ASSERT_EQ(psSerStructure->ui8Value[1], 0);
    ASSERT_EQ(psSerStructure->ui8Value[2], 1);
    ASSERT_EQ(psSerStructure->ui8Value[3], 2);
    ASSERT_EQ(psSerStructure->ui8Value[4], 4);
    ASSERT_EQ(psSerStructure->ui8Value[5], 8);
    ASSERT_EQ(psSerStructure->ui8Value[6], 16);
    ASSERT_EQ(psSerStructure->ui8Value[7], 32);
    ASSERT_EQ(psSerStructure->ui8Value[8], 64);
    ASSERT_EQ(psSerStructure->ui8Value[9], 128);
    ASSERT_EQ(psSerStructure->ui8Value[10], 0);

    // test two bits in it
    ui16Value = 3; // two bit used
    ui8Value = 3;  // two bit used
    ASSERT_NO_THROW(oCodec.getElement("TwoBitOnByteBorderInBeginning").setRawValue(&ui16Value, 0));
    ASSERT_NO_THROW(
        oCodec.getElement("TwoBitNotOnByteBorderInBeginning3").setRawValue(&ui8Value, 0));
    ASSERT_NO_THROW(
        oCodec.getElement("TwoBitNotOnByteBorderInBeginning4").setRawValue(&ui8Value, 0));
    ASSERT_NO_THROW(
        oCodec.getElement("TwoBitNotOnByteBorderInBeginning5").setRawValue(&ui8Value, 0));
    ASSERT_NO_THROW(
        oCodec.getElement("TwoBitNotOnByteBorderInBeginning6").setRawValue(&ui8Value, 0));
    ASSERT_NO_THROW(
        oCodec.getElement("TwoBitNotOnByteBorderInBeginning7").setRawValue(&ui8Value, 0));
    ASSERT_NO_THROW(
        oCodec.getElement("TwoBitNotOnByteBorderInBeginning8").setRawValue(&ui8Value, 0));
    ASSERT_NO_THROW(
        oCodec.getElement("TwoBitNotOnByteBorderInBeginning9").setRawValue(&ui8Value, 0));

    ASSERT_EQ(a_util::result::SUCCESS, codec::transformToBuffer(oCodec, oWriteBuffer, true));

    psSerStructure = (const tSerializedStructure11*)oWriteBuffer.getPtr();

    ASSERT_EQ(psSerStructure->ui8Value[0], 0);
    ASSERT_EQ(psSerStructure->ui8Value[1], 0);
    ASSERT_EQ(psSerStructure->ui8Value[2], 3);
    ASSERT_EQ(psSerStructure->ui8Value[3], 6);
    ASSERT_EQ(psSerStructure->ui8Value[4], 12);
    ASSERT_EQ(psSerStructure->ui8Value[5], 24);
    ASSERT_EQ(psSerStructure->ui8Value[6], 48);
    ASSERT_EQ(psSerStructure->ui8Value[7], 96);
    ASSERT_EQ(psSerStructure->ui8Value[8], 192);
    ASSERT_EQ(psSerStructure->ui8Value[9], 128);
    ASSERT_EQ(psSerStructure->ui8Value[10], 1);

    // test two bits in it while all bits are set in memory, but only relvant for serialization will
    // be copied to the data buffer
    ui16Value = 0xFFFF; // all bits used
    ui8Value = 0xFF;    // all bits used

    ASSERT_NO_THROW(oCodec.getElement("TwoBitOnByteBorderInBeginning").setRawValue(&ui16Value, 0));

    ASSERT_NO_THROW(
        oCodec.getElement("TwoBitNotOnByteBorderInBeginning3").setRawValue(&ui8Value, 0));

    ASSERT_NO_THROW(
        oCodec.getElement("TwoBitNotOnByteBorderInBeginning4").setRawValue(&ui8Value, 0));

    ASSERT_NO_THROW(
        oCodec.getElement("TwoBitNotOnByteBorderInBeginning5").setRawValue(&ui8Value, 0));

    ASSERT_NO_THROW(
        oCodec.getElement("TwoBitNotOnByteBorderInBeginning6").setRawValue(&ui8Value, 0));

    ASSERT_NO_THROW(
        oCodec.getElement("TwoBitNotOnByteBorderInBeginning7").setRawValue(&ui8Value, 0));

    ASSERT_NO_THROW(
        oCodec.getElement("TwoBitNotOnByteBorderInBeginning8").setRawValue(&ui8Value, 0));

    ASSERT_NO_THROW(
        oCodec.getElement("TwoBitNotOnByteBorderInBeginning9").setRawValue(&ui8Value, 0));

    ASSERT_EQ(a_util::result::SUCCESS, codec::transformToBuffer(oCodec, oWriteBuffer, true));

    psSerStructure = (const tSerializedStructure11*)oWriteBuffer.getPtr();

    ASSERT_EQ(psSerStructure->ui8Value[0], 0);
    ASSERT_EQ(psSerStructure->ui8Value[1], 0);
    ASSERT_EQ(psSerStructure->ui8Value[2], 3);
    ASSERT_EQ(psSerStructure->ui8Value[3], 6);
    ASSERT_EQ(psSerStructure->ui8Value[4], 12);
    ASSERT_EQ(psSerStructure->ui8Value[5], 24);
    ASSERT_EQ(psSerStructure->ui8Value[6], 48);
    ASSERT_EQ(psSerStructure->ui8Value[7], 96);
    ASSERT_EQ(psSerStructure->ui8Value[8], 192);
    ASSERT_EQ(psSerStructure->ui8Value[9], 128);
    ASSERT_EQ(psSerStructure->ui8Value[10], 1);

    // test two bits in it
    // while sometimes only 1 bit is set
    // to the data buffer

    uint8_t ui8NotAllset01 = 0xFD; // all bits used
    uint8_t ui8NotAllset00 = 0xFC; // all bits used
    uint8_t ui8NotAllset10 = 0xFE; // all bits used
    ASSERT_NO_THROW(oCodec.getElement("TwoBitOnByteBorderInBeginning").setRawValue(&ui16Value, 0));
    ASSERT_NO_THROW(
        oCodec.getElement("TwoBitNotOnByteBorderInBeginning3").setRawValue(&ui8NotAllset01, 0));
    ASSERT_NO_THROW(
        oCodec.getElement("TwoBitNotOnByteBorderInBeginning4").setRawValue(&ui8NotAllset00, 0));
    ASSERT_NO_THROW(
        oCodec.getElement("TwoBitNotOnByteBorderInBeginning5").setRawValue(&ui8NotAllset10, 0));
    ASSERT_NO_THROW(
        oCodec.getElement("TwoBitNotOnByteBorderInBeginning6").setRawValue(&ui8NotAllset01, 0));
    ASSERT_NO_THROW(
        oCodec.getElement("TwoBitNotOnByteBorderInBeginning7").setRawValue(&ui8NotAllset00, 0));
    ASSERT_NO_THROW(
        oCodec.getElement("TwoBitNotOnByteBorderInBeginning8").setRawValue(&ui8NotAllset10, 0));
    ASSERT_NO_THROW(
        oCodec.getElement("TwoBitNotOnByteBorderInBeginning9").setRawValue(&ui8NotAllset10, 0));

    ASSERT_EQ(a_util::result::SUCCESS, codec::transformToBuffer(oCodec, oWriteBuffer, true));
    psSerStructure = (const tSerializedStructure11*)oWriteBuffer.getPtr();

    ASSERT_EQ(psSerStructure->ui8Value[0], 0);
    ASSERT_EQ(psSerStructure->ui8Value[1], 0);
    ASSERT_EQ(psSerStructure->ui8Value[2], 3);
    ASSERT_EQ(psSerStructure->ui8Value[3], 2);
    ASSERT_EQ(psSerStructure->ui8Value[4], 0);
    ASSERT_EQ(psSerStructure->ui8Value[5], 16);
    ASSERT_EQ(psSerStructure->ui8Value[6], 16);
    ASSERT_EQ(psSerStructure->ui8Value[7], 0);
    ASSERT_EQ(psSerStructure->ui8Value[8], 128);
    ASSERT_EQ(psSerStructure->ui8Value[9], 0);
    ASSERT_EQ(psSerStructure->ui8Value[10], 1);

    // pSerializer = NULL;
}

/**
 * @detail Media Coder Serialization for bit values without byte gaps.
 * This test serializes bits as described and check the serialized data buffer.
 * @req_id ACORE-1265 ACORE-2641
 */
TEST(cTesterMediaCoder, TestCoderForEverySerializationPathDifferentPositionTogether)
{
    typedef struct {
        uint8_t ui8Value[9];
    } tSerializedStructure9;

    std::string strStructure = "<struct type=\"tTest\" alignment=\"8\" version=\"2\">";
    strStructure.append(
        "<element name=\"value10bits1\"  arraysize=\"1\" type=\"tUInt16\" alignment=\"4\" "
        "bytepos=\"1\" bitpos=\"0\" numbits=\"1\" byteorder=\"LE\" />");
    strStructure.append(
        "<element name=\"value11bits3\"  arraysize=\"1\" type=\"tUInt32\" alignment=\"2\" "
        "bytepos=\"1\" bitpos=\"1\" numbits=\"3\" byteorder=\"LE\" />");
    strStructure.append(
        "<element name=\"value14bits25\" arraysize=\"1\" type=\"tUInt32\" alignment=\"2\" "
        "bytepos=\"1\" bitpos=\"4\" numbits=\"25\" byteorder=\"LE\" />");
    strStructure.append(
        "<element name=\"value45bits32\" arraysize=\"1\" type=\"tUInt32\" alignment=\"2\" "
        "bytepos=\"4\" bitpos=\"5\" numbits=\"32\" byteorder=\"LE\" />");
    strStructure.append(
        "<element name=\"value85bits3\"  arraysize=\"1\" type=\"tUInt8\"  alignment=\"4\" "
        "bytepos=\"8\" bitpos=\"5\" numbits=\"3\" byteorder=\"LE\" />");
    strStructure.append("</struct>");

    codec::CodecFactory oFactory("tTest", strStructure.c_str());
    a_util::memory::MemoryBuffer oBuffer(oFactory.getStaticBufferSize());
    codec::Codec oCodec = oFactory.makeCodecFor(oBuffer.getPtr(), oBuffer.getSize());

    uint16_t ui16Value = 0x01;
    ASSERT_NO_THROW(oCodec.getElement("value10bits1").setRawValue(&ui16Value, 0));
    uint32_t ui32Value = 0x00;
    ASSERT_NO_THROW(oCodec.getElement("value11bits3").setRawValue(&ui32Value, 0));
    ui32Value = 0x01FFFFFF;
    ASSERT_NO_THROW(oCodec.getElement("value14bits25").setRawValue(&ui32Value, 0));
    ui32Value = 0x00;
    ASSERT_NO_THROW(oCodec.getElement("value45bits32").setRawValue(&ui32Value, 0));
    uint8_t ui8Value = 0x07;
    ASSERT_NO_THROW(oCodec.getElement("value85bits3").setRawValue(&ui8Value, 0));

    a_util::memory::MemoryBuffer oWriteBuffer;
    ASSERT_EQ(a_util::result::SUCCESS, codec::transformToBuffer(oCodec, oWriteBuffer, true));

    const tSerializedStructure9* psSerStructure =
        (const tSerializedStructure9*)oWriteBuffer.getPtr();

    ASSERT_EQ(psSerStructure->ui8Value[0], 0);
    ASSERT_EQ(psSerStructure->ui8Value[1], 0xF1);
    ASSERT_EQ(psSerStructure->ui8Value[2], 0xFF);
    ASSERT_EQ(psSerStructure->ui8Value[3], 0xFF);
    ASSERT_EQ(psSerStructure->ui8Value[4], 0x1F);
    ASSERT_EQ(psSerStructure->ui8Value[5], 0x00);
    ASSERT_EQ(psSerStructure->ui8Value[6], 0x00);
    ASSERT_EQ(psSerStructure->ui8Value[7], 0x00);
    ASSERT_EQ(psSerStructure->ui8Value[8], 0xE0);

    // deserialize into oBuffer
    codec::Codec oSerCodec =
        oFactory.makeCodecFor(oWriteBuffer.getPtr(), oWriteBuffer.getSize(), serialized);
    ASSERT_EQ(a_util::result::SUCCESS, codec::transformToBuffer(oSerCodec, oBuffer, true));
    oCodec = oFactory.makeCodecFor(oBuffer.getPtr(), oBuffer.getSize());

    ui16Value = 0;
    ASSERT_NO_THROW(oCodec.getElement("value10bits1").getRawValue(&ui16Value, 0));
    ASSERT_EQ(ui16Value, 0x01);
    ui32Value = 0x01;
    ASSERT_NO_THROW(oCodec.getElement("value11bits3").getRawValue(&ui32Value, 0));
    ASSERT_EQ(ui32Value, 0x00u);
    ui32Value = 0x00;
    ASSERT_NO_THROW(oCodec.getElement("value14bits25").getRawValue(&ui32Value, 0));
    ASSERT_EQ(ui32Value, 0x01FFFFFFu);
    ui32Value = 0xFFFF;
    ASSERT_NO_THROW(oCodec.getElement("value45bits32").getRawValue(&ui32Value, 0));
    ASSERT_EQ(ui32Value, 0x00u);
    ASSERT_NO_THROW(oCodec.getElement("value85bits3").getRawValue(&ui8Value, 0));
    ASSERT_EQ(ui8Value, uint8_t{0x07});
}

/**
 * @detail Media Coder Serialization for bit values without byte gaps.
 * This test serializes bits as described and check the serialized data buffer.
 * @req_id ACORE-1265 ACORE-2641
 */
TEST(cTesterMediaCoder, TestCoderForEverySerializationPathDifferentPositionTogether2)
{
    typedef struct {
        uint8_t ui8Value[9];
    } tSerializedStructure9;

    std::string strStructure = "<struct type=\"tTest\" alignment=\"8\" version=\"2\">";
    strStructure.append(
        "<element name=\"value10bits1\"  arraysize=\"1\" type=\"tUInt16\" alignment=\"4\" "
        "bytepos=\"1\" bitpos=\"0\" numbits=\"1\" byteorder=\"LE\" />");
    strStructure.append(
        "<element name=\"value11bits3\"  arraysize=\"1\" type=\"tUInt32\" alignment=\"2\" "
        "bytepos=\"1\" bitpos=\"1\" numbits=\"3\" byteorder=\"LE\" />");
    strStructure.append(
        "<element name=\"value14bits25\" arraysize=\"1\" type=\"tUInt32\" alignment=\"2\" "
        "bytepos=\"1\" bitpos=\"4\" numbits=\"25\" byteorder=\"LE\" />");
    strStructure.append(
        "<element name=\"value45bits32\" arraysize=\"1\" type=\"tUInt32\" alignment=\"2\" "
        "bytepos=\"4\" bitpos=\"5\" numbits=\"32\" byteorder=\"LE\" />");
    strStructure.append(
        "<element name=\"value85bits3\"  arraysize=\"1\" type=\"tUInt8\"  alignment=\"4\" "
        "bytepos=\"8\" bitpos=\"5\" numbits=\"3\" byteorder=\"LE\" />");
    strStructure.append("</struct>");

    codec::CodecFactory oFactory("tTest", strStructure.c_str());
    a_util::memory::MemoryBuffer oBuffer(oFactory.getStaticBufferSize());
    codec::Codec oCodec = oFactory.makeCodecFor(oBuffer.getPtr(), oBuffer.getSize());

    uint16_t ui16Value = 0x00;
    ASSERT_NO_THROW(oCodec.getElement("value10bits1").setRawValue(&ui16Value, 0));
    uint32_t ui32Value = 0x07;
    ASSERT_NO_THROW(oCodec.getElement("value11bits3").setRawValue(&ui32Value, 0));
    ui32Value = 0x00;
    ASSERT_NO_THROW(oCodec.getElement("value14bits25").setRawValue(&ui32Value, 0));
    ui32Value = 0xFFFFFFFF;
    ASSERT_NO_THROW(oCodec.getElement("value45bits32").setRawValue(&ui32Value, 0));
    uint8_t ui8Value = 0x00;
    ASSERT_NO_THROW(oCodec.getElement("value85bits3").setRawValue(&ui8Value, 0));

    a_util::memory::MemoryBuffer oWriteBuffer;
    ASSERT_EQ(a_util::result::SUCCESS, codec::transformToBuffer(oCodec, oWriteBuffer, true));

    const tSerializedStructure9* psSerStructure =
        (const tSerializedStructure9*)oWriteBuffer.getPtr();

    ASSERT_EQ(psSerStructure->ui8Value[0], 0);
    ASSERT_EQ(psSerStructure->ui8Value[1], 0x0E);
    ASSERT_EQ(psSerStructure->ui8Value[2], 0x00);
    ASSERT_EQ(psSerStructure->ui8Value[3], 0x00);
    ASSERT_EQ(psSerStructure->ui8Value[4], 0xE0);
    ASSERT_EQ(psSerStructure->ui8Value[5], 0xFF);
    ASSERT_EQ(psSerStructure->ui8Value[6], 0xFF);
    ASSERT_EQ(psSerStructure->ui8Value[7], 0xFF);
    ASSERT_EQ(psSerStructure->ui8Value[8], 0x1F);

    // deserialize into oBuffer
    codec::Codec oSerCodec =
        oFactory.makeCodecFor(oWriteBuffer.getPtr(), oWriteBuffer.getSize(), serialized);
    ASSERT_EQ(a_util::result::SUCCESS, codec::transformToBuffer(oSerCodec, oBuffer, true));
    oCodec = oFactory.makeCodecFor(oBuffer.getPtr(), oBuffer.getSize());

    ui16Value = 0xFF;
    ASSERT_NO_THROW(oCodec.getElement("value10bits1").getRawValue(&ui16Value, 0));
    ASSERT_EQ(ui16Value, 0x00);
    ui32Value = 0x01;
    ASSERT_NO_THROW(oCodec.getElement("value11bits3").getRawValue(&ui32Value, 0));
    ASSERT_EQ(ui32Value, 0x07u);
    ui32Value = 0xFEFE;
    ASSERT_NO_THROW(oCodec.getElement("value14bits25").getRawValue(&ui32Value, 0));
    ASSERT_EQ(ui32Value, 0x00u);
    ui32Value = 0xFFFF;
    ASSERT_NO_THROW(oCodec.getElement("value45bits32").getRawValue(&ui32Value, 0));
    ASSERT_EQ(ui32Value, 0xFFFFFFFFu);
    ASSERT_NO_THROW(oCodec.getElement("value85bits3").getRawValue(&ui8Value, 0));
    // this is because only 1 byte is changed by the get function
    ASSERT_EQ(ui8Value, uint8_t{0x00});
}

/**
 * @detail Media Coder Serialization for bit values without byte gaps in big endian representation.
 * This test serializes bits as described and check the serialized data buffer.
 * @req_id ACORE-1265 ACORE-2641
 */
TEST(cTesterMediaCoder, TestCoderForEverySerializationPathDifferentPositionTogetherInBigEndian)
{
    typedef struct {
        uint8_t ui8Value[9];
    } tSerializedStructure9;

    std::string strStructure = "<struct type=\"tTest\" alignment=\"8\" version=\"2\">";
    strStructure.append(
        "<element name=\"value10bits1\"  arraysize=\"1\" type=\"tUInt16\" alignment=\"4\" "
        "bytepos=\"1\" bitpos=\"0\" numbits=\"1\" byteorder=\"BE\" />");
    strStructure.append(
        "<element name=\"value11bits3\"  arraysize=\"1\" type=\"tUInt32\" alignment=\"2\" "
        "bytepos=\"1\" bitpos=\"1\" numbits=\"3\" byteorder=\"BE\" />");
    strStructure.append(
        "<element name=\"value14bits25\" arraysize=\"1\" type=\"tUInt32\" alignment=\"2\" "
        "bytepos=\"1\" bitpos=\"4\" numbits=\"25\" byteorder=\"BE\" />");
    strStructure.append(
        "<element name=\"value45bits32\" arraysize=\"1\" type=\"tUInt32\" alignment=\"2\" "
        "bytepos=\"4\" bitpos=\"5\" numbits=\"32\" byteorder=\"BE\" />");
    strStructure.append(
        "<element name=\"value85bits3\"  arraysize=\"1\" type=\"tUInt8\"  alignment=\"4\" "
        "bytepos=\"8\" bitpos=\"5\" numbits=\"3\" byteorder=\"BE\" />");
    strStructure.append("</struct>");

    codec::CodecFactory oFactory("tTest", strStructure.c_str());
    a_util::memory::MemoryBuffer oBuffer(oFactory.getStaticBufferSize());
    codec::Codec oCodec = oFactory.makeCodecFor(oBuffer.getPtr(), oBuffer.getSize());

    uint16_t ui16Value = 0x00;
    ASSERT_NO_THROW(oCodec.getElement("value10bits1").setRawValue(&ui16Value, 0));
    uint32_t ui32Value = 0x07;
    ASSERT_NO_THROW(oCodec.getElement("value11bits3").setRawValue(&ui32Value, 0));
    ui32Value = 0x00;
    ASSERT_NO_THROW(oCodec.getElement("value14bits25").setRawValue(&ui32Value, 0));
    ui32Value = 0xFFFFFFFF;
    ASSERT_NO_THROW(oCodec.getElement("value45bits32").setRawValue(&ui32Value, 0));
    uint8_t ui8Value = 0x00;
    ASSERT_NO_THROW(oCodec.getElement("value85bits3").setRawValue(&ui8Value, 0));

    a_util::memory::MemoryBuffer oWriteBuffer;
    ASSERT_EQ(a_util::result::SUCCESS, codec::transformToBuffer(oCodec, oWriteBuffer, true));
    const tSerializedStructure9* psSerStructure =
        (const tSerializedStructure9*)oWriteBuffer.getPtr();

    ASSERT_EQ(psSerStructure->ui8Value[0], 0);
    ASSERT_EQ(psSerStructure->ui8Value[1], 0x0E);
    ASSERT_EQ(psSerStructure->ui8Value[2], 0x00);
    ASSERT_EQ(psSerStructure->ui8Value[3], 0x00);
    ASSERT_EQ(psSerStructure->ui8Value[4], 0xE0);
    ASSERT_EQ(psSerStructure->ui8Value[5], 0xFF);
    ASSERT_EQ(psSerStructure->ui8Value[6], 0xFF);
    ASSERT_EQ(psSerStructure->ui8Value[7], 0xFF);
    ASSERT_EQ(psSerStructure->ui8Value[8], 0x1F);

    // deserialize into oBuffer
    codec::Codec oSerCodec =
        oFactory.makeCodecFor(oWriteBuffer.getPtr(), oWriteBuffer.getSize(), serialized);
    ASSERT_EQ(a_util::result::SUCCESS, codec::transformToBuffer(oSerCodec, oBuffer, true));
    oCodec = oFactory.makeCodecFor(oBuffer.getPtr(), oBuffer.getSize());

    ui16Value = 0xFF;
    ASSERT_NO_THROW(oCodec.getElement("value10bits1").getRawValue(&ui16Value, 0));
    ASSERT_EQ(ui16Value, 0x00);
    ui32Value = 0x01;
    ASSERT_NO_THROW(oCodec.getElement("value11bits3").getRawValue(&ui32Value, 0));
    ASSERT_EQ(ui32Value, 0x07u);
    ui32Value = 0xFEFE;
    ASSERT_NO_THROW(oCodec.getElement("value14bits25").getRawValue(&ui32Value, 0));
    ASSERT_EQ(ui32Value, 0x00u);
    ui32Value = 0xFFFF;
    ASSERT_NO_THROW(oCodec.getElement("value45bits32").getRawValue(&ui32Value, 0));
    ASSERT_EQ(ui32Value, 0xFFFFFFFFu);
    ASSERT_NO_THROW(oCodec.getElement("value85bits3").getRawValue(&ui8Value, 0));
    // this is because only 1 byte is changed by the get function
    ASSERT_EQ(ui8Value, uint8_t{0x00});
}

/**
 * @detail Media Coder Serialization for bit values without byte gaps in big endian representation.
 * This test serializes bits as described and check the serialized data buffer.
 * @req_id ACORE-1265 ACORE-2641
 */
TEST(cTesterMediaCoder, TestCoderForEverySerializationPathDifferentPositionTogetherInBigEndian2)
{
    typedef struct {
        uint8_t ui8Value[9];
    } tSerializedStructure9;

    std::string strStructure = "<struct type=\"tTest\" alignment=\"8\" version=\"2\">";
    strStructure.append(
        "<element name=\"value10bits1\"  arraysize=\"1\" type=\"tUInt16\" alignment=\"4\" "
        "bytepos=\"1\" bitpos=\"0\" numbits=\"1\" byteorder=\"BE\" />");
    strStructure.append(
        "<element name=\"value11bits3\"  arraysize=\"1\" type=\"tUInt32\" alignment=\"2\" "
        "bytepos=\"1\" bitpos=\"1\" numbits=\"3\" byteorder=\"BE\" />");
    strStructure.append(
        "<element name=\"value14bits25\" arraysize=\"1\" type=\"tUInt32\" alignment=\"2\" "
        "bytepos=\"1\" bitpos=\"4\" numbits=\"25\" byteorder=\"BE\" />");
    strStructure.append(
        "<element name=\"value45bits32\" arraysize=\"1\" type=\"tUInt32\" alignment=\"2\" "
        "bytepos=\"4\" bitpos=\"5\" numbits=\"32\" byteorder=\"BE\" />");
    strStructure.append(
        "<element name=\"value85bits3\"  arraysize=\"1\" type=\"tUInt8\"  alignment=\"4\" "
        "bytepos=\"8\" bitpos=\"5\" numbits=\"3\" byteorder=\"BE\" />");
    strStructure.append("</struct>");

    codec::CodecFactory oFactory("tTest", strStructure.c_str());
    a_util::memory::MemoryBuffer oBuffer(oFactory.getStaticBufferSize());
    codec::Codec oCodec = oFactory.makeCodecFor(oBuffer.getPtr(), oBuffer.getSize());

    uint16_t ui16Value = 0x01;
    ASSERT_NO_THROW(oCodec.getElement("value10bits1").setRawValue(&ui16Value, 0));
    uint32_t ui32Value = 0x00;
    ASSERT_NO_THROW(oCodec.getElement("value11bits3").setRawValue(&ui32Value, 0));
    ui32Value = 0x01FFFFFF;
    ASSERT_NO_THROW(oCodec.getElement("value14bits25").setRawValue(&ui32Value, 0));
    ui32Value = 0x00;
    ASSERT_NO_THROW(oCodec.getElement("value45bits32").setRawValue(&ui32Value, 0));
    uint8_t ui8Value = 0x07;
    ASSERT_NO_THROW(oCodec.getElement("value85bits3").setRawValue(&ui8Value, 0));

    a_util::memory::MemoryBuffer oWriteBuffer;
    ASSERT_EQ(a_util::result::SUCCESS, codec::transformToBuffer(oCodec, oWriteBuffer, true));
    const tSerializedStructure9* psSerStructure =
        (const tSerializedStructure9*)oWriteBuffer.getPtr();

    ASSERT_EQ(psSerStructure->ui8Value[0], 0);
    ASSERT_EQ(psSerStructure->ui8Value[1], 0xF1);
    ASSERT_EQ(psSerStructure->ui8Value[2], 0xFF);
    ASSERT_EQ(psSerStructure->ui8Value[3], 0xFF);
    ASSERT_EQ(psSerStructure->ui8Value[4], 0x1F);
    ASSERT_EQ(psSerStructure->ui8Value[5], 0x00);
    ASSERT_EQ(psSerStructure->ui8Value[6], 0x00);
    ASSERT_EQ(psSerStructure->ui8Value[7], 0x00);
    ASSERT_EQ(psSerStructure->ui8Value[8], 0xE0);

    // deserialize into oBuffer
    codec::Codec oSerCodec =
        oFactory.makeCodecFor(oWriteBuffer.getPtr(), oWriteBuffer.getSize(), serialized);
    ASSERT_EQ(a_util::result::SUCCESS, codec::transformToBuffer(oSerCodec, oBuffer, true));
    oCodec = oFactory.makeCodecFor(oBuffer.getPtr(), oBuffer.getSize());

    ui16Value = 0;
    ASSERT_NO_THROW(oCodec.getElement("value10bits1").getRawValue(&ui16Value, 0));
    ASSERT_EQ(ui16Value, 0x01);
    ui32Value = 0x01;
    ASSERT_NO_THROW(oCodec.getElement("value11bits3").getRawValue(&ui32Value, 0));
    ASSERT_EQ(ui32Value, 0x00u);
    ui32Value = 0x00;
    ASSERT_NO_THROW(oCodec.getElement("value14bits25").getRawValue(&ui32Value, 0));
    ASSERT_EQ(ui32Value, 0x01FFFFFFu);
    ui32Value = 0xFFFF;
    ASSERT_NO_THROW(oCodec.getElement("value45bits32").getRawValue(&ui32Value, 0));
    ASSERT_EQ(ui32Value, 0x00u);
    ASSERT_NO_THROW(oCodec.getElement("value85bits3").getRawValue(&ui8Value, 0));
    ASSERT_EQ(ui8Value, uint8_t{0x07});
}

/**
 * @detail Media Coder Serialization for bit values without byte gaps in big endian representation
 * and with an Array inside. This test serializes bits as described and check the serialized data
 * buffer.
 * @req_id ACORE-1265 ACORE-2641
 */
TEST(cTesterMediaCoder,
     TestCoderForEverySerializationPathDifferentPositionTogetherInBigEndianWithArray)
{
    typedef struct {
        uint8_t ui8Value[18];
    } tSerializedStructure18;

    std::string strStructure = "<struct type=\"tTest\" alignment=\"8\" version=\"2\">";
    strStructure.append(
        "<element name=\"value10bits1\"  arraysize=\"1\" type=\"tUInt16\" alignment=\"4\" "
        "bytepos=\"1\" bitpos=\"0\" numbits=\"1\" byteorder=\"BE\" />");
    strStructure.append(
        "<element name=\"value11bits3\"  arraysize=\"1\" type=\"tUInt32\" alignment=\"2\" "
        "bytepos=\"1\" bitpos=\"1\" numbits=\"3\" byteorder=\"BE\" />");
    strStructure.append(
        "<element name=\"value14bits25_0\" arraysize=\"1\" type=\"tUInt32\" alignment=\"2\" "
        "bytepos=\"1\" bitpos=\"4\" numbits=\"25\" byteorder=\"BE\" />");
    strStructure.append(
        "<element name=\"value14bits25_1\" arraysize=\"1\" type=\"tUInt32\" alignment=\"2\" "
        "bytepos=\"1\" bitpos=\"29\" numbits=\"25\" byteorder=\"BE\" />");
    strStructure.append(
        "<element name=\"value14bits25_2\" arraysize=\"1\" type=\"tUInt32\" alignment=\"2\" "
        "bytepos=\"1\" bitpos=\"54\" numbits=\"25\" byteorder=\"BE\" />");
    strStructure.append(
        "<element name=\"value14bits25_3\" arraysize=\"1\" type=\"tUInt32\" alignment=\"2\" "
        "bytepos=\"1\" bitpos=\"79\" numbits=\"25\" byteorder=\"BE\" />");
    strStructure.append(
        "<element name=\"value14bits25_4\" arraysize=\"1\" type=\"tUInt32\" alignment=\"2\" "
        "bytepos=\"1\" bitpos=\"104\" numbits=\"25\" byteorder=\"BE\" />");
    strStructure.append("</struct>");

    codec::CodecFactory oFactory("tTest", strStructure.c_str());
    a_util::memory::MemoryBuffer oBuffer(oFactory.getStaticBufferSize());
    codec::Codec oCodec = oFactory.makeCodecFor(oBuffer.getPtr(), oBuffer.getSize());

    uint16_t ui16Value = 0x01;
    ASSERT_NO_THROW(oCodec.getElement("value10bits1").setRawValue(&ui16Value, 0));
    uint32_t ui32Value = 0x00;
    ASSERT_NO_THROW(oCodec.getElement("value11bits3").setRawValue(&ui32Value, 0));
    ui32Value = 0x01FFFFFF;
    ASSERT_NO_THROW(oCodec.getElement("value14bits25_0").setRawValue(&ui32Value, 0));
    ui32Value = 0x00;
    ASSERT_NO_THROW(oCodec.getElement("value14bits25_1").setRawValue(&ui32Value, 0));
    ui32Value = 0x01FFFFFF;
    ASSERT_NO_THROW(oCodec.getElement("value14bits25_2").setRawValue(&ui32Value, 0));
    ui32Value = 0x00;
    ASSERT_NO_THROW(oCodec.getElement("value14bits25_3").setRawValue(&ui32Value, 0));
    ui32Value = 0x01FF0FFF;
    ASSERT_NO_THROW(oCodec.getElement("value14bits25_4").setRawValue(&ui32Value, 0));

    a_util::memory::MemoryBuffer oWriteBuffer;
    ASSERT_EQ(a_util::result::SUCCESS, codec::transformToBuffer(oCodec, oWriteBuffer, true));
    const tSerializedStructure18* psSerStructure =
        (const tSerializedStructure18*)oWriteBuffer.getPtr();

    ASSERT_EQ(psSerStructure->ui8Value[0], 0);
    ASSERT_EQ(psSerStructure->ui8Value[1], 0xF1); // arrayelem 0 begins on bit 4
    ASSERT_EQ(psSerStructure->ui8Value[2], 0xFF);
    ASSERT_EQ(psSerStructure->ui8Value[3], 0xFF);
    ASSERT_EQ(psSerStructure->ui8Value[4], 0x1F); // arrayelem 1 begins on bit 5
    ASSERT_EQ(psSerStructure->ui8Value[5], 0x00);
    ASSERT_EQ(psSerStructure->ui8Value[6], 0x00);
    ASSERT_EQ(psSerStructure->ui8Value[7], 0xC0); // arrayelem 2 begins on bit 6
    ASSERT_EQ(psSerStructure->ui8Value[8], 0xFF);
    ASSERT_EQ(psSerStructure->ui8Value[9], 0xFF);
    ASSERT_EQ(psSerStructure->ui8Value[10], 0x7F); // arrayelem 3 begins on bit 7
    ASSERT_EQ(psSerStructure->ui8Value[11], 0x00);
    ASSERT_EQ(psSerStructure->ui8Value[12], 0x00);
    ASSERT_EQ(psSerStructure->ui8Value[13], 0x00);
    ASSERT_EQ(psSerStructure->ui8Value[14], 0xFF); // arrayelem 4 begins on bit 0
    ASSERT_EQ(psSerStructure->ui8Value[15], 0x1F);
    ASSERT_EQ(psSerStructure->ui8Value[16], 0xFE);
    ASSERT_EQ(psSerStructure->ui8Value[17], 0x01);

    // deserialize into oBuffer
    codec::Codec oSerCodec =
        oFactory.makeCodecFor(oWriteBuffer.getPtr(), oWriteBuffer.getSize(), serialized);
    ASSERT_EQ(a_util::result::SUCCESS, codec::transformToBuffer(oSerCodec, oBuffer, true));
    oCodec = oFactory.makeCodecFor(oBuffer.getPtr(), oBuffer.getSize());

    ui16Value = 0;
    ASSERT_NO_THROW(oCodec.getElement("value10bits1").getRawValue(&ui16Value, 0));
    ASSERT_EQ(ui16Value, 0x01);
    ui32Value = 0x01;
    ASSERT_NO_THROW(oCodec.getElement("value11bits3").getRawValue(&ui32Value, 0));
    ASSERT_EQ(ui32Value, 0x00u);
    ASSERT_NO_THROW(oCodec.getElement("value14bits25_0").getRawValue(&ui32Value, 0));
    ASSERT_EQ(ui32Value, 0x01FFFFFFu);

    ASSERT_NO_THROW(oCodec.getElement("value14bits25_1").getRawValue(&ui32Value, 0));
    ASSERT_EQ(ui32Value, 0x00u);

    ASSERT_NO_THROW(oCodec.getElement("value14bits25_2").getRawValue(&ui32Value, 0));
    ASSERT_EQ(ui32Value, 0x01FFFFFFu);

    ASSERT_NO_THROW(oCodec.getElement("value14bits25_3").getRawValue(&ui32Value, 0));
    ASSERT_EQ(ui32Value, 0x00u);

    ASSERT_NO_THROW(oCodec.getElement("value14bits25_4").getRawValue(&ui32Value, 0));
    ASSERT_EQ(ui32Value, 0x01FF0FFFu);
}

/**
 * @detail Media Coder Serialization with many random values.
 * This test serializes and deserializes the given structure described.
 * Passed if: The deserialized data is the same as before.
 * @req_id ACORE-1265 ACORE-2641
 */
TEST(cTesterMediaCoder, TestCoderForWithSeveralValuesForSerialization)
{
    std::string strStructure = "<struct type=\"tTest\" alignment=\"8\" version=\"2\">";
    strStructure.append(
        "<element name=\"value1\"  arraysize=\"1\" type=\"tUInt16\" alignment=\"4\" bytepos=\"1\" "
        "bitpos=\"0\" numbits=\"1\" byteorder=\"LE\" />");
    strStructure.append(
        "<element name=\"value2\"  arraysize=\"1\" type=\"tUInt32\" alignment=\"2\" bytepos=\"1\" "
        "bitpos=\"1\" numbits=\"3\" byteorder=\"BE\" />");
    strStructure.append(
        "<element name=\"value3\"  arraysize=\"1\" type=\"tUInt64\" alignment=\"2\" bytepos=\"1\" "
        "bitpos=\"4\" numbits=\"3\" byteorder=\"LE\" />");
    strStructure.append("<element name=\"array1\" arraysize=\"5\" type=\"tUInt32\" alignment=\"2\" "
                        "bytepos=\"1\" bitpos=\"7\" byteorder=\"BE\" />");
    strStructure.append("<element name=\"array2\" arraysize=\"5\" type=\"tFloat64\" "
                        "alignment=\"2\" bytepos=\"24\"  byteorder=\"BE\" />");
    strStructure.append("</struct>");

    codec::CodecFactory oFactory("tTest", strStructure.c_str());
    a_util::memory::MemoryBuffer oData(oFactory.getStaticBufferSize());
    a_util::memory::MemoryBuffer oDataRead(oFactory.getStaticBufferSize());
    a_util::memory::MemoryBuffer oWriteBuffer(oFactory.getStaticBufferSize());

    for (uint64_t ui64Counter = 40000; ui64Counter < 40100; ui64Counter++) {
        uint16_t ui16TestValueWrite = uint16_t(ui64Counter);
        uint32_t ui32TestValueWrite = uint32_t(ui64Counter);
        uint64_t ui64TestValueWrite = ui64Counter;
        double f64TestValueWrite = double(ui64Counter) * 0.1;
        uint16_t ui16TestValueRead = 0;
        uint32_t ui32TestValueRead = 0;
        uint64_t ui64TestValueRead = 0;
        double f64TestValueRead = 0.0;

        codec::Codec oCodec = oFactory.makeCodecFor(oData.getPtr(), oData.getSize());

        ASSERT_NO_THROW(oCodec.getElement("value1").setRawValue(&ui16TestValueWrite, 0));
        ASSERT_NO_THROW(oCodec.getElement("value2").setRawValue(&ui32TestValueWrite, 0));
        ASSERT_NO_THROW(oCodec.getElement("value3").setRawValue(&ui64TestValueWrite, 0));
        for (int nIdx = 0; nIdx < 5; nIdx++) {
            ASSERT_NO_THROW(oCodec.getElement(a_util::strings::format("array1[%d]", nIdx))
                                .setRawValue(&ui32TestValueWrite, 0));
            ASSERT_NO_THROW(oCodec.getElement(a_util::strings::format("array2[%d]", nIdx))
                                .setRawValue(&f64TestValueWrite, 0));
        }

        ASSERT_EQ(a_util::result::SUCCESS, codec::transformToBuffer(oCodec, oWriteBuffer));
        codec::Codec oSerCodec =
            oFactory.makeCodecFor(oWriteBuffer.getPtr(), oWriteBuffer.getSize(), serialized);
        ASSERT_EQ(a_util::result::SUCCESS, codec::transformToBuffer(oSerCodec, oDataRead));

        oCodec = oFactory.makeCodecFor(oDataRead.getPtr(), oDataRead.getSize());

        ASSERT_NO_THROW(oCodec.getElement("value1").getRawValue(&ui16TestValueRead, 0));
        // one bit value only
        if (true) {
            ASSERT_EQ(ui16TestValueRead, (ui16TestValueWrite & 0x0001));

            ASSERT_NO_THROW(oCodec.getElement("value2").getRawValue(&ui32TestValueRead, 0));
            // three bit value only
            ASSERT_EQ(ui32TestValueRead, (ui32TestValueWrite & 0x00000007));

            ASSERT_NO_THROW(oCodec.getElement("value3").getRawValue(&ui64TestValueRead, 0));
            // three bit value only
            ASSERT_EQ(ui64TestValueRead, (ui64TestValueWrite & 0x0000000000000007));
        }
        else {
            ASSERT_EQ(ui16TestValueRead, (ui16TestValueWrite & 0x0100));

            ASSERT_NO_THROW(oCodec.getElement("value2").getRawValue(&ui32TestValueRead, 0));
            // three bit value only
            ASSERT_EQ(ui32TestValueRead, (ui32TestValueWrite & 0x07000000));

            ASSERT_NO_THROW(oCodec.getElement("value3").getRawValue(&ui64TestValueRead, 0));
            // three bit value only
            ASSERT_EQ(ui64TestValueRead, (ui64TestValueWrite & 0x07000000000000000LL));
        }
        for (int nIdx = 0; nIdx < 5; nIdx++) {
            ASSERT_NO_THROW(oCodec.getElement(a_util::strings::format("array1[%d]", nIdx))
                                .getRawValue(&ui32TestValueRead, 0));
            // twenty five bit value only
            if (true) {
                ASSERT_EQ(ui32TestValueRead, (ui32TestValueWrite & 0x01FFFFFF));
            }
            else {
                ASSERT_EQ(ui32TestValueRead, (ui32TestValueWrite & 0xFFFFFF01));
            }
            ASSERT_NO_THROW(oCodec.getElement(a_util::strings::format("array2[%d]", nIdx))
                                .getRawValue(&f64TestValueRead, 0));
            // all 64 Bit
            ASSERT_EQ(f64TestValueRead, f64TestValueWrite);
        }
    }
}

#pragma pack(push, 1)
typedef struct {
    int16_t i16Number;
    int16_t i16DxV;
    int16_t i16DyV;
    int16_t i16VxV;
    int16_t i16Width;
    int8_t ui8Spare[6]; // padding
} tscHCAObjectData;

typedef struct {
    int32_t i32DistanceY;
    int32_t i32YawAngleDiff;
    int32_t i32HorizCurve;
    int32_t i32HorizCurveChange;
    int32_t i32Foresight;
    int32_t i32Quality;
    int32_t i32TrackID;
    uint8_t ui8Spare[4]; // padding
} tscHCATrackData;

typedef struct {
    double f64TorqueFactor;
    double f64KBIShownVelo;
    double f64ESPVelo;
    bool bBHIndicatorLeft;
    bool bBHIndicatorRight;
    bool bABBeltWarnVF;
    uint8_t ui8MMIHCAIntrPoT;
    bool bMMIHCAVibrActive;
    bool bLDWActive;
    uint8_t ui8Spare[2];
    double f64MUSteerWhAngle;
    double f64ESPBrakePress;
    double f64FDSteerWhTorque;
    double f64SARAOmegaZb;
    double f64SARAAccelYb;
    double f64ESPVSignal;
    tscHCAObjectData asObject[10];
    tscHCATrackData asTrack[8];
    int32_t i32LaneId;
    int32_t i32LaneCount;
} tscHCAData;
#pragma pack(pop)

#pragma pack(push, 8)
typedef struct {
    tscHCAData sElementOfCoder;
} tscHCADataAlignedSize;
#pragma pack(pop)

/**
 * @detail Test Media Coder Serialized Size and Deserialized Size.
 * This test makes sure the coder follows the alignment and
 * byteposition to evaluate sizes for serialized and deserialized data.
 * Passed if: The expected sizes are equal to the evaluated sizes.
 * @req_id ACORE-5158
 */
TEST(cTesterMediaCoder, TestCheckRightSizeEvaluation)
{
    std::string strDDL;
    auto test_file = TEST_FILES_DIR "/hca.description";
    ASSERT_EQ(a_util::filesystem::readTextFile(test_file, strDDL), a_util::filesystem::OK);
    codec::CodecFactory oFactory("scHCAData", strDDL.c_str());

    size_t nSerSize = oFactory.getStaticBufferSize(serialized);
    size_t nDeSerSize = oFactory.getStaticBufferSize(deserialized);
    size_t nDeserSizeFromStruct = sizeof(tscHCADataAlignedSize);
    ASSERT_EQ(nDeserSizeFromStruct, 504U);
    ASSERT_EQ(nSerSize, nDeSerSize);
    ASSERT_EQ(nDeSerSize, nDeserSizeFromStruct);
}

#pragma pack(push, 1)
struct tempStruct {
    uint16_t value1;
    uint8_t arraySize;
    uint32_t dynamicArray[10];
    uint32_t value2;
};

struct tempStruct2 {
    uint16_t value1;
    uint8_t arraySize;
    tempStruct dynamicArray[2];
    uint32_t value2;
};
#pragma pack(pop)

/**
 * @detail Media Coder Dynamic Array Test
 * Test the media coder and serialization for dynamic arrays.
 * @req_id
 */
TEST(cTesterMediaCoder, TestCoderWithDynamicArrays)
{
    std::string strStructure = "<structs>";
    strStructure.append("<struct type=\"tInnerStruct\" alignment=\"1\" version=\"2\">");
    strStructure.append("<element name=\"innervalue1\"  arraysize=\"1\" type=\"tUInt16\" "
                        "alignment=\"1\" bytepos=\"0\" bitpos=\"0\" byteorder=\"LE\" />");
    strStructure.append("<element name=\"innerarraySize\"  arraysize=\"1\" type=\"tUInt8\" "
                        "alignment=\"1\" bytepos=\"2\" bitpos=\"0\" byteorder=\"LE\" />");
    strStructure.append(
        "<element name=\"innerdynamicArray\"  arraysize=\"innerarraySize\" type=\"tUInt32\" "
        "alignment=\"1\" bytepos=\"3\" bitpos=\"0\" byteorder=\"LE\" />");
    strStructure.append("<element name=\"innervalue2\"  arraysize=\"1\" type=\"tUInt32\" "
                        "alignment=\"1\" bytepos=\"-1\" bitpos=\"0\" byteorder=\"LE\" />");
    strStructure.append("</struct>");
    strStructure.append("<struct type=\"tTest\" alignment=\"1\" version=\"2\">");
    strStructure.append("<element name=\"value1\"  arraysize=\"1\" type=\"tUInt16\" "
                        "alignment=\"1\" bytepos=\"0\" bitpos=\"0\" byteorder=\"LE\" />");
    strStructure.append("<element name=\"arraySize\"  arraysize=\"1\" type=\"tUInt8\" "
                        "alignment=\"1\" bytepos=\"2\" bitpos=\"0\" byteorder=\"LE\" />");
    strStructure.append(
        "<element name=\"dynamicArray\"  arraysize=\"arraySize\" type=\"tInnerStruct\" "
        "alignment=\"1\" bytepos=\"3\" bitpos=\"0\" byteorder=\"LE\" />");
    strStructure.append("<element name=\"value2\"  arraysize=\"1\" type=\"tUInt32\" "
                        "alignment=\"1\" bytepos=\"-1\" bitpos=\"0\" byteorder=\"LE\" />");
    strStructure.append("</struct>");
    strStructure.append("</structs>");

    codec::CodecFactory oFactory("tTest", strStructure.c_str());

    std::vector<unsigned int> vecCompare;
    tempStruct2 temp;
    temp.value1 = 996;
    vecCompare.push_back(996);
    temp.arraySize = 2;
    vecCompare.push_back(2);
    for (int i = 0; i < 2; ++i) {
        temp.dynamicArray[i].value1 = 997;
        vecCompare.push_back(997);
        temp.dynamicArray[i].arraySize = 10;
        vecCompare.push_back(10);
        for (int j = 0; j < 10; ++j) {
            temp.dynamicArray[i].dynamicArray[j] = (i + 1) * j;
            vecCompare.push_back((i + 1) * j);
        }
        temp.dynamicArray[i].value2 = 998;
        vecCompare.push_back(998);
    }
    temp.value2 = 999;
    vecCompare.push_back(999);

    codec::Codec oCodec = oFactory.makeCodecFor(&temp, sizeof(tempStruct2));
    a_util::memory::MemoryBuffer oWriteBuffer;
    ASSERT_EQ(a_util::result::SUCCESS, codec::transformToBuffer(oCodec, oWriteBuffer, true));

    oCodec = oFactory.makeCodecFor(oWriteBuffer.getPtr(), oWriteBuffer.getSize(), serialized);
    a_util::memory::MemoryBuffer oDeserializedBuffer;
    ASSERT_EQ(a_util::result::SUCCESS, codec::transformToBuffer(oCodec, oDeserializedBuffer, true));

    oCodec = oFactory.makeCodecFor(oDeserializedBuffer.getPtr(), oDeserializedBuffer.getSize());

    size_t element_counter = 0;
    codec::forEachLeafElement(oCodec.getElements(), [&element_counter, &vecCompare](auto& element) {
        switch (element.getType()) {
        case codec::ElementType::cet_uint16: {
            uint16_t value;
            element.getRawValue(&value, sizeof(value));
            ASSERT_EQ(vecCompare.at(element_counter), value);
            break;
        }
        case codec::ElementType::cet_uint8: {
            uint8_t value;
            element.getRawValue(&value, sizeof(value));
            ASSERT_EQ(vecCompare.at(element_counter), value);
            break;
        }

        default: {
            uint32_t value;
            element.getRawValue(&value, sizeof(value));
            ASSERT_EQ(vecCompare.at(element_counter), value);
            break;
        }
        }
        element_counter++;
    });
}

#pragma pack(push, 1)
typedef enum : std::uint32_t { TEN = 10, FORTYTWO = 42, FIFTYSIX = 56 } tEnumTest;
typedef struct {
    uint32_t enumElement;
    uint32_t constantElement;
    uint32_t enumElement2;
} tStructWithConstantAndEnum;

#pragma pack(pop)

/**
 * @detail Media Coder Enum Constant Test
 * Tests the media coder and serialization for Enums and Constants.
 * @req_id
 */
TEST(cTesterMediaCoder, TestCoderWithEnumAndConstant)
{
    std::string strStructure = "<adtf:ddl xmlns:adtf=\"adtf\">";
    strStructure.append("<enums>");
    strStructure.append("<enum name=\"tTestEnum\" type=\"tUInt32\">");
    strStructure.append("<element name=\"TEN\" value=\"10\"/>");
    strStructure.append("<element name=\"FORTYTWO\" value=\"42\"/>");
    strStructure.append("<element name=\"FIFTYSIX\" value=\"56\"/>");
    strStructure.append("</enum>");
    strStructure.append("</enums>");
    strStructure.append("<structs>");
    strStructure.append(
        "<struct name=\"tStructWithConstantAndEnum\" version=\"1\" alignment=\"1\">");
    strStructure.append("<element name=\"enumElement\" type=\"tTestEnum\" bytepos=\"0\" "
                        "alignment=\"1\" arraysize=\"1\" byteorder=\"LE\"/>");
    strStructure.append("<element name=\"constantElement\" type=\"tTestEnum\" bytepos=\"4\" "
                        "alignment=\"1\" arraysize=\"1\" byteorder=\"LE\" value=\"FORTYTWO\"/>");
    strStructure.append("<element name=\"enumElement2\" type=\"tTestEnum\" bytepos=\"8\" "
                        "alignment=\"1\" arraysize=\"1\" byteorder=\"LE\" />");
    strStructure.append("</struct>");
    strStructure.append("</structs>");
    strStructure.append("</adtf:ddl>");

    tStructWithConstantAndEnum oStruct;
    oStruct.enumElement = FIFTYSIX;
    oStruct.enumElement2 = FORTYTWO;
    oStruct.constantElement =
        TEN; // this should be overwritten by the constant definition in the media description

    codec::CodecFactory oFactory("tStructWithConstantAndEnum", strStructure.c_str());
    codec::Codec oCodec = oFactory.makeCodecFor(&oStruct, sizeof(tStructWithConstantAndEnum));
    ASSERT_NO_THROW(oCodec.resetValues());

    ASSERT_EQ(oCodec.getElement("constantElement").getVariantValue().getUInt32(), FORTYTWO);
    ASSERT_EQ(oCodec.getElement("enumElement").getVariantValue().getUInt32(), FIFTYSIX);
    ASSERT_EQ(oCodec.getElement("enumElement2").getVariantValue().getUInt32(), FORTYTWO);

    ASSERT_NO_THROW(oCodec.getElement("enumElement").setValue(static_cast<uint32_t>(TEN)));

    a_util::memory::MemoryBuffer oWriteBuffer;
    ASSERT_EQ(a_util::result::SUCCESS, codec::transformToBuffer(oCodec, oWriteBuffer));

    oCodec = oCodec.makeCodecFor(oWriteBuffer.getPtr(), oWriteBuffer.getSize(), serialized);

    a_util::memory::MemoryBuffer oReadBuffer;
    ASSERT_EQ(a_util::result::SUCCESS, codec::transformToBuffer(oCodec, oReadBuffer));

    oCodec = oCodec.makeCodecFor(oReadBuffer.getPtr(), oReadBuffer.getSize(), deserialized);

    ASSERT_EQ(oCodec.getElement("constantElement").getVariantValue().getUInt32(), FORTYTWO);
    ASSERT_EQ(oCodec.getElement("enumElement").getVariantValue().getUInt32(), TEN);
    ASSERT_EQ(oCodec.getElement("enumElement2").getVariantValue().getUInt32(), FORTYTWO);
}

#pragma pack(push, 1)
typedef struct {
    uint16_t innerValue1;
    uint8_t innerPadding1[6];
    uint8_t innerArraySize;
    uint8_t innerPadding2[7];
    uint32_t innerDynamicArray[4];
    uint32_t innerValue2;
} tInnerStruct_v20;

typedef struct {
    uint16_t value1;
    uint8_t padding1[14];
    uint8_t arraySize;
    uint8_t padding2[15];
    // Begin unrolled array
    tInnerStruct_v20 dynamicArray_0;
    uint8_t innerpadding3_1[4];
    tInnerStruct_v20 dynamicArray_1;
    uint8_t innerpadding3_2[4];
    tInnerStruct_v20 dynamicArray_2;
    uint8_t innerpadding3_3[4];
    tInnerStruct_v20 dynamicArray_3;
    // End unrolled array
    uint8_t innerpadding3_4[4];
    uint32_t value2;
} tOuterStruct_v20;

typedef struct {
    uint16_t innerValue1;
    uint8_t innerPadding1[6];
    uint8_t innerArraySize;
    uint8_t innerPadding2[7];
    uint32_t innerDynamicArray[4];
    uint32_t innerValue2;
    uint8_t innerPadding3[4];
} tInnerStruct_v30;

typedef struct {
    uint16_t value1;
    uint8_t padding1[14];
    uint8_t arraySize;
    uint8_t padding2[15];
    tInnerStruct_v30 dynamicArray[4];
    uint32_t value2;
    uint8_t padding3[12];
} tOuterStruct_v30;

#pragma pack(pop)

/**
 * Temnplate function for filling the inner structure.
 * Use with tInnerStruct_v20, tInnerStruct_v30 and so on.
 * @param [inout] pvecCompare  An initialized vector. This vector will be filled with reference
 * values.
 * @param [inout] offset  The number to start with filling the struct. Will be updated during
 * filling.
 * @return The initialized struct.
 */
template <class I>
I FillInnerStruct(std::vector<unsigned int>* pvecCompare, unsigned int* pui32Offset)
{
    I sInner;
    a_util::memory::set(&sInner, sizeof(I), 0, sizeof(I));
    sInner.innerValue1 = static_cast<uint16_t>(*pui32Offset);
    pvecCompare->push_back(sInner.innerValue1);
    (*pui32Offset)++;
    sInner.innerArraySize = 4;
    pvecCompare->push_back(sInner.innerArraySize);
    for (unsigned int i = 0; i < sInner.innerArraySize; i++) {
        sInner.innerDynamicArray[i] = *pui32Offset;
        pvecCompare->push_back(sInner.innerDynamicArray[i]);
        (*pui32Offset)++;
    }
    sInner.innerValue2 = *pui32Offset;
    pvecCompare->push_back(sInner.innerValue2);
    (*pui32Offset)++;
    return sInner;
}

template <class S>
void ExecuteTest(S sReference,
                 const std::vector<unsigned int>& vecReference,
                 ddl::dd::Version eVersion);

/**
 * @detail Media Coder Dynamic Array Alignment Test
 * Test the media coder and serialization for dynamic arrays with alignment != 1
 * @req_id ACORE-6148
 */
TEST(cTesterMediaCoder, TestCoderWithDynamicArraysAlignmentNotOne)
{
    // Build up the test data
    std::vector<unsigned int> vecCompare_v20;
    std::vector<unsigned int> vecCompare_v30;
    tOuterStruct_v20 sReference_v20;
    tOuterStruct_v30 sReference_v30;
    a_util::memory::set(&sReference_v20, sizeof(sReference_v20), 0, sizeof(sReference_v20));
    a_util::memory::set(&sReference_v30, sizeof(sReference_v30), 0, sizeof(sReference_v30));
    uint32_t ui32Offset_v20 = 1;
    uint32_t ui32Offset_v30 = 1;
    sReference_v20.value1 = static_cast<uint16_t>(ui32Offset_v20);
    sReference_v30.value1 = static_cast<uint16_t>(ui32Offset_v30);
    vecCompare_v20.push_back(ui32Offset_v20);
    vecCompare_v30.push_back(ui32Offset_v30);
    ui32Offset_v20++;
    ui32Offset_v30++;
    sReference_v20.arraySize = 4;
    sReference_v30.arraySize = 4;
    vecCompare_v20.push_back(sReference_v20.arraySize);
    vecCompare_v30.push_back(sReference_v30.arraySize);
    sReference_v20.dynamicArray_0 =
        FillInnerStruct<tInnerStruct_v20>(&vecCompare_v20, &ui32Offset_v20);
    sReference_v20.dynamicArray_1 =
        FillInnerStruct<tInnerStruct_v20>(&vecCompare_v20, &ui32Offset_v20);
    sReference_v20.dynamicArray_2 =
        FillInnerStruct<tInnerStruct_v20>(&vecCompare_v20, &ui32Offset_v20);
    sReference_v20.dynamicArray_3 =
        FillInnerStruct<tInnerStruct_v20>(&vecCompare_v20, &ui32Offset_v20);
    for (size_t nIdx = 0; nIdx < sReference_v30.arraySize; nIdx++) {
        sReference_v30.dynamicArray[nIdx] =
            FillInnerStruct<tInnerStruct_v30>(&vecCompare_v30, &ui32Offset_v30);
    }
    sReference_v20.value2 = ui32Offset_v20;
    sReference_v30.value2 = ui32Offset_v30;
    vecCompare_v20.push_back(ui32Offset_v20);
    vecCompare_v30.push_back(ui32Offset_v30);

    ExecuteTest(sReference_v20, vecCompare_v20, ddl::dd::Version(2, 0));
    ExecuteTest(sReference_v30, vecCompare_v30, ddl::dd::Version(3, 0));
}

template <class S>
void ExecuteTest(S sReference,
                 const std::vector<unsigned int>& vecReference,
                 ddl::dd::Version eVersion)
{
    std::string strStructure = "<structs>";
    strStructure.append("<struct type=\"tInnerStruct\" alignment=\"8\" version=\"2\">");
    strStructure.append("<element name=\"innervalue1\"  arraysize=\"1\" type=\"tUInt16\" "
                        "alignment=\"8\" bytepos=\"0\" bitpos=\"0\" byteorder=\"LE\" />");
    strStructure.append("<element name=\"innerarraySize\"  arraysize=\"1\" type=\"tUInt8\" "
                        "alignment=\"8\" bytepos=\"2\" bitpos=\"0\" byteorder=\"LE\" />");
    strStructure.append(
        "<element name=\"innerdynamicArray\"  arraysize=\"innerarraySize\" type=\"tUInt32\" "
        "alignment=\"8\" bytepos=\"3\" bitpos=\"0\" byteorder=\"LE\" />");
    strStructure.append("<element name=\"innervalue2\"  arraysize=\"1\" type=\"tUInt32\" "
                        "alignment=\"8\" bytepos=\"-1\" bitpos=\"0\" byteorder=\"LE\" />");
    strStructure.append("</struct>");
    strStructure.append("<struct type=\"tOuterStruct\" alignment=\"16\" version=\"2\">");
    strStructure.append("<element name=\"value1\"  arraysize=\"1\" type=\"tUInt16\" "
                        "alignment=\"16\" bytepos=\"0\" bitpos=\"0\" byteorder=\"LE\" />");
    strStructure.append("<element name=\"arraySize\"  arraysize=\"1\" type=\"tUInt8\" "
                        "alignment=\"16\" bytepos=\"2\" bitpos=\"0\" byteorder=\"LE\" />");
    strStructure.append(
        "<element name=\"dynamicArray\"  arraysize=\"arraySize\" type=\"tInnerStruct\" "
        "alignment=\"16\" bytepos=\"3\" bitpos=\"0\" byteorder=\"LE\" />");
    strStructure.append("<element name=\"value2\"  arraysize=\"1\" type=\"tUInt32\" "
                        "alignment=\"16\" bytepos=\"-1\" bitpos=\"0\" byteorder=\"LE\" />");
    strStructure.append("</struct>");
    strStructure.append("</structs>");

    std::string strDDL;
    if (eVersion == ddl::dd::Version(2, 0)) {
        strDDL = strDDLv20Header + strStructure + "</adtf:ddl>";
    }
    else {
        strDDL = strDDLv30Header + strStructure + "</adtf:ddl>";
    }

    codec::CodecFactory oFactory("tOuterStruct", strDDL.c_str());

    codec::Decoder oDecoder = oFactory.makeDecoderFor(&sReference, sizeof(sReference));
    a_util::memory::MemoryBuffer oDestBuf;
    ASSERT_EQ(a_util::result::SUCCESS, codec::transformToBuffer(oDecoder, oDestBuf, true));

    // create a new coder to test deserialization
    oDecoder = oFactory.makeDecoderFor(oDestBuf.getPtr(), oDestBuf.getSize(), serialized);
    a_util::memory::MemoryBuffer oNewDestBuf;
    ASSERT_EQ(a_util::result::SUCCESS, codec::transformToBuffer(oDecoder, oNewDestBuf, true));

    oDecoder = oFactory.makeDecoderFor(oNewDestBuf.getPtr(), oNewDestBuf.getSize());

    size_t element_counter = 0;
    codec::forEachLeafElement(
        oDecoder.getElements(), [&element_counter, &vecReference](const auto& element) {
            switch (static_cast<a_util::variant::VariantType>(element.getType())) {
            case a_util::variant::VT_UInt16: {
                uint16_t value = element.template getValue<uint16_t>();
                ASSERT_EQ(vecReference.at(element_counter), value);
                break;
            }
            case a_util::variant::VT_UInt8: {
                uint8_t value = element.template getValue<uint8_t>();
                ASSERT_EQ(vecReference.at(element_counter), value);
                break;
            }

            default: {
                uint32_t value = element.template getValue<uint32_t>();
                ASSERT_EQ(vecReference.at(element_counter), value);
                break;
            }
            }
            element_counter++;
        });
}

/**
 * @detail Test the Path to an element in a MediaDescription
 * Tests if the function getPathForID returns the correct path to an element referenced by its ID in
 * a Media description Passed if: The values are set and retrieved correctly
 * @req_id ACORE-6631
 */
TEST(cTesterMediaCoder, TestgetFullPathToAnElement)
{
    std::string strToParse;

    strToParse = "<description> ";
    strToParse.append("<structs>");
    strToParse.append("    <struct alignment=\"1\" name=\"adtf.core.media_type\" version=\"1\">");
    strToParse.append("       <element alignment=\"1\" arraysize=\"1\" byteorder=\"LE\" "
                      "bytepos=\"0\" name=\"mediatype\" type=\"tMediaTypeInfo\" />");
    strToParse.append("    </struct>");

    strToParse.append("    <struct alignment=\"1\" name=\"tMediaTypeInfo\" version=\"1\">");
    strToParse.append("       <element alignment=\"1\" arraysize=\"1\" byteorder=\"LE\" "
                      "bytepos=\"0\" name=\"ui32MajorType\" type=\"tUInt32\" />");
    strToParse.append("       <element alignment=\"1\" arraysize=\"1\" byteorder=\"LE\" "
                      "bytepos=\"4\" name=\"ui32SubType\" type=\"tUInt32\" />");
    strToParse.append("       <element alignment=\"1\" arraysize=\"1\" byteorder=\"LE\" "
                      "bytepos=\"8\" name=\"ui32Flags\" type=\"tUInt32\" />");
    strToParse.append("     </struct>");

    strToParse.append("     <struct alignment=\"1\" name=\"tHeaderStructInfo\" version=\"1\">");
    strToParse.append("        <element alignment=\"1\" arraysize=\"1\" byteorder=\"LE\" "
                      "bytepos=\"0\" name=\"ui32HeaderInfoVal\" type=\"tUInt32\" />");
    strToParse.append("        <element alignment=\"1\" arraysize=\"1\" byteorder=\"LE\" "
                      "bytepos=\"4\" name=\"f64HeaderInfoVal\" type=\"tFloat64\" />");
    strToParse.append("     </struct>");

    strToParse.append("     <struct alignment=\"1\" name=\"tHeaderStruct\" version=\"1\">");
    strToParse.append("        <element alignment=\"1\" arraysize=\"1\" byteorder=\"LE\" "
                      "bytepos=\"0\" name=\"ui32HeaderVal\" type=\"tUInt32\" />");
    strToParse.append("        <element alignment=\"1\" arraysize=\"1\" byteorder=\"LE\" "
                      "bytepos=\"4\" name=\"f64HeaderVal\" type=\"tFloat64\" />");
    strToParse.append("        <element alignment=\"1\" arraysize=\"1\" byteorder=\"LE\" "
                      "bytepos=\"0\" name=\"sHeaderInfoStruct\" type=\"tHeaderStructInfo\" />");
    strToParse.append("     </struct>");

    strToParse.append("     <struct alignment=\"1\" name=\"tNestedStruct\" version=\"1\">");
    strToParse.append("         <element alignment=\"1\" arraysize=\"1\" byteorder=\"LE\" "
                      "bytepos=\"0\" name=\"sHeaderStruct\" type=\"tHeaderStruct\" />");
    strToParse.append("         <element alignment=\"1\" arraysize=\"25\" byteorder=\"LE\" "
                      "bytepos=\"12\" name=\"sSimpleStruct\" type=\"tSimpleStruct_LE\" />");
    strToParse.append("     </struct>");

    strToParse.append("     <struct alignment=\"1\" name=\"tSimpleStruct_LE\" version=\"1\">");
    strToParse.append("         <element alignment=\"1\" arraysize=\"1\" byteorder=\"LE\" "
                      "bytepos=\"0\" name=\"ui8Val\" type=\"tUInt8\" />");
    strToParse.append("         <element alignment=\"1\" arraysize=\"1\" byteorder=\"LE\" "
                      "bytepos=\"1\" name=\"ui16Val\" type=\"tUInt16\" />");
    strToParse.append("         <element alignment=\"1\" arraysize=\"1\" byteorder=\"LE\" "
                      "bytepos=\"3\" name=\"ui32Val\" type=\"tUInt32\" />");
    strToParse.append("         <element alignment=\"1\" arraysize=\"1\" byteorder=\"LE\" "
                      "bytepos=\"7\" name=\"i32Val\" type=\"tInt32\" />");
    strToParse.append("         <element alignment=\"1\" arraysize=\"1\" byteorder=\"LE\" "
                      "bytepos=\"11\" name=\"i64Val\" type=\"tInt64\" />");
    strToParse.append("         <element alignment=\"1\" arraysize=\"1\" byteorder=\"LE\" "
                      "bytepos=\"19\" name=\"f64Val\" type=\"tFloat64\" />");
    strToParse.append("         <element alignment=\"1\" arraysize=\"1\" byteorder=\"LE\" "
                      "bytepos=\"27\" name=\"f32Val\" type=\"tFloat32\" />");
    strToParse.append("       </struct>");
    strToParse.append("    </structs>");
    strToParse.append("</description>");

    codec::CodecFactory oFactory("tNestedStruct", strToParse.c_str());
    const StructElement* pElement;
    ASSERT_EQ(a_util::result::SUCCESS, oFactory.getStaticElement(0, pElement));
    std::cout << pElement->name.c_str();
    ASSERT_EQ(pElement->name, "sHeaderStruct.ui32HeaderVal");
}

#pragma pack(push, 1)
struct structWithZeroArraySize {
    uint16_t value1;
    uint8_t arraySize;
    // uint32_t dynamicArray[0];
    uint32_t value2;
};

struct structWithZeroInner {
    uint16_t value1;
    uint8_t arraySize;
    structWithZeroArraySize dynamicArray[2];
    uint32_t value2;
};
#pragma pack(pop)

/**
 * @detail Media Coder Dynamic Array Size Zero Test
 * Tests the media coder and serialization for dynamic arrays with an array size of 0
 * @req_id
 */
TEST(cTesterMediaCoder, TestCoderWithDynamicArraysSizeZero)
{
    std::string strStructure = "<structs>";
    strStructure.append("<struct type=\"tInnerStruct\" alignment=\"1\" version=\"2\">");
    strStructure.append("<element name=\"innervalue1\"  arraysize=\"1\" type=\"tUInt16\" "
                        "alignment=\"1\" bytepos=\"0\" bitpos=\"0\" byteorder=\"LE\" />");
    strStructure.append("<element name=\"innerarraySize\"  arraysize=\"1\" type=\"tUInt8\" "
                        "alignment=\"1\" bytepos=\"2\" bitpos=\"0\" byteorder=\"LE\" />");
    strStructure.append(
        "<element name=\"innerdynamicArray\"  arraysize=\"innerarraySize\" type=\"tUInt32\" "
        "alignment=\"1\" bytepos=\"3\" bitpos=\"0\" byteorder=\"LE\" />");
    strStructure.append("<element name=\"innervalue2\"  arraysize=\"1\" type=\"tUInt32\" "
                        "alignment=\"1\" bytepos=\"-1\" bitpos=\"0\" byteorder=\"LE\" />");
    strStructure.append("</struct>");
    strStructure.append("<struct type=\"tTest\" alignment=\"1\" version=\"2\">");
    strStructure.append("<element name=\"value1\"  arraysize=\"1\" type=\"tUInt16\" "
                        "alignment=\"1\" bytepos=\"0\" bitpos=\"0\" byteorder=\"LE\" />");
    strStructure.append("<element name=\"arraySize\"  arraysize=\"1\" type=\"tUInt8\" "
                        "alignment=\"1\" bytepos=\"2\" bitpos=\"0\" byteorder=\"LE\" />");
    strStructure.append(
        "<element name=\"dynamicArray\"  arraysize=\"arraySize\" type=\"tInnerStruct\" "
        "alignment=\"1\" bytepos=\"3\" bitpos=\"0\" byteorder=\"LE\" />");
    strStructure.append("<element name=\"value2\"  arraysize=\"1\" type=\"tUInt32\" "
                        "alignment=\"1\" bytepos=\"-1\" bitpos=\"0\" byteorder=\"LE\" />");
    strStructure.append("</struct>");
    strStructure.append("</structs>");

    codec::CodecFactory oFactory("tTest", strStructure.c_str());

    std::vector<unsigned int> vecCompare;
    structWithZeroInner temp;
    temp.value1 = 996;
    vecCompare.push_back(996);
    temp.arraySize = 2;
    vecCompare.push_back(2);
    for (int i = 0; i < 2; ++i) {
        temp.dynamicArray[i].value1 = 997;
        vecCompare.push_back(997);
        temp.dynamicArray[i].arraySize = 0;
        vecCompare.push_back(0);
        temp.dynamicArray[i].value2 = 998;
        vecCompare.push_back(998);
    }
    temp.value2 = 999;
    vecCompare.push_back(999);

    codec::Codec oCodec = oFactory.makeCodecFor(&temp, sizeof(structWithZeroInner));
    ASSERT_EQ(a_util::result::SUCCESS, oCodec.isValid());
    a_util::memory::MemoryBuffer oWriteBuffer;
    ASSERT_EQ(a_util::result::SUCCESS, codec::transformToBuffer(oCodec, oWriteBuffer));

    oCodec = oFactory.makeCodecFor(oWriteBuffer.getPtr(), oWriteBuffer.getSize(), serialized);
    a_util::memory::MemoryBuffer oReadBuffer;
    ASSERT_EQ(a_util::result::SUCCESS, codec::transformToBuffer(oCodec, oReadBuffer));

    oCodec = oCodec.makeCodecFor(oReadBuffer.getPtr(), oReadBuffer.getSize(), deserialized);

    size_t element_counter = 0;
    codec::forEachLeafElement(oCodec.getElements(), [&element_counter, &vecCompare](auto& element) {
        switch (static_cast<a_util::variant::VariantType>(element.getType())) {
        case a_util::variant::VT_UInt16: {
            uint16_t value = element.template getValue<uint16_t>();
            ASSERT_EQ(vecCompare.at(element_counter), value);
            break;
        }
        case a_util::variant::VT_UInt8: {
            uint8_t value = element.template getValue<uint8_t>();
            ASSERT_EQ(vecCompare.at(element_counter), value);
            break;
        }

        default: {
            uint32_t value = element.template getValue<uint32_t>();
            ASSERT_EQ(vecCompare.at(element_counter), value);
            break;
        }
        }
        element_counter++;
    });
}

#pragma pack(push, 1)

struct structWithZeroInner2 {
    uint16_t value1;
    uint8_t arraySize;
    //    structWithZeroArraySize2 dynamicArray[2];
    uint32_t value2;
};
#pragma pack(pop)

/**
 * @detail Media Coder Dynamic Array Size Zero Test 2
 * Tests the media coder and serialization for dynamic arrays with an array size of 0
 * @req_id
 */
TEST(cTesterMediaCoder, TestCoderWithDynamicArraysSizeZero2)
{
    std::string strStructure = "<structs>";
    strStructure.append("<struct type=\"tInnerStruct\" alignment=\"1\" version=\"2\">");
    strStructure.append("<element name=\"innervalue1\"  arraysize=\"1\" type=\"tUInt16\" "
                        "alignment=\"1\" bytepos=\"0\" bitpos=\"0\" byteorder=\"LE\" />");
    strStructure.append("<element name=\"innerarraySize\"  arraysize=\"1\" type=\"tUInt8\" "
                        "alignment=\"1\" bytepos=\"2\" bitpos=\"0\" byteorder=\"LE\" />");
    strStructure.append(
        "<element name=\"innerdynamicArray\"  arraysize=\"innerarraySize\" type=\"tUInt32\" "
        "alignment=\"1\" bytepos=\"3\" bitpos=\"0\" byteorder=\"LE\" />");
    strStructure.append("<element name=\"innervalue2\"  arraysize=\"1\" type=\"tUInt32\" "
                        "alignment=\"1\" bytepos=\"-1\" bitpos=\"0\" byteorder=\"LE\" />");
    strStructure.append("</struct>");
    strStructure.append("<struct type=\"tTest\" alignment=\"1\" version=\"2\">");
    strStructure.append("<element name=\"value1\"  arraysize=\"1\" type=\"tUInt16\" "
                        "alignment=\"1\" bytepos=\"0\" bitpos=\"0\" byteorder=\"LE\" />");
    strStructure.append("<element name=\"arraySize\"  arraysize=\"1\" type=\"tUInt8\" "
                        "alignment=\"1\" bytepos=\"2\" bitpos=\"0\" byteorder=\"LE\" />");
    strStructure.append(
        "<element name=\"dynamicArray\"  arraysize=\"arraySize\" type=\"tInnerStruct\" "
        "alignment=\"1\" bytepos=\"3\" bitpos=\"0\" byteorder=\"LE\" />");
    strStructure.append("<element name=\"value2\"  arraysize=\"1\" type=\"tUInt32\" "
                        "alignment=\"1\" bytepos=\"-1\" bitpos=\"0\" byteorder=\"LE\" />");
    strStructure.append("</struct>");
    strStructure.append("</structs>");

    std::vector<unsigned int> vecCompare;
    structWithZeroInner2 temp;
    temp.value1 = 996;
    vecCompare.push_back(996);
    temp.arraySize = 0;
    vecCompare.push_back(0);
    temp.value2 = 999;
    vecCompare.push_back(999);

    codec::CodecFactory oFactory("tTest", strStructure.c_str());
    codec::Codec oCodec = oFactory.makeCodecFor(&temp, sizeof(structWithZeroInner));
    a_util::memory::MemoryBuffer oWriteBuffer;
    ASSERT_EQ(a_util::result::SUCCESS, codec::transformToBuffer(oCodec, oWriteBuffer));

    oCodec = oFactory.makeCodecFor(oWriteBuffer.getPtr(), oWriteBuffer.getSize(), serialized);
    a_util::memory::MemoryBuffer oReadBuffer;
    ASSERT_EQ(a_util::result::SUCCESS, codec::transformToBuffer(oCodec, oReadBuffer));

    oCodec = oCodec.makeCodecFor(oReadBuffer.getPtr(), oReadBuffer.getSize(), deserialized);

    size_t element_counter = 0;
    codec::forEachLeafElement(oCodec.getElements(), [&element_counter, &vecCompare](auto& element) {
        switch (static_cast<a_util::variant::VariantType>(element.getType())) {
        case a_util::variant::VT_UInt16: {
            uint16_t value = element.template getValue<uint16_t>();
            ASSERT_EQ(vecCompare.at(element_counter), value);
            break;
        }
        case a_util::variant::VT_UInt8: {
            uint8_t value = element.template getValue<uint8_t>();
            ASSERT_EQ(vecCompare.at(element_counter), value);
            break;
        }

        default: {
            uint32_t value = element.template getValue<uint32_t>();
            ASSERT_EQ(vecCompare.at(element_counter), value);
            break;
        }
        }
        element_counter++;
    });
}

/**
 * @detail Media Coder Dynamic Array Size Zero Test 3
 * Tests the media coder and serialization for dynamic arrays with an array size of 0
 * @req_id
 */
TEST(cTesterMediaCoder, TestCoderWithDynamicArraysSizeZero3)
{
    std::string strStructure = "<structs>";
    strStructure.append("<struct type=\"tInnerStruct\" alignment=\"1\" version=\"2\">");
    strStructure.append("<element name=\"innervalue\"  arraysize=\"1000\" type=\"tUInt8\" "
                        "alignment=\"1\" bytepos=\"0\" bitpos=\"0\" byteorder=\"LE\" />");
    strStructure.append("</struct>");
    strStructure.append("<struct type=\"tTest\" alignment=\"1\" version=\"2\">");
    strStructure.append("<element name=\"arraySize\"  arraysize=\"1\" type=\"tUInt8\" "
                        "alignment=\"1\" bytepos=\"0\" bitpos=\"0\" byteorder=\"LE\" />");
    strStructure.append(
        "<element name=\"dynamicArray\"  arraysize=\"arraySize\" type=\"tInnerStruct\" "
        "alignment=\"1\" bytepos=\"1\" bitpos=\"0\" byteorder=\"LE\" />");
    strStructure.append("</struct>");
    strStructure.append("</structs>");

    uint8_t ui8ArraySize = 0;
    int nRealSize = sizeof(uint8_t);

    codec::CodecFactory oFactory("tTest", strStructure.c_str());
    codec::Codec oCodec = oFactory.makeCodecFor(&ui8ArraySize, sizeof(nRealSize));
    a_util::memory::MemoryBuffer oWriteBuffer;
    ASSERT_EQ(a_util::result::SUCCESS, codec::transformToBuffer(oCodec, oWriteBuffer));

    oCodec = oFactory.makeCodecFor(oWriteBuffer.getPtr(), oWriteBuffer.getSize(), serialized);
    a_util::memory::MemoryBuffer oReadBuffer;
    ASSERT_EQ(a_util::result::SUCCESS, codec::transformToBuffer(oCodec, oReadBuffer));

    oCodec = oCodec.makeCodecFor(oReadBuffer.getPtr(), oReadBuffer.getSize(), deserialized);

    ASSERT_EQ(oReadBuffer.getSize(), 1U);
    ASSERT_EQ(((uint8_t*)oReadBuffer.getPtr())[0], 0U);
}
