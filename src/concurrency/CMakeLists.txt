# Copyright @ 2023 VW Group. All rights reserved.
#
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

add_library(concurrency STATIC
            ../../include/a_util/concurrency.h
            ../../include/a_util/concurrency/atomic.h
            ../../include/a_util/concurrency/shared_mutex.h
            ../../include/a_util/concurrency/thread.h
            ../../include/a_util/concurrency/semaphore.h
            ../../include/a_util/concurrency/fast_mutex.h
            ../../include/a_util/concurrency/detail/semaphore_decl.h
            ../../include/a_util/concurrency/detail/semaphore_impl.h
            atomic_fallback.cpp
            fast_mutex.cpp
            shared_mutex.cpp
            )
target_link_libraries(concurrency PUBLIC base)

find_package(Threads REQUIRED)
target_link_libraries(concurrency PRIVATE $<BUILD_INTERFACE:Threads::Threads>)

set_target_properties(concurrency PROPERTIES 
    FOLDER a_util
    OUTPUT_NAME a_util_concurrency
    DEBUG_POSTFIX "d"
    COMPILE_PDB_NAME_DEBUG "a_util_concurrencyd"
    COMPILE_PDB_NAME_RELWITHDEBINFO "a_util_concurrency"
    COMPILE_PDB_NAME_RELEASE "a_util_concurrency"
    COMPILE_PDB_NAME_MINSIZEREL "a_util_concurrency"
)

if(MSVC)
    get_property(is_multi_config GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
    if (is_multi_config)
        set(pdb_file_name "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/a_util_concurrency$<$<CONFIG:Debug>:d>.pdb")
    else()
        set(pdb_file_name "${CMAKE_CURRENT_BINARY_DIR}/a_util_concurrency$<$<CONFIG:Debug>:d>.pdb")
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

install(TARGETS concurrency)
install(DIRECTORY ${dev_essential_SOURCE_DIR}/include/a_util/concurrency DESTINATION include/a_util)
install(FILES ${dev_essential_SOURCE_DIR}/include/a_util/concurrency.h DESTINATION include/a_util)
