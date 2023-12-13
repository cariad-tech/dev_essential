/**
 * @file
 *
 * Copyright @ 2021 VW Group. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <ddl/mapping/engine/trigger.h>

using namespace ddl::mapping::rt;

TriggerBase::~TriggerBase()
{
}

TargetSet& TriggerBase::getTargetList()
{
    return _targets;
}

a_util::result::Result TriggerBase::addTarget(Target* target)
{
    _targets.insert(target);
    return a_util::result::SUCCESS;
}

a_util::result::Result TriggerBase::removeTarget(Target* target)
{
    _targets.erase(target);
    return a_util::result::SUCCESS;
}
