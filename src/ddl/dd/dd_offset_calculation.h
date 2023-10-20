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

#ifndef OO_OFFSET_CALCULATION_H_INCLUDED
#define OO_OFFSET_CALCULATION_H_INCLUDED

#include <ddl/datamodel/datamodel_datadefinition.h>

namespace ddl {

namespace dd {

size_t calculateAlignedSize(size_t unaligned_byte_size, size_t alignment);

OptionalSize getSerializedHighestBit(const datamodel::StructType::Element& current);

size_t calculateSerializedBitSize(size_t array_size, size_t type_bit_size, OptionalSize numbits);

struct CalculatedSerializedPos {
    OptionalSize _serialized_byte_pos = {};
    OptionalSize _serialized_bit_pos = {};
    OptionalSize _serialized_bit_size = {};
    OptionalSize _serialized_type_bit_size = {};
    bool _valid = true;
    bool _is_dynamic = false;
    bool _is_after_dynamic = false;
    bool _reset_allowed = true;
};

CalculatedSerializedPos calculateSerializedPosSize(
    const datamodel::StructType::Element& current,
    const std::shared_ptr<datamodel::StructType::Element>& previous,
    datamodel::DataDefinition& ddl,
    datamodel::ElementType& elem_type_return);

struct CalculatedDeserializedPos {
    OptionalSize _deserialized_byte_pos = {};
    OptionalSize _deserialized_byte_size = {};
    size_t _deserialized_type_byte_size = 0;
    size_t _deserialized_type_aligned_byte_size = 0;

    bool _valid = true;
    bool _is_dynamic = false;
    bool _is_after_dynamic = false;
};

CalculatedDeserializedPos calculateDeserializedPosSize(
    const datamodel::StructType::Element& current,
    const std::shared_ptr<datamodel::StructType::Element>& previous,
    const Version& struct_ddl_version,
    datamodel::DataDefinition& ddl,
    datamodel::ElementType& elem_type_return);

} // namespace dd
} // namespace ddl

#endif // OO_OFFSET_CALCULATION_H_INCLUDED
