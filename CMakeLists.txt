# Copyright @ 2021 VW Group. All rights reserved.
#
#     This Source Code Form is subject to the terms of the Mozilla
#     Public License, v. 2.0. If a copy of the MPL was not distributed
#     with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
#
# If it is not possible or desirable to put the notice in a particular file, then
# You may include the notice in a location (such as a LICENSE file in a
# relevant directory) where a recipient would be likely to look for such a notice.
#
# You may add additional accurate notices of copyright ownership.

################################ Options and general project setup ################################
cmake_minimum_required(VERSION 3.17.0) # cxx_std_11

cmake_policy(SET CMP0048 NEW)

# Start project() now, since any include checks will use the correct compiler flags
file(STRINGS version VERSION LIMIT_COUNT 1)
project(dev_essential VERSION ${VERSION})

# Disable extensions here and require the chosen CMAKE_CXX_STANDARD
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# minimum is currently C++14, but default is C++17
if (NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 17)
endif()

set(dev_essential_cmake_include_components "." CACHE STRING "List of components to build, separated\
 by whitespace or semicolon (default: '.', meaning all components are built).")

include(scripts/cmake/set_position_independent_code.cmake)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
##check for default install prefix and cmake build type
include(scripts/cmake/check_cmake_install_prefix.cmake)
include(scripts/cmake/check_cmake_build_type.cmake)

# Enable multicore compilation on Windows
include(scripts/cmake/enable_multicore_compilation.cmake)

# Use integrated debug symbols on Windows (avoiding PDBs)
include(scripts/cmake/use_integrated_debug_symbols.cmake)

# set default symbol visibility for compilers supporting it
include(scripts/cmake/set_symbol_visibilty.cmake)

if("${dev_essential_cmake_include_components}" STREQUAL ".")
    add_subdirectory(extern/3rdparty/clara)
endif()
add_subdirectory(doc)
add_subdirectory(src)

# Enable testing if necessary
option(dev_essential_cmake_enable_integrated_tests
       "Enable tests as integrated build - requires googletest (default: OFF)"
       OFF)
if(dev_essential_cmake_enable_integrated_tests)
    # Generator expressions take 0 as FALSE/OFF and 1 as TRUE/ON
    set(dev_essential_cmake_enable_integrated_tests 1 CACHE BOOL "" FORCE)

    #ignore find_package calls inside the source tree from dev_essential components
    macro(find_package)
        if(NOT "${ARGV0}" MATCHES "^[dD][eE][vV][_][eE][sS][sS][eE][nN][tT][iI][aA][lL]")
            _find_package(${ARGV})
        endif()
    endmacro(find_package)

    message(CHECK_START "Checking third-party packages")
    list(APPEND CMAKE_MESSAGE_INDENT "  ")
    message(CHECK_START "GTest")
    find_package(GTest)
    if(NOT GTest_FOUND)
        message(CHECK_FAIL "not found. Integrated tests disabled.")
    else()
        message(CHECK_PASS "found ['${GTest_DIR}']")
    endif()
    list(POP_BACK CMAKE_MESSAGE_INDENT)
    if (GTest_FOUND)
        enable_testing()
        include(scripts/cmake/stub_generation.cmake)
        add_subdirectory(test/function)
    endif()
    add_subdirectory(test/sca)
endif(dev_essential_cmake_enable_integrated_tests)

# License Information must be delivered anyway!
install(FILES README.md DESTINATION ${CMAKE_INSTALL_PREFIX})
