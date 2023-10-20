# Copyright @ 2021 VW Group. All rights reserved.
#
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

add_library(logging STATIC
            ../../include/a_util/logging.h
            ../../include/a_util/logging/log.h
            log.cpp
            )
target_link_libraries(logging PUBLIC base
                              PRIVATE filesystem datetime)
set_target_properties(logging PROPERTIES FOLDER a_util
                                         OUTPUT_NAME a_util_logging)
install(TARGETS logging)
install(DIRECTORY ${dev_essential_SOURCE_DIR}/include/a_util/logging DESTINATION include/a_util)
install(FILES ${dev_essential_SOURCE_DIR}/include/a_util/logging.h DESTINATION include/a_util)
