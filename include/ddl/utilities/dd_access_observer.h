/**
 * @file
 * DataDefinition Model-Observer pattern utility
 *
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */

#ifndef DD_ACCESS_OBSERVER_INTERFACE_H_INCLUDED
#define DD_ACCESS_OBSERVER_INTERFACE_H_INCLUDED

#include <string>
#include <vector>

namespace ddl {

namespace dd {

namespace utility {

/**
 * @brief The ModelObserver utility template
 *
 * @tparam MODEL_SUBJECT_T the type of the subject to observe
 * @tparam EVENT_CODE_T the event code type
 */
template <typename MODEL_SUBJECT_T, typename EVENT_CODE_T>
class ModelObserverUtility {
public:
    /// local definition of the event code type
    typedef EVENT_CODE_T event_code_type;
    /// local definition of the subject code type
    typedef MODEL_SUBJECT_T subject_type;

protected:
    /**
     * @brief protected DTOR
     *
     */
    virtual ~ModelObserverUtility(){};

public:
    /**
     * @brief interface function to override.
     *
     * @param event_code the event code
     * @param changed_subject the subject changed
     * @param additional_info additional info (see i.e. @ref ddl::dd::datamodel::ModelEventCode)
     */
    virtual void modelChanged(event_code_type event_code,
                              subject_type& changed_subject,
                              const std::string& additional_info) = 0;
};

/**
 * @brief Model Subject utility to define a Model Subject that notifies one or more observers
 *
 * @tparam MODEL_SUBJECT_T the subject type
 * @tparam EVENT_CODE_T the event code type
 */
template <typename MODEL_SUBJECT_T, typename EVENT_CODE_T>
class ModelSubjectUtility {
public:
    /// local definition of the event code type
    typedef EVENT_CODE_T event_code_type;
    /// local definition of the subject type
    typedef MODEL_SUBJECT_T subject_type;
    /// local definition of the observer type to notify
    typedef ModelObserverUtility<MODEL_SUBJECT_T, EVENT_CODE_T> observer_type;
    /**
     * @brief CTOR
     *
     */
    ModelSubjectUtility() = default;
    /**
     * @brief copy CTOR which removes the observers in case of!
     *
     */
    ModelSubjectUtility(const ModelSubjectUtility&)
    {
        // nothing to do
        // copy means we erase the current observers so we do not copy it
    }
    /**
     * @brief copy assignment operator which removes the observers in case of!
     *
     * @return ModelSubjectUtility&
     */
    ModelSubjectUtility& operator=(const ModelSubjectUtility&)
    {
        // nothing to do
        // copy means we erase the current observers so we do not copy it
        return *this;
    }
    /**
     * @brief DTOR
     *
     */
    virtual ~ModelSubjectUtility() = default;

    /**
     * @brief adda a observer to notify.
     *
     * @param observer the observer instance
     */
    void attachObserver(observer_type* observer)
    {
        for (auto& current: _observers) {
            if (current == observer) {
                return;
            }
        }
        _observers.push_back(observer);
    }
    /**
     * @brief remove a observer if in list.
     *
     * @param observer the observer to remove
     */
    void detachObserver(observer_type* observer)
    {
        for (auto current = _observers.begin(); current != _observers.end(); ++current) {
            if (*current == observer) {
                _observers.erase(current);
                return;
            }
        }
    }

protected:
    /**
     * @brief helper utility function to notify the current observers
     *
     * @param event_code the eventcode
     * @param changed_subject the subject changed
     * @param additional_info additional info (see i.e. @ref ddl::dd::datamodel::ModelEventCode)
     */
    void notifyChanged(event_code_type event_code,
                       subject_type& changed_subject,
                       const std::string& additional_info)
    {
        // we take a copy, to prevent invalidation of the iterator
        auto observers_copy = _observers;
        for (auto& current: observers_copy) {
            current->modelChanged(event_code, changed_subject, additional_info);
        }
    }

private:
    std::vector<observer_type*> _observers;
};

} // namespace utility
} // namespace dd
} // namespace ddl

#endif // DD_ACCESS_OBSERVER_INTERFACE_H_INCLUDED
