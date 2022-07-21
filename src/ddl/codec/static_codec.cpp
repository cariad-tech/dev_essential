/**
 * @file
 * Implementation of the StaticCodec and the StaticDecoder.
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

#include "codec_elements.h"
#include "element_accessor.h"
#include "element_setter.h"

#include <a_util/result/error_def.h>
#include <a_util/strings/strings_convert.h>
#include <a_util/strings/strings_numeric_traits.h>
#include <ddl/codec/static_codec.h>
#include <ddl/legacy_error_macros.h>
#include <ddl/utilities/std_to_string.h>

// define all needed error types and values locally
_MAKE_RESULT(-5, ERR_INVALID_ARG);
_MAKE_RESULT(-14, ERR_NOT_INITIALIZED);

namespace ddl {
namespace codec {

StaticDecoder::StaticDecoder()
    : _codec_access(std::make_shared<const StructAccess>()), _first_element(*this)
{
}

StaticDecoder::StaticDecoder(std::shared_ptr<const StructAccess> codec_access,
                             const void* data,
                             size_t data_size,
                             ddl::DataRepresentation representation)
    : _codec_access(codec_access),
      _data(data),
      _data_size(data_size),
      _element_accessor(representation == deserialized ?
                            &ddl::codec::DeserializedAccessor::getInstance() :
                            &ddl::codec::SerializedAccessor::getInstance()),
      _first_element(CodecIndex(), *this)
{
}

StaticDecoder::StaticDecoder(StaticDecoder&& other)
    : _codec_access(std::move(other._codec_access)),
      _data(std::move(other._data)),
      _data_size(std::move(other._data_size)),
      _element_accessor(std::move(other._element_accessor)),
      _first_element(CodecIndex(), *this)
{
}

StaticDecoder& StaticDecoder::operator=(StaticDecoder&& other)
{
    _codec_access = std::move(other._codec_access);
    _data = std::move(other._data);
    _data_size = std::move(other._data_size);
    _element_accessor = std::move(other._element_accessor);
    _first_element.resetIndex(CodecIndex());
    return *this;
}

a_util::result::Result StaticDecoder::isValid() const
{
    if (_codec_access) {
        if (_codec_access->isDynamic()) {
            RETURN_ERROR_DESCRIPTION(ERR_INVALID_ARG,
                                     "StaticDecoder cannot resolve dynamic DataDescription");
        }
        RETURN_IF_FAILED(_codec_access->getInitResult());
        if (_data_size < getStaticBufferSize()) {
            return ERR_INVALID_ARG;
        }
        return {};
    }
    else {
        return ERR_NOT_INITIALIZED;
    }
}

StaticDecoder::Element StaticDecoder::getElement(const std::string& full_name) const
{
    auto index = _codec_access->resolve(full_name);
    return Element(std::move(index), *this);
}

StaticDecoder::Element StaticDecoder::getElement(const CodecIndex& index) const
{
    auto validated_index = index;
    _codec_access->resolve(validated_index, false);
    return Element(std::move(validated_index), *this);
}

const StaticDecoder::Elements& StaticDecoder::getElements() const
{
    return _first_element.getChildElements();
}

size_t StaticDecoder::getElementChildCount(const CodecIndex& codec_index) const
{
    return _codec_access->getElementChildCount(codec_index, false);
}

void StaticDecoder::getElementVariantValue(const CodecIndex& codec_index,
                                           a_util::variant::Variant& value) const
{
    a_util::result::Result result = {};
    if (codec_index.hasLayout()) {
        result = _element_accessor->getValue(codec_index.getLayout(), _data, _data_size, value);
    }
    else {
        ElementLayout element_layout = _codec_access->getCodecElementLayout(codec_index);
        result = _element_accessor->getValue(element_layout, _data, _data_size, value);
    }
    if (isFailed(result)) {
        throw std::runtime_error(result.getDescription());
    }
}

a_util::variant::Variant StaticDecoder::getElementVariantValue(const CodecIndex& codec_index) const
{
    a_util::variant::Variant value;
    getElementVariantValue(codec_index, value);
    return value;
}

#define GET_ENUM_CASE(__type)                                                                      \
    case a_util::variant::VariantType::VT_##__type: {                                              \
        const auto xValue = variant_value.as##__type();                                            \
        try {                                                                                      \
            return element_layout.type_info->getEnumElementName(xValue);                           \
        }                                                                                          \
        catch (...) {                                                                              \
        }                                                                                          \
        break;                                                                                     \
    }

std::string StaticDecoder::getElementStringValue(const CodecIndex& codec_index) const
{
    a_util::variant::Variant variant_value;
    const ElementLayout element_layout = _codec_access->getCodecElementLayout(codec_index);
    auto result = _element_accessor->getValue(element_layout, _data, _data_size, variant_value);
    if (isOk(result)) {
        if (element_layout.type_info->isEnum()) {
            switch (variant_value.getType()) {
                GET_ENUM_CASE(Bool)
                GET_ENUM_CASE(Int8)
                GET_ENUM_CASE(UInt8)
                GET_ENUM_CASE(Int16)
                GET_ENUM_CASE(UInt16)
                GET_ENUM_CASE(Int32)
                GET_ENUM_CASE(UInt32)
                GET_ENUM_CASE(Int64)
                GET_ENUM_CASE(UInt64)
                GET_ENUM_CASE(Float)
                GET_ENUM_CASE(Double)
            default:
                break;
            }
        }
        return variant_value.asString();
    }
    else {
        throw std::runtime_error(result.getDescription());
    }
}

void StaticDecoder::getElementRawValue(const CodecIndex& index,
                                       void* value,
                                       size_t value_size) const
{
    const ElementLayout element_layout = _codec_access->getCodecElementLayout(index);
    auto result = _element_accessor->getValue(element_layout, _data, _data_size, value, value_size);
    if (isFailed(result)) {
        throw std::runtime_error(result.getDescription());
    }
}

namespace {
const void* getAddress(const ElementLayout& element_layout,
                       const ElementAccessor* element_accessor,
                       const void* data,
                       size_t data_size)
{
    const size_t nBitPos = element_accessor->getRepresentation() == deserialized ?
                               element_layout.deserialized.bit_offset :
                               element_layout.serialized.bit_offset;

    if (nBitPos % 8) {
        return nullptr;
    }

    const size_t type_bit_size = element_accessor->getRepresentation() == deserialized ?
                                     element_layout.deserialized.type_bit_size :
                                     element_layout.serialized.type_bit_size;

    if ((nBitPos / 8) + (type_bit_size / 8) + (type_bit_size % 8 ? 1 : 0) > data_size) {
        return nullptr;
    }

    return static_cast<const uint8_t*>(data) + (nBitPos / 8);
}
} // namespace

const void* StaticDecoder::getElementAddress(const CodecIndex& index) const
{
    try {
        ElementLayout element_layout = _codec_access->getCodecElementLayout(index);
        return getAddress(element_layout, _element_accessor, _data, _data_size);
    }
    catch (const std::exception&) {
        // for legacy reason we do not throw here
        return nullptr;
    }
}

std::string StaticDecoder::getElementFullName(const CodecIndex& index) const
{
    std::string full_name;
    _codec_access->getCodecElementLayout(index, full_name);
    return full_name;
}

std::string StaticDecoder::getElementName(const CodecIndex& index) const
{
    return _codec_access->getName(index);
}

const std::string& StaticDecoder::getElementBaseName(const CodecIndex& index) const
{
    return _codec_access->getBaseName(index);
}

size_t StaticDecoder::getStaticBufferSize() const
{
    return _codec_access->getStaticBufferSize(getRepresentation());
}

size_t StaticDecoder::getStaticBufferSize(DataRepresentation rep) const
{
    return _codec_access->getStaticBufferSize(rep);
}

size_t StaticDecoder::getElementCount() const
{
    return _codec_access->getLeafIndexCount();
}

DataRepresentation StaticDecoder::getRepresentation() const
{
    return _element_accessor->getRepresentation();
}

void StaticDecoder::resolve(CodecIndex& codec_index) const
{
    _codec_access->resolve(codec_index, false);
}

CodecIndex StaticDecoder::resolve(size_t leaf_index) const
{
    return _codec_access->resolve(leaf_index);
}

a_util::result::Result StaticDecoder::getElement(
    size_t leaf_index, const ddl::StructElement*& legacy_struct_element) const
{
    return _legacy_element.getStructElement(*this, leaf_index, legacy_struct_element);
}

const void* StaticDecoder::getData() const noexcept
{
    return _data;
}

size_t StaticDecoder::getDataSize() const noexcept
{
    return _data_size;
}

/************************************************************
 *
 ************************************************************/
StaticCodec::StaticCodec() : StaticDecoder(), _first_element(CodecIndex(), *this)
{
}

StaticCodec::StaticCodec(const std::shared_ptr<const StructAccess> codec_access,
                         void* data,
                         size_t data_size,
                         DataRepresentation representation)
    : StaticDecoder(codec_access, data, data_size, representation),
      _first_element(CodecIndex(), *this)
{
}

StaticCodec::StaticCodec(StaticCodec&& other)
    : StaticDecoder(std::move(other)), _first_element(CodecIndex(), *this)
{
}

StaticCodec& StaticCodec::operator=(StaticCodec&& other)
{
    StaticDecoder::operator=(std::move(other));
    _first_element.resetIndex(CodecIndex());
    return *this;
}

StaticCodec::Element StaticCodec::getElement(const std::string& full_name)
{
    auto index = _codec_access->resolve(full_name);
    return Element(std::move(index), *this);
}

StaticCodec::Element StaticCodec::getElement(const CodecIndex& index)
{
    auto validated_index = index;
    _codec_access->resolve(validated_index, false);
    return Element(std::move(validated_index), *this);
}

StaticCodec::Elements& StaticCodec::getElements()
{
    return _first_element.getChildElements();
}

void StaticCodec::setElementVariantValue(const CodecIndex& codec_index,
                                         const a_util::variant::Variant& value)
{
    a_util::result::Result result = {};
    if (codec_index.hasLayout()) {
        result = _element_accessor->setValue(
            codec_index.getLayout(), const_cast<void*>(_data), _data_size, value);
    }
    else {
        const ElementLayout element_layout = _codec_access->getCodecElementLayout(codec_index);
        result = _element_accessor->setValue(
            element_layout, const_cast<void*>(_data), _data_size, value);
    }
    if (isFailed(result)) {
        throw std::runtime_error(result.getDescription());
    }
}

void StaticCodec::setElementStringValue(const CodecIndex& codec_index, const std::string& value)
{
    const ElementLayout element_layout = _codec_access->getCodecElementLayout(codec_index);
    const bool set_with_enum =
        element_layout.type_info->isEnum() ? setEnumValue(*this, codec_index, value) : false;
    if (!set_with_enum) {
        if (a_util::strings::isUInt64(value)) {
            a_util::variant::Variant variant_value = a_util::strings::toUInt64(value);
            setElementVariantValue(codec_index, variant_value);
        }
        else if (a_util::strings::isInt64(value)) {
            a_util::variant::Variant variant_value = a_util::strings::toInt64(value);
            setElementVariantValue(codec_index, variant_value);
        }
        else {
            a_util::variant::Variant variant_value = a_util::strings::toDouble(value);
            setElementVariantValue(codec_index, variant_value);
        }
    }
}

void StaticCodec::setElementRawValue(const CodecIndex& index, const void* value, size_t value_size)
{
    const ElementLayout element_layout = _codec_access->getCodecElementLayout(index);
    if (value_size != 0) {
        // we check for valid size
        size_t bit_size_to_check =
            (_element_accessor->getRepresentation() == DataRepresentation::serialized) ?
                element_layout.serialized.type_bit_size :
                element_layout.deserialized.type_bit_size;
        if (value_size * 8 < bit_size_to_check) {
            throw std::runtime_error("value_size in bits (" + std::to_string(value_size * 8) +
                                     ") is lower then elements data type size in bits (" +
                                     std::to_string(bit_size_to_check) + ")");
        }
    }
    const auto result =
        _element_accessor->setValue(element_layout, const_cast<void*>(_data), _data_size, value);
    if (isFailed(result)) {
        throw std::runtime_error(result.getDescription());
    }
}

void* StaticCodec::getElementAddress(const CodecIndex& index)
{
    return const_cast<void*>(StaticDecoder::getElementAddress(index));
}

void StaticCodec::resetValues(bool zero_values)
{
    forEachLeafElement(getElements(), [&zero_values](StaticCodec::Element& element) {
        element.reset(zero_values);
    });
}

void* StaticCodec::getData() noexcept
{
    // this const_cast is possible because the _data member of the StaticDecoder can only be
    // initialized with the (non const) void* of the StaticCodec CTOR
    return const_cast<void*>(_data);
}

} // namespace codec
} // namespace ddl
