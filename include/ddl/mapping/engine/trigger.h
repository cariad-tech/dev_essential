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

#ifndef TRIGGER_BASE_HEADER
#define TRIGGER_BASE_HEADER

#include <a_util/result.h>
#include <ddl/mapping/engine/target.h>

namespace ddl {
namespace mapping {
namespace rt {
/// base class representing a trigger
class TriggerBase {
public:
    /**
     * Virtual DTOR
     */
    virtual ~TriggerBase();

    /**
     * Method to start trigger
     *
     * @retval a_util::result::SUCCESS      Everything went fine
     * @retval ERR_FAILED       Error during runtime
     */
    virtual a_util::result::Result start() = 0;

    /**
     * Method to stop trigger
     *
     * @retval a_util::result::SUCCESS      Everything went fine
     */
    virtual a_util::result::Result stop() = 0;

public: // Base implementations
    /**
     * Getter for the list of registered targets
     * @return the list of targets
     */
    TargetSet& getTargetList();

    /**
     * Method to register a new target
     * @param[in] target The target to add
     * @retval a_util::result::SUCCESS      Everything went fine
     */
    a_util::result::Result addTarget(Target* target);

    /**
     * Method to deregister a target
     * @param[in] target The target to remove
     * @retval a_util::result::SUCCESS      Everything went fine
     */
    a_util::result::Result removeTarget(Target* target);

protected:
    /// nodoc
    TargetSet _targets;
};

/**
 * @brief type for trigger map.
 *
 */
typedef std::map<std::string, TriggerBase*> TriggerMap;

} // namespace rt
} // namespace mapping
} // namespace ddl
#endif // TRIGGER_BASE_HEADER
