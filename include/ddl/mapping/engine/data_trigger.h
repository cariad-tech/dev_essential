/**
 * @file
 *
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

    This Source Code Form is subject to the terms of the Mozilla
    Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

If it is not possible or desirable to put the notice in a particular file, then
You may include the notice in a location (such as a LICENSE file in a
relevant directory) where a recipient would be likely to look for such a notice.

You may add additional accurate notices of copyright ownership.
@endverbatim
 */

#ifndef DATA_TRIGGER_HEADER
#define DATA_TRIGGER_HEADER

#include <ddl/mapping/engine/trigger.h>

namespace ddl {
namespace mapping {
namespace rt {
/**
 * @brief the operator set within the config
 *
 */
enum Operator {
    /// equal op
    e_equal = 1,
    /// not equal op
    e_not_equal,
    /// less op
    e_less_than,
    /// greater op
    e_greater_than,
    /// less or equal op
    e_less_than_equal,
    /// greater or equal op
    e_greater_than_equal
};

/// DataTrigger implements a concrete periodic trigger in the runtime
class DataTrigger : public TriggerBase {
public:
    /**
     * CTOR
     * @param [in] env The mapping environment
     * @param [in] trigger_name The name of the trigger
     * @param [in] variable_name The value name for the operator
     * @param [in] comp_operator operator
     * @param [in] value value for the operator
     */
    DataTrigger(IMappingEnvironment& env,
                const std::string& trigger_name,
                const std::string& variable_name,
                const std::string& comp_operator,
                const double& value);

    /**
     * DTOR
     */
    ~DataTrigger();

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

    /**
     * Method to transmit targets
     *
     * @retval a_util::result::SUCCESS      Everything went fine
     */
    a_util::result::Result transmit();

    /**
     * Method to make the comparison
     *
     * @retval a_util::result::SUCCESS      Everything went fine
     */
    bool compare(double value);

    /**
     * Returns the period of the trigger in ms
     */
    const std::string& getVariable() const;

private:
    IMappingEnvironment& _env;
    std::string _name;
    std::string _variable_name;
    Operator _operator;
    double _value;
    bool _is_running;
    /// @endcond
};

} // namespace rt
} // namespace mapping
} // namespace ddl
#endif // DATA_TRIGGER_HEADER
