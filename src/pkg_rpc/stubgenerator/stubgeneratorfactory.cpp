/**
 * @file
 *
 * Copyright @ 2021 VW Group. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
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

#include "commandline.h"

#include <client/cppclientstubgenerator.h>
#include <client/jsclientstubgenerator.h>
#include <helper/cpphelper.h>
#include <jsonrpccpp/common/specificationparser.h>
#include <jsonrpccpp/version.h>
#include <server/cppserverstubgenerator.h>

using namespace jsonrpc;

bool StubGeneratorFactory::createStubGenerators(int argc,
                                                char** argv,
                                                std::vector<Procedure>& procedures,
                                                std::vector<StubGenerator*>& stubgenerators,
                                                FILE* my_stdout,
                                                FILE* my_stderr)
{
    // IILF to make the settings const and have the cli removed from memory after parsing is done
    const auto settings = [argc, argv]() -> Settings {
        Clipp cli;
        auto settings = cli.parse(argc, argv);
        if (settings.mode == Mode::parse_error) {
            // TODO: Show help or manpage here
            // Currently not implemented to keep the same output as in previous implementation
            // cli.printManPage();
        }
        if (settings.outfile.empty()) {
            settings.outfile = CPPHelper::class2Filename(settings.class_name);
        }
        return settings;
    }();

    if (settings.show_help) {
        fprintf(my_stdout, "Usage: %s ", argv[0]);
        ///@todo
        return true;
    }

    if (settings.show_version) {
        fprintf(my_stdout,
                "jsonrpcstub version %d.%d.%d\n",
                JSONRPC_CPP_MAJOR_VERSION,
                JSONRPC_CPP_MINOR_VERSION,
                JSONRPC_CPP_PATCH_VERSION);
        return true;
    }

    if (settings.specfile.empty()) {
        fprintf(my_stderr, "Invalid arguments: specfile must be provided.\n");
        return false;
    }

    try {
        std::string interface_definition;
        SpecificationParser::GetFileContent(settings.specfile, interface_definition);
        procedures = SpecificationParser::GetProceduresFromString(interface_definition);
        if (settings.verbose) {
            fprintf(my_stdout,
                    "Found %zu procedures in %s\n",
                    procedures.size(),
                    settings.specfile.c_str());
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

        if (settings.mode == Mode::cpp_server) {
            if (settings.verbose)
                fprintf(my_stdout, "Generating C++ Serverstub to: %s\n", settings.outfile.c_str());
            stubgenerators.push_back(new CPPServerStubGenerator(
                settings.class_name, procedures, settings.outfile, interface_definition));
        }

        if (settings.mode == Mode::cpp_client) {
            if (settings.verbose)
                fprintf(my_stdout, "Generating C++ Clientstub to: %s\n", settings.outfile.c_str());
            stubgenerators.push_back(
                new CPPClientStubGenerator(settings.class_name, procedures, settings.outfile));
        }

        if (settings.mode == Mode::js_client) {
            if (settings.verbose)
                fprintf(my_stdout,
                        "Generating JavaScript Clientstub to: %s\n",
                        settings.outfile.c_str());
            stubgenerators.push_back(
                new JSClientStubGenerator(settings.class_name, procedures, settings.outfile));
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
