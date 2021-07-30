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

set(UUID_DIR ${CMAKE_CURRENT_LIST_DIR}/uuid-1.6.2 CACHE INTERNAL "UUID directory")
add_library(uuid OBJECT
    ${UUID_DIR}/uuid++.cc
    ${UUID_DIR}/uuid.c
    ${UUID_DIR}/uuid_dce.c
    ${UUID_DIR}/uuid_mac.c
    ${UUID_DIR}/uuid_md5.c
    ${UUID_DIR}/uuid_prng.c
    ${UUID_DIR}/uuid_sha1.c
    ${UUID_DIR}/uuid_str.c
    ${UUID_DIR}/uuid_time.c
    ${UUID_DIR}/uuid_ui128.c
    ${UUID_DIR}/uuid_ui64.c
)
set_target_properties(uuid PROPERTIES FOLDER 3rdparty)
install(FILES uuid-1.6.2/license.txt DESTINATION doc/license/uuid-1.6.2)
