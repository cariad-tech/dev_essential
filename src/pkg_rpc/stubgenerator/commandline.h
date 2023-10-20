/**
 * @file
 * Command line processing wrapper for clipp
 *
 * Copyright @ 2022 VW Group. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef _COMMAND_LINE_CLASS_HEADER_
#define _COMMAND_LINE_CLASS_HEADER_

#include <memory>
#include <string>

namespace clipp {
class group; // forward declaration for unique_ptr
} // namespace clipp

enum class Mode : std::int32_t {
    help = 0,
    version,
    cpp_server,
    cpp_client,
    js_client,
    parse_error
};

struct Settings {
    Mode mode = {};
    bool verbose = {};
    bool show_help = {};
    bool show_version = {};
    std::string outfile = {};
    std::string specfile = {};
    std::string class_name = {};
};

// Command line interface wrapper for convenient access to the given cli parameters
class Clipp {
public:
    Clipp();
    ~Clipp() /*= default*/; // for unique_ptr
    Clipp(Clipp&&) = default;
    Clipp& operator=(Clipp&&) = default;
    Clipp(const Clipp&) = delete;
    Clipp& operator=(const Clipp&) = delete;

    void printManPage() const;
    const Settings& parse(int argc, char** argv) const;

private: // data and types
    std::unique_ptr<clipp::group> _cli;
    Settings _settings;
};

#endif // _COMMAND_LINE_CLASS_HEADER_
