/**
 * @file
 * OO DataDefinition Redesign
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

#ifndef DD_DDL_STRUCT_ACCESS_H_INCLUDED
#define DD_DDL_STRUCT_ACCESS_H_INCLUDED

#include <ddl/datamodel/datamodel_datadefinition.h>

#include <string>

namespace ddl {
namespace dd {

class TypeInfo;
class ElementTypeInfo;
class StructTypeAccess;

/**
 * @brief Class to access and obtain the byte position and other information of a instance of this
 * element within a struct (see @ref StructTypeAccess).
 *
 */
class StructElementAccess {
public:
    friend class StructTypeAccess;
    friend class StructElementAccessIterator;

    /**
     * @brief deleted CTOR
     *
     */
    StructElementAccess() = delete;
    /**
     * @brief empty element access.
     * This kind of element access will be returned, if i.e. @ref StructTypeAccess::getElementByPath
     * can not find the given path.
     * @see @ref operator bool()
     */
    StructElementAccess(std::nullptr_t);
    /**
     * @brief Default move CTOR
     *
     */
    StructElementAccess(StructElementAccess&&) = default;
    /**
     * @brief Default move assignment.
     * @return StructElementAccess& reference
     */
    StructElementAccess& operator=(StructElementAccess&&) = default;
    /**
     * @brief Default copy CTOR
     *
     */
    StructElementAccess(const StructElementAccess&) = default;
    /**
     * @brief Default copy assignment.
     * @return StructElementAccess& reference
     */
    StructElementAccess& operator=(const StructElementAccess&) = default;

    /**
     * @brief Retrieve if the StructElementAccess is valid or not!
     *
     * @return true is valid
     * @return false is not valid
     */
    operator bool() const;
    /**
     * @brief Get the Type Of the type. Retrieves the infomation of the elements type.
     *
     *
     * @return TypeOfType
     */
    TypeOfType getTypeOfType() const;
    /**
     * @brief Gets the Struct Type if the element is dependent on a structtype.
     *
     * @retval const std::shared_ptr<const datamodel::StructType> with a valid structtype if type of
     * element is a structtype
     * @retval empty const std::shared_ptr<const datamodel::StructType>() if element does not depend
     * on a structtype
     */
    std::shared_ptr<const datamodel::StructType> getStructType() const;
    /**
     * @brief Gets the Enum Type if the element is dependent on a enumtype.
     *
     * @retval const std::shared_ptr<const datamodel::EnumType> with a valid enumtype if type of
     * element is a enumtype
     * @retval empty const std::shared_ptr<const datamodel::EnumType>() if element does not depend
     * on a enumtype
     */
    std::shared_ptr<const datamodel::EnumType> getEnumType() const;
    /**
     * @brief Gets the Data Type if the element is dependent on a datatype.
     *
     * @retval const std::shared_ptr<const datamodel::DataType> with a valid datatype if type of
     * element is a datatype
     * @retval empty const std::shared_ptr<const datamodel::DataType>() if element does not depend
     * on a datatype
     */
    std::shared_ptr<const datamodel::DataType> getDataType() const;

    /**
     * @brief Get the Struct Type Access if the element depends on a structtype.
     * All byte positions within the returned StructTypeAccess are relativ to the elements
     * StructTypeAccess parent.
     *
     * @return const StructTypeAccess of the struct can also retrieved by getStructType.
     * @see getStructType
     */
    StructTypeAccess getStructTypeAccess(size_t array_pos = 0) const;

    /**
     * @brief Get the Element Information which is also part of the data definition.
     *
     * @return const datamodel::StructType::Element&
     */
    const datamodel::StructType::Element& getElement() const;
    /**
     * @brief Retieve information if this element is a dynamic array
     *
     * @return true the element is dynamic or it contains dynamic content.
     * @return false the element is not dynamic.
     */
    bool isDynamic() const;
    /**
     * @brief The element is on a byte position after a dynamic element, the byte position can not
     * be calculated until it is instanciated.
     *
     * @return true the element is after a dynamic content.
     * @return false  the element is not after dynamic content
     * @see isDynamic
     */
    bool isAfterDynamic() const;
    /**
     * @brief Get the Array Pos, if this element was retrieved via
     * StructTypeAccess::getElementByPath and the operator[]
     *
     * @retval valid OptionalSize if it was a array position
     * @retval invalid OptionalSize if it was not a array position or the array position was not
     * valid.
     */
    OptionalSize getArrayPos() const;
    /**
     * @brief Get the Deserialized Byte Pos relative to the first byte position of the struct
     * obtained by dd::DataDefinition::getStructTypeAccess.
     *
     * @retval a valid OptionalSize with a byte position
     * @retval a invalid OptionalSize if is after a dynamic content.
     */
    size_t getDeserializedBytePos(size_t array_size = 0) const;
    /**
     * @brief Get the Byte size (depends on alignments, ddlversion and arraysize)
     *
     * @retval a valid OptionalSize with a size position
     * @retval a invalid OptionalSize if it is a dynamic content.
     */
    size_t getDeserializedByteSize() const;

    /**
     * @brief Get the Serialized Type Byte size
     *
     * @retval a valid OptionalSize with a size position
     * @retval a invalid OptionalSize if it has a dynamic
     */
    size_t getDeserializedTypeByteSize() const;
    /**
     * @brief Get SerializedBytePos
     * obtained by dd::DataDefinition::getStructTypeAccess.
     *
     * @retval a valid OptionalSize with a byte position
     * @retval a invalid OptionalSize if is after a dynamic content.
     */
    size_t getSerializedBytePos(size_t array_size = 0) const;
    /**
     * @brief Get SerializedBitOsset
     * obtained by dd::DataDefinition::getStructTypeAccess.
     *
     * @retval a valid OptionalSize with a byte position
     * @retval a invalid OptionalSize if is after a dynamic content.
     */
    size_t getSerializedBitOffset(size_t array_size = 0) const;
    /**
     * @brief Get the serialized Bit size of the value
     *
     * @retval a valid OptionalSize with a size position
     * @retval a invalid OptionalSize if it is a dynamic content.
     */
    size_t getSerializedBitSize() const;
    /**
     * @brief Get the serialized bit size of one element
     *
     * @retval a valid OptionalSize with a size position
     * @retval a invalid OptionalSize if it is a dynamic content.
     */
    size_t getSerializedTypeBitSize() const;

private:
    /**
     * @brief Construct a new Struct Element Access object
     *
     * @param elem the element information of the dd::DataDefinition.
     * @param serialized_element_offset the serialized offset of the element
     * @param deserialized_element_offset
     * @param array_pos
     */
    StructElementAccess(const std::shared_ptr<const datamodel::StructType::Element>& elem,
                        OptionalSize serialized_element_bit_offset,
                        OptionalSize deserialized_element_byte_offset,
                        OptionalSize array_pos);

private:
    std::shared_ptr<const datamodel::StructType::Element> _element = {};
    const ElementTypeInfo* _element_type_info = nullptr;
    OptionalSize _deserialized_struct_byte_offset = {};
    OptionalSize _serialized_struct_bit_offset = {};
    OptionalSize _array_pos = {};
};

/**
 * @brief Iterator to iterate thru the StructTypeAccess.
 * @see @ref StructTypeAccess::begin, @ref StructTypeAccess::cbegin
 *
 */
class StructElementAccessIterator {
public:
    /**
     * @brief this iterator type definition
     */
    typedef StructElementAccessIterator self_type;
    /**
     * @brief iterator value type definition
     */
    typedef StructElementAccess value_type;
    /**
     * @brief reference value type definition
     */
    typedef StructElementAccess& reference;
    /**
     * @brief const reference value type definition
     */
    typedef const StructElementAccess& const_reference;
    /**
     * @brief pointer value type definition
     */
    typedef StructElementAccess* pointer;
    /**
     * @brief pointer to const data value type definition
     */
    typedef const StructElementAccess* const_pointer;
    /**
     * @brief difference value type definition
     */
    typedef int difference_type;
    /**
     * @brief iterator type definition
     */
    typedef std::forward_iterator_tag iterator_category;
    /**
     * @brief Construct a new Struct Element Access Iterator
     *
     * @param other_it Iterator it refers to
     * @param end_it end position it referes to
     * @param struct_ser serialized position of the struct access this element belongs to.
     * @param struct_deser deserialized position of the struct access this element belongs to.
     */
    StructElementAccessIterator(datamodel::StructType::Elements::const_iterator other_it,
                                datamodel::StructType::Elements::const_iterator end_it,
                                OptionalSize struct_ser,
                                OptionalSize struct_deser);
    /**
     * @brief increasing operator to the next position.
     *
     * @return self_type
     */
    self_type operator++();
    /**
     * @brief iterator increasing.
     *
     * @param junk junks to increase
     * @return self_type
     */
    self_type operator++(int junk);
    /**
     * @brief defering access (check for end!)
     *
     * @return const reference
     */
    const_reference operator*() const;
    /**
     * @brief pointer access (check for end!)
     *
     * @return const pointer
     */
    const_pointer operator->() const;
    /**
     * @brief checks for equality (refering the same element!)
     *
     * @param rhs the other iterator
     * @return true is equal
     * @return false is not equal
     */
    bool operator==(const self_type& rhs) const;
    /**
     * @brief checks for not equality (does not refer the same element!)
     *
     * @param rhs the other iterator
     * @return true is not equal
     * @return false is equal
     */
    bool operator!=(const self_type& rhs) const;

private:
    datamodel::StructType::Elements::const_iterator _base_it;
    datamodel::StructType::Elements::const_iterator _end_it;
    StructElementAccess _ref;
    OptionalSize _struct_ser;
    OptionalSize _struct_deser;
};

/**
 * @brief Accessing class for a instance of a struct. This will calculate the byte positions can be
 * used for serialialization and deserialization.
 *
 */
class StructTypeAccess {
public:
    friend class DataDefinition;
    friend class StructElementAccess;
    /**
     * @brief default CTOR is deleted
     */
    StructTypeAccess() = delete;
    /**
     * @brief Move CTOR
     */
    StructTypeAccess(StructTypeAccess&&) = default;
    /**
     * @brief Move assignment operator
     *
     * @return StructTypeAccess&
     */
    StructTypeAccess& operator=(StructTypeAccess&&) = default;
    /**
     * @brief copy assignment operator
     *
     * @return StructTypeAccess&
     */
    StructTypeAccess(const StructTypeAccess&) = default;
    /**
     * @brief Copy CTOR
     */
    StructTypeAccess& operator=(const StructTypeAccess&) = default;

    /**
     * @brief Construct a new Struct Type Access object
     *
     * @param struct_type the struct type it refers to.
     * @remark This is for optimizing recalculations of sizes and access and will only work well if
     *         the TypeInfo is set to the struct_types infomodel.
     */
    StructTypeAccess(const std::shared_ptr<const datamodel::StructType>& struct_type);

private:
    /**
     * @brief Construct a empty Struct Type Access.
     */
    StructTypeAccess(std::nullptr_t);
    /**
     * @brief Construct a new Struct Type Access object
     *
     * @param struct_type the struct type it refers to.
     * @param serialized_struct_offset the offset of this instance within another struct
     * @param deserialized_struct_offset the offset of this instance within another struct
     */
    StructTypeAccess(const std::shared_ptr<const datamodel::StructType>& struct_type,
                     OptionalSize serialized_struct_offset,
                     OptionalSize deserialized_struct_offset);

public:
    /**
     * @brief Get the StructElementAccess information class by path. Separator is "." or "/"
     * \n Following element access paths are possible:
     * \li "element_1" -> access to a element with the name "element_1"
     * \li "element_1.sub_2" -> access to a element with the name "element_1" which is a structtype
     * that has a element with the name "sub_2" \li "element_1/sub_2" -> same as before \li
     * "element_2[3]" -> access to a element with the name "element_2" which is a array element with
     * an possible array pos [3] \li "element_2[3].sub_2[24]" -> access to a element with the name
     * "element_2" which is a array element with an possible array pos [3], this array pos contains
     * a structtype with an element named sub_2 that is an array with an valid position [24] \li
     * "element_2[3]/sub_2[24]" -> same as before \n Following element access paths are NOT
     * possible: \li "element_1.sub_2/subsub_3" -> invalid. No mixture of separator possible! \li
     * "element_2[24]"  -> invalid, if the element_2 is no array or arraysize is below 24!
     *
     * @param name_path The path to the element.
     * @return StructElementAccess
     */
    StructElementAccess getElementByPath(const std::string name_path) const;
    /**
     * @brief The Struct access is a valid access type.
     *
     * @return true the access is valid.
     * @return false the access is not valid.
     */
    operator bool() const;
    /**
     * @brief Get the known Static Struct Size of this instance.
     *
     * @return size_t size of the instance (without dynamic content)
     */
    size_t getStaticStructSize() const;

    /**
     * @brief Get the known unaligned Static Struct Size of this instance (this is usually the last
     * position + sizeof last element)
     *
     * @return size_t size of the instance (without dynamic content)
     */
    size_t getStaticUnalignedStructSize() const;

    /**
     * @brief Get the known bitsize of this instance (this is usually the last
     * position + sizeof last element)
     *
     * @return size_t serialized bitsize of the instance (without dynamic content)
     */
    size_t getStaticSerializedBitSize() const;

    /**
     * @brief Get the Struct Type it refers to (for data definition information)
     *
     * @return const dd::datamodel::StructType&
     */
    const datamodel::StructType& getStructType() const;

    /**
     * @brief iterator type of this struct
     *
     */
    typedef const StructElementAccessIterator const_iterator;
    /**
     * @brief constant iterator access
     *
     * @return const_iterator
     */
    const_iterator cbegin() const;
    /**
     * @brief constant range based iterator access
     *
     * @return const_iterator
     */
    const_iterator begin() const;
    /**
     * @brief constant end iterator
     *
     * @return const_iterator
     */
    const_iterator cend() const;
    /**
     * @brief constant range based end iterator
     *
     * @return const_iterator
     */
    const_iterator end() const;

private:
    std::shared_ptr<const datamodel::StructType> _struct_type;
    OptionalSize _serialized_struct_as_element_offset =
        {}; // these value are greter 0 if the struct is used within anaother struct and we
            // calculate that element
    OptionalSize _deserialized_struct_as_element_offset =
        {}; // these value are greter 0 if the struct is used within anaother struct and we
            // calculate that element
    const TypeInfo* _type_info = nullptr;
};

} // namespace dd
} // namespace ddl

#endif // DD_DDL_STRUCT_ACCESS_H_INCLUDED
