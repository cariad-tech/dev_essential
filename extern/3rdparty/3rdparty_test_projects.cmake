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

##This file is included by integrated test projects and standalone project
# For the standalone test the directory is not a subdirectory, hence always add a custom binary dir.
# Also, this file is included from different directories depending on integrated or standalone test build,
# therefore the actual subdirectory needs to be prefixed by CMAKE_CURRENT_LIST_DIR
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/clara/test ${CMAKE_CURRENT_BINARY_DIR}/clara/test)