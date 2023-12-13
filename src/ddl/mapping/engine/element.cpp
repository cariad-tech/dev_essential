/**
 * @file
 *
 * Copyright @ 2021 VW Group. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <a_util/memory.h>
#include <ddl/mapping/engine/element.h>

#include <assert.h>

namespace ddl {
namespace mapping {
namespace rt {
// define all needed error types and values locally
_MAKE_RESULT(-3, ERR_UNEXPECTED);
_MAKE_RESULT(-5, ERR_INVALID_ARG);
_MAKE_RESULT(-42, ERR_INVALID_TYPE);
} // namespace rt
} // namespace mapping
} // namespace ddl

using namespace ddl::mapping;
using namespace ddl::mapping::rt;

namespace {
/// Helper method that sets a casted value in the coder
template <typename T>
static void SetCastedValue(void* pDestination, size_t nArrayIndex, T* pData, uint32_t type32)
{
    static_assert(!std::is_same<bool, typename std::remove_cv<T>::type>::value,
                  "For bool an overload must be implemented.");
    switch (type32) {
    case e_uint8: {
        uint8_t ui8Temp = static_cast<uint8_t>(*pData);
        a_util::memory::copy(
            (uint8_t*)pDestination + nArrayIndex, sizeof(uint8_t), &ui8Temp, sizeof(uint8_t));
    } break;
    case e_uint16: {
        uint16_t ui16Temp = static_cast<uint16_t>(*pData);
        a_util::memory::copy(
            (uint16_t*)pDestination + nArrayIndex, sizeof(uint16_t), &ui16Temp, sizeof(uint16_t));
    } break;
    case e_uint32: {
        uint32_t ui32Temp = static_cast<uint32_t>(*pData);
        a_util::memory::copy(
            (uint32_t*)pDestination + nArrayIndex, sizeof(uint32_t), &ui32Temp, sizeof(uint32_t));
    } break;
    case e_uint64: {
        uint64_t ui64Temp = static_cast<uint64_t>(*pData);
        a_util::memory::copy(
            (uint64_t*)pDestination + nArrayIndex, sizeof(uint64_t), &ui64Temp, sizeof(uint64_t));
    } break;
    case e_int8: {
        int8_t i8Temp = static_cast<int8_t>(*pData);
        a_util::memory::copy(
            (int8_t*)pDestination + nArrayIndex, sizeof(int8_t), &i8Temp, sizeof(int8_t));
    } break;
    case e_int16: {
        int16_t i16Temp = static_cast<int16_t>(*pData);
        a_util::memory::copy(
            (int16_t*)pDestination + nArrayIndex, sizeof(int16_t), &i16Temp, sizeof(int16_t));
    } break;
    case e_int32: {
        int32_t i32Temp = static_cast<int32_t>(*pData);
        a_util::memory::copy(
            (int32_t*)pDestination + nArrayIndex, sizeof(int32_t), &i32Temp, sizeof(int32_t));
    } break;
    case e_int64: {
        int64_t i64Temp = static_cast<int64_t>(*pData);
        a_util::memory::copy(
            (int64_t*)pDestination + nArrayIndex, sizeof(int64_t), &i64Temp, sizeof(int64_t));
    } break;
    case e_float32: {
        float f32Temp = static_cast<float>(*pData);
        a_util::memory::copy(
            (float*)pDestination + nArrayIndex, sizeof(float), &f32Temp, sizeof(float));
    } break;
    case e_float64: {
        double f64Temp = static_cast<double>(*pData);
        a_util::memory::copy(
            (double*)pDestination + nArrayIndex, sizeof(double), &f64Temp, sizeof(double));
    } break;
    case e_bool: {
        bool bTemp = *pData != 0 ? true : false;
        a_util::memory::copy((bool*)pDestination + nArrayIndex, sizeof(bool), &bTemp, sizeof(bool));
    } break;
    case e_char: {
        char cTemp = static_cast<char>(*pData);
        a_util::memory::copy((char*)pDestination + nArrayIndex, sizeof(char), &cTemp, sizeof(char));
    } break;
    default:
        assert(false);
        break;
    }
}

template <typename T>
static inline void TransformAndSetValueImpl(const void* pData,
                                            void* pDestination,
                                            size_t nArraySize,
                                            const MapTransformationBase* pTrans,
                                            uint32_t type32)
{
    assert(pTrans);
    assert(pData);
    for (unsigned int i = 0; i < nArraySize; i++) {
        const double fValue = pTrans->evaluate(static_cast<double>(*((T*)pData + i)));
        SetCastedValue(pDestination, i, &fValue, type32);
    }
}

template <typename T>
static inline void SetValueImpl(const void* pData,
                                void* pDestination,
                                size_t nArraySize,
                                uint32_t type32)
{
    assert(pData);
    for (size_t i = 0; i < nArraySize; i++) {
        SetCastedValue(pDestination, i, static_cast<const T*>(pData) + i, type32);
    }
}

} // namespace

TargetElement::TargetElement(Target* pTarget)
    : _target(pTarget),
      _element_ptr(nullptr),
      _type_map{{"tUInt8", static_cast<uint8_t>(e_uint8)},
                {"tUInt16", static_cast<uint8_t>(e_uint16)},
                {"tUInt32", static_cast<uint8_t>(e_uint32)},
                {"tUInt64", static_cast<uint8_t>(e_uint64)},
                {"tInt8", static_cast<uint8_t>(e_int8)},
                {"tInt16", static_cast<uint8_t>(e_int16)},
                {"tInt32", static_cast<uint8_t>(e_int32)},
                {"tInt64", static_cast<uint8_t>(e_int64)},
                {"tFloat32", static_cast<uint8_t>(e_float32)},
                {"tFloat64", static_cast<uint8_t>(e_float64)},
                {"tBool", static_cast<uint8_t>(e_bool)},
                {"tChar", static_cast<uint8_t>(e_char)},
                {"uint8_t", static_cast<uint8_t>(e_uint8)},
                {"uint16_t", static_cast<uint8_t>(e_uint16)},
                {"uint32_t", static_cast<uint8_t>(e_uint32)},
                {"uint64_t", static_cast<uint8_t>(e_uint64)},
                {"int8_t", static_cast<uint8_t>(e_int8)},
                {"int16_t", static_cast<uint8_t>(e_int16)},
                {"int32_t", static_cast<uint8_t>(e_int32)},
                {"int64_t", static_cast<uint8_t>(e_int64)},
                {"float", static_cast<uint8_t>(e_float32)},
                {"double", static_cast<uint8_t>(e_float64)},
                {"bool", static_cast<uint8_t>(e_bool)},
                {"char", static_cast<uint8_t>(e_char)}},
      _type_int(0),
      _element_access(nullptr),
      _array_size(0),
      _transformation(nullptr)
{
}

TargetElement::~TargetElement()
{
    delete _transformation;
    _transformation = nullptr;
}

a_util::result::Result TargetElement::create(void* pElementPtr,
                                             const ddl::dd::StructElementAccess& elem_access,
                                             const size_t nArraySize,
                                             const std::string& strElementName)
{
    _element_ptr = pElementPtr;
    _array_size = nArraySize;
    _element_name = strElementName;

    _element_access = elem_access;

    auto data_type = _element_access.getDataType();

    if (elem_access.getEnumType()) {
        if (!data_type) {
            return ERR_UNEXPECTED;
        }
    }

    if (data_type) {
        TypeMap::const_iterator it = _type_map.find(data_type->getName());
        if (it != _type_map.end()) {
            _type_int = it->second;
        }
    }

    return a_util::result::SUCCESS;
}

a_util::result::Result TargetElement::setTransformation(
    const MapTransformationBase* pTransformation)
{
    if (_transformation) {
        delete _transformation;
        _transformation = nullptr;
    }

    if (pTransformation) {
        _transformation = pTransformation->clone();
    }

    return a_util::result::SUCCESS;
}

a_util::result::Result TargetElement::setValue(const void* pData,
                                               uint32_t ui32SrcType,
                                               size_t szMem)
{
    // Scalars (and enums converted to scalars)
    if (_element_access.getDataType()) {
        if (_transformation) {
            switch (ui32SrcType) {
            case e_uint8:
                TransformAndSetValueImpl<uint8_t>(
                    pData, _element_ptr, _array_size, _transformation, _type_int);
                break;
            case e_uint16:
                TransformAndSetValueImpl<uint16_t>(
                    pData, _element_ptr, _array_size, _transformation, _type_int);
                break;
            case e_uint32:
                TransformAndSetValueImpl<uint32_t>(
                    pData, _element_ptr, _array_size, _transformation, _type_int);
                break;
            case e_uint64:
                TransformAndSetValueImpl<uint64_t>(
                    pData, _element_ptr, _array_size, _transformation, _type_int);
                break;
            case e_int8:
                TransformAndSetValueImpl<int8_t>(
                    pData, _element_ptr, _array_size, _transformation, _type_int);
                break;
            case e_int16:
                TransformAndSetValueImpl<int16_t>(
                    pData, _element_ptr, _array_size, _transformation, _type_int);
                break;
            case e_int32:
                TransformAndSetValueImpl<int32_t>(
                    pData, _element_ptr, _array_size, _transformation, _type_int);
                break;
            case e_int64:
                TransformAndSetValueImpl<int64_t>(
                    pData, _element_ptr, _array_size, _transformation, _type_int);
                break;
            case e_float32:
                TransformAndSetValueImpl<float>(
                    pData, _element_ptr, _array_size, _transformation, _type_int);
                break;
            case e_float64:
                TransformAndSetValueImpl<double>(
                    pData, _element_ptr, _array_size, _transformation, _type_int);
                break;
            case e_bool:
                TransformAndSetValueImpl<uint8_t>(
                    pData, _element_ptr, _array_size, _transformation, _type_int);
                break;
            case e_char:
                TransformAndSetValueImpl<char>(
                    pData, _element_ptr, _array_size, _transformation, _type_int);
                break;
            default:
                return ERR_INVALID_TYPE;
            }
        }
        else if (_type_int == ui32SrcType) {
            // Source and target have the same type
            a_util::memory::copy(_element_ptr, szMem, pData, szMem);
        }
        else {
            // Source and target have different types
            switch (ui32SrcType) {
            case e_uint8:
                SetValueImpl<uint8_t>(pData, _element_ptr, _array_size, _type_int);
                break;
            case e_uint16:
                SetValueImpl<uint16_t>(pData, _element_ptr, _array_size, _type_int);
                break;
            case e_uint32:
                SetValueImpl<uint32_t>(pData, _element_ptr, _array_size, _type_int);
                break;
            case e_uint64:
                SetValueImpl<uint64_t>(pData, _element_ptr, _array_size, _type_int);
                break;
            case e_int8:
                SetValueImpl<int8_t>(pData, _element_ptr, _array_size, _type_int);
                break;
            case e_int16:
                SetValueImpl<int16_t>(pData, _element_ptr, _array_size, _type_int);
                break;
            case e_int32:
                SetValueImpl<int32_t>(pData, _element_ptr, _array_size, _type_int);
                break;
            case e_int64:
                SetValueImpl<int64_t>(pData, _element_ptr, _array_size, _type_int);
                break;
            case e_float32:
                SetValueImpl<float>(pData, _element_ptr, _array_size, _type_int);
                break;
            case e_float64:
                SetValueImpl<double>(pData, _element_ptr, _array_size, _type_int);
                break;
            case e_bool:
                SetValueImpl<uint8_t>(pData, _element_ptr, _array_size, _type_int);
                break;
            case e_char:
                SetValueImpl<char>(pData, _element_ptr, _array_size, _type_int);
                break;
            default:
                return ERR_INVALID_TYPE;
            }
        }
    }
    else if (_element_access.getStructType()) {
        // Transformations are not allowed
        a_util::memory::copy(_element_ptr, szMem, pData, szMem);
    }
    else {
        return ERR_INVALID_ARG;
    }

    return a_util::result::SUCCESS;
}

Target* TargetElement::getTarget()
{
    return _target;
}

a_util::result::Result TargetElement::setDefaultValue(const std::string& strDefault)
{
    // Scalars and  arrays
    if (_element_access.getDataType()) {
        double fVal = 0;
        if (!strDefault.empty() && a_util::strings::isDouble(strDefault)) {
            fVal = a_util::strings::toDouble(strDefault);
        }

        for (unsigned int i = 0; i < _array_size; i++) {
            SetCastedValue(_element_ptr, i, &fVal, _type_int);
        }
    }

    // Structures
    if (_element_access.getStructType()) {
        // TODO: What is default value for a structure?
        return ERR_INVALID_ARG;
    }

    return a_util::result::SUCCESS;
}
