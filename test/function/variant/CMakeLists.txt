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

find_package(dev_essential COMPONENTS variant REQUIRED)

add_executable(variant_tests variant_test.cpp)

# Link test executable against gtest & gtest_main (also gets their include paths)
target_link_libraries(variant_tests PRIVATE GTest::gtest_main dev_essential::variant)
target_compile_options(variant_tests PRIVATE
                       $<$<AND:$<CXX_COMPILER_ID:GNU>,$<VERSION_LESS:$<CXX_COMPILER_VERSION>,5.4.0>>:-Wno-attributes>)

add_test(variant_tests variant_tests)

set_target_properties(variant_tests PROPERTIES FOLDER test/function/a_util/variant)
