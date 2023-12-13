# Copyright @ 2021 VW Group. All rights reserved.
#
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

add_library(variant STATIC
            ../../include/a_util/variant.h
            ../../include/a_util/variant/variant.h
            variant.cpp   #common implementation
            )
target_link_libraries(variant PUBLIC base
                              PRIVATE memory)
set_target_properties(variant PROPERTIES FOLDER a_util
                                         OUTPUT_NAME a_util_variant)
install(TARGETS variant)
install(DIRECTORY ${dev_essential_SOURCE_DIR}/include/a_util/variant DESTINATION include/a_util)
install(FILES ${dev_essential_SOURCE_DIR}/include/a_util/variant.h DESTINATION include/a_util)
