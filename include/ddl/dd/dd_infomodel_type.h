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
     * @brief internal type
     */
    named_container_info = element_type_info,
    /**
     * @brief for customer info (use a offset to implement own infos if necessary)
     */
    last_info
};

/**
 * @brief Validation level
 */
enum class ValidationLevel : uint8_t {
    /**
     * @brief The validation is not known. This means it is not calculated yet. Use i.e. @ref
     * ddl::dd::DataDefinition::validate
     */
    dont_know,
    /**
     * @brief the validation level is invalid, dependencies can not be obtained or the
     * dependency "to" is invalid (struct to struct i.e.).
     *
     */
    invalid,
    /**
     * @brief Structs and elements can be "good_enough" for the TypeInfo.
     * This means the Coder and Decoder can calculate the sizes and the all access Functions can
     * obtain values. Usually this should be the valid state. The problem is, that there are
     * DataDefinition out there (in the users world) using i.e. units, but does not define the
     * unit within that DD. At this point we do not want to break that code! So good enough
     * means: Not well defined in some cases, but good enough for calculate byte positions and
     * @ref TypeInfo.
     *
     * Only structs, enums and datatypes may have a validationlevel of "good_enough" if there
     * are any other DD Objects used in a @ref DataDefinition and they are "invalid", the DD
     * will also set to "invalid"!
     */
    good_enough,
    /**
     * @brief every thing is well defined.
     */
    valid
};

} // namespace dd
} // namespace ddl

#endif // DD_INFOMODEL_TYPE_H_INCLUDED