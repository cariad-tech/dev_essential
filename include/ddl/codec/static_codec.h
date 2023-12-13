/**
 * @file
 * Implementation of the StaticCodec and StaticDecoder.
 *
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */

#ifndef DDL_STATIC_CODEC_CLASS_HEADER
#define DDL_STATIC_CODEC_CLASS_HEADER

#include <a_util/memory.h>
#include <a_util/result.h>
#include <ddl/codec/codec_index.h>
#include <ddl/codec/codec_iterator.h>
#include <ddl/codec/data_representation.h>
#include <ddl/codec/legacy/access_element_legacy.h>
#include <ddl/codec/value_access.h>

#include <functional>
namespace ddl {
namespace codec {

// forward declarations
class ElementAccessor;
class StructAccess;
class CodecFactory;

/**
 * Decoder for static structures defined by a DataDefinition definition.
 */
class StaticDecoder {
public:
    /// DTOR
    ~StaticDecoder() = default;
    /**
     * Default constructor
     */
    StaticDecoder();
    /**
     * Noncopyable
     */
    StaticDecoder(const StaticDecoder&) = delete;
    /**
     * Noncopyable
     */
    StaticDecoder& operator=(const StaticDecoder&) = delete;
    /**
     * Move constructor.
     */
    StaticDecoder(StaticDecoder&&);
    /**
     * Move assignment operator.
     */
    StaticDecoder& operator=(StaticDecoder&&);
    /**
     * @return Whether or not the decoder is valid.
     * @retval ERR_INVALID_ARG The passed data is not large enough.
     */
    virtual a_util::result::Result isValid() const;
    /**
     * Static decoder element.
     */
    typedef DecoderElement<DecoderElementAccess<const StaticDecoder>> Element;
    /**
     * Retrieves an element instance based on the codec index of that element.
     * @param[in] codec_index The codec index.
     * @return The Codec Index of the DecoderElement.
     * @remark The element is only valid as long as the static decoder exists.
     * @throw throws std::runtime_error if not found.
     */
    Element getElement(const CodecIndex& codec_index) const;
    /**
     * Retrieves an element instance based on the full name of the element.
     * @param[in] full_element_name The full name of the element in point notation (i.e. \p
     * "element1.child_element[4].element_value").
     *
     * By using an empty string the deserialized decoding information within CodecIndex will
     * retrieve the buffers first address and whole buffersize. By refering an array without
     * dedicated array index position the deserialized decoding information within CodecIndex will
     * be set to the arrays first address and the whole array size. By refering an structured value
     * (not a leaf value) the deserialized decoding information within CodecIndex will be set to the
     * structs first address and the whole struct size.
     *
     * @return The Codec Index of the DecoderElement.
     * @remark The element is only valid as long as the static decoder exists.
     * @throw throws std::runtime_error if not found.
     */
    Element getElement(const std::string& full_element_name) const;
    /**
     * Iterator container to iterate all elements of the static decoder.
     * @see @ref forEachLeafElement, @ref forEachElement
     * @see @ref ddl::codec::ChildElements, @ref StaticDecoder::Element
     */
    typedef ChildElements<DecoderElementAccess<const StaticDecoder>> Elements;
    /**
     * Retrieves the elements of the static decoder to get element information.
     * @return Elements The elements iterator container.
     * @see @ref forEachLeafElement, @ref forEachElement, @ref StaticDecoder::Element, @ref
     * CodecIndex
     */
    const Elements& getElements() const;
    /**
     * Retrieves the amount of child elements for the given codec index.
     * @param[in] codec_index The codec index of the element.
     * @retval 0 has no children. May be a leaf element.
     * @retval >0 has children. May be a structured element.
     * @throw throws std::runtime_error if not found.
     */
    size_t getElementChildCount(const CodecIndex& codec_index = CodecIndex()) const;
    /**
     * @brief Retrieves the full name of the element within its main structure.
     *
     * If the element is an array you get the elements array name representation:
     * "main_element.element_name[0]" or main_element.element_name[1] ... etc. If the element is not
     * an array you get the elements name representation: "main_element.element_name".
     *
     * @param[in] codec_index The codec index of the element.
     * @return The full name  of the element in array representation depending on array size.
     * @throw throws std::runtime_error if not found.
     */
    std::string getElementFullName(const CodecIndex& codec_index) const;
    /**
     * @brief Retrieves the name of the element within its structured element.
     *
     * If the element is an array you get the elements array name representation: "element_name[0]"
     * or element_name[1] ... etc. If the element is not an array you get the elements name
     * representation: "element_name".
     *
     * @param[in] codec_index The codec index of the element.
     * @return The name of the element in array representation depending on array size.
     * @throw throws std::runtime_error if not found.
     */
    std::string getElementName(const CodecIndex& codec_index) const;
    /**
     * @brief Retrieves the base name of the element within its structured element.
     *
     * If the element is an array you get the elements base name representation : "element_name"
     * for each array element. If the element is not an array you get the elements name
     * representation is also the base name representation: "element_name" .
     *
     * @param[in] codec_index The codec index of the element.
     * @return The base name of the element.
     * @throw throws std::runtime_error if not found.
     */
    const std::string& getElementBaseName(const CodecIndex& codec_index) const;
    /**
     * Returns the current value of the given element as a T if supported.
     * @param[in] codec_index The index of the element.
     * @tparam T The type of the value to retrieve.
     * @return Returns the current value as T if supported.
     * @throw throws std::runtime_error if not found.
     */
    template <typename T>
    T getElementValue(const CodecIndex& codec_index) const
    {
        return ValueGetterSelect<StaticDecoder, T>::getValue(*this, codec_index);
    }
    /**
     * Returns the current value of the given element as a T if supported.
     * @param[in] leaf_codec_index The small leaf codec index of the element.
     * @tparam T The type of the value to retrieve.
     * @return Returns the current value as T if supported.
     * @throws std::runtime_error if there is no valid conversion or the given @p leaf_codec_index
     * is invalid
     */
    template <typename T>
    T getElementValue(const LeafCodecIndex& leaf_codec_index) const
    {
        return LeafValueGetter<T>::getValue(getData(), getDataSize(), leaf_codec_index.getLayout());
    }
    /**
     * Returns the current value of the given element as a variant.
     * @param[in] codec_index The index of the element.
     * @return a_util::variant::Variant Variant to return the current value to, type will be set.
     * @throw throws std::runtime_error if not found.
     */
    a_util::variant::Variant getElementVariantValue(const CodecIndex& codec_index) const;
    /**
     * Returns the current value of the given element as a variant.
     * @param[in] codec_index The index of the element.
     * @param[out] value Variant to return the current value to, type will be set.
     * @throw throws std::runtime_error if not found.
     */
    void getElementVariantValue(const CodecIndex& codec_index,
                                a_util::variant::Variant& value) const;
    /**
     * Returns the current value of the given element as a string.
     * If this element is a enum type the enum types element name will be set.
     * @param[in] codec_index The index of the element.
     * @return std::string String to return the current value to, if enum, enum value name will be
     * set.
     * @throw throws std::runtime_error if not found.
     */
    std::string getElementStringValue(const CodecIndex& codec_index) const;
    /**
     * Returns the current value of the given element as a variant.
     * @param[in] index The index of the element.
     * @param[out] value The will be set to the current value.
     * @param[in] value_size Size of the memory in \p value (if set to 0: unsafe usage of \p value
     * without size check)
     * @throw throws std::runtime_error if not found.
     */
    void getElementRawValue(const CodecIndex& index, void* value, size_t value_size) const;
    /**
     * @param[in] index The index of the element.
     * @return A pointer to the element or nullptr in case of an error.
     */
    const void* getElementAddress(const CodecIndex& index) const;
    /**
     * @brief Gets the static buffer size in bytes for the main structure.
     * @return The size in bytes of the structure in the current data representation (@ref
     * getRepresentation).
     */
    size_t getStaticBufferSize() const;
    /**
     * @brief Gets the static buffer size in bytes for the main structure.
     * @param[in] rep The data representation for which the buffer size should be returned.
     * @return The size in bytes of the structure in the requested data representation.
     */
    size_t getStaticBufferSize(DataRepresentation rep) const;
    /**
     * Returns the current data representation the decoder decodes from.
     * @return The data representation which this decoder handles.
     */
    DataRepresentation getRepresentation() const;

public: // legacy
    /**
     * @param[in] leaf_index The leaf index of the element.
     * @return The Codec Index of the element.
     * @throw throws std::runtime_error if not found.
     */
    CodecIndex resolve(size_t leaf_index) const;
    /**
     * is the leaf count
     * @return The amount of leaf elements contained in the data structure.
     */
    size_t getElementCount() const;
    /**
     * Returns the StructElement of the element with given leaf index.
     * @param[in] leaf_index The leaf index of the element.
     * @param[out] legacy_struct_element the struct element legacy description.
     * @return The amount of static elements contained in the handled structure.
     * @remark This is for legacy access only!
     */
    a_util::result::Result getElement(size_t leaf_index,
                                      const ddl::StructElement*& legacy_struct_element) const;

    /**
     * The codec index will be resolved for fast access (layout will be set)
     * @param[in] codec_index The codec index to resolve.
     * @throw throws std::runtime_error if not found.
     */
    void resolve(CodecIndex& codec_index) const;

    /**
     * @brief Get the current set data
     * @remark For performance issues this will not check any validity!
     * @return const void* The data pointer
     */
    const void* getData() const noexcept;
    /**
     * @brief Get the current size (in bytes) of the data.
     * @remark For performance issues this will not check any validity!
     * @return size_t
     */
    size_t getDataSize() const noexcept;

protected:
    friend class CodecFactory;

    /// For internal use only. @internal
    StaticDecoder(std::shared_ptr<const StructAccess> codec_access,
                  const void* data,
                  size_t data_size,
                  DataRepresentation representation);

protected:
    /// For internal use only. @internal
    std::shared_ptr<const StructAccess> _codec_access;
    /// For internal use only. @internal
    const void* _data = nullptr;
    /// For internal use only. @internal
    size_t _data_size = 0;
    /// For internal use only. @internal
    const ddl::codec::ElementAccessor* _element_accessor;

private:
    /// For internal use only. @internal
    ddl::access_element::StructElementLegacy<const StaticDecoder> _legacy_element;
    Element _first_element;
};

/**
 * Codec for static structures defined by a DataDefinition definition.
 */
class StaticCodec final : public StaticDecoder {
public:
    /**
     * Default constructor
     */
    StaticCodec();
    /**
     * Noncopyable
     */
    StaticCodec(const StaticCodec&) = delete;

    /**
     * Noncopyable
     */
    StaticCodec& operator=(const StaticCodec&) = delete;

    /**
     * Move constructor.
     */
    StaticCodec(StaticCodec&&);

    /**
     * Move assignment operator.
     */
    StaticCodec& operator=(StaticCodec&&);

    using StaticDecoder::getElement;
    using StaticDecoder::getElementAddress;
    /**
     * Static codec element.
     */
    typedef CodecElement<CodecElementAccess<StaticCodec>> Element;
    /**
     * Retrieves an element instance based on the codec index of that element.
     * @param[in] codec_index The codec index.
     * @return The Codec Index of the CodecElement.
     * @remark The element is only valid as long as the static codec exists.
     * @throw throws std::runtime_error if not found.
     */
    Element getElement(const CodecIndex& codec_index);
    /**
     * Retrieves an element instance based on the full name of the element.
     * @param[in] full_element_name The full name of the element in point notation (i.e. \p
     * "element1.child_element[4].element_value").
     *
     * By using an empty string the deserialized decoding information within CodecIndex will
     * retrieve the buffers first address and whole buffersize. By refering an array without
     * dedicated array index position the deserialized decoding information within CodecIndex will
     * be set to the arrays first address and the whole array size. By refering an structured value
     * (not a leaf value) the deserialized decoding information within CodecIndex will be set to the
     * structs first address and the whole struct size.
     *
     * @return The Codec Index of the CodecElement.
     * @remark The element is only valid as long as the static codec exists.
     * @throw throws std::runtime_error if not found.
     */
    Element getElement(const std::string& full_element_name);

    using StaticDecoder::getElements;
    /**
     * Iterator container to iterate all elements of the static decoder.
     * @see @ref forEachLeafElement, @ref forEachElement
     * @see @ref ddl::codec::ChildElements, @ref StaticCodec::Element
     */
    typedef ChildElements<CodecElementAccess<StaticCodec>> Elements;
    /**
     * Retrieves the elements of the static codec to get element information.
     * @return Elements The elements iterator container.
     * @see @ref forEachLeafElement, @ref forEachElement, @ref StaticCodec::Element, @ref CodecIndex
     */
    Elements& getElements();

    /**
     * Sets the current value of the given element from given \p value in type T.
     * This will cast the given value to the elements data type!
     * @param[in] codec_index The index of the element.
     * @param[in] value The value to set.
     * @tparam T Type of the value to set
     * @throw throws std::runtime_error if not found.
     */
    template <typename T>
    void setElementValue(const CodecIndex& codec_index, const T& value)
    {
        ValueSetterSelect<StaticCodec, T>::setValue(*this, codec_index, value);
    }
    /**
     * Sets the current value of the given element from given \p value in type T.
     * This will cast the given value to the elements data type!
     * @param[in] leaf_codec_index The leaf codec index of the element.
     * @param[in] value The value to set.
     * @tparam T Type of the value to set
     * @throws std::runtime_error if there is no valid conversion or the given @p leaf_codec_index
     * is invalid
     */
    template <typename T>
    void setElementValue(const LeafCodecIndex& leaf_codec_index, const T& value)
    {
        LeafValueSetter<T>::setValue(getData(), getDataSize(), leaf_codec_index.getLayout(), value);
    }
    /**
     * Sets the current value of the given element from the given variant.
     * If this element is a enum type the enum types element name can be used in \p value.
     *     Otherwise the string value is automatically converted to the elments data type, if
     * possible.
     * @param[in] codec_index The index of the element.
     * @param[in] value The value to set.
     * @throw throws std::runtime_error if not found.
     */
    void setElementVariantValue(const CodecIndex& codec_index,
                                const a_util::variant::Variant& value);
    /**
     * Sets the current value of the given element from the given string.
     * This will cast the given value to the elements data type!
     * @param[in] codec_index The index of the element.
     * @param[in] value The value to set.
     * @throw throws std::runtime_error if not found.
     */
    void setElementStringValue(const CodecIndex& codec_index, const std::string& value);
    /**
     * Sets the current value of the given element by copying its data
     * from the passed-in location.
     * @param[in] codec_index The codec index of the element.
     * @param[in] value The location where the data should be copied from.
     * @param[in] value_size Size of the memory in \p value (if set to 0: unsafe usage of \p value
     * without size check)
     * @throw throws std::runtime_error if not found or value_size is lower then elements value
     * memory to copy
     */
    void setElementRawValue(const CodecIndex& codec_index, const void* value, size_t value_size);
    /**
     * @param[in] codec_index The index of the element.
     * @return A pointer to the element
     * @throw throws std::runtime_error if not found.
     */
    void* getElementAddress(const CodecIndex& codec_index);
    /**
     * Sets all elements to their default values, constant values defined in the data definition or
     * zero if \p zero_values is set.
     * @param[in] zero_values set the value to 0 if no constant or default value set in data
     * definition.
     */
    void resetValues(bool zero_values = false);

    using StaticDecoder::getData;
    /**
     * @brief Get the current set data
     * @remark For performance issues this will not check any validity!
     * @return void* The data pointer
     */
    void* getData() noexcept;

private:
    friend class CodecFactory;
    /// For internal use only. @internal
    StaticCodec(std::shared_ptr<const StructAccess> codec_access,
                void* data,
                size_t data_size,
                DataRepresentation representation);
    Element _first_element;
};

} // namespace codec
} // namespace ddl

#include <ddl/codec/legacy/static_codec_legacy.h>

#endif // DDL_STATIC_CODEC_CLASS_HEADER
