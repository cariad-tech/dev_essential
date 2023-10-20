/**
 * @file
 * Command line processing wrapper implementation for clipp
 *
 * Copyright @ 2022 VW Group. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "commandline.h"

#include "../../strings/std_to_detail.h" // std::strtoull, std::strtoll for QNX
#if (!HAS_STD_STRTOLL)
namespace std {
using ::a_util::strings::detail::strtoll;
}
#endif // (!HAS_STD_STRTOLL)

#if (!HAS_STD_STRTOULL)
namespace std {
using ::a_util::strings::detail::strtoull;
}
#endif // (!HAS_STD_STRTOULL)

#include <clipp.h>
#include <iostream>

Clipp::Clipp() : _cli{std::make_unique<clipp::group>()}, _settings{Mode::parse_error}
{
    using clipp::option;
    using clipp::required;
    using clipp::value;

    auto class_name_value = (value("ClassName", _settings.class_name));
    auto outfile_value = (value("outfile", _settings.outfile));
    auto outfile_doc = "Generate stub to 'outfile' instead of the default one";

    // helper lambda for --cpp-server, --cpp-clilent and --js-client options
    auto makeGroup = [&](const std::string& server_or_client_option,
                         const std::string& server_or_client_description,
                         Mode mode) -> clipp::group {
        // NOTE: The '=' used in options and required must stay for backwards compatibility !
        auto server_or_client = (required(server_or_client_option + '=').set(_settings.mode, mode));
        auto server_or_client_file_option = (option(server_or_client_option + "-file="));
        auto mode_header_doc = "Generate " + server_or_client_description + " stub:";
        auto detailed_doc =
            "Generate " + server_or_client_description + " stub class to file 'classname.h'";
        return mode_header_doc % ((server_or_client & class_name_value) % detailed_doc,
                                  (server_or_client_file_option & outfile_value) % outfile_doc);
    };

    auto cpp_server_mode = makeGroup("--cpp-server", "C++ server", Mode::cpp_server);
    auto cpp_client_mode = makeGroup("--cpp-client", "C++ client", Mode::cpp_client);
    auto js_client_mode = makeGroup("--js-client", "JavaScript client", Mode::js_client);

    auto show_help = (option("-h", "--help").set(_settings.show_help)) % "Print this help and exit";
    auto show_version =
        (option("--version").set(_settings.show_version)) % "Print version and exit";
    auto specfile = (value("specfile", _settings.specfile) % "Path to the json specification file");
    auto verbose = (option("-v", "--verbose").set(_settings.verbose)) % "Print detailed output";

    *_cli = ((specfile, cpp_server_mode | cpp_client_mode | js_client_mode, verbose) | show_help |
             show_version);
}

// neccesary to prevent incomplete type error from unique_ptr
Clipp::~Clipp() = default;

void Clipp::printManPage() const
{
    using clipp::doc_formatting;
    std::cout << clipp::make_man_page(*_cli, "stubgenerator", doc_formatting{}.param_separator(""));
}

const Settings& Clipp::parse(int argc, char** argv) const
{
    clipp::parse(argc, argv, *_cli);
    return _settings;
}
