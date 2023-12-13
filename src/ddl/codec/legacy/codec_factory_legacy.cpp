/**
 * @file
 * Implementation of the ADTF default media description.
 *
 * Copyright @ 2021 VW Group. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "codec_access_legacy.h"

#include <ddl/codec/legacy/codec_factory_legacy.h>

namespace ddl {

CodecFactory::CodecFactory() : _layout(std::make_shared<StructLayout>(codec::CodecFactory()))
{
}

CodecFactory::CodecFactory(const std::string& struct_name, const std::string& ddl_string)
    : _layout(std::make_shared<StructLayout>(codec::CodecFactory(struct_name, ddl_string)))
{
}

CodecFactory::CodecFactory(const ddl::dd::StructType& struct_type, const dd::DataDefinition& ddl)
    : _layout(std::make_shared<StructLayout>(codec::CodecFactory(struct_type, ddl)))
{
}

CodecFactory::CodecFactory(const ddl::dd::StructTypeAccess& struct_type_access)
    : _layout(std::make_shared<StructLayout>(codec::CodecFactory(struct_type_access)))
{
}

CodecFactory::CodecFactory(const ddl::DDStructure& ddl_struct)
    : CodecFactory(ddl_struct.getStructType(), ddl_struct.getDD())
{
}

a_util::result::Result CodecFactory::isValid() const
{
    return _layout->getFactory().isValid();
}

size_t CodecFactory::getStaticElementCount() const
{
    return _layout->getFactory().getStaticElementCount();
}

a_util::result::Result CodecFactory::getStaticElement(size_t nIndex,
                                                      const StructElement*& pElement) const
{
    return _layout->getFactory().getStaticElement(nIndex, pElement);
}

size_t CodecFactory::getStaticBufferSize(DataRepresentation eRep) const
{
    return _layout->getFactory().getStaticBufferSize(eRep);
}

} // namespace ddl
