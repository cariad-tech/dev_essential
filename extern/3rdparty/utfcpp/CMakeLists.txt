# Copyright @ 2021 VW Group. All rights reserved.
#
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

set(UTFCPP_DIR ${CMAKE_CURRENT_LIST_DIR}/utf8_v2_3_4/source CACHE INTERNAL
    "Path to the utfcpp directory")
install(FILES utf8_v2_3_4/source/utf8.h DESTINATION doc/license/utf8_v2_3_4/source)
