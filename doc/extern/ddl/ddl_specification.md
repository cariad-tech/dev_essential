<!---
Copyright @ 2021 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
-->
<a name="ddl---xml-datamodel-specification"></a>
# DDL - XML Datamodel specification

<!-- TOC updateonsave:false depthfrom:1 depthto:2 orderedlist:false -->

- [DDL - XML Datamodel specification](#ddl---xml-datamodel-specification)
    - [Versioning](#versioning)
- [Specification](#specification)
    - [Description of General Datatypes in this specification](#description-of-general-datatypes-in-this-specification)
    - [header](#header)
    - [units](#units)
    - [datatypes](#datatypes)
    - [enums](#enums)
    - [structs](#structs)
    - [dynamic arrays](#dynamic-arrays)
    - [streammetatypes](#streammetatypes)
    - [streams](#streams)

<!-- /TOC -->


With DDL (Data Definition Language) a xml datamodel was introduced to ***describe*** the semantics of any data that is exchanged between arbitrary software components. The term ***description*** is used for a string or a file containing this xml datamodel.

*The exchange of the data may be:*

* sending data to or receiving data from a device
* sending data to or receiving data from another software component
* data stored within a binary file format 
 
The data must have one common characteristic to describe them with the help of DDL: Each data are is a contiguously byte array within memory.

<a name="versioning"></a>
## Versioning

The DDL was introduced with version 1.0+, current version is 4.1. 

### DDL Versions

An overview of changes in specification between the language versions is given below.
For detailed information the changes and language features are explained in chapter <a href="#specification">Specification</a>.

| DDL Version | Description of changes |
| ----------- | ---------------------- |
| 1.0         | first concept DDL version, never released |
| 1.0+ (1.01) | first released DDL version<br> - add *alignment* to *struct*<br> - change optional *bitpos* default from "1" to "0" |
| 1.02        | XML specification type changes: change some tags from *CHAR* to *STRING* and *STRING* to *CLASS* |
| 2.0         | - introduce support for dynamic arrays <br>- add constant *value* attribute for *element* |
| 3.0         | - change *alignment* behaviour for *struct* (now it influences the size of the struct in memory)<br>- add *ddl_version* attribute for *struct*<br>- introduce *min*, *max*, *default*, *scale*, *offset* to *element* |
| 4.0         | - rearange the *element* tag with new sub-tag *serialized* *deserialized*<br>- change *unit* in *element* from mandatory to optional<br>- introduce *stream_meta_types* |
| 4.1         | - add std types to the predefined datatypes ("bool", "uint8_t", "int8_t", ... "float", "double") |

<a name="specification"></a>
# Specification

The newer versions of the DDL Definition File Format are also specified by a XSD specification file.
See the XSD specification for further details:
- [DDL 3 XSD specification](specification/ddl3.xsd)
- [DDL 4 XSD specification](specification/ddl4.xsd)

<a name="description-of-general-datatypes-in-this-specification"></a>
## Description of General Datatypes in this specification

There are several types that are used in attributes and as tag data in the
DDL. The following table lists the types with a description of the values
and/or their formatting.

| Type | Allowed values |
| ---- | -------------- |
|CHAR|single character of [a-z][A-Z][0-9] _.-+/|
|STRING|characters of [a-z][A-Z][0-9] _.-+/|
|TEXT|all visible ASCII characters|
|UINT|unsigned integer values|
|INT|signed integer values|
|FLOAT|signed float values|
|DATE|allowed formats: yyyymmdd, dd-mm-yyyy, yyyy-mm-dd or dd.mm.yyyy|
|CLASS|any name value of another DDL element|
|ENUM|a predefined value from the description of the attribute|
|DDLVERSION|Version of 1.0/1.01/1.0+/1.02/2.0/3.0/4.0/4.1|

<a name="header"></a>
## header

The header section contains meta information about the document and version
information. Example:
````xml
<header>
    <language_version>4.1</language_version>
    <author>dev_essential team</author>
    <date_creation>20100407</date_creation>
    <date_change />
    <description>DDL Description File</description>
</header>
````

| Tag | Type | Description |
| --- | ---- | ----------- |
|language_version|DDLVERSION|Version number of the file|
|author|STRING|Author|
|date_creation|DATE|Creation date|
|date_change|DATE|Last modification date|
|description|TEXT|Short description|


All previously mentioned header tags are *mandatory*.

Additional information can be added by using the `ext_declaration` tag.
Example:

````xml
<header>
    ...
    <ext_declaration key="AnyKey" value="Any value for this key"/>
</header>
````

&nbsp;

| Attribute | Type | Description |
| --------- | ---- | ----------- |
|key|STRING|Name of the additional information|
|Value|TEXT|Value of the additional information|

&nbsp;

<a name="units"></a>
## units

The definition of units will be divided in SI-base units and own units. The
SI-based units are the following ones:

| SI (see ISO 1000) | no SI, but needed for daily usage |
| ----------------- | --------------------------------- |
|Metre|Degree|
|Gram|Radiant|
|Second|Unitless|
|Ampere| |
|Candela| |
|Kelvin| |
|Mole| |

Units are defined within the tags `<units>` and `</units>`.
Example:
````xml
<units>
    <baseunit description="Fundamental unit for electric current" name="Ampere" symbol="A" />
    <baseunit description="Fundamental unit for luminous intensity" name="Candela" symbol="cd" />
    <baseunit description="Non-SI standard unit for angle" name="Degree" symbol="deg" />
    <baseunit description="Fundamental unit for thermodynamic temperature" name="Kelvin" symbol="K" />
    <baseunit description="Fundamental unit for mass" name="Kilogram" symbol="kg" />
    <baseunit description="Fundamental unit for length" name="Metre" symbol="m" />
    <baseunit description="Fundamental unit for amount of substance" name="Mole" symbol="mol" />
    <baseunit description="Non-SI standard unit for angle" name="Radiant" symbol="rad" />
    <baseunit description="Fundamental unit for time" name="Second" symbol="s" />
    <baseunit description="No SI, but needed for own unit definitions" name="Unitless" symbol="" />

    <prefixes name="atto" power="-18" symbol="a" />
    <prefixes name="centi" power="-2" symbol="c" />
    <prefixes name="deca" power="1" symbol="da" />
    <prefixes name="deci" power="-1" symbol="d" />
    <prefixes name="exa" power="18" symbol="E" />
    <prefixes name="femto" power="-15" symbol="f" />
    <prefixes name="giga" power="9" symbol="G" />
    <prefixes name="hecto" power="2" symbol="h" />
    <prefixes name="kilo" power="3" symbol="k" />
    <prefixes name="mega" power="6" symbol="M" />
    <prefixes name="micro" power="-6" symbol="u" />
    <prefixes name="milli" power="-3" symbol="m" />
    <prefixes name="nano" power="-9" symbol="n" />
    <prefixes name="peta" power="15" symbol="P" />
    <prefixes name="pico" power="-12" symbol="p" />
    <prefixes name="tera" power="12" symbol="T" />
    <prefixes name="yocto" power="-24" symbol="y" />
    <prefixes name="yotta" power="24" symbol="Y" />
    <prefixes name="zepto" power="-21" symbol="z" />
    <prefixes name="zetta" power="21" symbol="Z" />
</units>
````


### baseunits

A concrete base unit definition will be specified by the tag
`<baseunit>` and `</baseunit>`

The baseunit needs the following mandatory attributes:

| Name | Type | Required | Default | Description |
| ---- | ---- | -------- | ------- | ----------- |
|name|STRING|mandatory| | Name of the base unit e.g. "metre" |
|symbol|STRING|mandatory| | Symbol of the base unit e.g. "m" |
|description|TEXT|optional| "" |Description of the represented base unit|


### prefixes

Prefixes between 10 power(-24) and 10 power(24) are predefined.
A prefix can be defined by the `<prefixes>` tag.

Every `<prefixes>` tag needs the following mandatory attributes:

| Name | Value | Required | Description | changes between 1.02 and 1.0+ |
| ---- | ----- | -------- |------------ | ----------------------------- |
|name|STRING|mandatory|Name of the prefix| |
|symbol|STRING|mandatory|Represents a short symbol e.g. k|changed to STRING from CHAR in DDL1.02|
|power|INT|mandatory|Defines the power of the prefix| |


### units
A self defined unit is specified within the `<unit>` and `</unit>`
tag and needs the following mandatory attributes:

| Name | Value | Required | Description |
| ---- | ----- | -------- | ----------- |
|name|STRING|mandatory|Name of the new unit|


The `<unit>` tags needs the following mandatory sub-tags:

| Name    | Value  | Required | Description |
| ------- | ------ | -------- | ----------- |
|numerator| STRING |mandatory|containing pi/PI or a floating-point value (Numerator of the new unit related to the baseunits)|
|denominator|STRING|mandatory|containing pi/PI or a floating-point value. The value '0' is not defined. (Denominator of the new unit related to the baseunits)|
|offset|FLOAT|mandatory|Offset to the baseunits|


The new unit is able to use several base units. To represent this, it is
possible to specify the related base units by the `<refUnit>` tag. This
tag uses the following mandatory attributes:

| Name | Value | Description | changes between 1.02 and 1.0+ |
| ---- | ----- |------------ | ----------------------------- |
|name|CLASS|The referenced unit|changed to CLASS from STRING in DDL 1.02|
|power|INT|Power of the new unit related to the base one| |
|prefix|CLASS|Reference to the prefix to use|changed to CLASS from STRING in DDL1.02|


### Calculation of new unit

The newly defined unit relates to the SI base units like this:

newUnit = offset + (numerator / denominator) * Product (prefix(n) * baseUnit(n) ^ power(n))


### Predefined units

The following base units are provided as default:

| Base Unit Name | Description | Symbol |
| -------------- | ----------- | ------ |
|Metre|Fundamental unit for length|m|
|Kilogram|Fundamental unit for mass|kg|
|Second|Fundamental unit for time|s|
|Ampere|Fundamental unit for electric current|A|
|Kelvin|Fundamental unit for thermodynamic temperature|K|
|Mole|Fundamental unit for amount of substance|mol|
|Candela|Fundamental unit for luminous intensity|cd|
|Degree|Non-SI standard unit for angle|deg|
|Radiant|Non-SI standard unit for angle|rad|
|Unitless|No SI, but needed for own unit definitions| |
|nou|No SI, but needed for no unit definitions| |

The following prefixes are provided as default:

| Prefix Name | Power | Symbol |
| ----------- | ----- | ------ |
|yotta|24|Y|
|zetta|21|Z|
|exa|18|E|
|peta|15|P|
|tera|12|T|
|giga|9|G|
|mega|6|M|
|kilo|3|k|
|hecto|2|h|
|deca|1|da|
|deci|-1|d|
|centi|-2|c|
|milli|-3|m|
|micro|-6|u|
|nano|-9|n|
|pico|-12|p|
|femto|-15|f|
|atto|-18|a|
|zepto|-21|z|
|yocto|-24|y|

<a name="datatypes"></a>
## datatypes

This section describes the primitive data types which can be used within the
struct elements. Example:

````xml
<datatypes>
    <datatype description="predefined tBool datatype"  size="8" name="tBool" />
    <datatype description="predefined tChar datatype"  size="8" name="tChar" />
    <datatype description="predefined tUInt8 datatype" size="8" name="tUInt8" />
    <datatype description="predefined tInt8 datatype"  size="8" name="tInt8" />
    ...
</datatypes>
````

&nbsp;

| Name | Type | Required | Description | changes between 2.0 and 3.0 | changes between 1.0+ and 1.0 |
| ---- | ---- | -------- | ----------- | --------------------------- | ---------------------------- |
|name|STRING|mandatory|Name of the primitive data type|attribute name changed to "name" from "type"| |
|size|UINT|mandatory|Number of bits (relevant for serialization)| | |
|description|STRING|optional|Description of the primitive data type| | |
|arraysize|UINT|ignored|= 1 -> primitive presentation> 1 -> array with number of elements. ***This feature is not supported within any DDL.***| | |
|unit|CLASS|optional|Referenced unit of the data type| | |
|min|STRING|optional|Minimum value of the data type| introduced in DDL 3.0| |
|max|STRING|optional|Maximum value of the data type| introduced in DDL 3.0| |


### The following predefined data types are automatically used within DDL:

The following predefined data types are automatically used within DDL. Since DDL 4.1 also the standard data type names can be used.

| Type | also since DDL 4.1 | Description | Number of bits |
| ---- | ------------------ | ----------- | -------------- |
|tBool|bool|boolean|8 (C++ data type)|
|tChar|char|character|8|
|tInt8|int8_t|signed integer|8|
|tUInt8|uint8_t|unsigned integer|8|
|tInt16|int16_t|signed integer|16|
|tUInt16|uint16_t|unsigned integer|16|
|tInt32|int32_t|signed integer|32|
|tUInt32|uint32_t|unsigned integer|32|
|tInt64|int64_t|signed integer|64|
|tUInt64|uint64_t|unsigned integer|64|
|tFloat32|float|IEEE Float|32|
|tFloat64|double|IEEE Float|64|

<a name="enums"></a>
## enums

This section describes the enum type which can be used within the struct
elements. Example:

````xml
<enums>
    <enum name="tValueDefinitions" type="tUInt32">
        <element name="ELEMENT_ONE" value="10"/>
        <element name="ELEMENT_TWO" value="20"/>
        <element name="ELEMENT_THREE" value="5"/>
    </enum>
    ...
</enums>
<struct alignment="1" name="tEnumData" version="1">
    <element alignment="1" arraysize="1" byteorder="LE" bytepos="0" name="enumData" type="tValueDefinitions" />
</struct>
````

&nbsp;

Enum Attributes

| Name | Type | Required | Description |
| ---- | ---- | -------- | ----------- |
|name|STRING|mandatory|Name of the enum|
|type|CLASS|mandatory|Referenced data type of the enum|


Enum Element Attributes

| Name | Type | Required | Description |
| ---- | ---- | -------- | ----------- |
|name|STRING|mandatory|Name of the element|
|value|INT or UINT|mandatory|Value of the element|


**Remarks:**
- An enum is also valid without enum elements.

&nbsp;

### constants

This section describes constants which are implemented using the enum type. Example:

````xml
<enums>
    <enum name="tConstants" type="tUInt32">
        <element name="CONSTANT_ONE" value="42"/>
        <element name="CONSTANT_TWO" value="10"/>
    </enum>
    ...
</enums>
<struct alignment="1" name="tEnumData" version="1">
    <element arraysize="1" name="enumData" type="tConstants" value="CONSTANT_ONE" />
        <serialzed byteorder="LE" bytepos="0"/>
        <deserialzed alignment="1"/>
    </element>
</struct>
````

&nbsp;

<a name="structs"></a>
## structs

The definition of structs allows to build complex data types. Example:

````xml
<!-- all element alignments and bytepos must be set in 4.1 -->
<struct alignment="4" name="tTest" version="1" ddlversion="4.1">
    <element name="bBool" type="bool" arraysize="1">
        <serialized byteorder="LE" bytepos="0" bitpos="0"/>
        <deserialized alignment="1"/>
    </element>
    <element name="nInt8" type="uint8_t" arraysize="1">
        <serialized byteorder="LE" bytepos="1" bitpos="0"/>
        <deserialized alignment="1"/>
    </element>
    <element name="nUInt32" type="uint32_t" arraysize="1">
        <serialized byteorder="LE" bytepos="2" bitpos="0"/>
        <deserialized alignment="4"/>
    </element>
    <element name="fFloat32" type="float" arraysize="1">
        <serialized byteorder="LE" bytepos="6" bitpos="0"/>
        <deserialized alignment="4"/>
    </element>
</struct>
````

The tag `<struct>` uses the following attributes:

| Name | Type | Required | Description | changes between 2.0 and 3.0 | changes between 1.0+ and 1.0 |
| ---- | ---- | -------- | ----------- | --------------------------- | ---------------------------- |
|name|STRING|mandatory|Description of the data type| | |
|version|UINT|mandatory|Version number of the specified struct type, by default is "1" | | |
|comment|TEXT|optional|Additional comments| | |
|alignment|ENUM of 1/2/4/8/16/32/64 (default is "1")|optional|Alignment value to get the whole packing of the complex data type which is important to get the calculated size of the structure (relevant for serialization)| From version 3.0 on, the alignment influences the size of the struct. The size will always be a multiple of the alignment.|introduced in DDL 1.0+|
|ddlversion|STRING|optional|The version of the size calculation scheme, see alignment. If not specified the version from the containing definition will be used.| | |


**Remarks:**
- If the alignment tag is not set within version 4.1, the alignment will be set to 1 by default!

The tag `<element>` uses the following attributes:

<table>
<tr>
   <th> Name </th><th> Type </th><th> Required </th>
   <th> Description </th>
   <th> changes between 3.0 and 4.0 </th>
   <th> changes between 2.0 and 3.0 </th>
   <th> changes between 1.02 and 2.0 </th>
   <th> changes between 1.02 and 1.0+ </th>
   <th> changes between 1.0+ and 1.0 </th>
</tr>
<tr>
   <td> type </td><td> CLASS </td><td> mandatory </td>
   <td> Reference to an existing data type </td>
   <td> </td>
   <td> </td>
   <td> </td>
   <td> </td>
   <td> </td>
</tr>
<tr>
   <td> name </td><td> STRING </td><td> mandatory </td>
   <td> Name of the element </td>
   <td> </td>
   <td> </td>
   <td> </td>
   <td> </td>
   <td> </td>
</tr>
<tr>
   <td> bytepos </td><td> UINT </td><td> deprecated </td>
   <td> Byte position of the data in the serialized representation of the containing struct. This is NOT relevant for the struct layout in memory (deserialized)! Started with '0' Elements following a dynamic arraymust have a byte pos of '-1' </td>
   <td> From version 4.0 on this information is specified within the `<serialized>` tag. </td>
   <td> </td>
   <td> </td>
   <td> </td>
   <td> </td>
</tr>
<tr>
   <td> bitpos </td><td> UINT </td><td> deprecated </td>
   <td>Bit position of the data in the serialized representation of the containing struct. This is NOT relevant for the struct layout in memory (deserialized)! default = 0 (in the range of 0 to 7) (relevant for serialization) </td>
   <td> From version 4.0 on this information is specified within the `<serialized>` tag. </td>
   <td> </td>
   <td> </td>
   <td> </td>
   <td>default value changes from 1 to 0 in DDL1.0+ </td>
</tr>
<tr>
   <td> numbits </td><td> UINT </td><td> deprecated </td>
   <td>Specifies the amount of bits used in the serialized representation, if not set the size of the type will be used. e.g. tInt8 with numbits of 7 (numbits can only be used to non-arrays)This is NOT relevant for the struct layout in memory (deserialized)! </td>
   <td> From version 4.0 on this information is specified within the `<serialized>` tag. </td>
   <td> </td>
   <td> </td>
   <td> </td>
   <td> </td>
</tr>
<tr>
   <td> byteorder </td><td> ENUM of LE/BE/Motorola/Intel </td><td> deprecated </td>
   <td>Defines the byte order in the serialized representation. </td>
   <td> From version 4.0 on this information is specified within the `<serialized>` tag. </td>
   <td> </td>
   <td> </td>
   <td> </td>
   <td> </td>
</tr>
<tr>
   <td> alignment </td><td> ENUM of 1/2/4/8/16/32/64 </td><td> deprecated </td>
   <td> Defines the alignment of the element in the deserialized representation. </td>
   <td> From version 4.0 on this information is specified within the `<deserialized>` tag. </td>
   <td> </td>
   <td> </td>
   <td> </td>
   <td> </td>
</tr>
<tr>
   <td> description </td><td> STRING </td><td> optional </td>
   <td> Description of the created data type. </td>
   <td> </td>
   <td> </td>
   <td> </td>
   <td> </td>
   <td> </td>
</tr>
<tr>
   <td> unit </td><td> CLASS </td><td> optional </td>
   <td> Referenced unit of the element </td>
   <td> </td>
   <td> Changed to optional </td>
   <td> </td>
   <td> </td>
   <td> </td>
</tr>
<tr>
   <td> comment </td><td> TEXT </td><td> optional </td>
   <td> Additional comments (may be used as documentation comment in code generation)</td>
   <td> </td>
   <td> </td>
   <td> </td>
   <td> </td>
   <td> </td>
</tr>
<tr>
   <td> arraysize </td><td> UINT or STRING </td><td> optional </td>
   <td> Defines the array size of the element. The default is "1". Starting with DDL 2.0 the name of a preceding struct element can be used to specify a dynamic array.arraysize="elementName" For a detailed explanation of dynamic arrays refer to *dynamic arrays*</td>
   <td> </td>
   <td> </td>
   <td> </td>
   <td> Dynamic array support </td>
   <td> </td>
</tr>
<tr>
   <td> value </td><td> CLASS </td><td> optional </td>
   <td> Reference to an enum element as constant value for struct element</td>
   <td> </td>
   <td> </td>
   <td> </td>
   <td> introduced in DDL 2.0 </td>
   <td> </td>
</tr>
<tr>
   <td> min </td><td> STRING </td><td> optional </td>
   <td> Minimum value of the element</td>
   <td> </td>
   <td> </td>
   <td> introduced in DDL 3.0 </td>
   <td> </td>
   <td> </td>
</tr>
<tr>
   <td> max </td><td> STRING </td><td> optional </td>
   <td> Maximum value of the element</td>
   <td> </td>
   <td> </td>
   <td> introduced in DDL 3.0 </td>
   <td> </td>
   <td> </td>
</tr>
<tr>
   <td> default </td><td> STRING </td><td> optional </td>
   <td> Default value of the element</td>
   <td> </td>
   <td> </td>
   <td> introduced in DDL 3.0 </td>
   <td> </td>
   <td> </td>
</tr>
<tr>
   <td> scale </td><td> STRING </td><td> optional </td>
   <td> Scaling value of the element</td>
   <td> </td>
   <td> </td>
   <td> introduced in DDL 3.0 </td>
   <td> </td>
   <td> </td>
</tr>
<tr>
   <td> offset </td><td> STRING </td><td> optional </td>
   <td> Offset value of the element</td>
   <td> </td>
   <td> </td>
   <td> introduced in DDL 3.0 </td>
   <td> </td>
   <td> </td>
</tr>
</table>


The tag `<serialized>` uses the following attributes:
<table>
<tr>
   <th> Name </th><th> Type </th><th> Required </th>
   <th> Description </th>
   <th> changes between 3.0 and 4.0 </th>
</tr>
<tr>
   <td> bytepos </td><td> UINT </td><td> mandatory </td>
   <td> Byte position of the data in the serialized representation of the containing struct. This is NOT relevant for the struct layout in memory (deserialized)!Usually start with '0'. For dynamic elements following a dynamic array must have a byte pos of "-1".</td>
   <td> introduced within the `<deserialized>` tag </td>
</tr>
<tr>
   <td> bitpos </td><td> UINT </td><td> optional </td>
   <td> Bit position of the data in the serialized representation of the containing struct. This is NOT relevant for the struct layout in memory (deserialized)! default = *0* (in the range of *0* to *7*) (relevant for serialization).</td>
   <td> introduced within the `<deserialized>` tag </td>
</tr>
<tr>
   <td> numbits </td><td> UINT </td><td> optional </td>
   <td> Specifies the amount of bits used in the serialized representation, if not set the size of the type will be used. e.g. tInt8 with numbits of 7 (numbits can only be used to non-arrays). This is NOT relevant for the struct layout in memory (deserialized)!</td>
   <td> introduced within the `<deserialized>` tag </td>
</tr>
<tr>
   <td> byteorder </td><td> ENUM of LE/BE/Motorola/Intel </td><td> mandatory </td>
   <td> Defines the byte order in the serialized representation.</td>
   <td> introduced within the `<deserialized>` tag </td>
</tr>
</table>


The tag `<deserialized>` uses the following attributes:

| Name | Type | Required | Description |
| ---- | ---- | -------- | ----------- |
|alignment|Enum of 0/1/2/4/8/16/32/64|mandatory|Defines the alignment of the element in the deserialized representation.|

**Remarks:**
- The number of structs defined in a DDL description file is not limited.
- If the name of another struct is used as type for an element, a hierarchical structure is created.

### Alignment of structs and in structs

This section will explain the different meanings of alignment used in the DDL. Since alignment
is only needed for deserialized representation, this section will only consider this kind of
representation.

Inside a struct, every element has a explicit alignment value. This value influences, at what
position the element will be placed inside the struct. The memory placement is initially
determined by the order of the element tags inside the struct. After that the alignment takes
effect. An element inside a struct will only be placed at memory positions that are multiples
of the alignment value. The considered memory address is always relative to the beginning of the
struct (memory position 0).
As an example we assume having the following struct definition:

````xml
<struct alignment="4" name="tStruct" version="1">
    <element name="ui8Array" type="tUInt8" arraysize="5">
        <serialized byteorder="LE" bytepos="0"/>
        <deserialized alignment="1"/>
    </element>
    <element name="ui32Value" type="tUInt32" arraysize="1">
        <serialized byteorder="LE" bytepos="5"/>
        <deserialized alignment="4"/>
    </element>
</struct>
````

In this case, ui8Array will be placed at address 0. Because of the order of the element tags,
ui32Value has to be placed after ui8Array. The next memory position after ui8Array is 5. But
since the element ui32Value has an alignment of 4 and therfore the memory address has to be a
multiple of 4, ui32Value will be placed at address 8 since this is the first address to matches
all requirements. The attibute bytepos is only valid for serialized representation.

The alignment of a struct does not affect the alignment of its elements. A struct's alignment only
determines the positioning of this struct inside of arrays.
Assuming we have the following struct:

````xml
<struct alignment="4" name="tInnerStruct" version="1">
    <element name="ui8Value1" type="tUInt8" arraysize="1">
        <serialized byteorder="LE" bytepos="0"/>
        <deserialized alignment="1"/>
    </element>
    <element name="ui8Value2" type="tUInt8" arraysize="1">
        <serialized byteorder="LE" bytepos="1"/>
        <deserialized alignment="1"/>
    </element>
</struct>
````

Lets now assume we have another struct `tOuterStruct`, that contains an array element and the
element's type is the struct `tInnerStruct`:

````xml
<struct alignment="1" name="tOuterStruct" version="1">
    <element name="aValue" type="tInnerStruct" arraysize="5">
        <serialized byteorder="LE" bytepos="0"/>
        <deserialized alignment="1"/>
    </element>
</struct>
````

Inside the array, the array element's positions are influenced by the alignment of the type
(`tInnerStruct`). This means that the first array element is at position 0, in relation to the
beginning of the array. The next free memory address would be 2, but since the alignment of the
struct is 4, the memory address has to be a multiple of 4 and the second element of the array now
is situated at memory address 4. The third will be at 8, the fourth at 12 and so on.

#### Why should I use anything else but an alignment of 1?

If for example you are planning to describe data with DDL that was originally defined as a struct
in a header file, you should also define the alignment values inside your DDL file accordingly.
If on the other hand you are shure, that the data you are using is always interpreted using DDL,
there is no need to use an alignment other than 1.

#### What consequences do the changes in DDL 3.0 have regarding alignment?

The changes in DDL 3.0 regarding alignment are meant to make it more easy to describe structs
defined in C(++). Since C(++) does not use alignment but packing for its structs, the alignment
in DDL now also influences the size of a struct defined in DDL (see new definition of alignment
in DDL 3.0). This means that arrays and structs may now become bigger in DDL 3.0 as they were in
DDL 2.x. For example the struct

````xml
<struct alignment="2" name="tFirstStruct" version="1" ddlversion="2.0/3.0">
    <element name="ui8Value" type="tUInt8" arraysize="1">
        <serialized byteorder="LE" bytepos="0"/>
        <deserialized alignment="1"/>
    </element>
</struct>
````

has the deserialized size of 1 byte in DDL 2.x and a deserialized size of 2 byte in DDL 3.0.
The struct

````xml
<struct alignment="1" name="tSecondStruct" version="1" ddlversion="2.0/3.0">
    <element name="aValue" type="tFirstStruct" arraysize="3">
        <serialized byteorder="LE" bytepos="0"/>
        <deserialized alignment="1"/>
    </element>
</struct>
````

has a size of 5 bytes in DDL 2.x and a size of 6 Bytes in DDL 3.0.
This is due to the fact, that in 2.x the array looks like this:

| Bytepos | Element |
| ------- | ------- |
|0|aValue[0]|
|1|padding|
|2|aValue[1]|
|3|padding|
|4|aValue[2]|


resulting in a size of 5 bytes.
The padding elements are inserted, so that the *following* Element is correctly aligned.
In DDL 3.0 the `tFirstStruct` has already a padding alement attached to its end so that its
size matches the requirements of the alignment attribute (multiple of 2). Therefore, the array
in 3.0 will look like this:

| Bytepos | Element |
| ------- | ------- |
|0 to 1|aValue[0]|
|2 to 3|aValue[1]|
|4 to 5|aValue[2]|

resulting in a size of 6 bytes.

#### Changing from DDL 2.x to 3.0

Assuming you have a couple of structs defined in DDL 2.x, if you now plan to change the language
version from 2.x to 3.0, that you are creating new structs, that may not be compatible to its
namesakes defined in DDL 2.x. Its better to use new names for the structs to differentiate
between the different versions.

<a name="dynamic-arrays"></a>
### dynamic arrays

Detailed explanation of the dynamic array functionality.


Dynamic arrays were introduced in DDL 2.0.

**Attention:**

The use of dynamic arrays will cause a severe performance drop
compared to static arrays.
It is therefore recommended to use static arrays whenever possible.

To minimize the performance impact of dynamic arrays the user should adhere
to the following guidelines:
- Always place dynamic data the end of a structure or structure hirarchy so
all static data precede the dynamic data.
- Use the struct element directly preceding the dynamic array for array size.
- Use dynamic arrays of primitive data types instead of complex data types.
- Prefer flat data over deeply structured data.
- Do not use nested dynamic arrays!
- Use `alignment="1"` for all elements.
- Use default values bitpos.
- Use a bytepos that matches the position in memory when alignment=1.

Dynamic Arrays are always a performance issue for the codecs. Address information 
of the elements are evaluated for the dynamic part each time a decoder/codec is initialized.

#### Fastest way to use dynamic arrays:

````xml
<struct alignment="1" name="tDynStruct" version="1">
    <element name="ui32SomeData" type="tUInt32" arraysize="1">
        <serialized byteorder="LE" bytepos="0"/>
        <deserialized alignment="1"/>
    </element>
    <element name="ui32DynArraySize" type="tUInt32" arraysize="1">
        <serialized byteorder="LE" bytepos="4"/>
        <deserialized alignment="1"/>
    </element>
    <element name="f64DynamicArray" type="tFloat64" arraysize="ui32DynArraySize">
        <serialized byteorder="LE" bytepos="8"/>
        <deserialized alignment="1"/>
    </element>
</struct>
````

#### When the dynamic data is not the last element the bytepos of the following elements must be -1

````xml
<struct alignment="1" name="tDynStruct" version="1">
    <element name="ui32DynArraySize" type="tUInt32" arraysize="1">
        <serialized byteorder="LE" bytepos="0"/>
        <deserialized alignment="1"/>
    </element>
    <element name="f64DynamicArray" type="tFloat64" arraysize="ui32DynArraySize">
        <serialized byteorder="LE" bytepos="4"/>
        <deserialized alignment="1"/>
    </element>
    <element name="ui32SomeData" type="tUInt32" arraysize="1">
        <serialized byteorder="LE" bytepos="-1"/>
        <deserialized alignment="1"/>
    </element>
</struct>
````

#### Dynamic array of complex data:

````xml
<struct alignment="1" name="tVector" version="1">
    <element name="f64X" type="tFloat64" arraysize="1">
        <serialized byteorder="LE" bytepos="0"/>
        <deserialized alignment="1"/>
    </element>
    <element name="f64Y" type="tFloat64" arraysize="1">
        <serialized byteorder="LE" bytepos="8"/>
        <deserialized alignment="1"/>
    </element>
    <element name="f64Z" type="tFloat64" arraysize="1">
        <serialized byteorder="LE" bytepos="16"/>
        <deserialized alignment="1"/>
    </element>
</struct>
<struct alignment="1" name="tDynStruct" version="1">
    <element name="ui32SomeData" type="tUInt32" arraysize="1">
        <serialized byteorder="LE" bytepos="0"/>
        <deserialized alignment="1"/>
    </element>
    <element name="ui32DynArraySize" type="tUInt32" arraysize="1">
        <serialized byteorder="LE" bytepos="4"/>
        <deserialized alignment="1"/>
    </element>
    <element name="tVecDynamicArray" type="tVector" arraysize="ui32DynArraySize">
        <serialized byteorder="LE" bytepos="8"/>
        <deserialized alignment="1"/>
    </element>
</struct>
````


<a name="streammetatypes"></a>
## streammetatypes

Stream Meta Types are defined within the tags `<streammetatypes>` and `</streammetatypes>`.
Example:

````xml
<streammetatypes>
  <streammetatype name="ddl/default" version="1">
      <property name="md_struct" type="string"/>
      <property name="md_definitions" type="string"/>
      <property name="md_data_serialized" type="bool"/>
  </streammetatype>
  <streammetatype name="ddl/video" version="1">
      <property name="width" type="tInt32"/>
      <property name="height" type="tInt32"/>
      <property name="format" type="string"/>
  </streammetatype>
  <streammetatype name="test" version="1" parent="adtf/default">
      <property name="test_prop" type="tInt32"/>
  </streammetatype>
</streammetatypes>
````

The tag `<streammetatype>` uses the following attributes:

| Name | Type | Required | Description |
| ---- | ---- | -------- | ----------- |
|name|STRING|mandatory|The identifier of the stream meta type|
|version|STRING|optional|Version information of the specified stream meta type i.e. "ant", "bat", ...|
|parent|TEXT|optional|Identifier of a parent stream meta type|

The tag `<property>` uses the following attributes:

| Name | Type | Required | Description |
| ---- | ---- | -------- | ----------- |
|name|STRING|mandatory|The name of the property|
|type|STRING|mandatory|The type of the property|

<a name="streams"></a>
## streams


````xml
<streams>
  <stream name="video" type="ddl/video" description="Video stream within a file described via stream type ddl/video" />
  <stream name="my_data" version="1" parent="adtf/default">
    <struct bytepos="0" type="tTest" />
  </stream>
</streams>
````

Streams are defined within the tags `<streams>` and `</streams>`.
They describe named instances of streams with concrete data content and a type of stream. 
Usually this stream description is only necessary to describe the content of a streaming file.

The tag `<stream>` uses the following attributes:

| Name | Type | Required | Description |
| ---- | ---- | -------- | ----------- |
|name|STRING|mandatory|The name of the stream|
|type|CLASS|mandatory|Referenced type of the stream (Is a reference to a Stream Meta Type or a Struct.)|
|description|STRING|optional|The description of the stream|

The optional tag `<struct>` uses the following attributes: 
This tag should  only be used, if 

| Name | Type | Required | Description |
| ---- | ---- | -------- | ----------- |
|bytepos|UINT|mandatory|The data described |
|type|CLASS|mandatory|The structured type used for the data area of the stream (Is a reference to a a Struct Type.)|
|name|STRING|optional|A name for the struct|
