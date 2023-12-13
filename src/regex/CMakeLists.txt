# Copyright @ 2021 VW Group. All rights reserved.
#
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

add_library(regex STATIC
    ../../include/a_util/regex.h
    ../../include/a_util/regex/regularexpression.h
    regularexpression.cpp
)
target_link_libraries(regex PUBLIC base
                            PRIVATE memory)
set_target_properties(regex PROPERTIES FOLDER a_util
                                       OUTPUT_NAME a_util_regex)
install(TARGETS regex)
install(DIRECTORY ${dev_essential_SOURCE_DIR}/include/a_util/regex DESTINATION include/a_util)
install(FILES ${dev_essential_SOURCE_DIR}/include/a_util/regex.h DESTINATION include/a_util)
