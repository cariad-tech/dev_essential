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

#ifndef OO_ALIGNMENT_CALCULATION_H_INCLUDED
#define OO_ALIGNMENT_CALCULATION_H_INCLUDED

#include <ddl/dd/dd.h>

namespace ddl {

namespace dd {

size_t obtainElementsAlignment(const DataType& data_type,
                               OptionalSize currentElementAlignment) noexcept;
size_t obtainElementsAlignment(const EnumType& enum_type,
                               const DataDefinition& source_dd,
                               OptionalSize currentElementAlignment) noexcept;
size_t obtainElementsAlignment(const StructType& struct_type,
                               OptionalSize currentElementAlignment) noexcept;

} // namespace dd
} // namespace ddl

#endif // OO_ALIGNMENT_CALCULATION_H_INCLUDED
