/**
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

#include "test_fixture.h"

#include <gmock/gmock.h>

// load ddl from file - missing file
TEST(negative_tests, loadDDLfromMissingFile)
{
    DDL_CAPI_Handle_T ddl;
    int32_t res = ddl_capi_load_from_file(INPUT_DIR "missing.description", &ddl);
    ASSERT_TRUE(res);
    const char* error = ddl_capi_last_error(ddl);
    EXPECT_THAT(error, testing::EndsWith("File was not found")); //
}

// load ddl from file - corrupt file
TEST(negative_tests, loadDDLfromCorruptFile)
{
    DDL_CAPI_Handle_T ddl;
    int32_t res = ddl_capi_load_from_file(INPUT_DIR "corrupt.description", &ddl);
    ASSERT_TRUE(res);
    const char* error = ddl_capi_last_error(ddl);
    EXPECT_THAT(error, testing::EndsWith("See validation protocol!"));
}

// write ddl to string - invalid ddl
TEST(negative_tests, writeDDLtoString)
{
    char* xml;
    DDL_CAPI_Handle_T ddl = nullptr;
    int32_t res = ddl_capi_ddl_tostring(ddl, &xml);
    ASSERT_TRUE(res);
    const char* error = ddl_capi_last_error(ddl);
    EXPECT_STREQ(error, "Invalid ddl handle");
}

// write ddl to file - invalid path
TEST_F(ddlCapiFixture, writeDDLtoFile)
{
    int32_t res = ddl_capi_ddl_tofile(ddl, "d://type.description");
    ASSERT_TRUE(res);
    const char* error = ddl_capi_last_error(ddl);
    EXPECT_STREQ(error, "writeToFile(xml_filepath): Failed to save dom to file");
}

// get alignment for a struct - unknown struct
TEST_F(ddlCapiFixture, getStructAlignmentUnknown)
{
    size_t alignment;
    int32_t res = ddl_capi_struct_data(ddl, "unknown_struct", &alignment);
    ASSERT_TRUE(res);
    const char* error = ddl_capi_last_error(ddl);
    EXPECT_STREQ(error, "Struct not found");
}

// get alignment for a struct - wrong type
TEST_F(ddlCapiFixture, getStructAlignmentWrong)
{
    size_t alignment;
    int32_t res = ddl_capi_struct_data(ddl, "tFEP_Driver_WiperState", &alignment);
    ASSERT_TRUE(res);
    const char* error = ddl_capi_last_error(ddl);
    EXPECT_STREQ(error, "Struct not found");
}

// get data of a struct element by name - unknown element
TEST_F(ddlCapiFixture, getElementData)
{
    char* type_name;
    type_t typeOfType;
    size_t arraysize;
    size_t bytepos;
    size_t bitpos;
    size_t alignment;
    size_t byteorder;
    int32_t res = ddl_capi_element_data(ddl,
                                        "tFEP_Driver_WiperControl",
                                        "unknown_element",
                                        &type_name,
                                        &typeOfType,
                                        &arraysize,
                                        &bytepos,
                                        &bitpos,
                                        &alignment,
                                        &byteorder);
    ASSERT_TRUE(res);
    const char* error = ddl_capi_last_error(ddl);
    EXPECT_STREQ(error, "Element not found");
}

// get data for a type by name - unknown type
TEST_F(ddlCapiFixture, getTypeData)
{
    bool predefined;
    size_t numbits;
    size_t arraysize;
    int32_t res = ddl_capi_type_data(ddl, "unknown_type", &predefined, &numbits, &arraysize);
    ASSERT_TRUE(res);
    const char* error = ddl_capi_last_error(ddl);
    EXPECT_STREQ(error, "Type not found");
}

// get data of an enum by name - unknown enum
TEST_F(ddlCapiFixture, getEnumData)
{
    char* type;
    bool predefined;
    size_t count;
    char** entry_names;
    char** entry_values;
    int32_t res = ddl_capi_enum_data(
        ddl, "unknown_enum", &type, &predefined, &count, &entry_names, &entry_values);
    ASSERT_TRUE(res);
    const char* error = ddl_capi_last_error(ddl);
    EXPECT_STREQ(error, "Enum not found");
}

// free string array - invalid array
TEST_F(ddlCapiFixture, freeStringArray)
{
    char** xml = nullptr;
    int32_t res = ddl_capi_stringarray_free(ddl, 5, &xml);
    ASSERT_TRUE(res);
    const char* error = ddl_capi_last_error(ddl);
    EXPECT_STREQ(error, "Invalid array location");
}

// get minimalistic ddl for specified type - unknown type
TEST_F(ddlCapiFixture, resolveType)
{
    char* xml;
    int32_t res = ddl_capi_resolve_type(ddl, "unknown_type", &xml);
    ASSERT_TRUE(res);
    const char* error = ddl_capi_last_error(ddl);
    EXPECT_STREQ(error, "Struct not found");
}

// load ddl from xml string - invalid ddl string
TEST(negative_tests, loadDDLfromString)
{
    char* xml = nullptr;
    DDL_CAPI_Handle_T ddl_xml;
    int32_t res = ddl_capi_load_from_string(xml, &ddl_xml);
    ASSERT_TRUE(res);
    const char* error = ddl_capi_last_error(ddl_xml);
    EXPECT_STREQ(error, "Invalid xml string");
}

// #################### codec tests ####################

// get size of struct for encoding - unknown struct
TEST_F(ddlCapiFixture, getStructSize)
{
    size_t data_size;
    int32_t res =
        ddl_capi_struct_buffer_size(ddl, "unknown_struct", DDL_CAPI_Data_deserialized, &data_size);
    ASSERT_TRUE(res);
    const char* error = ddl_capi_last_error(ddl);
    EXPECT_STREQ(error, "Struct not found");
}

// create a codec for a struct - unknown struct
#define DATASIZE 8
TEST_F(ddlCapiFixture, createStructCodec)
{
    DDL_CAPI_Codec_Handle_T codec;
    void* data = malloc(DATASIZE);
    int32_t res = ddl_capi_codec_create(
        ddl, "unknown_struct", data, DATASIZE, DDL_CAPI_Data_deserialized, &codec);
    ASSERT_TRUE(res);
    const char* error = ddl_capi_last_error(ddl);
    EXPECT_STREQ(error, "Struct not found");
}

// get index of element - invalid codec
TEST_F(ddlCapiCodecFixture, invalidCodec)
{
    size_t index;
    DDL_CAPI_Codec_Handle_T invalidCodec = nullptr;
    int32_t res = ddl_capi_codec_get_element_index(invalidCodec, "eRearState", &index);
    ASSERT_TRUE(res);
    const char* error = ddl_capi_last_codecerror(invalidCodec);
    EXPECT_STREQ(error, "Invalid codec handle");
}

// get index of element - unknwon element
TEST_F(ddlCapiCodecFixture, getElementIndex)
{
    size_t index;
    int32_t res = ddl_capi_codec_get_element_index(codec, "unknown_element", &index);
    ASSERT_TRUE(res);
    const char* error = ddl_capi_last_codecerror(codec);
    EXPECT_STREQ(error, "Result code '-20 '(ERR_NOT_FOUND) -  [File: ] [Line: -1] [Func: ]");
}

// set data of element by index - invalid index
TEST_F(ddlCapiCodecFixture, setElementByIndex)
{
    uint8_t rearState = 2;
    int32_t res = ddl_capi_codec_set_element_byIndex(codec, 100, &rearState);
    ASSERT_TRUE(res);
    const char* error = ddl_capi_last_codecerror(codec);
    EXPECT_STREQ(error, "Result code '-10 '(ERR_INVALID_INDEX) -  [File: ] [Line: -1] [Func: ]");
}

// get data of element by index - invalid index
TEST_F(ddlCapiCodecFixture, getElementByIndex)
{
    uint8_t rearState = 2;
    int32_t res = ddl_capi_codec_get_element_byIndex(codec, 100, &rearState);
    ASSERT_TRUE(res);
    const char* error = ddl_capi_last_codecerror(codec);
    EXPECT_STREQ(error, "Result code '-10 '(ERR_INVALID_INDEX) -  [File: ] [Line: -1] [Func: ]");
}

// set data of element by name - invalid name
TEST_F(ddlCapiCodecFixture, setElementByName)
{
    uint8_t rearState = 2;
    int32_t res = ddl_capi_codec_set_element_byName(codec, "unknown_element", &rearState);
    ASSERT_TRUE(res);
    const char* error = ddl_capi_last_codecerror(codec);
    EXPECT_STREQ(error, "Result code '-20 '(ERR_NOT_FOUND) -  [File: ] [Line: -1] [Func: ]");
}

// get the index of the array - unknown array
TEST_F(ddlCapiCodecFixture, getArrayIndex)
{
    size_t index;
    int32_t res = ddl_capi_codec_get_array_index(codec, "unknown_array", &index);
    ASSERT_TRUE(res);
    const char* error = ddl_capi_last_codecerror(codec);
    EXPECT_STREQ(error, "Result code '-20 '(ERR_NOT_FOUND) -  [File: ] [Line: -1] [Func: ]");
}

// set data of array by index - invalid index
TEST_F(ddlCapiCodecFixture, setArrayByInvalidIndex)
{
    uint8_t array_data[] = {11, 12, 13, 14};
    int32_t res = ddl_capi_codec_set_array_byIndex(codec, 40, array_data, 4);
    ASSERT_TRUE(res);
    const char* error = ddl_capi_last_codecerror(codec);
    EXPECT_STREQ(error, "Array not found");
}

// set data of array by index - invalid array
TEST_F(ddlCapiCodecFixture, setInvalidArrayByIndex)
{
    uint8_t* array_data = nullptr;
    int32_t res = ddl_capi_codec_set_array_byIndex(codec, 4, array_data, 4);
    ASSERT_TRUE(res);
    const char* error = ddl_capi_last_codecerror(codec);
    EXPECT_STREQ(error, "Copy array failed");
}

// set data of array by name - invalid name
TEST_F(ddlCapiCodecFixture, setInvalidArrayByName)
{
    uint8_t* array_data = nullptr;
    int32_t res = ddl_capi_codec_set_array_byName(codec, "unknown_array", array_data, 4);
    ASSERT_TRUE(res);
    const char* error = ddl_capi_last_codecerror(codec);
    EXPECT_STREQ(error, "Result code '-20 '(ERR_NOT_FOUND) -  [File: ] [Line: -1] [Func: ]");
}

// write all elements from source to destination - invalid codec
TEST_F(ddlCapiCodecFixture, copyCodecData)
{
    DDL_CAPI_Codec_Handle_T dest = nullptr;
    int32_t res = ddl_capi_codec_transform(dest, codec);
    ASSERT_TRUE(res);
    const char* error = ddl_capi_last_codecerror(dest);
    EXPECT_STREQ(error, "Invalid codec handle");
}

// get index of substruct - invalid substruct
TEST_F(ddlCapiSubstructFixture, getStructIndex)
{
    size_t index;
    int32_t res = ddl_capi_codec_get_substruct_index(superior, "unknown_substruct", &index);
    ASSERT_TRUE(res);
    const char* error = ddl_capi_last_codecerror(codec);
    EXPECT_STREQ(error, "");
}

// set data of substruct by name - invalid name
TEST_F(ddlCapiSubstructFixture, setSubstructByName)
{
    int32_t res = ddl_capi_codec_set_substruct_byName(superior, "unknown_struct", data, data_size);
    ASSERT_TRUE(res);
    const char* error = ddl_capi_last_codecerror(codec);
    EXPECT_STREQ(error, "");
}
