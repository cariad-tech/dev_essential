/**
 * @file
 * Implementation of the Codec and Decoder.
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

#ifndef DDL_CODEC_CLASS_HEADER
#define DDL_CODEC_CLASS_HEADER

#include <a_util/result.h>
#include <ddl/codec/static_codec.h>

namespace ddl {
namespace codec {

class Codec;
class StructAccess;

/**
 * Decoder for dynamic structures defined by a DataDefinition definition.
 */
class Decoder : public StaticDecoder {
public:
    /**
     * Default constructor.
     */
    Decoder();

    /**
     * Move constructor.
     */
    Decoder(Decoder&&);

    /**
     * Move assignment operator.
     */
    Decoder& operator=(Decoder&&);

    /**
     * No copy constructor.
     */
    Decoder(const Decoder&) = delete;

    /**
     * No copy assignment operator.
     */
    Decoder& operator=(const Decoder&) = delete;

    /**
     * @copydoc StaticDecoder::isValid
     */
    a_util::result::Result isValid() const override;
    /**
     * Decoder element.
     */
    typedef DecoderElement<DecoderElementAccess<const Decoder>> Element;
    /**
     * Retrieves an element instance based on the codec index of that element.
     * @param[in] codec_index The codec index.
     * @return The DecoderElement for the given codec index.
     * @remark The element is only valid as long as the factory exists.
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
     * @remark The element is only valid as long as the factory exists.
     * @throw throws std::runtime_error if not found.
     */
    Element getElement(const std::string& full_element_name) const;
    /**
     * Iterator container to iterate all elements of the decoder and get values.
     * @see @ref forEachLeafElement, @ref forEachElement, @ref ddl::codec::ChildElements, @ref
     * Decoder::Element
     */
    typedef ChildElements<DecoderElementAccess<const Decoder>> Elements;
    /**
     * Retrieves the elements of the decoder to get element information and get values.
     * @return Elements The elements iterator container.
     * @see @ref forEachLeafElement, @ref forEachElement, @ref Decoder::Element, @ref CodecIndex
     */
    const Elements& getElements() const;
    /**
     * @brief Gets the buffer size in bytes for the main structure.
     * @retval size_t of the structure depending on the current representation (@ref
     * getRepresentation)
     */
    size_t getBufferSize() const;
    /**
     * @brief Gets the buffer size in bytes for the main structure.
     * @param[in] representation The data representation for which the buffer size should be
     * returned.
     * @return The size of the structure in the requested data representation.
     */
    size_t getBufferSize(DataRepresentation representation) const;
    /**
     * Create a new codec with the current dynamic structure layout for a new data buffer.
     * @param[in] data The pointer to the new raw data.
     * @param[in] data_size The size of the new raw data.
     * @param[in] representation The representation that the data should be encoded in.
     * @return A codec.
     */
    Codec makeCodecFor(void* data, size_t data_size, DataRepresentation representation) const;

protected:
    friend class CodecFactory;
    /// For internal use only. @internal
    Decoder(std::shared_ptr<const StructAccess> codec_access,
            const void* data,
            size_t data_size,
            DataRepresentation representation);
    /// For internal use only. @internal
    Decoder(const Decoder& decoder,
            const void* data,
            size_t data_size,
            DataRepresentation representation);
    /// For internal use only. @internal
    friend class FactoryElementAccess<const Decoder>;

private:
    Element _first_element;
};

/**
 * Decoder for dynamic structures defined by a DataDefinition definition.
 * Currently the amount of dynamic elements is determined during construction
 * only (by the current values in the structure).
 */
class Codec : public Decoder {
public:
    /**
     * Default constructor.
     */
    Codec();
    /**
     * Move constructor.
     */
    Codec(Codec&&);
    /**
     * Move assignment operator.
     */
    Codec& operator=(Codec&&);
    /**
     * No copy constructor.
     */
    Codec(const Codec&) = delete;
    /**
     * No copy assignment operator.
     */
    Codec& operator=(const Codec&) = delete;

    using Decoder::getElement;
    using Decoder::getElementAddress;
    using Decoder::getElements;
    /**
     * Codec element.
     */
    typedef CodecElement<CodecElementAccess<Codec>> Element;
    /**
     * Retrieves an element instance based on the codec index of that element.
     * @param[in] codec_index The codec index.
     * @return The Codec Index of the CodecElement.
     * @remark The element is only valid as long as the codec exists.
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
     * @remark The element is only valid as long as the codec exists.
     * @throw throws std::runtime_error if not found.
     */
    Element getElement(const std::string& full_element_name);
    /**
     * Iterator container to iterate all elements of the codec.
     * @see @ref forEachLeafElement, @ref forEachElement
     * @see @ref ddl::codec::ChildElements, @ref Codec::Element
     */
    typedef ChildElements<CodecElementAccess<Codec>> Elements;
    /**
     * Retrieves the elements of the codec to get element information.
     * @return Elements The elements iterator container.
     * @see @ref forEachLeafElement, @ref forEachElement, @ref Codec::Element, @ref CodecIndex
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
        ValueSetterSelect<Codec, T>::setValue(*this, codec_index, value);
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
     * @param[in] value_size The size in bytes of the location (if 0 - unsafe usage!).
     * @throw throws std::runtime_error if not found or value_size is lower then elements value
     * memory to copy.
     */
    void setElementRawValue(const CodecIndex& codec_index, const void* value, size_t value_size);
    /**
     * @param[in] codec_index The index of the element.
     * @return A pointer to the element for write access.
     */
    void* getElementAddress(const CodecIndex& codec_index);
    /**
     * Sets all elements to their default values, constant values defined in the data definition or
     * zero if \p zero_values is set.
     * @param[in] zero_values set the value to 0 if no constant or default value set in data
     * definition.
     */
    void resetValues(bool zero_values = false);

    using Decoder::getData;
    /**
     * @brief Get the current set data
     * @remark For performance issues this will not check any validity!
     * @return void* The data pointer
     */
    void* getData() noexcept;

protected:
    friend class CodecFactory;
    friend class Decoder;
    /// For internal use only. @internal
    Codec(std::shared_ptr<const StructAccess> codec_access,
          void* data,
          size_t data_size,
          ddl::DataRepresentation rep);
    /// For internal use only. @internal
    Codec(const Decoder& decoder,
          void* data,
          size_t data_size,
          ddl::DataRepresentation representation);

private:
    Element _first_element;
};

} // namespace codec
} // namespace ddl

#include <ddl/codec/legacy/codec_legacy.h>

#endif // DDL_CODEC_CLASS_HEADER
