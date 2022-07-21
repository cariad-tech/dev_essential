/**
 * @file
 * Implementation of the Codec and the Decoder.
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
#include <ddl/codec/codec.h>
#include <ddl/legacy_error_macros.h>
#include <ddl/utilities/std_to_string.h>

namespace ddl {
namespace codec {
// define all needed error types and values locally
_MAKE_RESULT(-5, ERR_INVALID_ARG);

Decoder::Decoder() : StaticDecoder(), _first_element(*this)
{
}

Decoder::Decoder(const Decoder& decoder,
                 const void* data,
                 size_t data_size,
                 DataRepresentation representation)
    : StaticDecoder(decoder._codec_access, data, data_size, representation),
      _first_element(CodecIndex(), *this)
{
}

Decoder::Decoder(std::shared_ptr<const StructAccess> codec_access,
                 const void* data,
                 size_t data_size,
                 DataRepresentation representation)
    : StaticDecoder(codec_access, data, data_size, representation),
      _first_element(CodecIndex(), *this)
{
    auto copied_codec_access = codec_access->makeResolvedCodecAccess();
    _codec_access = copied_codec_access;
    copied_codec_access->resolveDynamic([&](const NamedCodecIndex& index) -> size_t {
        auto codec_index = copied_codec_access->resolve(index);
        return this->getElementVariantValue(codec_index).asUInt64();
    });
    _first_element.resetIndex(CodecIndex());
}

Decoder::Decoder(Decoder&& other)
    : StaticDecoder(std::move(other)), _first_element(CodecIndex(), *this)
{
}

Decoder& Decoder::operator=(Decoder&& other)
{
    StaticDecoder::operator=(std::move(other));
    _first_element.resetIndex(CodecIndex());
    return *this;
}

a_util::result::Result Decoder::isValid() const
{
    RETURN_IF_FAILED(_codec_access->getInitResult());
    if (_data_size < getBufferSize()) {
        return ERR_INVALID_ARG;
    }

    return a_util::result::SUCCESS;
}

Decoder::Element Decoder::getElement(const std::string& full_name) const
{
    auto index = _codec_access->resolve(full_name);
    return Element(std::move(index), *this);
}

Decoder::Element Decoder::getElement(const CodecIndex& index) const
{
    auto validated_index = index;
    _codec_access->resolve(validated_index, false);
    return Element(std::move(validated_index), *this);
}

const Decoder::Elements& Decoder::getElements() const
{
    return _first_element.getChildElements();
}

size_t Decoder::getBufferSize() const
{
    return _codec_access->getBufferSize(getRepresentation());
}

size_t Decoder::getBufferSize(DataRepresentation representation) const
{
    return _codec_access->getBufferSize(representation);
}

Codec Decoder::makeCodecFor(void* data, size_t data_size, DataRepresentation representation) const
{
    return Codec(*this, data, data_size, representation);
}

/************************************************************
 *
 ************************************************************/
Codec::Codec() : Decoder(), _first_element(*this)
{
}

Codec::Codec(std::shared_ptr<const StructAccess> codec_access,
             void* data,
             size_t data_size,
             DataRepresentation representation)
    : Decoder(codec_access, data, data_size, representation), _first_element(CodecIndex(), *this)
{
}

Codec::Codec(const Decoder& decoder,
             void* data,
             size_t data_size,
             DataRepresentation representation)
    : Decoder(decoder, data, data_size, representation), _first_element(CodecIndex(), *this)
{
}

Codec::Codec(Codec&& other) : Decoder(std::move(other)), _first_element(CodecIndex(), *this)
{
}

Codec& Codec::operator=(Codec&& other)
{
    Decoder::operator=(std::move(other));
    return *this;
}

Codec::Element Codec::getElement(const std::string& full_name)
{
    auto index = _codec_access->resolve(full_name);
    return Element(std::move(index), *this);
}

Codec::Element Codec::getElement(const CodecIndex& index)
{
    auto validated_index = index;
    _codec_access->resolve(validated_index, false);
    return Element(std::move(validated_index), *this);
}

Codec::Elements& Codec::getElements()
{
    return _first_element.getChildElements();
}

namespace {
template <typename T>
void setElementValueAsVariant(const StructAccess& access,
                              const ElementAccessor& accessor,
                              void* data,
                              const size_t data_size,
                              const CodecIndex& index,
                              const T& value)
{
    ElementLayout element_layout = access.getCodecElementLayout(index);
    auto result = accessor.setValue(element_layout, data, data_size, value);
    if (isFailed(result)) {
        throw std::runtime_error(result.getDescription());
    }
}
} // namespace

void Codec::setElementVariantValue(const CodecIndex& codec_index,
                                   const a_util::variant::Variant& value)
{
    setElementValueAsVariant(*_codec_access,
                             *_element_accessor,
                             const_cast<void*>(_data),
                             _data_size,
                             codec_index,
                             value);
}

void Codec::setElementStringValue(const CodecIndex& codec_index, const std::string& value)
{
    ElementLayout element_layout = _codec_access->getCodecElementLayout(codec_index);
    bool set_with_enum = false;
    if (element_layout.type_info->isEnum()) {
        set_with_enum = setEnumValue(*this, codec_index, value);
    }
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

void Codec::setElementRawValue(const CodecIndex& index, const void* value, size_t value_size)
{
    ElementLayout element_layout = _codec_access->getCodecElementLayout(index);
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
    auto result =
        _element_accessor->setValue(element_layout, const_cast<void*>(_data), _data_size, value);
    if (isFailed(result)) {
        throw std::runtime_error(result.getDescription());
    }
}

void* Codec::getElementAddress(const CodecIndex& index)
{
    // this const_cast is possible because the _data member of the StaticDecoder can only be
    // initialized with the (non const) void* of the Codec CTOR
    return const_cast<void*>(Decoder::getElementAddress(index));
}

void Codec::resetValues(bool zero_values)
{
    forEachLeafElement(getElements(),
                       [&zero_values](Codec::Element& element) { element.reset(zero_values); });
}

void* Codec::getData() noexcept
{
    // this const_cast is possible because the _data member of the StaticDecoder can only be
    // initialized with the (non const) void* of the Codec CTOR
    return const_cast<void*>(_data);
}

} // namespace codec
} // namespace ddl
