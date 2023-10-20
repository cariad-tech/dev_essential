<!---
Copyright @ 2021 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
-->

# DDL Library

# Description

The Data Defintion Language (DDL) Package provides utilities to handle data descriptions and data defined by these descriptions.

    #include <ddl/ddl.h>

* [DDL specification](ddl/ddl_specification.md)




_____________________

# DDL Library Parts

This library contains following internal parts:

## DD - DataDefinition Creation

A Memory Model for the DDL Language.

* See [DD Documentation](ddl/dd.md)

        ddl/dd/ddstructure.h
        ddl/dd/ddfile.h
        ddl/dd/ddstring.h



## Codec API

Access, decode and encode ddl described memory data.

* See [Codec Documentation](ddl/codec.md)

        ddl/codec/pkg_codec.h


## Mapping Functionality

Map data from one DDL structure to another

* See [Mapping specification](ddl/mapping_specification.md)

        ddl/mapping/pkg_mapping.h

________________________

# Dependencies

## a_util library

The libraries above depend on the [a_util](a_util.md) libraries.
