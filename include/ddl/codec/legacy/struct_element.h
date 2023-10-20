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

} // namespace ddl

#endif // DDL_STRUCT_ELEMENT_LEGACY_CLASS_HEADER
