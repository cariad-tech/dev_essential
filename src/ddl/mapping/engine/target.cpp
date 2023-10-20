/**
 * @file
 *
 * Copyright @ 2021 VW Group. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <a_util/result/error_def.h>
#include <ddl/codec/legacy/access_element.h>
#include <ddl/legacy_error_macros.h>
#include <ddl/mapping/configuration/map_configuration.h>
#include <ddl/mapping/engine/target.h>

#include <memory> //std::unique_ptr<>

namespace ddl {
namespace mapping {
namespace rt {
// define all needed error types and values locally
_MAKE_RESULT(-3, ERR_UNEXPECTED);
_MAKE_RESULT(-4, ERR_POINTER);
_MAKE_RESULT(-12, ERR_MEMORY);
_MAKE_RESULT(-20, ERR_INVALID_ELEMENT);
} // namespace rt
} // namespace mapping
} // namespace ddl

using namespace ddl::mapping;
using namespace ddl::mapping::rt;

Target::Target(IMappingEnvironment& oEnv) : _counter(0), _env(oEnv)
{
}

Target::~Target()
{
    for (TargetElementList::iterator it = _target_elements.begin(); it != _target_elements.end();
         ++it) {
        delete *it;
    }
    _target_elements.clear();
    _simulation_time_elements.clear();
}

a_util::result::Result Target::create(const MapConfiguration& oMapConfig,
                                      const MapTarget& oMapTarget,
                                      const std::string& strTargetDescription,
                                      SourceMap& oSources)
{
    _name = oMapTarget.getName();
    _type_name = oMapTarget.getType();
    ddl::codec::CodecFactory oFactory(_type_name.c_str(), strTargetDescription.c_str());
    RETURN_IF_FAILED(oFactory.isValid());

    // Alloc and zero memory
    _buffer.resize(oFactory.getStaticBufferSize(), 0);

    // Begin here, end when target is destroyed or after reset
    _codec = std::make_unique<ddl::codec::StaticCodec>(
        oFactory.makeStaticCodecFor(&_buffer[0], _buffer.size()));

    // this works only because datamodel was set via dd::DataDefintion which calculated the TypeInfo
    const auto struct_type_access =
        dd::StructTypeAccess(oMapConfig.getDD()->getStructTypes().get(_type_name));
    if (!struct_type_access) {
        return ERR_POINTER;
    }

    // Create element for each assignment in mapping configuration
    const MapAssignmentList& oAssignmentList = oMapTarget.getAssignmentList();
    for (MapAssignmentList::const_iterator itAssignment = oAssignmentList.begin();
         itAssignment != oAssignmentList.end();
         ++itAssignment) {
        auto elem_access = struct_type_access.getElementByPath(itAssignment->getTo());
        if (!elem_access) {
            return ERR_MEMORY;
        }
        bool bIsArrayElement = elem_access.getArrayPos();

        size_t szElement = elem_access.getElement().getArraySize().getArraySizeValue();
        if (bIsArrayElement) {
            szElement = 1;
        }

        std::string strPath = itAssignment->getTo();
        if (szElement > 1) {
            strPath.append("[0]");
        }

        auto struct_type_of_element = elem_access.getStructType();
        if (struct_type_of_element) {
            // No StructElement is created for the actual struct, only for its elements. Therefore,
            // if the assignment is a struct, we need to amend the path by the first element of the
            // struct
            if (struct_type_of_element->getElements().getSize() == 0)
                return ERR_UNEXPECTED;
            strPath.push_back('.');
            strPath.append((*struct_type_of_element->getElements().cbegin())->getName());
        }

        void* pTargetElementPtr = {};
        try {
            pTargetElementPtr = _codec->getElement(strPath).getAddress();
        }
        catch (const std::exception& ex) {
            RETURN_ERROR_DESCRIPTION(ERR_INVALID_ELEMENT, ex.what());
        }

        // Create target element
        std::unique_ptr<TargetElement> pElement = std::make_unique<TargetElement>(this);
        RETURN_IF_FAILED(
            pElement->create(pTargetElementPtr, elem_access, szElement, itAssignment->getTo()));

        // If a constant is given, there can be no transformation
        if (!itAssignment->getConstant().empty()) {
            _constant_elements.push_back(make_pair(itAssignment->getConstant(), pElement.get()));
        }
        else if (!itAssignment->getFunction().empty()) {
            if (itAssignment->getFunction() == "simulation_time") {
                _simulation_time_elements.push_back(pElement.get());
            }
            else if (itAssignment->getFunction() == "trigger_counter") {
                uint64_t nMod = 0;
                if (!itAssignment->getModulo().empty()) {
                    nMod = a_util::strings::toUInt64(itAssignment->getModulo());
                }
                _counter_elements.push_back(std::make_pair(nMod, pElement.get()));
            }
            else if (itAssignment->getFunction() == "received") {
                // Add assignments to source
                SourceMap::iterator itSource = oSources.find(itAssignment->getSource());
                if (itSource == oSources.end()) {
                    return ERR_UNEXPECTED;
                }

                RETURN_IF_FAILED(
                    itSource->second->addAssignment(oMapConfig, "received()", pElement.get()));
            }
        }
        else {
            // Add Transformation for Mapping configuration
            const std::string& strTransformation = itAssignment->getTransformation();
            if (!strTransformation.empty()) {
                RETURN_IF_FAILED(
                    pElement->setTransformation(oMapConfig.getTransformation(strTransformation)));
            }

            // Add assignments to source
            SourceMap::iterator itSource = oSources.find(itAssignment->getSource());
            if (itSource == oSources.end()) {
                return ERR_UNEXPECTED;
            }

            // getFrom() is empty when the source itself is referenced
            RETURN_IF_FAILED(itSource->second->addAssignment(
                oMapConfig, itAssignment->getFrom(), pElement.get()));
        }

        _target_elements.push_back(pElement.release());
    }

    // initialize memory
    RETURN_IF_FAILED(reset(oMapConfig));

    return a_util::result::SUCCESS;
}

a_util::result::Result Target::reset(const MapConfiguration& oMapConfig)
{
    a_util::result::Result nResult = a_util::result::SUCCESS;
    // Reset Counter
    _counter = 0;

    auto struct_type_access =
        dd::StructTypeAccess(oMapConfig.getDD()->getStructTypes().get(_type_name));
    if (!struct_type_access) {
        return ERR_POINTER;
    }

    // Set default values/constants or zero the content
    _codec->resetValues(true);

    // Set constant elements from the map configuration
    if (nResult) {
        for (Constants::iterator it = _constant_elements.begin(); it != _constant_elements.end();
             it++) {
            nResult = it->second->setDefaultValue(it->first);
        }
    }
    return nResult;
}

const std::string& Target::getName() const
{
    return _name;
}

const std::string& Target::getTypeName() const
{
    return _type_name;
}

const TargetElementList& Target::getElementList() const
{
    return _target_elements;
}

size_t Target::getSize() const
{
    return _buffer.size();
}

a_util::result::Result Target::updateAccessFunctionValues()
{
    // set simulation time for simulation_time assignments
    timestamp_t tmTime = _env.getTime();
    for (std::vector<TargetElement*>::const_iterator it = _simulation_time_elements.begin();
         it != _simulation_time_elements.end();
         it++) {
        (*it)->setValue(&tmTime, e_int64, sizeof(timestamp_t));
    }

    return a_util::result::SUCCESS;
}

a_util::result::Result Target::updateTriggerFunctionValues()
{
    // increment counter
    _counter++;

    // write counter into trigger_counter assignments
    for (TriggerCounters::const_iterator it = _counter_elements.begin();
         it != _counter_elements.end();
         it++) {
        uint64_t nValue = _counter;
        if (it->first != 0) {
            nValue %= it->first;
        }
        it->second->setValue(&nValue, e_uint64, sizeof(nValue));
    }

    return a_util::result::SUCCESS;
}

a_util::result::Result Target::getCurrentBuffer(void* pTargetBuffer, size_t szTargetBuffer)
{
    if (szTargetBuffer < _buffer.size()) {
        return ERR_MEMORY;
    }

    updateAccessFunctionValues();

    aquireReadLock();
    a_util::memory::copy(pTargetBuffer, szTargetBuffer, &_buffer[0], _buffer.size());
    releaseReadLock();
    return a_util::result::SUCCESS;
}

a_util::result::Result Target::getBufferRef(const void*& pBuffer, size_t& szTargetBuffer)
{
    updateAccessFunctionValues();

    // synchronization is done by the caller!
    pBuffer = &_buffer[0];
    szTargetBuffer = _buffer.size();

    return a_util::result::SUCCESS;
}
