/**
 * @file
 * OO DataDefinition info model types
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

#ifndef DD_INFOMODEL_TYPE_H_INCLUDED
#define DD_INFOMODEL_TYPE_H_INCLUDED

#include <stdint.h>

namespace ddl {

namespace dd {

/**
 * @brief The Info model type defines the kind of infomodel and interface.
 * @see @ref datamodel::Info, @ref datamodel::InfoMap
 */
enum InfoType : uint8_t {
    /**
     * @brief Validation Info
     * @see @ref ValidationInfo
     */
    validation_info,
    /**
     * @brief Validation Info Service
     * @see @ref ValidationServiceInfo
     */
    validation_service_info,
    /**
     * @brief Type Info
     * @see TypeInfo
     */
    type_info,
    /**
     * @brief Element Type Info
     * @see ElementTypeInfo
     */
    element_type_info,
    /**
     * @brief for customer info (use a offset to implement own infos if necessary)
     */
    last_info
};

} // namespace dd
} // namespace ddl

#endif // DD_INFOMODEL_TYPE_H_INCLUDED