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
set_target_properties(concurrency PROPERTIES FOLDER a_util
                                             OUTPUT_NAME a_util_concurrency)

install(TARGETS concurrency)
install(DIRECTORY ${dev_essential_SOURCE_DIR}/include/a_util/concurrency DESTINATION include/a_util)
install(FILES ${dev_essential_SOURCE_DIR}/include/a_util/concurrency.h DESTINATION include/a_util)
