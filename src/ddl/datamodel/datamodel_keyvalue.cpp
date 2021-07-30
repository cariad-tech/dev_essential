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

#include "ddl/datamodel/datamodel_keyvalue.h"

#include <exception>
#include <utility>

namespace ddl {

namespace dd {

namespace datamodel {

/*************************************************************************************************************/
// KeyValue pair for EnumType Element and Header Ext Declaration
/*************************************************************************************************************/
KeyValuePair::KeyValuePair(const std::string& name_key, const std::string& value_type)
    : _name_key(name_key), _value_type(value_type)
{
}

bool KeyValuePair::operator==(const KeyValuePair& other) const
{
    return (getName() == other.getName() && getValue() == other.getValue());
}

bool KeyValuePair::operator!=(const KeyValuePair& other) const
{
    return !operator==(other);
}

const std::string& KeyValuePair::getName() const
{
    return _name_key;
}

const std::string& KeyValuePair::getKey() const
{
    return _name_key;
}

const std::string& KeyValuePair::getValue() const
{
    return _value_type;
}

void KeyValuePair::setValue(const std::string& value)
{
    _value_type = value;
    notifyChanged({}, *this, "value");
}

const std::string& KeyValuePair::getType() const
{
    return _value_type;
}

void KeyValuePair::setType(const std::string& type)
{
    _value_type = type;
    notifyChanged({}, *this, "type");
}

void KeyValuePair::setName(const std::string& name)
{
    // Keep old name for reset
    std::string old_name = _name_key;
    _name_key = name;
    try {
        notifyChanged(utility::TypeAccessMapEventCode::map_item_renamed, *this, old_name);
    }
    catch (const dd::Error& error) {
        // keep consistent .. this exeption does not invalidate the whole model
        _name_key = old_name;
        throw error;
    }

    notifyChanged({}, *this, "name");
}

void KeyValuePair::setKey(const std::string& key)
{
    // Keep old name for reset
    std::string old_name = _name_key;
    _name_key = key;
    try {
        notifyChanged(utility::TypeAccessMapEventCode::map_item_renamed, *this, old_name);
    }
    catch (const dd::Error& error) {
        // keep consistent .. this exeption does not invalidate the whole model
        _name_key = old_name;
        throw error;
    }

    notifyChanged({}, *this, "key");
}

} // namespace datamodel
} // namespace dd
} // namespace ddl
