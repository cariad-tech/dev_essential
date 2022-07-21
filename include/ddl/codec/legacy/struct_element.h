/**
 * @file
 * Definition of old StructElement legacy header for old Codec API.
 *
 * @copyright
 * @verbatim
Copyright @ 2022 VW Group. All rights reserved.

    This Source Code Form is subject to the terms of the Mozilla
    Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

If it is not possible or desirable to put the notice in a particular file, then
You may include the notice in a location (such as a LICENSE file in a
relevant directory) where a recipient would be likely to look for such a notice.

You may add additional accurate notices of copyright ownership.
@endverbatim
 */

#ifndef DDL_STRUCT_ELEMENT_LEGACY_CLASS_HEADER
#define DDL_STRUCT_ELEMENT_LEGACY_CLASS_HEADER

#include <a_util/variant.h>

#include <map>

namespace ddl {

/**
 * Typedef for enumerations name -> value.
 */
typedef std::map<std::string, a_util::variant::Variant> AccessEnumType;

/**
 * Information about an element accessible with a decoder or codec.
 */
struct StructElement {
    std::string name;                  ///< The full name of the element.
    a_util::variant::VariantType type; ///< The type of the element.
    const AccessEnumType* p_enum;      ///< pointer to an enum, can be NULL.
};

/** @endcond */

} // namespace ddl

#endif // DDL_STRUCT_ELEMENT_LEGACY_CLASS_HEADER
