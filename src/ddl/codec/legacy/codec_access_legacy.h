/**
 * @file
 * Definition of a legacy binary compatible Codec Access class for old Codec API.
 *
 * Copyright @ 2021 VW Group. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef DDL_CODEC_ACCESS_LEGACY_CLASS_HEADER
#define DDL_CODEC_ACCESS_LEGACY_CLASS_HEADER

#include <ddl/codec/codec_factory.h>
#include <ddl/codec/static_codec.h>

namespace ddl {

// for legacy reason this acces must be forwarded thru this symbol named StructLayout
// because the the old implementation of factory has inlined makeCodecFor functions with a call to a
// CTOR using this symbol!
class StructLayout {
public:
    StructLayout(codec::CodecFactory&& factory) : _factory(std::move(factory))
    {
    }
    const codec::CodecFactory& getFactory() const
    {
        return _factory;
    }

private:
    codec::CodecFactory _factory;
};

struct LegacyCodecAccess {
    virtual ~LegacyCodecAccess() = default;
    virtual const codec::StaticDecoder* getStaticDecoder() const = 0;
    virtual void* getImplPtr() = 0;
    template <typename T>
    T* getImpl()
    {
        return static_cast<T*>(getImplPtr());
    }
};

template <class T>
struct LegacyCodecAccessImpl : LegacyCodecAccess {
    LegacyCodecAccessImpl() = default;
    LegacyCodecAccessImpl(T&& codec_or_decoder) : _codec_or_decoder(std::move(codec_or_decoder))
    {
    }
    const codec::StaticDecoder* getStaticDecoder() const override
    {
        return static_cast<const codec::StaticDecoder*>(&_codec_or_decoder);
    }
    void* getImplPtr() override
    {
        return static_cast<void*>(&_codec_or_decoder);
    }
    T _codec_or_decoder;
};

} // namespace ddl

#endif // DDL_CODEC_ACCESS_LEGACY_CLASS_HEADER
