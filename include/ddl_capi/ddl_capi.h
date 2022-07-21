/**
 * @file
 * Public C-API for Data Description Language
 *
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

    This Source Code Form is subject to the terms of the Mozilla
    Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

If it is not possible or desirable to put the notice in a particular file, then
You may include the notice in a location (such as a LICENSE file in a
relevant directory) where a recipient would be likely to look for such a notice.

You may add additional accurate notices of copyright ownership.
@endverbatim
 */

#ifndef DEV_ESSENTIAL_DDL_CAPI_H_INCLUDED_
#define DEV_ESSENTIAL_DDL_CAPI_H_INCLUDED_

#include <a_util/preprocessor/dll_export.h>

#include <stddef.h>
#include <stdint.h>

#ifndef __cplusplus
#include <stdbool.h>
#else
extern "C" {
#endif

/**
 * A handle for a ddl description.
 * It is optained by calling \p ddl_capi_load_from_file or \p ddl_capi_load_from_string
 * and must be freed using \p ddl_capi_free.
 */
typedef void* DDL_CAPI_Handle_T;

/**
 * A handle for encoding/decoding a specific struct.
 * It is optained by calling \p ddl_capi_codec_create and must be freed using \p
 * ddl_capi_codec_free.
 */
typedef void* DDL_CAPI_Codec_Handle_T;

/**
 * Enum type for the representation of the input data.
 */
typedef enum {
    DDL_CAPI_Data_serialized = 0, /**< value for serialized representation */
    DDL_CAPI_Data_deserialized    /**< value for deserialized representation */
} DDL_CAPI_DataRepresentation;

/**
 * Enum type for the representation of struct element data type.
 */
typedef enum {
    data_type,   /**< value for data type */
    enum_type,   /**< value for enum type */
    struct_type, /**< value for struct type */
} type_t;

//---- helper functions -----
/**
 * Frees a string at \p memory.
 * Must be called after any call to a ddl c-api function that returns a string to clean up memory.
 * @param[in] memory   pointer to string in memory to be freed
 * @return    0 on success.
 */
A_UTIL_DLL_EXPORT int32_t ddl_capi_string_free(char** const memory);

/**
 * Frees a string array at \p memory.
 * Must be called after any call to a ddl c-api function that returns a string array to clean up
 * memory.
 * @param[in] ddl      handle of type DDL_CAPI_Handle
 * @param[in] count    number of elements in string array
 * @param[in] memory   pointer to array location in memory to be freed
 * @return    0 on success.
 */
A_UTIL_DLL_EXPORT int32_t ddl_capi_stringarray_free(const DDL_CAPI_Handle_T ddl,
                                                    size_t count,
                                                    char*** const memory);

/**
 * Returns the last error for the given \p ddl.
 * The returned string is valid until the next function call.
 * @param[in] ddl   handle of type DDL_CAPI_Handle
 * @return    a description of last occured error
 */
A_UTIL_DLL_EXPORT const char* ddl_capi_last_error(const DDL_CAPI_Handle_T ddl);

/**
 * Returns the last error for the given \p codec.
 * The returned string is valid until the next function call.
 * @param[in] ddl   handle of type DDL_CAPI_Codec_Handle
 * @return    a description of last occured error
 */
A_UTIL_DLL_EXPORT const char* ddl_capi_last_codecerror(const DDL_CAPI_Codec_Handle_T ddl);

//---- functions to parse / handle a ddl -----
/**
 * Loads a \p ddl from a file with the given \p path.
 * @param[in]  path   path to a ddl file
 * @param[out] ddl    handle of type DDL_CAPI_Handle
 * @return     0 on success.
 */
A_UTIL_DLL_EXPORT int32_t ddl_capi_load_from_file(const char* path, DDL_CAPI_Handle_T* const ddl);

/**
 * Loads a \p ddl from the \p xml string.
 * @param[in]  xml   xml-string with ddl
 * @param[out] ddl   handle of type DDL_CAPI_Handle
 * @return     0 on success.
 */
A_UTIL_DLL_EXPORT int32_t ddl_capi_load_from_string(const char* xml, DDL_CAPI_Handle_T* const ddl);

/**
 * Frees a \p ddl handle.
 * Must be called after a successful call to function ddl_capi_load_from_XXX to clean up memory.
 * Successful means that ddl_capi_load_from_XXX returns a valid pointer even if return is not 0.
 * @param[in] ddl   handle of type DDL_CAPI_Handle
 * @return    0 on success.
 */
A_UTIL_DLL_EXPORT int32_t ddl_capi_free(DDL_CAPI_Handle_T* const ddl);

/**
 * Returns the \p xml string for a \p ddl handle.
 * The returned string \p xml needs to be cleaned up using \p ddl_capi_string_free after use.
 * @param[in]  ddl   handle of type DDL_CAPI_Handle
 * @param[out] xml   xml-string containig the ddl description
 * @return     0 on success.
 */
A_UTIL_DLL_EXPORT int32_t ddl_capi_ddl_tostring(const DDL_CAPI_Handle_T ddl, char** const xml);

/**
 * Saves a \p ddl at the given file \p path.
 * @param[in] ddl    handle of type DDL_CAPI_Handle
 * @param[in] path   path to a ddl file
 * @return    0 on success.
 */
A_UTIL_DLL_EXPORT int32_t ddl_capi_ddl_tofile(const DDL_CAPI_Handle_T ddl, const char* path);

/**
 * Retrieve all \p structs_names defined in the \p ddl.
 * The returned string array \p struct_names needs to be cleaned up using \p
 * ddl_capi_stringarray_free after use.
 * @param[in]  ddl            handle of type DDL_CAPI_Handle
 * @param[out] count          number of structs
 * @param[out] struct_names   a string array containing all struct names
 * @return     0 on success.
 */
A_UTIL_DLL_EXPORT int32_t ddl_capi_structs(const DDL_CAPI_Handle_T ddl,
                                           size_t* const count,
                                           char*** const struct_names);

/**
 * Retrieve the alignment for a given struct \p name.
 * For the documentation of the data please have a look in the ddl documentation.
 * @param[in]  ddl         handle of type DDL_CAPI_Handle
 * @param[in]  name        name of the struct
 * @param[out] alignment   the alignment of the struct
 * @return     0 on success.
 */
A_UTIL_DLL_EXPORT int32_t ddl_capi_struct_data(const DDL_CAPI_Handle_T ddl,
                                               const char* name,
                                               size_t* const alignment);

/**
 * Retrieve all \p struct_elements names for a given struct \p name.
 * The returned string array \p struct_elements needs to be cleaned up using \p
 * ddl_capi_stringarray_free after use.
 * @param[in]  ddl               handle of type DDL_CAPI_Handle
 * @param[in]  name              name of the struct
 * @param[out] count             number of elements in the structs
 * @param[out] struct_elements   string array containing all struct element names (fields)
 * @return     0 on success.
 */
A_UTIL_DLL_EXPORT int32_t ddl_capi_struct_elements(const DDL_CAPI_Handle_T ddl,
                                                   const char* name,
                                                   size_t* const count,
                                                   char*** const struct_elements);

/**
 * Retrieve data for a given element \p element_name of struct \p struct_name.
 * For the documentation of the data please have a look in the ddl documentation.
 * If \p arraysize is negative then the array is dynamic.
 * The returned string \p type needs to be cleaned up using \p ddl_capi_string_free after use.
 * @param[in]  ddl            handle of type DDL_CAPI_Handle
 * @param[in]  struct_name    name of the struct
 * @param[in]  element_name   name of the struct element
 * @param[out] type_name      string containing the type name of the element
 * @param[out] typeOfType     the underlaying type of element type_name
 * @param[out] arraysize      array size of the element
 * @param[out] bytepos        the byte position of the element
 * @param[out] bitpos         the bit position of the element
 * @param[out] alignment      the alignment of the element
 * @param[out] byteorder      the byte order of the element
 * @return     0 on success.
 */
A_UTIL_DLL_EXPORT int32_t ddl_capi_element_data(const DDL_CAPI_Handle_T ddl,
                                                const char* struct_name,
                                                const char* element_name,
                                                char** const type_name,
                                                type_t* const typeOfType,
                                                size_t* const arraysize,
                                                size_t* const bytepos,
                                                size_t* const bitpos,
                                                size_t* const alignment,
                                                size_t* const byteorder);

/**
 * Retrieve data for a given type \p type_name.
 * \p arraysize is dynamic if negativ.
 * For the documentation of the data please have a look in the ddl documentation.
 * @param[in]  ddl          handle of type DDL_CAPI_Handle
 * @param[in]  type_name    type name
 * @param[out] predefined   whether the type is predefined
 * @param[out] numbits      the number of bits the type consists of
 * @param[out] arraysize    the array size of the type
 * @return     0 on success.
 */
A_UTIL_DLL_EXPORT int32_t ddl_capi_type_data(const DDL_CAPI_Handle_T ddl,
                                             const char* type_name,
                                             bool* const predefined,
                                             size_t* const numbits,
                                             size_t* const arraysize);

/**
 * Retrieve data for a given enum \p enum_name.
 * For the documentation of the data please have a look in the ddl documentation.
 * The returned string \p type needs to be cleaned up using \p ddl_capi_string_free after use.
 * The returned string arrays \p entry_names and \p entry_values of size \p count need to be cleaned
 * up using \p ddl_capi_stringarray_free after use.
 * @param[in]  ddl            handle of type DDL_CAPI_Handle
 * @param[in]  enum_name      enum name
 * @param[out] type           name of the underlying type
 * @param[out] predefined     whether the enum is predefined
 * @param[out] count          number of enum elements
 * @param[out] entry_names    a string array containing the names of all enum elements
 * @param[out] entry_values   a string array containing the values of all enum elements
 * @return     0 on success.
 */
A_UTIL_DLL_EXPORT int32_t ddl_capi_enum_data(const DDL_CAPI_Handle_T ddl,
                                             const char* enum_name,
                                             char** const type,
                                             bool* const predefined,
                                             size_t* const count,
                                             char*** const entry_names,
                                             char*** const entry_values);

//---- functions to modify ddl -----
/**
 * Retrieve a minimalistic DDL that only resolves the specified \p type.
 * The returned string \p xml needs to be cleaned up using \p ddl_capi_string_free after use.
 * @param[in] ddl    handle of type DDL_CAPI_Handle
 * @param[in] type   the type that the new ddl resolves
 * @param[out] xml   an xml-string containing the new ddl
 * @return    0 on success.
 */
A_UTIL_DLL_EXPORT int32_t ddl_capi_resolve_type(const DDL_CAPI_Handle_T ddl,
                                                const char* type,
                                                char** const xml);

//---- functions to code / decode data -----
/**
 * Returns the \p data_size of \p struct_name in the specified \p representation.
 * @param[in]  ddl              handle of type DDL_CAPI_Handle
 * @param[in]  struct_name      name of the struct to en-/decode
 * @param[in]  representation   representation of the data
 * @param[out] data_size        the size of an instance of the struct in specified representation
 * @return     0 on success.
 */
A_UTIL_DLL_EXPORT int32_t
ddl_capi_struct_buffer_size(const DDL_CAPI_Handle_T ddl,
                            const char* struct_name,
                            const DDL_CAPI_DataRepresentation representation,
                            size_t* const data_size);

/**
 * Creates a \p codec for the given \p struct_name. The returned handle \p codec must be freed with
 * \p ddl_capi_codec_free. \p data must be valid until the \p codec handle is freed. \p data_size
 * must be large enough to hold the struct data (\see ddl_capi_struct_buffer_size)
 * @param[in]  ddl              handle of type DDL_CAPI_Handle
 * @param[in]  struct_name      name of the struct to en-/decode
 * @param[in]  data             pointer to an allocated memory region of size \p data_size to use
 * @param[in]  data_size        size of the memory region
 * @param[in]  representation   representation of the data
 * @param[out] codec            a handle of type DDL_CAPI_Codec_Handle
 * @return     0 on success.
 */
A_UTIL_DLL_EXPORT int32_t ddl_capi_codec_create(const DDL_CAPI_Handle_T ddl,
                                                const char* struct_name,
                                                void* const data,
                                                const size_t data_size,
                                                const DDL_CAPI_DataRepresentation representation,
                                                DDL_CAPI_Codec_Handle_T* const codec);

/**
 * Frees a \p codec handle.
 * Must be called after a successful call to function ddl_capi_codec_create to clean up memory.
 * Successful means that ddl_capi_codec_create returns a valid pointer even if return is not 0.
 * @param[in] codec   a handle of type DDL_CAPI_Codec_Handle
 * @return    0 on success.
 */
A_UTIL_DLL_EXPORT int32_t ddl_capi_codec_free(DDL_CAPI_Codec_Handle_T* const codec);

/**
 * Gets the \p index of the specified \p element.
 * @param[in]  codec     a handle of type DDL_CAPI_Codec_Handle
 * @param[in]  element   path to the element relative to the \p codec
 * @param[out] index     index of that element
 * @return     0 on success.
 */
A_UTIL_DLL_EXPORT int32_t ddl_capi_codec_get_element_index(const DDL_CAPI_Codec_Handle_T codec,
                                                           const char* element,
                                                           size_t* const index);

/**
 * Sets the \p data of the element specified by \p index.
 * \p data must be a pointer to memory with at least the same size as the element specified by the
 * index.
 * @param[in] codec   a handle of type DDL_CAPI_Codec_Handle
 * @param[in] index   index of the element
 * @param[in] data    pointer to the data the element should be set to
 * @return    0 on success.
 */
A_UTIL_DLL_EXPORT int32_t ddl_capi_codec_set_element_byIndex(const DDL_CAPI_Codec_Handle_T codec,
                                                             const size_t index,
                                                             const void* data);

/**
 * Sets the \p data of \p element.
 * @param[in] codec     a handle of type DDL_CAPI_Codec_Handle
 * @param[in] element   path to the element relative to the \p codec
 * @param[in] data      pointer to the data the element should be set to
 * @return    0 on success.
 */
A_UTIL_DLL_EXPORT int32_t ddl_capi_codec_set_element_byName(const DDL_CAPI_Codec_Handle_T codec,
                                                            const char* element,
                                                            const void* data);

/**
 * Gets the \p index of the specified \p substruct
 * @param[in]  codec       a handle of type DDL_CAPI_Codec_Handle
 * @param[in]  substruct   path to the sub-struct relative to the \p codec
 * @param[out] index       index of the sub-struct
 * @return     0 on success.
 */
A_UTIL_DLL_EXPORT int32_t ddl_capi_codec_get_substruct_index(const DDL_CAPI_Codec_Handle_T codec,
                                                             const char* substruct,
                                                             size_t* const index);

/**
 * Sets the \p data of the sub-struct specified by \p index.
 * \p data must be in the format and representation specified by \p codec.
 * @param[in] codec       a handle of type DDL_CAPI_Codec_Handle
 * @param[in] index       index of the sub-struct
 * @param[in] data        pointer to the data the sub-struct should be set to
 * @param[in] data_size   size of \p data in byte
 * @return    0 on success.
 */
A_UTIL_DLL_EXPORT int32_t ddl_capi_codec_set_substruct_byIndex(const DDL_CAPI_Codec_Handle_T codec,
                                                               const size_t index,
                                                               const void* data,
                                                               const size_t data_size);

/**
 * Sets the \p data of \p substruct.
 * \p data must be in the format and representation specified by \p codec.
 * @param[in] codec       a handle of type DDL_CAPI_Codec_Handle
 * @param[in] substruct   path to the sub-struct relative to the \p codec
 * @param[in] data        pointer to the data the sub-struct should be set to
 * @param[in] data_size   size of \p data in byte
 * @return    0 on success.
 */
A_UTIL_DLL_EXPORT int32_t ddl_capi_codec_set_substruct_byName(const DDL_CAPI_Codec_Handle_T codec,
                                                              const char* substruct,
                                                              const void* data,
                                                              const size_t data_size);

/**
 * Gets the \p index of the specified \p array
 * @param[in]  codec   a handle of type DDL_CAPI_Codec_Handle
 * @param[in]  array   path to the array relative to the \p codec
 * @param[out] index   index of the array
 * @return     0 on success.
 */
A_UTIL_DLL_EXPORT int32_t ddl_capi_codec_get_array_index(const DDL_CAPI_Codec_Handle_T codec,
                                                         const char* array,
                                                         size_t* const index);

/**
 * Gets the \p size of the specified \p array
 * @param[in]  codec       a handle of type DDL_CAPI_Codec_Handle
 * @param[in]  array       path to the array relative to the \p codec
 * @param[out] data_size   the byte size of the given array
 * @return     0 on success.
 */
A_UTIL_DLL_EXPORT int32_t ddl_capi_codec_get_array_size(const DDL_CAPI_Codec_Handle_T codec,
                                                        const char* array,
                                                        size_t* const data_size);

/**
 * Stores \p data into the array specified by \p index.
 * \p data must be in the format and representation specified by \p codec.
 * Additionally \p data_size must be the size of the array in the format and representation
 * specified by \p codec.
 * @param[in] codec       a handle of type DDL_CAPI_Codec_Handle
 * @param[in] index       index of the array
 * @param[in] data        pointer to the data the array should be set to
 * @param[in] data_size   size of \p data in bytes
 * @return    0 on success.
 */
A_UTIL_DLL_EXPORT int32_t ddl_capi_codec_set_array_byIndex(const DDL_CAPI_Codec_Handle_T codec,
                                                           const size_t index,
                                                           const void* data,
                                                           const size_t data_size);

/**
 * Writes \p data to the location of \p array.
 * \p data must be in the format and representation specified by \p codec.
 * Additionally \p data_size must be the size of the array in the format and representation
 * specified by \p codec.
 * @param[in] codec       a handle of type DDL_CAPI_Codec_Handle
 * @param[in] array       path to the array relative to the \p codec
 * @param[in] data        pointer to the data the array should be set to
 * @param[in] data_size   size of \p data in bytes
 * @return    0 on success.
 */
A_UTIL_DLL_EXPORT int32_t ddl_capi_codec_set_array_byName(const DDL_CAPI_Codec_Handle_T codec,
                                                          const char* array,
                                                          const void* data,
                                                          const size_t data_size);

/**
 * Gets the \p data of the element specified by \p index.
 * \p data must be big enough to store the element.
 * @param[in] codec   a handle of type DDL_CAPI_Codec_Handle
 * @param[in] index   index specifying the elemenet
 * @param[in] data    pointer to the memory where the element data should be stored
 * @return    0 on success.
 */
A_UTIL_DLL_EXPORT int32_t ddl_capi_codec_get_element_byIndex(const DDL_CAPI_Codec_Handle_T codec,
                                                             size_t index,
                                                             void* const data);

/**
 * Gets the \p data of \p element.
 * \p data must be big enough to store the element.
 * @param[in] codec     a handle of type DDL_CAPI_Codec_Handle
 * @param[in] element   path to the element relative to the \p codec
 * @param[in] data      pointer to the memory where the element data should be stored
 * @return    0 on success.
 */
A_UTIL_DLL_EXPORT int32_t ddl_capi_codec_get_element_byName(const DDL_CAPI_Codec_Handle_T codec,
                                                            const char* element,
                                                            void* const data);

/**
 * Copies the substruct specified by \p index into \p data in the format and representation
 * specified by \p codec. \p data_size bytes are copied into \p data which must be big enough.
 * Additionally \p data_size must be the size of the substruct in the format and representation
 * specified by \p codec.
 * @param[in] codec       a handle of type DDL_CAPI_Codec_Handle
 * @param[in] index       index specifying the sub-struct
 * @param[in] data        pointer to the memory where the sub-struct data should be stored
 * @param[in] data_size   size of the sub-struct in bytes
 * @return    0 on success.
 */
A_UTIL_DLL_EXPORT int32_t ddl_capi_codec_get_substruct_byIndex(const DDL_CAPI_Codec_Handle_T codec,
                                                               const size_t index,
                                                               void* const data,
                                                               const size_t data_size);

/**
 * Copies the \p substruct into \p data in the format and representation specified by \p codec.
 * \p data_size bytes are copied into \p data which must be big enough.
 * Additionally \p data_size must be the size of the substruct in the format and representation
 * specified by \p codec.
 * @param[in] codec       a handle of type DDL_CAPI_Codec_Handle
 * @param[in] substruct   path to the sub-struct relative to the \p codec
 * @param[in] data        pointer to the memory where the sub-struct data should be stored
 * @param[in] data_size   size of the sub-struct in bytes
 * @return    0 on success.
 */
A_UTIL_DLL_EXPORT int32_t ddl_capi_codec_get_substruct_byName(const DDL_CAPI_Codec_Handle_T codec,
                                                              const char* substruct,
                                                              void* const data,
                                                              const size_t data_size);

/**
 * Copies the array specified by \p index into \p data in the format and representation specified by
 * \p codec. \p data_size bytes are copied into \p data which must be big enough. Additionally
 * \p data_size must be the size of the array in the format and representation specified by \p
 * codec.
 * @param[in] codec   a handle of type DDL_CAPI_Codec_Handle
 * @param[in] index   index specifying the array
 * @param[in] data    pointer to the memory where the array data should be stored
 * @param[in] size    size of the array in bytes
 * @return    0 on success.
 */
A_UTIL_DLL_EXPORT int32_t ddl_capi_codec_get_array_byIndex(const DDL_CAPI_Codec_Handle_T codec,
                                                           const size_t index,
                                                           void* const data,
                                                           const size_t size);

/**
 * Copies the \p array into \p data in the format and representation specified by \p codec.
 * \p data_size bytes are copied into \p data which must be big enough.
 * Additionally \p data_size must be the size of the array in the format and representation
 * specified by \p codec.
 * @param[in] codec   a handle of type DDL_CAPI_Codec_Handle
 * @param[in] array   path to the array relative to the \p codec
 * @param[in] data    pointer to the memory where the array data should be stored
 * @param[in] size    size of the array in bytes
 * @return    0 on success.
 */
A_UTIL_DLL_EXPORT int32_t ddl_capi_codec_get_array_byName(const DDL_CAPI_Codec_Handle_T codec,
                                                          const char* array,
                                                          void* const data,
                                                          const size_t size);

/**
 * Writes all elements from \p source into \p destination.
 * @param[in] dest     a handle of type DDL_CAPI_Codec_Handle from which the data is read
 * @param[in] source   a handle of type DDL_CAPI_Codec_Handle to which the data is written
 * @return    0 on success.
 */
A_UTIL_DLL_EXPORT int32_t ddl_capi_codec_transform(const DDL_CAPI_Codec_Handle_T dest,
                                                   const DDL_CAPI_Codec_Handle_T source);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // DEV_ESSENTIAL_DDL_CAPI_H_INCLUDED_
