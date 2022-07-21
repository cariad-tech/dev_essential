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

add_subdirectory(../../extern/3rdparty/pugixml ${CMAKE_BINARY_DIR}/extern/3rdparty/pugixml)

add_library(xml STATIC
            ../../include/a_util/xml.h
            ../../include/a_util/xml/dom.h
            dom.cpp
            )
target_link_libraries(xml PUBLIC base
                          PRIVATE $<BUILD_INTERFACE:pugixml>)
set_target_properties(xml PROPERTIES FOLDER a_util
                                     OUTPUT_NAME a_util_xml)
install(TARGETS xml)
install(DIRECTORY ${dev_essential_SOURCE_DIR}/include/a_util/xml DESTINATION include/a_util)
install(FILES ${dev_essential_SOURCE_DIR}/include/a_util/xml.h DESTINATION include/a_util)
