# Copyright @ 2021 VW Group. All rights reserved.
#
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

if(QNXNTO)
    # The jsoncpp library uses __QNXNTO__ rather than __QNX__, but this is set by the compiler itself.
    # However, the it uses extensions of the standard like generic templates, so we cannot use -std=c++11:
    set(CMAKE_CXX_EXTENSIONS ON)
endif()

set(jsoncpp_DIR ${CMAKE_CURRENT_LIST_DIR}/jsoncpp-1.8.4)
set(jsoncpp_SRC
    ${jsoncpp_DIR}/src/lib_json/json_tool.h
    ${jsoncpp_DIR}/src/lib_json/json_reader.cpp
    ${jsoncpp_DIR}/src/lib_json/json_valueiterator.inl
    ${jsoncpp_DIR}/src/lib_json/json_value.cpp
    ${jsoncpp_DIR}/src/lib_json/json_writer.cpp
)

add_library(jsoncpp OBJECT ${jsoncpp_SRC})
target_include_directories(jsoncpp PUBLIC $<BUILD_INTERFACE:${jsoncpp_DIR}/include>)
target_compile_definitions(jsoncpp PRIVATE $<$<PLATFORM_ID:Windows>:_CRT_SECURE_NO_WARNINGS>)
set_target_properties(jsoncpp PROPERTIES FOLDER 3rdparty/object_libraries)

install(DIRECTORY ${jsoncpp_DIR}/include/json DESTINATION include)
