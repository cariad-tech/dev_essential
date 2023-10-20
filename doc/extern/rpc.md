<!---
Copyright @ 2021 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
-->

# RPC Package

The RPC Package defines a generic interprocess communication (remote procedure call) programming model.
The main header to include is:

````cpp
#include <rpc/rpc.h>
````

## Introduction

One of the basic prerequisites for creating a set of independent tools is
the possibility to let the tools communicate with each other. A common way
of handling communication between different processes may be provided by
an extensible RPC mechanism.

## Basics

Starting from ADTF 3.1 the RPC package uses libjsoncpp and libjson-rpc-cpp
to perform the heavy lifting of handling remote calls. These libraries are
integrated into this library, so there is no need for them to be installed
on your system seperately. For additional documentation on stubs and json
take a look at https://github.com/cinemast/libjson-rpc-cpp.

## RPC Example

First start with defining you interface via a *.json* file, in this case a simple calculator

````js
 [
    {
        "name": "Add",
        "params": {
            "nValue1": 1,
            "nValue2": 1
        },
        "returns": 1
    },
    {
        "name": "Subtract",
        "params": {
            "nValue1": 1,
            "nValue2": 1
        },
        "returns": 1
    },
    {
        "name": "Multiply",
        "params": {
            "nValue1": 1,
            "nValue2": 1
        },
        "returns": 1
    },
    {
        "name": "Divide",
        "params": {
            "nValue1": 1,
            "nValue2": 1
        },
        "returns": 1
    }
]
````

To generate the neccessary stub headers you can use the supplied CMake macros

````
    jsonrpc_generate_client_stub
````

and

````
    jsonrpc_generate_server_stub
````

In our case somethin like the following will do the job:

````
jsonrpc_generate_client_stub(${CMAKE_CURRENT_SOURCE_DIR}/calculator.json rpc_stubs::cCalculatorClientStub ${CMAKE_CURRENT_BINARY_DIR}/calculatorclientstub.h)
jsonrpc_generate_server_stub(${CMAKE_CURRENT_SOURCE_DIR}/calculator.json rpc_stubs::cCalculatorServerStub ${CMAKE_CURRENT_BINARY_DIR}/calculatorserverstub.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
````

You can then create a simple client with the help of the rpc::jsonrpc_remote_object template:

````cpp
#include <rpc/rpc.h>
#include <calculatorclientstub.h>
...
rpc::jsonrpc_remote_object<rpc_stubs::cCalculatorClientStub> oRemoteCalculator("http://localhost:1234/calculator");
tInt nThree = oRemoteCalculator.Add(1, 2);
````

To implement the remote objects functionality you need to subclass rpc::jsonrpc_object_server and implement the pure virtual methods of the generated stub.

````cpp
#include <rpc/rpc.h>
#include <calculatorserverstub.h>

class cCalculator: public rpc::jsonrpc_object_server<rpc_stubs::cCalculatorServerStub>
{
    public:
        int Add(int nValue1, int nValue2) override
        {
            return nValue1 + nValue2;
        }

        int Subtract(int nValue1, int nValue2) override
        {
            return nValue1 - nValue2;
        }

        int Multiply(int nValue1, int nValue2) override
        {
            return nValue1 * nValue2;
        }

        int Divide(int nValue1, int nValue2) override
        {
            return nValue1 / nValue2;
        }
};
````

In order for the remote object to be accessible we need to create an RPC server

````cpp
void Server()
{
    rpc::cJSONRPCServer oServer;
    cCalculator oCalculator;

    oServer.RegisterRPCObject("calculator", &oCalculator);
    oServer.StartListening("http://0.0.0.0:1234");

    // wait for quit

    oServer.StopListening();
    oServer.UnregisterRPCObject("calculator");
}
````

## Dependency

The RPC Package depends on the [a_util](a_util.md) library.

## License

The package RPC is delivered under the
[MPL - Mozilla Public License - Version 2.0](doc/extern/license/MPL2.0.txt)

The package RPC is delivered with the following 3rdparty dependencies:
- [cpp-httplib](https://github.com/yhirose/cpp-httplib)
- [jsoncpp](https://github.com/open-source-parsers/jsoncpp)
- [libjson-cpp-rpc](https://github.com/cinemast/libjson-rpc-cpp/)

Their license disclaimers can be found [here](doc/extern/license/licenses.txt).
