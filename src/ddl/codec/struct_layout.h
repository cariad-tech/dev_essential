/**
 * @file
 * Implementation of the ADTF default media description.
 *
 * Copyright @ 2021 VW Group. All rights reserved.
 *
 *     This Source Code Form is subject to the terms of the Mozilla
 *     Public License, v. 2.0. If a copy of the MPL was not distributed
 *     with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * If it is not possible or desirable to put the notice in a particular file, then
 * You may include the notice in a location (such as a LICENSE file in a
 * relevant directory) where a recipient would be likely to look for such a notice.
 *
 * You may add additional accurate notices of copyright ownership.
 */

#ifndef DDL_STRUCT_LAYOUT_CLASS_HEADER
#define DDL_STRUCT_LAYOUT_CLASS_HEADER

#include "ddl/codec/struct_element.h"
#include "ddl/dd/dd_struct_access.h"

namespace ddl {
class DDLComplex;

/**
 * @internal
 * This class is for internal use only.
 */
class StructLayout {
public:
    StructLayout();
    StructLayout(const dd::StructTypeAccess& ddl_struct_access);

    a_util::result::Result isValid() const
    {
        return _calculations_result;
    }

    const std::vector<StructLayoutElement>& getStaticElements() const
    {
        return _static_elements;
    }

    const std::vector<DynamicStructLayoutElement>& getDynamicElements() const
    {
        return _dynamic_elements;
    }

    bool hasDynamicElements() const
    {
        return !_dynamic_elements.empty();
    }

    bool hasEnums() const
    {
        return !_enums.empty();
    }

    const Offsets& getStaticBufferBitSizes() const
    {
        return _static_buffer_sizes;
    }

    size_t getStaticBufferSize(DataRepresentation rep) const;

private:
    a_util::result::Result calculate(const dd::StructTypeAccess& ddl_struct_access);

private:
    std::vector<StructLayoutElement> _static_elements;
    std::vector<DynamicStructLayoutElement> _dynamic_elements;
    std::map<std::string, AccessEnumType> _enums;
    Offsets _static_buffer_sizes;
    a_util::result::Result _calculations_result;
};

} // namespace ddl

#endif
