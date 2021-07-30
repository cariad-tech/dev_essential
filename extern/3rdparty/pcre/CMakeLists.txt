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

# PCRE is not built as separate target, but its CMake generates some build headers that we need.
# Therefore we generate the header using the shipped CMake, but exclude the targets from our Solution.
# explicitly set the following variables into the parent scope, to be usable in the modules
set(PCRE_DIR ${CMAKE_CURRENT_LIST_DIR}/pcre-8.39 CACHE INTERNAL "Path to PCRE dir")
set(PCRE_CUSTOM_BINARY_DIR ${PROJECT_BINARY_DIR}/pcre_prebuild  CACHE INTERNAL "Path to PCRE binary dir")

set(PCRE_BUILD_PCREGREP OFF)
set(PCRE_BUILD_TESTS OFF)
set(PCRE_SHOW_REPORT OFF)
add_subdirectory(${PCRE_DIR} ${PCRE_CUSTOM_BINARY_DIR} EXCLUDE_FROM_ALL)
install(FILES pcre-8.39/LICENCE DESTINATION doc/license/pcre-8.39)
