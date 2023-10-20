/**
 * @file
 * Description is missing.
 *
 * Copyright @ 2022 VW Group. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

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

    CodecFactory oFactory_v20("tAlignedStructArray", strDDLv20.c_str());
    CodecFactory oFactory_v30("tAlignedStructArray", strDDLv30.c_str());
    Codec oCodec_v20 = oFactory_v20.makeCodecFor(&sArray_v20, sizeof(sArray_v20));
    Codec oCodec_v30 = oFactory_v30.makeCodecFor(&sArray_v30, sizeof(sArray_v30));

    CheckAlignedStructArrayCoderAccess<tAlignedStructArray_v20, tAlignedStruct_v20>(
        oCodec_v20, &sArray_v20, true);
    CheckAlignedStructArrayCoderAccess<tAlignedStructArray_v30, tAlignedStruct_v30>(
        oCodec_v30, &sArray_v30, true);

    CheckAlignedStructArrayCoderArrayAccess<tAlignedStructArray_v20, tAlignedStruct_v20>(
        oCodec_v20, &sArray_v20);
    CheckAlignedStructArrayCoderArrayAccess<tAlignedStructArray_v30, tAlignedStruct_v30>(
        oCodec_v30, &sArray_v30);
}
