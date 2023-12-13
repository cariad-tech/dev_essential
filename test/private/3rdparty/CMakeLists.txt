# Copyright @ 2022 VW Group. All rights reserved.
#
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

##This directory is included only as integrated test
# The actual tests are usually included inside the third party repos, so if we use
# add_subdirectory() here, we have to tell CMake the binary dir (the second parameter)
add_subdirectory(${PROJECT_SOURCE_DIR}/extern/3rdparty/clipp/test
                 ${CMAKE_CURRENT_BINARY_DIR}/clipp/test)