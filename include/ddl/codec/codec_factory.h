/**
 * @file
 * Implementation of new CodecFactory based on CodecIndex.
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

#ifndef DDL_CODEC_FACTORY_CLASS_HEADER
#define DDL_CODEC_FACTORY_CLASS_HEADER

#include <ddl/codec/codec.h>
#include <ddl/codec/codec_iterator.h>
#include <ddl/codec/legacy/access_element_legacy.h>
#include <ddl/codec/static_codec.h>
#include <ddl/dd/dd.h>
#include <ddl/dd/ddstructure.h>

namespace ddl {
namespace codec {
/**
 * Factory class for ddl codecs.
 */
class CodecFactory {
public:
    /**
     * Empty constructor. This exists to enable uninitialized member variables of this type
     * that are move-assigned later on.
     */
    CodecFactory();
    /**
     * @brief copy CTOR
     */
    CodecFactory(const CodecFactory&);
    /**
     * @brief copy assignment operator
     * @return CodecFactory&
     */
    CodecFactory& operator=(const CodecFactory&);

    /**
     * @brief move CTOR
     */
    CodecFactory(CodecFactory&&);
    /**
     * @brief move assignment operator
     * @return CodecFactory&
     */
    CodecFactory& operator=(CodecFactory&&);

    /// Default dtor
    ~CodecFactory() = default;

    /**
     * Constructor that take a DataDefinition string for initialization.
     * @param[in] struct_name The name of the struct for which codecs should be generated.
     * @param[in] dd_string The DataDefinition description within a (xml)string.
     */
    CodecFactory(const std::string& struct_name, const std::string& dd_string);

    /**
     * Constructor that uses an OO-DataDefinition struct for initialization.
     * @param[in] struct_type The struct definition.
     * @param[in] dd The DD
     */
    CodecFactory(const ddl::dd::StructType& struct_type, const dd::DataDefinition& dd);

    /**
     * Constructor that uses an OO-DataDefinition struct for initialization.
     * @param[in] dd_struct The struct definition.
     */
    CodecFactory(const DDStructure& dd_struct);

    /**
     * Constructor that uses an StructTypeAccess for initialization.
     * @param[in] struct_type_access The struct type access.
     */
    CodecFactory(const ddl::dd::StructTypeAccess& struct_type_access);

    /**
     * Check if the factory is in a valid state.
     * @return Any errors during construction.
     */
    a_util::result::Result isValid() const;

    /**
     * Creates a static decoder for the given data.
     * @param[in] data The pointer to the raw data.
     * @param[in] data_size The size of the raw data.
     * @param[in] rep The representation that the data is encoded in.
     * @return a static decoder.
     */
    inline StaticDecoder makeStaticDecoderFor(const void* data,
                                              size_t data_size,
                                              DataRepresentation rep = deserialized) const
    {
        return StaticDecoder(_codec_access, data, data_size, rep);
    }

    /**
     * Creates a static codec for the given data.
     * @param[in] data The pointer to the raw data.
     * @param[in] data_size The size of the raw data.
     * @param[in] rep The representation that the data is encoded in.
     * @return a static codec.
     */
    inline StaticCodec makeStaticCodecFor(void* data,
                                          size_t data_size,
                                          DataRepresentation rep = deserialized) const
    {
        return StaticCodec(_codec_access, data, data_size, rep);
    }

    /**
     * Creates a decoder for the given data.
     * @param[in] data The pointer to the raw data.
     * @param[in] data_size The size of the raw data.
     * @param[in] rep The representation that the data is encoded in.
     * @return a decoder.
     */
    Decoder makeDecoderFor(const void* data,
                           size_t data_size,
                           DataRepresentation rep = deserialized) const;

    /**
     * Creates a codec for the given data.
     * @param[in] data The pointer to the raw data.
     * @param[in] data_size The size of the raw data.
     * @param[in] rep The representation that the data is encoded in.
     * @return a codec.
     */
    Codec makeCodecFor(void* data, size_t data_size, DataRepresentation rep = deserialized) const;
    /**
     * Type of a single factory element.
     */
    typedef FactoryElement<FactoryElementAccess<const CodecFactory>> Element;
    /**
     * Retrieves an element instance based on the codec index of that element.
     * @param[in] codec_index The codec index.
     * @return The Codec Index of the FactoryElement.
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
     * @return The Codec Index of the FactoryElement.
     * @remark The element is only valid as long as the factory exists.
     * @throw throws std::runtime_error if not found.
     */
    Element getElement(const std::string& full_element_name) const;
    /**
     * Iterator container to iterate all elements of the factory.
     * @see @ref forEachLeafElement, @ref forEachElement
     * @see @ref ddl::codec::ChildElements, @ref CodecFactory::Element
     */
    typedef ChildElements<FactoryElementAccess<const CodecFactory>> Elements;
    /**
     * Retrieves the elements of the factory to get element information.
     * @return Elements The elements iterator container.
     * @see @ref forEachLeafElement, @ref forEachElement, @ref CodecFactory::Element, @ref
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
     * representation is also the base name representation: "element_name".
     *
     * @param[in] codec_index The codec index of the element.
     * @return The base name of the element.
     * @throw throws std::runtime_error if not found.
     */
    const std::string& getElementBaseName(const CodecIndex& codec_index) const;
    /**
     * @brief Gets the static buffer size in bytes for the structure.
     *
     * @param rep option to retrieve size in bytes for the deserialized or serialized
     * representation.
     * @retval size_t size in bytes of the structure depending on the given representation @p rep
     */
    size_t getStaticBufferSize(DataRepresentation rep = deserialized) const;

    /**
     * The codec index will be resolved for fast access.
     * @param[in] codec_index The codec index to resolve.
     * @return CodecIndex The resolved codec index.
     * @throw throws std::runtime_error if not found.
     */
    void resolve(CodecIndex& codec_index) const;

public: // legacy functions
    /**
     * Legacy: Resolves given the leaf index to a codec index.
     * @param[in] leaf_index The leaf index of the element.
     * @return the codec index with information about the element if found.
     * @throw std::runtime_error if index not found.
     * @remark This is for legacy access only!
     */
    CodecIndex resolve(size_t leaf_index) const;
    /**
     * Legacy: Returns the amount of static (leaf) elements.
     * @return The amount of static elements contained in the handled structure.
     * @remark This is for legacy access only!
     *         This amount is the fully resolved amount of ALL reachable (POD) leafs.
     */
    size_t getStaticElementCount() const;
    /**
     * Legacy: Returns the StructElement of the element with given leaf index.
     * @param[in] leaf_index The leaf index of the element.
     * @param[out] legacy_struct_element the struct element legacy description.
     * @return The amount of static elements contained in the handled structure.
     * @remark This is for legacy access only!
     */
    a_util::result::Result getStaticElement(size_t leaf_index,
                                            const ddl::StructElement*& legacy_struct_element) const;

private:
    /// For internal use only.  @internal The struct layout.
    std::shared_ptr<const StructAccess> _codec_access;
    /// For internal use only. @internal The constructor result.
    a_util::result::Result _constructor_result;
    /// For internal use only. @internal for iterating elements
    Element _first_element;
    /// For internal use only. @internal For legacy access element
    ddl::access_element::StructElementLegacy<const CodecFactory> _legacy_element;
};

namespace detail {
/**
 * @brief Get the Decoder or Factory leaf element count
 *
 * @tparam T The decoder or factory type
 * @param decoder_or_factory the decoder or factory
 * @return size_t the (leaf) element count value
 */
template <typename T>
size_t getDecoderOrFactoryElementCount(const T& decoder_or_factory)
{
    return decoder_or_factory.getStaticElementCount();
}
/**
 * @brief Get the Decoders leaf element count for a ddl::codec::Decoder
 *
 * @param decoder the decoder to retrieve the element count
 * @return size_t the (leaf) element count value
 */
template <>
inline size_t getDecoderOrFactoryElementCount(const ddl::codec::Decoder& decoder)
{
    return decoder.getElementCount();
}
/**
 * @brief Get the Codecs leaf element count for a ddl::codec::Codec
 *
 * @param codec the decoder to retrieve the element count
 * @return size_t the (leaf) element count value
 */
template <>
inline size_t getDecoderOrFactoryElementCount(const ddl::codec::Codec& codec)
{
    return codec.getElementCount();
}
} // namespace detail

/**
 * @brief Retrieves all codec indices for the given codec of type \p T.
 * @tparam T The codec type or factory.
 * @param[in] decoder_or_factory the codec or factory of type \p T
 * @return std::vector<CodecIndex> the codec indices
 */
template <typename T>
std::vector<CodecIndex> getCodecIndices(const T& decoder_or_factory)
{
    std::vector<CodecIndex> indices;
    indices.reserve(detail::getDecoderOrFactoryElementCount<T>(decoder_or_factory));
    forEachLeafElement(decoder_or_factory.getElements(),
                       [&indices](const auto& element) { indices.push_back(element.getIndex()); });
    return indices;
}
/**
 * @brief Get the leaf indices object.
 *
 * @tparam T The type of the decoder or factory to retrieve all leaf indices from.
 * @param decoder_or_factory The decoder or factory to retrieve all indices from.
 * @param rep The data representation to extract the leaf codec index for
 * @return std::vector<LeafCodecIndex>
 * @see LeafCodecIndex
 */
template <typename T>
std::vector<LeafCodecIndex> getLeafCodecIndices(const T& decoder_or_factory,
                                                ddl::DataRepresentation rep)
{
    std::vector<LeafCodecIndex> indices;
    indices.reserve(detail::getDecoderOrFactoryElementCount<T>(decoder_or_factory));
    forEachLeafElement(decoder_or_factory.getElements(), [&indices, &rep](const auto& element) {
        indices.push_back(LeafCodecIndex(element.getIndex(), rep));
    });
    return indices;
}

} // namespace codec
} // namespace ddl

#include <ddl/codec/legacy/codec_factory_legacy.h>

#endif // DDL_CODEC_FACTORY_CLASS_HEADER
