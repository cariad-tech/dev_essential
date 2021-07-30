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

add_subdirectory(../../extern/3rdparty/pcre ${CMAKE_BINARY_DIR}/extern/3rdparty/pcre)

function(pcre_patch_hardcode_mangle filename)
    # copy file to memory
    file(READ ${PCRE_DIR}/${filename} CONTENT)

    # now patch the hardcoded symbol
    string(REPLACE
           "#if defined(__GNUC__) && __GNUC__ >= 3 && defined(__ELF__) && !defined(__INTEL_COMPILER)" "#if 0"
           CONTENT
           "${CONTENT}"
    )

    # write the file to the binary dir
    file(WRITE ${PCRE_CUSTOM_BINARY_DIR}/${filename} "${CONTENT}")
endfunction()

# patch the pcrecpp namespace to be able to link both the old utils and the new utils
pcre_patch_hardcode_mangle(pcrecpp.cc)

set(PCRE_SOURCES
    ${PCRE_CUSTOM_BINARY_DIR}/pcre_chartables.c
    ${PCRE_CUSTOM_BINARY_DIR}/pcrecpp.cc
    ${PCRE_DIR}/pcre_scanner.cc
    ${PCRE_DIR}/pcre_stringpiece.cc
    ${PCRE_DIR}/pcre_byte_order.c
    ${PCRE_DIR}/pcre_compile.c
    ${PCRE_DIR}/pcre_config.c
    ${PCRE_DIR}/pcre_dfa_exec.c
    ${PCRE_DIR}/pcre_exec.c
    ${PCRE_DIR}/pcre_fullinfo.c
    ${PCRE_DIR}/pcre_get.c
    ${PCRE_DIR}/pcre_globals.c
    ${PCRE_DIR}/pcre_jit_compile.c
    ${PCRE_DIR}/pcre_maketables.c
    ${PCRE_DIR}/pcre_newline.c
    ${PCRE_DIR}/pcre_ord2utf8.c
    ${PCRE_DIR}/pcre_refcount.c
    ${PCRE_DIR}/pcre_string_utils.c
    ${PCRE_DIR}/pcre_study.c
    ${PCRE_DIR}/pcre_tables.c
    ${PCRE_DIR}/pcre_ucd.c
    ${PCRE_DIR}/pcre_valid_utf8.c
    ${PCRE_DIR}/pcre_version.c
    ${PCRE_DIR}/pcre_xclass.c
)

add_library(pcre_object OBJECT ${PCRE_SOURCES})
target_include_directories(pcre_object
    PRIVATE "$<BUILD_INTERFACE:${PCRE_CUSTOM_BINARY_DIR}>"
            "$<BUILD_INTERFACE:${PCRE_DIR}>"
)
target_compile_definitions(pcre_object PRIVATE HAVE_CONFIG_H PCRE_STATIC pcrecpp=pcrecppinternal)
set_target_properties(pcre_object PROPERTIES FOLDER 3rdparty/object_libraries)

add_library(regex STATIC
    ../../include/a_util/regex.h
    ../../include/a_util/regex/regularexpression.h
    regularexpression.cpp
    $<TARGET_OBJECTS:pcre_object>
)
target_link_libraries(regex PUBLIC base
                            PRIVATE memory)
target_include_directories(regex PRIVATE "$<BUILD_INTERFACE:${PCRE_CUSTOM_BINARY_DIR}>")
target_compile_definitions(regex PRIVATE HAVE_CONFIG_H PCRE_STATIC pcrecpp=pcrecppinternal)
set_target_properties(regex PROPERTIES FOLDER a_util
                                       OUTPUT_NAME a_util_regex)
install(TARGETS regex)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/a_util/regex DESTINATION include/a_util)
install(FILES ${CMAKE_SOURCE_DIR}/include/a_util/regex.h DESTINATION include/a_util)
