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

#include "struct_layout.h"

#include <a_util/result/error_def.h>
#include <ddl/dd/ddenum.h>
#include <ddl/legacy_error_macros.h>

#include <algorithm>

namespace ddl {
// define all needed error types and values locally
_MAKE_RESULT(-5, ERR_INVALID_ARG);
_MAKE_RESULT(-19, ERR_NOT_SUPPORTED);
_MAKE_RESULT(-37, ERR_NOT_INITIALIZED);

StructLayout::StructLayout(const dd::StructTypeAccess& ddl_struct_access)
{
    _static_buffer_sizes.deserialized = 0;
    _static_buffer_sizes.serialized = 0;
    _calculations_result = calculate(ddl_struct_access);
}

StructLayout::StructLayout() : _calculations_result(ERR_NOT_INITIALIZED)
{
    _static_buffer_sizes.deserialized = 0;
    _static_buffer_sizes.serialized = 0;
}

class SupportedTypes {
private:
    SupportedTypes()
    {
        _supported_types["tBool"] = {a_util::variant::VariantType::VT_Bool, sizeof(bool) * 8};
        _supported_types["bool"] = {a_util::variant::VariantType::VT_Bool, sizeof(bool) * 8};
        _supported_types["tChar"] = {a_util::variant::VariantType::VT_Int8, sizeof(char) * 8};
        _supported_types["char"] = {a_util::variant::VariantType::VT_Int8, sizeof(char) * 8};

        _supported_types["tInt8"] = {a_util::variant::VariantType::VT_Int8, sizeof(int8_t) * 8};
        _supported_types["int8_t"] = {a_util::variant::VariantType::VT_Int8, sizeof(int8_t) * 8};
        _supported_types["tInt16"] = {a_util::variant::VariantType::VT_Int16, sizeof(int16_t) * 8};
        _supported_types["int16_t"] = {a_util::variant::VariantType::VT_Int16, sizeof(int16_t) * 8};
        _supported_types["tInt32"] = {a_util::variant::VariantType::VT_Int32, sizeof(int32_t) * 8};
        _supported_types["int32_t"] = {a_util::variant::VariantType::VT_Int32, sizeof(int32_t) * 8};
        _supported_types["tInt64"] = {a_util::variant::VariantType::VT_Int64, sizeof(int64_t) * 8};
        _supported_types["int64_t"] = {a_util::variant::VariantType::VT_Int64, sizeof(int64_t) * 8};

        _supported_types["tUInt8"] = {a_util::variant::VariantType::VT_UInt8, sizeof(uint8_t) * 8};
        _supported_types["uint8_t"] = {a_util::variant::VariantType::VT_UInt8, sizeof(uint8_t) * 8};
        _supported_types["tUInt16"] = {a_util::variant::VariantType::VT_UInt16,
                                       sizeof(uint16_t) * 8};
        _supported_types["uint16_t"] = {a_util::variant::VariantType::VT_UInt16,
                                        sizeof(uint16_t) * 8};
        _supported_types["tUInt32"] = {a_util::variant::VariantType::VT_UInt32,
                                       sizeof(uint32_t) * 8};
        _supported_types["uint32_t"] = {a_util::variant::VariantType::VT_UInt32,
                                        sizeof(uint32_t) * 8};
        _supported_types["tUInt64"] = {a_util::variant::VariantType::VT_UInt64,
                                       sizeof(uint64_t) * 8};
        _supported_types["uint64_t"] = {a_util::variant::VariantType::VT_UInt64,
                                        sizeof(uint64_t) * 8};

        _supported_types["tFloat32"] = {a_util::variant::VariantType::VT_Float, sizeof(float) * 8};
        _supported_types["float"] = {a_util::variant::VariantType::VT_Float, sizeof(float) * 8};
        _supported_types["tFloat64"] = {a_util::variant::VariantType::VT_Double,
                                        sizeof(double) * 8};
        _supported_types["double"] = {a_util::variant::VariantType::VT_Double, sizeof(double) * 8};
    }

public:
    static const SupportedTypes& getInst()
    {
        static SupportedTypes s_types;
        return s_types;
    }
    a_util::result::Result getType(const std::string& strType,
                                   a_util::variant::VariantType& type,
                                   size_t& nTypeSize) const
    {
        const auto& found = _supported_types.find(strType);
        if (found != _supported_types.end()) {
            type = found->second._type;
            nTypeSize = found->second._size;
            return {};
        }
        else {
            return ERR_NOT_SUPPORTED;
        }
    }

private:
    struct Info {
        a_util::variant::VariantType _type;
        size_t _size;
    };
    std::unordered_map<std::string, Info> _supported_types;
};

static a_util::result::Result getType(const std::string& strType,
                                      a_util::variant::VariantType& type,
                                      size_t& nTypeSize)
{
    return SupportedTypes::getInst().getType(strType, type, nTypeSize);
}

class cConverter {
public:
    cConverter(std::vector<StructLayoutElement>& static_elements,
               std::vector<DynamicStructLayoutElement>& dynamic_elements,
               std::map<std::string, AccessEnumType>& oEnums)
        : m_bDynamicSectionStarted(false),
          _static_elements(static_elements),
          _dynamic_elements(dynamic_elements),
          _enums(oEnums)
    {
        m_sOffsets.deserialized = 0;
        m_sOffsets.serialized = 0;
    }

    a_util::result::Result Convert(const dd::StructTypeAccess& ddl_struct_access)
    {
        m_sOffsets.deserialized = ddl_struct_access.getStaticStructSize() * 8;
        m_sOffsets.serialized = ddl_struct_access.getStaticSerializedBitSize();
        return addStruct(ddl_struct_access, "");
    }

    Offsets getStaticBufferBitSizes()
    {
        return m_sOffsets;
    }

private:
    a_util::result::Result add(const dd::StructElementAccess& element_access,
                               const std::string& strFullName,
                               const std::string& strConstant,
                               const size_t array_index,
                               bool bDynamic)
    {
        const auto POD_type = element_access.getDataType();
        const auto enum_type = element_access.getEnumType();
        AccessEnumType* pCodecEnum = NULL;
        if (enum_type) {
            pCodecEnum = getOrCreateCodecEnum(*enum_type);
        }

        if (POD_type) {
            return addPODElement(
                element_access, strFullName, pCodecEnum, strConstant, array_index, bDynamic);
        }
        else {
            size_t used_array_index = array_index;
            if (bDynamic) {
                used_array_index = (size_t)-1;
            }
            auto struct_access = element_access.getStructTypeAccess(used_array_index);
            if (struct_access) {
                return addStruct(struct_access, strFullName);
            }

            return ERR_INVALID_ARG;
        }
    }

    AccessEnumType* getOrCreateCodecEnum(const dd::EnumType& enum_type)
    {
        std::map<std::string, AccessEnumType>::iterator itEnum = _enums.find(enum_type.getName());
        if (itEnum == _enums.end()) {
            AccessEnumType oCodecEnum;
            for (const auto& ref_value: enum_type.getElements()) {
                oCodecEnum.insert(
                    std::make_pair(ref_value.second->getName(),
                                   a_util::variant::Variant(ref_value.second->getValue().c_str())));
            }
            itEnum = _enums.insert(std::make_pair(enum_type.getName(), oCodecEnum)).first;
        }
        return &itEnum->second;
    }

    a_util::result::Result addPODElement(const dd::StructElementAccess& elem,
                                         const std::string& strFullName,
                                         AccessEnumType* p_enum,
                                         const std::string& strConstant,
                                         const size_t array_index,
                                         bool is_for_dynamic)
    {
        StructLayoutElement sElement;
        sElement.name = strFullName;

        if (p_enum) {
            RETURN_IF_FAILED(getType(elem.getEnumType()->getDataTypeName(),
                                     sElement.type,
                                     sElement.deserialized.bit_size));
        }
        else {
            RETURN_IF_FAILED(getType(
                elem.getElement().getTypeName(), sElement.type, sElement.deserialized.bit_size));
        }

        if (is_for_dynamic) {
            sElement.deserialized.bit_offset = 0;
            sElement.serialized.bit_offset = 0;
        }
        else {
            sElement.deserialized.bit_offset = elem.getDeserializedBytePos(array_index) * 8;
            sElement.serialized.bit_offset = elem.getSerializedBitOffset(array_index);
        }
        auto elem_bit_size = elem.getSerializedBitSize();
        auto type_bit_size = elem.getSerializedTypeBitSize();
        if (elem_bit_size != 0 && elem_bit_size < type_bit_size) {
            sElement.serialized.bit_size = elem_bit_size;
        }
        else {
            sElement.serialized.bit_size = type_bit_size;
        }
        sElement.byte_order = elem.getElement().getByteOrder();
        sElement.p_enum = p_enum;
        sElement.constant = findConstant(strConstant);
        _static_elements.push_back(std::move(sElement));

        return {};
    }

    const a_util::variant::Variant* findConstant(const std::string& strConstant)
    {
        if (!strConstant.empty()) {
            for (std::map<std::string, AccessEnumType>::const_iterator it = _enums.begin();
                 it != _enums.end();
                 ++it) {
                AccessEnumType::const_iterator itValue = it->second.find(strConstant);
                if (itValue != it->second.end()) {
                    return &itValue->second;
                }
            }
        }

        return NULL;
    }

    a_util::result::Result addStruct(const dd::StructTypeAccess& ddl_struct_access,
                                     const std::string& strFullName)
    {
        const std::string struct_prefix = (!strFullName.empty()) ? strFullName + "." : "";
        const bool was_dynamic_section_started_before = m_bDynamicSectionStarted;

        for (const auto& struct_element_access: ddl_struct_access) {
            RETURN_IF_FAILED(addElement(struct_element_access, struct_prefix));
        }

        if (m_bDynamicSectionStarted && !was_dynamic_section_started_before) {
            // add an alignment marker to ensure that the next element starts at the
            // correct position
            _dynamic_elements.push_back(*ddl_struct_access.getStructType().getAlignment());
        }

        return {};
    }

    a_util::result::Result addElement(const dd::StructElementAccess& element_access,
                                      const std::string& strStructPrefix)
    {
        const auto& ref_element = element_access.getElement();
        if (!m_bDynamicSectionStarted) {
            m_bDynamicSectionStarted = element_access.isDynamic();
        }

        const size_t nArraySize =
            std::max(ref_element.getArraySize().getArraySizeValue(), size_t(1u));
        for (size_t nArrayIndex = 0; nArrayIndex < nArraySize; ++nArrayIndex) {
            std::string strCurrentElementName = strStructPrefix + ref_element.getName();
            if (nArraySize > 1) {
                strCurrentElementName += a_util::strings::format("[%d]", nArrayIndex);
            }

            if (!m_bDynamicSectionStarted) {
                RETURN_IF_FAILED(add(element_access,
                                     strCurrentElementName,
                                     ref_element.getValue(),
                                     nArrayIndex,
                                     false));
            }
            else {
                RETURN_IF_FAILED(
                    addDynamicElement(element_access, strCurrentElementName, strStructPrefix));
            }
        }

        return {};
    }

    a_util::result::Result addDynamicElement(const dd::StructElementAccess& element_access,
                                             const std::string& strFullName,
                                             const std::string& strStructPrefix)
    {
        DynamicStructLayoutElement sDynamicElement;
        cConverter oChildConverter(
            sDynamicElement.static_elements, sDynamicElement.dynamic_elements, _enums);

        const auto& elem_ref = element_access.getElement();
        RETURN_IF_FAILED(oChildConverter.add(element_access, "", elem_ref.getValue(), 0, true));

        sDynamicElement.name = strFullName;
        sDynamicElement.alignment = elem_ref.getAlignment();

        if (element_access.isDynamic()) {
            if (elem_ref.getArraySize().getArraySizeElementName().empty()) {
                return ERR_INVALID_ARG;
            }

            sDynamicElement.size_element_name =
                strStructPrefix + elem_ref.getArraySize().getArraySizeElementName();
        }

        _dynamic_elements.push_back(std::move(sDynamicElement));

        return {};
    }

protected:
    Offsets m_sOffsets;
    bool m_bDynamicSectionStarted;
    std::vector<StructLayoutElement>& _static_elements;
    std::vector<DynamicStructLayoutElement>& _dynamic_elements;
    std::map<std::string, AccessEnumType>& _enums;
};

a_util::result::Result StructLayout::calculate(const dd::StructTypeAccess& ddl_struct_access)
{
    cConverter oConverter(_static_elements, _dynamic_elements, _enums);
    RETURN_IF_FAILED(oConverter.Convert(ddl_struct_access));
    _static_buffer_sizes = oConverter.getStaticBufferBitSizes();

    return {};
}

size_t StructLayout::getStaticBufferSize(DataRepresentation eRep) const
{
    size_t nResult =
        eRep == deserialized ? _static_buffer_sizes.deserialized : _static_buffer_sizes.serialized;
    if (nResult % 8) {
        return nResult / 8 + 1;
    }
    return nResult / 8;
}

} // namespace ddl
