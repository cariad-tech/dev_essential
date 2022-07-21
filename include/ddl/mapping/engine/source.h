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
 *
 */

#ifndef DDL_MAPPING_RT_SOURCE_HEADER
#define DDL_MAPPING_RT_SOURCE_HEADER

#include <a_util/result.h>
#include <ddl/codec/codec_factory.h>
#include <ddl/mapping/configuration/map_source.h>
#include <ddl/mapping/engine/element.h>
#include <ddl/mapping/engine/mapping_environment_intf.h>

#include <memory>

namespace ddl {
namespace mapping {
namespace rt {
///@cond nodoc
struct AssignmentStruct {
    size_t buffer_size;
    uint32_t type32;
    uintptr_t element_ptr_offset;

    AssignmentStruct() : buffer_size(0), type32(0), element_ptr_offset(static_cast<uintptr_t>(-1))
    {
    }

    bool operator==(const AssignmentStruct& other) const
    {
        return (element_ptr_offset == other.element_ptr_offset);
    }
};
///@endcond nodoc

} // namespace rt
} // namespace mapping
} // namespace ddl

namespace ddl {
namespace mapping {
namespace rt {
class TriggerBase;
class Target;
class TargetElement;
/**
 * @brief the engine source
 *
 */
class Source : public ISignalListener {
public: // types
#if defined(__GNUC__) && (__GNUC__ == 5) && defined(__QNX__)
#pragma GCC diagnostic ignored                                                                     \
    "-Wattributes" // standard type attributes are ignored when used in templates
#endif

    // Map of source elements and a vector of target elements
    /// @cond nodec
    typedef std::vector<std::pair<AssignmentStruct, TargetElementList>> Assignments;
    typedef std::vector<std::pair<TriggerBase*, AssignmentStruct>> Triggers;
    typedef std::vector<uint8_t> MemoryBuffer;
    typedef std::set<const Target*> TargetRefList;
    /// @endcond nodec

#if defined(__GNUC__) && (__GNUC__ == 5) && defined(__QNX__)
#pragma GCC diagnostic warning                                                                     \
    "-Wattributes" // standard type attributes are ignored when used in templates
#endif

public:
    /**
     * CTOR
     * @param [in] env The mapping environment
     */
    Source(IMappingEnvironment& env);

    /**
     * @brief no copy CTOR
     */
    Source(const Source&) = delete;

    /**
     * @brief no copy assignment operator
     * @return Source&
     */
    Source& operator=(const Source&) = delete;

    /**
     * DTOR
     */
    ~Source();

    /**
     * Creation method to fill the object with data.
     * @param[in] map_source The soucre representation from mapping configuration
     * @param[in] type_description The description for the source type
     * @retval a_util::result::SUCCESS      Everything went fine
     * @retval ERR_FAILED       Error while filing the object
     */
    a_util::result::Result create(const mapping::MapSource& map_source,
                                  const std::string& type_description);

    /**
     * Method to add a new pair of source element and target element to the intern assignment list
     * @param[in] map_config The Configuration instance
     * @param[in] source_element The name of the source element to map
     * @param[in] target_element The target element instance
     * @retval a_util::result::SUCCESS      Everything went fine
     */
    a_util::result::Result addAssignment(const ddl::mapping::MapConfiguration& map_config,
                                         const std::string& source_element,
                                         TargetElement* target_element);

    /**
     * Method to add a new trigger to the intern trigger list
     * @param[in] map_config The Configuration instance
     * @param[in] trigger      The trigger
     * @retval a_util::result::SUCCESS      Everything went fine
     */
    a_util::result::Result addTrigger(const MapConfiguration& map_config, TriggerBase* trigger);

    /**
     * Method to remove all elements of a target from the intern assignment list
     * @param[in] target The target to unmap
     * @retval a_util::result::SUCCESS      Everything went fine
     */
    a_util::result::Result removeAssignmentsFor(const Target* target);

    /**
     * Getter for the source type
     * @return the source type
     */
    const std::string& getTypeName() const;

    /**
     * Getter for the assignment list
     * @return the assignment list
     */
    const Assignments& getAssigmentList() const;

    /**
     * Method to handle a sample when it is received.
     * @param[in] data The memory location of the received buffer
     * @param[in] size The memory size of the received buffer
     * @retval a_util::result::SUCCESS      Everything went fine
     */
    a_util::result::Result onSampleReceived(const void* data, size_t size);

private:
    IMappingEnvironment& _env;
    handle_t _handle;
    std::string _name;
    std::string _type_name;
    std::string _type_description;
    Assignments _assignments;
    TargetRefList _targets;
    std::unique_ptr<ddl::codec::CodecFactory> _codec_factory;
    TypeMap _type_map;
    TargetElementList _received_elements;
    Triggers _triggers;
};

/// Public composite types used in the mapping::rt namespace
typedef std::map<std::string, Source*> SourceMap;

} // namespace rt
} // namespace mapping
} // namespace ddl

#endif // DDL_MAPPING_RT_SOURCE_HEADER
