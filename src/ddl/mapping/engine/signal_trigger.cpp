/**
 * @file
 *
 * Copyright @ 2021 VW Group. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <ddl/mapping/engine/signal_trigger.h>

using namespace ddl::mapping::rt;

SignalTrigger::SignalTrigger(IMappingEnvironment& oEnv, const std::string& strTriggerName)
    : _env(oEnv), _name(strTriggerName), _is_running(false)
{
}

SignalTrigger::~SignalTrigger()
{
    // With stop() being a pure virtual function of the base class, we have to tell the compiler to
    // specifically use that one. Without 'SignalTrigger::' an existing overriden function might be
    // called during destruction which is UB because the child class might already been destructed.
    SignalTrigger::stop();
}

a_util::result::Result SignalTrigger::start()
{
    _is_running = true;
    return a_util::result::SUCCESS;
}

a_util::result::Result SignalTrigger::stop()
{
    _is_running = false;
    return a_util::result::SUCCESS;
}

a_util::result::Result SignalTrigger::transmit()
{
    if (_is_running) {
        for (TargetSet::iterator it = _targets.begin(); it != _targets.end(); ++it) {
            const void* pBuffer = NULL;
            size_t szBuffer = 0;
            (*it)->aquireReadLock();
            (*it)->updateTriggerFunctionValues();
            if ((*it)->getBufferRef(pBuffer, szBuffer)) {
                _env.sendTarget((handle_t)*it, pBuffer, szBuffer, 0);
            }
            (*it)->releaseReadLock();
        }
    }

    return a_util::result::SUCCESS;
}
