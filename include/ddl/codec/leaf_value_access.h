/**
 * @file
 * The leaf value access decoding and encoding is a header only optimization
 * for leaf values of standard types at 1-byte aligned positions.
 *
 * @copyright
 * @verbatim
Copyright @ 2022 VW Group. All rights reserved.

    This Source Code Form is subject to the terms of the Mozilla
    Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

If it is not possible or desirable to put the notice in a particular file, then
You may include the notice in a location (such as a LICENSE file in a
relevant directory) where a recipient would be likely to look for such a notice.

You may add additional accurate notices of copyright ownership.
@endverbatim
 */

#ifndef DDL_PREVIEW_LEAF_VALUE_ACCESS_PRIVATE_CLASS_HEADER
#define DDL_PREVIEW_LEAF_VALUE_ACCESS_PRIVATE_CLASS_HEADER

#include <a_util/base/type_traits.h>
#include <ddl/codec/bitserializer.h>
#include <ddl/codec/codec_index.h>
#include <ddl/codec/data_representation.h>
#include <ddl/utilities/std_to_string.h>

#include <cstring>
#include <stdexcept>

namespace ddl {
namespace codec {

/**
 * @brief LeafLayout data representation flags
 */
enum class LeafDataRepresentation : uint8_t {
    /**
     * @brief first bit is zero for deserialized data representation of the @p
     * LeafCodecIndex::data_flags
     */
    deserialized = 0x00,
    /**
     * @brief first bit is set for serialized data representation of the @p
     * LeafCodecIndex::data_flags
     */
    serialized = 0x01,
    /**
     * @brief first and second bit is set for serialized data representation in big endianess
     * of the @p LeafCodecIndex::data_flags
     */
    serialized_be = 0x03,
    /**
     * @brief first bit is set second bit is not set for serialized data representation in little
     * endianess of the @p LeafCodecIndex::data_flags
     */
    serialized_le = 0x01
};

/**
 * Valid ElementTypes for the LeafLayout
 */
enum class LeafElementType : uint8_t {
    let_bool = static_cast<uint8_t>(ElementType::cet_bool), //!< LeafElementType type is bool
    let_int8 = static_cast<uint8_t>(ElementType::cet_int8), //!< LeafElementType type is std::int8_t
    let_uint8 =
        static_cast<uint8_t>(ElementType::cet_uint8), //!< LeafElementType type is std::uint8_t
    let_int16 =
        static_cast<uint8_t>(ElementType::cet_int16), //!< LeafElementType type is std::int16_t
    let_uint16 =
        static_cast<uint8_t>(ElementType::cet_uint16), //!< LeafElementType type is std::uint16_t
    let_int32 =
        static_cast<uint8_t>(ElementType::cet_int32), //!< LeafElementType type is std::int32_t
    let_uint32 =
        static_cast<uint8_t>(ElementType::cet_uint32), //!< LeafElementType type is std::uint32_t
    let_int64 =
        static_cast<uint8_t>(ElementType::cet_int64), //!< LeafElementType type is std::int64_t
    let_uint64 =
        static_cast<uint8_t>(ElementType::cet_uint64), //!< LeafElementType type is std::uint64_t
    let_float = static_cast<uint8_t>(ElementType::cet_float),   //!< LeafElementType type is float
    let_double = static_cast<uint8_t>(ElementType::cet_double), //!< LeafElementType type is double
};

/**
 * @brief small leaf layout information to access codec data very fast.
 * @see LeafCodecIndex
 */
struct LeafLayout {
    /// invalid position for @p byte_pos
    static constexpr uint32_t _invalid_pos = static_cast<uint32_t>(-1);
    /// max type byte size of an element (usually this can not be reached for standard data types)
    static constexpr size_t _max_byte_size = 31;
    /// max type bit size of an element (usually this can not be reached for standard data types)
    static constexpr size_t _max_bit_size = 255;

    /// byte position of the element for serialized \b or deserialized
    uint32_t byte_pos =
        _invalid_pos; // we do not need to use size_t here because CodecLayout will have bits!
    /// bit position of the element for serialized only
    uint8_t bit_pos = {}; // bitpos is only set in serialzed representation
    /// type of the value. accept all valid leaf types, except user defined types
    LeafElementType element_type{}; // elementtype
    /// type bit size of an element for serialized \b or deserialized
    uint8_t bit_size = {}; // max. 255bit for userdefined types!
    /// flags of the described layout
    uint8_t data_flags = static_cast<uint8_t>(LeafDataRepresentation::deserialized);
};

/**
 * @brief The Leaf codec index is a small layout information index to access
 *        the decoders/codecs data very fast.
 * There are following constraints:
 * \li only possible to retrieve layout information for a leaf within the struct
 * \li only standard data types are supported
 * \li no customized data type can be used (standard must have standard sizes, user types are not
 * possible) \li with this index it is not possible to identify the name of the element \li no
 * complex types or the whole array element can be accessed \li it can only describe serialized \b
 * or deserialized data representation, you need to set while creating it
 */
class LeafCodecIndex {
private:
    template <bool ThrowError>
    using throw_error = std::integral_constant<bool, ThrowError>;

public:
    /**
     * @brief no CTOR
     */
    LeafCodecIndex() = delete;
    /**
     * @brief CTOR
     * @param codec_index the valid codec index to create the LeafCodecIndex from
     * @param rep the data representation to extract for decoding
     */
    LeafCodecIndex(const CodecIndex& codec_index,
                   ddl::DataRepresentation rep = DataRepresentation::deserialized)
    {
        convertToLeafLayout</*throw_error=*/true>(codec_index, _leaf_layout, rep);
    }
    /**
     * @brief Get the leaf layout
     * @return const LeafLayout&
     */
    const LeafLayout& getLayout() const noexcept
    {
        return _leaf_layout;
    }

    /**
     * @brief Retrieves a valid, small, 8-bytes LeafLayout from a CodecIndex.
     *
     * @tparam ThrowError Throws if the conversion is not possible. (If set to false, the return
     * value must be checked!)
     * @param codec_index The CodecIndex to retrieve the lyout information from
     * @param leaf_layout The new small leaf layout
     * @param rep The layout information for a serialzed or deserialized representation
     * @return true The layout retrieval was successfully
     * @return false The layout retrieval was not successfully
     */
    template <bool ThrowError>
    static bool convertToLeafLayout(const CodecIndex& codec_index,
                                    LeafLayout& leaf_layout,
                                    ddl::DataRepresentation rep)
    {
        if (!checkLeafLayout<ThrowError>(codec_index, rep)) {
            return false;
        }
        return convertToLeafLayout(codec_index, leaf_layout, rep, throw_error<ThrowError>{});
    }

private:
    /// For internal use only. @internal
    static bool convertToLeafLayout(const CodecIndex& codec_index,
                                    LeafLayout& leaf_layout,
                                    ddl::DataRepresentation rep,
                                    throw_error<false>) noexcept
    {
        const auto& layout = codec_index.getLayout();
        leaf_layout.element_type = static_cast<LeafElementType>(layout.type_info->getType());
        if (rep == DataRepresentation::deserialized) {
            leaf_layout.data_flags = static_cast<uint8_t>(LeafDataRepresentation::deserialized);
            leaf_layout.byte_pos = static_cast<uint32_t>(layout.deserialized.bit_offset / 8);
            if (layout.deserialized.bit_size > std::numeric_limits<uint8_t>::max()) {
                return false;
            }
            // no bitpos in serialized representation
            leaf_layout.bit_size = static_cast<uint8_t>(layout.deserialized.type_bit_size);
        }
        else {
            leaf_layout.data_flags |=
                (layout.byte_order == dd::ByteOrder::e_be) ?
                    static_cast<uint8_t>(LeafDataRepresentation::serialized_be) :
                    static_cast<uint8_t>(LeafDataRepresentation::serialized_le);
            leaf_layout.bit_pos = layout.serialized.bit_offset % 8;
            leaf_layout.byte_pos = static_cast<uint32_t>(layout.serialized.bit_offset / 8 +
                                                         ((leaf_layout.bit_pos == 0) ? 0 : 1));
            if (layout.deserialized.bit_size > std::numeric_limits<uint8_t>::max()) {
                return false;
            }
            leaf_layout.bit_size = static_cast<uint8_t>(layout.serialized.type_bit_size_used);
            if (layout.serialized.type_bit_size_used == layout.deserialized.type_bit_size &&
                leaf_layout.bit_pos == 0 &&
                layout.byte_order == dd::ByteOrderDefault::getPlatformDefault()) {
                // this means: the same type is used in serialized and deserialized without byteswap
                //            the simple fast memcopy operation can be used
                // switch back to deserialzed option, but keep bytepos!
                leaf_layout.data_flags = static_cast<uint8_t>(LeafDataRepresentation::deserialized);
            }
        }
        return true;
    }

    /// For internal use only. @internal
    static bool convertToLeafLayout(const CodecIndex& codec_index,
                                    LeafLayout& leaf_layout,
                                    ddl::DataRepresentation rep,
                                    throw_error<true>)
    {
        if (!convertToLeafLayout(codec_index, leaf_layout, rep, throw_error<false>{})) {
            throw std::runtime_error(
                "element size exceeds maximal bit size for small leaf layout of" +
                std::to_string(std::numeric_limits<uint8_t>::max()));
        }
        return true;
    }

    /// For internal use only. @internal
    template <typename T>
    static bool checkTypeSize(const CodecIndex& codec_index,
                              ddl::DataRepresentation rep,
                              bool exact_check_on_serialized,
                              throw_error<false>) noexcept
    {
        const auto size_to_check = (rep == ddl::DataRepresentation::serialized) ?
                                       (codec_index.getLayout().serialized.type_bit_size / 8) :
                                       (codec_index.getLayout().deserialized.type_bit_size / 8);
        if (rep == ddl::DataRepresentation::serialized && !exact_check_on_serialized) {
            if (sizeof(T) < size_to_check) {
                return false;
            }
        }
        return (sizeof(T) == size_to_check);
    }

    /// For internal use only. @internal
    template <typename T>
    static bool checkTypeSize(const CodecIndex& codec_index,
                              ddl::DataRepresentation rep,
                              bool exact_check,
                              throw_error<true>)
    {
        if (!checkTypeSize<T>(codec_index, rep, exact_check, throw_error<false>{})) {
            throw std::runtime_error(
                "Standard datatype is only supported if bit size is not customized. Found "
                "element type with different size to standard!");
        }
        return true;
    }

    /// For internal use only. @internal
    template <bool ThrowError>
    static bool checkLeafLayout(const CodecIndex& codec_index, ddl::DataRepresentation rep)
    {
        switch (codec_index.getType()) {
        case ElementType::cet_empty: // fall thru
        case ElementType::cet_sub_codec: {
            if (ThrowError) {
                throw std::runtime_error("Small leaf layouts can only be used for leaf elements!");
            }
            return false;
            break;
        }
        case ElementType::cet_bool: {
            return checkTypeSize<bool>(codec_index, rep, false, throw_error<ThrowError>{});
            break;
        }
        case ElementType::cet_int8:
        case ElementType::cet_uint8: {
            return checkTypeSize<int8_t>(codec_index, rep, false, throw_error<ThrowError>{});
            break;
        }
        case ElementType::cet_int16:
        case ElementType::cet_uint16: {
            return checkTypeSize<int16_t>(codec_index, rep, false, throw_error<ThrowError>{});
            break;
        }
        case ElementType::cet_int32:
        case ElementType::cet_uint32: {
            return checkTypeSize<int32_t>(codec_index, rep, false, throw_error<ThrowError>{});
            break;
        }
        case ElementType::cet_float: {
            return checkTypeSize<int32_t>(codec_index, rep, true, throw_error<ThrowError>{});
            break;
        }
        case ElementType::cet_int64:
        case ElementType::cet_uint64: {
            return checkTypeSize<int64_t>(codec_index, rep, false, throw_error<ThrowError>{});
            break;
        }
        case ElementType::cet_double: {
            return checkTypeSize<int64_t>(codec_index, rep, true, throw_error<ThrowError>{});
            break;
        }
        case ElementType::cet_user_type: {
            if (ThrowError) {
                throw std::runtime_error(
                    "user supported types are not supported in small leaf layout");
            }
            else {
                return false;
            }
        }
        }
        return true;
    }
    /// For internal use only. @internal
    LeafLayout _leaf_layout;
};

#if defined(__GNUC__) && (__GNUC__ == 5) && defined(__QNX__)
#pragma GCC diagnostic ignored                                                                     \
    "-Wattributes" // standard type attributes are ignored when used in templates
#endif             // defined(__GNUC__) && (__GNUC__ == 5) && defined(__QNX__)

namespace detail {

template <typename ValueType>
struct SupportedLeafValueTypeExplicit
    : public std::is_arithmetic<a_util::underlying_type_or_type_t<ValueType>> {
};

template <typename ElementValueTypeFrom, typename TargetValueTypeTo>
struct SupportTargetTypeConversion
    : public std::integral_constant<
          bool,
          (!std::is_enum<TargetValueTypeTo>::value &&
           a_util::is_explicitly_convertible_to_v<ElementValueTypeFrom, TargetValueTypeTo>) ||
              (std::is_enum<TargetValueTypeTo>::value &&
               !std::is_floating_point<ElementValueTypeFrom>::value)> {
};

template <typename ValueType>
struct SupportAnyFromFundamentalConversion
    : public std::integral_constant<
          bool,
          a_util::is_explicitly_convertible_to_v<bool, ValueType> ||
              a_util::is_explicitly_convertible_to_v<int8_t, ValueType> ||
              a_util::is_explicitly_convertible_to_v<uint8_t, ValueType> ||
              a_util::is_explicitly_convertible_to_v<int16_t, ValueType> ||
              a_util::is_explicitly_convertible_to_v<uint16_t, ValueType> ||
              a_util::is_explicitly_convertible_to_v<int32_t, ValueType> ||
              a_util::is_explicitly_convertible_to_v<uint32_t, ValueType> ||
              a_util::is_explicitly_convertible_to_v<int64_t, ValueType> ||
              a_util::is_explicitly_convertible_to_v<uint64_t, ValueType> ||
              a_util::is_explicitly_convertible_to_v<float, ValueType> ||
              a_util::is_explicitly_convertible_to_v<double, ValueType>> {
};

/**
 * @brief Converter to convert a value of @p ElementValueType to a value of @p TargetValueType
 *
 * @tparam ElementValueType The source value type to convert from
 * @tparam TargetValueType The target value type to convert to
 * @tparam conversion_allowed evaluates to a bool value if conversion is possible
 */
template <typename ElementValueType,
          typename TargetValueType,
          bool conversion_allowed =
              SupportTargetTypeConversion<ElementValueType, TargetValueType>::value>
struct TargetValueConverter {
    /**
     * @brief Converts a @p element_value to as value of type TargetValueType
     *
     * @param element_value The value to convert from
     * @return TargetValueType The result value to convert to
     */
    static TargetValueType convert(const ElementValueType& element_value)
    {
        return static_cast<TargetValueType>(element_value);
    }
};

/**
 * @brief Converter to convert a value of @p ElementValueType to a value of @p TargetValueType
 *
 * @tparam ElementValueType The source value type to convert from
 * @tparam TargetValueType The target value type to convert to
 */
template <typename ElementValueType, typename TargetValueType>
struct TargetValueConverter<ElementValueType, TargetValueType, false> {
    /**
     * @brief Should convert a value of ElementValueType to a value of type ElementValueType,
     *        but this specialized code is used while conversion is not valid!
     *
     * @throws std::runtime_error for unsupported conversion
     */
    static TargetValueType convert(const ElementValueType&)
    {
        static_assert(SupportAnyFromFundamentalConversion<TargetValueType>::value,
                      "The used type must at least convertible from one fundamental type (bool, "
                      "any int, float, double) explicitly!");
        throw std::runtime_error("unsupported type conversion requested");
    }
};

template <typename ElementValueTypeTo, typename SourceValueTypeFrom>
struct SupportSourceTypeConversion
    : public std::integral_constant<
          bool,
          (!std::is_enum<SourceValueTypeFrom>::value &&
           a_util::is_explicitly_convertible_to_v<SourceValueTypeFrom, ElementValueTypeTo>) ||
              (std::is_enum<SourceValueTypeFrom>::value &&
               !std::is_floating_point<ElementValueTypeTo>::value)> {
};

template <typename ValueType>
struct SupportAnyToFundamentalConversion
    : public std::integral_constant<
          bool,
          a_util::is_explicitly_convertible_to_v<ValueType, bool> ||
              a_util::is_explicitly_convertible_to_v<ValueType, int8_t> ||
              a_util::is_explicitly_convertible_to_v<ValueType, uint8_t> ||
              a_util::is_explicitly_convertible_to_v<ValueType, int16_t> ||
              a_util::is_explicitly_convertible_to_v<ValueType, uint16_t> ||
              a_util::is_explicitly_convertible_to_v<ValueType, int32_t> ||
              a_util::is_explicitly_convertible_to_v<ValueType, uint32_t> ||
              a_util::is_explicitly_convertible_to_v<ValueType, int64_t> ||
              a_util::is_explicitly_convertible_to_v<ValueType, uint64_t> ||
              a_util::is_explicitly_convertible_to_v<ValueType, float> ||
              a_util::is_explicitly_convertible_to_v<ValueType, double>> {
};

/**
 * @brief Converter to convert from a value of @p SourceValueType to a value of @p ElementValueType
 *
 * @tparam ElementValueType The elements value target type
 * @tparam SourceValueType The values source type
 * @tparam conversion_allowed evaluates to a bool value if conversion is possible
 */
template <typename ElementValueType,
          typename SourceValueType,
          bool conversion_allowed =
              SupportSourceTypeConversion<ElementValueType, SourceValueType>::value>
struct SourceValueConverter {
    /**
     * @brief Converts a @p value of SourceValueType to a @p element_value of type ElementValueType
     *
     * @param element_value The converted value
     * @param value the value to convert
     * @return true The conversion is possible
     * @return false The converion was not possible (i.e. enum type to float)
     */
    static bool convert(ElementValueType& element_value, const SourceValueType& value)
    {
        element_value = static_cast<ElementValueType>(value);
        return true;
    }
};

/**
 * @brief Converter to convert from a value of @p SourceValueType to a value of @p ElementValueType
 *
 * @tparam ElementValueType The elements value target type
 * @tparam SourceValueType The values source type
 */
template <typename ElementValueType, typename SourceValueType>
struct SourceValueConverter<ElementValueType, SourceValueType, false> {
    /**
     * @brief Should convert a @p value of SourceValueType to a @p element_value of type
     * ElementValueType, but this specialized code is used while conversion is not valid!
     *
     * @return true The conversion is possible
     * @return false The converion was not possible (i.e. enum type to float)
     */
    static bool convert(ElementValueType&, const SourceValueType&)
    {
        static_assert(SupportAnyToFundamentalConversion<SourceValueType>::value,
                      "The used type must at least convertible to one fundamental type (bool, any "
                      "int, float, double) explicitly");
        return false;
    }
};

/**
 * @brief Read the bytes from a @p data area and converts this value to the @p TargetValueType
 *
 * @tparam ElementValueType The type for retrieving the value (this idetifies the size of the value
 * within @p data )
 * @tparam TargetValueType The target value type to convert to
 * @param data The data pointer of the memory
 * @param data_size The data size of @p data (in bytes)
 * @param byte_pos The byte position to retrieve the value
 * @return TargetValueType
 * @throws std::runtime_error if there is no valid conversion or the given bytepos is invalid
 */
template <typename ElementValueType, typename TargetValueType>
TargetValueType readBytes(const void* data, const size_t& data_size, uint32_t byte_pos)
{
    ElementValueType value;
    if (byte_pos + sizeof(ElementValueType) > data_size) {
        throw std::runtime_error("copy action exceeds buffersize");
    }
    std::memcpy(&value, static_cast<const uint8_t*>(data) + byte_pos, sizeof(ElementValueType));
    return TargetValueConverter<ElementValueType, TargetValueType>::convert(value);
}

/**
 * @brief Writes the value of @p source_value to the @p data area after conversion to the elements
 * @p ElementValueType
 *
 * @tparam ElementValueType The type of the element within the data area
 * @tparam SourceValueType The source value type to write
 * @param data the data pointer of the memory
 * @param data_size the data size of @p data (in bytes)
 * @param byte_pos the byte pos to write the value
 * @param source_value The source value to convert and write
 * @throws std::runtime_error if there is no valid conversion or the given bytepos is invalid
 */
template <typename ElementValueType, typename SourceValueType>
void writeBytes(void* data,
                const size_t& data_size,
                uint32_t byte_pos,
                const SourceValueType& source_value)
{
    if (byte_pos + sizeof(ElementValueType) > data_size) {
        throw std::runtime_error("copy action exceeds buffersize");
    }
    else {
        ElementValueType value;
        if (SourceValueConverter<ElementValueType, SourceValueType>::convert(value, source_value)) {
            std::memcpy(static_cast<uint8_t*>(data) + byte_pos, &value, sizeof(ElementValueType));
        }
        else {
            throw std::runtime_error("unsupported type conversion requested");
        }
    }
}

/**
 * @brief Read the bits from a @p data area and converts this value to the @p TargetValueType
 *
 * @tparam ElementValueType The type for retrieving the value
 * @tparam TargetValueType The target value type to convert to
 * @param data The data pointer of the memory
 * @param data_size The data size of @p data (in bytes)
 * @param bit_offset The @b bit offset of the value to retrieve in @p ElementValueType
 * @param bit_size The size of the value to retrieve in @p ElementValueType
 * @param byte_order The byteorder of the value to retrieve in @p ElementValueType
 * @return TargetValueType
 * @throws std::runtime_error if there is no valid conversion or the given bytepos is invalid
 */
template <typename ElementValueType, typename TargetValueType>
TargetValueType readBits(const void* data,
                         size_t data_size,
                         size_t bit_offset,
                         size_t bit_size,
                         a_util::memory::Endianess byte_order)
{
    ElementValueType value;
    a_util::memory::BitSerializer bit_reader(const_cast<void*>(data), data_size);
    auto res = bit_reader.read<ElementValueType>(bit_offset, bit_size, &value, byte_order);
    if (a_util::result::isOk(res)) {
        return TargetValueConverter<ElementValueType, TargetValueType>::convert(value);
    }
    throw std::runtime_error(res.getDescription());
}

/**
 * @brief Write the bits of the element to the @p data area after conversion from @p source_value.
 *
 * @tparam ElementValueType The elements type the write
 * @tparam SourceValueType The type to convert from
 * @param data The data pointer of the memory
 * @param data_size The data size of @p data (in bytes)
 * @param bit_offset The @b bit offset of the value to write
 * @param bit_size The size of the value to write
 * @param byte_order The byteorder of the value to write
 * @param source_value The source value to convert from
 * @throws std::runtime_error if there is no valid conversion or the given bitpos is invalid
 */
template <typename ElementValueType, typename SourceValueType>
void writeBits(void* data,
               size_t data_size,
               size_t bit_offset,
               size_t bit_size,
               a_util::memory::Endianess byte_order,
               const SourceValueType& source_value)
{
    ElementValueType value;
    if (SourceValueConverter<ElementValueType, SourceValueType>::convert(value, source_value)) {
        a_util::memory::BitSerializer bit_reader(data, data_size);
        auto res = bit_reader.write<ElementValueType>(bit_offset, bit_size, value, byte_order);
        if (a_util::result::isFailed(res)) {
            throw std::runtime_error(res.getDescription());
        }
    }
    else {
        throw std::runtime_error("unsupported type conversion requested");
    }
}

/**
 * @brief Retrieves a value from the given @p data area with the layout information of the @p layout
 * @remark The method is only able to copy @b bytes!
 *
 * @tparam ValueType The requested value type to retrieve the element in
 * @param data The data pointer of the memory
 * @param data_size The data size of @p data (in bytes)
 * @param layout The layout information
 * @return ValueType
 * @throws std::runtime_error if there is no valid conversion or the given @p layout is invalid
 */
template <typename ValueType>
ValueType readDeserializedBytes(const void* data, const size_t& data_size, const LeafLayout& layout)
{
    switch (layout.element_type) {
    case LeafElementType::let_bool: {
        return readBytes<bool, ValueType>(data, data_size, layout.byte_pos);
    }
    case LeafElementType::let_uint8: {
        return readBytes<uint8_t, ValueType>(data, data_size, layout.byte_pos);
    }
    case LeafElementType::let_int8: {
        return readBytes<int8_t, ValueType>(data, data_size, layout.byte_pos);
    }
    case LeafElementType::let_uint16: {
        return readBytes<uint16_t, ValueType>(data, data_size, layout.byte_pos);
    }
    case LeafElementType::let_int16: {
        return readBytes<int16_t, ValueType>(data, data_size, layout.byte_pos);
    }
    case LeafElementType::let_uint32: {
        return readBytes<uint32_t, ValueType>(data, data_size, layout.byte_pos);
    }
    case LeafElementType::let_int32: {
        return readBytes<int32_t, ValueType>(data, data_size, layout.byte_pos);
    }
    case LeafElementType::let_uint64: {
        return readBytes<uint64_t, ValueType>(data, data_size, layout.byte_pos);
    }
    case LeafElementType::let_int64: {
        return readBytes<int64_t, ValueType>(data, data_size, layout.byte_pos);
    }
    case LeafElementType::let_float: {
        return readBytes<float, ValueType>(data, data_size, layout.byte_pos);
    }
    case LeafElementType::let_double: {
        return readBytes<double, ValueType>(data, data_size, layout.byte_pos);
    }
    default: // all others are not supported
    {
        break;
    }
    }
    throw std::runtime_error("invalid type");
}

/**
 * @brief Retrieves a value from the given @p data area with the layout information of the @p layout
 *
 * @tparam ValueType The requested value type to retrieve the element in
 * @param data The data pointer of the memory
 * @param data_size The data size of @p data (in bytes)
 * @param layout The layout information
 * @return ValueType
 * @throws std::runtime_error if there is no valid conversion or the given @p layout is invalid
 */
template <typename ValueType>
ValueType readSerializedBits(const void* data, const size_t& data_size, const LeafLayout& layout)
{
    const size_t bit_pos = layout.byte_pos * 8 + layout.bit_pos;
    const size_t bit_size = layout.bit_size;
    const a_util::memory::Endianess byte_order =
        ((layout.data_flags & static_cast<uint8_t>(LeafDataRepresentation::serialized_be)) ==
         static_cast<uint8_t>(LeafDataRepresentation::serialized_be)) ?
            a_util::memory::bit_big_endian :
            a_util::memory::bit_little_endian;
    switch (layout.element_type) {
    case LeafElementType::let_bool: {
        return readBits<bool, ValueType>(data, data_size, bit_pos, bit_size, byte_order);
    }
    case LeafElementType::let_uint8: {
        return readBits<uint8_t, ValueType>(data, data_size, bit_pos, bit_size, byte_order);
    }
    case LeafElementType::let_int8: {
        return readBits<int8_t, ValueType>(data, data_size, bit_pos, bit_size, byte_order);
    }
    case LeafElementType::let_uint16: {
        return readBits<uint16_t, ValueType>(data, data_size, bit_pos, bit_size, byte_order);
    }
    case LeafElementType::let_int16: {
        return readBits<int16_t, ValueType>(data, data_size, bit_pos, bit_size, byte_order);
    }
    case LeafElementType::let_uint32: {
        return readBits<uint32_t, ValueType>(data, data_size, bit_pos, bit_size, byte_order);
    }
    case LeafElementType::let_int32: {
        return readBits<int32_t, ValueType>(data, data_size, bit_pos, bit_size, byte_order);
    }
    case LeafElementType::let_uint64: {
        return readBits<uint64_t, ValueType>(data, data_size, bit_pos, bit_size, byte_order);
    }
    case LeafElementType::let_int64: {
        return readBits<int64_t, ValueType>(data, data_size, bit_pos, bit_size, byte_order);
    }
    case LeafElementType::let_float: {
        return readBits<float, ValueType>(data, data_size, bit_pos, bit_size, byte_order);
    }
    case LeafElementType::let_double: {
        return readBits<double, ValueType>(data, data_size, bit_pos, bit_size, byte_order);
    }
    default: // all others are not supported
    {
        break;
    }
    }
    throw std::runtime_error("invalid type");
}

/**
 * @brief Writes a value to the given @p data area with the layout information of the @p layout
 *
 * @tparam ValueType The value type to set the elements content from
 * @param data The data pointer of the memory
 * @param data_size The data size of @p data (in bytes)
 * @param layout The layout information
 * @param value The value to set the element
 * @throws std::runtime_error if there is no valid conversion or the given @p layout is invalid
 */
template <typename ValueType>
void writeDeserializedBytes(void* data,
                            const size_t& data_size,
                            const LeafLayout& layout,
                            const ValueType& value)
{
    switch (layout.element_type) {
    case LeafElementType::let_bool: {
        return writeBytes<bool, ValueType>(data, data_size, layout.byte_pos, value);
    }
    case LeafElementType::let_uint8: {
        return writeBytes<uint8_t, ValueType>(data, data_size, layout.byte_pos, value);
    }
    case LeafElementType::let_int8: {
        return writeBytes<int8_t, ValueType>(data, data_size, layout.byte_pos, value);
    }
    case LeafElementType::let_uint16: {
        return writeBytes<uint16_t, ValueType>(data, data_size, layout.byte_pos, value);
    }
    case LeafElementType::let_int16: {
        return writeBytes<int16_t, ValueType>(data, data_size, layout.byte_pos, value);
    }
    case LeafElementType::let_uint32: {
        return writeBytes<uint32_t, ValueType>(data, data_size, layout.byte_pos, value);
    }
    case LeafElementType::let_int32: {
        return writeBytes<int32_t, ValueType>(data, data_size, layout.byte_pos, value);
    }
    case LeafElementType::let_uint64: {
        return writeBytes<uint64_t, ValueType>(data, data_size, layout.byte_pos, value);
    }
    case LeafElementType::let_int64: {
        return writeBytes<int64_t, ValueType>(data, data_size, layout.byte_pos, value);
    }
    case LeafElementType::let_float: {
        return writeBytes<float, ValueType>(data, data_size, layout.byte_pos, value);
    }
    case LeafElementType::let_double: {
        return writeBytes<double, ValueType>(data, data_size, layout.byte_pos, value);
    }
    default: // all others are not supported
    {
        break;
    }
    }
    throw std::runtime_error("invalid type");
}

/**
 * @brief Writes a value to the given @p data area with the layout information of the @p layout
 * @remark The method is only able to copy @b bytes!
 *
 * @tparam ValueType The value type to set the elements content from
 * @param data The data pointer of the memory
 * @param data_size The data size of @p data (in bytes)
 * @param layout The layout information
 * @param value The value to set the element
 * @throws std::runtime_error if there is no valid conversion or the given @p layout is invalid
 */
template <typename ValueType>
void writeSerializedBits(void* data,
                         const size_t& data_size,
                         const LeafLayout& layout,
                         const ValueType& value)
{
    const size_t bit_pos = layout.byte_pos * 8 + layout.bit_pos;
    const size_t bit_size = layout.bit_size;
    const a_util::memory::Endianess byte_order =
        ((layout.data_flags & static_cast<uint8_t>(LeafDataRepresentation::serialized_be)) ==
         static_cast<uint8_t>(LeafDataRepresentation::serialized_be)) ?
            a_util::memory::bit_big_endian :
            a_util::memory::bit_little_endian;
    switch (layout.element_type) {
    case LeafElementType::let_bool: {
        return writeBits<bool, ValueType>(data, data_size, bit_pos, bit_size, byte_order, value);
    }
    case LeafElementType::let_uint8: {
        return writeBits<uint8_t, ValueType>(data, data_size, bit_pos, bit_size, byte_order, value);
    }
    case LeafElementType::let_int8: {
        return writeBits<int8_t, ValueType>(data, data_size, bit_pos, bit_size, byte_order, value);
    }
    case LeafElementType::let_uint16: {
        return writeBits<uint16_t, ValueType>(
            data, data_size, bit_pos, bit_size, byte_order, value);
    }
    case LeafElementType::let_int16: {
        return writeBits<int16_t, ValueType>(data, data_size, bit_pos, bit_size, byte_order, value);
    }
    case LeafElementType::let_uint32: {
        return writeBits<uint32_t, ValueType>(
            data, data_size, bit_pos, bit_size, byte_order, value);
    }
    case LeafElementType::let_int32: {
        return writeBits<int32_t, ValueType>(data, data_size, bit_pos, bit_size, byte_order, value);
    }
    case LeafElementType::let_uint64: {
        return writeBits<uint64_t, ValueType>(
            data, data_size, bit_pos, bit_size, byte_order, value);
    }
    case LeafElementType::let_int64: {
        return writeBits<int64_t, ValueType>(data, data_size, bit_pos, bit_size, byte_order, value);
    }
    case LeafElementType::let_float: {
        return writeBits<float, ValueType>(data, data_size, bit_pos, bit_size, byte_order, value);
    }
    case LeafElementType::let_double: {
        return writeBits<double, ValueType>(data, data_size, bit_pos, bit_size, byte_order, value);
    }
    default: // all others are not supported
    {
        break;
    }
    }
    throw std::runtime_error("invalid type");
}

} // namespace detail

#if defined(__GNUC__) && (__GNUC__ == 5) && defined(__QNX__)
#pragma GCC diagnostic warning                                                                     \
    "-Wattributes" // standard type attributes are ignored when used in templates
#endif             // defined(__GNUC__) && (__GNUC__ == 5) && defined(__QNX__)

/**
 * Value getter to retrieve the current value of the given element as a @p ValueType if supported
 * from a data area.
 * @tparam ValueType The value type supported (by default bool, all int types, float, double and
 *                   std::string are supported).
 * @remark If you want to support your own data type, then create a specialization of this template.
 */
template <typename ValueType>
struct LeafValueGetter {
    /**
     * Returns the current value of the given element as a @p ValueType if supported
     * @param leaf_layout The small leaf layout of the element to get.
     * @param data The data area to retrieve the value from
     * @param data_size The data area size in bytes
     * @return ValueType Returns the current value as ValueType if supported.
     * @throws std::runtime_error if there is no valid conversion or the given @p layout is invalid
     */
    static ValueType getValue(const void* data,
                              const size_t data_size,
                              const LeafLayout& leaf_layout)
    {
        if ((leaf_layout.data_flags & static_cast<uint8_t>(LeafDataRepresentation::serialized)) ==
            static_cast<uint8_t>(LeafDataRepresentation::serialized)) {
            return detail::readSerializedBits<ValueType>(data, data_size, leaf_layout);
        }
        else {
            return detail::readDeserializedBytes<ValueType>(data, data_size, leaf_layout);
        }
    }
};

/**
 * Value setter to set the current value of the given element from a type @p ValueType if supported
 * @tparam CodecType The type of the codec.
 * @tparam ValueType The value type supported (by default bool, all int types, float and double are
 * supported).
 * @remark If you want to support your own data type, then create a specialization of this template.
 */
template <typename ValueType>
struct LeafValueSetter {
    /**
     * Sets the current value if the given element as a @p ValueType if supported.
     * @param leaf_layout The small leaf layout of the element to get.
     * @param data The data area to set the value to
     * @param data_size The data area size in bytes
     * @param[in] value The value to set as ValueType, if supported.
     * @throws std::runtime_error if there is no valid conversion or the given @p layout is invalid
     */
    static void setValue(void* data,
                         const size_t data_size,
                         const LeafLayout& leaf_layout,
                         const ValueType& value)
    {
        if ((leaf_layout.data_flags & static_cast<uint8_t>(LeafDataRepresentation::serialized)) ==
            static_cast<uint8_t>(LeafDataRepresentation::serialized)) {
            detail::writeSerializedBits<ValueType>(data, data_size, leaf_layout, value);
        }
        else {
            detail::writeDeserializedBytes<ValueType>(data, data_size, leaf_layout, value);
        }
    }
};
} // namespace codec
} // namespace ddl

#ifdef DDL_NO_SUPPORT_FOR_CONSTEXPR_CONDITION
#undef DDL_NO_SUPPORT_FOR_CONSTEXPR_CONDITION
#endif

#endif
