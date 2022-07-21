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

#include <ddl_capi/ddl_capi.h>

#include <gtest/gtest.h>

// basic fixture for api tests
class ddlCapiFixture : public ::testing::Test {
public:
    static void SetUpTestSuite()
    {
        // obtain handle and load ddl from file
        int32_t result = ddl_capi_load_from_file(INPUT_DIR "test.description", &ddl);
        EXPECT_FALSE(result);
    }

    static void TearDownTestSuite()
    {
        int32_t result = ddl_capi_free(&ddl);
        EXPECT_FALSE(result);
    }

    static DDL_CAPI_Handle_T ddl;
};
DDL_CAPI_Handle_T ddlCapiFixture::ddl = nullptr;

// derived fixture for codec tests
class ddlCapiCodecFixture : public ddlCapiFixture {
public:
    static void SetUpTestSuite()
    {
        ddlCapiFixture::SetUpTestSuite();

        // get size of struct for encoding (getStructSize)
        int32_t result = ddl_capi_struct_buffer_size(
            ddl, "tFEP_Driver_WiperControl", DDL_CAPI_Data_deserialized, &data_size);
        EXPECT_FALSE(result);

        // create a codec for a struct (createStructCodec)
        data = malloc(data_size);
        result = ddl_capi_codec_create(
            ddl, "tFEP_Driver_WiperControl", data, data_size, DDL_CAPI_Data_deserialized, &codec);
        EXPECT_FALSE(result);
    }

    static void TearDownTestSuite()
    {
        // free codec handles and memory blocks
        int32_t result = ddl_capi_codec_free(&codec);
        EXPECT_FALSE(result);
        free(data);

        ddlCapiFixture::TearDownTestSuite();
    }

    static void* data;
    static size_t data_size;
    static DDL_CAPI_Codec_Handle_T codec;
};
size_t ddlCapiCodecFixture::data_size;
void* ddlCapiCodecFixture::data = nullptr;
DDL_CAPI_Codec_Handle_T ddlCapiCodecFixture::codec = nullptr;

// derived fixture for substruct tests
class ddlCapiSubstructFixture : public ddlCapiCodecFixture {
    static void* parent_data;

public:
    static void SetUpTestSuite()
    {
        ddlCapiCodecFixture::SetUpTestSuite();

        // get size of superior struct
        size_t parent_size;
        ddl_capi_struct_buffer_size(
            ddl, "tFEP_Driver_DriverCtrl", DDL_CAPI_Data_deserialized, &parent_size);

        // create a codec for superior struct
        parent_data = malloc(parent_size);
        ddl_capi_codec_create(ddl,
                              "tFEP_Driver_DriverCtrl",
                              parent_data,
                              parent_size,
                              DDL_CAPI_Data_deserialized,
                              &superior);
    }

    static void TearDownTestSuite()
    {
        // free codec handles and memory blocks
        ddl_capi_codec_free(&superior);
        free(parent_data);

        ddlCapiCodecFixture::TearDownTestSuite();
    }

    static DDL_CAPI_Codec_Handle_T superior;
};
void* ddlCapiSubstructFixture::parent_data;
DDL_CAPI_Codec_Handle_T ddlCapiSubstructFixture::superior;
