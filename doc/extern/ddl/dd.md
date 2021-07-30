<!---
Copyright @ 2021 VW Group. All rights reserved.
 
     This Source Code Form is subject to the terms of the Mozilla
     Public License, v. 2.0. If a copy of the MPL was not distributed
     with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 
If it is not possible or desirable to put the notice in a particular file, then
You may include the notice in a location (such as a LICENSE file in a
relevant directory) where a recipient would be likely to look for such a notice.
 
You may add additional accurate notices of copyright ownership.
-->

# DataDefinition and its datamodel

# Introduction
To facilitate the handling of DDL hierarchies there
is a datamodel created (ddl::dd::datamodel::DataDefinition).
This datamodel is validated by a ddl::dd::DataDefinition which keeps the datamodel clean and
well-formed.

Use the datamodel to handle the model by yourself. We recommend to use following DataDefinition
creation API.

# DataDefinition creation API
There are 3 ways to create a valid DataDefinition.

## Create a file and read it
* See ddl::DDFile

## Create a string and read it
* See ddl::DDString

## Create a DataDefinition via DDStructure
* See ddl::DDStructure
* See ddl::DDStructureGenerator

# Binary model classes

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



