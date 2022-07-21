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

// helper function to compare string arrays
bool compare(char** result, const char* target[], size_t count)
{
    size_t i, j;
    for (i = 0; i < count; ++i) {
        for (j = 0; j < count; ++j) {
            if (!strcmp(result[i], target[j])) {
                break; // hit: continue search
            }
        }
        if (j == count) {
            return false; // no hit: abort search
        }
    }
    return true; // all found: finished search
}

// load ddl from file
TEST_F(ddlCapiFixture, loadDDLfromFile)
{
    ASSERT_NE(ddl, nullptr);
    const char* error = ddl_capi_last_error(ddl);
    EXPECT_STREQ(error, "");
}

// write ddl to string
TEST_F(ddlCapiFixture, writeDDLtoString)
{
    char* xml;
    int32_t res = ddl_capi_ddl_tostring(ddl, &xml);
    EXPECT_FALSE(res);

    // free string of ddl
    res = ddl_capi_string_free(&xml);
    EXPECT_FALSE(res);
}

// get struct names from ddl
TEST_F(ddlCapiFixture, getStructNames)
{
    size_t count;
    char** names;
    int32_t res = ddl_capi_structs(ddl, &count, &names);
    const char* structs[] = {"tFEP_Driver_DriverCtrl",
                             "tFEP_Driver_LongitudinalControl",
                             "tFEP_Driver_LateralControl",
                             "tFEP_Driver_LightControl",
                             "tFEP_Driver_WiperControl",
                             "tFEP_Driver_AccControl",
                             "tFEP_Driver_AalaControl",
                             "tFEP_Driver_AdditionalControl",
                             "tDynStruct"};
    ASSERT_FALSE(res);
    EXPECT_EQ(count, sizeof(structs) / sizeof(*structs));
    EXPECT_TRUE(compare(names, structs, count));

    // free struct names of ddl
    res = ddl_capi_stringarray_free(ddl, count, &names);
    EXPECT_FALSE(res);
}

// get alignment for a struct
TEST_F(ddlCapiFixture, getStructAlignment)
{
    size_t alignment;
    int32_t res = ddl_capi_struct_data(ddl, "tFEP_Driver_WiperControl", &alignment);
    ASSERT_FALSE(res);
    EXPECT_EQ(alignment, 1U);
}

// get element names of a struct
TEST_F(ddlCapiFixture, getElementNames)
{
    size_t count;
    char** names;
    int32_t res = ddl_capi_struct_elements(ddl, "tFEP_Driver_WiperControl", &count, &names);
    const char* elements[] = {"eFrontState", "eRearState", "bWashFront", "bWashRear", "ui8Spare"};
    ASSERT_FALSE(res);
    EXPECT_EQ(count, sizeof(elements) / sizeof(*elements));
    EXPECT_TRUE(compare(names, elements, count));

    // free element names of ddl struct
    res = ddl_capi_stringarray_free(ddl, count, &names);
    EXPECT_FALSE(res);
}

// get data of a struct element by name
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
                                        "ui8Spare",
                                        &type_name,
                                        &typeOfType,
                                        &arraysize,
                                        &bytepos,
                                        &bitpos,
                                        &alignment,
                                        &byteorder);
    ASSERT_FALSE(res);
    EXPECT_STREQ(type_name, "tUInt8");
    EXPECT_EQ(typeOfType, data_type);
    EXPECT_EQ(arraysize, 4);
    EXPECT_EQ(bytepos, 4U);
    EXPECT_EQ(alignment, 1U);

    // free string of ddl element type
    res = ddl_capi_string_free(&type_name);
    EXPECT_FALSE(res);
}

// get data of a struct element by name (enumtype)
TEST_F(ddlCapiFixture, getElementDataEnum)
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
                                        "eRearState",
                                        &type_name,
                                        &typeOfType,
                                        &arraysize,
                                        &bytepos,
                                        &bitpos,
                                        &alignment,
                                        &byteorder);
    ASSERT_FALSE(res);
    EXPECT_STREQ(type_name, "tFEP_Driver_WiperState");
    EXPECT_EQ(typeOfType, enum_type);
    EXPECT_EQ(arraysize, 1);
    EXPECT_EQ(bytepos, 1U);
    EXPECT_EQ(alignment, 1U);

    // free string of ddl element type
    res = ddl_capi_string_free(&type_name);
    EXPECT_FALSE(res);
}

// get data for a type by name
TEST_F(ddlCapiFixture, getTypeData)
{
    bool predefined;
    size_t numbits;
    size_t arraysize;
    int32_t res = ddl_capi_type_data(ddl, "tUInt8", &predefined, &numbits, &arraysize);
    ASSERT_FALSE(res);
    EXPECT_TRUE(predefined);
    EXPECT_EQ(numbits, 8U);
    EXPECT_EQ(arraysize, 1);
}

// get data of a enum by name
TEST_F(ddlCapiFixture, getEnumData)
{
    char* type;
    bool predefined;
    size_t count;
    char** entry_names;
    char** entry_values;
    int32_t res = ddl_capi_enum_data(
        ddl, "tFEP_Driver_WiperState", &type, &predefined, &count, &entry_names, &entry_values);
    const char* names[] = {"WS_Off", "WS_Auto", "WS_Interval", "WS_OnStandard", "WS_OnFast"};
    const char* values[] = {"0", "1", "2", "3", "4"};
    ASSERT_FALSE(res);
    EXPECT_STREQ(type, "tUInt8");
    EXPECT_EQ(count, sizeof(names) / sizeof(*names));
    EXPECT_TRUE(compare(entry_names, names, count));
    EXPECT_TRUE(compare(entry_values, values, count));

    // free string of ddl enum type
    res = ddl_capi_string_free(&type);
    EXPECT_FALSE(res);
    // free enum element names of ddl enum
    res = ddl_capi_stringarray_free(ddl, count, &entry_names);
    EXPECT_FALSE(res);
    // free enum element values of ddl enum
    res = ddl_capi_stringarray_free(ddl, count, &entry_values);
    EXPECT_FALSE(res);
}

// combined test of ddl handling (due to dependencies)
TEST_F(ddlCapiFixture, ddlHandling)
{
    // get minimalistic ddl for specified type (getMinimalisticDDL)
    char* xml;
    int32_t res = ddl_capi_resolve_type(ddl, "tFEP_Driver_WiperControl", &xml);
    ASSERT_FALSE(res);

    // load ddl from xml string (loadDDLfromString)
    DDL_CAPI_Handle_T ddl_xml;
    res = ddl_capi_load_from_string(xml, &ddl_xml);
    EXPECT_FALSE(res);
    // free string of minimalistic ddl
    res = ddl_capi_string_free(&xml);
    EXPECT_FALSE(res);

    // save ddl to file (saveDDLtoFile)
    res = ddl_capi_ddl_tofile(ddl_xml, OUTPUT_DIR "type.description");
    EXPECT_FALSE(res);
    // free handle of minimalistic ddl
    res = ddl_capi_free(&ddl_xml);
    EXPECT_FALSE(res);
}

// #################### codec tests ####################

// prepare for encoding a struct
TEST_F(ddlCapiCodecFixture, prepareEncoding)
{
    ASSERT_NE(data, nullptr);
    ASSERT_NE(codec, nullptr);
    const char* error = ddl_capi_last_codecerror(codec);
    EXPECT_STREQ(error, "");
}

#define ELEMENTINDEX 1ULL
// get index of an element
TEST_F(ddlCapiCodecFixture, getElementIndex)
{
    size_t index;
    int32_t res = ddl_capi_codec_get_element_index(codec, "eRearState", &index);
    ASSERT_FALSE(res);
    EXPECT_EQ(index, ELEMENTINDEX);
}

// combined test of element handling by index (due to dependencies)
TEST_F(ddlCapiCodecFixture, handleElementByIndex)
{
    // set data of element by index (setElementByIndex)
    uint8_t rearState = 2;
    int32_t res = ddl_capi_codec_set_element_byIndex(
        codec, ELEMENTINDEX, &rearState); // use index from test 'getElementIndex'
    EXPECT_FALSE(res);

    // get data of element by index (getElementByIndex)
    uint8_t _rearState;
    res = ddl_capi_codec_get_element_byIndex(codec, ELEMENTINDEX, &_rearState);
    ASSERT_FALSE(res);
    EXPECT_EQ(rearState, _rearState);
}

// combined test of element handling by name (due to dependencies)
TEST_F(ddlCapiCodecFixture, handleElementByName)
{
    // set data of element by name (setElementByName)
    uint8_t washFront = 1;
    int32_t res = ddl_capi_codec_set_element_byName(codec, "bWashFront", &washFront);
    EXPECT_FALSE(res);

    // get data of element by name (getElementByName)
    uint8_t _washFront;
    res = ddl_capi_codec_get_element_byName(codec, "bWashFront", &_washFront);
    ASSERT_FALSE(res);
    EXPECT_EQ(washFront, _washFront);
}

#define ARRAYINDEX 4U
// get the index of the array
TEST_F(ddlCapiCodecFixture, getArrayIndex)
{
    size_t index;
    int32_t res = ddl_capi_codec_get_array_index(codec, "ui8Spare", &index);
    ASSERT_FALSE(res);
    EXPECT_EQ(index, ARRAYINDEX);
}

#define ARRAYSIZE 4U
// get the size of the array
TEST_F(ddlCapiCodecFixture, getArraySize)
{
    size_t size;
    int32_t res = ddl_capi_codec_get_array_size(codec, "ui8Spare", &size);
    ASSERT_FALSE(res);
    EXPECT_EQ(size, ARRAYSIZE);
}

// combined test of array data handling by index (due to dependencies)
TEST_F(ddlCapiCodecFixture, handleArrayDataByIndex)
{
    // set array data by index (setArrayByIndex)
    uint8_t array_data[] = {11, 12, 13, 14};
    int32_t res = ddl_capi_codec_set_array_byIndex(
        codec, ARRAYINDEX, array_data, ARRAYSIZE); // use index from test 'getArrayIndex'
    EXPECT_FALSE(res);                             // use data_size from test 'getArraySize'

    // get array data by index (getArrayByIndex)
    uint8_t _array_data[4];
    res = ddl_capi_codec_get_array_byIndex(codec, ARRAYINDEX, _array_data, ARRAYSIZE);
    ASSERT_FALSE(res);
    EXPECT_EQ(memcmp(array_data, _array_data, 4), 0);
}

// combined test of array data handling by name (due to dependencies)
TEST_F(ddlCapiCodecFixture, handleArrayDataByName)
{
    // set array data by name (setArrayByName)
    uint8_t array_data[] = {11, 12, 13, 14};
    int32_t res = ddl_capi_codec_set_array_byName(codec, "ui8Spare", array_data, ARRAYSIZE);
    EXPECT_FALSE(res);

    // get array data by name (getArrayByName)
    uint8_t _array_data[4];
    res = ddl_capi_codec_get_array_byName(codec, "ui8Spare", _array_data, ARRAYSIZE);
    ASSERT_FALSE(res);
    EXPECT_EQ(memcmp(array_data, _array_data, 4), 0);
}

// write all elements from source to destination
TEST_F(ddlCapiCodecFixture, copyCodecData)
{
    // create a codec for destination struct
    void* dest_data = malloc(data_size);
    DDL_CAPI_Codec_Handle_T dest;
    int32_t res = ddl_capi_codec_create(
        ddl, "tFEP_Driver_WiperControl", dest_data, data_size, DDL_CAPI_Data_deserialized, &dest);

    // execute the prepared testcase
    const char* testStr = "blahblah";
    memmove(data, testStr, data_size);
    res = ddl_capi_codec_transform(dest, codec);
    ASSERT_FALSE(res);
    EXPECT_EQ(memcmp(dest_data, testStr, data_size), 0);

    // free codec handles and memory blocks
    res = ddl_capi_codec_free(&dest);
    EXPECT_FALSE(res);
    free(dest_data);
}

#define STRUCTINDEX 29U
// get index of a substruct
TEST_F(ddlCapiSubstructFixture, getStructIndex)
{
    size_t index;
    int32_t res = ddl_capi_codec_get_substruct_index(superior, "sWiperControl", &index);
    ASSERT_FALSE(res);
    EXPECT_EQ(index, STRUCTINDEX);
}

// combined test of substruct data handling by index (due to dependencies)
TEST_F(ddlCapiSubstructFixture, handleStructDataByIndex)
{
    // set substruct data by index (setStructByIndex)
    const char* testStr = "hlabhlab";
    memmove(data, testStr, data_size);
    int32_t res = ddl_capi_codec_set_substruct_byIndex(superior, STRUCTINDEX, data, data_size);
    EXPECT_FALSE(res);

    // get substruct data by index (getStructByIndex)
    uint8_t _data[8];
    res = ddl_capi_codec_get_substruct_byIndex(superior, STRUCTINDEX, _data, data_size);
    ASSERT_FALSE(res);
    EXPECT_EQ(memcmp(_data, testStr, data_size), 0);
}

// combined test of substruct data handling by name (due to dependencies)
TEST_F(ddlCapiSubstructFixture, handleStructDataByName)
{
    // set data of substruct by name (setStructByName)
    const char* testStr = "labhlabh";
    memmove(data, testStr, data_size);
    int32_t res = ddl_capi_codec_set_substruct_byName(superior, "sWiperControl", data, data_size);
    EXPECT_FALSE(res);

    // get data of substruct by name (getStructByName)
    uint8_t _data[8];
    res = ddl_capi_codec_get_substruct_byName(superior, "sWiperControl", _data, data_size);
    ASSERT_FALSE(res);
    EXPECT_EQ(memcmp(_data, testStr, data_size), 0);
}
