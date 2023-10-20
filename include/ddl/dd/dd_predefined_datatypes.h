/**
 * @file
 * OO DataDefinition DataTypes header for template based DataType usages and the predefined types
 *
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */

#ifndef DD_PREDEFINED_DATATYPES_H_INCLUDED
#define DD_PREDEFINED_DATATYPES_H_INCLUDED

#include <a_util/preprocessor/detail/disable_warnings.h>
#include <ddl/datamodel/datamodel_types.h>
#include <ddl/dd/dd_common_types.h>
#include <ddl/utilities/std_to_string.h>

#include <limits>
#include <memory>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <vector>

namespace ddl {

/**
 * @brief Generator template to create DataType for the plain c-types.
 *
 * @tparam T The POD type to use
 */
template <typename T>
class DataType : public dd::datamodel::DataType {
public:
    /**
     * @brief CTOR. creates the datatype with a different name then defined by default.
     */
    DataType(const std::string&)
    {
        static_assert(std::is_arithmetic<T>::value,
                      "To use this type, only primitive types are allowed");
    }
    /**
     * @brief CTOR. creates the datatype with the default name for the type \p T.
     *
     */
    DataType()
    {
        static_assert(std::is_arithmetic<T>::value,
                      "To use this type, only primitive types are allowed");
    }
};

} // namespace ddl

#if defined(__GNUC__) && ((__GNUC__ == 5) && (__GNUC_MINOR__ == 2))
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
#endif // defined(__GNUC__) && ((__GNUC__ == 5) && (__GNUC_MINOR__ == 2))

/**
 * Expands to @c DataType<type> inheriting from @c dd::datamodel::DataType
 * @param type Type to create the specialized template for
 * @param default_name Name to use for the type by default
 */
#define DDL_DATA_TYPE_INT(type, default_name)                                                      \
    namespace ddl {                                                                                \
    template <>                                                                                    \
    class DataType<type> : public dd::datamodel::DataType {                                        \
    public:                                                                                        \
        DataType(const std::string& name)                                                          \
            : dd::datamodel::DataType(name,                                                        \
                                      sizeof(type) * 8,                                            \
                                      std::string("Predefined DataType for ") + name,              \
                                      1,                                                           \
                                      "",                                                          \
                                      std::to_string((std::numeric_limits<type>::min)()),          \
                                      std::to_string((std::numeric_limits<type>::max)()),          \
                                      alignof(type))                                               \
        {                                                                                          \
        }                                                                                          \
        DataType() : DataType(default_name)                                                        \
        {                                                                                          \
        }                                                                                          \
    };                                                                                             \
    } /* namespace ddl */                                                                          \
    static_assert(true, "Simply to make sure a closing semicolon ';' is used for the macro")

/// @cond nodoc
DDL_DATA_TYPE_INT(uint8_t, "tUInt8");
DDL_DATA_TYPE_INT(uint16_t, "tUInt16");
DDL_DATA_TYPE_INT(uint32_t, "tUInt32");
DDL_DATA_TYPE_INT(uint64_t, "tUInt64");

DDL_DATA_TYPE_INT(int8_t, "tInt8");
DDL_DATA_TYPE_INT(int16_t, "tInt16");
DDL_DATA_TYPE_INT(int32_t, "tInt32");
DDL_DATA_TYPE_INT(int64_t, "tInt64");
/// @endcond

namespace ddl {

/**
 * @brief The datatype for bool
 *
 * @tparam bool
 */
template <>
class DataType<bool> : public dd::datamodel::DataType {
public:
    /**
     * @brief CTOR. creates the datatype with a different name then defined by default.
     *
     * @param name the name to use for the datatype.
     */
    DataType(const std::string& name)
        : dd::datamodel::DataType(name,
                                  sizeof(bool) * 8,
                                  std::string("Predefined DataType for ") + name,
                                  1,
                                  {},
                                  {},
                                  {},
                                  alignof(bool))
    {
    }
    /**
     * @brief CTOR. creates the datatype with the default name for the type \c bool.
     *
     */
    DataType() : DataType("tBool")
    {
    }
};

/**
 * @brief The datatype for char
 *
 * @tparam char
 */
template <>
class DataType<char> : public dd::datamodel::DataType {
public:
    /**
     * @brief CTOR. creates the datatype with a different name then defined by default.
     *
     * @param name the name to use for the datatype.
     */
    DataType(const std::string& name)
        : dd::datamodel::DataType(name,
                                  sizeof(char) * 8,
                                  std::string("Predefined DataType for ") + name,
                                  1,
                                  {},
                                  {},
                                  {},
                                  alignof(char))
    {
    }
    /**
     * @brief CTOR. creates the datatype with the default name for the type \c char.
     *
     */
    DataType() : DataType("tChar")
    {
    }
};

/**
 * @brief The datatype for float
 *
 * @tparam float
 */
template <>
class DataType<float> : public dd::datamodel::DataType {
public:
    /**
     * @brief CTOR. creates the datatype with a different name then defined by default.
     *
     * @param name the name to use for the datatype.
     */
    DataType(const std::string& name)
        : dd::datamodel::DataType(name,
                                  sizeof(float) * 8,
                                  std::string("Predefined DataType for ") + name,
                                  1,
                                  {},
                                  {},
                                  {},
                                  alignof(float))
    {
    }
    /**
     * @brief CTOR. creates the datatype with the default name for the type \c float.
     *
     */
    DataType() : DataType("tFloat32")
    {
    }
};

/**
 * @brief The datatype for double
 *
 * @tparam double
 */
template <>
class DataType<double> : public dd::datamodel::DataType {
public:
    /**
     * @brief CTOR. creates the datatype with a different name then defined by default.
     *
     * @param name the name to use for the datatype.
     */
    DataType(const std::string& name)
        : dd::datamodel::DataType(name,
                                  sizeof(double) * 8,
                                  std::string("Predefined DataType for ") + name,
                                  1,
                                  {},
                                  {},
                                  {},
                                  alignof(double))
    {
    }
    /**
     * @brief CTOR. creates the datatype with the default name for the type \c double.
     *
     */
    DataType() : DataType("tFloat64")
    {
    }
};

/**
 * @brief Predefined types of the DataDefinition.
 * These types can be used without explicitely adding it to a dd::DataDefinition!
 *
 * Following types are possible:
 * \li DataType<bool>()
 * \li DataType<bool>("bool")
 * \li DataType<char>()
 * \li DataType<char>("char")
 * \li DataType<int8_t>()
 * \li DataType<int8_t>("int8_t")
 * \li DataType<uint8_t>()
 * \li DataType<uint8_t>("uint8_t")
 * \li DataType<int16_t>()
 * \li DataType<int16_t>("int16_t")
 * \li DataType<uint16_t>()
 * \li DataType<uint16_t>("uint16_t")
 * \li DataType<int32_t>()
 * \li DataType<int32_t>("int32_t")
 * \li DataType<uint32_t>()
 * \li DataType<uint32_t>("uint32_t")
 * \li DataType<int64_t>()
 * \li DataType<int64_t>("int64_t")
 * \li DataType<uint64_t>()
 * \li DataType<uint64_t>("uint64_t")
 * \li DataType<float>()
 * \li DataType<float>("float")
 * \li DataType<double>()
 * \li DataType<double>("double")
 *
 */
class PredefinedDataTypes {
private:
    /**
     * @brief Construct a new Predefined Data Types object
     *
     */
    PredefinedDataTypes();
    /**
     * @brief Destructor
     *
     */
    ~PredefinedDataTypes();

public:
    /**
     * @brief Get the Instance object
     *
     * @return PredefinedDataTypes&
     */
    static const PredefinedDataTypes& getInstance();
    /**
     * @brief Get the Predefined Type object by name
     *
     * @param[in] name Name of the predefined type to look for
     * @return std::shared_ptr<dd::datamodel::DataType>
     */
    std::shared_ptr<dd::datamodel::DataType> getPredefinedType(const std::string& name) const;
    /**
     * @brief Get the default alignment of a Predefined Type by name
     *
     * @param[in] name Type name to search the default alignment for
     * @return dd::OptionalSize
     */
    dd::OptionalSize getDefaultAlignment(const std::string& name) const;
    /**
     * @brief Get the aliases for this type to determine similar type (like tBool and bool)
     *
     * @param[in] name Type name to search the aliases for
     * @return Vector object of strings containing all aliases for @c name
     */
    std::vector<std::string> getAliasTypes(const std::string& name) const;
    /**
     * @brief Gets a vector of all the predefined data types.
     *
     * @return std::vector<std::shared_ptr<dd::datamodel::DataType>> The predefined data types.
     */
    std::vector<std::shared_ptr<dd::datamodel::DataType>> getPredefinedTypes() const;

private:
    struct PredefInfo {
        std::shared_ptr<dd::datamodel::DataType> _type;
        std::vector<std::string> _aliases;
    };
    const std::unordered_map<std::string, PredefInfo> _defined_types;
};

} // namespace ddl

#if defined(__GNUC__) && ((__GNUC__ == 5) && (__GNUC_MINOR__ == 2))
#pragma GCC diagnostic pop
#endif // defined(__GNUC__) && ((__GNUC__ == 5) && (__GNUC_MINOR__ == 2))

#endif // DD_PREDEFINED_DATATYPES_H_INCLUDED
