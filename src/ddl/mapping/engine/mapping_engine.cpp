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
#include <ddl/legacy_error_macros.h>
#include <ddl/mapping/engine/mapping_engine.h>

namespace ddl {
namespace mapping {
namespace rt {
// define all needed error types and values locally
_MAKE_RESULT(-4, ERR_POINTER);
_MAKE_RESULT(-5, ERR_INVALID_ARG);
_MAKE_RESULT(-16, ERR_NOT_IMPL);
_MAKE_RESULT(-40, ERR_INVALID_STATE);
_MAKE_RESULT(-42, ERR_INVALID_TYPE);
} // namespace rt
} // namespace mapping
} // namespace ddl

using namespace ddl::mapping;
using namespace ddl::mapping::rt;

MappingEngine::MappingEngine(IMappingEnvironment& oEnv) : _env(oEnv), _running(false), _map_config()
{
}

MappingEngine::~MappingEngine()
{
    stop();
    unmapAll();
}

a_util::result::Result MappingEngine::setConfiguration(const MapConfiguration& oConfig)
{
    _map_config = oConfig;
    return a_util::result::SUCCESS;
}

a_util::result::Result MappingEngine::Map(const std::string& strTargetName, handle_t& hMappedSignal)
{
    // If the target is already in the List, return invalid error
    if (_targets.find(strTargetName) != _targets.end()) {
        return ERR_INVALID_ARG;
    }

    const MapTarget* const pMapTarget = _map_config.getTarget(strTargetName);
    // If the target is not found in the mapping configuration, return invalid error
    if (nullptr == pMapTarget) {
        return ERR_INVALID_ARG;
    }

    const char* desc = nullptr;
    if (!_env.resolveType(pMapTarget->getType().c_str(), desc)) {
        return ERR_INVALID_TYPE;
    }
    if (nullptr == desc) {
        return ERR_POINTER;
    }
    // save a copy, never know how long the environment holds the memory
    const std::string strTargetDesc = desc;

    // Backup storage for error cleanup
    SourceMap oSourceCleanup;
    TriggerMap oTriggerCleanup;
    a_util::result::Result nRes;

    // Create sources
    const MapSourceNameList& lstSources = pMapTarget->getReferencedSources();
    for (auto it = lstSources.cbegin(); it != lstSources.cend(); ++it) {
        const MapSource* const pMapSource = _map_config.getSource(*it);
        if (_sources.find(pMapSource->getName()) == _sources.end()) {
            const char* strSourceDesc = nullptr;
            nRes = _env.resolveType(pMapSource->getType().c_str(), strSourceDesc);
            if (!nRes) {
                break;
            }

            auto pSrc = std::make_unique<Source>(_env);
            nRes = pSrc->create(*pMapSource, strSourceDesc);
            if (!nRes) {
                break;
            }

            _sources[pMapSource->getName()] = pSrc.get();
            oSourceCleanup[pMapSource->getName()] = pSrc.release();
        }
    }

    Target* pTarget = nullptr;
    if (nRes) {
        // Create Target
        auto tmp_target = std::make_unique<Target>(_env);
        nRes = tmp_target->create(_map_config, *pMapTarget, strTargetDesc, _sources);
        if (nRes) {
            pTarget = tmp_target.release();
            _targets[strTargetName] = pTarget;
        }
    }

    if (nRes) {
        // Create Triggers
        const MapTriggerList& oTriggerList = pMapTarget->getTriggerList();
        for (auto it = oTriggerList.cbegin(); it != oTriggerList.cend(); it++) {
            const MapPeriodicTrigger* const pMapPTrigger =
                dynamic_cast<const MapPeriodicTrigger*>(*it);
            const MapSignalTrigger* const pMapSigTrigger =
                dynamic_cast<const MapSignalTrigger*>(*it);
            const MapDataTrigger* const pMapDataTrigger = dynamic_cast<const MapDataTrigger*>(*it);

            if (!pMapPTrigger && !pMapSigTrigger && !pMapDataTrigger) {
                nRes = ERR_NOT_IMPL;
                break;
            }

            if (pMapPTrigger) {
                std::string strTrigName;
                if (pMapPTrigger->getPeriod() == 0) {
                    strTrigName = "ini";
                }
                else {
                    strTrigName = a_util::strings::toString(pMapPTrigger->getPeriod()) + "ms";
                }

                if (_triggers.find(strTrigName) == _triggers.end()) {
                    auto pTrigger = std::make_unique<PeriodicTrigger>(
                        _env, strTrigName, pMapPTrigger->getPeriod());
                    nRes = pTrigger->create();
                    if (!nRes) {
                        break;
                    }

                    _triggers[strTrigName] = pTrigger.get();
                    oTriggerCleanup[strTrigName] = pTrigger.release();
                }

                _triggers[strTrigName]->addTarget(pTarget);
            }

            if (pMapSigTrigger) {
                const std::string strSourceName = pMapSigTrigger->getVariable();

                TriggerMap::iterator iter = _triggers.find(strSourceName);
                if (iter == _triggers.end()) {
                    SignalTrigger* const pTrigger = new SignalTrigger(_env, strSourceName);
                    std::pair<TriggerMap::iterator, bool> oNewElem =
                        _triggers.insert(std::make_pair(strSourceName, pTrigger));
                    iter = oNewElem.first;

                    oTriggerCleanup[strSourceName] = pTrigger;
                }

                iter->second->addTarget(pTarget);
                _sources[strSourceName]->addTrigger(_map_config, iter->second);
            }

            if (pMapDataTrigger) {
                std::string name = pMapDataTrigger->getSource() + "." +
                                   pMapDataTrigger->getVariable() + pMapDataTrigger->getOperator();
                name.append(a_util::strings::format("%f", pMapDataTrigger->getValue()));

                TriggerMap::iterator iter = _triggers.find(name);
                if (_triggers.find(name) == _triggers.end()) {
                    DataTrigger* pTrigger = new DataTrigger(_env,
                                                            name,
                                                            pMapDataTrigger->getVariable(),
                                                            pMapDataTrigger->getOperator(),
                                                            pMapDataTrigger->getValue());
                    std::pair<TriggerMap::iterator, bool> oNewElem =
                        _triggers.insert(std::make_pair(name, pTrigger));
                    iter = oNewElem.first;

                    oTriggerCleanup[name] = pTrigger;
                }

                iter->second->addTarget(pTarget);
                _sources[pMapDataTrigger->getSource()]->addTrigger(_map_config, iter->second);
            }
        }
    }

    // cleanup on error
    if (!nRes) {
        for (SourceMap::iterator it = oSourceCleanup.begin(); it != oSourceCleanup.end(); ++it) {
            _sources.erase(it->first);
            delete it->second;
        }

        for (TriggerMap::iterator it = _triggers.begin(); it != _triggers.end(); ++it) {
            it->second->removeTarget(pTarget);
        }

        for (TriggerMap::iterator it = oTriggerCleanup.begin(); it != oTriggerCleanup.end(); ++it) {
            _triggers.erase(it->first);
            delete it->second;
        }
    }

    if (nRes && pTarget) {
        hMappedSignal = pTarget;
        _env.targetMapped(strTargetName.c_str(),
                          pTarget->getTypeName().c_str(),
                          hMappedSignal,
                          pTarget->getSize());
    }

    return nRes;
}

a_util::result::Result MappingEngine::reset()
{
    if (_running) {
        return ERR_INVALID_STATE;
    }

    // Reset the target buffers
    for (TargetMap::iterator it = _targets.begin(); it != _targets.end(); it++) {
        RETURN_IF_FAILED(it->second->reset(_map_config));
    }

    return a_util::result::SUCCESS;
}

a_util::result::Result MappingEngine::start()
{
    if (_running) {
        return ERR_INVALID_STATE;
    }
    _running = true;

    // start the triggers in order to send targets
    for (TriggerMap::iterator it = _triggers.begin(); it != _triggers.end(); it++) {
        it->second->start();
    }

    return a_util::result::SUCCESS;
}

bool MappingEngine::hasTriggers(handle_t mapped_signal) const
{
    Target* target_signal = reinterpret_cast<Target*>(mapped_signal);
    if (!target_signal) {
        return false;
    }
    bool has_trigger = false;
    for (const auto& trigga: _triggers) {
        if (has_trigger) {
            break;
        }
        const auto& target_list = trigga.second->getTargetList();
        for (const auto& target: target_list) {
            if (target == target_signal) {
                has_trigger = true;
                break;
            }
        }
    }
    return has_trigger;
}

a_util::result::Result MappingEngine::stop()
{
    if (!_running) {
        return ERR_INVALID_STATE;
    }
    _running = false;

    // stop the triggers in order to send targets
    for (TriggerMap::iterator it = _triggers.begin(); it != _triggers.end(); it++) {
        it->second->stop();
    }
    return a_util::result::SUCCESS;
}

a_util::result::Result MappingEngine::unmap(handle_t hMappedSignal)
{
    if (_running) {
        return ERR_INVALID_STATE;
    }

    Target* pTarget = reinterpret_cast<Target*>(hMappedSignal);
    if (!pTarget) {
        return ERR_POINTER;
    }

    std::vector<TriggerMap::iterator> vecEraseTrig;
    for (TriggerMap::iterator it = _triggers.begin(); it != _triggers.end(); ++it) {
        it->second->removeTarget(pTarget);
        if (it->second->getTargetList().empty()) {
            delete it->second;
            vecEraseTrig.push_back(it);
        }
    }
    for (std::vector<TriggerMap::iterator>::iterator it = vecEraseTrig.begin();
         it != vecEraseTrig.end();
         ++it) {
        _triggers.erase(*it);
    }

    std::vector<SourceMap::iterator> vecEraseSrc;
    for (SourceMap::iterator it = _sources.begin(); it != _sources.end(); ++it) {
        it->second->removeAssignmentsFor(pTarget);
        if (it->second->getAssigmentList().empty()) {
            delete it->second;
            vecEraseSrc.push_back(it);
        }
    }
    for (std::vector<SourceMap::iterator>::iterator it = vecEraseSrc.begin();
         it != vecEraseSrc.end();
         ++it) {
        _sources.erase(*it);
    }

    _env.targetUnmapped(pTarget->getName().c_str(), hMappedSignal);
    _targets.erase(pTarget->getName());
    delete pTarget;

    return a_util::result::SUCCESS;
}

a_util::result::Result MappingEngine::unmapAll()
{
    if (_running) {
        return ERR_INVALID_STATE;
    }

    while (!_targets.empty()) {
        unmap(reinterpret_cast<handle_t>(_targets.begin()->second));
    }

    return a_util::result::SUCCESS;
}

a_util::result::Result MappingEngine::getCurrentData(handle_t hMappedSignal,
                                                     void* pTargetBuffer,
                                                     size_t szTargetBuffer) const
{
    Target* pTarget = reinterpret_cast<Target*>(hMappedSignal);
    if (!pTarget) {
        return ERR_POINTER;
    }
    if (!pTargetBuffer) {
        return ERR_POINTER;
    }

    return pTarget->getCurrentBuffer(pTargetBuffer, szTargetBuffer);
}

// empty definitions of mapping environment destruktors
ISignalListener::~ISignalListener()
{
}
IPeriodicListener::~IPeriodicListener()
{
}
IMappingEnvironment::~IMappingEnvironment()
{
}
