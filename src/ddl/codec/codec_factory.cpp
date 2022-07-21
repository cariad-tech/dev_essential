/**
 * @file
 * Implementation of the CodecFactory.
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

#include <a_util/strings/strings_functions.h>
#include <ddl/codec/codec_factory.h>
#include <ddl/dd/ddstring.h>

namespace ddl {
namespace codec {

// define all needed error types and values locally
_MAKE_RESULT(-37, ERR_NOT_INITIALIZED);
_MAKE_RESULT(-38, ERR_INVALID_DDL);

CodecFactory::CodecFactory()
    : _codec_access(std::make_shared<StructAccess>()),
      _constructor_result(ERR_NOT_INITIALIZED),
      _first_element(*this)
{
}

CodecFactory::CodecFactory(const CodecFactory& other)
    : _codec_access(other._codec_access),
      _constructor_result(other._constructor_result),
      _first_element(CodecIndex(), *this)
{
}

CodecFactory& CodecFactory::operator=(const CodecFactory& other)
{
    _codec_access = other._codec_access;
    _constructor_result = other._constructor_result;
    _first_element.resetIndex(CodecIndex());
    return *this;
}

CodecFactory::CodecFactory(CodecFactory&& other)
    : _codec_access(std::move(other._codec_access)),
      _constructor_result(std::move(other._constructor_result)),
      _first_element(CodecIndex(), *this)
{
}

CodecFactory& CodecFactory::operator=(CodecFactory&& other)
{
    _codec_access = std::move(other._codec_access);
    _constructor_result = std::move(other._constructor_result);
    _first_element.resetIndex(CodecIndex());
    return *this;
}

CodecFactory::CodecFactory(const std::string& struct_name, const std::string& ddl_string)
    : _first_element(*this)
{
    try {
        // this might throw and set the result to ERR_INVALID_DDL in catch block
        DataDefinition created_dd = DDString::fromXMLString(struct_name, ddl_string);
        _codec_access = std::make_shared<StructAccess>(created_dd.getStructTypeAccess(struct_name),
                                                       created_dd.getVersion());
        _constructor_result = _codec_access->getInitResult();
    }
    catch (const dd::Error& dd_err) {
        std::string error_desc =
            a_util::strings::join(dd::transformProblemList(dd_err.problems()), ";");
        _constructor_result = a_util::result::Result(
            ERR_INVALID_DDL, error_desc.c_str(), __LINE__, __FILE__, "CodecFactory::CodecFactory");
    }

    if (!_codec_access) {
        _codec_access = std::make_shared<StructAccess>();
    }
    _first_element.resetIndex(CodecIndex());
}

CodecFactory::CodecFactory(const ddl::dd::StructTypeAccess& struct_type_access)
    : _codec_access(std::make_shared<StructAccess>(struct_type_access)),
      _first_element(CodecIndex(), *this)
{
    _constructor_result = _codec_access->getInitResult();
}

CodecFactory::CodecFactory(const ddl::dd::StructType& struct_type, const dd::DataDefinition& dd)
    : CodecFactory(dd.getStructTypeAccess(struct_type.getName()))
{
}

CodecFactory::CodecFactory(const ddl::DDStructure& ddl_struct)
    : CodecFactory(ddl_struct.getDD().getStructTypeAccess(ddl_struct.getStructName()))
{
}

a_util::result::Result CodecFactory::isValid() const
{
    return _constructor_result;
}

/**
 * Creates a decoder for the given data.
 * @param[in] data The pointer to the raw data.
 * @param[in] data_size The size of the raw data.
 * @param[in] rep The representation that the data is encoded in.
 * @return a decoder.
 */
Decoder CodecFactory::makeDecoderFor(const void* data,
                                     size_t data_size,
                                     DataRepresentation rep) const
{
    // we make a deep copy and resolve the dynamic content
    return Decoder(_codec_access, data, data_size, rep);
}

/**
 * Creates a codec for the given data.
 * @param[in] data The pointer to the raw data.
 * @param[in] data_size The size of the raw data.
 * @param[in] rep The representation that the data is encoded in.
 * @return a codec.
 */

Codec CodecFactory::makeCodecFor(void* data, size_t data_size, DataRepresentation rep) const
{
    return Codec(_codec_access, data, data_size, rep);
}

CodecFactory::Element CodecFactory::getElement(const std::string& full_name) const
{
    auto index = _codec_access->resolve(full_name);
    return Element(std::move(index), *this);
}

CodecFactory::Element CodecFactory::getElement(const CodecIndex& index) const
{
    auto validated_index = index;
    _codec_access->resolve(validated_index, false);
    return Element(std::move(validated_index), *this);
}

const CodecFactory::Elements& CodecFactory::getElements() const
{
    return _first_element.getChildElements();
}

size_t CodecFactory::getElementChildCount(const CodecIndex& codec_index) const
{
    return _codec_access->getElementChildCount(codec_index, true);
}

std::string CodecFactory::getElementFullName(const CodecIndex& index) const
{
    std::string full_name;
    _codec_access->getCodecElementLayout(index, full_name);
    return full_name;
}

std::string CodecFactory::getElementName(const CodecIndex& index) const
{
    // we search only in the single element of the root
    return _codec_access->getName(index);
}

const std::string& CodecFactory::getElementBaseName(const CodecIndex& index) const
{
    // we search only in the single element of the root
    return _codec_access->getBaseName(index);
}

size_t CodecFactory::getStaticBufferSize(DataRepresentation representation) const
{
    return _codec_access->getStaticBufferSize(representation);
}

void CodecFactory::resolve(CodecIndex& codec_index) const
{
    return _codec_access->resolve(codec_index, false);
}

CodecIndex CodecFactory::resolve(size_t leaf_index) const
{
    return _codec_access->resolve(leaf_index);
}

size_t CodecFactory::getStaticElementCount() const
{
    return _codec_access->getLeafIndexCount();
}

a_util::result::Result CodecFactory::getStaticElement(
    size_t leaf_index, const ddl::StructElement*& legacy_struct_element) const
{
    return _legacy_element.getStructElement(*this, leaf_index, legacy_struct_element);
}

} // namespace codec
} // namespace ddl
