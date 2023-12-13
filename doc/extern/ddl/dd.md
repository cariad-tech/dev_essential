<!---
Copyright @ 2021 VW Group. All rights reserved.
 
This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
-->

<a name="datadefinition-and-its-datamodel"></a>
# DataDefinition and its datamodel

<!-- TOC -->

- [DataDefinition and its datamodel](#datadefinition-and-its-datamodel)
    - [Introduction](#introduction)
    - [The DDL Library](#the-ddl-library)

<!-- /TOC -->

<a name="introduction"></a>
## Introduction

The DDL (Data Definition Language) was introduced with the software ADTF 
(Automotive Data- and Time-Triggered Framework) developed and 
deployed by AUDI AG/VW Group since many years. 

The intension was to have a common dynamic description for byte positions 
and value types for any structured data and memory blocks.
Since the software was designed for C++ applications the DDL is similar to it and 
uses there data types by default. 

### Example

*DDL description for data of following struct:*

     struct MyData // => usually aligned size to 4 in c++ because value_3 is aligned to 4
     {
          uint8_t  value_1; // => usually aligned to 1 in c++
          uint16_t value_2; // => usually aligned to 2 in c++
          uint8_t  value_3; // => usually aligned to 1 in c++
          uint32_t value_3; // => usually aligned to 4 in c++
     };

*Description in DDL 4.1 as is (in XML):*
* use of default alignments and byteorders
* use of default byteorder "little endianess"
````xml  
<struct name="MyData12Byte" alignment="4">
     <element name="value_1" type="uint8_t">
        <serialized bytepos="0" byteorder="LE"/>
        <deserialzed alignment="1"/>
     </element>
     <element name="value_2" type="uint16_t">     
        <serialized bytepos="1" byteorder="LE"/>
        <deserialzed alignment="2"/>
     </element>
     <element name="value_3" type="uint8_t">
        <serialized bytepos="3" byteorder="LE"/>
        <deserialzed alignment="1"/>
     </element>
     <element name="value_4" type="uint32_t">
        <serialized bytepos="4" byteorder="LE"/>
        <deserialzed alignment="4"/>
     </element>
</struct>
````

<table>
  <tr>
     <td colspan="12"><b>deserialized representation</b>(12 bytes in memory)</td>
  </tr>
  <tr>
    <td>byte 0</td>
    <td>byte 1</td>
    <td>byte 2</td>
    <td>byte 3</td>
    <td>byte 4</td>
    <td>byte 5</td>
    <td>byte 6</td>
    <td>byte 7</td>
    <td>byte 8</td>
    <td>byte 9</td>
    <td>byte 10</td>
    <td>byte 11</td>
  </tr>
  <tr>
    <td colspan="1">uint8_t value_1</td>
    <td colspan="1">(padding)</td>
    <td colspan="2">uint16_t value_2</td>
    <td colspan="1">uint8_t value_3</td>
    <td colspan="3">(padding)</td>
    <td colspan="4">uint32_t value_4</td>
  </tr>
</table>
<table>
  <tr>
     <td colspan="8"><b>serialized representation</b> (8 bytes in network or file)</td>
  </tr>
  <tr>
    <td>byte 0</td>
    <td>byte 1</td>
    <td>byte 2</td>
    <td>byte 3</td>
    <td>byte 4</td>
    <td>byte 5</td>
    <td>byte 6</td>
    <td>byte 7</td>
  </tr>
   <tr>
    <td colspan="1">uint8_t value_1</td>
    <td colspan="2">uint16_t value_2</td>
    <td colspan="1">uint8_t value_3</td>
    <td colspan="4">uint32_t value_4</td>
  </tr>
</table>

<a name="the-ddl-library"></a>
## The DDL Library

To facilitate the handling of DDL hierarchies there is a datamodel provided (ddl::dd::datamodel::DataDefinition).
This datamodel is validated by a ddl::dd::DataDefinition which keeps the datamodel clean and well-formed.

The datamodel itself and its specification is described within [DDL - XML Datamodel specification](ddl_specification.md).

Use the datamodel to handle the model by yourself. We recommend to use following DataDefinition creation API.

### DataDefinition creation API

There are 3 ways to create a valid DataDefinition.

#### Create a file or read it

* See ddl::DDFile

#### Create a string or read it

* See ddl::DDString

#### Create a DataDefinition via DDStructure or "type reflection"

* See ddl::DDStructure
* See ddl::DDStructureGenerator

### Binary model classes

| DDL Tag/Attribute | DDL Class |
| ----------------- | --------- |
|`<ddl:ddl>`|ddl::dd::DataDefinition|
|`<header>`|ddl::dd::Header|
|`<baseunit>`|ddl::dd::BaseUnit|
|`<prefix>`|ddl::dd::UnitPrefix|
|`<unit>`|ddl::dd::Unit|
|`<datatype>`|ddl::dd::DataType|
|`<enum>`|ddl::dd::EnumType|
|`<struct>`|ddl::dd::StructType|
|`<streammetatype>`|ddl::dd::StreamMetaType|
|`<stream>`|ddl::dd::Stream|



