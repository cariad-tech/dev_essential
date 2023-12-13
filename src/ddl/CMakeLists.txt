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
set_target_properties(ddl PROPERTIES FOLDER ddl)

# a_util is public since its part of the ddl api
target_link_libraries(ddl PUBLIC concurrency result memory variant xml
                          PRIVATE logging datetime system)
target_compile_options(ddl PUBLIC $<$<AND:$<NOT:$<CXX_COMPILER_ID:MSVC>>,$<COMPILE_LANGUAGE:CXX>>:-frtti>
                                  $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<COMPILE_LANGUAGE:CXX>>:/GR>)
target_compile_definitions(ddl PRIVATE DEV_ESSENTIAL_DISABLE_DEPRECATED_WARNINGS)

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
install(FILES ${dev_essential_SOURCE_DIR}/doc/extern/ddl/specification/ddl3.xsd
              ${dev_essential_SOURCE_DIR}/doc/extern/ddl/specification/ddl4.xsd
              ${dev_essential_SOURCE_DIR}/doc/extern/ddl/specification/mapping_configuration.xsd
        DESTINATION doc/specification)
install(DIRECTORY ${dev_essential_SOURCE_DIR}/include/ddl DESTINATION include)
