/**
 * @file
 * OO DataDefinition Redesign
 *
 * Copyright @ 2021 VW Group. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
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
