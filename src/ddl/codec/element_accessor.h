/**
 * @file
 * Implementation of the private ElementAccessor.
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

#ifndef DDL_PREVIEW_STRUCT_ELEMENT_ACCESS_PRIVATE_CLASS_HEADER
#define DDL_PREVIEW_STRUCT_ELEMENT_ACCESS_PRIVATE_CLASS_HEADER

#include <a_util/result/result_type_decl.h>
#include <ddl/codec/codec_index.h>
#include <ddl/codec/data_representation.h>

namespace ddl {
namespace codec {
class ElementAccessor {
public:
    virtual a_util::result::Result getValue(const codec::ElementLayout& element,
                                            const void* data,
                                            size_t data_size,
                                            void* element_value,
                                            size_t element_value_size) const = 0;
    virtual a_util::result::Result setValue(const codec::ElementLayout& element,
                                            void* data,
                                            size_t data_size,
                                            const void* element_value) const = 0;

    virtual DataRepresentation getRepresentation() const = 0;

    a_util::result::Result getValue(const codec::ElementLayout& element,
                                    const void* data,
                                    size_t data_size,
                                    a_util::variant::Variant& value) const;

    a_util::result::Result setValue(const codec::ElementLayout& element,
                                    void* data,
                                    size_t data_size,
                                    const a_util::variant::Variant& value) const;
};

class DeserializedAccessor : public ElementAccessor {
public:
    virtual a_util::result::Result getValue(const codec::ElementLayout& element,
                                            const void* data,
                                            size_t data_size,
                                            void* element_value,
                                            size_t element_value_size) const override;
    virtual a_util::result::Result setValue(const codec::ElementLayout& element,
                                            void* data,
                                            size_t data_size,
                                            const void* element_value) const override;

    virtual DataRepresentation getRepresentation() const override
    {
        return deserialized;
    }

    static const ElementAccessor& getInstance();
};

class SerializedAccessor : public ElementAccessor {
public:
    virtual a_util::result::Result getValue(const codec::ElementLayout& element,
                                            const void* data,
                                            size_t data_size,
                                            void* element_value,
                                            size_t element_value_size) const override;
    virtual a_util::result::Result setValue(const codec::ElementLayout& element,
                                            void* data,
                                            size_t data_size,
                                            const void* element_value) const override;

    virtual DataRepresentation getRepresentation() const override
    {
        return serialized;
    }

    static const ElementAccessor& getInstance();
};
} // namespace codec
} // namespace ddl

#endif // DDL_PREVIEW_STRUCT_ELEMENT_ACCESS_PRIVATE_CLASS_HEADER
