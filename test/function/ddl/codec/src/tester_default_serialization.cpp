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

#include "tester_default_serialization.h"

#include "ddl/codec/codec_factory.h"
#include "ddl/dd/dd.h"
#include "ddl/dd/ddfile.h"

#include <gtest/gtest.h>

using namespace ddl;

template <typename STRUCT, typename STRUCT_PACKED>
void testSizes(const std::string& name_of_struct, const std::string& name_of_struct_packed)
{
    std::string test_filename = TEST_FILES_DIR "/test_default_serialization.description";
    dd::DataDefinition dd;
    ASSERT_NO_THROW(dd = DDFile::fromXMLFile(test_filename););

    const CodecFactory factory(*dd.getStructTypes().get(name_of_struct), dd);
    ASSERT_EQ(factory.getStaticBufferSize(ddl::deserialized), sizeof(STRUCT));
    ASSERT_EQ(factory.getStaticBufferSize(ddl::serialized), sizeof(STRUCT_PACKED));

    const CodecFactory factory_packed(*dd.getStructTypes().get(name_of_struct_packed), dd);
    ASSERT_EQ(factory_packed.getStaticBufferSize(ddl::deserialized), sizeof(STRUCT_PACKED));
    ASSERT_EQ(factory_packed.getStaticBufferSize(ddl::serialized), sizeof(STRUCT_PACKED));
}

TEST(CodecTestSerialization, TestSizes)
{
    testSizes<tExample00, pack_1::tExample00>("tExample00", "pack_1::tExample00");
    testSizes<tExample01, pack_1::tExample01>("tExample01", "pack_1::tExample01");
    testSizes<tExample02, pack_1::tExample02>("tExample02", "pack_1::tExample02");
    testSizes<tExample03, pack_1::tExample03>("tExample03", "pack_1::tExample03");
    testSizes<tExample04, pack_1::tExample04>("tExample04", "pack_1::tExample04");
    testSizes<tExample05, pack_1::tExample05>("tExample05", "pack_1::tExample05");
    testSizes<tExample06, pack_1::tExample06>("tExample06", "pack_1::tExample06");
    testSizes<tExample07, pack_1::tExample07>("tExample07", "pack_1::tExample07");
    testSizes<tExample08, pack_1::tExample08>("tExample08", "pack_1::tExample08");
    testSizes<tExample09, pack_1::tExample09>("tExample09", "pack_1::tExample09");
    testSizes<tExample10, pack_1::tExample10>("tExample10", "pack_1::tExample10");
    testSizes<tExample11, pack_1::tExample11>("tExample11", "pack_1::tExample11");
    testSizes<tExample12, pack_1::tExample12>("tExample12", "pack_1::tExample12");
    testSizes<tExample13, pack_1::tExample13>("tExample13", "pack_1::tExample13");
    testSizes<tExample14, pack_1::tExample14>("tExample14", "pack_1::tExample14");
    testSizes<tExample15, pack_1::tExample15>("tExample15", "pack_1::tExample15");
    testSizes<tExample16, pack_1::tExample16>("tExample16", "pack_1::tExample16");
    testSizes<tExample17, pack_1::tExample17>("tExample17", "pack_1::tExample17");
    testSizes<tExample18, pack_1::tExample18>("tExample18", "pack_1::tExample18");
    testSizes<tExample19, pack_1::tExample19>("tExample19", "pack_1::tExample19");
    testSizes<tExample20, pack_1::tExample20>("tExample20", "pack_1::tExample20");
    testSizes<tExample21, pack_1::tExample21>("tExample21", "pack_1::tExample21");
    testSizes<tExample22, pack_1::tExample22>("tExample22", "pack_1::tExample22");
    testSizes<tExample23, pack_1::tExample23>("tExample23", "pack_1::tExample23");
}
