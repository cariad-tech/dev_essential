/**
 * @file
 * OO DataDefinition Common Design
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

#ifndef DD_TYPES_H_INCLUDED
#define DD_TYPES_H_INCLUDED

#include "ddl/utilities/dd_access_optional.h"

#include <string>

namespace ddl {

namespace dd {

/**
 * @brief Optional Size Type
 *
 */
using OptionalSize = utility::Optional<size_t>;

/**
 * @brief Classification of a Type
 *
 */
enum TypeOfType : uint8_t {
    /**
     * @brief the type is unknown
     *
     */
    invalid_type,
    /**
     * @brief the type is a data type (@ref DataType)
     *
     */
    data_type,
    /**
     * @brief the type is a enum type (@ref EnumType)
     *
     */
    enum_type,
    /**
     * @brief the type is a struct type (@ref StructType)
     *
     */
    struct_type,
    /**
     * @brief the type is an stream meta type (@ref StreamMetaType)
     *
     */
    stream_meta_type
};

/**
 * @brief Classification of unit
 *
 */
enum TypeOfUnit : uint8_t {
    /**
     * @brief the type of the unit is unknown
     *
     */
    invalid_unit,
    /**
     * @brief the unit is a unit (@ref Unit)
     *
     */
    unit,
    /**
     * @brief the unit is a base unit (@ref BaseUnit)
     *
     */
    base_unit
};

/**
 * @brief Alignment defintion
 *
 */
enum Alignment : size_t {
    e0 = 1, /**< for backward compatibility */
    e1 = 1, /**< Default alignment */
    e2 = 2,
    e4 = 4,
    e8 = 8,
    e16 = 16,
    e32 = 32,
    e64 = 64,
    e_invalid = 255
};

/**
 * @brief Alignment Conversion utility
 *
 */
class AlignmentConversion {
public:
    /**
     * @brief convert alignment to a string.
     *
     * @param alignment the alignment to convert
     * @return std::string
     */
    static std::string toString(Alignment alignment);
    /**
     * @brief convert a string to an alignment.
     *
     * @param alignment the alignment string
     * @param default_val if not set or invalid, the default alignment to return.
     * @return Alignment
     */
    static Alignment fromString(const std::string& alignment,
                                const Alignment& default_val = Alignment::e_invalid);
};

/**
 * @brief Alignment ConvValidation utility
 *
 */
class AlignmentValidation {
public:
    /**
     * @brief check if the Alignment is a valid value
     *
     * @param alignment the alignment to check
     * @return bool
     */
    static bool isValid(size_t alignment);
};

/**
 * Representation of the byteorder enumeration
 */
enum ByteOrder {
    platform_not_supported = 0x00,
    plattform_little_endian_8 = 0x01,
    platform_big_endian_8 = 0x02,
    e_noe = platform_not_supported,
    e_le = plattform_little_endian_8,
    e_be = platform_big_endian_8
};

/**
 * @brief ByteOrder Conversion utility
 *
 */
class ByteOrderConversion {
public:
    /**
     * @brief convert byte_order to a string.
     *
     * @param byte_order the byte_order to convert
     * @return std::string
     */
    static std::string toString(ByteOrder byte_order);
    /**
     * @brief convert a string to a byte_order.
     *
     * @param byte_order the byte_order string
     * @param default_val if not set or invalid, the default byte_order to return.
     * @return ByteOrder
     */
    static ByteOrder fromString(const std::string& byte_order,
                                const ByteOrder& default_val = ByteOrder::platform_not_supported);
};

/**
 * @brief Default ByteOrder Discovery utility class
 *
 */
class ByteOrderDefault {
public:
    /**
     * @brief Get the current Platform Byteorder
     *
     * @return ByteOrder
     */
    static ByteOrder getPlatformDefault();
};

/**
 * @brief DDL Version.
 *
 */
class Version {
public:
    /**
     * @brief default CTOR
     *
     */
    Version();
    /**
     * @brief CTOR - Version from string
     *
     * @param version
     */
    Version(const std::string& version);

    /**
     * @brief CTOR
     *
     * @param major the major value
     * @param minor the minor value
     */
    Version(uint32_t major, uint32_t minor);

    /**
     * @brief Get the Major value
     *
     * @return uint32_t
     */
    uint32_t getMajor() const;
    /**
     * @brief Get the Minor value
     *
     * @return uint32_t
     */
    uint32_t getMinor() const;

    /**
     * @brief the vrsion as string
     *
     * @return std::string
     */
    std::string toString() const;

    /**
     * @brief is valid or not
     *
     * @return true is valid
     * @return false is not valid
     */
    bool isValidVersion() const;

    /**
     * @brief Get the Default Version
     *
     * @return const Version&
     */
    static const Version& getDefaultVersion()
    {
        return ddl_version_current;
    }

    /* ignore patch version for all comparisons */
    /// @cond nodoc
    bool operator==(const Version& other) const;
    bool operator!=(const Version& other) const;
    bool operator>(const Version& other) const;
    bool operator<(const Version& other) const;
    bool operator<=(const Version& other) const;
    bool operator>=(const Version& other) const;
    /// @endcond nodoc

    /// invalid Version
    static const Version ddl_version_invalid;
    /// version not set Version
    static const Version ddl_version_notset;
    /// Language Version 1.0
    static const Version ddl_version_10;
    /// Language Version 1.0+
    static const Version ddl_version_11;
    /// Language Version 1.2
    static const Version ddl_version_12;
    /// Language Version 2.0
    static const Version ddl_version_20;
    /// Language Version 3.0
    static const Version ddl_version_30;
    /// Language Version 4.0
    static const Version ddl_version_40;
    /// Language Version 4.1
    static const Version ddl_version_41;
    /// the newest version (currently it is @ref ddl_version_41).
    static const Version ddl_version_current;

private:
    uint32_t _major;
    uint32_t _minor;
};

/**
 * @brief Version conversion utility class.
 *
 */
class VersionConversion {
public:
    /**
     * @brief Version to string conversion
     *
     * @param version the version to convert
     * @return std::string
     */
    static std::string toString(const Version& version);
    /**
     * @brief Creates the Version from a string
     *
     * @param version the version as string to convert from
     * @param default_val if valid version is not found, this value is default
     * @return Version
     */
    static Version fromString(const std::string& version,
                              const Version& default_val = Version::ddl_version_notset);
};

/**
 * Sinc DDL 2.0 it is possible to define arraysize with a string that indicates a dynamic array.
 * The string it self is a elementname of the containing struct type.
 */
class ArraySize {
public:
    /**
     * default CTOR.
     */
    ArraySize() = default;
    /**
     * CTOR
     * @param array_size sets the array size
     */
    ArraySize(size_t array_size);
    /**
     * assignment operator
     * @param array_size sets the array size
     * @return ArraySize&
     */
    ArraySize& operator=(size_t array_size);
    /**
     * CTOR
     * @param array_size_element_name sets the dynamic array size elment name
     */
    ArraySize(const std::string& array_size_element_name);
    /**
     * assignment operator
     * @param array_size_element_name sets the dynamic array size elment name
     * @return ArraySize&
     */
    ArraySize& operator=(const std::string& array_size_element_name);
    /**
     * set the array size
     * @param array_size sets the array size
     */
    void setArraySizeValue(size_t array_size);
    /**
     * get the array size
     */
    size_t getArraySizeValue() const;
    /**
     * sets the dynamic array size
     * @param array_size_element_name sets the dynamic array size elment name
     */
    void setArraySizeElementName(const std::string& array_size_element_name);
    /**
     * gets the dynamic array size
     * @return std::string&
     */
    const std::string& getArraySizeElementName() const;
    /**
     * indicates if this is a dynamic erray size.
     */
    bool isDynamicArraySize() const;

    /**
     * compares the Array size with the other
     * @param [other] the other array size
     * @retval true if they are equal
     * @retval false if they are not equal
     */
    bool operator==(const ArraySize& other) const;
    /**
     * compares the Array size with the other
     * @param [other] the other array size
     * @retval false if they are equal
     * @retval true if they are not equal
     */
    bool operator!=(const ArraySize& other) const;

private:
    size_t _array_size = {1};
    std::string _array_size_element_name = {};
};

} // namespace dd
} // namespace ddl

#endif // DD_TYPES_H_INCLUDED