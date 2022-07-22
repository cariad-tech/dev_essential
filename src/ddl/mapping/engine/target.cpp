/**
 * @file
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

#include <a_util/result/error_def.h>
#include <ddl/codec/access_element.h>
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
    ddl::CodecFactory oFactory(_type_name.c_str(), strTargetDescription.c_str());
    RETURN_IF_FAILED(oFactory.isValid());

    // Alloc and zero memory
    _buffer.resize(oFactory.getStaticBufferSize(), 0);

    // Begin here, end when target is destroyed or after reset
    _codec.reset(new ddl::StaticCodec(oFactory.makeStaticCodecFor(&_buffer[0], _buffer.size())));

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

        size_t nIdx = 0;
        RETURN_IF_FAILED(ddl::access_element::findIndex(*_codec, strPath, nIdx));
        void* const pTargetElementPtr = _codec->getElementAddress(nIdx);

        // Create target element
        std::unique_ptr<TargetElement> pElement(new TargetElement(this));
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

    // Set default values from DataDefinition in a newly created data sample
    for (size_t nIdx = 0; nIdx < _codec->getElementCount(); ++nIdx) {
        const ddl::StructElement* pElement = NULL;
        if (isFailed(_codec->getElement(nIdx, pElement))) {
            continue;
        }

        auto elem_access = struct_type_access.getElementByPath(pElement->name);

        std::string strDefaultValue;
        if (elem_access) {
            // Element Description was found ... check for a default value
            if (!elem_access.getElement().getDefault().empty()) {
                strDefaultValue = elem_access.getElement().getDefault();
            }
        }
        else {
            // Set error if no error happend so far
            if (isOk(nResult)) {
                nResult = ERR_UNEXPECTED;
            }
        }

        if (strDefaultValue.empty()) {
            // No default detected, just reset this entry
            ddl::access_element::reset(*_codec, pElement->name);
        }
        else {
            a_util::variant::Variant var;
            switch (pElement->type) {
            case a_util::variant::VT_Bool:
                var.reset(a_util::strings::toBool(strDefaultValue));
                break;
            case a_util::variant::VT_Int8:
                var.reset(a_util::strings::toInt8(strDefaultValue));
                break;
            case a_util::variant::VT_UInt8:
                var.reset(a_util::strings::toUInt8(strDefaultValue));
                break;
            case a_util::variant::VT_Int16:
                var.reset(a_util::strings::toInt16(strDefaultValue));
                break;
            case a_util::variant::VT_UInt16:
                var.reset(a_util::strings::toUInt16(strDefaultValue));
                break;
            case a_util::variant::VT_Int32:
                var.reset(a_util::strings::toInt32(strDefaultValue));
                break;
            case a_util::variant::VT_UInt32:
                var.reset(a_util::strings::toUInt32(strDefaultValue));
                break;
            case a_util::variant::VT_Int64:
                var.reset(a_util::strings::toInt64(strDefaultValue));
                break;
            case a_util::variant::VT_UInt64:
                var.reset(a_util::strings::toUInt64(strDefaultValue));
                break;
            case a_util::variant::VT_Float:
                var.reset(a_util::strings::toFloat(strDefaultValue));
                break;
            case a_util::variant::VT_Double:
                var.reset(a_util::strings::toDouble(strDefaultValue));
                break;
            default:
                ddl::access_element::reset(*_codec, pElement->name);
                break;
            }

            if (!var.isEmpty()) {
                auto nCurrentResult = _codec->setElementValue(nIdx, var);
                // Set error if no error happend so far
                if (isOk(nResult)) {
                    nResult = nCurrentResult;
                }
            }
        }
    }

    // Set constant elements
    if (isOk(nResult)) {
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
