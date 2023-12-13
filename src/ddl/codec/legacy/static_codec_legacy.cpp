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

#include <a_util/result/error_def.h>
#include <ddl/codec/legacy/static_codec_legacy.h>
#include <ddl/legacy_error_macros.h>

#include <stdexcept>

namespace ddl {
// define all needed error types and values locally
_MAKE_RESULT(-10, ERR_INVALID_INDEX);

StaticDecoder::StaticDecoder()
    : _legacy_access(std::make_shared<LegacyCodecAccessImpl<codec::StaticDecoder>>())
{
}

StaticDecoder::StaticDecoder(std::shared_ptr<const StructLayout> layout,
                             const void* data,
                             size_t data_size,
                             DataRepresentation rep)
    : _legacy_access(std::make_shared<LegacyCodecAccessImpl<codec::StaticDecoder>>(
          layout->getFactory().makeStaticDecoderFor(data, data_size, rep)))
{
}

StaticDecoder::StaticDecoder(const std::shared_ptr<LegacyCodecAccess>& legacy_codec_access)
    : _legacy_access(legacy_codec_access)
{
}

a_util::result::Result StaticDecoder::isValid() const
{
    return _legacy_access->getStaticDecoder()->isValid();
}

size_t StaticDecoder::getElementCount() const
{
    return _legacy_access->getStaticDecoder()->getElementCount();
}

a_util::result::Result StaticDecoder::getElement(size_t nIndex,
                                                 const StructElement*& pElement) const
{
    return _legacy_access->getStaticDecoder()->getElement(nIndex, pElement);
}

a_util::result::Result StaticDecoder::getElementValue(size_t nIndex, void* pValue) const
{
    try {
        auto codec_index = _legacy_access->getStaticDecoder()->resolve(nIndex);
        //!!! potentional unsafe, because there is no size check internally !!
        _legacy_access->getStaticDecoder()->getElementRawValue(codec_index, pValue, 0);
        return {};
    }
    catch (const std::exception& ex) {
        RETURN_ERROR_DESCRIPTION(ERR_INVALID_INDEX, ex.what());
    }
}

a_util::result::Result StaticDecoder::getElementValue(size_t nIndex,
                                                      a_util::variant::Variant& oValue) const
{
    try {
        auto codec_index = _legacy_access->getStaticDecoder()->resolve(nIndex);
        oValue = _legacy_access->getStaticDecoder()->getElementVariantValue(codec_index);
        return {};
    }
    catch (const std::exception& ex) {
        RETURN_ERROR_DESCRIPTION(ERR_INVALID_INDEX, ex.what());
    }
}

const void* StaticDecoder::getElementAddress(size_t nIndex) const
{
    try {
        auto codec_index = _legacy_access->getStaticDecoder()->resolve(nIndex);
        return _legacy_access->getStaticDecoder()->getElementAddress(codec_index);
    }
    catch (const std::exception&) {
        return nullptr;
    }
}

size_t StaticDecoder::getStaticBufferSize(DataRepresentation eRep) const
{
    return _legacy_access->getStaticDecoder()->getStaticBufferSize(eRep);
}

DataRepresentation StaticDecoder::getRepresentation() const
{
    return _legacy_access->getStaticDecoder()->getRepresentation();
}

/***************************************************************************************/
/***************************************************************************************/

StaticCodec::StaticCodec()
    : StaticDecoder(std::make_shared<LegacyCodecAccessImpl<codec::StaticDecoder>>())
{
}

StaticCodec::StaticCodec(std::shared_ptr<const StructLayout> layout,
                         void* data,
                         size_t data_size,
                         DataRepresentation rep)
    : StaticDecoder(std::make_shared<LegacyCodecAccessImpl<codec::StaticDecoder>>(
          layout->getFactory().makeStaticCodecFor(data, data_size, rep)))
{
}

a_util::result::Result StaticCodec::setElementValue(size_t nIndex, const void* pValue)
{
    try {
        auto static_codec = _legacy_access->getImpl<codec::StaticCodec>();
        auto codec_index = static_codec->resolve(nIndex);
        //!!! this is potentional unsafe !!
        static_codec->setElementRawValue(codec_index, pValue, 0);
        return {};
    }
    catch (const std::exception& ex) {
        RETURN_ERROR_DESCRIPTION(ERR_INVALID_INDEX, ex.what());
    }
}

a_util::result::Result StaticCodec::setElementValue(size_t nIndex,
                                                    const a_util::variant::Variant& oValue)
{
    try {
        auto static_codec = _legacy_access->getImpl<codec::StaticCodec>();
        auto codec_index = static_codec->resolve(nIndex);
        static_codec->setElementVariantValue(codec_index, oValue);
        return {};
    }
    catch (const std::exception& ex) {
        RETURN_ERROR_DESCRIPTION(ERR_INVALID_INDEX, ex.what());
    }
}

void* StaticCodec::getElementAddress(size_t nIndex)
{
    try {
        auto static_codec = _legacy_access->getImpl<codec::StaticCodec>();
        auto codec_index = static_codec->resolve(nIndex);
        return static_codec->getElementAddress(codec_index);
    }
    catch (const std::exception&) {
        return nullptr;
    }
}

a_util::result::Result StaticCodec::setConstants()
{
    _legacy_access->getImpl<codec::StaticCodec>()->resetValues();
    return {};
}

} // namespace ddl
