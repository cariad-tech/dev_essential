/**
 * @file
 * Test implementation.
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
 */

#include "../../_common/adtf_compat.h"

#define DEV_ESSENTIAL_DISABLE_DEPRECATED_WARNINGS

#include <a_util/system.h>
#include <ddl/codec/codec_factory.h>
#include <ddl/dd/ddfile.h>
#include <ddl/serialization/serialization.h>

#include <gtest/gtest.h>

#include <chrono>
#include <list>
#if (defined(__GNUC__) && (__GNUC__ < 7)) || (defined(_MSC_VER) && (_MSC_VER < 1920))
#define NO_VARIANT_TEST
#else
#include <variant>
#endif

using namespace ddl;

void dumpElements(const codec::StaticDecoder& decoder)
{
    auto first_element_address =
        static_cast<const uint8_t*>(decoder.getElements().begin()->getAddress());
    codec::forEachLeafElement(decoder.getElements(), [&first_element_address](auto& element) {
        std::cout << element.getFullName() << " @ "
                  << static_cast<const uint8_t*>(element.getAddress()) - first_element_address
                  << ": " << element.getStringValue() << "\n";
    });
}

namespace static_struct {
struct ChildStruct {
    int8_t value_dummy;
    int32_t value[3];
    int8_t after;
};

struct TestStruct {
    ChildStruct child[2];
};

const char* test_description =
    R"(<?xml version="1.0" encoding="iso-8859-1" standalone="no"?>
    <structs>
        <struct alignment="4" name="child_struct" version="2">
            <element alignment="1" arraysize="1" byteorder="BE" bytepos="0" name="value_dummy" type="tInt8"/>
            <element alignment="4" arraysize="3" byteorder="BE" bytepos="1" name="value" type="tInt32"/>
            <element alignment="1" arraysize="1" byteorder="BE" bytepos="13" name="after" type="tInt8"/>
        </struct>
        <struct alignment="4" name="test" version="2">
            <element alignment="4" arraysize="2" byteorder="LE" bytepos="0" name="child" type="child_struct"/>
        </struct>
    </structs>)";

const TestStruct test_data = {{
    {0x01, {0x02, 0x03, 0x04}, 0x05},
    {0x06, {0x07, 0x08, 0x09}, 0x0A},
}};

namespace serialized {
#pragma pack(push, 1)

struct ChildStruct {
    int8_t value_dummy;
    int32_t value[3];
    int8_t after;
};

struct TestStruct {
    ChildStruct child[2];
};

const TestStruct test_data = {{
    {0x01, {0x02000000, 0x03000000, 0x04000000}, 0x05},
    {0x06, {0x07000000, 0x08000000, 0x09000000}, 0x0A},
}};
#pragma pack(pop)
} // namespace serialized

} // namespace static_struct

template <typename T>
void test_static(codec::CodecFactory& factory, const T& test_data, DataRepresentation eRep)
{
    std::cout << a_util::strings::format(
                     "%d == %d", factory.getStaticBufferSize(eRep), sizeof(test_data))
              << std::endl;
    ASSERT_EQ(factory.getStaticBufferSize(eRep), sizeof(test_data));
    T test = test_data;
    codec::StaticDecoder decoder = factory.makeStaticDecoderFor(&test, sizeof(test), eRep);
    codec::StaticCodec codec = factory.makeStaticCodecFor(&test, sizeof(test), eRep);

    ASSERT_EQ(a_util::result::SUCCESS, factory.isValid());
    ASSERT_EQ(a_util::result::SUCCESS, decoder.isValid());
    ASSERT_EQ(a_util::result::SUCCESS, codec.isValid());

    ASSERT_EQ(decoder.getElementCount(), 10);
    dumpElements(decoder);

    ASSERT_EQ(decoder.getElement("child[0].value_dummy").template getValue<uint8_t>(), 1);
    ASSERT_EQ(decoder.getElement("child[0].value[0]").template getValue<int32_t>(), 2);
    ASSERT_EQ(decoder.getElement("child[0].value[1]").template getValue<int32_t>(), 3);
    ASSERT_EQ(decoder.getElement("child[0].value[2]").template getValue<int32_t>(), 4);

    // we do the same as above with the variant solution
    ASSERT_EQ(decoder.getElement("child[0].after").getVariantValue().getInt8(), 5);
    ASSERT_EQ(decoder.getElement("child[1].value_dummy").getVariantValue().getInt8(), 6);
    ASSERT_EQ(decoder.getElement("child[1].value[0]").getVariantValue().getInt32(), 7);
    ASSERT_EQ(decoder.getElement("child[1].value[1]").getVariantValue().getInt32(), 8);
    ASSERT_EQ(decoder.getElement("child[1].value[2]").getVariantValue().getInt32(), 9);
    ASSERT_EQ(decoder.getElement("child[1].after").getVariantValue().getInt8(), 10);

    ASSERT_EQ(decoder.getElement("child[0].value_dummy").getType(),
              ddl::codec::ElementType::cet_int8);
    ASSERT_EQ(decoder.getElement("child[0].value[0]").getType(),
              ddl::codec::ElementType::cet_int32);

    int32_t nNewData = 0x10;
    auto resolved_index = codec.resolve(1);
    ASSERT_NO_THROW(codec.setElementRawValue(resolved_index, &nNewData, sizeof(nNewData)));
    ASSERT_TRUE(0x10 == test.child[0].value[0] || 0x10000000 == test.child[0].value[0]);

    ASSERT_NO_THROW(codec.getElement("child[1].value[2]").template setValue<int32_t>(0x20););
    ASSERT_TRUE(0x20 == test.child[1].value[2] || 0x20000000 == test.child[1].value[2]);
}

/**
 * @detail Check deserialized codec static information
 * @req_id ACORE-7704 ACORE-7997 CDPKGDDL-12
 */
TEST(CodecTest, TestStatic)
{
    codec::CodecFactory factory("test", static_struct::test_description);

    test_static(factory, static_struct::test_data, deserialized);

    // just check some indices
    auto found_element_after = factory.getElement("child[0].after");
    auto found_index_after = found_element_after.getIndex();
    // two indices ... 2 level access
    ASSERT_EQ(found_index_after.getSize(), 2);

    ASSERT_EQ(found_index_after[0].getIndex(), 0);
    ASSERT_EQ(found_index_after[0].getArrayPos(), 0);

    ASSERT_EQ(found_index_after[1].getIndex(), 2);
    ASSERT_EQ(found_index_after[1].getArrayPos(), 0);

    auto found_element_struct = factory.getElement("child[1]");
    auto found_index_struct = found_element_struct.getIndex();
    // one index ... first level access
    ASSERT_EQ(found_index_struct.getSize(), 1);

    ASSERT_EQ(found_index_struct[0].getIndex(), 0);
    ASSERT_EQ(found_index_struct[0].getArrayPos(), 1);

    // check if no array value within the name gets the arraypos 0
    auto found_element_array_first = factory.getElement("child");
    auto found_index_array_first = found_element_array_first.getIndex();
    // one index ... first level access
    ASSERT_EQ(found_index_array_first.getSize(), 1);

    ASSERT_EQ(found_index_array_first[0].getIndex(), 0);
    ASSERT_EQ(found_index_array_first[0].getArrayPos(), 0);
}

/**
 * @detail  Check serialized codec static information
 * @req_id ACORE-7704 CDPKGDDL-13
 */
TEST(CodecTest, TestStaticSerialized)
{
    codec::CodecFactory factory("test", static_struct::test_description);
    test_static(factory, static_struct::serialized::test_data, serialized);
}

namespace simple {
struct MainStruct {
    int8_t array_size;
    int32_t array[4];
    int16_t after;
};

const char* test_description =
    R"(<?xml version="1.0" encoding="iso-8859-1" standalone="no"?>
    <structs>
        <struct alignment="4" name="main" version="2">
            <element alignment="1" arraysize="1" byteorder="LE" bytepos="0" name="array_size" type="tInt8"/>
            <element alignment="4" arraysize="array_size" byteorder="LE" bytepos="1" name="array" type="tInt32"/>
            <element alignment="2" arraysize="1" byteorder="LE" bytepos="-1" name="after" type="tInt16"/>
        </struct>
    </structs>)";

const MainStruct test_data = {4, {1, 2, 3, 4}, 8};

namespace serialized {
#pragma pack(push, 1)
struct MainStruct {
    int8_t array_size;
    int32_t array[4];
    int16_t after;
};

const MainStruct test_data = {4, {1, 2, 3, 4}, 8};
#pragma pack(pop)
} // namespace serialized

} // namespace simple

template <typename T>
void TestDynamicSimple(codec::CodecFactory& factory, const T& test_data, DataRepresentation eRep)
{
    codec::Decoder decoder = factory.makeDecoderFor(&test_data, sizeof(test_data), eRep);
    dumpElements(decoder);

    ASSERT_EQ(a_util::result::SUCCESS, factory.isValid());
    ASSERT_EQ(a_util::result::SUCCESS, decoder.isValid());

    ASSERT_EQ(sizeof(T), decoder.getBufferSize(eRep));
    std::cout << a_util::strings::toString((uint64_t)decoder.getElementCount()).c_str();
    ASSERT_EQ(decoder.getElementCount(), 6);

    ASSERT_EQ(decoder.getElement("array_size").getVariantValue().getInt8(), 4);
    ASSERT_EQ(decoder.getElement("array[0]").getVariantValue().getInt32(), 1);
    ASSERT_EQ(decoder.getElement("array[1]").getVariantValue().getInt32(), 2);
    ASSERT_EQ(decoder.getElement("array[2]").getVariantValue().getInt32(), 3);
    ASSERT_EQ(decoder.getElement("array[3]").getVariantValue().getInt32(), 4);
    ASSERT_EQ(decoder.getElement("after").getVariantValue().getInt16(), 8);
}

/**
 * @detail  Check deserialized codec dynamic information
 * @req_id ACORE-7704 CDPKGDDL-12
 */

TEST(CodecTest, TestDynamicSimple)
{
    codec::CodecFactory factory("main", simple::test_description);
    ::TestDynamicSimple(factory, simple::test_data, deserialized);
}

/**
 * @detail  Check serialized codec dynamic information
 * @req_id ACORE-7704 CDPKGDDL-13
 */

TEST(CodecTest, TestDynamicSimpleSerialized)
{
    codec::CodecFactory factory("main", simple::test_description);
    ::TestDynamicSimple(factory, simple::serialized::test_data, serialized);
}

namespace complex {
struct ArrayChildStruct {
    int8_t child_size;
    int32_t child_array[3];
    int8_t child_size2;
    int32_t child_array2[2];
    int16_t fixed_array[3];
};

struct tArrayTest {
    int8_t array_size;
    ArrayChildStruct array[2];
};

struct MainStruct {
    int8_t before;
    tArrayTest test;
    int8_t after;
};

const char* test_description =
    R"(<?xml version="1.0" encoding="iso-8859-1" standalone="no"?>
    <structs>
        <struct alignment="4" name="child_struct" version="2">
            <element alignment="1" arraysize="1" byteorder="LE" bytepos="0" name="child_size" type="tInt8"/>
            <element alignment="4" arraysize="child_size" byteorder="LE" bytepos="1" name="child_array" type="tInt32"/>
            <element alignment="1" arraysize="1" byteorder="LE" bytepos="-1" name="child_size2" type="tInt8"/>
            <element alignment="4" arraysize="child_size2" byteorder="LE" bytepos="-1" name="child_array2" type="tInt32"/>
            <element alignment="2" arraysize="3" byteorder="LE" bytepos="-1" name="fixed_array" type="tInt16"/> <!--// does not end at a multiple of 4 -->
        </struct>
        <struct alignment="4" name="test" version="2">
            <element alignment="1" arraysize="1" byteorder="LE" bytepos="0" name="array_size" type="tInt8"/>
            <element alignment="4" arraysize="array_size" byteorder="LE" bytepos="1" name="array" type="child_struct"/>
        </struct>
        <struct alignment="4" name="main" version="2">
            <element alignment="1" arraysize="1" byteorder="LE" bytepos="0" name="before" type="tInt8"/>
            <element alignment="4" arraysize="1" byteorder="LE" bytepos="1" name="test" type="test"/>
            <element alignment="1" arraysize="1" byteorder="LE" bytepos="-1" name="after" type="tInt8"/>
        </struct>
    </structs>)";

const MainStruct test_data = {4,
                              {2,
                               {{3, {1, 2, 3}, 2, {10, 20}, {40, 50, 60}},
                                {3, {11, 22, 33}, 2, {110, 220}, {400, 500, 600}}}},
                              8};

namespace serialized {
#pragma pack(push, 1)
struct ArrayChildStruct {
    int8_t child_size;
    int32_t child_array[3];
    int8_t child_size2;
    int32_t child_array2[2];
    int16_t fixed_array[3];
};

struct tArrayTest {
    int8_t array_size;
    ArrayChildStruct array[2];
};

struct MainStruct {
    int8_t before;
    tArrayTest test;
    int8_t after;
};

const MainStruct test_data = {4,
                              {2,
                               {{3, {1, 2, 3}, 2, {10, 20}, {40, 50, 60}},
                                {3, {11, 22, 33}, 2, {110, 220}, {400, 500, 600}}}},
                              8};

#pragma pack(pop)
} // namespace serialized

} // namespace complex

template <typename T>
void TestDynamicComplex(codec::CodecFactory& factory, const T& test_data, DataRepresentation eRep)
{
    codec::Decoder decoder = factory.makeDecoderFor(&test_data, sizeof(test_data), eRep);

    dumpElements(decoder);

    ASSERT_EQ(a_util::result::SUCCESS, factory.isValid());
    ASSERT_EQ(a_util::result::SUCCESS, decoder.isValid());

    ASSERT_EQ(sizeof(T), decoder.getBufferSize(eRep));
    std::cout << a_util::strings::toString((uint64_t)decoder.getElementCount()).c_str();
    ASSERT_EQ(decoder.getElementCount(), 23);

    ASSERT_EQ(decoder.getElement("before").getVariantValue().getInt8(), 4);
    ASSERT_EQ(decoder.getElement("test.array_size").getVariantValue().getInt8(), 2);
    ASSERT_EQ(decoder.getElement("test.array[0].child_size").getVariantValue().getInt8(), 3);
    ASSERT_EQ(decoder.getElement("test.array[0].child_array[0]").getVariantValue().getInt32(), 1);
    ASSERT_EQ(decoder.getElement("test.array[0].child_array[1]").getVariantValue().getInt32(), 2);
    ASSERT_EQ(decoder.getElement("test.array[0].child_array[2]").getVariantValue().getInt32(), 3);
    ASSERT_EQ(decoder.getElement("test.array[0].child_size2").getVariantValue().getInt8(), 2);
    ASSERT_EQ(decoder.getElement("test.array[0].child_array2[0]").getVariantValue().getInt32(), 10);
    ASSERT_EQ(decoder.getElement("test.array[0].child_array2[1]").getVariantValue().getInt32(), 20);
    ASSERT_EQ(decoder.getElement("test.array[0].fixed_array[0]").getVariantValue().getInt16(), 40);
    ASSERT_EQ(decoder.getElement("test.array[0].fixed_array[1]").getVariantValue().getInt16(), 50);
    ASSERT_EQ(decoder.getElement("test.array[0].fixed_array[2]").getVariantValue().getInt16(), 60);
    ASSERT_EQ(decoder.getElement("test.array[1].child_size").getVariantValue().getInt8(), 3);
    ASSERT_EQ(decoder.getElement("test.array[1].child_array[0]").getVariantValue().getInt32(), 11);
    ASSERT_EQ(decoder.getElement("test.array[1].child_array[1]").getVariantValue().getInt32(), 22);
    ASSERT_EQ(decoder.getElement("test.array[1].child_array[2]").getVariantValue().getInt32(), 33);
    ASSERT_EQ(decoder.getElement("test.array[1].child_size2").getVariantValue().getInt8(), 2);
    ASSERT_EQ(decoder.getElement("test.array[1].child_array2[0]").getVariantValue().getInt32(),
              110);
    ASSERT_EQ(decoder.getElement("test.array[1].child_array2[1]").getVariantValue().getInt32(),
              220);
    ASSERT_EQ(decoder.getElement("test.array[1].fixed_array[0]").getVariantValue().getInt16(), 400);
    ASSERT_EQ(decoder.getElement("test.array[1].fixed_array[1]").getVariantValue().getInt16(), 500);
    ASSERT_EQ(decoder.getElement("test.array[1].fixed_array[2]").getVariantValue().getInt16(), 600);
    ASSERT_EQ(decoder.getElement("after").getVariantValue().getInt8(), 8);
}

/**
 * @detail  Check deserialized codec dynamic information with complex data
 * @req_id ACORE-7704 CDPKGDDL-12
 */
TEST(CodecTest, TestDynamicComplex)
{
    codec::CodecFactory factory("main", complex::test_description);
    ::TestDynamicComplex(factory, complex::test_data, deserialized);
}

/**
 * @detail  Check serialized codec dynamic information with complex data
 * @req_id ACORE-7704 CDPKGDDL-13
 */
TEST(CodecTest, TestDynamicComplexSerialized)
{
    codec::CodecFactory factory("main", complex::test_description);
    ::TestDynamicComplex(factory, complex::serialized::test_data, serialized);
}

class cTestPerf {
public:
    virtual inline ~cTestPerf(){};
    virtual const char* getName() = 0;
    virtual void CreateCodec(const char* strStructName,
                             const char* strDescription,
                             void* pData,
                             size_t nDataSize) = 0;
};

class cTestOldPerf : public cTestPerf {
public:
    const char* getName()
    {
        return "Old Codec Variant";
    }

    void CreateCodec(const char* strStructName,
                     const char* strDescription,
                     void* pData,
                     size_t nDataSize)
    {
        ddl::CodecFactory factory(strStructName, strDescription);
        _decoder = factory.makeDecoderFor(pData, nDataSize);
    }
    ddl::Decoder _decoder;
};

class cTestNewPerf1 : public cTestPerf {
public:
    const char* getName()
    {
        return "New CodecIndex Variant";
    }

    void CreateCodec(const char* strStructName,
                     const char* strDescription,
                     void* pData,
                     size_t nDataSize)
    {
        codec::CodecFactory factory(strStructName, strDescription);
        _decoder = factory.makeDecoderFor(pData, nDataSize);
    }
    codec::Decoder _decoder;
};

void TestPerf(const char* strStructName, const char* strDescription, void* pData, size_t nDataSize)
{
    size_t nRepeats = 10000;

    std::list<cTestPerf*> oTesters;

    // oTesters.push_back(new cTestOldPerf()); // this is not necessary because the old uses the new
    // now!
    oTesters.push_back(new cTestNewPerf1());

    std::cout << std::endl << "Perform Creation Tests:" << std::endl;
    for (std::list<cTestPerf*>::iterator it = oTesters.begin(); it != oTesters.end(); ++it) {
        timestamp_t now = a_util::system::getCurrentMicroseconds();
        {
            for (size_t nRound = 0; nRound < nRepeats; ++nRound) {
                (*it)->CreateCodec(strStructName, strDescription, pData, nDataSize);
            }
        }
        timestamp_t nTime = a_util::system::getCurrentMicroseconds() - now;
        std::cout << a_util::strings::format("%s: %d repeats create %lld (%lld) ",
                                             (*it)->getName(),
                                             nRepeats,
                                             nTime,
                                             nTime / nRepeats)
                  << std::endl;
    }

    while (!oTesters.empty()) {
        delete oTesters.front();
        oTesters.pop_front();
    }
}

/**
 * @detail  Test codec creation performances for static information
 * @req_id ACORE-7704
 */

TEST(CodecTest, TestStaticPerf)
{
    static_struct::TestStruct test = static_struct::test_data;
    TestPerf("test", static_struct::test_description, &test, sizeof(test));
}

/**
 * @detail  Test codec creation performances for dynamic information
 * @req_id ACORE-7704
 */

TEST(CodecTest, TestDynamicPerf)
{
    simple::MainStruct test = simple::test_data;
    TestPerf("main", simple::test_description, &test, sizeof(test));

    complex::MainStruct testComplex = complex::test_data;
    TestPerf("main", complex::test_description, &testComplex, sizeof(testComplex));
}

namespace enums {
struct MainStruct {
    int32_t static_value;
    int8_t array_size;
    int32_t array[3];
    int32_t after;
};

const char* test_description =
    R"(<?xml version="1.0" encoding="iso-8859-1" standalone="no"?>
    <adtf:ddl>
        <enums>
            <enum name="Test" type="tInt32">
                <element name="A" value="1"/>
                <element name="B" value="2"/>
                <element name="C" value="3"/>
            </enum>
            <enum name="Test2" type="tInt32">
                <element name="D" value="1"/>
                <element name="E" value="2"/>
                <element name="F" value="3"/>
            </enum>
        </enums>
        <structs>
            <struct alignment="4" name="main" version="2">
                <element alignment="4" arraysize="1" byteorder="LE" bytepos="0" name="static" type="Test"/>
                <element alignment="1" arraysize="1" byteorder="LE" bytepos="4" name="array_size" type="tInt8"/>
                <element alignment="4" arraysize="array_size" byteorder="LE" bytepos="5" name="array" type="Test2"/>
                <element alignment="4" arraysize="1" byteorder="LE" bytepos="-1" name="after" type="Test2"/>
            </struct>
        </structs>
    </adtf:ddl>)";

const MainStruct test_data = {2, 3, {1, 2, 3}, 3};

} // namespace enums

/**
 * @detail  Check decoder functionalities for enums
 * @req_id ACORE-7704
 */
TEST(CodecTest, TestEnums)
{
    codec::CodecFactory factory("main", enums::test_description);
    ASSERT_EQ(a_util::result::SUCCESS, factory.isValid());
    codec::Decoder decoder = factory.makeDecoderFor(&enums::test_data, sizeof(enums::test_data));
    dumpElements(decoder);
    ASSERT_EQ(a_util::result::SUCCESS, decoder.isValid());

    ASSERT_EQ(decoder.getElement("static").getValue<std::string>(), "B");
    ASSERT_EQ(decoder.getElement("array_size").getValue<std::string>(), "3");
    ASSERT_EQ(decoder.getElement("array[0]").getValue<std::string>(), "D");
    ASSERT_EQ(decoder.getElement("array[1]").getStringValue(), "E");
    ASSERT_EQ(decoder.getElement("array[2]").getStringValue(), "F");
    ASSERT_EQ(decoder.getElement("after").getStringValue(), "F");
}

namespace constants {
struct MainStruct {
    int32_t static_value;
    int8_t array_size;
    int32_t array[3];
    int32_t after;
};

const char* test_description =
    R"(<?xml version="1.0" encoding="iso-8859-1" standalone="no"?>
    <adtf:ddl>
        <enums>
        <enum name="Test" type="tInt32">
            <element name="A" value="1"/>
            <element name="B" value="2"/>
            <element name="C" value="3"/>
        </enum>
        <enum name="Test2" type="tInt32">
            <element name="D" value="1"/>
            <element name="E" value="2"/>
            <element name="F" value="3"/>
        </enum>
        </enums>
        <structs>
            <struct alignment="4" name="main" version="2">
                <element alignment="4" arraysize="1" byteorder="LE" bytepos="0" name="static" type="Test" value="A"/>
              <element alignment="1" arraysize="1" byteorder="LE" bytepos="4" name="array_size" type="tInt8"/>
              <element alignment="4" arraysize="array_size" byteorder="LE" bytepos="5" name="array" type="Test2" value="D"/>"
               <element alignment="4" arraysize="1" byteorder="LE" bytepos="-1" name="after" type="Test2" value="E"/>"
            </struct>
        </structs>
    </adtf:ddl>)";

const MainStruct test_data = {0, 3, {0, 0, 0}, 0};

} // namespace constants

/**
 * @detail  Check decoder functionalities for constants
 * @req_id ACORE-7704
 */
TEST(CodecTest, TestConstants)
{
    codec::CodecFactory factory("main", constants::test_description);
    ASSERT_EQ(a_util::result::SUCCESS, factory.isValid());
    constants::MainStruct test_data = constants::test_data;
    codec::Codec codec = factory.makeCodecFor(&test_data, sizeof(test_data));
    dumpElements(codec);
    ASSERT_EQ(a_util::result::SUCCESS, codec.isValid());

    ASSERT_EQ(codec.getElement("static").getValue<int32_t>(), 0);
    ASSERT_EQ(codec.getElement("array_size").getValue<int32_t>(), 3);
    ASSERT_EQ(codec.getElement("array[0]").getValue<int32_t>(), 0);
    ASSERT_EQ(codec.getElement("array[1]").getValue<int32_t>(), 0);
    ASSERT_EQ(codec.getElement("array[2]").getValue<int32_t>(), 0);
    ASSERT_EQ(codec.getElement("after").getValue<int32_t>(), 0);

    ASSERT_NO_THROW(codec.resetValues());

    ASSERT_EQ(codec.getElement("static").getValue<int32_t>(), 1);
    ASSERT_EQ(codec.getElement("array_size").getValue<int32_t>(), 3);
    ASSERT_EQ(codec.getElement("array[0]").getValue<int32_t>(), 1);
    ASSERT_EQ(codec.getElement("array[1]").getValue<int32_t>(), 1);
    ASSERT_EQ(codec.getElement("array[2]").getValue<int32_t>(), 1);
    ASSERT_EQ(codec.getElement("after").getValue<int32_t>(), 2);
}

namespace all_types {
#ifdef _MSC_VER
#define ALIGNED_ELEM(__align, __def) __declspec(align(__align)) __def;
#else
#define ALIGNED_ELEM(__align, __def) __def __attribute__((aligned(__align)));
#endif // _WIN32

struct MainStruct {
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

const char* test_description =
    R"(<?xml version="1.0" encoding="iso-8859-1" standalone="no"?>
        <struct alignment="4" name="main" version="2">
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

const MainStruct test_data = {true, 1, 2, 3, 4, 5, 6, 7, 8, (float)3.1415, 2.7182, 'x'};

} // namespace all_types

/**
 * @detail  Check decoder functionalities for all types
 * @req_id ACORE-7704 CDPKGDDL-12 CDPKGDDL-13
 */
TEST(CodecTest, TestAllTypes)
{
    using namespace all_types;

    codec::CodecFactory factory("main", test_description);
    ASSERT_EQ(a_util::result::SUCCESS, factory.isValid());

    codec::Decoder decoder = factory.makeDecoderFor(&test_data, sizeof(test_data));
    a_util::memory::MemoryBuffer oSerializedBuffer;
    ASSERT_EQ(a_util::result::SUCCESS, codec::transformToBuffer(decoder, oSerializedBuffer, true));

    decoder =
        factory.makeDecoderFor(oSerializedBuffer.getPtr(), oSerializedBuffer.getSize(), serialized);
    a_util::memory::MemoryBuffer oBuffer;
    ASSERT_EQ(a_util::result::SUCCESS, codec::transformToBuffer(decoder, oBuffer, true));

    const MainStruct* pRes = reinterpret_cast<MainStruct*>(oBuffer.getPtr());
    ASSERT_EQ(pRes->bBool, test_data.bBool);
    ASSERT_EQ(pRes->nInt8, test_data.nInt8);
    ASSERT_EQ(pRes->nUInt8, test_data.nUInt8);
    ASSERT_EQ(pRes->nInt16, test_data.nInt16);
    ASSERT_EQ(pRes->nUInt16, test_data.nUInt16);
    ASSERT_EQ(pRes->nInt32, test_data.nInt32);
    ASSERT_EQ(pRes->nUInt32, test_data.nUInt32);
    ASSERT_EQ(pRes->nInt64, test_data.nInt64);
    ASSERT_EQ(pRes->nUInt64, test_data.nUInt64);
    ASSERT_EQ(pRes->fFloat32, test_data.fFloat32);
    ASSERT_EQ(pRes->fFloat64, test_data.fFloat64);
    ASSERT_EQ(pRes->nChar, test_data.nChar);
}

namespace alignment_of_substructs {
#pragma pack(push, 2)
struct SubStruct {
    int32_t value1;
    int32_t value2;
};

#pragma pack(pop)

#pragma pack(push, 1)
struct MainStruct {
    int8_t padding;
    SubStruct sub_struct[4];
};
#pragma pack(pop)

const char* test_description =
    R"(<?xml version="1.0" encoding="iso-8859-1" standalone="no"?>
    <structs>
        <struct alignment="2" name="SubStruct" version="2">
            <element alignment="2" arraysize="1" byteorder="BE" bitpos="1" bytepos="0" name="value1" type="tInt32"/>
            <element alignment="2" arraysize="1" byteorder="BE" bitpos="1" bytepos="0" name="value2" type="tInt32"/>
        </struct>
        <struct alignment="1" name="MainStruct" version="2">
            <element alignment="1" arraysize="1" byteorder="BE" bitpos="1" bytepos="0" name="padding" type="tInt8"/>
            <element alignment="1" arraysize="4" byteorder="BE" bitpos="1" bytepos="0" name="sub_struct" type="SubStruct"/>
        </struct>
    </structs>)";

const MainStruct test_data = {0, {{1, 2}, {3, 4}, {5, 6}, {7, 8}}};

} // namespace alignment_of_substructs

/**
 * @detail  Check alignment of substructures
 * @req_id CDDDL-54
 */
TEST(CodecTest, TestAlignmentOfSubstructs)
{
    using namespace alignment_of_substructs;

    codec::CodecFactory factory("MainStruct", test_description);
    ASSERT_EQ(a_util::result::SUCCESS, factory.isValid());
    ASSERT_EQ(factory.getStaticBufferSize(), sizeof(MainStruct));

    codec::Decoder decoder = factory.makeDecoderFor(&test_data, sizeof(test_data));
    dumpElements(decoder);

    ASSERT_EQ(decoder.getElement("sub_struct[0].value1").getValue<int8_t>(),
              test_data.sub_struct[0].value1);
    ASSERT_EQ(decoder.getElement("sub_struct[0].value2").getValue<int8_t>(),
              test_data.sub_struct[0].value2);

    ASSERT_EQ(decoder.getElement("sub_struct[1].value1").getValue<int8_t>(),
              test_data.sub_struct[1].value1);
    ASSERT_EQ(decoder.getElement("sub_struct[1].value2").getValue<int8_t>(),
              test_data.sub_struct[1].value2);

    ASSERT_EQ(decoder.getElement("sub_struct[2].value1").getValue<int8_t>(),
              test_data.sub_struct[2].value1);
    ASSERT_EQ(decoder.getElement("sub_struct[2].value2").getValue<int8_t>(),
              test_data.sub_struct[2].value2);

    ASSERT_EQ(decoder.getElement("sub_struct[3].value1").getValue<int8_t>(),
              test_data.sub_struct[3].value1);
    ASSERT_EQ(decoder.getElement("sub_struct[3].value2").getValue<int8_t>(),
              test_data.sub_struct[3].value2);
}

void TestError(const codec::CodecFactory& factory, void* pData, size_t nDataSize, bool bValid)
{
    {
        codec::StaticDecoder test_codec = factory.makeStaticDecoderFor(pData, nDataSize);
        if (bValid) {
            ASSERT_EQ(a_util::result::SUCCESS, test_codec.isValid());
        }
        else {
            ASSERT_NE(a_util::result::SUCCESS, test_codec.isValid());
        }
        ASSERT_ANY_THROW(test_codec.resolve(10));
        ASSERT_ANY_THROW(a_util::variant::Variant oValue =
                             test_codec.getElement("any_val").getVariantValue());
    }

    {
        codec::Decoder test_codec = factory.makeDecoderFor(pData, nDataSize);
        if (bValid) {
            ASSERT_EQ(a_util::result::SUCCESS, test_codec.isValid());
        }
        else {
            ASSERT_NE(a_util::result::SUCCESS, test_codec.isValid());
        }
        ASSERT_ANY_THROW(test_codec.resolve(10));
        ASSERT_ANY_THROW(a_util::variant::Variant oValue =
                             test_codec.getElement("any_val").getVariantValue());
    }

    {
        codec::StaticCodec test_codec = factory.makeStaticCodecFor(pData, nDataSize);
        if (bValid) {
            ASSERT_EQ(a_util::result::SUCCESS, test_codec.isValid());
        }
        else {
            ASSERT_NE(a_util::result::SUCCESS, test_codec.isValid());
        }
        ASSERT_ANY_THROW(test_codec.resolve(10));
        ASSERT_ANY_THROW(a_util::variant::Variant oValue =
                             test_codec.getElement("any_val").getVariantValue());
    }

    {
        codec::Codec test_codec = factory.makeCodecFor(pData, nDataSize);
        if (bValid) {
            ASSERT_EQ(a_util::result::SUCCESS, test_codec.isValid());
        }
        else {
            ASSERT_NE(a_util::result::SUCCESS, test_codec.isValid());
        }
        ASSERT_ANY_THROW(test_codec.resolve(10));
        ASSERT_ANY_THROW(a_util::variant::Variant oValue =
                             test_codec.getElement("any_val").getVariantValue());
    }
}

/**
 * @detail  Test error handling in decoder
 * @req_id ACORE-7704 ACORE-8004
 */
TEST(CodecTest, TestErrorHandling)
{
    using namespace static_struct;

    // invalid index
    {
        auto factory = codec::CodecFactory("test", test_description);
        ASSERT_EQ(a_util::result::SUCCESS, factory.isValid());
        TestStruct test_struct;
        TestError(factory, &test_struct, sizeof(test_struct), true);
    }

    // invalid data
    {
        auto factory = codec::CodecFactory("test", test_description);
        ASSERT_EQ(a_util::result::SUCCESS, factory.isValid());
        TestError(factory, NULL, 0, false);
    }

    // invalid struct name
    {
        auto factory = codec::CodecFactory("", test_description);
        ASSERT_NE(a_util::result::SUCCESS, factory.isValid());
        TestStruct test_struct;
        TestError(factory, &test_struct, sizeof(test_struct), false);
    }

    // invalid description
    {
        auto factory = codec::CodecFactory("test", "");
        ASSERT_NE(a_util::result::SUCCESS, factory.isValid());
        TestStruct test_struct;
        TestError(factory, &test_struct, sizeof(test_struct), false);
    }
}

struct tLeaf {
    uint8_t value1;
    uint8_t value2;

    static ddl::DDStructure GetDefinition()
    {
        return DDStructureGenerator<tLeaf>("tLeaf")
            .addElement("value1", &tLeaf::value1)
            .addElement("value2", &tLeaf::value2);
    }
};

inline bool operator==(const tLeaf& lhs, const tLeaf& rhs)
{
    return std::tie(lhs.value1, lhs.value2) == std::tie(rhs.value1, rhs.value2);
}

struct tIntermediate {
    tLeaf sLeaf;
    uint8_t value3;

    static ddl::DDStructure GetDefinition()
    {
        return DDStructureGenerator<tIntermediate>("tIntermediate")
            .addElement("sLeaf", &tIntermediate::sLeaf, tLeaf::GetDefinition())
            .addElement("value3", &tIntermediate::value3);
    }
};

inline bool operator==(const tIntermediate& lhs, const tIntermediate& rhs)
{
    return std::tie(lhs.sLeaf, lhs.value3) == std::tie(rhs.sLeaf, rhs.value3);
}

struct tParent {
    tIntermediate aIntermediate[2];
    uint8_t nParentValue;

    static ddl::DDStructure GetDefinition()
    {
        return DDStructureGenerator<tParent>("tParent")
            .addElement("aIntermediate", &tParent::aIntermediate, tIntermediate::GetDefinition())
            .addElement("nParentValue", &tParent::nParentValue);
    }
};

/**
 * @detail Test addresses in codec with structs in array
 * @req_id
 */
TEST(CodecTest, TestArrayAccessWithStruct)
{
    tParent test_data{{
                          {{1, 2}, 3},
                          {{4, 5}, 6},
                      },
                      7};

    ddl::codec::CodecFactory codec_factory(tParent::GetDefinition());
    auto codec = codec_factory.makeCodecFor(&test_data, sizeof(test_data));

    const tLeaf* leaf =
        static_cast<const tLeaf*>(codec.getElement("aIntermediate[1].sLeaf").getAddress());
    tLeaf check_leaf = {4, 5};
    ASSERT_EQ(*leaf, check_leaf);

    auto value1 = codec.getElement("aIntermediate[1].sLeaf.value1").getVariantValue();
    auto value2 = codec.getElement("aIntermediate[1].sLeaf.value2").getVariantValue();
    auto value3 = codec.getElement("aIntermediate[1].value3").getVariantValue();

    auto parentvalue = codec.getElement("nParentValue").getVariantValue();

    ASSERT_EQ(value1.asUInt8(), 4);
    ASSERT_EQ(value2.asUInt8(), 5);
    ASSERT_EQ(value3.asUInt8(), 6);
    ASSERT_EQ(parentvalue.asUInt8(), 7);
}

namespace static_test_leaf {

const auto test_description =
    R"(<?xml version="1.0" encoding="iso-8859-1" standalone="no"?>
<structs>
    <struct alignment="4" name="child_struct" version="2">
        <element alignment="4" arraysize="1" byteorder="LE" bytepos="0" name="value_dummy" type="tInt32"/>
        <element alignment="4" arraysize="1" byteorder="LE" bytepos="4" name="value" type="tInt32"/>"
        <element alignment="4" arraysize="1" byteorder="LE" bytepos="8" name="value2" type="tInt32"/>"
        <element alignment="4" arraysize="1" byteorder="LE" bytepos="12" name="value_float" type="tFloat32"/>"
    </struct>"
    <struct alignment="4" name="test" version="2">"
        <element alignment="4" arraysize="1" byteorder="LE" bytepos="0" name="child" type="child_struct"/>"
    </struct>
</structs>)";

} // namespace static_test_leaf

TEST(CodecTest, TestStaticLeafCodecIndex)
{
    using namespace static_test_leaf;
    a_util::memory::MemoryBuffer buffer;

    codec::CodecFactory factory("test", static_test_leaf::test_description);
    ASSERT_TRUE(buffer.allocate(factory.getStaticBufferSize()));

    auto child_value_leaf_index = codec::LeafCodecIndex(
        factory.getElement("child.value").getIndex(), ddl::DataRepresentation::deserialized);
    auto child_value_dummy_leaf_index = codec::LeafCodecIndex(
        factory.getElement("child.value_dummy").getIndex(), ddl::DataRepresentation::deserialized);

    {
        auto codec = factory.makeCodecFor(buffer.getPtr(), buffer.getSize());
        EXPECT_NO_THROW(codec.setElementValue(child_value_leaf_index, 123));
        EXPECT_NO_THROW(codec.setElementValue(child_value_dummy_leaf_index, 4321));
    }

    auto decoder = factory.makeStaticDecoderFor(buffer.getPtr(), buffer.getSize());
    EXPECT_EQ(decoder.getElement(std::string("child.value")).getValue<int32_t>(), 123);
    EXPECT_EQ(decoder.getElement(std::string("child.value_dummy")).getValue<int64_t>(), 4321);

    EXPECT_EQ(decoder.getElementValue<int32_t>(child_value_leaf_index), 123);
    EXPECT_EQ(decoder.getElementValue<int64_t>(child_value_dummy_leaf_index), 4321);
}

namespace dynamic_test_leaf {

struct ArrayChildStruct {
    int32_t child_size;
    int32_t child_array[3];
};

struct ArrayTest {
    int32_t array_size;
    ArrayChildStruct array[10];
};

const auto test_description =
    R"(<?xml version="1.0" encoding="iso-8859-1" standalone="no"?>
    <structs>
        <struct alignment="4" name="child_struct" version="2">
            <element alignment="4" arraysize="1" byteorder="LE" bytepos="0" name="child_size" type="tInt32"/>
            <element alignment="4" arraysize="child_size" byteorder="LE" bytepos="0" name="child_array" type="tInt32"/>
        </struct>
        <struct alignment="4" name="test" version="2">
            <element alignment="4" arraysize="1" byteorder="LE" bytepos="0" name="array_size" type="tInt32"/>
            <element alignment="4" arraysize="array_size" byteorder="LE" bytepos="0" name="array" type="child_struct"/>
        </struct>
    </structs>)";

} // namespace dynamic_test_leaf

TEST(CodecTest, TestDynamicLeafCodecIndex)
{
    using namespace dynamic_test_leaf;

    const codec::CodecFactory factory("test", test_description);

    // initialize the test data for the dynamic initializing of Codec
    // for this test case we set the array size to 3 elements in test_data
    // and to 3 elements within each child_array
    // the last 7 elements of ArrayTest are not used
    const ArrayTest test_data{3, {3, {0, 1, 2}, 3, {0, 1, 2}, 3, {0, 1, 2}}};

    const auto decoder = factory.makeDecoderFor(&test_data, sizeof(test_data));
    const auto indices = getLeafCodecIndices(decoder, decoder.getRepresentation());
    const std::vector<int32_t> expected_values{3, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2};
    ASSERT_EQ(indices.size(), expected_values.size());

    for (size_t current_index = 0; current_index < indices.size(); ++current_index) {
        EXPECT_EQ(expected_values[current_index],
                  decoder.getElementValue<int32_t>(indices[current_index]));
    }
}

namespace {
static void testPerformance(const std::function<void()>& test_call,
                            size_t test_count,
                            const std::string& test_description)
{
    using namespace std::chrono_literals;
    using namespace std::chrono;

    const auto first_measured_time_point = steady_clock::now();

    test_call();

    const auto elapsed_measured_time_point = steady_clock::now() - first_measured_time_point;
    const auto elapsed_time_per_iteration = elapsed_measured_time_point / test_count;
    std::cout << BUILD_TYPE << " Time elapsed: " << test_description << " (" << test_count
              << " iterations): " << std::endl
              << "    " << duration_cast<microseconds>(elapsed_time_per_iteration).count()
              << " micro sec per 1 iteration" << std::endl
              << "    Total (" << duration_cast<microseconds>(elapsed_measured_time_point).count()
              << " micro sec)" << std::endl;
}
} // namespace

TEST(CodecTest, TestSampleDecodingPerformanceCodecAccess)
{
    const std::string file_to_use = TEST_FILES_DIR "/test_performance.description";

    size_t test_decoding_count = 100;

    for (auto current_representation: {ddl::deserialized, ddl::serialized}) {
        const std::string current_representation_text =
            (current_representation == ddl::deserialized) ? "deserialized" : "serialized";
        std::cout << std::endl
                  << "----------------------------------------------------------------------"
                  << std::endl
                  << " Representation: " << current_representation_text << std::endl
                  << "----------------------------------------------------------------------"
                  << std::endl
                  << std::endl;
        { // prepare new factory and decoder
            auto ddl_from_file = ddl::DDFile::fromXMLFile(file_to_use);

            const codec::CodecFactory codec_factory_new(
                *ddl_from_file.getStructTypes().get("BigDataType"), ddl_from_file);
            ASSERT_EQ(codec_factory_new.isValid(), a_util::result::SUCCESS);

            a_util::memory::MemoryBuffer buffer(
                codec_factory_new.getStaticBufferSize(current_representation));

            const auto codec_indices = codec::getCodecIndices(codec_factory_new);
            const auto leaf_codec_indices =
                codec::getLeafCodecIndices(codec_factory_new, current_representation);

            const auto codec = codec_factory_new.makeStaticCodecFor(
                buffer.getPtr(), buffer.getSize(), current_representation);

            // we measure the access with codec index now
            {
                testPerformance(
                    [&]() {
                        for (size_t current_test = 0; current_test < test_decoding_count;
                             current_test++) {
                            for (const auto& current_leaf_index: codec_indices) {
                                auto value = codec.getElementValue<double>(current_leaf_index);
                                a_util::maybe_unused(value);
                            }
                        }
                    },
                    test_decoding_count,
                    "Decoding with new CodecIndex");
            }

            // we measure the access with the iterator now
            {
                testPerformance(
                    [&]() {
                        for (size_t current_test = 0; current_test < test_decoding_count;
                             current_test++) {
                            codec::forEachLeafElement(codec.getElements(), [&](auto& element) {
                                auto value = element.template getValue<double>();
                                a_util::maybe_unused(value);
                            });
                        }
                    },
                    test_decoding_count,
                    "Decoding with for_each_leaf");
            }

            // we measure the access with the leaf codec index
            {
                testPerformance(
                    [&]() {
                        for (size_t current_test_count = 0;
                             current_test_count < test_decoding_count;
                             current_test_count++) {
                            for (const auto& current_leaf_index: leaf_codec_indices) {
                                auto value = codec.getElementValue<double>(current_leaf_index);
                                a_util::maybe_unused(value);
                            }
                        }
                    },
                    test_decoding_count,
                    "Decoding with new LeafCodecIndex");
            }
        }
    }
}

namespace static_test_leaf {
enum class EnumTypeValue : uint32_t { value_1 = 1, value_123 = 123 };

enum EnumTypeValueDummy { dummy_value_1 = 1, dummy_value_123 = 123 };
} // namespace static_test_leaf

TEST(CodecTest, CheckEnumUsage)
{
    using namespace static_test_leaf;

    const codec::CodecFactory factory("test", test_description);
    a_util::memory::MemoryBuffer buffer(factory.getStaticBufferSize());

    const auto child_value_leaf_index = codec::LeafCodecIndex(
        factory.getElement("child.value").getIndex(), ddl::DataRepresentation::deserialized);
    const auto child_value_dummy_leaf_index = codec::LeafCodecIndex(
        factory.getElement("child.value_dummy").getIndex(), ddl::DataRepresentation::deserialized);
    const auto child_value2_leaf_index = codec::LeafCodecIndex(
        factory.getElement("child.value2").getIndex(), ddl::DataRepresentation::deserialized);
    const auto child_value_float_leaf_index = codec::LeafCodecIndex(
        factory.getElement("child.value_float").getIndex(), ddl::DataRepresentation::deserialized);

    {
        auto codec = factory.makeStaticCodecFor(buffer.getPtr(), buffer.getSize());
        EXPECT_NO_THROW(
            codec.setElementValue<EnumTypeValue>(child_value_leaf_index, EnumTypeValue::value_1));
        EXPECT_NO_THROW(codec.setElementValue<EnumTypeValueDummy>(
            child_value_dummy_leaf_index, EnumTypeValueDummy::dummy_value_123));
        EXPECT_NO_THROW(codec.setElementValue<int32_t>(child_value2_leaf_index, 22));

        // check if exception is thrown if you want to set the value to a floating point
        EXPECT_ANY_THROW(codec.setElementValue<EnumTypeValueDummy>(
            child_value_float_leaf_index, EnumTypeValueDummy::dummy_value_123));
    }

    const auto decoder = factory.makeStaticDecoderFor(buffer.getPtr(), buffer.getSize());

    EXPECT_EQ(decoder.getElementValue<EnumTypeValue>(child_value_leaf_index),
              EnumTypeValue::value_1);
    EXPECT_EQ(decoder.getElementValue<EnumTypeValueDummy>(child_value_dummy_leaf_index),
              EnumTypeValueDummy::dummy_value_123);
    EXPECT_NE(decoder.getElementValue<EnumTypeValue>(child_value2_leaf_index),
              EnumTypeValue::value_1);
    EXPECT_ANY_THROW(decoder.getElementValue<EnumTypeValueDummy>(child_value_float_leaf_index));
}

TEST(CodecTest, LeafLayout_LeafCodecIndex_is_trivially_copyable)
{
    static_assert(std::is_trivially_copyable<ddl::codec::LeafLayout>::value,
                  "LeafLayout is not trivially copyable");
    static_assert(std::is_trivially_copyable<ddl::codec::LeafCodecIndex>::value,
                  "LeafCodecIndex is not trivially copyable");
}

struct MyStructWithTypeConversion {
    MyStructWithTypeConversion(const int32_t& value) : m_value(static_cast<int64_t>(value))
    {
    }
    MyStructWithTypeConversion(const uint32_t& value) : m_value(static_cast<int64_t>(value))
    {
    }
    MyStructWithTypeConversion(const int64_t& value) : m_value(value)
    {
    }
    MyStructWithTypeConversion(const uint64_t& value) : m_value(static_cast<int64_t>(value))
    {
    }
    MyStructWithTypeConversion(const float& value) : m_value(static_cast<int64_t>(value))
    {
    }
    MyStructWithTypeConversion(const double& value) : m_value(static_cast<int64_t>(value))
    {
    }
    operator int32_t() const
    {
        return static_cast<int32_t>(m_value);
    }
    explicit operator double() const
    {
        return static_cast<double>(m_value);
    }
    bool operator==(const MyStructWithTypeConversion& other) const
    {
        return (m_value == other.m_value);
    }
    int64_t m_value;
};

struct MyStructWithInvalidConversion {
    MyStructWithInvalidConversion(const float& fValue) : m_value(static_cast<uint64_t>(fValue))
    {
    }
    operator bool() const
    {
        return static_cast<bool>(m_value);
    }
    bool operator==(const MyStructWithInvalidConversion& other) const
    {
        return (m_value == other.m_value);
    }
    uint64_t m_value;
};

struct MyStructWithNoConversion {
    bool operator==(const MyStructWithNoConversion& other) const
    {
        return (m_value == other.m_value);
    }
    uint64_t m_value;
};

TEST(CodecTest, GetAndSetFor_is_convertible_types)
{
    using namespace static_test_leaf;

    const codec::CodecFactory factory("test", test_description);
    a_util::memory::MemoryBuffer buffer(factory.getStaticBufferSize());

    const auto child_value_leaf_index = codec::LeafCodecIndex(
        factory.getElement("child.value").getIndex(), ddl::DataRepresentation::deserialized);
    const auto child_value_dummy_leaf_index = codec::LeafCodecIndex(
        factory.getElement("child.value_dummy").getIndex(), ddl::DataRepresentation::deserialized);
    const auto child_value2_leaf_index = codec::LeafCodecIndex(
        factory.getElement("child.value2").getIndex(), ddl::DataRepresentation::deserialized);

    {
        auto codec = factory.makeStaticCodecFor(buffer.getPtr(), buffer.getSize());
        EXPECT_NO_THROW(codec.setElementValue<MyStructWithTypeConversion>(
            child_value_leaf_index, MyStructWithTypeConversion(1)));
        EXPECT_NO_THROW(codec.setElementValue<MyStructWithInvalidConversion>(
            child_value_dummy_leaf_index, MyStructWithInvalidConversion(1.0)));
        // MyStructWithNoConversion test;
        // REQUIRE_THROWS(codec.SetElementValue<MyStructWithNoConversion>(oChildValue2LeafIndex,
        // test));
        EXPECT_NO_THROW(codec.setElementValue<a_util::variant::Variant>(
            child_value2_leaf_index, a_util::variant::Variant(1234)));
    }

    const auto decoder = factory.makeStaticDecoderFor(buffer.getPtr(), buffer.getSize());

    EXPECT_EQ(decoder.getElementValue<MyStructWithTypeConversion>(child_value_leaf_index),
              MyStructWithTypeConversion(int32_t(1)));
#ifdef WIN32
// to compile this we need to switch off the warning
// this is if the user ignores this warnings!!
#pragma warning(push)
#pragma warning(disable : 4244)
#endif
    EXPECT_EQ(decoder.getElementValue<MyStructWithInvalidConversion>(child_value_dummy_leaf_index),
              MyStructWithInvalidConversion(1.0));
// MyStructWithNoConversion test2;
// REQUIRE_THROWS(decoder.GetElementValue<MyStructWithNoConversion>(oChildValue2LeafIndex) ==
// test2);
#ifdef WIN32
#pragma warning(pop)
#endif
    EXPECT_EQ(decoder.getElementValue<a_util::variant::Variant>(child_value2_leaf_index),
              a_util::variant::Variant(1234));
}

#ifndef NO_VARIANT_TEST

TEST(CodecTest, GetAndSetFor_is_convertible_variant)
{
    using namespace static_test_leaf;

    const codec::CodecFactory factory("test", test_description);
    a_util::memory::MemoryBuffer buffer(factory.getStaticBufferSize());

    const auto child_value_leaf_index = codec::LeafCodecIndex(
        factory.getElement("child.value").getIndex(), ddl::DataRepresentation::deserialized);
    const auto child_value_dummy_leaf_index = codec::LeafCodecIndex(
        factory.getElement("child.value_dummy").getIndex(), ddl::DataRepresentation::deserialized);
    const auto child_value2_leaf_index = codec::LeafCodecIndex(
        factory.getElement("child.value2").getIndex(), ddl::DataRepresentation::deserialized);
    const auto child_value_float_leaf_index = codec::LeafCodecIndex(
        factory.getElement("child.value_float").getIndex(), ddl::DataRepresentation::deserialized);

    {
        auto codec = factory.makeStaticCodecFor(buffer.getPtr(), buffer.getSize());
        EXPECT_NO_THROW(codec.setElementValue<bool>(child_value_leaf_index, true));
        EXPECT_NO_THROW(codec.setElementValue<bool>(child_value2_leaf_index, true));
        EXPECT_NO_THROW(codec.setElementValue<int32_t>(child_value_dummy_leaf_index, 120));
        EXPECT_NO_THROW(codec.setElementValue<float>(child_value_float_leaf_index, 120.0f));
    }

    auto decoder = factory.makeStaticDecoderFor(buffer.getPtr(), buffer.getSize());

#ifdef _MSC_VER
// to compile this we need to switch off the warning
// this is if the user ignores this warnings!!
#pragma warning(push)
#pragma warning(disable : 4244)
#endif

    EXPECT_TRUE(decoder.getElementValue<std::variant<int64_t>>(child_value_leaf_index) ==
                std::variant<int64_t>(true));

    using variant_bool_int32 = std::variant<bool, int32_t>;
    EXPECT_EQ(decoder.getElementValue<variant_bool_int32>(child_value_dummy_leaf_index),
              variant_bool_int32(120));

    using variant_bool_double = std::variant<bool, double>;
    EXPECT_THROW(decoder.getElementValue<variant_bool_double>(child_value_dummy_leaf_index),
                 std::runtime_error);

#ifdef WIN32
#pragma warning(pop)
#endif

    using full_defined_variant = std::variant<bool,
                                              uint8_t,
                                              int8_t,
                                              uint16_t,
                                              int16_t,
                                              uint32_t,
                                              int32_t,
                                              uint64_t,
                                              int64_t,
                                              float,
                                              double>;

    EXPECT_EQ(decoder.getElementValue<full_defined_variant>(child_value_dummy_leaf_index),
              full_defined_variant(int32_t(120)));

    EXPECT_EQ(decoder.getElementValue<full_defined_variant>(child_value_float_leaf_index),
              full_defined_variant(120.0f));
}

#endif // NO_VARIANT_TEST