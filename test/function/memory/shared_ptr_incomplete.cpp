/**
 * @file
 * Incomplete test implementation
 *
 * Copyright @ 2021 VW Group. All rights reserved.
 *
 *     This Source Code Form is subject to the terms of the Mozilla
 *     Public License, v. 2.0. If a copy of the MPL was not distributed
 *     with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * If it is not possible or desirable to put the notice in a particular file, then
 * You may include the notice in a location (such as a LICENSE file in a
 * relevant directory) where a recipient would be likely to look for such a notice.
 *
 * You may add additional accurate notices of copyright ownership.
 */

#include "a_util/memory.h"

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
