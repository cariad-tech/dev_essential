/**
 * @file
 * OO DataDefinition Redesign - Data model Observer - Subject pattern.
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

#ifndef DD_DATA_MODEL_BASE_H_INCLUDED
#define DD_DATA_MODEL_BASE_H_INCLUDED

#include <ddl/dd/dd_error.h>
#include <ddl/utilities/dd_access_observer.h>

#include <string>
#include <unordered_map>

namespace ddl {

namespace dd {

namespace datamodel {

/**
 * @brief Model event code for the data model observer.
 *
 */
enum ModelEventCode {
    /**
     * @brief any content of the item was changed.
     *
     * \n Possible items
     * \li header
     * \li datatype
     * \li enumtype
     * \li structtype
     * \li streammetatype
     * \li stream
     *
     * The event has a additional parameter "additional_info":
     * This is set to the name of attribute changed. Following rule (camelCase to snake_case):
     * \li if the function setSymbol() was call this is set to "symbol"
     * \li if the function setStructVersion() was called this is set to "struct_version"
     */
    item_changed,
    /**
     * @brief the item was removed.
     * \n Possible items
     * \li header
     * \li datatype
     * \li enumtype
     * \li structtype
     * \li streammetatype
     * \li stream
     */
    item_removed,
    /**
     * @brief the item was added.
     * \n Possible items
     * \li header
     * \li datatype
     * \li enumtype
     * \li structtype
     * \li streammetatype
     * \li stream
     */
    item_added,
    /**
     * @brief the item was renamed.
     * \n Possible items
     * \li header
     * \li datatype
     * \li enumtype
     * \li structtype
     * \li streammetatype
     * \li stream
     *
     * The event has a additional parameter "additional_info":
     * This is set to the old name of item that was changed.
     */
    item_renamed,
    /**
     * @brief the sub item was added
     * \li for struct -> element
     * \li for header -> ext declaration
     * \li for enum -> element
     * \li for streammetatype -> property
     * \li stream -> streamstruct
     */
    subitem_added,
    /**
     * @brief the sub item was removed
     * \li for struct -> element at the end
     * \li for header -> ext declaration
     * \li for enum -> element
     * \li for streammetatype -> property
     * \li stream -> streamstruct
     */
    subitem_removed,
    /**
     * @brief the sub item was changed
     * \li for struct -> element
     * \li for header -> ext declaration
     * \li for enum -> element
     * \li for streammetatype -> property
     * \li stream -> streamstruct
     *
     * The event has a additional parameter "additional_info":
     * This is set to the name of attribute changed. Following rule (camelCase to snake_case):
     * \li if the function setSymbol() was call this is set to "symbol"
     * \li if the function setType() was called this is set to "type"
     * \li if the function setArraySize() was called this is set to "array_size"
     */
    subitem_changed,
    /**
     * @brief the sub item was renamed
     * \li for struct -> element
     * \li for header -> ext declaration
     * \li for enum -> element
     * \li for streammetatype -> property
     * \li stream -> streamstruct
     *
     * The event has a additional parameter "additional_info":
     * This is set to the old name of item that was changed.
     */
    subitem_renamed,
    /**
     * @brief the last item was removed.
     * \n Possible items
     * \li structtype
     */
    subitem_popped,
    /**
     * @brief the sub item was inserted
     * \li for struct -> element
     */
    subitem_inserted
};

/**
 * @brief Subject template class that is observable.
 *
 * @tparam TYPE DataDefinition object type that is the subject to observe.
 */
template <typename TYPE>
using ModelSubject = utility::ModelSubjectUtility<TYPE, ModelEventCode>;

/**
 * @brief Observer template class that observe the \p TYPE
 *
 * @tparam TYPE DataDefinition object type that is the subject to observe.
 */
template <typename TYPE>
using ModelObserver = utility::ModelObserverUtility<TYPE, ModelEventCode>;

} // namespace datamodel
} // namespace dd
} // namespace ddl

#endif // DD_DATA_MODEL_BASE_H_INCLUDED
