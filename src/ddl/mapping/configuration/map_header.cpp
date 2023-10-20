/**
 * @file
 *
 * Copyright @ 2021 VW Group. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <a_util/datetime.h>
#include <a_util/system.h>
#include <ddl/mapping/configuration/map_configuration.h>
#include <ddl/mapping/configuration/map_header.h>
#include <ddl/utilities/std_to_string.h>

#include <sstream>

namespace ddl {
namespace mapping {
// define all needed error types and values locally
_MAKE_RESULT(-5, ERR_INVALID_ARG);
} // namespace mapping
} // namespace ddl

using namespace ddl::mapping;

MapHeader::MapHeader()
{
    reset();
}

const std::string& MapHeader::getDescription() const
{
    return _desc;
}

const std::string& MapHeader::getDllPaths() const
{
    return _ddl;
}

const std::string& MapHeader::getLanguageVersion() const
{
    return _lang_version;
}

const std::string& MapHeader::getAuthor() const
{
    return _author;
}

const std::string& MapHeader::getCreationDate() const
{
    return _creation_date;
}

const std::string& MapHeader::getModificationDate() const
{
    return _mod_date;
}

void MapHeader::reset()
{
    _desc = "Auto-generated with Signal Mapping Editor";
    _ddl.clear();
    _lang_version = "1.00";
    _author = a_util::system::getCurrentUserName();
    _creation_date = a_util::datetime::getCurrentLocalDateTime().format("%c");
    _mod_date = _creation_date;
}

a_util::result::Result MapHeader::setDescription(const std::string& strDescription)
{
    _desc = strDescription;
    return a_util::result::SUCCESS;
}

a_util::result::Result MapHeader::setDdlPaths(const std::string& strDdlPaths)
{
    _ddl = strDdlPaths;
    return a_util::result::SUCCESS;
}

a_util::result::Result MapHeader::parseVersion(const std::string& version, MapErrorList& lstErrors)
{
    if (version.empty()) {
        lstErrors.push_back("DDL mapping <language_version> is empty");
        return ERR_INVALID_ARG;
    }

    const auto dotpos = version.find_first_of('.');

    std::string major_token;
    std::string minor_token;
    if (dotpos == std::string::npos) {
        std::stringstream ss;
        ss << "DDL mapping, error converting <language_version> content: " << version
           << ", could not find dot '.' separator";
        lstErrors.push_back(ss.str());
        return ERR_INVALID_ARG;
    }
    else {
        major_token = version.substr(0, dotpos);
        if (major_token.empty()) {
            std::stringstream ss;
            ss << "DDL mapping, error parsing <language_version> content: " << version
               << ", major version is empty";
            lstErrors.push_back(ss.str());
            return ERR_INVALID_ARG;
        }

        auto rst = version.size() - (dotpos + 1);
        if (rst > 0) {
            minor_token = version.substr(dotpos + 1, version.size() - rst + 1);
        }
    }

    uint32_t major = 0;

    try {
        major = std::stoul(major_token);
    }
    catch (const std::exception& ex) {
        std::stringstream ss;
        ss << "DDL mapping, error converting <language_version> content: " << version
           << ", major version: " + major_token + " invalid: " + ex.what() << ".";
        lstErrors.push_back(ss.str());
        return ERR_INVALID_ARG;
    }

    uint32_t minor = 0;

    if (minor_token.empty()) {
        std::stringstream ss;
        ss << "DDL mapping, error parsing <language_version> content: " << version
           << ", minor version is empty";
        lstErrors.push_back(ss.str());
        return ERR_INVALID_ARG;
    }
    else {
        try {
            minor = std::stoul(minor_token);
        }
        catch (const std::exception& ex) {
            std::stringstream ss;
            ss << "DDL mapping, error converting <language_version> content " << version
               << ", minor version: " + minor_token + " invalid: " + ex.what() << ".";
            lstErrors.push_back(ss.str());
            return ERR_INVALID_ARG;
        }
    }
    if ((major != 1)) {
        std::stringstream ss;
        ss << "DDL mapping, error converting <language_version> content " << version
           << ", parsed major version: " << major
           << ", is not 1. No other language version exists.";
        lstErrors.push_back(ss.str());
        return ERR_INVALID_ARG;
    }
    else if (minor != 0) {
        std::stringstream ss;
        ss << "DDL mapping, error converting <language_version> content " << version
           << ", parsed minor version: " << minor
           << ", is not 0. No other language version exists.";
        lstErrors.push_back(ss.str());
        return ERR_INVALID_ARG;
    }
    else {
        return a_util::result::SUCCESS;
    }
}

a_util::result::Result MapHeader::loadFromDOM(const a_util::xml::DOMElement& oHeader,
                                              MapErrorList& lstErrors)
{
    const a_util::xml::DOMElement oLang = oHeader.getChild("language_version");
    const a_util::xml::DOMElement oAuthor = oHeader.getChild("author");
    const a_util::xml::DOMElement oCreated = oHeader.getChild("date_creation");
    const a_util::xml::DOMElement oModified = oHeader.getChild("date_change");
    const a_util::xml::DOMElement oDescription = oHeader.getChild("description");

    if (oLang.isNull() || oAuthor.isNull() || oCreated.isNull() || oModified.isNull() ||
        oDescription.isNull()) {
        lstErrors.push_back("Missing <language_version>, <author>, <date_creation>, "
                            "<date_change> or <description> element in header");
        return ERR_INVALID_ARG;
    }

    const auto header_parse_result = parseVersion(oLang.getData(), lstErrors);
    if (header_parse_result) {
        _lang_version = oLang.getData();
    }
    else {
        return header_parse_result;
    }

    _author = oAuthor.getData();
    _creation_date = oCreated.getData();
    _mod_date = oModified.getData();
    setDescription(oDescription.getData());

    // optional
    const a_util::xml::DOMElement oDdlPaths = oHeader.getChild("ddl");
    if (!oDdlPaths.isNull()) {
        setDdlPaths(oDdlPaths.getData());
    }
    else {
        _ddl.clear();
    }

    return a_util::result::SUCCESS;
}

a_util::result::Result MapHeader::writeToDOM(a_util::xml::DOMElement oDOMElement)
{
    oDOMElement.setName("header");

    _mod_date = a_util::datetime::getCurrentLocalDateTime().format("%c");

    a_util::xml::DOMElement oLang = oDOMElement.createChild("language_version");
    a_util::xml::DOMElement oAuthor = oDOMElement.createChild("author");
    a_util::xml::DOMElement oCreated = oDOMElement.createChild("date_creation");
    a_util::xml::DOMElement oModified = oDOMElement.createChild("date_change");
    a_util::xml::DOMElement oDescription = oDOMElement.createChild("description");

    oLang.setData(_lang_version);
    oAuthor.setData(_author);
    oCreated.setData(_creation_date);
    oDescription.setData(_desc);
    oModified.setData(_mod_date);

    if (!_ddl.empty()) {
        a_util::xml::DOMElement oDdlPaths = oDOMElement.createChild("ddl");
        oDdlPaths.setData(_ddl);
    }

    return a_util::result::SUCCESS;
}
