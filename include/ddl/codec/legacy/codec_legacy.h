/**
 * @file
 * Definition of old Codec legacy header for old Codec API.
 *
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */

#ifndef DDL_CODEC_LEGACY_CLASS_HEADER
#define DDL_CODEC_LEGACY_CLASS_HEADER

#include <a_util/preprocessor/deprecated.h> // DEV_ESSENTIAL_DEPRECATED()
#include <a_util/result.h>
#include <ddl/codec/codec.h>
#include <ddl/codec/legacy/static_codec_legacy.h>

/// @cond INTERNAL_DOCUMENTATION
#define DEV_ESSENTIAL_DEPRECATED_DECODER                                                           \
    DEV_ESSENTIAL_DEPRECATED("Use the new ddl::codec::Decoder() instead.")

#define DEV_ESSENTIAL_DEPRECATED_CODEC                                                             \
    DEV_ESSENTIAL_DEPRECATED("Use the new ddl::codec::Codec() instead.")
/// @endcond

namespace ddl {
class Codec;

/**
 * Legacy offset structure for binary compatiblity
 */
struct LegacyOffsets {
    size_t deserialized; ///< legacy value
    size_t serialized;   ///< legacy value
};

/**
 * Decoder for dynamic structures defined by a DataDefinition definition.
 * @dev_essential_deprecated Use the new @ref ddl::codec::Decoder() instead.
 */
class Decoder : public StaticDecoder {
public:
    /**
     * Default constructor.
     */
    DEV_ESSENTIAL_DEPRECATED_DECODER
    Decoder() = default;

    /**
     * Move constructor.
     */
    Decoder(Decoder&&) = default;

    /**
     * Move assignment operator.
     */
    DEV_ESSENTIAL_DEPRECATED_DECODER
    Decoder& operator=(Decoder&&) = default;

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
     * @copydoc StaticDecoder::getElementCount
     */
    size_t getElementCount() const override;

    /**
     * @param[in] rep The data representation for which the buffer size should be returned.
     * @return The size of the structure in the requested data representation.
     */
    size_t getBufferSize(DataRepresentation rep = deserialized) const;

    /**
     * Create a new codec with the current dynamic structure layout for a new data buffer.
     * @param[in] data The pointer to the new raw data.
     * @param[in] data_size The size of the new raw data.
     * @param[in] rep The representation that the data should be encoded in.
     * @return A codec.
     */
    Codec makeCodecFor(void* data, size_t data_size, DataRepresentation rep) const;

protected:
    friend class CodecFactory;
    /// this virtual pointer must stay for binary compatiblity. @internal
    virtual const void* getLayoutElement(size_t index) const;
    /// For internal use only. @internal
    Decoder(const std::shared_ptr<LegacyCodecAccess>& legacy_codec_access);

    /// This CTOR must stay there for binary compatible reason due to an inline usage within old
    /// CodecFactory
    Decoder(std::shared_ptr<const StructLayout>,
            const void* data,
            size_t data_size,
            DataRepresentation rep);

private:
    /// For internal use only. @internal
    /// For binary compatibility this must stay
    std::shared_ptr<std::vector<int32_t>> _compat_dynamic_elements;
    /// For internal use only. @internal
    /// For binary compatibility this must stay
    LegacyOffsets _compat_buffer_sizes;
};

/**
 * Decoder for dynamic structures defined by a DataDefinition definition.
 * Currently the amount of dynamic elements is determined during construction
 * only (by the current values in the structure).
 * @dev_essential_deprecated Use the new @ref ddl::codec::Codec() instead.
 */
class Codec : public Decoder {
public:
    /**
     * Default constructor.
     */
    DEV_ESSENTIAL_DEPRECATED_CODEC
    Codec() = default;

    /**
     * Move constructor.
     */
    Codec(Codec&&) = default;

    /**
     * Move assignment operator.
     */
    DEV_ESSENTIAL_DEPRECATED_CODEC
    Codec& operator=(Codec&&) = default;

    /**
     * Sets the current value of the given element by copying its data
     * from the passed-in location.
     * @param[in] index The index of the element.
     * @param[in] value The location where the data should be copied from.
     * @retval ERR_INVALID_INDEX Invalid element index.
     */
    a_util::result::Result setElementValue(size_t index, const void* value);

    /**
     * Sets the current value of the given element to the given value.
     * @param[in] index The index of the element.
     * @param[in] value The value.
     * @retval ERR_INVALID_INDEX Invalid element index.
     */
    a_util::result::Result setElementValue(size_t index, const a_util::variant::Variant& value);

    /**
     * @param[in] index The index of the element.
     * @return A pointer to the element or NULL in case of an error.
     */
    void* getElementAddress(size_t index);
    using StaticDecoder::getElementAddress;

    /**
     * Sets all elements to their constant values defined in the DataDefinition.
     * @return Standard result.
     */
    a_util::result::Result setConstants();

protected:
    friend class CodecFactory;
    friend class Decoder;
    /// For internal use only. @internal
    /// This CTOR must stay there for binary compatible reason due to an inline usage within old
    /// CodecFactory
    Codec(std::shared_ptr<const StructLayout> layout,
          void* data,
          size_t data_size,
          DataRepresentation rep);
    /// For internal use only. @internal
    Codec(codec::Codec&& codec);
};

} // namespace ddl

#endif // DDL_CODEC_LEGACY_CLASS_HEADER
