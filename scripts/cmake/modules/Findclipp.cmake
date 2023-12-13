# Copyright @ 2022 VW Group. All rights reserved.
#
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

include(FindPackageHandleStandardArgs)

# first specifically look for the CMake version of clipp
# (available if built from master, but not in latest tag v1.2.3)
find_package(clipp QUIET NO_MODULE)

# if we found the clipp cmake package then we are done, and can print what we found and return
if(clipp_FOUND)
    find_package_handle_standard_args(clipp CONFIG_MODE)
    return()
endif()

find_file (clipp_HEADER clipp.h)

if(clipp_HEADER)
    file(STRINGS "${clipp_HEADER}" clipp_header_content)
    # check each line for version
    foreach(line ${clipp_header_content})
        string(REGEX MATCH "^ \*.+ version ([0-9]+\.[0-9]+\.[0-9]+)$" _ ${line})
        if(CMAKE_MATCH_1)
            set(clipp_VERSION ${CMAKE_MATCH_1})
            get_filename_component(clipp_dir ${clipp_HEADER} DIRECTORY)
            add_library(clipp::clipp INTERFACE IMPORTED)
            set_target_properties(clipp::clipp
                                  PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${clipp_dir}"
                                             INTERFACE_COMPILE_FEATURES "cxx_std_11")
            break()
        endif()
    endforeach()

    # handle COMPONENTS part of find_package(clipp COMPONENTS)
    if (NOT clipp_FIND_COMPONENTS)
        # At least clipp component needs to be handled
        set(clipp_FIND_COMPONENTS clipp)
    endif()

    set(clipp_clipp_FOUND TRUE)
    mark_as_advanced(clipp_HEADER) # do not show in any GUI
endif()

find_package_handle_standard_args(clipp REQUIRED_VARS clipp_VERSION HANDLE_COMPONENTS)
