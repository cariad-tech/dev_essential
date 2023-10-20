# Copyright @ 2021 VW Group. All rights reserved.
#
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

find_package(dev_essential REQUIRED COMPONENTS ddl)

add_subdirectory(_common)
add_subdirectory(infomodel/src)
add_subdirectory(dd/src)
add_subdirectory(codec_legacy/src)
add_subdirectory(codec/src)
add_subdirectory(mapping/src)
