/**
 * @file
 * Implementation of the CodecIndex.
 *
 * @copyright
 * @verbatim
Copyright @ 2022 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */

#ifndef DDL_CODEC_INDEX_CLASS_HEADER
#define DDL_CODEC_INDEX_CLASS_HEADER

#include <ddl/codec/codec_type_info.h>
#include <ddl/dd/dd_common_types.h>

#include <string>
#include <vector>

namespace ddl {
/**
 * Structure for bit position of elements
 */
struct Position {
    /**
     * @brief the bit offset
     */
    size_t bit_offset = {};
    /**
     * @brief the elements bitsize (the whole element)
     */
    size_t bit_size = {};
};
/**
 * @brief Namespace for the *new* faster CodecFactory/Decoder/Codec implementation.
 */
namespace codec {

/**
 * Binary compatible reusing of structure for position of elements
 */
using Position = ddl::Position;

/**
 * Position with type size information
 */
struct PositionWithTypeSize : public Position {
    /**
     * @brief type size in bits (in case of array, this is the size of one element within the array)
     */
    size_t type_bit_size = {};
};
/**
 * Position with type size information
 */
struct PositionWithAlignedTypeSize : public PositionWithTypeSize {
    /**
     * @brief aligned type size in bits
     */
    size_t type_bit_size_aligned = {};
};

/**
 * Position with used bit type size information
 */
struct PositionWithUsedBitsTypeSize : public PositionWithTypeSize {
    /**
     * @brief type size in bits which is used (may be a bitfield)
     */
    size_t type_bit_size_used = {};
};

/**
 * Base layout definition for one element.
 */
struct ElementLayoutBase {
    /**
     * @brief deserialized position.
     */
    PositionWithAlignedTypeSize deserialized = {};
    /**
     * @brief serialized position.
     */
    PositionWithUsedBitsTypeSize serialized = {};
    /**
     * @brief Byte order of the element value
     */
    ddl::dd::ByteOrder byte_order = {};
    /**
     * @brief Array size of the element value
     */
    size_t array_size = {};
    /**
     * @brief The child element count.
     */
    size_t child_element_count = {};
    /**
     * @brief Pointer to the type_info
     */
    const CodecTypeInfo* type_info = {};
    /**
     * @brief Pointer to the constant_info if the value is a constant.
     */
    const CodecConstantInfo* constant_info = {};
    /**
     * @brief Pointer to the default_value_info if the value has a default value set.
     */
    const CodecDefaultValueInfo* default_value_info = {};
};

/**
 * Layout defintion for one element with set array position.
 */
struct ElementLayout : public ElementLayoutBase {
    /**
     * @brief The array position of the element.
     */
    size_t array_pos = {};
};

/**
 * Fast Access Index Type for the coders.
 * The Codec Index ist a vector of element indexes
 * {{[element_index][array_pos]},{[element_sub_index][array_pos]}} pairs to access the elements of
 * one level in a fast way. You can access leafs \b and elements at each level.
 * @see @ref forEachLeafElement, @ref forEachElement, or the getElements() of each codec or decoder.
 */
class CodecIndex {
public:
    /**
     * @brief The element index at the @ref CodecIndex.
     *
     */
    class ElementIndex {
    public:
        /**
         * @brief CTOR
         */
        ElementIndex() = default;
        /**
         * @brief CTOR
         *
         * @param element_index The index to set, array_pos 0 is forced
         */
        ElementIndex(size_t element_index);
        /**
         * @brief CTOR
         *
         * @param element_index The index to set
         * @param array_pos The array_pos to set
         */
        ElementIndex(size_t element_index, size_t array_pos);

        /**
         * @brief equal operator.
         *
         * @param other The element index to compare.
         * @return true if equal
         * @return false if not equal
         */
        bool operator==(const ElementIndex& other) const;
        /**
         * @brief Not equal operator.
         *
         * @param other The element index to compare.
         * @return true if not equal
         * @return false if equal
         */
        bool operator!=(const ElementIndex& other) const;

        /**
         * @brief Get the Index
         *
         * @return size_t
         */
        size_t getIndex() const;
        /**
         * @brief Get the Array Pos
         *
         * @return size_t
         */
        size_t getArrayPos() const;

        /**
         * @brief Incrememts the index, keeps array pos.
         *
         * @return ElementIndex&
         */
        ElementIndex& operator++();

        /**
         * @brief Invalid element index.
         */
        static constexpr size_t _invalid_element_index = static_cast<size_t>(-1);
        /**
         * @brief Invalid array pos.
         */
        static constexpr size_t _invalid_array_pos = static_cast<size_t>(-1);

    private:
        size_t _element_index = _invalid_element_index;
        size_t _array_pos = _invalid_array_pos;
    };
    /**
     * @brief Construct a new Codec Index
     */
    CodecIndex() = default;
    /**
     * @brief Access operator to retrieve a elment index on the given pos
     *
     * @param pos Position of the element index to retrieve.
     * @return const ElementIndex&
     */
    const ElementIndex& operator[](size_t pos) const;
    /**
     * @brief Equal operator.
     *
     * @param other The codec index to compare.
     * @return true if equal
     * @return false if not equal
     */
    bool operator==(const CodecIndex& other) const;
    /**
     * @brief Not equal operator.
     *
     * @param other The codec index to compare.
     * @return true if not equal
     * @return false if equal
     */
    bool operator!=(const CodecIndex& other) const;
    /**
     * @brief Get the Size of the codec index in element index count
     *
     * @return size_t
     */
    size_t getSize() const;
    /**
     * @brief Gets the Size of the codec index in element index count for generic programming.
     *
     * @return size_t
     */
    size_t size() const;
    /**
     * @brief Has layout set
     *
     * @return true has valid layout set
     * @return false has no valid layout set
     */
    bool hasLayout() const;
    /**
     * @brief Get the elements type if CodecIndex is valid
     * @return ElementType
     */
    ElementType getType() const;
    /**
     * @brief Get the Layout
     *
     * @return const ElementLayout&
     */
    const ElementLayout& getLayout() const;

private:
    friend class StructAccess;
    friend class ChildElementAccess;
    template <typename ElementAccessType, typename ChildElementsType>
    friend class FactoryElement;
    template <typename ElementAccessType, typename ChildElementsType>
    friend class DecoderElement;
    template <typename ElementAccessType, typename ChildElementsType>
    friend class CodecElement;
    template <typename ElementAccessType>
    friend class ChildElements;

    CodecIndex(const CodecIndex& codec_index, ElementIndex index);
    CodecIndex(ElementIndex index);
    CodecIndex(const CodecIndex& codec_index, size_t count, ElementIndex index);
    CodecIndex(const std::vector<ElementIndex>& indexes);
    CodecIndex(ElementIndex index, const ElementLayout& layout);
    CodecIndex(const CodecIndex& codec_index,
               size_t count,
               ElementIndex index,
               const ElementLayout& layout);

    CodecIndex& operator++();
    CodecIndex operator++(int);

    void setLayout(const ElementLayout& layout);
    void addElementIndex(ElementIndex index);
    CodecIndex::ElementIndex back() const noexcept;
    CodecIndex getIndexForArrayPos(size_t array_pos) const;

private:
    std::vector<ElementIndex> _indices = {};
    ElementLayout _layout = {};
    bool _layout_valid = {};
};

/**
 * @brief Converts the index to a string.
 *
 * @param index the index to convert
 * @return std::string
 */
std::string toString(const CodecIndex& index);

} // namespace codec
} // namespace ddl

#endif // DDL_CODEC_INDEX_CLASS_HEADER
