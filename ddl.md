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

# DDL Library

# Description

The Data Defintion Language (DDL) Package provides utilities to handle data descriptions and data defined by these descriptions.

    #include <ddl/ddl.h>

* [DDL specification](doc/extern/ddl/ddl_specification.md)




_____________________

# DDL Library Parts

This library contains following internal parts:

## DD - DataDefinition Creation

A Memory Model for the DDL Language.

* See [DD Documentation](doc/extern/ddl/dd.md)

        ddl/dd/ddstructure.h
        ddl/dd/ddfile.h
        ddl/dd/ddstring.h



## Codec API

Access, decode and encode ddl described memory data.

* See [Codec Documentation](doc/extern/ddl/codec.md)

        ddl/codec/pkg_codec.h


## DDL-2-header and header-2-DDL generation

Generating a c-header file from DDL and vice versa.

* See [DDL Generators](doc/extern/ddl/ddl_generators.md)

        ddl/ddlgenerators/generatorlibrary
        ddl/ddlgenerators/ddl2header
        ddl/ddlgenerators/header2ddl

## Mapping Functionality

Map data from one DDL structure to another

* See [Mapping specification](doc/extern/ddl/mapping_specification.md)

        ddl/mapping/pkg_mapping.h

________________________

# Dependencies

## a_util library

The libraries above depend on the [a_util](a_util.md) libraries.
