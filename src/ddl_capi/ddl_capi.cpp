/**
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

#include <a_util/result.h>
#include <a_util/strings.h>
#include <ddl/ddl.h>
#include <ddl_capi/ddl_capi.h>

#include <algorithm>
#include <memory>
#include <string>

namespace {

_MAKE_RESULT(-10, ERR_INVALID_INDEX);
_MAKE_RESULT(-20, ERR_NOT_FOUND);
_MAKE_RESULT(-22, ERR_FAILED);

template <typename Object>
struct Handle {
    Object* ptr = nullptr;
    std::string last_error;
};

typedef ddl::dd::DataDefinition CAPI_DDL;
class CAPI_DDL_CODEC : public ddl::codec::Codec {
public:
    virtual ~CAPI_DDL_CODEC() = default;
    CAPI_DDL_CODEC(ddl::codec::Codec&& other) : ddl::codec::Codec(std::move(other))
    {
    }
};

template <typename T>
const char* get_last_error(const void* const handle)
{
    const T* _handle = static_cast<const T*>(handle);
    return _handle->last_error.c_str();
}

void allocate_and_fill_cstring(char** const out, const std::string& in)
{
    size_t str_length = in.length() + 1;
    *out = static_cast<char*>(malloc(str_length * sizeof(char)));
    a_util::memory::copy(*out, str_length, in.c_str(), str_length);
}

template <typename T, class F>
void allocate_and_fill_cstring_array(const T& in, char*** const out, F get_string)
{
    uint32_t count = 0;
    *out = (char** const)malloc(in.getSize() * sizeof(char*));
    for (const auto& entry: in) {
        allocate_and_fill_cstring(&(*out)[count++], get_string(entry));
    }
}

} // namespace

A_UTIL_DLL_EXPORT const char* ddl_capi_last_error(const DDL_CAPI_Handle_T ddl)
{
    if (ddl)
        return get_last_error<Handle<CAPI_DDL>>(ddl);
    else
        return "Invalid ddl handle";
}

#define RETURN_ON_ERROR_PUBLIC(exe, ddl)                                                           \
    {                                                                                              \
        auto result = exe;                                                                         \
        if (a_util::result::isFailed(result)) {                                                    \
            ddl->last_error = a_util::result::toString(result);                                    \
            return result.getErrorCode();                                                          \
        }                                                                                          \
    }
#define CREATE_ERROR(error_code, error_message, last_error_instance)                               \
    {                                                                                              \
        auto result =                                                                              \
            a_util::result::Result(error_code, error_message, __LINE__, __FILE__, __func__);       \
        last_error_instance->last_error = a_util::result::toString(result);                        \
    }

A_UTIL_DLL_EXPORT int32_t ddl_capi_load_from_file(const char* const path,
                                                  DDL_CAPI_Handle_T* const ddl)
{
    try {
        auto _ddl = std::make_unique<Handle<CAPI_DDL>>();
        auto dd = std::make_unique<CAPI_DDL>();

        try {
            *dd = ddl::DDFile::fromXMLFile(path);
        }
        catch (const ddl::dd::Error& error) {
            CREATE_ERROR(ERR_FAILED, error.what(), _ddl);
            *ddl = _ddl.release();
            return -1;
        }

        _ddl->ptr = dd.release();
        *ddl = _ddl.release();
        return 0;
    }

    catch (const std::bad_alloc&) {
        *ddl = nullptr;
        return -1;
    }
}

A_UTIL_DLL_EXPORT int32_t ddl_capi_load_from_string(const char* const xml,
                                                    DDL_CAPI_Handle_T* const ddl)
{
    try {
        auto _ddl = std::make_unique<Handle<CAPI_DDL>>();
        auto dd = std::make_unique<CAPI_DDL>();

        if (!xml) {
            CREATE_ERROR(ERR_FAILED, "Invalid xml string", _ddl);
            *ddl = _ddl.release();
            return -1;
        }

        try {
            *dd = ddl::DDString::fromXMLString(xml);
        }
        catch (const ddl::dd::Error& error) {
            CREATE_ERROR(ERR_FAILED, error.what(), _ddl);
            *ddl = _ddl.release();
            return -1;
        }

        _ddl->ptr = dd.release();
        *ddl = _ddl.release();
        return 0;
    }

    catch (const std::bad_alloc&) {
        *ddl = nullptr;
        return -1;
    }
}

A_UTIL_DLL_EXPORT int32_t ddl_capi_free(DDL_CAPI_Handle_T* const ddl)
{
    // check for valid handle
    if (!*ddl) {
        return -1;
    }

    // free handle and data definition
    auto const _ddl = static_cast<Handle<CAPI_DDL>*>(*ddl);
    if (_ddl && _ddl->ptr) {
        delete _ddl->ptr;
        _ddl->ptr = nullptr;
    }
    *ddl = nullptr;
    delete _ddl;

    return 0;
}

A_UTIL_DLL_EXPORT int32_t ddl_capi_ddl_tostring(const DDL_CAPI_Handle_T ddl, char** const xml)
{
    // check for valid handle
    if (!ddl) {
        return -1;
    }

    *xml = nullptr;
    auto const _ddl = static_cast<Handle<CAPI_DDL>*>(ddl);
    allocate_and_fill_cstring(xml, ddl::DDString::toXMLString(*_ddl->ptr));

    return 0;
}

A_UTIL_DLL_EXPORT int32_t ddl_capi_string_free(char** const memory)
{
    free(*memory);
    *memory = nullptr;
    return 0;
}

A_UTIL_DLL_EXPORT int32_t ddl_capi_ddl_tofile(const DDL_CAPI_Handle_T ddl, const char* const path)
{
    // check for valid handle
    if (!ddl) {
        return -1;
    }

    auto const _ddl = static_cast<Handle<CAPI_DDL>*>(ddl);
    try {
        ddl::DDFile::toXMLFile(*_ddl->ptr, path);
    }
    catch (const ddl::dd::Error& error) {
        CREATE_ERROR(ERR_FAILED, error.what(), _ddl);
        return -1;
    }

    return 0;
}

A_UTIL_DLL_EXPORT int32_t ddl_capi_resolve_type(const DDL_CAPI_Handle_T ddl,
                                                const char* const type,
                                                char** const xml)
{
    // check for valid handle
    if (!ddl) {
        return -1;
    }

    // check for requested type
    auto const _ddl = static_cast<Handle<CAPI_DDL>*>(ddl);
    if (!_ddl->ptr->getStructTypes().contains(type)) {
        CREATE_ERROR(ERR_NOT_FOUND, "Struct not found", _ddl);
        return -1;
    }

    allocate_and_fill_cstring(xml, ddl::DDString::toXMLString(type, *_ddl->ptr));

    return 0;
}

A_UTIL_DLL_EXPORT int32_t ddl_capi_structs(const DDL_CAPI_Handle_T ddl,
                                           size_t* const count,
                                           char*** const struct_names)
{
    // check for valid handle
    if (!ddl) {
        return -1;
    }

    // get names of all structs in data definition
    auto const _ddl = static_cast<Handle<CAPI_DDL>*>(ddl);
    const ddl::dd::datamodel::DataDefinition::StructTypes structTypes = _ddl->ptr->getStructTypes();
    *count = structTypes.getSize();
    allocate_and_fill_cstring_array(
        structTypes,
        struct_names,
        [](const std::pair<const std::string, const std::shared_ptr<ddl::dd::datamodel::StructType>>
               structType) { return structType.first; });

    return 0;
}

A_UTIL_DLL_EXPORT int32_t ddl_capi_stringarray_free(const DDL_CAPI_Handle_T ddl,
                                                    size_t count,
                                                    char*** const memory)
{
    // check for valid handle
    if (!ddl) {
        return -1;
    }

    auto const _ddl = static_cast<Handle<CAPI_DDL>*>(ddl);
    if (!*memory) {
        CREATE_ERROR(ERR_FAILED, "Invalid array location", _ddl);
        return -1;
    }

    std::for_each(*memory, *memory + count, [](char* mem) {
        free(mem);
        mem = nullptr;
    });

    free(*memory);
    *memory = nullptr;
    return 0;
}

A_UTIL_DLL_EXPORT int32_t ddl_capi_struct_data(const DDL_CAPI_Handle_T ddl,
                                               const char* const name,
                                               size_t* const alignment)
{
    // check for valid handle
    if (!ddl) {
        return -1;
    }

    // check for requested struct
    auto const _ddl = static_cast<Handle<CAPI_DDL>*>(ddl);
    if (!_ddl->ptr->getStructTypes().contains(name)) {
        CREATE_ERROR(ERR_NOT_FOUND, "Struct not found", _ddl);
        return -1;
    }

    *alignment = _ddl->ptr->getStructTypeAccess(name).getStructType().getAlignment().get();

    return 0;
}

A_UTIL_DLL_EXPORT int32_t ddl_capi_struct_elements(const DDL_CAPI_Handle_T ddl,
                                                   const char* const name,
                                                   size_t* const count,
                                                   char*** const struct_elements)
{
    // check for valid handle
    if (!ddl) {
        return -1;
    }

    // check for requested struct
    auto const _ddl = static_cast<Handle<CAPI_DDL>*>(ddl);
    if (!_ddl->ptr->getStructTypes().contains(name)) {
        CREATE_ERROR(ERR_NOT_FOUND, "Struct not found", _ddl);
        return -1;
    }

    // get names of all elements of a structs in data definition
    const ddl::dd::datamodel::StructType::Elements structElements =
        _ddl->ptr->getStructTypeAccess(name).getStructType().getElements();
    *count = structElements.getSize();
    allocate_and_fill_cstring_array(
        structElements,
        struct_elements,
        [](const std::shared_ptr<ddl::dd::datamodel::StructType::Element> structElement) {
            return structElement->getName();
        });

    return 0;
}

A_UTIL_DLL_EXPORT int32_t ddl_capi_element_data(const DDL_CAPI_Handle_T ddl,
                                                const char* const struct_name,
                                                const char* const element_name,
                                                char** const type_name,
                                                type_t* const typeOfType,
                                                size_t* const arraysize,
                                                size_t* const bytepos,
                                                size_t* const bitpos,
                                                size_t* const alignment,
                                                size_t* const byteorder)
{
    // check for valid handle
    if (!ddl) {
        return -1;
    }

    // check for requested struct
    auto const _ddl = static_cast<Handle<CAPI_DDL>*>(ddl);
    if (!_ddl->ptr->getStructTypes().contains(struct_name)) {
        CREATE_ERROR(ERR_NOT_FOUND, "Struct not found", _ddl);
        return -1;
    }

    // get access to requested struct element in data definition
    const ddl::dd::StructTypeAccess typeAccess = _ddl->ptr->getStructTypeAccess(struct_name);
    if (!typeAccess.getStructType().getElements().contains(element_name)) {
        CREATE_ERROR(ERR_NOT_FOUND, "Element not found", _ddl);
        return -1;
    }

    const ddl::dd::datamodel::StructType::Element element =
        typeAccess.getElementByPath(element_name).getElement();

    // a map to assign internal to external enum representation
    static const std::map<ddl::dd::TypeOfType, type_t> int2ext = {
        {ddl::dd::TypeOfType::data_type, data_type},
        {ddl::dd::TypeOfType::enum_type, enum_type},
        {ddl::dd::TypeOfType::struct_type, struct_type},
    };

    // get details of an element of a struct in data definition
    allocate_and_fill_cstring(type_name, element.getTypeName());
    *typeOfType = int2ext.find(_ddl->ptr->getTypeOfType(*type_name))->second;
    *arraysize = element.getArraySize().getArraySizeValue();
    *bytepos = element.getBytePos().get();
    *bitpos = element.getBitPos().get();
    *alignment = element.getAlignment();
    *byteorder = element.getByteOrder();

    return 0;
}

A_UTIL_DLL_EXPORT int32_t ddl_capi_type_data(const DDL_CAPI_Handle_T ddl,
                                             const char* const type_name,
                                             bool* const predefined,
                                             size_t* const numbits,
                                             size_t* const arraysize)
{
    // check for valid handle
    if (!ddl) {
        return -1;
    }

    // check for requested type
    auto const _ddl = static_cast<Handle<CAPI_DDL>*>(ddl);
    if (!_ddl->ptr->getDataTypes().contains(type_name)) {
        CREATE_ERROR(ERR_NOT_FOUND, "Type not found", _ddl);
        return -1;
    }

    // get predefined data types in data definition
    const ddl::PredefinedDataTypes& predefinedDataTypes = ddl::PredefinedDataTypes::getInstance();
    const ddl::dd::OptionalSize arraySize =
        _ddl->ptr->getDataTypes().access(type_name)->getArraySize();

    // get details of requested data type in data definition
    *predefined = predefinedDataTypes.getPredefinedType(type_name) ? true : false;
    *numbits = _ddl->ptr->getDataTypes().access(type_name)->getBitSize();
    *arraysize = arraySize._valid ? arraySize.get() : 1;

    return 0;
}

A_UTIL_DLL_EXPORT int32_t ddl_capi_enum_data(const DDL_CAPI_Handle_T ddl,
                                             const char* const enum_name,
                                             char** const type,
                                             bool* const predefined,
                                             size_t* const count,
                                             char*** const entry_names,
                                             char*** const entry_values)
{
    // check for valid handle
    if (!ddl) {
        return -1;
    }

    // check for requested enum
    auto const _ddl = static_cast<Handle<CAPI_DDL>*>(ddl);
    if (!_ddl->ptr->getEnumTypes().contains(enum_name)) {
        CREATE_ERROR(ERR_NOT_FOUND, "Enum not found", _ddl);
        return -1;
    }

    allocate_and_fill_cstring(type, _ddl->ptr->getEnumTypes().access(enum_name)->getDataTypeName());

    *predefined = 0; // predefined enums are not available in ddl

    const ddl::dd::datamodel::EnumType::Elements enumElements =
        _ddl->ptr->getEnumTypes().access(enum_name)->getElements();
    *count = enumElements.getSize();

    // get enum identifiers
    allocate_and_fill_cstring_array(
        enumElements,
        entry_names,
        [](const std::pair<const std::string,
                           const std::shared_ptr<ddl::dd::datamodel::KeyValuePair>> enumElement) {
            return enumElement.first;
        });

    // get enum enumerators
    allocate_and_fill_cstring_array(
        enumElements,
        entry_values,
        [](const std::pair<const std::string, std::shared_ptr<ddl::dd::datamodel::KeyValuePair>>
               enumElement) { return enumElement.second->getValue(); });

    return 0;
}

ddl::DataRepresentation convert_c_datastate_into_ddl(const DDL_CAPI_DataRepresentation state)
{
    switch (state) {
    case DDL_CAPI_Data_serialized:
        return ddl::serialized;
    case DDL_CAPI_Data_deserialized:
        return ddl::deserialized;
    default:
        return ddl::deserialized;
    }
}

A_UTIL_DLL_EXPORT const char* ddl_capi_last_codecerror(const DDL_CAPI_Codec_Handle_T ddl)
{
    if (ddl)
        return get_last_error<Handle<CAPI_DDL_CODEC>>(ddl);
    else
        return "Invalid codec handle";
}

A_UTIL_DLL_EXPORT int32_t
ddl_capi_struct_buffer_size(const DDL_CAPI_Handle_T ddl,
                            const char* const struct_name,
                            const DDL_CAPI_DataRepresentation representation,
                            size_t* const data_size)
{
    // check for valid handle
    if (!ddl) {
        return -1;
    }

    // check for requested struct
    auto const _ddl = static_cast<Handle<CAPI_DDL>*>(ddl);
    if (!_ddl->ptr->getStructTypes().contains(struct_name)) {
        CREATE_ERROR(ERR_NOT_FOUND, "Struct not found", _ddl);
        return -1;
    }

    const ddl::codec::CodecFactory codec_factory(_ddl->ptr->getStructTypeAccess(struct_name));
    RETURN_ON_ERROR_PUBLIC(codec_factory.isValid(), _ddl);

    *data_size = codec_factory.getStaticBufferSize(convert_c_datastate_into_ddl(representation));

    return 0;
}

A_UTIL_DLL_EXPORT int32_t ddl_capi_codec_create(const DDL_CAPI_Handle_T ddl,
                                                const char* const struct_name,
                                                void* const data,
                                                const size_t data_size,
                                                const DDL_CAPI_DataRepresentation representation,
                                                DDL_CAPI_Codec_Handle_T* const codec)
{
    // check for valid handle
    if (!ddl) {
        return -1;
    }

    // check for requested struct
    auto const _ddl = static_cast<Handle<CAPI_DDL>*>(ddl);
    if (!_ddl->ptr->getStructTypes().contains(struct_name)) {
        _ddl->last_error = "Struct not found";
        return -1;
    }

    const ddl::codec::CodecFactory codec_factory(_ddl->ptr->getStructTypeAccess(struct_name));
    RETURN_ON_ERROR_PUBLIC(codec_factory.isValid(), _ddl);

    auto const _codec = new Handle<CAPI_DDL_CODEC>();
    *codec = static_cast<DDL_CAPI_Codec_Handle_T>(_codec);

    _codec->ptr = new CAPI_DDL_CODEC(
        codec_factory.makeCodecFor(data, data_size, convert_c_datastate_into_ddl(representation)));

    return 0;
}

A_UTIL_DLL_EXPORT int32_t ddl_capi_codec_free(DDL_CAPI_Codec_Handle_T* const codec)
{
    // check for valid handle
    if (!*codec) {
        return -1;
    }

    // free handle and codec
    auto const _codec = static_cast<Handle<CAPI_DDL_CODEC>*>(*codec);
    if (_codec->ptr) {
        delete _codec->ptr;
        _codec->ptr = nullptr;
    }
    *codec = nullptr;
    delete _codec;

    return 0;
}

namespace {
size_t getIndexForName(ddl::codec::Codec& codec, const char* const name)
{
    auto left_name_to_find = std::string(name);
    size_t found_index = static_cast<size_t>(-1);
    size_t index_counter = 0;
    ddl::codec::forEachLeafElement(
        codec.getElements(),
        [&index_counter, &found_index, &left_name_to_find](auto& current_element) {
            auto current_full_name = current_element.getFullName();
            if (current_full_name.find(left_name_to_find) == 0 &&
                found_index == static_cast<size_t>(-1)) {
                found_index = index_counter;
            }
            ++index_counter;
        });
    return found_index;
}
} // namespace

A_UTIL_DLL_EXPORT int32_t ddl_capi_codec_get_element_index(const DDL_CAPI_Codec_Handle_T codec,
                                                           const char* const element,
                                                           size_t* const index)
{
    // check for valid handle
    if (!codec) {
        return -1;
    }

    // get index of requested element
    auto const codec_instance = static_cast<Handle<CAPI_DDL_CODEC>*>(codec);
    size_t found_index = getIndexForName(*codec_instance->ptr, element);
    if (found_index != static_cast<size_t>(-1)) {
        *index = found_index;
    }
    else {
        CREATE_ERROR(ERR_NOT_FOUND,
                     std::string("Element not found '" + std::string(element) + "'").c_str(),
                     codec_instance);
        return -2;
    }
    return 0;
}

A_UTIL_DLL_EXPORT int32_t ddl_capi_codec_set_element_byIndex(const DDL_CAPI_Codec_Handle_T codec,
                                                             const size_t index,
                                                             const void* const data)
{
    // check for valid handle
    if (!codec) {
        return -1;
    }

    // set value of requested element
    auto const codec_instance = static_cast<Handle<CAPI_DDL_CODEC>*>(codec);
    try {
        auto resolved_index = codec_instance->ptr->resolve(index);
        codec_instance->ptr->setElementRawValue(resolved_index, data, 0);
    }
    catch (const std::runtime_error& error) {
        CREATE_ERROR(ERR_INVALID_INDEX, error.what(), codec_instance);
        return -2;
    }

    return 0;
}

A_UTIL_DLL_EXPORT int32_t ddl_capi_codec_set_element_byName(const DDL_CAPI_Codec_Handle_T codec,
                                                            const char* const element,
                                                            const void* const data)
{
    // check for valid handle
    if (!codec) {
        return -1;
    }

    // set value of requested element
    auto const codec_instance = static_cast<Handle<CAPI_DDL_CODEC>*>(codec);
    try {
        // unsafe call!!
        codec_instance->ptr->getElement(element).setRawValue(data, 0);
    }
    catch (const std::runtime_error& error) {
        CREATE_ERROR(ERR_INVALID_INDEX, error.what(), codec_instance);
        return -2;
    }
    return 0;
}

A_UTIL_DLL_EXPORT int32_t ddl_capi_codec_get_substruct_index(const DDL_CAPI_Codec_Handle_T codec,
                                                             const char* const substruct,
                                                             size_t* const index)
{
    // check for valid handle
    if (!codec) {
        return -1;
    }

    // get index of requested substruct
    auto const codec_instance = static_cast<Handle<CAPI_DDL_CODEC>*>(codec);
    size_t found_index = getIndexForName(*codec_instance->ptr, substruct);
    if (found_index != static_cast<size_t>(-1)) {
        *index = found_index;
    }
    else {
        CREATE_ERROR(ERR_NOT_FOUND,
                     std::string("Element not found '" + std::string(substruct) + "'").c_str(),
                     codec_instance);
        return -2;
    }

    return 0;
}

A_UTIL_DLL_EXPORT int32_t ddl_capi_codec_set_substruct_byIndex(const DDL_CAPI_Codec_Handle_T codec,
                                                               const size_t index,
                                                               const void* const data,
                                                               const size_t data_size)
{
    // check for valid handle
    if (!codec) {
        return -1;
    }

    // check for valid index
    auto const codec_instance = static_cast<Handle<CAPI_DDL_CODEC>*>(codec);
    void* address;
    try {
        auto resolved_index = codec_instance->ptr->resolve(index);
        address = codec_instance->ptr->getElementAddress(resolved_index);
    }
    catch (const std::runtime_error& error) {
        CREATE_ERROR(ERR_INVALID_INDEX, error.what(), codec_instance);
        return -2;
    }

    // set values of requested substruct
    if (!a_util::memory::copy(address, data_size, data, data_size)) {
        CREATE_ERROR(ERR_FAILED, "Copy substruct failed", codec_instance);
        return -2;
    }

    return 0;
}

A_UTIL_DLL_EXPORT int32_t ddl_capi_codec_set_substruct_byName(const DDL_CAPI_Codec_Handle_T codec,
                                                              const char* const substruct,
                                                              const void* const data,
                                                              const size_t data_size)
{
    // check for valid name
    size_t index;
    int32_t res = ddl_capi_codec_get_substruct_index(codec, substruct, &index);
    if (res != 0) {
        return res;
    }

    // set values of requested substruct
    return ddl_capi_codec_set_substruct_byIndex(codec, index, data, data_size);
}

A_UTIL_DLL_EXPORT int32_t ddl_capi_codec_get_array_index(const DDL_CAPI_Codec_Handle_T codec,
                                                         const char* const array,
                                                         size_t* const index)
{
    // check for valid handle
    if (!codec) {
        return -1;
    }

    // get index of requested array
    auto const codec_instance = static_cast<Handle<CAPI_DDL_CODEC>*>(codec);
    std::string array_name(array);
    if (array_name.back() != ']') {
        array_name += "[0]";
    }
    size_t found_index = getIndexForName(*codec_instance->ptr, array_name.c_str());
    if (found_index != static_cast<size_t>(-1)) {
        *index = found_index;
    }
    else {
        CREATE_ERROR(ERR_NOT_FOUND,
                     std::string("Element not found '" + std::string(array) + "'").c_str(),
                     codec_instance);
        return -2;
    }

    return 0;
}

A_UTIL_DLL_EXPORT int32_t ddl_capi_codec_get_array_size(const DDL_CAPI_Codec_Handle_T codec,
                                                        const char* const array,
                                                        size_t* const data_size)
{
    // check for valid handle
    if (!codec) {
        return -1;
    }

    // get size of requested array
    auto const codec_instance = static_cast<Handle<CAPI_DDL_CODEC>*>(codec);
    try {
        *data_size =
            codec_instance->ptr->getElement(array).getIndex().getLayout().deserialized.bit_size / 8;
    }
    catch (std::runtime_error& error) {
        CREATE_ERROR(ERR_INVALID_INDEX, error.what(), codec_instance);
        return -2;
    }
    return 0;
}

A_UTIL_DLL_EXPORT int32_t ddl_capi_codec_set_array_byIndex(const DDL_CAPI_Codec_Handle_T codec,
                                                           const size_t index,
                                                           const void* const data,
                                                           const size_t data_size)
{
    // check for valid handle
    if (!codec) {
        return -1;
    }

    // check for valid index
    auto const codec_instance = static_cast<Handle<CAPI_DDL_CODEC>*>(codec);
    void* address;
    try {
        auto resolved_index = codec_instance->ptr->resolve(index);
        address = codec_instance->ptr->getElementAddress(resolved_index);
    }
    catch (const std::runtime_error& error) {
        CREATE_ERROR(ERR_INVALID_INDEX, error.what(), codec_instance);
        return -2;
    }

    // set values of requested array
    if (!a_util::memory::copy(address, data_size, data, data_size)) {
        CREATE_ERROR(ERR_FAILED, "Copy array failed", codec_instance);
        return -2;
    }

    return 0;
}

A_UTIL_DLL_EXPORT int32_t ddl_capi_codec_set_array_byName(const DDL_CAPI_Codec_Handle_T codec,
                                                          const char* const array,
                                                          const void* const data,
                                                          const size_t data_size)
{
    // check for valid name and get index of requested array
    size_t index;
    int32_t res = ddl_capi_codec_get_array_index(codec, array, &index);
    if (res != 0) {
        return res;
    }

    // set values of requested array
    return ddl_capi_codec_set_array_byIndex(codec, index, data, data_size);
}

A_UTIL_DLL_EXPORT int32_t ddl_capi_codec_get_element_byIndex(const DDL_CAPI_Codec_Handle_T codec,
                                                             size_t index,
                                                             void* const data)
{
    // check for valid handle
    if (!codec) {
        return -1;
    }

    // get index of requested element
    auto const codec_instance = static_cast<Handle<CAPI_DDL_CODEC>*>(codec);
    try {
        auto resolved_index = codec_instance->ptr->resolve(index);
        codec_instance->ptr->getElementRawValue(resolved_index, data, 0);
    }
    catch (const std::runtime_error& error) {
        CREATE_ERROR(ERR_INVALID_INDEX, error.what(), codec_instance);
        return -2;
    }

    return 0;
}

A_UTIL_DLL_EXPORT int32_t ddl_capi_codec_get_element_byName(const DDL_CAPI_Codec_Handle_T codec,
                                                            const char* const element,
                                                            void* const data)
{
    // check for valid handle
    if (!codec) {
        return -1;
    }

    // check for valid name and get values of requested element
    auto const codec_instance = static_cast<Handle<CAPI_DDL_CODEC>*>(codec);
    try {
        codec_instance->ptr->getElement(element).getRawValue(data, 0);
    }
    catch (const std::runtime_error& error) {
        CREATE_ERROR(ERR_INVALID_INDEX, error.what(), codec_instance);
        return -2;
    }

    return 0;
}

A_UTIL_DLL_EXPORT int32_t ddl_capi_codec_get_substruct_byIndex(const DDL_CAPI_Codec_Handle_T codec,
                                                               const size_t index,
                                                               void* const data,
                                                               const size_t data_size)
{
    // check for valid handle
    if (!codec) {
        return -1;
    }

    // check for valid index
    auto const codec_instance = static_cast<Handle<CAPI_DDL_CODEC>*>(codec);
    const void* address;
    try {
        auto resolved_index = codec_instance->ptr->resolve(index);
        address = codec_instance->ptr->getElementAddress(resolved_index);
    }
    catch (const std::runtime_error& error) {
        CREATE_ERROR(ERR_INVALID_INDEX, error.what(), codec_instance);
        return -2;
    }

    // get values of requested substruct
    if (!a_util::memory::copy(data, data_size, address, data_size)) {
        CREATE_ERROR(ERR_FAILED, "Copy substruct failed", codec_instance);
        return -1;
    }

    return 0;
}

A_UTIL_DLL_EXPORT int32_t ddl_capi_codec_get_substruct_byName(const DDL_CAPI_Codec_Handle_T codec,
                                                              const char* const substruct,
                                                              void* const data,
                                                              const size_t data_size)
{
    // check for valid name and get index of requested substruct
    size_t index;
    int32_t res = ddl_capi_codec_get_substruct_index(codec, substruct, &index);
    if (res != 0) {
        return res;
    }

    // get values of requested substruct
    return ddl_capi_codec_get_substruct_byIndex(codec, index, data, data_size);
}

A_UTIL_DLL_EXPORT int32_t ddl_capi_codec_get_array_byIndex(const DDL_CAPI_Codec_Handle_T codec,
                                                           const size_t index,
                                                           void* const data,
                                                           const size_t size)
{
    // check for valid handle
    if (!codec) {
        return -1;
    }

    // check for valid index
    auto const codec_instance = static_cast<Handle<CAPI_DDL_CODEC>*>(codec);
    const void* address;
    try {
        auto resolved_index = codec_instance->ptr->resolve(index);
        address = codec_instance->ptr->getElementAddress(resolved_index);
    }
    catch (const std::runtime_error& error) {
        CREATE_ERROR(ERR_INVALID_INDEX, error.what(), codec_instance);
        return -2;
    }

    // get values of requested array
    if (!a_util::memory::copy(data, size, address, size)) {
        CREATE_ERROR(ERR_FAILED, "Copy array failed", codec_instance);
        return -1;
    }

    return 0;
}

A_UTIL_DLL_EXPORT int32_t ddl_capi_codec_get_array_byName(const DDL_CAPI_Codec_Handle_T codec,
                                                          const char* const array,
                                                          void* const data,
                                                          const size_t size)
{
    // check for valid name and get index of requested array
    size_t index;
    int32_t res = ddl_capi_codec_get_array_index(codec, array, &index);
    if (res != 0) {
        return res;
    }

    // get values of requested array
    return ddl_capi_codec_get_array_byIndex(codec, index, data, size);
}

A_UTIL_DLL_EXPORT int32_t ddl_capi_codec_transform(const DDL_CAPI_Codec_Handle_T dest,
                                                   const DDL_CAPI_Codec_Handle_T source)
{
    // check for valid handles
    if (!dest || !source) {
        return -1;
    }

    // copy all elements from source to destination.
    auto const _dest = static_cast<Handle<CAPI_DDL_CODEC>*>(dest);
    auto const _source = static_cast<Handle<CAPI_DDL_CODEC>*>(source);
    RETURN_ON_ERROR_PUBLIC(ddl::codec::transform(*_source->ptr, *_dest->ptr), _dest);

    return 0;
}
