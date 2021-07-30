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

set(PUGI_SOURCES ${PUGI_DIR}/src/pugixml.hpp
                 ${PUGI_DIR}/src/pugiconfig.hpp
                 ${PUGI_DIR}/src/pugixml.cpp)
add_library(pugi_xml OBJECT ${PUGI_SOURCES})
set_target_properties(pugi_xml PROPERTIES FOLDER 3rdparty/object_libraries)

add_library(xml STATIC
            ../../include/a_util/xml.h
            ../../include/a_util/xml/dom.h
            dom.cpp
            $<TARGET_OBJECTS:pugi_xml>
            )
target_link_libraries(xml PUBLIC base)
set_target_properties(xml PROPERTIES FOLDER a_util
                                     OUTPUT_NAME a_util_xml)
install(TARGETS xml)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/a_util/xml DESTINATION include/a_util)
install(FILES ${CMAKE_SOURCE_DIR}/include/a_util/xml.h DESTINATION include/a_util)
