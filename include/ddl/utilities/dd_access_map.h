/**
 * @file
 * DataDefinition Utility class for observable items
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

#ifndef DD_DD_ACCESS_MAP_H_INCLUDED
#define DD_DD_ACCESS_MAP_H_INCLUDED

#include <ddl/dd/dd_error.h>
#include <ddl/utilities/dd_access_observer.h>

#include <memory>
#include <unordered_map>

namespace ddl {
namespace dd {
namespace utility {

/**
 * @brief Internal event code to inform the parent DD Object about the change of an item within the
 * list.
 * @remark for internal use only. The user API will use @ref ddl::dd::datamodel::ModelEventCode
 * only.
 *
 */
enum TypeAccessMapEventCode {
    map_item_added,
    map_item_removed,
    map_item_changed,
    map_item_renamed,
    map_subitem_added,
    map_subitem_removed,
    map_subitem_changed,
    map_subitem_renamed,
    map_subitem_popped,
    map_subitem_inserted
};

/**
 * @brief helper template to observe map content.
 * @remark for internal use only.
 *
 * @tparam T the subject class to observe
 */
template <typename T>
using TypeAccessMapSubject = ModelSubjectUtility<T, TypeAccessMapEventCode>;

/**
 * @brief helper template to observe map content.
 * @remark for internal use only.
 *
 * @tparam T the subject class to observe
 */
template <typename T>
using TypeAccessMapObserver = ModelObserverUtility<T, TypeAccessMapEventCode>;

/**
 * @brief Utility class for observable named items where the order is NOT important
 *
 * @tparam DDL_TYPE_TO_ACCESS the value type
 * @tparam TYPE_VALIDATOR_CLASS the validator class to inform on changes
 */
template <typename DDL_TYPE_TO_ACCESS, typename TYPE_VALIDATOR_CLASS>
class TypeAccessMap : public TypeAccessMapObserver<DDL_TYPE_TO_ACCESS> {
public:
    /// local definition of the access type
    typedef DDL_TYPE_TO_ACCESS access_type;
    /// local definition of the value type
    typedef std::shared_ptr<DDL_TYPE_TO_ACCESS> value_type;
    /// local definition of the container type
    typedef std::unordered_map<std::string, value_type> container_type;
    /// local definition of the container iterator
    typedef typename container_type::iterator iterator;
    /// local definition of the container const_iterator
    typedef typename container_type::const_iterator const_iterator;
    /// local definition of the parent type to register observe
    typedef TypeAccessMapObserver<DDL_TYPE_TO_ACCESS> parent_type;
    /// local definition of the internal subject type
    typedef TypeAccessMapSubject<DDL_TYPE_TO_ACCESS> map_subject_type;
    /// local definition of the observer type
    typedef typename map_subject_type::observer_type observer_type;
    /// local definition of the eventcode type
    typedef typename parent_type::event_code_type event_code_type;
    /// local definition of the subject type
    typedef typename parent_type::subject_type subject_type;
    /// friend validator class
    friend TYPE_VALIDATOR_CLASS;

public:
    /**
     * @brief no default CTOR!
     *
     */
    TypeAccessMap() = delete;
    /**
     * @brief CTOR
     *
     * @param validator the validator for name check!
     * @param validation_info info for errors
     */
    TypeAccessMap(TYPE_VALIDATOR_CLASS* validator, const std::string& validation_info)
        : _validator(validator), _validation_info(validation_info)
    {
    }
    /**
     * @brief DTOR
     *
     */
    virtual ~TypeAccessMap()
    {
        clear();
    }
    /**
     * @brief move CTOR
     */
    TypeAccessMap(TypeAccessMap&& other)
    {
        *this = std::move(other);
    }
    /**
     * @brief move assignment operator
     *
     * @return TypeAccessMap&
     */
    TypeAccessMap& operator=(TypeAccessMap&& other)
    {
        clear();
        _validation_info = std::move(other._validation_info);
        _types = std::move(other._types);
        // the other will remove itself as observer with clear
        for (const auto& current: _types) {
            // the other will not observe this anymore
            (static_cast<map_subject_type*>(current.second.get()))
                ->detachObserver(static_cast<observer_type*>(&other));
            // i want to observe this
            (static_cast<map_subject_type*>(current.second.get()))
                ->attachObserver(static_cast<observer_type*>(this));
        }
        _validator = nullptr;
        return *this;
    }

    /**
     * @brief copies (deepcopy!) CTOR
     *
     * @param other the other list
     */
    TypeAccessMap(const TypeAccessMap& other)
        : _validator(nullptr), _validation_info(other._validation_info)
    {
        for (auto current: other._types) {
            auto new_type = std::make_shared<DDL_TYPE_TO_ACCESS>(*current.second);
            // i want to observe this
            (static_cast<map_subject_type*>(new_type.get()))
                ->attachObserver(static_cast<observer_type*>(this));
            _types[current.first] = new_type;
        }
    }
    /**
     * @brief copies (deepcopy!) and overwrite the current content.
     *
     * @param other the other list
     * @return TypeAccessList&
     */
    TypeAccessMap& operator=(const TypeAccessMap& other)
    {
        clear();
        _validation_info = other._validation_info;
        for (auto current: other._types) {
            auto new_type = std::make_shared<DDL_TYPE_TO_ACCESS>(*current.second);
            // i want to observe this
            (static_cast<map_subject_type*>(new_type.get()))
                ->attachObserver(static_cast<observer_type*>(this));
            _types[current.first] = new_type;
        }
        _validator = nullptr;
        return *this;
    }

public:
    /**
     * @brief get the item with the given name \p type_name
     *
     * @param type_name the item to find
     * @return std::shared_ptr<const DDL_TYPE_TO_ACCESS>
     */
    std::shared_ptr<const DDL_TYPE_TO_ACCESS> get(const std::string& type_name) const
    {
        auto it_find = _types.find(type_name);
        if (it_find != _types.cend()) {
            std::shared_ptr<const DDL_TYPE_TO_ACCESS> const_return = it_find->second;
            return const_return;
        }
        return {};
    }
    /**
     * @brief determines is the type name exists in this map
     *
     * @param type_name the type name to check
     * @return true if the typename exists
     * @return false the typename does not exist
     */
    bool contains(const std::string& type_name) const
    {
        const auto& it_find = _types.find(type_name);
        if (it_find != _types.cend()) {
            return true;
        }
        return false;
    }
    /**
     * @brief determines if the item is in this map.
     *
     * @param type_to_find the item to find
     * @return true the item is in this map
     * @return false the item is not in this map
     */
    bool contains(const DDL_TYPE_TO_ACCESS& type_to_find) const
    {
        const auto& it_find = _types.find(type_to_find.getName());
        if (it_find != _types.cend()) {
            return type_to_find == *(it_find->second);
        }
        return false;
    }
    /**
     * @brief determines if the \p other is a subset of this map
     *
     * @param other the other map
     * @return true the other map is a subset of this
     * @return false the other is not a subset of this
     */
    bool contains(const TypeAccessMap& other) const
    {
        for (const auto& other_content: other._types) {
            if (!contains(*(other_content.second))) {
                return false;
            }
        }
        return true;
    }
    /**
     * @brief adds the given item
     *
     * @param type_to_add the item to add
     * @throws throws if a item with that name already exists
     */
    void add(const DDL_TYPE_TO_ACCESS& type_to_add)
    {
        const auto type_found = get(type_to_add.getName());
        // have a look if the type already exists here
        if (type_found) {
            // check if they are equal
            if (*type_found != type_to_add) {
                throw ddl::dd::Error(_validation_info + "::add",
                                     {type_to_add.getName()},
                                     "value with the given name already exists");
            }
            return;
        }
        else {
            // has already another type
            bool already_exists = contains(type_to_add.getName());
            if (_validator && !already_exists) {
                already_exists = _validator->validateContains(type_to_add);
            }
            if (already_exists) {
                throw ddl::dd::Error(_validation_info + "::add",
                                     {type_to_add.getName()},
                                     "value with the given name already exists");
            }
        }
        auto new_type_value = std::make_shared<DDL_TYPE_TO_ACCESS>(type_to_add);
        // i want to observe this
        (static_cast<map_subject_type*>(new_type_value.get()))
            ->attachObserver(static_cast<observer_type*>(this));
        _types[type_to_add.getName()] = new_type_value;
        if (_validator) {
            _validator->notifyChangedMapContent(
                TypeAccessMapEventCode::map_item_added, *new_type_value, type_to_add.getName());
        }
    }

    /**
     * @brief emplaces the given item
     *
     * @param type_to_add the item to emplace
     * @throws throws if a item with that name already exists
     */
    void emplace(DDL_TYPE_TO_ACCESS&& type_to_add)
    {
        const auto type_found = get(type_to_add.getName());
        // have a look if the type already exists here
        if (type_found) {
            // check if they are equal
            if (*type_found != type_to_add) {
                throw ddl::dd::Error(_validation_info + "::add",
                                     {type_to_add.getName()},
                                     "value with the given name already exists");
            }
        }
        else {
            // has already another type
            bool already_exists = contains(type_to_add.getName());
            if (_validator && !already_exists) {
                already_exists = _validator->validateContains(type_to_add);
            }
            if (already_exists) {
                throw ddl::dd::Error(_validation_info + "::add",
                                     {type_to_add.getName()},
                                     "value with the given name already exists");
            }
        }
        auto new_type_value = std::make_shared<DDL_TYPE_TO_ACCESS>(std::move(type_to_add));
        // i want to observe this
        (static_cast<map_subject_type*>(new_type_value.get()))
            ->attachObserver(static_cast<observer_type*>(this));
        _types[new_type_value->getName()] = new_type_value;
        if (_validator) {
            _validator->notifyChangedMapContent(
                TypeAccessMapEventCode::map_item_added, *new_type_value, new_type_value->getName());
        }
    }
    /**
     * @brief item to remove
     *
     * @param type_name the item name to remove
     */
    void remove(const std::string& type_name)
    {
        auto removed_value = access(type_name);
        if (removed_value) {
            _types.erase(type_name);
            (static_cast<map_subject_type*>(removed_value.get()))
                ->detachObserver(static_cast<observer_type*>(this));
            if (_validator) {
                _validator->notifyChangedMapContent(
                    TypeAccessMapEventCode::map_item_removed, *removed_value, type_name);
            }
            return;
        }
        throw ddl::dd::Error(
            _validation_info + "::remove", {type_name}, "value with the given name does not exist");
    }
    /**
     * @brief change access to an item
     *
     * @param type_name the item name
     * @return std::shared_ptr<DDL_TYPE_TO_ACCESS>
     */
    std::shared_ptr<DDL_TYPE_TO_ACCESS> access(const std::string& type_name)
    {
        auto it_find = _types.find(type_name);
        if (it_find != _types.end()) {
            return it_find->second;
        }
        return {};
    }
    /**
     * @brief Get the Size
     *
     * @return size_t
     */
    size_t getSize() const
    {
        return _types.size();
    }
    /**
     * @brief the range based begin iterator
     *
     * @return iterator
     */
    iterator begin()
    {
        return _types.begin();
    }
    /**
     * @brief the range based end iterator
     *
     * @return iterator
     */
    iterator end()
    {
        return _types.end();
    }
    /**
     * @brief const begin iterator access
     *
     * @return const_iterator
     */
    const_iterator cbegin() const
    {
        return _types.cbegin();
    }
    /**
     * @brief const end iterator access
     *
     * @return const_iterator
     */
    const_iterator cend() const
    {
        return _types.cend();
    }
    /**
     * @brief range based begin operator for const access
     *
     * @return const_iterator
     */
    const_iterator begin() const
    {
        return cbegin();
    }
    /**
     * @brief range based end operator for const access
     *
     * @return const_iterator
     */
    const_iterator end() const
    {
        return cend();
    }
    /**
     * @brief equality operator.
     *
     * @param other the other TypeAccessMap to compare this map to.
     * @return true  the TypeAccessMap are equal (in size and content)
     * @return false the TypeAccessMap are not equal.
     */
    bool operator==(const TypeAccessMap& other) const
    {
        if (getSize() != other.getSize()) {
            return false;
        }
        else {
            return contains(other);
        }
    }
    /**
     * @brief non equality operator.
     *
     * @param other the other TypeAccessMap to compare this map to.
     * @return false the TypeAccessMap are equal (in size and content)
     * @return true  the TypeAccessMap are not equal.
     */
    bool operator!=(const TypeAccessMap& other) const
    {
        return !operator==(other);
    }
    /**
     * @brief clears the list and remove this as observer
     *
     */
    void clear()
    {
        for (auto& current: _types) {
            // unregister me as observer
            (static_cast<map_subject_type*>(current.second.get()))
                ->detachObserver(static_cast<observer_type*>(this));
        }
        _types.clear();
    }

public:
    /**
     * @brief overrides the observers utility function.
     *
     * @param event_code the internal event_code
     * @param subject_changed the subject (these are the items of the list)
     * @param additional_info the additional info (if any)
     */
    void modelChanged(event_code_type event_code,
                      subject_type& subject_changed,
                      const std::string& additional_info)
    {
        if (event_code == map_item_renamed) {
            bool already_exists = contains(subject_changed.getName());
            if (_validator && !already_exists) {
                already_exists = _validator->validateContains(subject_changed);
            }
            if (already_exists) {
                throw ddl::dd::Error(
                    _validation_info + "::modelChanged",
                    {subject_changed.getName()},
                    "Renaming not possible. Value with the given name already exists");
            }
            else {
                // here additional info is the old name
                iterator current = _types.find(additional_info);
                // we need to readd it
                auto value = current->second;
                _types.erase(current);
                _types[value->getName()] = value;
                // after erasing do not touch the iterator anymore
            }
        }
        if (_validator) {
            // i should discover the shared_ptr here, but not needed at the moment .. so we try to
            // get performance here
            _validator->notifyChangedMapContent(event_code, subject_changed, additional_info);
        }
    }

private:
    /**
     * @brief copies the content of the of the list (by reseting the observer) and resets the
     * validator to the given one.
     *
     *
     * @param destination the destination list where to copy the content of this.
     * @param validator the validator to set in the destination!
     */
    void deepCopy(TypeAccessMap& destination, TYPE_VALIDATOR_CLASS* validator) const
    {
        destination = *this;
        destination._validator = validator;
    }
    container_type _types;
    TYPE_VALIDATOR_CLASS* _validator;
    std::string _validation_info;
};

} // namespace utility
} // namespace dd
} // namespace ddl

#endif // DD_DD_ACCESS_MAP_H_INCLUDED
