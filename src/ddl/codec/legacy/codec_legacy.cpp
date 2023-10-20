/**
 * @file
 * Implementation of the ADTF default media description.
 *
 * Copyright @ 2022 VW Group. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "codec_access_legacy.h"

#include <ddl/codec/legacy/codec_legacy.h>

namespace ddl {
// define all needed error types and values locally
_MAKE_RESULT(-10, ERR_INVALID_INDEX);

Decoder::Decoder(std::shared_ptr<const StructLayout> layout,
                 const void* data,
                 size_t data_size,
                 DataRepresentation rep)
    : StaticDecoder(std::make_shared<LegacyCodecAccessImpl<codec::Decoder>>(
          layout->getFactory().makeDecoderFor(data, data_size, rep)))
{
}

Decoder::Decoder(const std::shared_ptr<LegacyCodecAccess>& legacy_codec_access)
    : StaticDecoder(legacy_codec_access)
{
}

a_util::result::Result Decoder::isValid() const
{
    return StaticDecoder::isValid();
}

size_t Decoder::getElementCount() const
{
    return StaticDecoder::getElementCount();
}

size_t Decoder::getBufferSize(DataRepresentation rep) const
{
    return _legacy_access->getImpl<codec::Decoder>()->getBufferSize(rep);
}

Codec Decoder::makeCodecFor(void* data, size_t data_size, DataRepresentation rep) const
{
    return Codec(_legacy_access->getImpl<codec::Decoder>()->makeCodecFor(data, data_size, rep));
}

const void* Decoder::getLayoutElement(size_t) const
{
    // this implementation must stay only for binary compatibility reason
    return nullptr;
}

/*****************************************************************************/
/*****************************************************************************/

Codec::Codec(std::shared_ptr<const StructLayout> layout,
             void* data,
             size_t data_size,
             DataRepresentation rep)
    : Decoder(std::make_shared<LegacyCodecAccessImpl<codec::Codec>>(
          layout->getFactory().makeCodecFor(data, data_size, rep)))
{
}

Codec::Codec(codec::Codec&& codec)
    : Decoder(std::make_shared<LegacyCodecAccessImpl<codec::Codec>>(std::move(codec)))
{
}

a_util::result::Result Codec::setElementValue(size_t nIndex, const void* pValue)
{
    try {
        auto codec = _legacy_access->getImpl<codec::Codec>();
        auto codec_index = codec->resolve(nIndex);
        //!!! this is potentional unsafe !!
        codec->setElementRawValue(codec_index, pValue, 0);
        return {};
    }
    catch (const std::exception& ex) {
        RETURN_ERROR_DESCRIPTION(ERR_INVALID_INDEX, ex.what());
    }
}

a_util::result::Result Codec::setElementValue(size_t nIndex, const a_util::variant::Variant& oValue)
{
    try {
        auto codec = _legacy_access->getImpl<codec::Codec>();
        auto codec_index = codec->resolve(nIndex);
        codec->setElementVariantValue(codec_index, oValue);
        return {};
    }
    catch (const std::exception& ex) {
        RETURN_ERROR_DESCRIPTION(ERR_INVALID_INDEX, ex.what());
    }
}

void* Codec::getElementAddress(size_t nIndex)
{
    return const_cast<void*>(StaticDecoder::getElementAddress(nIndex));
}

a_util::result::Result Codec::setConstants()
{
    _legacy_access->getImpl<codec::Codec>()->resetValues();
    return {};
}

} // namespace ddl
