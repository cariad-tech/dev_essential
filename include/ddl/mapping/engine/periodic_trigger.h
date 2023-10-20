/**
 * @file
 *
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */

#ifndef PERIODIC_TRIGGER_HEADER
#define PERIODIC_TRIGGER_HEADER

#include <a_util/result.h>
#include <ddl/mapping/engine/trigger.h>

namespace ddl {
namespace mapping {
namespace rt {
/// PeriodicTrigger implements a concrete periodic trigger in the runtime
class PeriodicTrigger : public TriggerBase, private IPeriodicListener {
public:
    /**
     * CTOR
     * @param [in] env The mapping environment
     * @param [in] trigger_name The name of the trigger
     * @param [in] period The period of the trigger
     */
    PeriodicTrigger(IMappingEnvironment& env, const std::string& trigger_name, double period);

    /**
     * DTOR
     */
    ~PeriodicTrigger();

    /**
     * Creates and initializes the periodic trigger
     */
    a_util::result::Result create();

    /**
     * Method to start trigger
     *
     * @retval a_util::result::SUCCESS      Everything went fine
     * @retval ERR_FAILED       Error during runtime
     */
    a_util::result::Result start();

    /**
     * Method to stop trigger
     *
     * @retval a_util::result::SUCCESS      Everything went fine
     */
    a_util::result::Result stop();

private: // IPeriodicListener
    /// @cond nodoc
    void onTimer(timestamp_t now);

private:
    IMappingEnvironment& _env;
    std::string _name;
    double _period;
    bool _running;
    /// @endcond
};

} // namespace rt
} // namespace mapping
} // namespace ddl
#endif // PERIODIC_TRIGGER_HEADER
