# Copyright @ 2021 VW Group. All rights reserved.
#
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

add_library(csv_reader STATIC
            ../../include/a_util/parser.h
            ../../include/a_util/parser/csv_reader.h
            csv_reader.cpp
            )
target_link_libraries(csv_reader PUBLIC base)
set_target_properties(csv_reader PROPERTIES FOLDER a_util
                                            OUTPUT_NAME a_util_csv_reader)

install(TARGETS csv_reader)
install(DIRECTORY ${dev_essential_SOURCE_DIR}/include/a_util/parser DESTINATION include/a_util)
install(FILES ${dev_essential_SOURCE_DIR}/include/a_util/parser.h DESTINATION include/a_util)
