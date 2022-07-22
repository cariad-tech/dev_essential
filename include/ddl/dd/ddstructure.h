/**
 * @file
 * OO DataDefinition DDStructure Header.
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

#ifndef DDSTRUCTURE_H_INCLUDED
#define DDSTRUCTURE_H_INCLUDED

#include <ddl/dd/dd.h>
#include <ddl/dd/ddelement.h>
#include <ddl/utilities/std_to_string.h>

#include <type_traits>

namespace ddl {

///@cond nodoc
class DDElement;
///@endcond nodoc

/**
 * @brief Utility class for a complete valid data definition of one StructType and its dependencies.
 * This class is for convenience to use create a StructType data defintion in code.
 * <br> The class is to use as follows:
 * @code
 *
 * DDEnum e1("e1", DataType<int8_t>(), {{"val11", "1"}, {"val12", "2"}, {"val13", "3"}});
 * DDStructure a("A");
 * a.addElement<int32_t>("elem1");
 * a.addElement("elem2", DataType<uint16_t>());
 * a.addElement("elem3", e1, "val13");
 *
 * DDStructure b("B");
 * b.addElement("nestedA1", a);
 * b.addElement("nestedA2", a);
 *
 * //now you can do something with the created DD
 * b.getDD().isValid();
 * std::string dd_struct = b.getStructName();
 * std::string dd_description = b.getStructDescription();
 *
 * @endcode
 *
 * @see @ref ddl::DDEnum, @ref ddl::DataType
 */
class DDStructure {
public:
    /**
     * @brief Construct a new DDStructure object
     *
     */
    DDStructure() = delete;
    /**
     * @brief Construct a new DDStructure object
     * @param other the structure to move.
     *
     */
    DDStructure(DDStructure&& other);
    /**
     * @brief Construct a new DDStructure object
     * @param other the structure to copy.
     *
     */
    DDStructure(const DDStructure& other);
    /**
     * @brief move assignment operator
     * @param other the structure to move.
     *
     * @return DDStructure&
     */
    DDStructure& operator=(DDStructure&& other);
    /**
     * @brief copy assignment operator
     * @param other the structure to copy.
     *
     * @return DDStructure&
     */
    DDStructure& operator=(const DDStructure& other);
    /**
     * @brief Destroy the DDStructure object
     *
     */
    ~DDStructure() = default;
    /**
     * @brief Construct a new DDStructure object
     *
     * @param name typename of the new StructType
     * @param struct_version version of the struct
     * @param alignment optional alignment (otherwise the alignment 1 is default)
     * @param comment optional comment
     * @param ddl_version optional DDL language version of this struct (defines the size of the
     * struct by alignment or not!)
     *
     */
    explicit DDStructure(const std::string& name,
                         const uint32_t struct_version = 1,
                         dd::OptionalSize alignment = {},
                         const std::string& comment = {},
                         const dd::Version& ddl_version = {});
    /**
     * @brief Construct a new DDStructure object
     *
     * @param name typename of the new StructType
     * @param xml_string the xml string to read
     * @param ddl_xml_file_version_to_parse optional DDL language version of the string.
     * @throw ddl::dd::Error if the xml string is not valid against the xsd definition! (depending
     *                       on the DDL Version).
     * @throw ddl::dd::Error if the validation level of the created DataDefinition is not at least
     *                       "good_enough"!
     * @throw ddl::dd::Error if the given \p name is not part of this description.
     */
    explicit DDStructure(
        const std::string& name,
        const std::string& xml_string,
        const dd::Version& ddl_xml_file_version_to_parse = dd::Version::ddl_version_notset);
    /**
     * @brief Adds one element to the struct using a (POD) DataType.
     *
     * Usage:
     * @code
     * DDStructure a("A");
     * a.addElement("elem1", DataType<uint16_t>());
     * @endcode
     *
     * @param element_name the name of the element
     * @param data_type the data type
     * @param array_size the arrysize of the element (by default 1)
     * @remark all default DataTypes are always part of the DataDefinition if needed. See @ref
     *         ddl::PredefinedDataTypes.
     * @throw ddl::dd::Error if the given \p element_name already exists.
     */
    DDStructure& addElement(const std::string& element_name,
                            const dd::DataType& data_type,
                            size_t array_size = 1);
    /**
     * @brief Adds one element to the struct using a (POD) DataType.
     *
     * Usage:
     * @code
     * DDStructure a("A");
     * a.addElement("elem1", DataType<uint16_t>(), 1, 2);
     * @endcode
     *
     * @param element_name the name of the element
     * @param data_type the data type
     * @param array_size the arrysize of the element (by default 1)
     * @param alignment alignment of the element
     * @remark all default DataTypes are always part of the DataDefinition if needed. See @ref
     *         ddl::PredefinedDataTypes.
     * @throw ddl::dd::Error if the given \p element_name already exists.
     */
    DDStructure& addElement(const std::string& element_name,
                            const dd::DataType& data_type,
                            size_t array_size,
                            size_t alignment);
    /**
     * @brief Adds one element to the struct using a (POD) DataType.
     *
     * Usage:
     * @code
     * DDDataType custom_type("custom", 1);
     * DDStructure a("A");
     * a.addElement("elem1", custom_type);
     * @endcode
     *
     * @param element_name the name of the element
     * @param data_type the data type
     * @param array_size the arrysize of the element (by default 1)
     * @remark all default DataTypes are always part of the DataDefinition if needed. See @ref
     *         ddl::PredefinedDataTypes.
     * @throw ddl::dd::Error if the given \p element_name already exists.
     */
    DDStructure& addElement(const std::string& element_name,
                            const DDDataType& data_type,
                            size_t array_size = 1);

    /**
     * @brief Adds one element to the struct using a (POD) DataType.
     *
     * Usage:
     * @code
     * DDDataType custom_type("custom", 1);
     * DDStructure a("A");
     * a.addElement("elem1", custom_type);
     * @endcode
     *
     * @param element_name the name of the element
     * @param data_type the data type
     * @param array_size the arrysize of the element (by default 1)
     * @param alignment alignment of the element
     * @remark all default DataTypes are always part of the DataDefinition if needed. See @ref
     *         ddl::PredefinedDataTypes.
     * @throw ddl::dd::Error if the given \p element_name already exists.
     */
    DDStructure& addElement(const std::string& element_name,
                            const DDDataType& data_type,
                            size_t array_size,
                            size_t alignment);
    /**
     * @brief Adds one element to the struct using a (POD) DataType.
     *
     * Usage:
     * @code
     * DDStructure a("A");
     * a.addElement<uint16_t>("elem1");
     * a.addElement<uint16_t>("elem2", "tUInt16");
     * @endcode
     *
     * @tparam PREDEF_DATA_TYPE The POD type (float, int32_t ... etc. )
     * @param element_name the name of the element
     * @param array_size the arrysize of the element (by default 1)
     * @param special_type_name this typename is used instead of the typename predefined in @ref
     *                          DataType
     * @remark all default DataTypes are always part of the DataDefinition if needed. See @ref
     *         ddl::PredefinedDataTypes.
     * @throw ddl::dd::Error if the given \p element_name already exists.
     */
    template <typename PREDEF_DATA_TYPE>
    DDStructure& addElement(const std::string& element_name,
                            size_t array_size = 1,
                            const std::string& special_type_name = {})
    {
        if (special_type_name.empty()) {
            addElement(element_name, DataType<PREDEF_DATA_TYPE>(), array_size);
        }
        else {
            addElement(element_name, DataType<PREDEF_DATA_TYPE>(special_type_name), array_size);
        }
        return *this;
    }

    /**
     * @brief Adds one element to the struct using a (POD) DataType.
     *
     * Usage:
     * @code
     * DDStructure a("A");
     * a.addElement<uint16_t>("elem1", 1, 2);
     * a.addElement<uint16_t>("elem2", 1, 4, "tUInt16");
     * @endcode
     *
     * @tparam PREDEF_DATA_TYPE The POD type (float, int32_t ... etc. )
     * @param element_name the name of the element
     * @param array_size the arrysize of the element (by default 1)
     * @param special_type_name this typename is used instead of the typename predefined in @ref
     *                          DataType
     * @param alignment alignment of the element
     * @remark all default DataTypes are always part of the DataDefinition if needed. See @ref
     *         ddl::PredefinedDataTypes.
     * @throw ddl::dd::Error if the given \p element_name already exists.
     */
    template <typename PREDEF_DATA_TYPE>
    DDStructure& addElement(const std::string& element_name,
                            size_t array_size,
                            size_t alignment,
                            const std::string& special_type_name = {})
    {
        if (special_type_name.empty()) {
            addElement(element_name, DataType<PREDEF_DATA_TYPE>(), array_size, alignment);
        }
        else {
            addElement(
                element_name, DataType<PREDEF_DATA_TYPE>(special_type_name), array_size, alignment);
        }
        return *this;
    }

    /**
     * @brief Adds one element to the struct using the convenience class DDEnum.
     *
     * Usage:
     * @code
     * DDEnum e1("e1", DataType<int8_t>(), { {"val11", "1"}, {"val12", "2"}, {"val13", "3"} });
     * DDStructure a("A");
     * a.addElement("elem1", e1);
     * @endcode
     *
     * @param element_name the name of the element
     * @param enum_type the enum type
     * @param array_size the arrysize of the element (by default 1)
     * @param constant_value the constant value name, defined within \p enum_type
     * @throw ddl::dd::Error if the given \p element_name already exists.
     */
    DDStructure& addElement(const std::string& element_name,
                            const DDEnum& enum_type,
                            size_t array_size,
                            const std::string& constant_value = {});

    /**
     * @brief Adds one element to the struct using the convenience class DDEnum.
     *
     * Usage:
     * @code
     * DDEnum e1("e1", DataType<int8_t>(), { {"val11", "1"}, {"val12", "2"}, {"val13", "3"} });
     * DDStructure a("A");
     * a.addElement("elem1", e1);
     * @endcode
     *
     * @param element_name the name of the element
     * @param enum_type the enum type
     * @param array_size the arrysize of the element (by default 1)
     * @param alignment alignment of the element
     * @param constant_value the constant value name, defined within \p enum_type
     * @throw ddl::dd::Error if the given \p element_name already exists.
     */
    DDStructure& addElement(const std::string& element_name,
                            const DDEnum& enum_type,
                            size_t array_size,
                            size_t alignment,
                            const std::string& constant_value = {});

    /**
     * @brief Adds one element to the struct using the convenience class DDEnum.
     *
     * Usage:
     * @code
     * DDEnum e1("e1", DataType<int8_t>(), { {"val11", "1"}, {"val12", "2"}, {"val13", "3"} });
     * DDStructure a("A");
     * a.addElement("elem1", e1);
     * @endcode
     *
     * @param element_name the name of the element
     * @param enum_type the enum type
     * @param constant_value the constant value name, defined within \p enum_type
     * @throw ddl::dd::Error if the given \p element_name already exists.
     */
    DDStructure& addElement(const std::string& element_name,
                            const DDEnum& enum_type,
                            const std::string& constant_value = {});

    /**
     * @brief Adds one element to the struct using the convenience class DDStructure.
     *
     * Usage:
     * @code
     * DDEnum e1("e1", DataType<int8_t>(), { {"val11", "1"}, {"val12", "2"}, {"val13", "3"} });
     * DDStructure a("A");
     * a.addElement("elem1", e1);
     * a.addElement<uint16_t>("elem2");
     * DDStructure b("B");
     * b.addElement("nested1", a);
     * b.addElement("nested2", a);
     * @endcode
     *
     * @param element_name the name of the element
     * @param struct_type the struct type
     * @param array_size the arrysize of the element (by default 1)
     * @throw ddl::dd::Error if the given \p element_name already exists.
     */
    DDStructure& addElement(const std::string& element_name,
                            const DDStructure& struct_type,
                            size_t array_size = 1);

    /**
     * @brief Adds one element to the struct using the convenience class DDStructure.
     *
     * Usage:
     * @code
     * DDEnum e1("e1", DataType<int8_t>(), { {"val11", "1"}, {"val12", "2"}, {"val13", "3"} });
     * DDStructure a("A");
     * a.addElement("elem1", e1);
     * a.addElement<uint16_t>("elem2");
     * DDStructure b("B");
     * b.addElement("nested1", a);
     * b.addElement("nested2", a);
     * @endcode
     *
     * @param element_name the name of the element
     * @param struct_type the struct type
     * @param array_size the arraysize of the element (by default 1)
     * @param alignment the alignment for the element
     * @throw ddl::dd::Error if the given \p element_name already exists.
     */
    DDStructure& addElement(const std::string& element_name,
                            const DDStructure& struct_type,
                            size_t array_size,
                            size_t alignment);
    /**
     * @brief Adds one element to the struct using the convenience class DDElement.
     *
     * Usage:
     * @code
     * DDStructure a("A");
     * a.addElement(ddl::DDElement("elem1", ddl::DataType<uint32_t>()));
     * @endcode
     *
     * @param element the DDElement
     * @throw ddl::dd::Error if the name of the given \p element already exists.
     */
    DDStructure& addElement(const DDElement& element);
    /**
     * @brief Adds a list of element to the struct using the convenience class DDElement.
     *
     * Usage:
     * @code
     * DDEnum e1("e1", DataType<int8_t>(), { {"val11", "1"}, {"val12", "2"}, {"val13", "3"} }
     * DDStructure a("A");
     * a.addElements({ DDElement("elem1", DataType<uint64_t>()),
     *                 DDElement("elem2", DataType<float>()) } );
     * a.addElements({ {"elem3", DataType<double>() },
     *                 {"elem4", DataType<bool>() },
     *                 {"elem5", e1 } });
     * @endcode
     *
     * @param elements the list of DDElement
     * @throw ddl::dd::Error if a name of the  given \p elements already exists.
     */
    DDStructure& addElements(const std::vector<DDElement>& elements);

    /**
     * @brief iterator to iterate the elements of the struct
     */
    typedef dd::datamodel::StructType::Elements::const_iterator const_iterator;

    /**
     * @brief returns the first elements iterator.
     *
     * @return const_iterator
     */
    const_iterator cbegin() const
    {
        return _struct_type->getElements().cbegin();
    };
    /**
     * @brief returns the end elements iterator.
     *
     * @return const_iterator
     */
    const_iterator cend() const
    {
        return _struct_type->getElements().cend();
    }
    /**
     * @brief returns the first elenents iterator for range based iterations.
     *
     * @return const_iterator
     */
    const_iterator begin() const
    {
        return _struct_type->getElements().begin();
    };
    /**
     * @brief returns the end elements iterator.
     *
     * @return const_iterator
     */
    const_iterator end() const
    {
        return _struct_type->getElements().end();
    }
    /**
     * @brief returns a valid DDL.
     *
     * @return const dd::DataDefinition&
     */
    const dd::DataDefinition& getDD() const;
    /**
     * @brief Get the Struct Type object
     *
     * @return const dd::StructType&
     */
    const dd::StructType& getStructType() const;

    /**
     * @brief Gets the Struct Data Description as XML String
     *
     * @return std::string
     */
    std::string getStructDescription() const;
    /**
     * @brief Gets the Struct Name
     *
     * @return std::string
     */
    std::string getStructName() const;

    /**
     * @brief binary compares the structs
     *
     * @param other the other struct to binary compare to
     * @return true The structs are binary comaptible in memory
     * @return false the struct are not binary compatible
     */
    bool isCompatible(const DDStructure& other) const;
    /**
     * @brief equal in names and descriptions
     *
     * @param other the other struct
     * @return true the structs are identically
     * @return false the structs are different
     */
    bool isEqual(const DDStructure& other) const;

    /**
     * @brief Retrieves the current evaluated deserialized size (in bytes) of the structure.
     * @return the size (in bytes)
     */
    size_t getSize() const;

    /**
     * @brief Retrieves the current evaluated alignment of the structure.
     * @return the alignment
     */
    size_t getAlignment() const;

    /**
     * @brief Set additional struct information like comment.
     *        This is important to create additional info in type reflection for
     *        DDStructureGenerator.
     *
     * @param comment The comment of the struct type.
     */
    void setStructInfo(const std::string& comment);
    /**
     * @brief Set additional element information to the given @p element_name.
     * @remark The element with the name @p element_name must have been added before!
     *         The info will only be set if the string of the parameter is not empty!
     *
     * @param element_name The element name to set the additional info to
     * @param description The description
     * @param comment The comment
     * @param value The value
     * @param minimum_value The minimum value
     * @param maximum_value The maximum value
     * @param default_value The default value
     * @param scale The scale
     * @param offset The offset
     *
     * @throws dd::Error if element with @p element_name does not exist.
     */
    void setElementInfo(const std::string& element_name,
                        const std::string& description = {},
                        const std::string& comment = {},
                        const std::string& value = {},
                        const std::string& minimum_value = {},
                        const std::string& maximum_value = {},
                        const std::string& default_value = {},
                        const std::string& scale = {},
                        const std::string& offset = {});
    /**
     * @brief Sets additional element unit information to the given @p element_name.
     * *      Any other unit information will be overwritten.
     * @remark The element with the name @p element_name must have been added before!
     *
     * @param element_name The element name
     * @param unit The unit to set
     *
     * @throws dd::Error if element with @p element_name does not exist.
     */
    void setElementUnit(const std::string& element_name, const DDUnit& unit);
    /**
     * @brief Sets additional element base unit information to the given @p element_name.
     *        Any other unit information will be overwritten.
     * @remark The element with the name @p element_name must have been added before!
     *
     * @param element_name The element name
     * @param base_unit The baseunit to set
     *
     * @throws dd::Error if element with @p element_name does not exist.
     */
    void setElementUnit(const std::string& element_name, const dd::BaseUnit& base_unit);

private:
    template <typename T, bool align_with_padding>
    friend class DDStructureGenerator;

    void popLastElement();

    dd::DataDefinition _dd;
    std::shared_ptr<dd::StructType> _struct_type;
    dd::OptionalSize _initial_alignment = {};
};

///@cond nodoc
namespace detail {

template <typename T, typename Enable = void>
struct arrayCount {
    static constexpr size_t value = 1;
};

template <typename T>
struct arrayCount<T, typename std::enable_if<std::is_array<T>::value>::type> {
    static constexpr size_t value =
        sizeof(T) / sizeof(typename std::remove_pointer<typename std::decay<T>::type>::type);
};

template <typename T, typename MemberType>
size_t memberPointerToOffset(MemberType T::*member)
{
    T* start = 0;
    return reinterpret_cast<size_t>(&(start->*member));
}

} // namespace detail
///@endcond nodoc

/**
 * @brief Creating a valid Structure Data Definition by a existing type and its member types.
 *
 * @tparam T The Struct type to use.
 * @tparam align_with_padding Set to true to align the structure and structure members with padding
 * bytes to reach expected size of T.
 *
 * @code
 * struct NestedStruct {
 *   uint16_t value1;
 *   uint32_t value2;
 *   uint8_t  value3;
 *   };
 * //this creates a valid DDStructure to use
 * auto nested_definition = DDStructureGenerator<NestedStruct>("NestedStruct")
 *                             .addElement("value1", &NestedStruct::value1)
 *                             .addElement("value2", &NestedStruct::value2)
 *                             .addElement("value3", &NestedStruct::value3);
 *
 * //this creates a valid enum type
 * enum MyEnum { a, b, c };
 * auto my_enum_type = DDEnumGenerator<MyEnum>("MyEnum", {{"a", a}, {"b", b}, {"c", c}});
 *
 * //create a more complex structure
 * //static arrays are determined by type
 * struct MyStruct {
 *   uint32_t     value1;
 *   NestedStruct nested;
 *   MyEnum       nested_enum[2];
 * };
 * auto my_struct = DDStructureGenerator<MyStruct>("MyStruct")
 *                              .addElement("value1", &MyStruct::value1)
 *                              .addElement("nested", &MyStruct::nested, nested_definition)
 *                              .addElement("nested_enum", &MyStruct::nested_enum, my_enum_type);
 * @endcode
 */
template <typename T, bool align_with_padding = true>
class DDStructureGenerator {
    static_assert(std::is_trivially_copyable<T>::value,
                  "You can only use structs with plain old datatypes and complex members. Do not "
                  "use pointers!");

public:
    /**
     * @brief CTOR to create a structure type based on a type.
     *
     * @param name The name of the type.
     * @param struct_version the version of the type.
     */
    explicit DDStructureGenerator(const std::string& name, uint32_t struct_version = 1)
        : _structure(name, struct_version, alignof(T))
    {
    }
    /**
     * Adds a new member of data type (POD) by member reference pointer.
     * @param [in] name The name of the member.
     * @param [in] member_offset The pointer to the member.
     * @return A reference to the structure.
     */
    template <typename MemberType>
    DDStructureGenerator& addElement(const std::string& name, MemberType T::*member_offset)
    {
        popAlignment();
        _structure
            .addElement<typename std::remove_pointer<typename std::decay<MemberType>::type>::type>(
                name,
                detail::arrayCount<MemberType>::value,
                *evaluateAlignment(
                    name, detail::memberPointerToOffset(member_offset), alignof(MemberType)));
        pushAlignment();
        return *this;
    }

    /**
     * Adds a new member of an enumeration type by member reference pointer.
     * @param [in] name The name of the member.
     * @param [in] member_offset The pointer to the member.
     * @param [in] enum_type The enumeration type.
     * @return A reference to the structure.
     */
    template <typename MemberType>
    DDStructureGenerator& addElement(const std::string& name,
                                     MemberType T::*member_offset,
                                     const DDEnum& enum_type)
    {
        popAlignment();
        _structure.addElement(name,
                              enum_type,
                              detail::arrayCount<MemberType>::value,
                              *evaluateAlignment(name,
                                                 detail::memberPointerToOffset(member_offset),
                                                 alignof(MemberType)));
        pushAlignment();
        return *this;
    }
    /**
     * Adds a new member of an structure type by member reference pointer.
     * @param [in] name The name of the member.
     * @param [in] member_offset The pointer to the member.
     * @param [in] struct_type The structure type.
     * @return A reference to the structure.
     */
    template <typename MemberType>
    DDStructureGenerator& addElement(const std::string& name,
                                     MemberType T::*member_offset,
                                     const DDStructure& struct_type)
    {
        popAlignment();
        _structure.addElement(name,
                              struct_type,
                              detail::arrayCount<MemberType>::value,
                              *evaluateAlignment(name,
                                                 detail::memberPointerToOffset(member_offset),
                                                 alignof(MemberType)));
        pushAlignment();
        return *this;
    }

    /**
     * @brief Gets the Struct Data Description as XML String
     *
     * @return std::string
     */
    std::string getStructDescription() const
    {
        validate();
        return _structure.getStructDescription();
    }
    /**
     * @brief Gets the Struct Name
     *
     * @return std::string
     */
    std::string getStructName() const
    {
        return _structure.getStructName();
    }

    /**
     * @brief returns a valid DDL.
     *
     * @return const dd::DataDefinition&
     * @throw ddl::dd::Error Throws if the evaluated size is not sizeof(T). Check if there may be
     *                       missed elements.
     */
    const dd::DataDefinition& getDD() const
    {
        validate();
        return _structure.getDD();
    }

    /**
     * @brief Get the Struct Type object
     *
     * @return const dd::StructType&
     * @throw ddl::dd::Error Throws if the evaluated size is not sizeof(T). Check if there may be
     *                       missed elements.
     */
    const dd::StructType& getStructType() const
    {
        validate();
        return _structure.getStructType();
    }
    /**
     * @brief Get the current valid DDStructure object
     *
     * @return const DDStructure&
     * @throw ddl::dd::Error Throws if the evaluated size is not sizeof(T). Check if there may be
     *                       missed elements.
     */
    const DDStructure& getStructure() const
    {
        validate();
        return _structure;
    }

    /**
     * @brief Retrieves the current evaluated size of the structure.
     * @return the size
     */
    size_t getSize() const
    {
        return _structure.getSize();
    }

    /**
     * @brief Retrieves the current evaluated alignment of the structure.
     * @return the alignment
     */
    size_t getAlignment() const
    {
        return _structure.getAlignment();
    }

    /**
     * @brief Get the current valid DDStructure object
     *
     * @return const DDStructure&
     * @throw ddl::dd::Error Throws if the evaluated size is not sizeof(T). Check if there may be
     *                       missed elements.
     */
    operator const DDStructure&() const
    {
        return getStructure();
    }

    /**
     * @copydoc DDStructure::setStructInfo
     */
    void setStructInfo(const std::string& comment)
    {
        _structure.setStructInfo(comment);
    }

    /**
     * @copydoc DDStructure::setElementInfo
     */
    void setElementInfo(const std::string& element_name,
                        const std::string& description = {},
                        const std::string& comment = {},
                        const std::string& value = {},
                        const std::string& minimum_value = {},
                        const std::string& maximum_value = {},
                        const std::string& default_value = {},
                        const std::string& scale = {},
                        const std::string& offset = {})
    {
        _structure.setElementInfo(element_name,
                                  description,
                                  comment,
                                  value,
                                  minimum_value,
                                  maximum_value,
                                  default_value,
                                  scale,
                                  offset);
    }
    /**
     * @brief Sets additional element unit information to the given @p element_name.
     *        Any other unit information will be overwritten.
     * @remark The element with the name @p element_name must have been added before!
     *
     * @param element_name The element name
     * @param unit The unit to set
     *
     * @throws dd::Error if element with @p element_name does not exist.
     */
    void setElementUnit(const std::string& element_name, const DDUnit& unit)
    {
        _structure.setElementUnit(element_name, unit);
    }
    /**
     * @brief Sets additional element base unit information to the given @p element_name.
     * *      Any other unit information will be overwritten.
     * @remark The element with the name @p element_name must have been added before!
     *
     * @param element_name The element name
     * @param base_unit The baseunit to set
     *
     * @throws dd::Error if element with @p element_name does not exist.
     */
    void setElementUnit(const std::string& element_name, const dd::BaseUnit& base_unit)
    {
        _structure.setElementUnit(element_name, base_unit);
    }

private:
    void pushAlignment()
    {
        if (align_with_padding) {
            auto current_struct_size = _structure.getSize();
            if (sizeof(T) > current_struct_size) {
                _structure.addElement<uint8_t>(
                    "__padding_final", sizeof(T) - current_struct_size, 1);
                _padded_final = true;
            }
        }
    }
    void popAlignment()
    {
        if (align_with_padding) {
            if (_padded_final) {
                _structure.popLastElement();
                _padded_final = false;
            }
        }
    }
    void validate() const
    {
        constexpr auto padding_aligned = align_with_padding;
        if (!padding_aligned) {
            if (_structure.getSize() != sizeof(T)) {
                throw ddl::dd::Error(
                    "DDStructureGenerator::validate",
                    "The generated struct '" + _structure.getStructName() +
                        "' does not have the expected size of (" + std::to_string(sizeof(T)) +
                        ") - The evaluated a size is (" + std::to_string(_structure.getSize()) +
                        "). Check if there are missed or misordered elements. ");
            }
        }
    }

    dd::OptionalSize evaluateAlignment(const std::string& element_name,
                                       size_t member_offset,
                                       size_t member_type_alignment)
    {
        // obtain the unaligned size
        auto struct_access = _structure.getDD().getStructTypeAccess(getStructName());
        auto current_struct_offset = struct_access.getStaticUnalignedStructSize();

        if (member_offset < current_struct_offset) {
            throw ddl::dd::Error(
                "evaluateAlignment",
                {std::to_string(member_offset)},
                "The evaluated struct size of '" + getStructName() + "' is already (" +
                    std::to_string(current_struct_offset) + ") - Trying to add a member at (" +
                    std::to_string(member_offset) +
                    ") at a address prior the last element. Check misordered addElement calls.");
        }

        // evaluate difference between last added element and current offset
        auto difference = member_offset - current_struct_offset;

        // find the minimum alignment to begin alignment discovery
        // this is the data_type alignment or the struct alignment
        size_t minimum_alignment = member_type_alignment;
        if (member_type_alignment > alignof(T)) {
            minimum_alignment = alignof(T);
        }

        auto valid_alignment_range = {1u, 2u, 4u, 8u, 16u, 32u, 64u};

        // find a valid alignment in the range of member_type_alignment and struct_alignment
        for (size_t alignment: valid_alignment_range) {
            // maximum alignment is the alignment of the struct
            if (alignment <= alignof(T)) {
                if (alignment >= minimum_alignment) {
                    if (alignment > difference && member_offset % alignment == 0) {
                        // we do not add padding bytes between if it fits with alignment
                        return alignment;
                    }
                }
            }
            else {
                break;
            }
        }

        // if diffence is below minimum alignment and we could not reach the position
        // the struct has been packed by the user to another alignment then the struct alignment
        if (difference < minimum_alignment) {
            for (size_t alignment: valid_alignment_range) {
                if (alignment < minimum_alignment) {
                    if (alignment > difference && member_offset % alignment == 0) {
                        // we do not add padding bytes between if it fits with alignment
                        return alignment;
                    }
                }
                else {
                    break;
                }
            }
        }

        // we could not reach the position with alignments, so we
        // align with padding if switch on
        if (align_with_padding) {
            if (difference != 0) {
                _structure.addElement<uint8_t>("__padding_" + element_name, difference, 1);
            }
            // We need to return alignment 1 to use the very next position
            return 1;
        }

        throw ddl::dd::Error(
            "evaluateAlignment",
            {std::to_string(member_offset)},
            "For the generated struct '" + getStructName() +
                "' it is not possible to find a valid member alignment to reach offset (" +
                std::to_string(member_offset) + ") from position (" +
                std::to_string(current_struct_offset) + ")");
    }

    DDStructure _structure;
    bool _padded_final = false;
};

} // namespace ddl

#endif // DDSTRUCTURE_H_INCLUDED
