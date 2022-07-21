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

set(jsonrpccpp_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libjson-rpc-cpp-1.1.0)

#Configure json include subdir
set(JSONCPP_INCLUDE_PREFIX json)
configure_file("${jsonrpccpp_DIR}/src/jsonrpccpp/common/jsonparser.h.in"
               "configured/jsonrpccpp/common/jsonparser.h")

#Configure jsonrpc protocol version which we can find inside the root CMakeLists.txt of json-rpc-cpp
file(STRINGS ${jsonrpccpp_DIR}/CMakeLists.txt _jsonrpccpp_cmakelists_file_content)
foreach(_current_line ${_jsonrpccpp_cmakelists_file_content})
    if(${_current_line} MATCHES "^set[(][A-Z]+_VERSION[ ]+[0-9]+") #matches e.g. set(MAJOR_VERSION 1)
        string(REGEX REPLACE "^set[(]([A-Z]+_VERSION).+$"
                             "\\1" _current_version_key ${_current_line})
        string(REGEX REPLACE "^set[(][A-Z]+_VERSION[ ]+([0-p]+)[)]$"
                             "\\1" _current_version_val ${_current_line})
        set(${_current_version_key} ${_current_version_val})
    endif()
endforeach(_current_line ${_jsonrpccpp_cmakelists_file_content})

if(NOT DEFINED MAJOR_VERSION OR NOT DEFINED MINOR_VERSION OR NOT DEFINED PATCH_VERSION OR NOT DEFINED SO_VERSION)
    message(FATAL_ERROR "Could not determine correct jsonrpc protocol version.")
endif(NOT DEFINED MAJOR_VERSION OR NOT DEFINED MINOR_VERSION OR NOT DEFINED PATCH_VERSION OR NOT DEFINED SO_VERSION)

configure_file("${jsonrpccpp_DIR}/src/jsonrpccpp/version.h.in"
               "configured/jsonrpccpp/version.h")

file(GLOB jsonrpc_source_common ${jsonrpccpp_DIR}/src/jsonrpccpp/common/*.c*)
file(GLOB jsonrpc_install_common ${jsonrpccpp_DIR}/src/jsonrpccpp/common/errors.h
                                 ${jsonrpccpp_DIR}/src/jsonrpccpp/common/exception.h
                                 ${jsonrpccpp_DIR}/src/jsonrpccpp/common/jsonparser.h
                                 ${jsonrpccpp_DIR}/src/jsonrpccpp/common/procedure.h
                                 ${jsonrpccpp_DIR}/src/jsonrpccpp/common/specification.h
                                 ${jsonrpccpp_DIR}/src/jsonrpccpp/common/specificationparser.h
                                 ${jsonrpccpp_DIR}/src/jsonrpccpp/common/specificationwriter.h)
file(GLOB jsonrpc_source_server ${jsonrpccpp_DIR}/src/jsonrpccpp/server/*.c*)
file(GLOB jsonrpc_install_server ${jsonrpccpp_DIR}/src/jsonrpccpp/server/*.h)
file(GLOB jsonrpc_source_client ${jsonrpccpp_DIR}/src/jsonrpccpp/client/*.c*)
file(GLOB jsonrpc_install_client ${jsonrpccpp_DIR}/src/jsonrpccpp/client/*.h)
file(GLOB jsonrpc_install ${jsonrpccpp_DIR}/src/jsonrpccpp/client.h
                          ${jsonrpccpp_DIR}/src/jsonrpccpp/server.h
                          ${jsonrpccpp_DIR}/src/jsonrpccpp/version.h)

set(jsonrpccpp_SRC ${jsonrpc_source_common} ${jsonrpc_source_server} ${jsonrpc_source_client})

get_target_property(JSONCPP_INCLUDE_DIRECTORIES jsoncpp INCLUDE_DIRECTORIES)
add_library(libjson-rpc-cpp OBJECT ${jsonrpccpp_SRC})
# dynamic_cast in jsonrpccpp/server/abstractserver.h
target_compile_options(libjson-rpc-cpp PRIVATE $<$<AND:$<NOT:$<CXX_COMPILER_ID:MSVC>>,$<COMPILE_LANGUAGE:CXX>>:-frtti -Wno-deprecated-declarations>
                                               $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<COMPILE_LANGUAGE:CXX>>:/GR>)
target_include_directories(libjson-rpc-cpp SYSTEM
                           PRIVATE $<BUILD_INTERFACE:${jsonrpccpp_DIR}/src>
                                   $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/configured/>
                                   $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/configured/jsonrpccpp/common>
                                   ${JSONCPP_INCLUDE_DIRECTORIES})
if(MSVC)
    target_include_directories(libjson-rpc-cpp PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/compat/msvc>)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    if(CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "GNU")
        # See: https://github.com/msys2/MINGW-packages/issues/5786
        target_compile_options(libjson-rpc-cpp PRIVATE -femulated-tls)
    endif()
endif()
target_compile_features(libjson-rpc-cpp PRIVATE cxx_std_11) # C++11 for self
set_target_properties(libjson-rpc-cpp PROPERTIES FOLDER 3rdparty/object_libraries)

install(FILES ${jsonrpc_install} DESTINATION include/jsonrpccpp)
install(FILES ${jsonrpc_install_common} DESTINATION include/jsonrpccpp/common)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/configured/jsonrpccpp/common/jsonparser.h
        DESTINATION include/jsonrpccpp/common)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/configured/jsonrpccpp/version.h
        DESTINATION include/jsonrpccpp/common)
install(FILES ${jsonrpc_install_server} DESTINATION include/jsonrpccpp/server)
install(FILES ${jsonrpc_install_client} DESTINATION include/jsonrpccpp/client)


#stubgenerator object lib for linking by patched json_stubgenerator
add_library(libjson-rpc-cpp-stubgenerator
            OBJECT ${jsonrpccpp_DIR}/src/stubgenerator/codegenerator.cpp
                   ${jsonrpccpp_DIR}/src/stubgenerator/main.cpp
                   ${jsonrpccpp_DIR}/src/stubgenerator/stubgenerator.cpp
                   ${jsonrpccpp_DIR}/src/stubgenerator/client/cppclientstubgenerator.cpp
                   ${jsonrpccpp_DIR}/src/stubgenerator/client/jsclientstubgenerator.cpp
                   ${jsonrpccpp_DIR}/src/stubgenerator/server/cppserverstubgenerator.cpp
                   ${jsonrpccpp_DIR}/src/stubgenerator/helper/cpphelper.cpp)
target_include_directories(libjson-rpc-cpp-stubgenerator
                           PRIVATE ${jsonrpccpp_DIR}/src/stubgenerator
                                   ${jsonrpccpp_DIR}/src
                                   ${CMAKE_CURRENT_BINARY_DIR}/configured
                                   ${CMAKE_CURRENT_BINARY_DIR}/configured/jsonrpccpp/common
                                   ${JSONCPP_INCLUDE_DIRECTORIES}
                                   ${CMAKE_CURRENT_SOURCE_DIR}/compat/common)
target_compile_features(libjson-rpc-cpp-stubgenerator PRIVATE cxx_std_11) # C++11 for self
set_target_properties(libjson-rpc-cpp-stubgenerator PROPERTIES FOLDER 3rdparty/object_libraries)
