# Copyright @ 2022 VW Group. All rights reserved.
#
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

# Disable cmake emitting an error if add_test() instead of gtest_discover_tests() is used
set(_dev_essential_cmake_enable_add_test_command TRUE)

# Add test targets and register the test targets for ctest
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/../clipp-1.2.3/test source_files)
foreach(src IN LISTS source_files)
    cmake_path(GET src STEM stem)
    add_executable(clipp-${stem} ${src})
    add_test(clipp-${stem} clipp-${stem})
    set_target_properties(clipp-${stem} PROPERTIES FOLDER test/private/3rdparty/clipp)
endforeach()
