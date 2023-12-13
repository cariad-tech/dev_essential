# Copyright @ 2021 VW Group. All rights reserved.
#
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

add_library(ddl_capi SHARED ddl_capi.cpp
                            ../../include/ddl_capi/ddl_capi.h)

set_target_properties(ddl_capi PROPERTIES FOLDER ddl_capi)

target_compile_definitions(ddl_capi PUBLIC A_UTIL_DO_EXPORT)

target_link_libraries(ddl_capi PUBLIC preprocessor PRIVATE ddl)

install(TARGETS ddl_capi)
install(DIRECTORY ${dev_essential_SOURCE_DIR}/include/ddl_capi DESTINATION include)

# test ddl-c-api, only for internal use
add_library(ddl_capi_c_test OBJECT ddl_capi.c)
set_target_properties(ddl_capi_c_test PROPERTIES FOLDER ddl_capi)
target_link_libraries(ddl_capi_c_test PRIVATE ddl_capi)
