# Copyright @ 2021 VW Group. All rights reserved.
#
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

add_library(datetime STATIC
            ../../include/a_util/datetime.h
            ../../include/a_util/datetime/datetime.h
            datetime.cpp
            )
target_link_libraries(datetime PUBLIC base
                               PRIVATE regex)
set_target_properties(datetime PROPERTIES FOLDER a_util
                                          OUTPUT_NAME a_util_datetime)
install(TARGETS datetime)
install(DIRECTORY ${dev_essential_SOURCE_DIR}/include/a_util/datetime DESTINATION include/a_util)
install(FILES ${dev_essential_SOURCE_DIR}/include/a_util/datetime.h DESTINATION include/a_util)
