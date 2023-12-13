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
#include <ddl/mapping/engine/data_trigger.h>
#include <ddl/mapping/engine/signal_trigger.h>
#include <ddl/mapping/engine/source.h>

#include <assert.h>

namespace ddl {
namespace mapping {
namespace rt {
// define all needed error types and values locally
_MAKE_RESULT(-4, ERR_POINTER);
_MAKE_RESULT(-42, ERR_INVALID_TYPE);
_MAKE_RESULT(-20, ERR_INVALID_ELEMENT);
} // namespace rt
} // namespace mapping
} // namespace ddl

using namespace ddl::mapping;
using namespace ddl::mapping::rt;

Source::Source(IMappingEnvironment& oEnv) : _env(oEnv), _handle(0)
{
    _type_map["tUInt8"] = e_uint8;
    _type_map["tUInt16"] = e_uint16;
    _type_map["tUInt32"] = e_uint32;
    _type_map["tUInt64"] = e_uint64;
    _type_map["tInt8"] = e_int8;
    _type_map["tInt16"] = e_int16;
    _type_map["tInt32"] = e_int32;
    _type_map["tInt64"] = e_int64;
    _type_map["tFloat32"] = e_float32;
    _type_map["tFloat64"] = e_float64;
    _type_map["tBool"] = e_bool;
    _type_map["tChar"] = e_char;
}

Source::~Source()
{
    if (_handle) {
        _env.unregisterSource(_handle);
    }
}

a_util::result::Result Source::create(const MapSource& oMapSource,
                                      const std::string& strTypeDescription)
{
    _name = oMapSource.getName();
    _type_name = oMapSource.getType();
    _type_description = strTypeDescription;

    _codec_factory =
        std::make_unique<ddl::codec::CodecFactory>(_type_name.c_str(), _type_description.c_str());
    if (_codec_factory->isValid()) {
        return _env.registerSource(_name.c_str(), _type_name.c_str(), this, _handle);
    }

    return ERR_INVALID_TYPE;
}

a_util::result::Result Source::addTrigger(const MapConfiguration& oMapConfig, TriggerBase* oTrigger)
{
    Triggers::iterator it = _triggers.begin();
    for (; it != _triggers.end(); ++it) {
        if (it->first == oTrigger)
            break;
    }

    if (it == _triggers.end()) {
        AssignmentStruct oStruct;
        DataTrigger* pDataTrigger = dynamic_cast<DataTrigger*>(oTrigger);

        if (pDataTrigger) {
            // Get structure from datamodel which must have set the TypeModel!
            auto struct_access =
                dd::StructTypeAccess(oMapConfig.getDD()->getStructTypes().get(_type_name));
            if (!struct_access) {
                return ERR_POINTER;
            }
            auto elem_access = struct_access.getElementByPath(pDataTrigger->getVariable());
            if (!elem_access) {
                return ERR_POINTER;
            }

            // Get ID in DataDefinition
            ddl::codec::CodecFactory oFac(struct_access);
            ddl::codec::StaticDecoder oDecoder =
                _codec_factory->makeStaticDecoderFor(NULL, oFac.getStaticBufferSize());

            try {
                // Get element pointer offset
                oStruct.element_ptr_offset = reinterpret_cast<uintptr_t>(
                    oDecoder.getElement(pDataTrigger->getVariable()).getAddress());
            }
            catch (const std::exception& ex) {
                RETURN_ERROR_DESCRIPTION(ERR_INVALID_ELEMENT, ex.what());
            }

            auto data_type_name = elem_access.getElement().getTypeName();
            // Get Type from TypeMap
            TypeMap::const_iterator it_name = _type_map.find(data_type_name);
            if (it_name != _type_map.end()) {
                oStruct.type32 = it_name->second;
            }

            // find element size
            auto data_type_source = elem_access.getDataType();
            if (data_type_source) {
                oStruct.buffer_size = data_type_source->getBitSize() / 8;
            }
            else {
                return ERR_INVALID_TYPE;
            }
        }

        _triggers.push_back(std::make_pair(oTrigger, oStruct));
    }

    return a_util::result::SUCCESS;
}

const std::string& Source::getTypeName() const
{
    return _type_name;
}

const Source::Assignments& Source::getAssigmentList() const
{
    return _assignments;
}

a_util::result::Result Source::addAssignment(const MapConfiguration& oMapConfig,
                                             const std::string& strSourceElement,
                                             TargetElement* pTargetElement)
{
    if (strSourceElement == "received()") {
        _received_elements.push_back(pTargetElement);
    }
    else {
        AssignmentStruct oStruct;

        // Get structure from DataDefinition
        auto struct_access =
            dd::StructTypeAccess(oMapConfig.getDD()->getStructTypes().get(_type_name));
        if (!struct_access) {
            return ERR_POINTER;
        }

        size_t szArraySize = 1;

        // If strSourceElement is empty, the whole Structure will be copied
        if (strSourceElement.empty()) {
            oStruct.buffer_size = _codec_factory->getStaticBufferSize();
            oStruct.element_ptr_offset = 0;
        }
        else {
            // Get Element from DataDefinition Struct
            auto elem_access = struct_access.getElementByPath(strSourceElement);
            if (!elem_access) {
                return ERR_POINTER;
            }
            bool bIsArrayElement = elem_access.getArrayPos();

            // Get ID in DataDefinition Coder
            if (!bIsArrayElement) {
                szArraySize = elem_access.getElement().getArraySize().getArraySizeValue();
            }

            // Get Type from TypeMap
            TypeMap::const_iterator it = _type_map.find(elem_access.getElement().getTypeName());
            if (it != _type_map.end()) {
                oStruct.type32 = it->second;
            }

            // find element size
            auto type_of_type = elem_access.getTypeOfType();

            if (type_of_type == ddl::dd::data_type) {
                oStruct.buffer_size = elem_access.getDataType()->getBitSize() * szArraySize / 8;
            }
            else if (type_of_type == ddl::dd::struct_type) {
                // TODO: check if this struct can be a different one than the one in _codec_factory
                ddl::codec::CodecFactory oFactory(elem_access.getStructType()->getName().c_str(),
                                                  _type_description.c_str());

                oStruct.buffer_size = oFactory.getStaticBufferSize();
            }
            else if (type_of_type == ddl::dd::enum_type) {
                // for enum types the base type is also set!
                oStruct.buffer_size = elem_access.getDataType()->getBitSize() * szArraySize / 8;
                ;

                // we need to set the interpretation type to the underlying type here
                it = _type_map.find(elem_access.getDataType()->getName());
                if (it != _type_map.end()) {
                    oStruct.type32 = it->second;
                }
            }
            else {
                return ERR_INVALID_TYPE;
            }

            std::string strPath = strSourceElement;
            if (szArraySize > 1) {
                strPath.append("[0]");
            }

            ddl::codec::CodecFactory oFac(struct_access);
            ddl::codec::StaticDecoder oDecoder =
                _codec_factory->makeStaticDecoderFor(NULL, oFac.getStaticBufferSize());

            try {
                // Get element pointer offset
                oStruct.element_ptr_offset =
                    reinterpret_cast<uintptr_t>(oDecoder.getElement(strPath).getAddress());
            }
            catch (const std::exception& ex) {
                RETURN_ERROR_DESCRIPTION(ERR_INVALID_TYPE, ex.what());
            }
        }

        Assignments::iterator itAssigns = _assignments.end();
        for (itAssigns = _assignments.begin(); itAssigns != _assignments.end(); ++itAssigns) {
            if (itAssigns->first == oStruct) {
                break;
            }
        }

        if (itAssigns == _assignments.end()) {
            _assignments.push_back(std::make_pair(oStruct, TargetElementList()));
            _assignments.back().second.push_back(pTargetElement);
        }
        else {
            itAssigns->second.push_back(pTargetElement);
        }
    }

    _targets.insert(pTargetElement->getTarget());

    return a_util::result::SUCCESS;
}

a_util::result::Result Source::removeAssignmentsFor(const Target* pTarget)
{
    for (Assignments::iterator itAssignments = _assignments.begin();
         itAssignments != _assignments.end();) {
        TargetElementList& oAssignedElements = itAssignments->second;
        for (TargetElementList::iterator itElements = oAssignedElements.begin();
             itElements != oAssignedElements.end();) {
            if ((*itElements)->getTarget() == pTarget) {
                // element is owned and deleted by target
                itElements = oAssignedElements.erase(itElements);
            }
            else {
                ++itElements;
            }
        }

        if (itAssignments->second.empty()) {
            itAssignments = _assignments.erase(itAssignments);
        }
        else {
            ++itAssignments;
        }
    }

    _targets.erase(pTarget);

    return a_util::result::SUCCESS;
}

a_util::result::Result Source::onSampleReceived(const void* pData, size_t)
{
    if (!pData) {
        return ERR_POINTER;
    }

    // lock all currently mapped target buffers
    // note: theres some optimization potential here:
    //   -- rework the assignment container to be able to sort the assignments by target
    //   -- lock only the current target during iteration over all assignments
    // for now it is fast enough since the number of mapped targets isn't to high
    for (TargetRefList::const_iterator it = _targets.begin(); it != _targets.end(); ++it) {
        (*it)->aquireWriteLock();
    }

    // write true into all received(<this_signal>) assignments
    bool bValue = true;
    for (TargetElementList::iterator it = _received_elements.begin();
         it != _received_elements.end();
         it++) {
        (*it)->setValue(&bValue, e_bool, sizeof(bValue));
    }

    // write all assignments that stem from this source
    for (Assignments::iterator itAssign = _assignments.begin(); itAssign != _assignments.end();
         ++itAssign) {
        void* pValue = (void*)((uintptr_t)pData + itAssign->first.element_ptr_offset);

        const std::vector<TargetElement*>& vecElements = itAssign->second;
        uint32_t type32 = itAssign->first.type32;
        size_t buffer_size = itAssign->first.buffer_size;

        for (size_t idx = 0; idx < vecElements.size(); ++idx) {
            vecElements[idx]->setValue(pValue, type32, buffer_size);
        }
    }

    // unlock all currently mapped target buffers
    for (TargetRefList::const_iterator it = _targets.begin(); it != _targets.end(); ++it) {
        (*it)->releaseWriteLock();
    }

    // call signal triggers
    for (Triggers::iterator it = _triggers.begin(); it != _triggers.end(); it++) {
        SignalTrigger* pSigTrigger = dynamic_cast<SignalTrigger*>(it->first);
        DataTrigger* pDataTrigger = dynamic_cast<DataTrigger*>(it->first);
        if (pSigTrigger) {
            pSigTrigger->transmit();
        }
        else if (pDataTrigger) {
            // read the current value and cast it into a float64
            double f64Val = 0;
            void* pValue = (void*)((uintptr_t)pData + it->second.element_ptr_offset);
            switch (it->second.type32) {
            case e_uint8:
                f64Val = *(static_cast<uint8_t*>(pValue));
                break;
            case e_uint16:
                f64Val = *(static_cast<uint16_t*>(pValue));
                break;
            case e_uint32:
                f64Val = *(static_cast<uint32_t*>(pValue));
                break;
            case e_uint64:
                f64Val = static_cast<double>(*(static_cast<uint64_t*>(pValue)));
                break;
            case e_int8:
                f64Val = *(static_cast<int8_t*>(pValue));
                break;
            case e_int16:
                f64Val = *(static_cast<int16_t*>(pValue));
                break;
            case e_int32:
                f64Val = *(static_cast<int32_t*>(pValue));
                break;
            case e_int64:
                f64Val = static_cast<double>(*(static_cast<int64_t*>(pValue)));
                break;
            case e_float32:
                f64Val = *(static_cast<float*>(pValue));
                break;
            case e_float64:
                f64Val = *(static_cast<double*>(pValue));
                break;
            case e_bool:
                f64Val = *(static_cast<bool*>(pValue));
                break;
            case e_char:
                f64Val = *(static_cast<char*>(pValue));
                break;
            default:
                assert(false);
                break;
            }

            // compare/interpret the operator comparison
            if (pDataTrigger->compare(f64Val)) {
                pDataTrigger->transmit();
            }
        }
    }

    return a_util::result::SUCCESS;
}
