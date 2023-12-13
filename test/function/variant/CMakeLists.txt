# Copyright @ 2021 VW Group. All rights reserved.
#
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

find_package(dev_essential COMPONENTS variant REQUIRED)

add_executable(variant_tests variant_test.cpp)
target_link_libraries(variant_tests PRIVATE GTest::gtest_main dev_essential::variant)
target_compile_options(variant_tests PRIVATE
                       $<$<AND:$<CXX_COMPILER_ID:GNU>,$<VERSION_LESS:$<CXX_COMPILER_VERSION>,5.4.0>>:-Wno-attributes>)
set_target_properties(variant_tests PROPERTIES FOLDER test/function/a_util/variant)
gtest_discover_tests(variant_tests)