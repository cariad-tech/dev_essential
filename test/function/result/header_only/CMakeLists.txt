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

find_package(dev_essential COMPONENTS result REQUIRED)

add_executable(result_type_test result_type_test.cpp)
target_link_libraries(result_type_test PRIVATE GTest::gtest_main dev_essential::result)
add_test(result_type_test result_type_test)
set_target_properties(result_type_test PROPERTIES FOLDER test/function/a_util/result)

add_executable(ref_counted_object_test ref_counted_object_test.cpp)
target_link_libraries(ref_counted_object_test PRIVATE GTest::gtest_main dev_essential::base)
add_test(ref_counted_object_test ref_counted_object_test)
set_target_properties(ref_counted_object_test PROPERTIES FOLDER test/function/a_util/result)

# a_util 5.0.0 library to test backwards binary compatibility
add_library(a_util_result_legacy_shared
            SHARED lib/a_util_5_0_0/a_util/concurrency/impl/atomic_fallback.cpp
                   lib/a_util_5_0_0/a_util/result/impl/result_type.cpp
                   lib/a_util_5_0_0/a_util/result/impl/error_description.cpp
                   # shared lib part
                   lib/a_util_result_legacy.h
                   lib/a_util_result_legacy.cpp)
target_link_libraries(a_util_result_legacy_shared PRIVATE GTest::gtest)
target_compile_definitions(a_util_result_legacy_shared
                           PRIVATE A_UTIL_ENABLE_DYNAMIC_ERROR_DESCRIPTION
                                   A_UTIL_ENABLE_COMPILER_CXX_VARIADIC_TEMPLATES=1
                                   A_UTIL_ENABLE_COMPILER_CXX_RVALUE_REFERENCES=1
                                   A_UTIL_DO_EXPORT)
# We cannot change the implementation, because we want to test against the old implementation
# So we have to get rid of the warnings via compiler flags
target_compile_options(a_util_result_legacy_shared PRIVATE $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wno-pedantic -Wno-extra>)
target_include_directories(a_util_result_legacy_shared SYSTEM PRIVATE lib/a_util_5_0_0)
target_compile_features(a_util_result_legacy_shared PRIVATE cxx_std_14)
set_target_properties(a_util_result_legacy_shared
                      PROPERTIES FOLDER test/function/a_util/result/dependencies)

# actual tester executable
add_executable(result_type_compat_test result_type_compat_test.cpp)
target_link_libraries(result_type_compat_test
                      PRIVATE GTest::gtest_main a_util_result_legacy_shared dev_essential::result)
add_test(result_type_compat_test result_type_compat_test)
set_target_properties(result_type_compat_test PROPERTIES FOLDER test/function/a_util/result)
target_compile_definitions(result_type_compat_test PRIVATE A_UTIL_DO_EXPORT)
