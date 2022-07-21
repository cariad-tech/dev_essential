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

add_subdirectory(base)
add_subdirectory(concurrency)
add_subdirectory(datetime)
add_subdirectory(filesystem)
add_subdirectory(logging)
add_subdirectory(memory)
add_subdirectory(csv_reader)
add_subdirectory(regex)
add_subdirectory(result)
add_subdirectory(strings)
add_subdirectory(system)
add_subdirectory(variant)
add_subdirectory(xml)
add_subdirectory(process)
add_subdirectory(preprocessor)
add_subdirectory(pkg_rpc)
add_subdirectory(ddl)
add_subdirectory(ddl_capi)

##get all tests listed inside ***_test_projects.cmake files (as cmake subdirectories)
include(../../extern/3rdparty/3rdparty_test_projects.cmake)
