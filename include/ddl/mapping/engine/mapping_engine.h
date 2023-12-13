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

/*! Mapping Engine
 *
 * \section intro_sec Introduction
 *
 * This is the Mapping Engine documentation.
 */

#ifndef MAPPINGENGINE_HEADER
#define MAPPINGENGINE_HEADER

#include <ddl/mapping/configuration/map_configuration.h>
#include <ddl/mapping/engine/data_trigger.h>
#include <ddl/mapping/engine/element.h>
#include <ddl/mapping/engine/mapping_environment_intf.h>
#include <ddl/mapping/engine/periodic_trigger.h>
#include <ddl/mapping/engine/signal_trigger.h>
#include <ddl/mapping/engine/source.h>
#include <ddl/mapping/engine/target.h>
#include <ddl/mapping/engine/trigger.h>

namespace ddl {
namespace mapping {
namespace rt {
/**
 * @brief The mapping engine to create the targets.
 *
 */
class MappingEngine {
public:
    /**
     * CTOR
     * @param[in] env - Mapping environment, used as interface
     */
    MappingEngine(IMappingEnvironment& env);

    /**
     * DTOR
     */
    ~MappingEngine();

    /**
     * Setter for the current mapping configuration
     * @param[in] config - The Configuration instance
     *
     * @retval a_util::result::SUCCESS      Everything went fine
     */
    a_util::result::Result setConfiguration(const MapConfiguration& config);

    /**
     * Method to instanciate or expand the mapping structure for one particular target
     * @param [in] target_name The target name
     * @param [out] mapped_signal - Destination parameter for the signal handle
     *
     * @retval a_util::result::SUCCESS      Everything went fine
     * @retval ERR_INVALID_ARG  Error target already mapped or not found in mapping configuration
     * @retval ERR_FAILED       Error during creation of the mapping
     */
    a_util::result::Result Map(const std::string& target_name, handle_t& mapped_signal);

    /**
     * Method to reset target buffers
     *
     * @retval a_util::result::SUCCESS      Everything went fine
     * @retval ERR_FAILED       Error during runtime
     */
    a_util::result::Result reset();

    /**
     * Method to start mapping
     *
     * @retval a_util::result::SUCCESS      Everything went fine
     * @retval ERR_FAILED       Error during runtime
     */
    a_util::result::Result start();

    /**
     * Method to stop mapping
     *
     * @retval a_util::result::SUCCESS      Everything went fine
     */
    a_util::result::Result stop();

    /**
     * Method to remove a target from the existing mapping structure
     * @param [in] mapped_signal - Target handle
     *
     * @retval a_util::result::SUCCESS      Everything went fine
     * @retval ERR_INVALID_ARG  Error target not found in mapping structure
     * @retval ERR_FAILED       Error during removal
     */
    a_util::result::Result unmap(handle_t mapped_signal);

    /**
     * Method to know if signal has triggers or not
     *
     * @param [in] mapped_signal The target handle
     * @retval true Signal has triggers
     * @retval false Signal has no triggers
     */
    bool hasTriggers(handle_t mapped_signal) const;

    /**
     * Method to send current data
     *
     * @param [in] mapped_signal The target handle
     * @param [in] target_buffer The pointer referencing the target buffer
     * @param [in] target_buffer_size The size of the target buffer
     * @retval a_util::result::SUCCESS Everything went fine
     * @retval ERR_INVALID_ARG Invalid target handle or target buffer is NULL
     * @retval ERR_MEMORY      Target buffer not large enough
     */
    a_util::result::Result getCurrentData(handle_t mapped_signal,
                                          void* target_buffer,
                                          size_t target_buffer_size) const;

    /**
     * Method to reinitialize the existing mapping structure
     *
     * @retval a_util::result::SUCCESS      Everything went fine
     * @retval ERR_FAILED       Error during reinitialization
     */
    a_util::result::Result unmapAll();

private:
    /**
     * Method to give an initial value to all targets
     *
     * @retval a_util::result::SUCCESS      Everything went fine
     * @retval ERR_FAILED       Error during initialization
     */
    a_util::result::Result initializeModel();

private:
    IMappingEnvironment& _env;
    bool _running;

    MapConfiguration _map_config;
    TargetMap _targets;
    SourceMap _sources;
    TriggerMap _triggers;
};

} // namespace rt
} // namespace mapping
} // namespace ddl
#endif // MAPPINGENGINE_HEADER
