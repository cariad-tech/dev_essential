# Copyright @ 2021 VW Group. All rights reserved.
#
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

add_subdirectory(../../extern/3rdparty/pugixml ${CMAKE_BINARY_DIR}/extern/3rdparty/pugixml)

add_library(xml STATIC
            ../../include/a_util/xml.h
            ../../include/a_util/xml/dom.h
            dom.cpp
            )
target_link_libraries(xml PUBLIC base
                          PRIVATE strings
                                  $<BUILD_INTERFACE:pugixml>)
set_target_properties(xml PROPERTIES FOLDER a_util
                                     OUTPUT_NAME a_util_xml)
install(TARGETS xml)
install(DIRECTORY ${dev_essential_SOURCE_DIR}/include/a_util/xml DESTINATION include/a_util)
install(FILES ${dev_essential_SOURCE_DIR}/include/a_util/xml.h DESTINATION include/a_util)
