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

##this test is all about finding clara using find_package on customer side
if(dev_essential_cmake_enable_integrated_tests)
    include_directories(${CMAKE_CURRENT_LIST_DIR}/../Clara-1.1.2)
else()
    find_package(dev_essential REQUIRED) #necessary to actually being able to find package Clara
    find_package(Clara REQUIRED)
endif()

##only include test if clara can be compiled at all - currently C++11 is necessary
cmake_policy(SET CMP0067 NEW) #honor cxx standard in try_compile for current toolchain
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/try_compile_clara/clara.cpp
     "#include \"${CMAKE_CURRENT_LIST_DIR}/../Clara-1.1.2/clara.hpp\"\n"
     "int main(){return 0;}")

try_compile(_clara_compiles
            ${CMAKE_CURRENT_BINARY_DIR}/try_compile_clara
            SOURCES ${CMAKE_CURRENT_BINARY_DIR}/try_compile_clara/clara.cpp
            CXX_STANDARD_REQUIRED ON
            CXX_STANDARD 11
            CXX_EXTENSIONS OFF
            )

set(component_clara_AVAILABLE ${_clara_compiles} CACHE INTERNAL
    "Indicate whether component clara is available")

##the actual test
if(_clara_compiles)
    add_executable(clara_include_test clara_include_test.cpp)
    target_link_libraries(clara_include_test PRIVATE GTest::gtest_main Clara)
    add_test(clara_include_test clara_include_test)
    set_target_properties(clara_include_test PROPERTIES FOLDER 3rdparty/test)
else()
    message(STATUS "Clara not supported on this platform. Test disabled.")
endif()