/**
 * @file
 * OO DataDefinition Redesign
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

#include "dd_alignment_calculation.h"

namespace ddl {

namespace dd {

size_t obtainElementsAlignment(const DataType& data_type,
                               OptionalSize currentElementAlignment) noexcept
{
    if (currentElementAlignment && AlignmentValidation::isValid(*currentElementAlignment)) {
        return *currentElementAlignment;
    }
    else {
        return data_type.getDefaultAlignment();
    }
}

size_t obtainElementsAlignment(const EnumType& enum_type,
                               const DataDefinition& source_dd,
                               OptionalSize currentElementAlignment) noexcept
{
    if (currentElementAlignment && AlignmentValidation::isValid(*currentElementAlignment)) {
        return *currentElementAlignment;
    }
    else {
        const auto dt = source_dd.getDataTypes().get(enum_type.getDataTypeName());
        if (dt) {
            return dt->getDefaultAlignment();
        }
        else {
            return Alignment::e1;
        }
    }
}

size_t obtainElementsAlignment(const StructType& struct_type,
                               OptionalSize currentElementAlignment) noexcept
{
    if (currentElementAlignment && AlignmentValidation::isValid(*currentElementAlignment)) {
        return *currentElementAlignment;
    }
    else {
        return struct_type.getAlignment();
    }
}

} // namespace dd
} // namespace ddl
