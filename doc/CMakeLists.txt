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

##installation of documentation related files
install(FILES extern/changelog.md DESTINATION doc)
install(FILES extern/license/MPL2.0.txt DESTINATION ${CMAKE_INSTALL_PREFIX})
install(DIRECTORY extern/license DESTINATION doc)

# check for valid version string
string(REGEX MATCH "^[0-9]+\.[0-9]+\.[0-9]+$" VER ${VERSION})
if(NOT VER)
    message(FATAL_ERROR "${VERSION} is not a valid semver espression for version")
endif()

# check for valid entry in doxyfile
file(STRINGS extern/Doxyfile RES REGEX "^PROJECT_NUMBER += v${VER}$")
if(NOT RES)
    message(FATAL_ERROR "Doxyfile does not contain the current version ${VER}")
endif()
