/**
 * @file
 * Incomplete test implementation
 *
 * Copyright @ 2021 VW Group. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <a_util/memory.h>

struct Incomplete {
    bool& destroyed;
    Incomplete(bool& destroyed) : destroyed(destroyed)
    {
        destroyed = false;
    }

    ~Incomplete()
    {
        destroyed = true;
    }
};

a_util::memory::shared_ptr<Incomplete> get_new_instance(bool& destroyed)
{
    return a_util::memory::shared_ptr<Incomplete>(new Incomplete(destroyed));
}
