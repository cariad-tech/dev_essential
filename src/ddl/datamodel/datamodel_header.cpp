/**
 * @file
 * OO DataDefinition Redesign
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

#include "ddl/datamodel/datamodel_header.h"

#include <exception>
#include <utility>

namespace ddl {

namespace dd {

namespace datamodel {

/*************************************************************************************************************/
// Header
/*************************************************************************************************************/
Header::Header(const Version& ddl_version,
               const std::string& author,
               const std::string& date_creation,
               const std::string& date_change,
               const std::string& description,
               const std::vector<ExtDeclaration>& ext_declarations)
    : ModelSubject<Header>(),
      InfoMap(),
      _ddl_version(ddl_version),
      _author(author),
      _date_creation(date_creation),
      _date_change(date_change),
      _description(description),
      _ext_declarations(this, "Header::ExtDeclarations")
{
    for (auto& ext: ext_declarations) {
        getExtDeclarations().add(ext);
    }
}

Header::Header(const Header& other)
    : ModelSubject<Header>(), InfoMap(), _ext_declarations(this, "Header::ExtDeclarations")
{
    *this = other;
}

Header& Header::operator=(const Header& other)
{
    setLanguageVersion(other._ddl_version);
    setAuthor(other._author);
    setDateCreation(other._date_creation);
    setDateChange(other._date_change);
    setDescription(other._description);
    other._ext_declarations.deepCopy(_ext_declarations, this);
    return *this;
}

Version Header::getLanguageVersion() const
{
    return _ddl_version;
}

void Header::setLanguageVersion(const Version& ddl_version)
{
    _ddl_version = ddl_version;
    notifyChanged(item_changed, *this, "version");
}

const std::string& Header::getAuthor() const
{
    return _author;
}

void Header::setAuthor(const std::string& author)
{
    _author = author;
    notifyChanged({}, *this, "author");
}

const std::string& Header::getDateCreation() const
{
    return _date_creation;
}

void Header::setDateCreation(const std::string& date_creation)
{
    _date_creation = date_creation;
    notifyChanged(item_changed, *this, "date_creation");
}

const std::string& Header::getDateChange() const
{
    return _date_change;
}

void Header::setDateChange(const std::string& date_change)
{
    _date_change = date_change;
    notifyChanged(item_changed, *this, "date_change");
}

const std::string& Header::getDescription() const
{
    return _description;
}

void Header::setDescription(const std::string& description)
{
    _description = description;
    notifyChanged(item_changed, *this, "description");
}

const Header::ExtDeclarations& Header::getExtDeclarations() const
{
    return _ext_declarations;
}

Header::ExtDeclarations& Header::getExtDeclarations()
{
    return _ext_declarations;
}

bool Header::validateContains(const ExtDeclarations::access_type& ext_declaration) const
{
    return _ext_declarations.contains(ext_declaration.getName());
}

void Header::notifyChangedMapContent(utility::TypeAccessMapEventCode code,
                                     ExtDeclarations::access_type&,
                                     const std::string& additional_info)
{
    using namespace utility;
    if (code == map_item_added) {
        datamodel::ModelSubject<Header>::notifyChanged(
            ModelEventCode::subitem_added, *this, additional_info);
    }
    else if (code == map_item_removed) {
        datamodel::ModelSubject<Header>::notifyChanged(
            ModelEventCode::subitem_removed, *this, additional_info);
    }
    else if (code == map_item_changed) {
        datamodel::ModelSubject<Header>::notifyChanged(
            ModelEventCode::subitem_changed, *this, additional_info);
    }
    else if (code == map_item_renamed) {
        datamodel::ModelSubject<Header>::notifyChanged(
            ModelEventCode::subitem_renamed, *this, additional_info);
    }
}

bool Header::operator==(const Header& other) const
{
    return getLanguageVersion() == other.getLanguageVersion() && getAuthor() == other.getAuthor() &&
           getDateChange() == other.getDateChange() &&
           getDateCreation() == other.getDateCreation() &&
           getDescription() == other.getDescription() &&
           _ext_declarations.operator==(other._ext_declarations);
}

bool Header::operator!=(const Header& other) const
{
    return !operator==(other);
}

} // namespace datamodel
} // namespace dd
} // namespace ddl
