/**
 * @file
 * Implementation of the decoder and coder value accesss setter and getter
 *
 * Copyright @ 2022 VW Group. All rights reserved.
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

#ifndef DDL_PREVIEW_VALUE_ACCESS_PRIVATE_CLASS_HEADER
#define DDL_PREVIEW_VALUE_ACCESS_PRIVATE_CLASS_HEADER

#include <a_util/result.h>
#include <ddl/codec/leaf_value_access.h>

#include <vector>

namespace ddl {
namespace codec {

#if defined(__GNUC__) && (__GNUC__ == 5) && defined(__QNX__)
#pragma GCC diagnostic ignored                                                                     \
    "-Wattributes" // standard type attributes are ignored when used in templates
#endif             // defined(__GNUC__) && (__GNUC__ == 5) && defined(__QNX__)

namespace detail {

template <typename ValueType>
struct SupportedGetterValueType
    : public std::is_convertible<a_util::variant::Variant,
                                 a_util::underlying_type_or_type_t<ValueType>> {
};

template <typename ValueType>
struct SupportedSetterValueType
    : public std::is_convertible<a_util::underlying_type_or_type_t<ValueType>,
                                 a_util::variant::Variant> {
};

} // namespace detail

#if defined(__GNUC__) && (__GNUC__ == 5) && defined(__QNX__)
#pragma GCC diagnostic warning                                                                     \
    "-Wattributes" // standard type attributes are ignored when used in templates
#endif             // defined(__GNUC__) && (__GNUC__ == 5) && defined(__QNX__)

/**
 * Value getter to retrieve the current value of the given element as a @p ValueType if supported
 * @tparam DecoderType The type of the decoder.
 * @tparam ValueType The value type supported (by default bool, all int types, float, double and
 *                   std::string are supported).
 * @remark If you want to support your own data type, then create a specialization of this template.
 */
template <typename DecoderType, typename ValueType>
struct ValueGetter {
    /**
     * Returns the current value of the given element as a @p ValueType if supported
     * @param[in] decoder The decoder.
     * @param[in] codec_index The index of the element.
     * @param[out] value ValueType Returns the current value as ValueType if supported.
     * @throw throws std::runtime_error if not found by decoder.
     */
    static void getValue(const DecoderType& decoder,
                         const CodecIndex& codec_index,
                         ValueType& value)
    {
        static_assert(detail::SupportedGetterValueType<ValueType>::value,
                      "Unsupported type for decode. "
                      "By default bool, all int types, float, double, std::string is supported. "
                      "To support your own data type, create a specialization of this template");
        a_util::variant::Variant variant_value;
        decoder.getElementVariantValue(codec_index, variant_value);
        value = static_cast<ValueType>(
            static_cast<a_util::underlying_type_or_type_t<ValueType>>(variant_value));
    }
};

/**
 * Value getter specialization for @c std::string
 * @tparam DecoderType The decoder type.
 */
template <typename DecoderType>
struct ValueGetter<DecoderType, std::string> {
    /**
     * Returns the current value of the given element as a std::string
     * @param[in] decoder The decoder.
     * @param[in] codec_index The index of the element.
     * @param[out] value std::string Returns the current value as std::string.
     * @throw throws std::runtime_error if not found by decoder.
     */
    static void getValue(const DecoderType& decoder,
                         const CodecIndex& codec_index,
                         std::string& value)
    {
        value = decoder.getElementStringValue(codec_index);
    }
};

/**
 * Value setter to set the current value of the given element from a type @p ValueType if supported
 * @tparam CodecType The type of the codec.
 * @tparam ValueType The value type supported (by default bool, all int types, float, double and
 *                   std::string are supported).
 * @remark If you want to support your own data type, then create a specialization of this template.
 */
template <typename CodecType, typename ValueType>
struct ValueSetter {
    /**
     * Sets the current value if the given element as a @p ValueType if supported.
     * @param[in] codec The codec.
     * @param[in] codec_index The index of the element.
     * @param[in] value The value to set as ValueType, if supported.
     * @throw throws std::runtime_error if not found from codec.
     */
    static void setValue(CodecType& codec, const CodecIndex& codec_index, const ValueType& value)
    {
        static_assert(detail::SupportedSetterValueType<ValueType>::value,
                      "Unsupported type for decode. "
                      "By default bool, all int types, float, double, std::string is supported. "
                      "To support your own data type, create a specialization of this template");
        codec.setElementVariantValue(
            codec_index,
            a_util::variant::Variant(
                static_cast<a_util::underlying_type_or_type_t<ValueType>>(value)));
    }
};

/**
 * Value setter specialization for @c std::string
 * @tparam CodecType The codec type.
 */
template <typename CodecType>
struct ValueSetter<CodecType, std::string> {
    /**
     * Sets the value of the given element from std::string.
     * @param[in] codec The codec.
     * @param[in] codec_index The index of the element.
     * @param[in] value The value to set as std::string.
     * @throw throws std::runtime_error if not found by codec
     */
    static void setValue(CodecType& codec, const CodecIndex& codec_index, const std::string& value)
    {
        codec.setElementStringValue(codec_index, value);
    }
};

/**
 * Value getter to retrieve the current value of the given element as a @p ValueType if supported
 * @tparam DecoderType The type of the decoder.
 * @tparam ValueType The value type supported (by default bool, all int types, float, double and
 *                   std::string are supported).
 * @remark If you want to support your own data type, then create a specialization of this
 * ValueGetter.
 */
template <typename DecoderType,
          typename ValueType,
          bool is_supported_leaf_type = detail::SupportedLeafValueTypeExplicit<ValueType>::value>
struct ValueGetterSelect {
    /**
     * Returns the current value of the given element as a @p ValueType if supported
     * @param[in] codec_index The codec index of the element.
     * @param[in] decoder The decoder to retrieve the value from.
     * @return ValueType Returns the current value as ValueType if supported.
     * @throw throws std::runtime_error if not found by decoder.
     */
    static ValueType getValue(const DecoderType& decoder, const CodecIndex& codec_index)
    {
        ValueType value;
        ValueGetter<DecoderType, ValueType>::getValue(decoder, codec_index, value);
        return value;
    }
};

/**
 * Value getter to retrieve the current value of the given element as a @p ValueType if supported
 * @tparam DecoderType The type of the decoder.
 * @tparam ValueType The value type supported (by default bool, all int types, float, double and
 *                   std::string are supported).
 * @remark If you want to support your own data type, then create a specialization of this
 * ValueGetter.
 */
template <typename DecoderType, typename ValueType>
struct ValueGetterSelect<DecoderType, ValueType, true> {
    /**
     * Returns the current value of the given element as a @p ValueType if supported
     * @param[in] codec_index The codec index of the element.
     * @param[in] decoder The decoder to retrieve the value from.
     * @return ValueType Returns the current value as ValueType if supported.
     * @throw throws std::runtime_error if not found by decoder.
     */
    static ValueType getValue(const DecoderType& decoder, const CodecIndex& codec_index)
    {
        LeafLayout leaf_layout;
        if (LeafCodecIndex::convertToLeafLayout<false>(
                codec_index, leaf_layout, decoder.getRepresentation())) {
            return LeafValueGetter<ValueType>::getValue(
                decoder.getData(), decoder.getDataSize(), leaf_layout);
        }
        else {
            ValueType value;
            ValueGetter<DecoderType, ValueType>::getValue(decoder, codec_index, value);
            return value;
        }
    }
};

/**
 * Value setter to set the value of the given element as a @p ValueType if supported
 * @tparam CodecType The type of the codec.
 * @tparam ValueType The value type supported (by default bool, all int types, float, double and
 *                   std::string are supported).
 * @remark If you want to support your own data type, then create a specialization of ValueSetter.
 */
template <typename CodecType,
          typename ValueType,
          bool is_supported_leaf_type = detail::SupportedLeafValueTypeExplicit<ValueType>::value>
struct ValueSetterSelect {
    /**
     * Sets the current value of the given element as a @p ValueType if supported
     * @param[in] codec_index The codec index of the element.
     * @param[in] codec The codec to set the value to.
     * @param[in] value the value as ValueType to set.
     * @throw throws std::runtime_error if not found by codec.
     */
    static void setValue(CodecType& codec, const CodecIndex& codec_index, const ValueType& value)
    {
        ValueSetter<CodecType, ValueType>::setValue(codec, codec_index, value);
    }
};

/**
 * Value setter to set the value of the given element as a @p ValueType if supported
 * @tparam CodecType The type of the codec.
 * @tparam ValueType The value type supported (by default bool, all int types, float, double and
 *                   std::string are supported).
 * @remark If you want to support your own data type, then create a specialization of ValueSetter.
 */
template <typename CodecType, typename ValueType>
struct ValueSetterSelect<CodecType, ValueType, true> {
    /**
     * Sets the current value of the given element as a @p ValueType if supported
     * @param[in] codec_index The codec index of the element.
     * @param[in] codec The codec to set the value to.
     * @param[in] value the value as ValueType to set.
     * @throw throws std::runtime_error if not found by codec.
     */
    static void setValue(CodecType& codec, const CodecIndex& codec_index, const ValueType& value)
    {
        LeafLayout leaf_layout;
        if (LeafCodecIndex::convertToLeafLayout<false>(
                codec_index, leaf_layout, codec.getRepresentation())) {
            LeafValueSetter<ValueType>::setValue(
                codec.getData(), codec.getDataSize(), leaf_layout, value);
        }
        else {
            ValueSetter<CodecType, ValueType>::setValue(codec, codec_index, value);
        }
    }
};

} // namespace codec
} // namespace ddl

#endif // DDL_PREVIEW_VALUE_ACCESS_PRIVATE_CLASS_HEADER
