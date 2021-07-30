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

#ifndef OO_ALIGNMENT_CALCULATION_H_INCLUDED
#define OO_ALIGNMENT_CALCULATION_H_INCLUDED

#include "ddl/dd/dd.h"

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
