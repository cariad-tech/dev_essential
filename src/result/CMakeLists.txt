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

add_subdirectory(../../extern/3rdparty/boost ${CMAKE_BINARY_DIR}/extern/3rdparty/boost)

option(dev_essential_cmake_enable_result_header_only "Whether or not to build header only version\
 of dev_essential_result library (default: OFF)" OFF)

# Distinguish between header only standalone library and fully featured component of dev_essential library
if (NOT ${dev_essential_cmake_enable_result_header_only})
    option(dev_essential_cmake_enable_dynamic_error_description "Enable dynamic memory allocation for\
 dev_essential_result component (default: ON)" ON)

    add_library(result STATIC
                ##public
                ../../include/a_util/result.h
                ../../include/a_util/result/error_def.h
                ../../include/a_util/result/result_info.h
                ../../include/a_util/result/result_info_decl.h
                ../../include/a_util/result/result_type.h
                ../../include/a_util/result/result_type_decl.h
                ../../include/a_util/result/result_util.h
                ../../include/a_util/result/detail/error_description.h
                ../../include/a_util/result/detail/error_description_intf.h
                ../../include/a_util/result/detail/error_description_impl.h
                ../../include/a_util/result/detail/reference_counted_object_intf.h
                ../../include/a_util/result/detail/reference_counted_object.h
                ../../include/a_util/result/detail/result_description_decl.h
                ../../include/a_util/result/detail/result_description_impl.h
                ../../include/a_util/result/detail/result_info_impl.h
                ../../include/a_util/result/detail/result_type_impl.h
                ../../include/a_util/result/detail/reference_counted_object_disable_dynamic_alloc.h
                result_util.cpp
                )
    target_compile_definitions(result PRIVATE
                               $<$<STREQUAL:${dev_essential_cmake_enable_dynamic_error_description},ON>:A_UTIL_ENABLE_DYNAMIC_ERROR_DESCRIPTION>)

    set_target_properties(result PROPERTIES FOLDER a_util
                                            OUTPUT_NAME a_util_result)
    target_link_libraries(result PRIVATE strings)
    install(TARGETS result)
    install(DIRECTORY ${dev_essential_SOURCE_DIR}/include/a_util/result DESTINATION include/a_util)
else()
    # Standalone header only project with minor missing features
    add_library(result INTERFACE)
    install(DIRECTORY ${dev_essential_SOURCE_DIR}/include/a_util/result DESTINATION include/a_util
            PATTERN "error_def.h" EXCLUDE   #dependency to dev_essential_strings
            PATTERN "result_util.h" EXCLUDE #dependency to dev_essential_strings
            )
    configure_file(empty_header.h.in error_def.h COPY_ONLY)
    configure_file(empty_header.h.in result_util.h COPY_ONLY)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/error_def.h ${CMAKE_CURRENT_BINARY_DIR}/result_util.h
            DESTINATION include/a_util/result)
endif()

target_include_directories(result SYSTEM
                           INTERFACE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/extern>) # 3rdparty
target_link_libraries(result INTERFACE base)
install(FILES ${dev_essential_SOURCE_DIR}/include/a_util/result.h DESTINATION include/a_util)
install(FILES ${dev_essential_SOURCE_DIR}/scripts/debugging/result.natvis
        DESTINATION scripts/debugging)
