/**
 * @file
 * Definition of old StaticCodec legacy header for old Codec API.
 *
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */

#ifndef DDL_STATIC_CODEC_LEGACY_CLASS_HEADER
#define DDL_STATIC_CODEC_LEGACY_CLASS_HEADER

#include <a_util/preprocessor/deprecated.h> // DEV_ESSENTIAL_DEPRECATED()
#include <ddl/codec/static_codec.h>

/// @cond INTERNAL_DOCUMENTATION
#define DEV_ESSENTIAL_DEPRECATED_STATIC_DECODER                                                    \
    DEV_ESSENTIAL_DEPRECATED("Use the new ddl::codec::StaticDecoder() instead.")

#define DEV_ESSENTIAL_DEPRECATED_STATIC_CODEC                                                      \
    DEV_ESSENTIAL_DEPRECATED("Use the new ddl::codec::StaticCodec() instead.")
/// @endcond

namespace ddl {

// forward declaration for legacy codec access
struct LegacyCodecAccess;
class StructLayout;

/**
 * Decoder for static structures defined by a DataDefinition definition.
 * @dev_essential_deprecated Use the new @ref ddl::codec::StaticDecoder() instead.
 */
class StaticDecoder {
public:
    /// virtual DTOR
    ~StaticDecoder() = default;
    /**
     * Default constructor
     */
    DEV_ESSENTIAL_DEPRECATED_STATIC_DECODER
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
    StaticDecoder(StaticDecoder&&) = default;

    /**
     * Move assignment operator.
     */
    DEV_ESSENTIAL_DEPRECATED_STATIC_DECODER
    StaticDecoder& operator=(StaticDecoder&&) = default;

    /**
     * @return Whether or not the decoder is valid.
     * @retval ERR_INVALID_ARG The passed data is not large enough.
     */
    virtual a_util::result::Result isValid() const;

    /**
     * @return The amount of elements contained in the data structure.
     */
    virtual size_t getElementCount() const;

    /**
     * Access information about an element.
     * @param[in] index The index of the element.
     * @param[out] element Pointer that will be updated to point to the element information.
     * @retval ERR_INVALID_INDEX Invalid element index.
     */
    a_util::result::Result getElement(size_t index, const StructElement*& element) const;

    /**
     * Returns the current value of the given element by copying its data
     * to the passed-in location.
     * @param[in] index The index of the element.
     * @param[out] value The location where the value should be copied to.
     * @retval ERR_INVALID_INDEX Invalid element index.
     */
    a_util::result::Result getElementValue(size_t index, void* value) const;

    /**
     * Returns the current value of the given element as a variant.
     * @param[in] index The index of the element.
     * @param[out] value The will be set to the current value.
     * @retval ERR_INVALID_INDEX Invalid element index.
     */
    a_util::result::Result getElementValue(size_t index, a_util::variant::Variant& value) const;

    /**
     * @param[in] index The index of the element.
     * @return A pointer to the element or NULL in case of an error.
     */
    const void* getElementAddress(size_t index) const;

    /**
     * @param[in] rep The data representation for which the buffer size should be returned.
     * @return The size of the structure in the requested data representation.
     */
    size_t getStaticBufferSize(DataRepresentation rep = deserialized) const;

    /**
     * @return The data representation which this decoder handles.
     */
    DataRepresentation getRepresentation() const;

protected:
    friend class CodecFactory;
    /// For internal use only. @internal
    /// This CTOR must stay there for binary compatible reason due to an inline usage within old
    /// CodecFactory
    StaticDecoder(std::shared_ptr<const StructLayout> layout,
                  const void* data,
                  size_t data_size,
                  DataRepresentation rep);

    /// For internal use only. @internal
    StaticDecoder(const std::shared_ptr<LegacyCodecAccess>& legacy_codec_access);

protected:
    /// For internal use only. @internal
    std::shared_ptr<LegacyCodecAccess> _legacy_access;
    /// For internal use only. @internal
    /// This must stay for binary compatibility
    const void* _compat_data;
    /// For internal use only. @internal
    /// This must stay for binary compatibility
    size_t _compat_data_size;
    /// For internal use only. @internal
    /// This must stay for binary compatibility
    const void* _compat_element_accessor;
};

/**
 * Codec for static structures defined by a DataDefinition definition.
 * @dev_essential_deprecated Use the new @ref ddl::codec::StaticCodec() instead.
 */
class StaticCodec : public StaticDecoder {
public:
    /**
     * Default constructor
     */
    DEV_ESSENTIAL_DEPRECATED_STATIC_CODEC
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
    StaticCodec(StaticCodec&&) = default;

    /**
     * Move assignment operator.
     */
    DEV_ESSENTIAL_DEPRECATED_STATIC_CODEC
    StaticCodec& operator=(StaticCodec&&) = default;

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
    /// For internal use only. @internal
    /// This CTOR must stay there for binary compatible reason due to an inline usage within old
    /// CodecFactory
    StaticCodec(std::shared_ptr<const StructLayout> layout,
                void* data,
                size_t data_size,
                DataRepresentation rep);
};

} // namespace ddl

#endif // DDL_STATIC_CODEC_LEGACY_CLASS_HEADER
