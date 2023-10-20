<!---
Copyright @ 2021 VW Group. All rights reserved.
 
This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
-->
<a name="ddl-decoder%2Fcodec"></a>
# DDL Decoder/Codec


<!-- TOC -->

- [DDL Decoder/Codec](#ddl-decodercodec)
    - [Accessing Data with a Decoder/Codec](#accessing-data-with-a-decodercodec)
    - [Decoding](#decoding)
    - [Encoding](#encoding)
    - [Selecting the Data Representation](#selecting-the-data-representation)
    - [Inspection](#inspection)
    - [Transformation](#transformation)

<!-- /TOC -->

<a name="accessing-data-with-a-decoder%2Fcodec"></a>
## Accessing Data with a Decoder/Codec

Let's take a look at a basic structure:

````xml
<struct alignment="4" name="tTest" version="1" ddlversion="2.0">
    <element name="bBool" type="tBool" arraysize="1">
        <serialized byteorder="LE" bytepos="0" bitpos="0" numbits="8"/>
        <deserialized alignment="1"/>
    </element>
    <element name="nInt8" type="tInt8" arraysize="1">
        <serialized byteorder="LE" bytepos="1" bitpos="0" numbits="8"/>
        <deserialized alignment="1"/>
    </element>
    <element name="nUInt32" type="tUInt32" arraysize="1">
        <serialized byteorder="LE" bytepos="2" bitpos="0" numbits="32"/>
        <deserialized alignment="4"/>
    </element>
    <element name="fFloat32" type="tFloat32" arraysize="1">
        <serialized byteorder="LE" bytepos="6" bitpos="0" numbits="32"/>
        <deserialized alignment="4"/>
    </element>
</struct>
````

<a name="decoding"></a>
## Decoding

Read access is handled with the ddl::codec::StaticDecoder or ddl::codec::Decoder class:

````cpp
tFloat64 readData(const void* const_data, size_t data_size)
{
    // this should of course be cached (as a member etc.)
    ddl::CodecFactory factory("tTest", "<ddl:ddl>....");

    // retrieve the codec index for a value for fast access
    auto codec_index_for_fFloat32 = factory.getElement("fFloat32").getIndex();

    // start decoding for data
    auto decoder = factory.makeDecoderFor(const_data, data_size);
    float value;

    // for name based lookup use getElement
    {
        value = decoder.getElement("fFloat32").getValue<float>();
    }

    // alternativley you can of course use indexed based lookup for fast access
    {
        value = decoder.getElementValue<float>(codec_index_for_fFloat32);
    }

    return value;
}
````

The example shows the usage of the ddl::codec::Decoder class with which handling of dynamic elements is possible (see also dynamic arrays within structs). A more efficient decoding initialization is provided via ddl::codec::StaticDecoder, but it allows only access to the static elements.

<a name="encoding"></a>
## Encoding

Write access is handled with the ddl::codec::Codec or ddl::codec::StaticCodec class:

````cpp
void writeData(void* data, size_t data_size, tFloat64 value)
{
    // this should of course be cached (as a member etc.)
    ddl::codec::CodecFactory factory("tTest", "<ddl:ddl>....");
    // retrieve the codec index for a value for fast access
    auto codec_index_for_fFloat32 = factory.getElement("fFloat32").getIndex();

    // start decoding / encoding for data
    auto codec = factory.makeCodecFor(data, data_size);

    // name based lookup
    codec.getElement("fFloat32").setValue(value);
    // or codec index based for fast access
    codec.setElementValue(codec_index_for_fFloat32, value);
}
````

The example shows the usage of the ddl::codec::Coder class with which handling of dynamic elements is possible (see also dynamic arrays within structs). A more efficient coding initialization is provided via ddl::codec::StaticCodec, but it allows only access to the static elements.

<a name="selecting-the-data-representation"></a>
## Selecting the Data Representation

By default decoders/codecs are created for the deserialized representation.
To create them for the serialized representation pass the correct parameters to the "make..." methods.

````cpp
auto decoder = factory.makeDecoderFor(const_data, data_size,
                                      DataRepresentation::serialized);
````

<a name="inspection"></a>
## Inspection

You can inspect the struct handled by a decoder/codec with the help of forEachLeafElement and ddl::codec::StaticDecoder::getElements:

````cpp
void dumpStructElements(const ddl::codec::StaticDecoder& decoder)
{
    forEachLeafElement(decoder.getElements(),
                       [](auto& element)
                       {
                           std::cout << element.getFullName();
                       });
}
````

<a name="transformation"></a>
## Transformation

Converting between the representations can be done with ddl::codec::transform:

````cpp
a_util::memory::MemoryBuffer getSerializedBuffer(const ddl::codec::Decoder& decoder)
{
    const auto serialized_size = decoder.getStaticBufferSize(ddl::DataRepresentation::serialized);
    a_util::memory::MemoryBuffer buffer(serialized_size);
    auto codec = decoder.makeCodecFor(
        buffer.getPtr(), buffer.getSize(), ddl::DataRepresentation::serialized);
    ddl::codec::transform(decoder, codec);
    return buffer;
}
````

