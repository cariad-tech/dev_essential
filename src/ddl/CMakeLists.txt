# Copyright @ 2021 VW Group. All rights reserved.
#
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

# Include headers
include(utilities/utilities.headers)
include(datamodel/datamodel.headers)
include(dd/dd.headers)
include(utilities/utilities.headers)

include(codec/codec.headers)
include(serialization/serialization.headers)
include(mapping/mapping.headers)


# Include sources
include(utilities/utilities.sources)
include(datamodel/datamodel.sources)
include(dd/dd.sources)

include(codec/codec.sources)
include(serialization/serialization.sources)
include(mapping/mapping.sources)

add_library(ddl STATIC
    ${DD_UTILITIES_CPP}
    ${DD_DATAMODEL_CPP}
    ${DD_CPP}
    ${CODEC_CPP}
    ${SERIALIZATION_CPP}
    ${MAPPING_CPP}

    ${DD_UTILITIES_H}
    ${DD_DATAMODEL_H}
    ${DD_H}
    ${CODEC_H}
    ${SERIALIZATION_H}
    ${MAPPING_CONFIGURATION_H}
    ${MAPPING_ENGINE_H}
    ${MAPPING_H}
    ../../include/ddl/ddl.h
    ../../include/ddl/legacy_error_macros.h
)

set_target_properties(ddl PROPERTIES 
    FOLDER ddl
    OUTPUT_NAME ddl
    DEBUG_POSTFIX "d"
    COMPILE_PDB_NAME_DEBUG "ddld"
    COMPILE_PDB_NAME_RELWITHDEBINFO "ddl"
    COMPILE_PDB_NAME_RELEASE "ddl"
    COMPILE_PDB_NAME_MINSIZEREL "ddl"
)

if(MSVC)
    get_property(is_multi_config GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
    if (is_multi_config)
        set(pdb_file_name "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/ddl$<$<CONFIG:Debug>:d>.pdb")
    else()
        set(pdb_file_name "${CMAKE_CURRENT_BINARY_DIR}/ddl$<$<CONFIG:Debug>:d>.pdb")
    endif()

    install(
        FILES
            "${pdb_file_name}"
        DESTINATION
            "lib"
        CONFIGURATIONS
            "Debug" "RelWithDebInfo"
    )
endif()

# a_util is public since its part of the ddl api
target_link_libraries(ddl PUBLIC concurrency result memory variant xml
                          PRIVATE logging datetime system)
target_compile_options(ddl PUBLIC $<$<AND:$<NOT:$<CXX_COMPILER_ID:MSVC>>,$<COMPILE_LANGUAGE:CXX>>:-frtti>
                                  $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<COMPILE_LANGUAGE:CXX>>:/GR>)
target_compile_definitions(ddl PRIVATE DEV_ESSENTIAL_DISABLE_DEPRECATED_WARNINGS)

if (dev_essential_force_type_access_list_compatibility)
    message(STATUS "Set forced compatibility mode for TypeAccessList dev_essential_TYPE_ACCESS_LIST_COMPATIBILITY=1")
    target_compile_definitions(ddl PUBLIC -Ddev_essential_TYPE_ACCESS_LIST_COMPATIBILITY=1)
else()
    if (MSVC)
        if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 19.20)
            message(STATUS "Set compatibility mode for TypeAccessList dev_essential_TYPE_ACCESS_LIST_COMPATIBILITY=1")
            target_compile_definitions(ddl PUBLIC -Ddev_essential_TYPE_ACCESS_LIST_COMPATIBILITY=1)
        else()
            message(STATUS "Switch off compatibility mode for TypeAccessList dev_essential_TYPE_ACCESS_LIST_COMPATIBILITY=0")
            target_compile_definitions(ddl PUBLIC -Ddev_essential_TYPE_ACCESS_LIST_COMPATIBILITY=0)
        endif()
    endif()
    
    if(GCC)
        if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 8)
            message(STATUS "Set compatibility mode for TypeAccessList dev_essential_TYPE_ACCESS_LIST_COMPATIBILITY=1")
            target_compile_definitions(ddl PUBLIC -Ddev_essential_TYPE_ACCESS_LIST_COMPATIBILITY=1)
        else()
            message(STATUS "Switch off compatibility mode for TypeAccessList dev_essential_TYPE_ACCESS_LIST_COMPATIBILITY=0")
            target_compile_definitions(ddl PUBLIC -Ddev_essential_TYPE_ACCESS_LIST_COMPATIBILITY=0)
        endif()
    endif()
endif()


install(TARGETS ddl)
install(FILES ${DD_UTILITIES_INSTALL} DESTINATION ${DD_UTILITIES_DIR})
install(FILES ${DD_DATAMODEL_INSTALL} DESTINATION ${DD_DATAMODEL_DIR})
install(FILES ${DD_INSTALL} DESTINATION ${DD_H_DIR})
install(FILES ${CODEC_INSTALL} DESTINATION ${CODEC_DIR})
install(FILES ${CODEC_INSTALL_LEGACY} DESTINATION ${CODEC_DIR}/legacy)
install(FILES ${SERIALIZATION_INSTALL} DESTINATION ${SERIALIZATION_DIR})
install(FILES ${MAPPING_CONFIGURATION_INSTALL} DESTINATION ${MAPPING_DIR}/configuration)
install(FILES ${MAPPING_ENGINE_INSTALL} DESTINATION ${MAPPING_DIR}/engine)
install(FILES ${MAPPING_INSTALL} DESTINATION ${MAPPING_DIR})
install(FILES ../../include/ddl/ddl.h
              ../../include/ddl/legacy_error_macros.h
        DESTINATION include/ddl)
install(FILES ${dev_essential_SOURCE_DIR}/doc/html/ddl/specification/ddl3.xsd
              ${dev_essential_SOURCE_DIR}/doc/html/ddl/specification/ddl4.xsd
              ${dev_essential_SOURCE_DIR}/doc/html/ddl/specification/mapping_configuration.xsd
        DESTINATION doc/specification)
install(DIRECTORY ${dev_essential_SOURCE_DIR}/include/ddl DESTINATION include)
