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

add_subdirectory(../../extern/3rdparty/utfcpp ${CMAKE_BINARY_DIR}/extern/3rdparty/utfcpp)

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
target_include_directories(strings
                           PRIVATE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/extern>) # 3rdparty/
target_link_libraries(strings PUBLIC base)
set_target_properties(strings PROPERTIES FOLDER a_util
                                         OUTPUT_NAME a_util_strings)
install(TARGETS strings)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/a_util/strings DESTINATION include/a_util)
install(FILES ${CMAKE_SOURCE_DIR}/include/a_util/strings.h DESTINATION include/a_util)
