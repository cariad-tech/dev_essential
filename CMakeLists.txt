# Copyright @ 2023 VW Group. All rights reserved.
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

cmake_minimum_required(VERSION 3.23)

cmake_policy(SET CMP0048 NEW)

set(LABEL_NAME "dev_essential" CACHE STRING "The name for the project")
set(BUILD_VERSION "1.99.99" CACHE STRING "The version used for the build")
set(GIT_COMMIT "local" CACHE STRING "SHA-1 commit hash.")

string(TOLOWER "${LABEL_NAME}" PRJ_NAME)
string(MAKE_C_IDENTIFIER "${PRJ_NAME}" PRJ_NAME)

project(${PRJ_NAME} VERSION ${BUILD_VERSION})

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

enable_testing()

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(CMAKE_CXX_EXTENSIONS OFF)

# Strict validation and debug symbols in all build types
if(MSVC)
    # Enable inlining in RelWithDebInfo - no good  reason to disable it just because we want debug symbols?!
    STRING (REGEX REPLACE "[/|-]Ob1" "/Ob3" CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")

    STRING (REGEX REPLACE "[/|-]RTC(su|[1su])" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    STRING (REGEX REPLACE "[/|-]RTC(su|[1su])" "" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")

    foreach(c ${CMAKE_CONFIGURATION_TYPES} ${CMAKE_BUILD_TYPE})
        foreach(t EXE SHARED MODULE)
            # Remove /INCREMENTAL:NO from all build types, we don't want PDBs and binaries blow up by factor x2 just so they could be re-built faster!
            STRING (REGEX REPLACE "[/|-]INCREMENTAL(:YES)?" "" CMAKE_${t}_LINKER_FLAGS_${c} "${CMAKE_${t}_LINKER_FLAGS_${c}}")
        endforeach()
    endforeach()

    add_compile_options(
        # Symbols in all build types, since the lib is getting exported
        /Zi
        # Maximum warnings
        /W4
        # Disable compatibility mode in type system, no functional changes in conformant code
        # Not covered by CMAKE_CXX_EXTENSIONS....
        /permissive-
        /Zc:rvalueCast
        /Zc:ternary
        /Zc:inline
        /Zc:throwingNew
        /Zc:strictStrings
        /Zc:lambda
        /Zc:__cplusplus
        /Zc:referenceBinding
        # Enable selective data linkage.
        /Gw
        # Enable selective code linkage.
        /Gy
        # Allow redundant string pooling.
        /GF
        # Enable compilation units with more than 65k symbols.
        /bigobj
        # Disable aggressive stack zeroing.
        /GS-
    )
    add_link_options(
        /DEBUG
        /INCREMENTAL:NO
        /OPT:ICF=4
        /OPT:REF
    )
else()
    add_compile_options(
        -g -ggdb3
        -O3
        -Wall -Wextra -Wpedantic
        -ffunction-sections
        -fdata-sections
    )
    add_link_options(
        -Wl,-Bsymbolic
        -Wl,--no-undefined
        -Wl,--gc-sections
    )
    set(THREADS_PREFER_PTHREAD_FLAG TRUE)
endif()

# this option is necessary if you do not build with gcc 7, but want to check the binary compatibility
set(dev_essential_force_type_access_list_compatibility OFF CACHE BOOL "Switch of the auto identification of ddl::dd::utility::TypeAccessList compatibilities and force compatibility")

include(scripts/cmake/set_position_independent_code.cmake)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/scripts/cmake/modules/")
set(dev_essential_cmake_include_components "." CACHE STRING "List of components to test, separated\
 by whitespace or semicolon (default: '.', meaning all components are tested).")

add_subdirectory(3rdparty)
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

    message(CHECK_START "Checking availability of external packages")
    list(APPEND CMAKE_MESSAGE_INDENT "  ")
    message(CHECK_START "GTest")
    find_package(GTest 1.10.0) # INSTANTIATE_TEST_SUITE_P added in 1.10.0
    if(NOT GTest_FOUND)
        message(CHECK_FAIL "not found. Integrated tests disabled.")
    else()
        message(CHECK_PASS "found ['${GTest_DIR}']")
    endif()
    list(POP_BACK CMAKE_MESSAGE_INDENT)
    if (GTest_FOUND)
        enable_testing()
        include(GoogleTest)
        include(scripts/cmake/stub_generation.cmake)
        include(scripts/cmake/add_test_overwrite.cmake)
        add_subdirectory(test/function)
    endif()
endif()

install(FILES LICENSE README.md RELEASENOTES.md DESTINATION .)
