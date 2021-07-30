/**
 * @file
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

/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    stubgeneratorfactory.cpp
 * @date    11/19/2014
 * @author  Peter Spiess-Knafl <peter.knafl@gmail.com>
 * @license See attached LICENSE.txt
 ************************************************************************/

#include "stubgeneratorfactory.h"

#include "client/cppclientstubgenerator.h"
#include "client/jsclientstubgenerator.h"
#include "commandline.h"
#include "helper/cpphelper.h"
#include "server/cppserverstubgenerator.h"

#include <iostream>
#include <jsonrpccpp/common/specificationparser.h>
#include <jsonrpccpp/version.h>

using namespace jsonrpc;
using namespace std;

bool StubGeneratorFactory::createStubGenerators(int argc,
                                                char** argv,
                                                vector<Procedure>& procedures,
                                                vector<StubGenerator*>& stubgenerators,
                                                FILE* my_stdout,
                                                FILE* my_stderr)
{
    rpc::cCommandLine oCmd(argc, const_cast<const char**>(argv));

    if (oCmd.GetFlag("help") || oCmd.GetFlag("h")) {
        fprintf(my_stdout, "Usage: %s ", argv[0]);
        ///@todo
        return true;
    }

    if (oCmd.GetFlag("version")) {
        fprintf(my_stdout,
                "jsonrpcstub version %d.%d.%d\n",
                JSONRPC_CPP_MAJOR_VERSION,
                JSONRPC_CPP_MINOR_VERSION,
                JSONRPC_CPP_PATCH_VERSION);
        return true;
    }

    if (oCmd.GetValues().size() < 2) {
        fprintf(my_stderr, "Invalid arguments: specfile must be provided.\n");
        return false;
    }

    bool verbose = oCmd.GetFlag("verbose") || oCmd.GetFlag("v");

    try {
        string interface_definition;
        SpecificationParser::GetFileContent(oCmd.GetValue(1).c_str(), interface_definition);
        procedures = SpecificationParser::GetProceduresFromString(interface_definition);
        if (verbose) {
            fprintf(my_stdout,
                    "Found %zu procedures in %s\n",
                    procedures.size(),
                    oCmd.GetValue(1).c_str());
            for (unsigned int i = 0; i < procedures.size(); ++i) {
                if (procedures.at(i).GetProcedureType() == RPC_METHOD) {
                    fprintf(my_stdout, "\t[Method]         ");
                }
                else {
                    fprintf(my_stdout, "\t[Notification]   ");
                }
                fprintf(my_stdout, "%s\n", procedures.at(i).GetProcedureName().c_str());
            }
            fprintf(my_stdout, "\n");
        }

        if (!oCmd.GetProperty("cpp-server").empty()) {
            string filename =
                oCmd.GetProperty(
                        "cpp-server-file",
                        CPPHelper::class2Filename(oCmd.GetProperty("cpp-server").c_str()).c_str())
                    .c_str();
            if (verbose)
                fprintf(my_stdout, "Generating C++ Serverstub to: %s\n", filename.c_str());
            stubgenerators.push_back(
                new CPPServerStubGenerator(oCmd.GetProperty("cpp-server").c_str(),
                                           procedures,
                                           filename,
                                           interface_definition));
        }

        if (!oCmd.GetProperty("cpp-client").empty()) {
            string filename =
                oCmd.GetProperty(
                        "cpp-client-file",
                        CPPHelper::class2Filename(oCmd.GetProperty("cpp-client").c_str()).c_str())
                    .c_str();
            if (verbose)
                fprintf(my_stdout, "Generating C++ Clientstub to: %s\n", filename.c_str());
            stubgenerators.push_back(new CPPClientStubGenerator(
                oCmd.GetProperty("cpp-client").c_str(), procedures, filename));
        }

        if (!oCmd.GetProperty("js-client").empty()) {
            string filename =
                oCmd.GetProperty(
                        "js-client-file",
                        CPPHelper::class2Filename(oCmd.GetProperty("js-client").c_str()).c_str())
                    .c_str();
            if (verbose)
                fprintf(my_stdout, "Generating JavaScript Clientstub to: %s\n", filename.c_str());
            stubgenerators.push_back(new JSClientStubGenerator(
                oCmd.GetProperty("js-client").c_str(), procedures, filename));
        }
    }
    catch (const JsonRpcException& ex) {
        fprintf(my_stderr, "%s\n", ex.what());
        return false;
    }
    return true;
}

void StubGeneratorFactory::deleteStubGenerators(std::vector<StubGenerator*>& stubgenerators)
{
    for (unsigned int i = 0; i < stubgenerators.size(); ++i) {
        delete stubgenerators[i];
    }
}
