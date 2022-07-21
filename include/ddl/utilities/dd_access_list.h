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

#ifndef DD_DD_ACCESS_LIST_H_INCLUDED
#define DD_DD_ACCESS_LIST_H_INCLUDED

#include <ddl/dd/dd_common_types.h>
#include <ddl/dd/dd_error.h>
#include <ddl/utilities/dd_access_observer.h>

#include <algorithm>
#include <memory>
#include <unordered_map>
#if defined(__GNUC__) && (__GNUC__ >= 7)
#include <string_view>
#elif defined(_MSC_VER) && (_MSC_VER >= 1920)
#include <string_view>
#endif

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
enum TypeAccessListEventCode {
    list_item_added,
    list_item_removed,
    list_item_changed,
    list_item_renamed,
    list_subitem_added,
    list_subitem_removed,
    list_subitem_changed,
    list_item_popped,
    list_subitem_popped,
    list_item_inserted,
    list_subitem_inserted
};

/**
 * @brief helper template to observe list content.
 * @remark for internal use only.
 *
 * @tparam T the subject class to observe
 */
template <typename T>
using TypeAccessListSubject = ModelSubjectUtility<T, TypeAccessListEventCode>;

/**
 * @brief helper template to observe list content.
 * @remark for internal use only.
 *
 * @tparam T the subject class to observe
 */
template <typename T>
using TypeAccessListObserver = ModelObserverUtility<T, TypeAccessListEventCode>;

/**
 * @brief Utility class for observable named items where the order is important
 *
 * @tparam DDL_TYPE_TO_ACCESS the value type
 * @tparam TYPE_VALIDATOR_CLASS the validator class to inform on changes
 */
template <typename DDL_TYPE_TO_ACCESS, typename TYPE_VALIDATOR_CLASS>
class TypeAccessList : public TypeAccessListObserver<DDL_TYPE_TO_ACCESS> {
public:
    /// local definition of the access type
    typedef DDL_TYPE_TO_ACCESS access_type;
    /// local definition of the value type
    typedef std::shared_ptr<DDL_TYPE_TO_ACCESS> value_type;
    /// local definition of the container type
    typedef std::vector<value_type> container_type;
    /// local definition of the container type
#if defined(__GNUC__) && (__GNUC__ > 7)
    typedef std::unordered_map<std::string_view, value_type> container_named_type;
#elif defined(_MSC_VER) && (_MSC_VER >= 1920)
    typedef std::unordered_map<std::string_view, value_type> container_named_type;
#else
    typedef std::unordered_map<std::string, value_type> container_named_type;
#endif
    /// local definition of the container iterator
    typedef typename container_type::iterator iterator;
    /// local definition of the container const_iterator
    typedef typename container_type::const_iterator const_iterator;
    /// local definition of the parent type to register observer
    typedef TypeAccessListObserver<DDL_TYPE_TO_ACCESS> parent_type;
    /// local definition of the internal subject type
    typedef TypeAccessListSubject<DDL_TYPE_TO_ACCESS> map_subject_type;
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
    TypeAccessList() = delete;
    /**
     * @brief CTOR
     *
     * @param validator the validator for name check!
     * @param validation_info info for errors
     */
    TypeAccessList(TYPE_VALIDATOR_CLASS* validator, const std::string& validation_info)
        : _validator(validator), _validation_info(validation_info)
    {
    }
    /**
     * @brief DTOR
     *
     */
    virtual ~TypeAccessList()
    {
        clear();
    }
    /**
     * @brief move CTOR
     */
    TypeAccessList(TypeAccessList&& other)
    {
        *this = std::move(other);
    }
    /**
     * @brief move assignment operator
     *
     * @return TypeAccessList&
     */
    TypeAccessList& operator=(TypeAccessList&& other)
    {
        clear();
        _validation_info = std::move(other._validation_info);
        _types = std::move(other._types);
        // the other will not observe this anymore after this clear
        for (const auto& current: _types) {
            // the other will not observe this anymore
            (static_cast<map_subject_type*>(current.get()))
                ->detachObserver(static_cast<observer_type*>(&other));
            // i want to observe this
            (static_cast<map_subject_type*>(current.get()))
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
    TypeAccessList(const TypeAccessList& other)
        : _validator(nullptr), _validation_info(other._validation_info)
    {
        for (auto current: other._types) {
            auto new_type = std::make_shared<DDL_TYPE_TO_ACCESS>(*current);
            // i want to observe this
            (static_cast<map_subject_type*>(new_type.get()))
                ->attachObserver(static_cast<observer_type*>(this));
            _types.push_back(new_type);
        }
    }
    /**
     * @brief copies (deepcopy!) and overwrite the current content.
     *
     * @param other the other list
     * @return TypeAccessList&
     */
    TypeAccessList& operator=(const TypeAccessList& other)
    {
        clear();
        _validation_info = other._validation_info;
        for (auto current: other._types) {
            auto new_type = std::make_shared<DDL_TYPE_TO_ACCESS>(*current);
            // i want to observe this
            (static_cast<map_subject_type*>(new_type.get()))
                ->attachObserver(static_cast<observer_type*>(this));
            _types.push_back(new_type);
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
        if (_validator) {
            const container_named_type* named_items = _validator->getNamedItemList();
            const auto found = named_items->find(type_name);
            if (found != named_items->end()) {
                return found->second;
            }
        }
        else {
            for (auto& current: _types) {
                if (current->getName() == type_name) {
                    std::shared_ptr<const DDL_TYPE_TO_ACCESS> const_return = current;
                    return const_return;
                }
            }
        }
        return {};
    }

    /**
     * @brief Get the Pos Of the item with the name \p type_name
     *
     * @param type_name the item to get the pos of
     * @retval valid OptionalSize with the position within the list (0 based)
     * @retval invalid not in list
     */
    OptionalSize getPosOf(const std::string& type_name) const
    {
        size_t current_pos = 0;
        for (auto& current: _types) {
            if (current->getName() == type_name) {
                return current_pos;
            }
            ++current_pos;
        }

        return {};
    }

    /**
     * @brief determines is the type name exists in this list
     *
     * @param type_name the type name to check
     * @return true if the typename exists
     * @return false the typename does not exist
     */
    bool contains(const std::string& type_name) const
    {
        if (_validator) {
            const container_named_type* named_items = _validator->getNamedItemList();
            const auto found = named_items->find(type_name);
            if (found != named_items->end()) {
                return true;
            }
        }
        else {
            for (auto& current: _types) {
                if (current->getName() == type_name) {
                    return true;
                }
            }
        }
        return false;
    }

    /**
     * @brief determines if the item is in this list.
     *
     * @param type_to_find the item to find
     * @return true the item is in this list
     * @return false the item is not in this list
     */
    bool contains(const DDL_TYPE_TO_ACCESS& type_to_find) const
    {
        const auto value_found = get(type_to_find.getName());
        if (value_found) {
            return type_to_find == *value_found;
        }
        return false;
    }

    /**
     * @brief determines if the \p other is a subset of this list
     *
     * @param other the other list
     * @return true the other list is a subset of this
     * @return false the other is not a subset of this
     */
    bool contains(const TypeAccessList& other) const
    {
        for (const auto& other_content: other._types) {
            if (!contains(*other_content)) {
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
        if (checkExistenceAndEquality(type_to_add)) {
            // this will immediatelly return if the type exists and is equal
            // otherwise the check will throw!
            return;
        }

        auto new_type_value = std::make_shared<DDL_TYPE_TO_ACCESS>(type_to_add);
        // i want to observe this
        (static_cast<map_subject_type*>(new_type_value.get()))
            ->attachObserver(static_cast<observer_type*>(this));
        _types.push_back(new_type_value);
        if (_validator) {
            container_named_type* named_items = _validator->getNamedItemList();
            (*named_items)[new_type_value->getName()] = new_type_value;
        }
        if (_validator) {
            _validator->notifyChangedListContent(
                TypeAccessListEventCode::list_item_added, *new_type_value, type_to_add.getName());
        }
    }

    /**
     * @brief inserts the given item at the given pos
     *
     * @param type_to_add the item to insert
     * @param pos_idx the pos where to insert the item
     * @throws throws if a item with that name already exists, or the \c pos_idx is invalid or out
     * of range
     */
    void insert(const size_t pos_idx, const DDL_TYPE_TO_ACCESS& type_to_add)
    {
        if (pos_idx > getSize()) {
            throw ddl::dd::Error(
                _validation_info + "::insert", {type_to_add.getName()}, "given pos_idx is invalid");
        }
        else if (pos_idx == getSize()) {
            add(type_to_add);
        }
        else {
            if (checkExistenceAndEquality(type_to_add)) {
                // this will immediatelly return if the type exists and is equal
                // otherwise the check will throw!
                return;
            }
            auto new_type_value = std::make_shared<DDL_TYPE_TO_ACCESS>(type_to_add);
            // i want to observe this
            (static_cast<map_subject_type*>(new_type_value.get()))
                ->attachObserver(static_cast<observer_type*>(this));

            auto cit = cbegin();
            std::advance(cit, pos_idx);
            _types.insert(cit, new_type_value);
            if (_validator) {
                container_named_type* named_items = _validator->getNamedItemList();
                (*named_items)[new_type_value->getName()] = new_type_value;
            }
            if (_validator) {
                _validator->notifyChangedListContent(TypeAccessListEventCode::list_item_inserted,
                                                     *new_type_value,
                                                     type_to_add.getName());
            }
        }
    }

    /**
     * @brief emplace the given item
     *
     * @param type_to_add the item to emplace
     * @throws throws if a item with that name already exists
     */
    void emplace(DDL_TYPE_TO_ACCESS&& type_to_add)
    {
        if (checkExistenceAndEquality(type_to_add)) {
            // this will immediatelly return if the type exists and is equal
            // otherwise the check will throw!
            return;
        }
        auto new_type_value = std::make_shared<DDL_TYPE_TO_ACCESS>(std::move(type_to_add));
        // i want to observe this
        (static_cast<map_subject_type*>(new_type_value.get()))
            ->attachObserver(static_cast<observer_type*>(this));
        _types.push_back(new_type_value);
        if (_validator) {
            container_named_type* named_items = _validator->getNamedItemList();
            (*named_items)[new_type_value->getName()] = new_type_value;
        }
        if (_validator) {
            _validator->notifyChangedListContent(TypeAccessListEventCode::list_item_added,
                                                 *new_type_value,
                                                 new_type_value->getName());
        }
    }

    /**
     * @brief item to remove
     *
     * @param type_name the item name to remove
     */
    void remove(const std::string& type_name)
    {
        value_type removed_value;
        typename container_type::iterator current_it;
        for (current_it = _types.begin(); current_it != _types.end(); ++current_it) {
            if ((*current_it)->getName() == type_name) {
                removed_value = (*current_it);
                break;
            }
        }
        if (removed_value) {
            _types.erase(current_it);
            if (_validator) {
                container_named_type* named_items = _validator->getNamedItemList();
                named_items->erase(type_name);
            }
            (static_cast<subject_type*>(removed_value.get()))
                ->detachObserver(static_cast<observer_type*>(this));
            if (_validator) {
                _validator->notifyChangedListContent(
                    TypeAccessListEventCode::list_item_removed, *removed_value, type_name);
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
        if (_validator) {
            container_named_type* named_items = _validator->getNamedItemList();
            auto value_found = named_items->find(type_name);
            if (value_found != named_items->end()) {
                return value_found->second;
            }
        }
        else {
            for (const auto& current: _types) {
                if (current->getName() == type_name) {
                    return current;
                }
            }
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
     * @param other the other TypeAccessList to compare this list to.
     * @return true  the TypeAccessList are equal (in size and content)
     * @return false the TypeAccessList are not equal.
     */
    bool operator==(const TypeAccessList& other) const
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
     * @param other the other TypeAccessList to compare this list to.
     * @return false the TypeAccessList are equal (in size and content)
     * @return true the TypeAccessList are not equal.
     */
    bool operator!=(const TypeAccessList& other) const
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
            (static_cast<subject_type*>(current.get()))
                ->detachObserver(static_cast<observer_type*>(this));
        }
        if (_validator) {
            container_named_type* named_items = _validator->getNamedItemList();
            named_items->clear();
        }
        _types.clear();
    }

    /**
     * @brief removes the last element if exists.
     * @throw ddl::dd::Error if it is empty.
     */
    void popBack()
    {
        if (!_types.empty()) {
            auto last_element = _types[_types.size() - 1];
            // unregister me as observer
            (static_cast<subject_type*>(last_element.get()))
                ->detachObserver(static_cast<observer_type*>(this));
            // pop last element
            if (_validator) {
                container_named_type* named_items = _validator->getNamedItemList();
                named_items->erase(last_element->getName());
            }
            _types.pop_back();
            if (_validator) {
                _validator->notifyChangedListContent(TypeAccessListEventCode::list_item_popped,
                                                     *last_element,
                                                     last_element->getName());
            }
        }
        else {
            throw ddl::dd::Error(_validation_info + "::popBack", {}, "list is empty");
        }
    }

    /**
     * @brief removes the last element if exists.
     * @throw ddl::dd::Error if it is empty.
     */
    void popBack()
    {
        if (!_types.empty()) {
            auto last_element = _types[_types.size() - 1];
            // unregister me as observer
            (static_cast<subject_type*>(last_element.get()))
                ->detachObserver(static_cast<observer_type*>(this));
            // pop last element
            _types.pop_back();
            if (_validator) {
                _validator->notifyChangedListContent(TypeAccessListEventCode::list_item_removed,
                                                     *last_element,
                                                     last_element->getName());
            }
        }
        else {
            throw ddl::dd::Error(_validation_info + "::popBack", {}, "list is empty");
        }
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
        if (event_code == list_item_renamed) {
            bool already_exists = false;
            if (_validator) {
                already_exists = _validator->validateContains(subject_changed);
            }
            else {
                already_exists = countContains(subject_changed.getName()) > 1;
            }
            if (already_exists) {
                throw ddl::dd::Error(
                    _validation_info + "::modelChanged",
                    {subject_changed.getName()},
                    "Renaming not possible. Value with the given name already exists");
            }
            else {
                if (_validator) {
                    container_named_type* named_items = _validator->getNamedItemList();
                    typename container_named_type::iterator found_in_names = named_items->begin();
                    for (; found_in_names != named_items->end(); ++found_in_names) {
                        // we need to search it to reset the stringview
                        if (found_in_names->second->getName() == subject_changed.getName()) {
                            auto old_value = found_in_names->second;
                            named_items->erase(found_in_names);
                            (*named_items)[old_value->getName()] = old_value;
                            break;
                        }
                    }
                }
            }
        }
        if (_validator) {
            // i should discover the shared_ptr here, but not needed at the moment
            _validator->notifyChangedListContent(event_code, subject_changed, additional_info);
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
    void deepCopy(TypeAccessList& destination, TYPE_VALIDATOR_CLASS* validator) const
    {
        destination = *this;
        destination.setValidator(validator);
    }
    /**
     * @brief determines the count of the type name exists in this list
     *
     * @param type_name the type name to check
     * @return true if the typename exists
     * @return false the typename does not exist
     */
    size_t countContains(const std::string& type_name) const
    {
        return std::count_if(_types.begin(), _types.end(), [type_name](const value_type& value) {
            return value->getName() == type_name;
        });
    }

    bool checkExistenceAndEquality(const DDL_TYPE_TO_ACCESS& type_to_add)
    {
        // has already another type
        bool already_exists = false;
        if (_validator) {
            already_exists = _validator->validateContains(type_to_add);
        }
        else {
            already_exists = contains(type_to_add.getName());
        }
        if (already_exists) {
            // have a look if the type does exist in this list
            const auto type_found = get(type_to_add.getName());
            // have a look if the type already exists here
            if (type_found) {
                // check if they are equal
                if (*type_found == type_to_add) {
                    // its okay ... its the same type/content
                    return true;
                }
                throw ddl::dd::Error(_validation_info + "::add",
                                     {type_to_add.getName()},
                                     "value with the given name already exists");
            }
            else {
                throw ddl::dd::Error(_validation_info + "::add",
                                     {type_to_add.getName()},
                                     "value with the given name already exists");
            }
        }
        return false;
    }

    void setValidator(TYPE_VALIDATOR_CLASS* validator)
    {
        _validator = validator;
        if (_validator) {
            container_named_type* named_items = _validator->getNamedItemList();
            named_items->clear();
            for (auto& value: _types) {
                (*named_items)[value->getName()] = value;
            }
        }
    }

    container_type _types;
    TYPE_VALIDATOR_CLASS* _validator = {};
    std::string _validation_info;
};

} // namespace utility
} // namespace dd
} // namespace ddl

#endif // DD_DD_ACCESS_LIST_H_INCLUDED
