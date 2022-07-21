/**
 * @file
 * OO DataDefinition Type objects (DataType, EnumType, StructType, StreamMetaType)
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

#ifndef DD_DATA_MODEL_TYPES_H_INCLUDED
#define DD_DATA_MODEL_TYPES_H_INCLUDED

#include <ddl/datamodel/datamodel_base.h>
#include <ddl/datamodel/datamodel_keyvalue.h>
#include <ddl/datamodel/infomodel_base.h>
#include <ddl/dd/dd_common_types.h>
#include <ddl/utilities/dd_access_list.h>
#include <ddl/utilities/dd_access_map.h>

#include <string>
#include <unordered_map>
#include <vector>

namespace ddl {

namespace dd {

namespace datamodel {

/**
 * @brief type Base defintion
 *
 */
class TypeBase {
public:
    /**
     * @brief CTOR
     *
     */
    TypeBase() = default;
    /**
     * @brief copy CTOR
     *
     */
    TypeBase(const TypeBase&) = default;
    /**
     * @brief move CTOR
     *
     */
    TypeBase(TypeBase&&) = default;
    /**
     * @brief copy assignment operator
     *
     * @return TypeBase&
     */
    TypeBase& operator=(const TypeBase&) = default;
    /**
     * @brief move assignment operator
     *
     * @return TypeBase&
     */
    TypeBase& operator=(TypeBase&&) = default;
    /**
     * @brief CTOR
     *
     * @param name The name of the type
     */
    TypeBase(const std::string& name);
    /**
     * @brief DTOR
     *
     */
    virtual ~TypeBase() = default;

    /**
     * @brief Get the Type Of the Type (data_type, enum_type, struct_type, stream_meta_type).
     *
     *
     * @return TypeOfType
     */
    virtual TypeOfType getTypeOfType() const = 0;
    /**
     * @brief Set the Name
     *
     * @param name The name of type.
     * @throws This might throw if the set name does already exist in the container this type is
     * part of.
     */
    virtual void setName(const std::string& name);
    /**
     * @brief Get the Name
     *
     * @return const std::string&
     */
    const std::string& getName() const;

protected:
    /// name storage value
    std::string _name;
};

/**
 * @brief observable DataDefinition object class to describe (POD) DataType.
 *
 */
class DataType : public TypeBase,
                 public ModelSubject<DataType>,
                 public utility::TypeAccessMapSubject<DataType>,
                 public InfoMap {
public:
    /**
     * @brief default CTOR
     *
     */
    DataType(){};
    /**
     * @brief copy CTOR
     *
     */
    DataType(const DataType&) = default;
    /**
     * @brief move CTOR
     *
     */
    DataType(DataType&&) = default;
    /**
     * @brief copy assignment operator.
     *
     * @return DataType&
     */
    DataType& operator=(const DataType&) = default;
    /**
     * @brief move assignment operator.
     *
     * @return DataType&
     */
    DataType& operator=(DataType&&) = default;
    /**
     * @brief CTOR
     *
     * @param name name of the datatype
     * @param bit_size bit size of the datatype
     * @param description description of the datatype
     * @param array_size arraysize of the datatype (this is not support in this implementation of
     * StructTypeAccess and typeinfo model!)
     * @param unit_name unit name of the datatype
     * @param minimum_value the minimum value (for information only)
     * @param maximum_value the maximum value (for information only)
     * @param default_type_alignment Type alignment (this is optional, because it is not part of the
     *                               DD, but important for automatic calculation of member
     *                               positions)
     */
    DataType(const std::string& name,
             size_t bit_size,
             const std::string& description = {},
             OptionalSize array_size = {},
             const std::string& unit_name = {},
             const std::string& minimum_value = {},
             const std::string& maximum_value = {},
             OptionalSize default_type_alignment = {});

    /**
     * @brief CTOR with initializer
     *
     * @param initializer the list containing the same values as the CTOR before. Valid in this
     * order: \li 0 - name \li 1 - bit_size \li 2 - description \li 3 - array_size \li 4 - unit_name
     * \li 5 - minimum_value
     * \li 6 - maximum_value
     *
     * Example usage:
     * @code
     * DataType my_dt = {"32_bit_type", "32", "My 32 bit type is great!"};
     * @endcode
     */
    DataType(const std::initializer_list<std::string>& initializer);
    /**
     * @brief DTOR.
     *
     */
    virtual ~DataType() = default;
    /**
     * @brief equality operator.
     *
     * @param other the other datatype to compare this type to.
     * @return true The datatype is equal (the description, minimum and maximum value will not be
     * compared!)
     * @return false the datatype is not equal
     */
    bool operator==(const DataType& other) const;
    /**
     * @brief non equality operator.
     *
     * @param other the other datatype to compare this type to.
     * @return false The datatype is equal (the description, minimum and maximum value will not be
     * compared!)
     * @return true the datatype is not equal
     */
    bool operator!=(const DataType& other) const;

    /**
     * @copydoc TypeBase::getTypeOfType
     */
    TypeOfType getTypeOfType() const override;
    /**
     * @copydoc TypeBase::setName
     */
    void setName(const std::string& name) override;

    /**
     * @brief Get the Bit Size
     *
     * @return size_t
     */
    size_t getBitSize() const;
    /**
     * @brief Set the Bit Size
     *
     * @param size the number of bits of this type
     */
    void setBitSize(size_t size);

    /**
     * @brief Get the Array Size (optional)
     *
     * @return OptionalSize
     */
    OptionalSize getArraySize() const;
    /**
     * @brief Set the Array Size object
     *
     * @param array_size the optional array to set.
     */
    void setArraySize(OptionalSize array_size);

    /**
     * @brief Get the Unit Name
     *
     * @return const std::string&
     */
    const std::string& getUnitName() const;
    /**
     * @brief Set the Unit Name
     *
     * @param unit_name
     */
    void setUnitName(const std::string& unit_name);

    /**
     * @brief Get the Min Value for information
     * (introduced in DDL 3.0)
     *
     * @return const std::string&
     */
    const std::string& getMin() const;
    /**
     * @brief Set the Min
     * (introduced in DDL 3.0)
     *
     * @param minimum_value the minimum value to set for information
     */
    void setMin(const std::string& minimum_value);

    /**
     * @brief Get the Max Value for information
     * (introduced in DDL 3.0)
     *
     * @return const std::string&
     */
    const std::string& getMax() const;
    /**
     * @brief Set the Max
     * (introduced in DDL 3.0)
     *
     * @param maximum_value the maximum value to set for information
     */
    void setMax(const std::string& maximum_value);

    /**
     * @brief Get the Description
     *
     * @return const std::string&
     */
    const std::string& getDescription() const;
    /**
     * @brief Set the Description
     *
     * @param description the description to set
     */
    void setDescription(const std::string& description);

    /**
     * @brief Get the default Alignment if set
     *
     * @return const OptionalSize&
     */
    const OptionalSize& getDefaultAlignment() const;

private:
    void notify(const std::string& additional_info);
    size_t _bit_size = 0;
    std::string _description = {};
    OptionalSize _array_size = {};
    std::string _unit_name = {};
    std::string _minimum_value = {};
    std::string _maximum_value = {};
    OptionalSize _default_type_alignment = {};
};

/**
 * @brief observable DataDefinition object class to describe EnumType.
 *
 */
class EnumType : public TypeBase,
                 public ModelSubject<EnumType>,
                 public utility::TypeAccessMapSubject<EnumType>,
                 public InfoMap {
public: // types
    /**
     * @brief Enum type element defintion as Name Value pair.
     *
     */
    using Element = KeyValuePair;
    /**
     * @brief container class for elements in EnumType
     *
     */
    using Elements = utility::TypeAccessMap<Element, EnumType>;
    /// @cond nodoc
    friend Elements;
    /// @endcond nodoc

public:
    /**
     * @brief no default CTOR
     *
     */
    EnumType() = delete;
    /**
     * @brief move CTOR.
     *
     */
    EnumType(EnumType&&);
    /**
     * @brief move assignment operator
     *
     * @return EnumType&
     */
    EnumType& operator=(EnumType&&);
    /**
     * @brief Copy CTOR
     *
     * @param other the other enumtype to copy
     */
    EnumType(const EnumType& other);
    /**
     * @brief copy assignment operator
     * @param other the other enumtype to copy
     * @return EnumType&
     */
    EnumType& operator=(const EnumType& other);
    /**
     * @brief Construct a new Enum Type object
     *
     * @param name name of the enum
     * @param data_type_name  name of the data type this enumtype refers to
     * @param elements vector of elements to add while constructing
     * @throws may throw if \p elements contains elements with trhe same name twice!
     */
    EnumType(const std::string& name,
             const std::string& data_type_name,
             const std::vector<Element>& elements = {});
    /**
     * @brief DTOR
     *
     */
    virtual ~EnumType() = default;
    /**
     * @brief equality operator.
     *
     * @param other the enum type to compare the enum to
     * @return true the enum is equal (aslo its elements).
     * @return false the enum is not equal.
     */
    bool operator==(const EnumType& other) const;
    /**
     * @brief non equality operator.
     *
     * @param other the enum type to compare the enum to
     * @return false the enum is equal (aslo its elements).
     * @return true the enum is not equal.
     */
    bool operator!=(const EnumType& other) const;

    /**
     * @brief Get the Elements object
     *
     * @return const Elements&
     */
    const Elements& getElements() const;
    /**
     * @brief Get the Elements object
     *
     * @return Elements&
     */
    Elements& getElements();

    /**
     * @copydoc TypeBase::getTypeOfType
     */
    TypeOfType getTypeOfType() const override;
    /**
     * @copydoc TypeBase::setName
     */
    void setName(const std::string& name) override;

    /**
     * @brief Get the Data Type Name
     *
     * @return const std::string&
     */
    const std::string& getDataTypeName() const;
    /**
     * @brief Set the Data Type Name
     *
     * @param data_type_name name of the datatype this enumtype referes to.
     */
    void setDataTypeName(const std::string& data_type_name);

private:
    void notify(ModelEventCode code,
                utility::TypeAccessMapEventCode forward_code,
                const std::string& additional_info);
    bool validateContains(const Elements::access_type& element_name) const;
    void notifyChangedMapContent(utility::TypeAccessMapEventCode code,
                                 Elements::access_type& element,
                                 const std::string& additional_info);

    std::string _data_type_name = {};
    Elements _elements;
};

/**
 * @brief observable DataDefinition object class to describe StructType.
 *
 */
class StructType : public TypeBase,
                   public ModelSubject<StructType>,
                   public utility::TypeAccessMapSubject<StructType>,
                   public InfoMap {
public:
    /**
     * @brief no default CTOR
     *
     */
    StructType() = delete;
    /**
     * @brief move CTOR.
     *
     */
    StructType(StructType&& other);
    /**
     * @brief move assignment operator.
     *
     * @return StructType&
     */
    StructType& operator=(StructType&& other);
    /**
     * @brief copy CTOR
     *
     * @param other the structtype to copy from.
     */
    StructType(const StructType& other);
    /**
     * @brief copy assignment operator
     *
     * @param other the structtype to copy from.
     * @return StructType&
     */
    StructType& operator=(const StructType& other);
    /**
     * @brief DTOR
     *
     */
    virtual ~StructType() = default;

    /**
     * @brief class to describe the serialized information of a StructType::Element.
     */
    class SerializedInfo {
    public:
        /**
         * @brief default CTOR.
         *
         */
        SerializedInfo(){};
        /**
         * @brief copy CTOR
         *
         */
        SerializedInfo(const SerializedInfo&) = default;
        /**
         * @brief move CTOR
         *
         */
        SerializedInfo(SerializedInfo&&) = default;
        /**
         * @brief copy assignment operator
         *
         * @return SerializedInfo&
         */
        SerializedInfo& operator=(const SerializedInfo&) = default;
        /**
         * @brief move assignment operator
         *
         * @return SerializedInfo&
         */
        SerializedInfo& operator=(SerializedInfo&&) = default;
        /**
         * @brief DTOR
         *
         */
        virtual ~SerializedInfo() = default;

        /**
         * @brief CTOR
         *
         * @param byte_pos the byte position
         * @param byte_order the byte order
         * @param bit_pos the bit position (optional)
         * @param num_bits the number of bits used (optional)
         */
        SerializedInfo(OptionalSize byte_pos,
                       ByteOrder byte_order = ByteOrderDefault::getPlatformDefault(),
                       OptionalSize bit_pos = {},
                       OptionalSize num_bits = {});
        /**
         * @brief CTOR with string list initializing
         *
         * @param intializer the initializer with strings. Valid in this order:
         * \li 0 - byte_pos
         * \li 1 - byte_order
         * \li 2 - bit_pos
         * \li 3 - num_bits
         *
         * Example usage:
         * @code
         * StructType my_struct_type;
         * //the {"24", "LE", "8", "5"} is the SerializedInfo
         * my_tsruct_type.addElement({ "bit_usage_5", "int32_t", { 0 }, {"24", "LE", "8", "5"});
         * @endcode
         */
        SerializedInfo(const std::initializer_list<size_t>& intializer);
        /**
         * @brief equality operator.
         *
         * @param other the other serialized info to compare to.
         * @return true the serialized info is equal
         * @return false the serialized info is not equal
         */
        bool operator==(const SerializedInfo& other) const;

        /**
         * @brief Get the Byte Pos
         * @remark If not valid returned it will be set automatically while typeinfo model is
         * created!
         *
         * @return OptionalSize a valid byte pos (if invalid it will be set)
         */
        OptionalSize getBytePos() const;
        /**
         * @brief Set the Byte Pos
         *
         * @param byte_pos the bytepos to set
         * @param prevent_notification if set to true this change is not observable.
         */
        void setBytePos(OptionalSize byte_pos, bool prevent_notification = false);

        /**
         * @brief Get the Bit Pos (if set)
         * @remark If not valid returned it will be set automatically while typeinfo model is
         * created if necessary!
         *
         * @return OptionalSize valid bitpos or invalid if not set
         */
        OptionalSize getBitPos() const;
        /**
         * @brief Sets the Bit Pos
         *
         * @param bit_pos the bit pos if it is valid
         * @param prevent_notification if set to true this change is not observable.
         */
        void setBitPos(OptionalSize bit_pos, bool prevent_notification = false);

        /**
         * @brief Get the Num Bits (if set)
         *
         * @return OptionalSize
         */
        OptionalSize getNumBits() const;
        /**
         * @brief Set the Num Bits
         *
         * @param num_bits the num bits to set.
         */
        void setNumBits(OptionalSize num_bits);

        /**
         * @brief Get the Byte Order
         *
         * @return ByteOrder
         */
        ByteOrder getByteOrder() const;
        /**
         * @brief Set the Byte Order
         *
         * @param byte_order the byte order to set
         */
        void setByteOrder(ByteOrder byte_order);

    protected:
        /**
         * @brief notification forwarding of serialized information to the StructType::Element
         * object.
         *
         * param std::string string additional_info - The name of the additional info in the
         * notification to forward.
         */
        virtual void notifyChangeSerialized(const std::string&){};

    private:
        OptionalSize _byte_pos = {};
        OptionalSize _bit_pos = {};
        OptionalSize _num_bits = {};
        ByteOrder _byte_order = e_le;
    };

    /**
     * @brief class to describe the deserialized information of a StructType::Element.
     */
    class DeserializedInfo {
    public:
        /**
         * @brief Default CTOR
         *
         */
        DeserializedInfo(){};
        /**
         * @brief copy CTOR
         *
         */
        DeserializedInfo(const DeserializedInfo&) = default;
        /**
         * @brief move CTOR
         *
         */
        DeserializedInfo(DeserializedInfo&&) = default;
        /**
         * @brief copy assignment operator
         *
         * @return DeserializedInfo&
         */
        DeserializedInfo& operator=(const DeserializedInfo&) = default;
        /**
         * @brief move assignment operator
         *
         * @return DeserializedInfo&
         */
        DeserializedInfo& operator=(DeserializedInfo&&) = default;
        /**
         * @brief DTOR
         *
         */
        virtual ~DeserializedInfo() = default;

        /**
         * @brief CTOR
         *
         * @param alignment the alignment to set
         */
        DeserializedInfo(size_t alignment);
        /**
         * @brief CTOR
         *
         * @param intializer  the inializer list containing a string fror the alignment
         */
        DeserializedInfo(const std::initializer_list<size_t>& intializer);

        /**
         * @brief equality operator.
         *
         * @param other the other information to compare to.
         * @return true the info is equal.
         * @return false the info is not equal.
         */
        bool operator==(const DeserializedInfo& other) const;

        /**
         * @brief Get the Alignment
         *
         * @return size_t
         */
        virtual size_t getAlignment() const;
        /**
         * @brief Set the Alignment
         *
         * @param alignment the alignment to set.
         * @remark This is observable.
         * @remark please use the valid alignments via dd::Alignment
         */
        void setAlignment(size_t alignment);

    protected:
        /**
         * @brief Forwarding the notification
         *
         * param std::string string additional_info - the additional info of the item changed
         * notification.
         */
        virtual void notifyChangeDeserialized(const std::string&){};

    private:
        size_t _alignment = Alignment::e_invalid;
    };

    /**
     * @brief observable DataDefinition object class for a Element of a StructType.
     *
     */
    class Element : public SerializedInfo,
                    public DeserializedInfo,
                    public utility::TypeAccessListSubject<Element>,
                    public InfoMap {
    public:
        /**
         * @brief default CTOR
         *
         */
        Element(){};
        /**
         * @brief copy CTOR
         *
         */
        Element(const Element&) = default;
        /**
         * @brief move CTOR
         *
         */
        Element(Element&&) = default;
        /**
         * @brief copy assignment operator
         *
         * @return Element&
         */
        Element& operator=(const Element&) = default;
        /**
         * @brief move assignment operator
         *
         * @return Element&
         */
        Element& operator=(Element&&) = default;

        /**
         * @brief CTOR
         *
         * @param name name of the element
         * @param type_name type name of the element (datatype, enumtype or structtype)
         * @param deserialized_info the deserialized info (default is a alignment of 1)
         * @param serialized_info serialized info (default is LE, bytepos will be calculated
         * automatically in typeinfo model)
         * @param array_size array size of the element (default is 1) for setting dynamic use a
         *                   string with a valid previous element name
         * @param description description of the element
         * @param comment comment of the element
         * @param unit_name the unit name of the element
         * @param value a constant value of the element (only valid if the type is set to a enumtype
         * and this value is part of the enumtype elements)
         * @param minimum_value minimum value (for information only)
         * @param maximum_value maximum value (for information only)
         * @param default_value default value can be set for POD types (supported is a interger or a
         * float/double value)
         * @param scale scale value (for information only)
         * @param offset offset value (for information only)
         */
        Element(const std::string& name,
                const std::string& type_name,
                const DeserializedInfo& deserialized_info,
                const SerializedInfo& serialized_info,
                const dd::ArraySize& array_size = {},
                const std::string& description = {},
                const std::string& comment = {},
                const std::string& unit_name = {},
                const std::string& value = {},
                const std::string& minimum_value = {},
                const std::string& maximum_value = {},
                const std::string& default_value = {},
                const std::string& scale = {},
                const std::string& offset = {});

        /**
         * @brief equality operator.
         *
         * @param other the other element to compare to.
         * @return true the elements is equal.
         * @return false the elements is not equal.
         */
        bool operator==(const Element& other) const;
        /**
         * @brief non equality operator.
         *
         * @param other the other element to compare to.
         * @return false the elements is equal.
         * @return true  the elements is not equal.
         */
        bool operator!=(const Element& other) const;

        /**
         * @brief Get the Name
         *
         * @return const std::string&
         */
        const std::string& getName() const;
        /**
         * @brief Set the Name
         *
         * @param name the name to set
         * @remark This is observable.
         * @throws this may throw if a element with the same name already exists in the container
         * this element is part of.
         */
        void setName(const std::string& name);

        /**
         * @brief Get the Type Name
         *
         * @return const std::string&
         */
        const std::string& getTypeName() const;
        /**
         * @brief Set the Type Name
         *
         * @param type_name the type name to set ((POD) datatype, enumtype, structtype).
         * @remark This is observable.
         */
        void setTypeName(const std::string& type_name);

        /**
         * @brief Get the Description
         *
         * @return const std::string&
         */
        const std::string& getDescription() const;
        /**
         * @brief Set the Description
         *
         * @param description the description
         * @remark This is observable.
         */
        void setDescription(const std::string& description);

        /**
         * @brief Get the Comment
         *
         * @return const std::string&
         */
        const std::string& getComment() const;
        /**
         * @brief Set the Comment object
         *
         * @param comment the comment to set
         * @remark This is observable.
         */
        void setComment(const std::string& comment);

        /**
         * @brief Get the Array Size
         *
         * @return ArraySize return the arraysize
         */
        const ArraySize& getArraySize() const;
        /**
         * @brief Set the Array Size
         *
         * @param array_size the arraysize to set
         * @remark This is observable.
         */
        void setArraySize(const ArraySize& array_size);

        /**
         * @brief Get the Unit Name
         *
         * @return const std::string&
         */
        const std::string& getUnitName() const;
        /**
         * @brief Set the Unit Name
         *
         * @param unit_name the unit name
         * @remark This is observable.
         */
        void setUnitName(const std::string& unit_name);

        /**
         * @brief Get the Value
         *
         * @return const std::string&
         */
        const std::string& getValue() const;
        /**
         * @brief Set the Value
         * This is only valid if the data type is set to an enum type where this constant value is
         * part of.
         *
         * @param value the constant value to set
         * @remark This is observable.
         *
         */
        void setValue(const std::string& value);

        /**
         * @brief Get the Min (for information only).
         * This was introduced in DDL 3.0.
         *
         * @return const std::string&
         */
        const std::string& getMin() const;
        /**
         * @brief Set the Min (for information only).
         *
         * @param minimum_value the minimum value
         * @remark This is observable.
         */
        void setMin(const std::string& minimum_value);

        /**
         * @brief Get the Max (for information only).
         * This was introduced in DDL 3.0.
         *
         * @return const std::string&
         */
        const std::string& getMax() const;
        /**
         * @brief Set the Max (for information only).
         *
         * @param maximum_value the maximum value
         * @remark This is observable.
         */
        void setMax(const std::string& maximum_value);

        /**
         * @brief Get the Default object
         *
         * @return const std::string&
         */
        const std::string& getDefault() const;
        /**
         * @brief Set the Default Value.
         * This is only valid if the default value is convertable to the datatype used.
         *
         * @param default_value the value for the default.
         * @remark This is observable.
         */
        void setDefault(const std::string& default_value);

        /**
         * @brief Get the Scale (for information only)
         *
         * @return const std::string&
         */
        const std::string& getScale() const;
        /**
         * @brief Set the Scale (for information only)
         *
         * @param scale the scale factor
         * @remark This is observable.
         */
        void setScale(const std::string& scale);

        /**
         * @brief Get the Offset (for information only)
         *
         * @return const std::string&
         */
        const std::string& getOffset() const;
        /**
         * @brief Set the Offset (for information only)
         *
         * @param offset the offset to set
         * @remark This is observable.
         */
        void setOffset(const std::string& offset);

    private:
        void notifyChangeSerialized(const std::string& additional_info) override;
        void notifyChangeDeserialized(const std::string& additional_info) override;

    private:
        std::string _name;
        std::string _type_name;
        std::string _description;
        std::string _unit_name;
        std::string _comment;
        ArraySize _array_size = 1;
        std::string _value;
        std::string _minimum_value;
        std::string _maximum_value;
        std::string _default_value;
        std::string _scale;
        std::string _offset;
    };

    /**
     * @brief CTOR
     *
     * @param name The name for the structtype
     * @param struct_version the struct version (should be some unsigned integer value)
     * @param alignment the alignment of the struct (default is 1)
     * @param comment the comment
     * @param ddl_version dedicated language version for this structtype must be used (this
     * influences the structsize and its alignment!)
     * @param elements vector of elements, that will be added immediatelly.
     * @throws ddl::dd::Error may throw if \p elements contains elements with the same name twice
     */
    StructType(const std::string& name,
               const std::string& struct_version = "1",
               OptionalSize alignment = {},
               const std::string& comment = {},
               dd::Version ddl_version = dd::Version::ddl_version_notset,
               const std::vector<Element>& elements = {});
    /**
     * @brief equality operator.
     *
     * @param other the other structtype to compare to.
     * @return true the type is equal (and all its elements).
     * @return false the type is not equal.
     */
    bool operator==(const StructType& other) const;
    /**
     * @brief non equality operator.
     *
     * @param other the other structtype to compare to.
     * @return false the type is equal (and all its elements).
     * @return true the type is not equal.
     */
    bool operator!=(const StructType& other) const;

    /**
     * @copydoc TypeBase::getTypeOfType
     */
    TypeOfType getTypeOfType() const override;
    /**
     * @copydoc TypeBase::setName
     */
    void setName(const std::string& name) override;

    /**
     * @brief Get the Alignment (if set)
     *
     * @return OptionalSize
     */
    OptionalSize getAlignment() const;
    /**
     * @brief Set the Alignment
     *
     * @param alignment the alignment to set.
     * @remark This is observable.
     */
    void setAlignment(OptionalSize alignment);

    /**
     * @brief Get the Comment
     *
     * @return const std::string&
     */
    const std::string& getComment() const;
    /**
     * @brief Set the Comment
     *
     * @param comment the comment to set
     * @remark This is observable.
     */
    void setComment(const std::string& comment);

    /**
     * @brief Get the set Version (the struct version! Not the DDL Version!)
     *
     * @return const std::string&
     */
    const std::string& getVersion() const;
    /**
     * @brief Get the Version (the struct version! Not the DDL Version!)
     *
     * @return const std::string&
     * @remark This is observable.
     */
    void setVersion(const std::string& struct_version);

    /**
     * @brief Get the set language Version
     *
     * @return Version
     * @retval dd::Version::ddl_version_notset if it was not set
     */
    Version getLanguageVersion() const;
    /**
     * @brief Set the Language Version
     *
     * @param ddl_version the language version to set
     * @remark This is observable.
     */
    void setLanguageVersion(const Version& ddl_version);

public:
    /**
     * @brief container of elements for the StructType::Element.
     */
    using Elements = utility::TypeAccessList<Element, StructType>;
    /// @cond nodoc
    friend Elements;
    /// @endcond nodoc
    /**
     * @brief Get the Elements object
     *
     * @return const Elements&
     */
    const Elements& getElements() const;
    /**
     * @brief Get the Elements object
     *
     * @return Elements&
     */
    Elements& getElements();

private:
    void notify(ModelEventCode code,
                utility::TypeAccessMapEventCode map_code,
                const std::string& additional_info);
    bool validateContains(const Elements::access_type& element_name) const;
    void notifyChangedListContent(utility::TypeAccessListEventCode code,
                                  Elements::access_type& element,
                                  const std::string& additional_info);

    std::string _struct_version;
    dd::Version _ddl_version;
    std::string _comment;
    OptionalSize _alignment = Alignment::e1;
    Elements _elements;
};

/**
 * @brief observable DataDefinition object class to describe StreamMetaType.
 *
 */
class StreamMetaType : public TypeBase,
                       public ModelSubject<StreamMetaType>,
                       public utility::TypeAccessMapSubject<StreamMetaType>,
                       public InfoMap {
public: // types
    /**
     * @brief defintion of properties by using name - type pair.
     *
     */
    using Property = KeyValuePair;
    /**
     * @brief container of properties for the StreamMetaType::Property.
     *
     */
    using Properties = utility::TypeAccessMap<Property, StreamMetaType>;
    /// @cond nodoc
    friend Properties;
    /// @endcond nodoc

public:
    /**
     * @brief no default CTOR
     *
     */
    StreamMetaType() = delete;
    /**
     * @brief move CTOR
     *
     */
    StreamMetaType(StreamMetaType&&);
    /**
     * @brief move assignment operator
     *
     * @return StreamMetaType&
     */
    StreamMetaType& operator=(StreamMetaType&&);
    /**
     * @brief copy CTOR
     *
     * @param other the other type to copy from
     */
    StreamMetaType(const StreamMetaType& other);
    /**
     * @brief copy assignment operator
     *
     * @param other the other type to copy from
     * @return StreamMetaType&
     */
    StreamMetaType& operator=(const StreamMetaType& other);
    /**
     * @brief CTOR
     *
     * @param name the name of the streammetatype
     * @param version the version of the streammetatype
     * @param parent the parent of the streammetatype
     * @param properties a initial set of properties
     * @throws may throw if \p properties contains properties with the same name twice.
     */
    StreamMetaType(const std::string& name,
                   const std::string& version,
                   const std::string& parent = {},
                   const std::vector<Property>& properties = {});
    /**
     * @brief DTOR
     *
     */
    virtual ~StreamMetaType() = default;
    /**
     * @brief equality operator.
     *
     * @param other the other type to compare to.
     * @return true the type is equal (and all its properties).
     * @return false the type is not equal.
     */
    bool operator==(const StreamMetaType& other) const;
    /**
     * @brief non equality operator.
     *
     * @param other the other type to compare to.
     * @return false the type is equal (and all its properties).
     * @return true the type is not equal.
     */
    bool operator!=(const StreamMetaType& other) const;

    /**
     * @brief Get the Properties object
     *
     * @return const Properties&
     */
    const Properties& getProperties() const;
    /**
     * @brief Get the Properties object
     *
     * @return Properties&
     */
    Properties& getProperties();

    /**
     * @copydoc TypeBase::getTypeOfType
     */
    TypeOfType getTypeOfType() const override;
    /**
     * @copydoc TypeBase::setName
     */
    void setName(const std::string& name) override;

    /**
     * @brief Get the Parent
     *
     * @return const std::string&
     */
    const std::string& getParent() const;
    /**
     * @brief Set the Parent
     * This is only valid if the parent is a valid other streammetatype.
     *
     * @param parent the parent to set.
     * @remark This is observable.
     */
    void setParent(const std::string& parent);

    /**
     * @brief Get the Version
     *
     * @return const std::string&
     */
    const std::string& getVersion() const;
    /**
     * @brief Set the Version
     *
     * @param version the version to set
     * @remark This is observable.
     */
    void setVersion(const std::string& version);

private:
    void notify(ModelEventCode code,
                utility::TypeAccessMapEventCode forward_code,
                const std::string& additional_info);
    bool validateContains(const Properties::access_type& prop) const;
    void notifyChangedMapContent(utility::TypeAccessMapEventCode code,
                                 Properties::access_type& element,
                                 const std::string& additional_info);

    std::string _version = {};
    std::string _parent = {};
    Properties _properties;
};

/**
 * @brief helper union class for an element that uses one of the datatype, enumtype, structtype
 *
 */
struct ElementType {
    /**
     * @brief the type of the type used (if valid)
     *
     */
    TypeOfType _type_of_type = invalid_type;
    /**
     * @brief is set to an valid reference if the type is data_type OR enum_type!
     */
    std::shared_ptr<datamodel::DataType> _data_type = {};
    /**
     * @brief is set to an valid reference if the type is enum_type!
     */
    std::shared_ptr<datamodel::EnumType> _enum_type = {};
    /**
     * @brief is set to an valid reference if the type is struct_type!
     */
    std::shared_ptr<datamodel::StructType> _struct_type = {};
};

} // namespace datamodel
} // namespace dd
} // namespace ddl

#endif // DD_DATA_MODEL_TYPES_H_INCLUDED
