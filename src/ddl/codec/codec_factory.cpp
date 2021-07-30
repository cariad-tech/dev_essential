/**
 * @file
 * Implementation of the ADTF default media description.
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

#include "ddl/codec/codec_factory.h"

#include "a_util/strings.h"
#include "ddl/dd/ddstring.h"
#include "struct_layout.h"

namespace ddl {
// define all needed error types and values locally
_MAKE_RESULT(-10, ERR_INVALID_INDEX);
_MAKE_RESULT(-20, ERR_NOT_FOUND);
_MAKE_RESULT(-37, ERR_NOT_INITIALIZED);
_MAKE_RESULT(-38, ERR_INVALID_DDL);

CodecFactory::CodecFactory() : _layout(new StructLayout()), _constructor_result(ERR_NOT_INITIALIZED)
{
}

CodecFactory::CodecFactory(const std::string& struct_name, const std::string& ddl_string)
{
    DataDefinition created_dd;
    try {
        created_dd = DDString::fromXMLString(struct_name, ddl_string);
        _constructor_result = a_util::result::SUCCESS;
    }
    catch (const dd::Error& dd_err) {
        std::string error_desc =
            a_util::strings::join(dd::transformProblemList(dd_err.problems()), ";");
        _constructor_result = a_util::result::Result(
            ERR_INVALID_DDL, error_desc.c_str(), __LINE__, __FILE__, "CodecFactory::CodecFactory");
    }
    if (isOk(_constructor_result)) {
        auto struct_access = created_dd.getStructTypeAccess(struct_name);
        if (struct_access) {
            _layout.reset(new StructLayout(struct_access));
            _constructor_result = _layout->isValid();
        }
        else {
            _constructor_result = ERR_NOT_FOUND;
        }
    }

    if (!_layout) {
        _layout.reset(new StructLayout());
    }
}

CodecFactory::CodecFactory(const ddl::dd::StructType& struct_type, const dd::DataDefinition& ddl)
    : _layout(std::make_shared<StructLayout>(ddl.getStructTypeAccess(struct_type.getName())))
{
    _constructor_result = _layout->isValid();
}
CodecFactory::CodecFactory(const ddl::dd::StructTypeAccess& struct_type_access)
    : _layout(std::make_shared<StructLayout>(struct_type_access))
{
    _constructor_result = _layout->isValid();
}

CodecFactory::CodecFactory(const ddl::DDStructure& ddl_struct)
    : CodecFactory(ddl_struct.getStructType(), ddl_struct.getDD())
{
}

a_util::result::Result CodecFactory::isValid() const
{
    return _constructor_result;
}

size_t CodecFactory::getStaticElementCount() const
{
    return _layout->getStaticElements().size();
}

a_util::result::Result CodecFactory::getStaticElement(size_t nIndex,
                                                      const StructElement*& pElement) const
{
    if (nIndex >= getStaticElementCount()) {
        return ERR_INVALID_INDEX;
    }
    pElement = &_layout->getStaticElements()[nIndex];

    return a_util::result::SUCCESS;
}

size_t CodecFactory::getStaticBufferSize(DataRepresentation eRep) const
{
    return _layout->getStaticBufferSize(eRep);
}

} // namespace ddl
