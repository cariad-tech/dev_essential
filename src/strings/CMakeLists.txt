# Copyright @ 2023 VW Group. All rights reserved.
#
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

add_subdirectory(../../3rdparty/utfcpp ${CMAKE_BINARY_DIR}/3rdparty/utfcpp)

add_library(strings STATIC
            ../../include/a_util/strings.h
            ../../include/a_util/strings/strings_convert.h
            ../../include/a_util/strings/strings_convert_decl.h
            ../../include/a_util/strings/strings_format.h
            ../../include/a_util/strings/strings_functions.h
            ../../include/a_util/strings/strings_numeric_traits.h
            ../../include/a_util/strings/strings_numeric_traits_decl.h
            ../../include/a_util/strings/unicode.h
            ../../include/a_util/strings/detail/strings_convert_impl.h
            ../../include/a_util/strings/detail/strings_numeric_traits_impl.h
            strings_format_detail.h
            strings_default_conversion.h
            std_to_detail.h
            strings.cpp
            strings_format.cpp
            strings_convert.cpp
            strings_default_conversion.cpp
            strings_numeric_to_string.cpp
            strings_numeric_traits.cpp
            unicode.cpp
            )
if(MSVC)
    set(_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING
        $<$<COMPILE_FEATURES:cxx_std_17>:_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING>)
    set_source_files_properties(unicode.cpp PROPERTIES COMPILE_DEFINITIONS
                                ${_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING})
endif()
target_include_directories(strings SYSTEM
                           PRIVATE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>) # 3rdparty/
target_link_libraries(strings PUBLIC base)
set_target_properties(strings PROPERTIES 
    FOLDER a_util
    OUTPUT_NAME a_util_strings
    DEBUG_POSTFIX "d"
    COMPILE_PDB_NAME_DEBUG "a_util_stringsd"
    COMPILE_PDB_NAME_RELWITHDEBINFO "a_util_strings"
    COMPILE_PDB_NAME_RELEASE "a_util_strings"
    COMPILE_PDB_NAME_MINSIZEREL "a_util_strings"
)

if(MSVC)
    get_property(is_multi_config GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
    if (is_multi_config)
        set(pdb_file_name "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/a_util_strings$<$<CONFIG:Debug>:d>.pdb")
    else()
        set(pdb_file_name "${CMAKE_CURRENT_BINARY_DIR}/a_util_strings$<$<CONFIG:Debug>:d>.pdb")
    endif()

    install(
            FILES
                    "${pdb_file_name}"
            DESTINATION
                    "lib"
            CONFIGURATIONS
                    "Debug" "RelWithDebInfo"
    )
endif()
install(TARGETS strings)
install(DIRECTORY ${dev_essential_SOURCE_DIR}/include/a_util/strings DESTINATION include/a_util)
install(FILES ${dev_essential_SOURCE_DIR}/include/a_util/strings.h DESTINATION include/a_util)
