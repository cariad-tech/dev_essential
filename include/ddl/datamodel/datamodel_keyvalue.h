/**
 * @file
 * OO DataDefinition Redesign
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
 */

#ifndef DD_DATA_MODEL_KEYVALUE_H_INCLUDED
#define DD_DATA_MODEL_KEYVALUE_H_INCLUDED

#include "ddl/datamodel/infomodel_base.h"
#include "ddl/utilities/dd_access_map.h"

#include <string>

namespace ddl {

namespace dd {

namespace datamodel {

/**
 * @brief observable KeyValuePair
 *
 */
class KeyValuePair : public utility::TypeAccessMapSubject<KeyValuePair>, public InfoMap {
public:
    /**
     * @brief Construct a observable new Key Value Pair
     *
     */
    KeyValuePair() = default;
    /**
     * @brief copy CTOR
     *
     */
    KeyValuePair(const KeyValuePair&) = default;
    /**
     * @brief move CTOR
     *
     */
    KeyValuePair(KeyValuePair&&) = default;
    /**
     * @brief copy assignment operator
     *
     * @return KeyValuePair&
     */
    KeyValuePair& operator=(const KeyValuePair&) = default;
    /**
     * @brief move assignment operator
     *
     * @return KeyValuePair&
     */
    KeyValuePair& operator=(KeyValuePair&&) = default;
    /**
     * @brief CTOR.
     *
     * @param name_key The name or key of the pair.
     * @param value_type The value or type of the pair.
     */
    KeyValuePair(const std::string& name_key, const std::string& value_type);
    /**
     * @brief DTOR
     *
     */
    virtual ~KeyValuePair() = default;

    /**
     * @brief equality operator.
     *
     * @param other the other key valu to compare
     * @return true the key and value are equal.
     * @return false the key and value are not equal.
     */
    bool operator==(const KeyValuePair& other) const;
    /**
     * @brief non-equality operator.
     *
     * @param other the other key valu to compare
     * @return false the key and value are equal.
     * @return true the key and value are not equal.
     */
    bool operator!=(const KeyValuePair& other) const;

    /**
     * @brief Get the Name (same as key)
     *
     * @return const std::string&
     */
    const std::string& getName() const;
    /**
     * @brief Sets the Name (same as key)
     *
     * @param name the name to set
     * @remark This change is observable.
     * @throws Each change may throw if a value with this name already exists.
     */
    void setName(const std::string& name);

    /**
     * @brief Get the Key (same as name)
     *
     * @return const std::string&
     */
    const std::string& getKey() const;
    /**
     * @brief Set the Key (same as name)
     *
     * @param key the key
     * @remark This change is observable.
     * @throws Each change may throw if a value with this key already exists.
     */
    void setKey(const std::string& key);

    /**
     * @brief Get the Value (same as type)
     *
     * @return const std::string&
     */
    const std::string& getValue() const;
    /**
     * @brief Set the Value (same as type)
     *
     * @param value the value
     * @remark This change is observable.
     */
    void setValue(const std::string& value);

    /**
     * @brief Get the Type (same as value)
     *
     * @return const std::string&
     */
    const std::string& getType() const;
    /**
     * @brief Set the Type (same as value)
     *
     * @param type the type to set.
     * @remark This change is observable.
     */
    void setType(const std::string& type);

private:
    std::string _name_key;
    std::string _value_type;
};

} // namespace datamodel
} // namespace dd
} // namespace ddl

#endif // DD_DATA_MODEL_KEYVALUE_H_INCLUDED
