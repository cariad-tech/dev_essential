# Copyright @ 2021 VW Group. All rights reserved.
#
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

# Add compat header files as projetc files when using with target_link_libraries(MyProject compat)
add_library(ddl_test_compat INTERFACE)
target_sources(ddl_test_compat INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/adtf_compat.h)