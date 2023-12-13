/**
 * @file
 * Implementation of the private element setter helper functions.
 *
 * Copyright @ 2022 VW Group. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef DDL_PREVIEW_ELEMENT_SETTER_PRIVATE_CLASS_HEADER
#define DDL_PREVIEW_ELEMENT_SETTER_PRIVATE_CLASS_HEADER

#include <ddl/codec/codec_index.h>

namespace ddl {
namespace codec {

template <typename T, bool no_throw = true>
bool setEnumValue(T& codec, const CodecIndex& codec_index, const std::string& enum_value_name)
{
    const auto& element_layout = codec_index.getLayout();
    if (no_throw) {
        if (!element_layout.type_info->isEnum() ||
            !element_layout.type_info->hasEnumElementName(enum_value_name)) {
            return false;
        }
    }
    codec.setElementVariantValue(codec_index,
                                 element_layout.type_info->getEnumElementValue(enum_value_name));
    return true;
}

} // namespace codec
} // namespace ddl

#endif // DDL_PREVIEW_ELEMENT_SETTER_PRIVATE_CLASS_HEADER
