/**
 * @file
 * Implementation of the ADTF default media description.
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

#ifndef DDL_STRUCT_ELEMENT_CLASS_HEADER
#define DDL_STRUCT_ELEMENT_CLASS_HEADER

#include "a_util/result.h"
#include "a_util/variant.h"
#include "ddl/dd/dd_common_types.h"

#include <map>
#include <string>
#include <vector>

namespace ddl {

#ifndef DDL_DATA_REPRESENTATION_DEFINED
#define DDL_DATA_REPRESENTATION_DEFINED
/**
 * Enumeration for the data representation
 */
enum DataRepresentation {
    serialized,                 ///< serialized data, i.e network, on disks, can msgs, ...
    deserialized,               ///< deserialized data, c-structs, arrays, ...
    Serialized = serialized,    ///< alias names for legacy reasons
    Deserialized = deserialized ///< alias names for legacy reasons
};
/// Typedef provided for compatibility reasons
using tDataRepresentation = DataRepresentation;
#endif

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

// The following classes are for internal use only

/** @cond INTERNAL_DOCUMENTATION */

struct Position {
    size_t bit_offset;
    size_t bit_size;
};

struct StructLayoutElement : public StructElement {
    Position deserialized;
    Position serialized;
    ddl::dd::ByteOrder byte_order;
    const a_util::variant::Variant* constant;
};

struct DynamicStructLayoutElement {
    std::string name;
    size_t alignment;
    std::string size_element_name;
    std::vector<StructLayoutElement> static_elements;
    std::vector<DynamicStructLayoutElement> dynamic_elements;

    DynamicStructLayoutElement() = default;

    DynamicStructLayoutElement(size_t alignment) : alignment(alignment)
    {
    }

    bool isAlignmentElement() const
    {
        return name.empty();
    }

    bool isDynamicArray() const
    {
        return !size_element_name.empty();
    }
};

struct Offsets {
    size_t deserialized;
    size_t serialized;
};

/** @endcond */

} // namespace ddl

#endif
