/**
 * @file
 * Implementation of the private Element Accessor.
 *
 * Copyright @ 2021 VW Group. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "element_accessor.h"

#include <a_util/memory.h>
#include <a_util/result/error_def.h>
#include <ddl/codec/bitserializer.h>
#include <ddl/legacy_error_macros.h>

#include <assert.h>

namespace ddl {
namespace codec {
// define all needed error types and values locally
_MAKE_RESULT(-5, ERR_INVALID_ARG);
_MAKE_RESULT(-19, ERR_NOT_SUPPORTED);

template <typename T>
a_util::result::Result get_typed_element_value(const codec::ElementLayout& element,
                                               const void* data,
                                               size_t data_size,
                                               a_util::variant::Variant& value,
                                               const ElementAccessor& accessor)
{
    T temp_value;
    RETURN_IF_FAILED(accessor.getValue(element, data, data_size, &temp_value, sizeof(T)));
    value = temp_value;
    return a_util::result::SUCCESS;
}

#define GET_CASE_TYPE(__variant_type, __data_type)                                                 \
    case ddl::codec::ElementType::__variant_type: {                                                \
        return get_typed_element_value<__data_type>(element, data, data_size, value, *this);       \
    }

template <typename T>
a_util::result::Result set_typed_element_value(const codec::ElementLayout& element,
                                               void* data,
                                               size_t data_size,
                                               const a_util::variant::Variant& value,
                                               const ElementAccessor& accessor)
{
    T temp_value = value;
    return accessor.setValue(element, data, data_size, &temp_value);
}

#define SET_CASE_TYPE(__variant_type, __data_type)                                                 \
    case ddl::codec::ElementType::__variant_type: {                                                \
        return set_typed_element_value<__data_type>(element, data, data_size, value, *this);       \
    }

a_util::result::Result ElementAccessor::getValue(const codec::ElementLayout& element,
                                                 const void* data,
                                                 size_t data_size,
                                                 a_util::variant::Variant& value) const
{
    switch (element.type_info->getType()) {
        GET_CASE_TYPE(cet_bool, bool)
        GET_CASE_TYPE(cet_int8, int8_t)
        GET_CASE_TYPE(cet_uint8, uint8_t)
        GET_CASE_TYPE(cet_int16, int16_t)
        GET_CASE_TYPE(cet_uint16, uint16_t)
        GET_CASE_TYPE(cet_int32, int32_t)
        GET_CASE_TYPE(cet_uint32, uint32_t)
        GET_CASE_TYPE(cet_int64, int64_t)
        GET_CASE_TYPE(cet_uint64, uint64_t)
        GET_CASE_TYPE(cet_float, float)
        GET_CASE_TYPE(cet_double, double)
    default:
        return ERR_NOT_SUPPORTED;
    }
}

a_util::result::Result ElementAccessor::setValue(const codec::ElementLayout& element,
                                                 void* data,
                                                 size_t data_size,
                                                 const a_util::variant::Variant& value) const
{
    switch (element.type_info->getType()) {
        SET_CASE_TYPE(cet_bool, bool)
        SET_CASE_TYPE(cet_int8, int8_t)
        SET_CASE_TYPE(cet_uint8, uint8_t)
        SET_CASE_TYPE(cet_int16, int16_t)
        SET_CASE_TYPE(cet_uint16, uint16_t)
        SET_CASE_TYPE(cet_int32, int32_t)
        SET_CASE_TYPE(cet_uint32, uint32_t)
        SET_CASE_TYPE(cet_int64, int64_t)
        SET_CASE_TYPE(cet_uint64, uint64_t)
        SET_CASE_TYPE(cet_float, float)
        SET_CASE_TYPE(cet_double, double)
    default:
        return ERR_NOT_SUPPORTED;
    }
}

a_util::result::Result DeserializedAccessor::getValue(const codec::ElementLayout& element,
                                                      const void* data,
                                                      size_t data_size,
                                                      void* element_value,
                                                      size_t element_value_size) const
{
    assert(element.deserialized.bit_offset % 8 == 0);
    assert(element.deserialized.type_bit_size % 8 == 0);
    const size_t byte_offset = element.deserialized.bit_offset / 8;
    const size_t byte_size = element.deserialized.type_bit_size / 8;
    if (data_size < byte_offset + byte_size) {
        RETURN_ERROR_DESCRIPTION(ERR_INVALID_ARG,
                                 "The element size is behind the codecs buffer size");
    }
    // this is to not corrupt memory
    // if element_value_size is 0 its unsafe
    if (element_value_size != 0 && element_value_size < byte_size) {
        RETURN_ERROR_DESCRIPTION(ERR_INVALID_ARG,
                                 "The element size is greater then the given buffer size %zu < %zu",
                                 byte_size,
                                 element_value_size);
    }
    a_util::memory::copy(
        element_value, byte_size, static_cast<const uint8_t*>(data) + byte_offset, byte_size);
    return a_util::result::SUCCESS;
}

a_util::result::Result DeserializedAccessor::setValue(const codec::ElementLayout& element,
                                                      void* data,
                                                      size_t data_size,
                                                      const void* element_value) const
{
    assert(element.deserialized.bit_offset % 8 == 0);
    assert(element.deserialized.type_bit_size % 8 == 0);
    const size_t byte_offset = element.deserialized.bit_offset / 8;
    const size_t byte_size = element.deserialized.type_bit_size / 8;
    if (data_size < byte_offset + byte_size) {
        return ERR_INVALID_ARG;
    }
    a_util::memory::copy(
        static_cast<uint8_t*>(data) + byte_offset, byte_size, element_value, byte_size);
    return a_util::result::SUCCESS;
}

const ElementAccessor& DeserializedAccessor::getInstance()
{
    static DeserializedAccessor oInstance;
    return oInstance;
}

const ElementAccessor& SerializedAccessor::getInstance()
{
    static SerializedAccessor oInstance;
    return oInstance;
}

template <typename T>
a_util::result::Result read_typed_bits(const codec::ElementLayout& element,
                                       const void* data,
                                       size_t data_size,
                                       void* element_value,
                                       size_t element_value_size)
{
    // if 0 ... unsafe usage
    if (element_value_size != 0) {
        if (element_value_size < sizeof(T)) {
            return ERR_INVALID_ARG;
        }
    }
    a_util::memory::BitSerializer helper(const_cast<void*>(data), data_size);
    return helper.read<T>(element.serialized.bit_offset,
                          element.serialized.type_bit_size_used,
                          static_cast<T*>(element_value),
                          static_cast<a_util::memory::Endianess>(element.byte_order));
}

#define GET_CASE_TYPE_SER(__variant_type, __data_type)                                             \
    case ddl::codec::ElementType::__variant_type: {                                                \
        return read_typed_bits<__data_type>(                                                       \
            element, data, data_size, element_value, element_value_size);                          \
    }

a_util::result::Result SerializedAccessor::getValue(const codec::ElementLayout& element,
                                                    const void* data,
                                                    size_t data_size,
                                                    void* element_value,
                                                    size_t element_value_size) const
{
    switch (element.type_info->getType()) {
        GET_CASE_TYPE_SER(cet_bool, bool)
        GET_CASE_TYPE_SER(cet_int8, int8_t)
        GET_CASE_TYPE_SER(cet_uint8, uint8_t)
        GET_CASE_TYPE_SER(cet_int16, int16_t)
        GET_CASE_TYPE_SER(cet_uint16, uint16_t)
        GET_CASE_TYPE_SER(cet_int32, int32_t)
        GET_CASE_TYPE_SER(cet_uint32, uint32_t)
        GET_CASE_TYPE_SER(cet_int64, int64_t)
        GET_CASE_TYPE_SER(cet_uint64, uint64_t)
        GET_CASE_TYPE_SER(cet_float, float)
        GET_CASE_TYPE_SER(cet_double, double)
    default:
        return ERR_NOT_SUPPORTED;
    }
}

template <typename T>
a_util::result::Result write_typed_bits(const codec::ElementLayout& element,
                                        void* data,
                                        size_t data_size,
                                        const void* element_value)
{
    a_util::memory::BitSerializer helper(data, data_size);

    return helper.write<T>(element.serialized.bit_offset,
                           element.serialized.type_bit_size_used,
                           *static_cast<const T*>(element_value),
                           static_cast<a_util::memory::Endianess>(element.byte_order));
}

#define SET_CASE_TYPE_SER(__variant_type, __data_type)                                             \
    case ddl::codec::ElementType::__variant_type: {                                                \
        return write_typed_bits<__data_type>(element, data, data_size, element_value);             \
    }

a_util::result::Result SerializedAccessor::setValue(const codec::ElementLayout& element,
                                                    void* data,
                                                    size_t data_size,
                                                    const void* element_value) const
{
    switch (element.type_info->getType()) {
        SET_CASE_TYPE_SER(cet_bool, bool)
        SET_CASE_TYPE_SER(cet_int8, int8_t)
        SET_CASE_TYPE_SER(cet_uint8, uint8_t)
        SET_CASE_TYPE_SER(cet_int16, int16_t)
        SET_CASE_TYPE_SER(cet_uint16, uint16_t)
        SET_CASE_TYPE_SER(cet_int32, int32_t)
        SET_CASE_TYPE_SER(cet_uint32, uint32_t)
        SET_CASE_TYPE_SER(cet_int64, int64_t)
        SET_CASE_TYPE_SER(cet_uint64, uint64_t)
        SET_CASE_TYPE_SER(cet_float, float)
        SET_CASE_TYPE_SER(cet_double, double)
    default:
        return ERR_NOT_SUPPORTED;
    }
}

} // namespace codec
} // namespace ddl
