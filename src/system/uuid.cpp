/**
 * @file
 * UUID API
 *
 * Copyright @ 2021 VW Group. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "uuid++.hh"

#include <a_util/concurrency/fast_mutex.h>
#include <a_util/concurrency/mutex.h>
#include <a_util/system.h>

namespace a_util {
namespace system {
std::string generateUUIDv4()
{
    static uuid instance;
    static a_util::concurrency::fast_mutex mutex;

    std::string res;
    res.reserve(36);

    {
        a_util::concurrency::unique_lock<a_util::concurrency::fast_mutex> lock(mutex);
        instance.make(UUID_MAKE_V4);
        char* ptr = instance.string();
        if (ptr) {
            res.assign(ptr);
            free(ptr);
        }
    }

    return res;
}

} // namespace system
} // namespace a_util
