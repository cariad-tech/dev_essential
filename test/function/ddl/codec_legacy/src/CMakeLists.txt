# Copyright @ 2021 VW Group. All rights reserved.
#
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

# codec_legacy tests
add_executable(ddl_codec_legacy_tests tester_codec_legacy.cpp)
set_target_properties(ddl_codec_legacy_tests PROPERTIES FOLDER test/function/ddl
                                                        TIMEOUT 300)
target_link_libraries(ddl_codec_legacy_tests PRIVATE dev_essential::ddl
                                                     GTest::gtest_main
                                                     $<$<PLATFORM_ID:Linux>:Threads::Threads>)
target_compile_definitions(ddl_codec_legacy_tests PRIVATE DEV_ESSENTIAL_DISABLE_DEPRECATED_WARNINGS)
gtest_discover_tests(ddl_codec_legacy_tests)

# mediacoder_regression_legacy tests
cmake_path(CONVERT "${CMAKE_CURRENT_LIST_DIR}/../files/"
           TO_CMAKE_PATH_LIST test_files_dir
           NORMALIZE)
add_executable(ddl_mediacoder_regression_legacy_tests tester_mediacoder_regression_legacy.cpp)
target_compile_definitions(ddl_mediacoder_regression_legacy_tests
                           PRIVATE TEST_FILES_DIR="${test_files_dir}"
                                   DEV_ESSENTIAL_DISABLE_DEPRECATED_WARNINGS)
set_target_properties(ddl_mediacoder_regression_legacy_tests PROPERTIES FOLDER test/function/ddl
                                                                        TIMEOUT 300)
target_link_libraries(ddl_mediacoder_regression_legacy_tests
                      PRIVATE ddl_test_compat
                              dev_essential::ddl
                              GTest::gtest_main
                              $<$<PLATFORM_ID:Linux>:Threads::Threads>)
gtest_discover_tests(ddl_mediacoder_regression_legacy_tests)