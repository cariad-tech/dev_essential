# Copyright @ 2021 VW Group. All rights reserved.
#
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

set(PUGI_DIR ${CMAKE_CURRENT_LIST_DIR}/pugixml-1.8 CACHE INTERNAL "Path to the pugi directory")
install(FILES pugixml-1.8/readme.txt DESTINATION doc/license/pugixml-1.8)
add_library(pugixml INTERFACE)
target_include_directories(pugixml SYSTEM INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_definitions(pugixml INTERFACE PUGIXML_HEADER_ONLY
                                             pugi= #defines namespace pugi to an anonymous namespace
                                             $<$<CXX_COMPILER_ID:MSVC>:_CRT_SECURE_NO_WARNINGS>
                          )
