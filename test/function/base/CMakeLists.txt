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

find_package(dev_essential COMPONENTS base REQUIRED)

#delegate test
add_executable(delegate_tests delegate_test.cpp)
target_link_libraries(delegate_tests PRIVATE GTest::gtest_main dev_essential::base)
add_test(delegate_tests delegate_tests)
set_target_properties(delegate_tests PROPERTIES FOLDER test/function/a_util/base)

#maybe_unused test
add_executable(maybe_unused_tests maybe_unused_test.cpp)
target_link_libraries(maybe_unused_tests PRIVATE GTest::gtest_main dev_essential::base)
add_test(maybe_unused_tests maybe_unused_tests)
set_target_properties(maybe_unused_tests PROPERTIES FOLDER test/function/a_util/base)

target_compile_options(maybe_unused_tests PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/WX>)
target_compile_options(maybe_unused_tests PRIVATE $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Werror>)

# test whether all public header files are self contained
include("${CMAKE_CURRENT_LIST_DIR}/test_public_headers_are_self_contained.cmake")