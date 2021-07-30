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

set(UTFCPP_DIR ${CMAKE_CURRENT_LIST_DIR}/utf8_v2_3_4/source CACHE INTERNAL
    "Path to the utfcpp directory")
install(FILES utf8_v2_3_4/source/utf8.h DESTINATION doc/license/utf8_v2_3_4/source)
