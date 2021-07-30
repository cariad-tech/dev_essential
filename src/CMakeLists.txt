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

##set global compiler options
include(../scripts/cmake/a_util_disable_rtti.cmake)

##set library name conventions
include(../scripts/cmake/set_library_naming_conventions.cmake)

## interface target usable as target to pull in all individual component libraries at once
add_library(dev_essential INTERFACE)
install(TARGETS dev_essential
        EXPORT dev_essential_targets)

# Either build all targets...
set(_targets base
             strings
             filesystem
             memory
             xml
             regex
             variant
             datetime
             system
             concurrency
             logging
             result
             csv_reader
             process
             preprocessor
             pkg_rpc
             ddl
             )

# ... or just a user defined subset
if(NOT "${dev_essential_cmake_include_components}" STREQUAL ".")
    set(_targets ${dev_essential_cmake_include_components})
endif()

include(CMakePackageConfigHelpers)
set(_arch_independent ARCH_INDEPENDENT) # if still set after foreach(), only header only components
foreach(_target ${_targets})
    ## Build and install targets
    add_subdirectory(${_target})

    ## Only interface targets can be exported arch_independent
    get_target_property(_target_type ${_target} TYPE)
    message(DEBUG "---- Build and export ${_target_type}: ${_target}")
    if(DEFINED _arch_independent AND NOT "${_target_type}" STREQUAL "INTERFACE_LIBRARY")
        unset(_arch_independent)
    endif()

    ## Alias targets for correct linking in dev_essential integrated test targets
    add_library(dev_essential::${_target} ALIAS ${_target})
    target_link_libraries(dev_essential INTERFACE ${_target})
    
    ## Add target to export set
    install(TARGETS ${_target} EXPORT dev_essential_targets)
endforeach()

write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/dev_essential-config-version.cmake
                                 COMPATIBILITY AnyNewerVersion
                                 ${_arch_independent})
configure_file(dev_essential-config.cmake.in dev_essential-config.cmake COPYONLY)
install(EXPORT dev_essential_targets
        DESTINATION lib/cmake/dev_essential
        NAMESPACE dev_essential::)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/dev_essential-config-version.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/dev_essential-config.cmake
        DESTINATION lib/cmake/dev_essential)
