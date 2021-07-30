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

set(_clara_version 1.1.2)
install(FILES Clara-${_clara_version}/clara.hpp
        DESTINATION include/3rdparty/clara/Clara-${_clara_version}/single_include)
install(FILES Clara-1.1.2/LICENSE.txt
              Clara-1.1.2/README.md
        DESTINATION doc/license/Clara-1.1.2)

##Packaging
add_library(Clara INTERFACE)
target_include_directories(Clara
    INTERFACE $<INSTALL_INTERFACE:include/3rdparty/clara/Clara-${_clara_version}/single_include>
              "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Clara-${_clara_version}/>"
)
target_compile_features(Clara INTERFACE cxx_std_11)

install(TARGETS Clara EXPORT Clara)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/clara-config-version.cmake
                                 VERSION ${_clara_version}
                                 COMPATIBILITY AnyNewerVersion
                                 ARCH_INDEPENDENT)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/clara-config-version.cmake
        DESTINATION lib/cmake/3rdparty)
install(EXPORT Clara FILE clara-config.cmake DESTINATION lib/cmake/3rdparty)
unset(_clara_version)