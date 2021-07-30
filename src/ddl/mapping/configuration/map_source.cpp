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

#include "ddl/mapping/configuration/map_source.h"

#include "a_util/result/error_def.h"
#include "ddl/legacy_error_macros.h"
#include "ddl/mapping/configuration/map_configuration.h"

namespace ddl {
namespace mapping {

// define all needed error types and values locally
_MAKE_RESULT(-5, ERR_INVALID_ARG);
_MAKE_RESULT(-40, ERR_INVALID_STATE);
} // namespace mapping
} // namespace ddl

using namespace ddl::mapping;

MapSource::MapSource(MapConfiguration* pConfig) : _is_valid(true), _config(pConfig)
{
}

MapSource::MapSource(MapConfiguration* pConfig, std::string name, std::string type_name)
    : _config(pConfig), _is_valid(true)
{
    _name = name;
    a_util::strings::trim(_name);
    _type_name = type_name;
    a_util::strings::trim(_type_name);
}

const std::string& MapSource::getName() const
{
    return _name;
}

const std::string& MapSource::getType() const
{
    return _type_name;
}

bool MapSource::isValid() const
{
    return _is_valid;
}

a_util::result::Result MapSource::loadFromDOM(const a_util::xml::DOMElement& oSource)
{
    const a_util::xml::DOMAttributes mapAttrs = oSource.getAttributes();
    a_util::xml::DOMAttributes::const_iterator itName = mapAttrs.find("name");
    if (itName == mapAttrs.end() || itName->second.empty()) {
        _config->appendError("Missing or empty name attribute for a <source>");
        return ERR_INVALID_ARG;
    }
    _name = itName->second;
    a_util::strings::trim(_name);

    a_util::xml::DOMAttributes::const_iterator itType = mapAttrs.find("type");
    if (itType == mapAttrs.end() || itType->second.empty()) {
        _config->appendError(a_util::strings::format("Missing or empty type attribute for \
                                         <source> '%s'",
                                                     itName->second.c_str()));
        return ERR_INVALID_ARG;
    }
    _type_name = itType->second;
    a_util::strings::trim(_type_name);

    return a_util::result::SUCCESS;
}

a_util::result::Result MapSource::writeToDOM(a_util::xml::DOMElement& oDOMElement) const
{
    oDOMElement.setName("source");
    oDOMElement.setAttribute("name", _name);
    oDOMElement.setAttribute("type", _type_name);
    return a_util::result::SUCCESS;
}

a_util::result::Result MapSource::setName(const std::string& strNewName)
{
    if (!_config) {
        return ERR_INVALID_STATE;
    }
    RETURN_IF_FAILED(_config->resetErrors());
    RETURN_IF_FAILED(_config->checkSignalName(strNewName));

    // modify references in Targets
    for (MapTargetList::iterator itTrg = _config->_targets.begin();
         itTrg != _config->_targets.end();
         itTrg++) {
        if (itTrg->_sources.find(_name) != itTrg->_sources.end()) {
            itTrg->modifySourceName(_name, strNewName);
        }
    }

    _name = strNewName;

    return _config->checkMappingConsistency();
}

a_util::result::Result MapSource::setType(const std::string& type_name)
{
    if (!_config) {
        return ERR_INVALID_STATE;
    }
    RETURN_IF_FAILED(_config->resetErrors());
    RETURN_IF_FAILED(_config->checkSignalType(type_name));
    _type_name = type_name;
    return _config->checkDDLConsistency();
}
