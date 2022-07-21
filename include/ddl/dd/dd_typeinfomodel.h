/**
 * @file
 * OO DataDefinition Type Info Model
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

#ifndef DD_TYPEINFOMODEL_H_INCLUDED
#define DD_TYPEINFOMODEL_H_INCLUDED

#include <ddl/datamodel/datamodel_datadefinition.h>
#include <ddl/dd/dd_infomodel_type.h>

#include <string>

namespace ddl {

namespace dd {

/**
 * TypeInfo model will check for type.
 * The TypeInfo is needed to:
 * \li set up the byte positions
 * \li set references to the types
 * \li fast calculation of sizes for the coder
 * \li set up dynamic content information
 */
class TypeInfo : public datamodel::Info<TypeInfo> {
public:
    /// type info ID to use it in @ref datamodel::Info.
    static constexpr const uint8_t INFO_TYPE_ID = dd::InfoType::type_info;
    /**
     * @brief default CTOR
     *
     */
    TypeInfo() = default;
    /**
     * @brief CTOR to create a Type Info that is initial updated with data_type information.
     *
     * @param data_type the data type to initialize the info for
     * @param parent_dd the parent DD to retrieve dependencies from
     */
    TypeInfo(const datamodel::DataType& data_type, datamodel::DataDefinition& parent_dd);
    /**
     * @brief CTOR to create a Type Info that is initial updated with enum_type information.
     *
     * @param enum_type the data type to initialize the info for
     * @param parent_dd the parent DD to retrieve dependencies from
     */
    TypeInfo(const datamodel::EnumType& enum_type, datamodel::DataDefinition& parent_dd);

    /**
     * @brief Get the Type Bit Size
     *
     * @return size_t the bit size of the type
     */
    size_t getTypeBitSize() const;
    /**
     * @brief Get the Type Alignment
     *
     * @return size_t alignment
     */
    size_t getTypeAlignment() const;
    /**
     * @brief Get the Type Byte Size (depending on ddl version in structs this is aligned or not)
     *
     * @return size_t
     */
    size_t getTypeByteSize() const;

    /**
     * @brief Get the Type Aligned Byte Size
     *
     * @return size_t the byte size
     */
    size_t getTypeAlignedByteSize() const;

    /**
     * @brief Get the Type Unaligned Byte Size
     *
     * @return size_t the byte size
     */
    size_t getTypeUnalignedByteSize() const;

    /**
     * @brief the content is valid, all references to other DD Objects (for calculation) are valid.
     * This is set to false also if a recursion loop in the definition was detected.
     *
     * @return true the content is valid
     * @return false the content is not valid
     */
    bool isValid() const;
    /**
     * @brief contains dynamic content (usually this is only possible for struct_type)
     *
     * @return true the struct_type contains dynamic content
     * @return false the struct_type does not contain dynamic content
     */
    bool isDynamic() const;

    /**
     * @brief Updates the content for a data_type.
     *
     * @param data_type the data_type
     * @param parent_dd the parent DD
     */
    void update(const datamodel::DataType& data_type, datamodel::DataDefinition& parent_dd);
    /**
     * @brief Updates the content for a enum_type.
     *
     * @param enum_type the enum_type
     * @param parent_dd the parent DD
     */
    void update(const datamodel::EnumType& enum_type, datamodel::DataDefinition& parent_dd);
    /**
     * @brief Updates the content for a struct_type. It will also create or update the @ref
     * ElementTypeInfo for all contained elements (if necessary). To force a whole recalculation of
     * all elements and its there TypeInfo set \p recalculate_all to true. This function will detect
     * recursions!
     *
     * @param struct_type the struct_type
     * @param parent_dd the parent DD
     * @param recalculate_all force recalulation of all elements and their depending TypeInfo.
     */
    void update(datamodel::StructType& struct_type,
                datamodel::DataDefinition& parent_dd,
                bool recalculate_all = false);

private:
    size_t _type_bit_size = 0;
    size_t _type_alignment = 0;
    size_t _type_byte_size = 0;
    size_t _type_aligned_byte_size = 0;
    size_t _type_unaligned_byte_size = 0;
    bool _is_dynamic = false;
    bool _is_valid = false;

    bool _already_discovering = false;
};

/**
 * Element Size Info is important to get the calculate the TypeInfo for StructType.
 * It will only updated by the TypeInfo::update method for StructType.
 */
class ElementTypeInfo : public datamodel::Info<ElementTypeInfo> {
public:
    /**
     * @brief definiton of info type to use the @ref datamodel::Info template.
     *
     */
    static constexpr const uint8_t INFO_TYPE_ID = dd::InfoType::element_type_info;

    /**
     * @brief Get the Deserialized Byte Pos
     * This position is always invalid for positions after a dynamic content (@ref isAfterDynamic)
     *
     * @param array_pos this ould calculate and return the deserialized pos for the aligned array
     * pos.
     * @return OptionalSize the position if valid.
     */
    OptionalSize getDeserializedBytePos(size_t array_pos = 0) const;
    /**
     * @brief Get the Deserialized Byte Size
     * always aligned, depending on the struct_ddl_version
     *
     * @return OptionalSize the deserialize size (aligned for ddl version >= 3.0, not aligned for
     * other)
     * @see getDeserializedTypeAlignedByteSize
     */
    OptionalSize getDeserializedByteSize() const;
    /**
     * @brief Get the Deserialized Type Byte Size
     * usually this is the same like @ref getDeserializedByteSize, but only for array_size = 1
     * always aligned, depending on the struct_ddl_version >= 3.0
     *
     * @return size_t the byte size of the type only
     */
    size_t getDeserializedTypeByteSize() const;
    /**
     * @brief Get the Deserialized Type Byte Size
     * usually this is the same like @ref getDeserializedByteSize, but only for array_size = 1
     * usually this is the size for all elements of an array except the last one on
     * struct_ddl_version >= 3.0
     *
     * @return size_t the byte size of the type only
     */
    size_t getDeserializedTypeAlignedByteSize() const;

    /**
     * @brief Get the Serialized Byte Pos
     * This position is always invalid for positions after a dynamic content (@ref isAfterDynamic)
     *
     * @param array_pos the position in the array to calculate the offset for
     * @return OptionalSize the position if valid
     */
    OptionalSize getSerializedBytePos(size_t array_pos = 0) const;
    /**
     * @brief Get the Serialized Bit Pos of the element as bit offset to the beginning of the struct
     * This position is always invalid for positions after a dynamic content (@ref isAfterDynamic)
     *
     * @param array_pos the position in the array to calculate the offset for
     * @return OptionalSize the position if valid
     */
    OptionalSize getSerializedBitOffset(size_t array_pos = 0) const;
    /**
     * @brief Get the Serialized Bit Size
     *
     * @return size_t the bitsize if set or the one of the type
     */
    size_t getSerializedBitSize() const;
    /**
     * @brief Get the Serialized Type Bit Size
     *
     * @return size_t size of the type (only size for array size=1)
     */
    size_t getSerializedTypeBitSize() const;

    /**
     * @brief Determines if the element itself or if it is element of a struct_type defines a
     * dynamic array.
     *
     * @return true The elemnt is a dynmic array or is a instance of a struct_type that contains
     * dynamic arrays
     * @return false this is a static content, sizes can be calculated.
     */
    bool isDynamic() const;
    /**
     * @brief The element is after a dynamic content.
     * All positions can only be calculated if the sizes of the elements before are known.
     *
     * @return true the element is after a dynamic content.
     * @return false the element is not after a dynamic content
     */
    bool isAfterDynamic() const;
    /**
     * @brief Determines if all size can be calculated, all references are known and the
     * dependencies are well defined. Usually the @ref ddl::dd::DataDefinition implementation
     * prevent this, but
     *
     * @return true it is valid
     * @return false it is not valid
     */
    bool isValid() const;

    /**
     * @brief Get the Type Of Type object
     * Fast access to the type information.
     *
     * @return TypeOfType
     */
    TypeOfType getTypeOfType() const;
    /**
     * @brief Get the Struct Type Object
     * @return returns a valid pointer if the type of this element is a struct_type and the
     * infomation is valid (see @ref isValid())!
     * @return const std::shared_ptr<const datamodel::StructType>
     */
    std::shared_ptr<const datamodel::StructType> getStructType() const;
    /**
     * @brief Get the Enum Type object
     * @return returns a valid pointer if the type of this element is a enum_type and the infomation
     * is valid (see @ref isValid())!
     * @return const std::shared_ptr<const datamodel::EnumType>
     */
    std::shared_ptr<const datamodel::EnumType> getEnumType() const;
    /**
     * @brief Get the Data Type object. If the type is enum_type the value is set to the defined
     * Data Type Object the enum_type depends on.
     * @return returns a valid pointer if the type of this element is a data_type (or enum_type) and
     * the infomation is valid (see @ref isValid())!
     * @return const std::shared_ptr<const datamodel::DataType>
     */
    std::shared_ptr<const datamodel::DataType> getDataType() const;

    /**
     * @brief Updates all information of the element.
     * This will recalculate the infomation and set the byte position and type references if valid.
     *
     * @param element the element to create the information for
     * @param previous_element the previous element
     * @param struct_ddl_version the language version set in the parent struct type (this influences
     * the alignmend etc.)
     * @param parent_dd the parent DD to retrieve all dependencies.
     */
    void update(datamodel::StructType::Element& element,
                const std::shared_ptr<datamodel::StructType::Element>& previous_element,
                const Version& struct_ddl_version,
                datamodel::DataDefinition& parent_dd);

private:
    OptionalSize _deserialized_byte_pos = {};
    OptionalSize _deserialized_byte_size = {};
    size_t _deserialized_type_byte_size = 0;
    size_t _deserialized_type_aligned_byte_size = 0;

    OptionalSize _serialized_byte_pos = {};
    OptionalSize _serialized_absolute_bit_offset = {};
    OptionalSize _serialized_bit_size = {};
    size_t _serialized_type_bit_size = 0;

    bool _is_dynamic = false;
    bool _is_after_dynamic = false;
    bool _is_valid = false;

    datamodel::ElementType _element_type;
};

} // namespace dd
} // namespace ddl

#endif // DD_TYPEINFOMODEL_H_INCLUDED