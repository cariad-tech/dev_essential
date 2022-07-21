/**
 * @file
 * DDDefault header
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

#ifndef DDDEFAULT_H_INCLUDED
#define DDDEFAULT_H_INCLUDED

#include <ddl/dd/dd.h>

namespace ddl {
/**
 * @brief Convenience class to create a valid predefined Data Definition with predefined types.
 * <br> This implementation can be used as follows:
 * @code
 * //create the default data defintion
 * auto my_dd_1 = DDDefault::create();
 *
 * @endcode
 *
 * @see @ref DDDefault::CreateOption
 *
 */
class DDDefault {
public:
    /**
     * CreateOption for @ref create.
     */
    enum class CreateOption {
        /**
         * add all predefined data types to the result of @ref DDDefault::create return value.
         */
        use_all_types = 0,
        /**
         * add only predefined types to the result of @ref DDDefault::create return value that
         * belongs to the C++ standard data types:
         * \li uint8_t, int8_t ...
         * \li bool, char, float, double
         */
        use_std_types_only = 1,
        /**
         * add only predefined types to the result of @ref DDDefault::create return value that
         * belongs to the ddl data types:
         * \li tUInt8, tInt8 ...
         * \li tBool, tChar, tFloat32, tFloat64
         */
        use_ddl_types_only = 2
    };
    /**
     * @brief Creates a default data defintion with predefined types, base units and unit prefixes.
     *
     * @param create_option choose an option to create with all defined types, with standard types
     * only or with ddl types only.
     * @see @ref CreateOption
     *
     * @return dd::DataDefinition the valid Data Definiton of the file.
     */
    static dd::DataDefinition create(CreateOption create_option = CreateOption::use_all_types);
};

} // namespace ddl

#endif // DDDEFAULT_H_INCLUDED
