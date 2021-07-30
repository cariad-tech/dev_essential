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

If it is not possible or desirable to put the notice in a particular file, then
You may include the notice in a location (such as a LICENSE file in a
relevant directory) where a recipient would be likely to look for such a notice.

You may add additional accurate notices of copyright ownership.
@endverbatim
 */

#ifndef DD_PREDEFINED_DATATYPES_H_INCLUDED
#define DD_PREDEFINED_DATATYPES_H_INCLUDED

#include "a_util/preprocessor/detail/disable_warnings.h"
#include "ddl/datamodel/datamodel_types.h"
#include "ddl/dd/dd_common_types.h"
#include "ddl/utilities/std_to_string.h"

#include <limits>
#include <memory>
#include <string>
#include <type_traits>
#include <unordered_map>

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
     *
     * @param name the name to use for the datatype.
     */
    DataType(const std::string& name)
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

#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
/**
 * @brief
 * @param _type_ Type to create the specialized template for
 * @param _name_ Name to use for the type by default
 */
#define DDL_DATA_TYPE_INT(_type_, _name_)                                                          \
    namespace ddl {                                                                                \
    template <>                                                                                    \
    class DataType<_type_> : public dd::datamodel::DataType {                                      \
    public:                                                                                        \
        DataType(const std::string& name)                                                          \
            : dd::datamodel::DataType(name,                                                        \
                                      sizeof(_type_) * 8,                                          \
                                      std::string("Predefined DataType for ") + name,              \
                                      1,                                                           \
                                      "",                                                          \
                                      std::to_string(std::numeric_limits<_type_>::min()),          \
                                      std::to_string(std::numeric_limits<_type_>::max()),          \
                                      alignof(_type_))                                             \
        {                                                                                          \
        }                                                                                          \
        DataType() : DataType(_name_)                                                              \
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
/// @endcond nodoc

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
    PredefinedDataTypes()
    {
        _defined_types["bool"] = {std::make_shared<DataType<bool>>("bool"), {"tBool"}};
        _defined_types["tBool"] = {std::make_shared<DataType<bool>>("tBool"), {"bool"}};

        _defined_types["char"] = {std::make_shared<DataType<char>>("char"), {"tChar"}};
        _defined_types["tChar"] = {std::make_shared<DataType<char>>("tChar"), {"char"}};

        _defined_types["uint8_t"] = {std::make_shared<DataType<uint8_t>>("uint8_t"), {"tUInt8"}};
        _defined_types["int8_t"] = {std::make_shared<DataType<int8_t>>("uint8_t"), {"tInt8"}};
        _defined_types["tUInt8"] = {std::make_shared<DataType<uint8_t>>("tUInt8"), {"uint8_t"}};
        _defined_types["tInt8"] = {std::make_shared<DataType<int8_t>>("tInt8"), {"int8_t"}};

        _defined_types["uint16_t"] = {std::make_shared<DataType<uint16_t>>("uint16_t"),
                                      {"tUInt16"}};
        _defined_types["int16_t"] = {std::make_shared<DataType<int16_t>>("int16_t"), {"tInt16"}};
        _defined_types["tUInt16"] = {std::make_shared<DataType<uint16_t>>("tUInt16"), {"uint16_t"}};
        _defined_types["tInt16"] = {std::make_shared<DataType<int16_t>>("tInt16"), {"int16_t"}};

        _defined_types["uint32_t"] = {std::make_shared<DataType<uint32_t>>("uint32_t"),
                                      {"tUInt32"}};
        _defined_types["int32_t"] = {std::make_shared<DataType<int32_t>>("int32_t"), {"tInt32"}};
        _defined_types["tUInt32"] = {std::make_shared<DataType<uint32_t>>("tUInt32"), {"uint32_t"}};
        _defined_types["tInt32"] = {std::make_shared<DataType<int32_t>>("tInt32"), {"int32_t"}};

        _defined_types["uint64_t"] = {std::make_shared<DataType<uint64_t>>("uint64_t"),
                                      {"tUInt64"}};
        _defined_types["int64_t"] = {std::make_shared<DataType<int64_t>>("int64_t"), {"tInt64"}};
        _defined_types["tUInt64"] = {std::make_shared<DataType<uint64_t>>("tUInt64"), {"uint64_t"}};
        _defined_types["tInt64"] = {std::make_shared<DataType<int64_t>>("tInt64"), {"int64_t"}};

        _defined_types["float"] = {std::make_shared<DataType<float>>("float"), {"tFloat32"}};
        _defined_types["tFloat32"] = {std::make_shared<DataType<float>>("tFloat32"), {"float"}};
        _defined_types["double"] = {std::make_shared<DataType<double>>("double"), {"tFloat64"}};
        _defined_types["tFloat64"] = {std::make_shared<DataType<double>>("tFloat64"), {"double"}};
    }

public:
    /**
     * @brief Get the Instance object
     *
     * @return PredefinedDataTypes&
     */
    static PredefinedDataTypes& getInstance()
    {
        static PredefinedDataTypes types;
        return types;
    }
    /**
     * @brief Get the Predefined Type object by name
     *
     * @param name the name to look for
     * @return std::shared_ptr<dd::datamodel::DataType>
     */
    std::shared_ptr<dd::datamodel::DataType> getPredefinedType(const std::string& name) const
    {
        auto found = _defined_types.find(name);
        if (found != _defined_types.end()) {
            return found->second._type;
        }
        return {};
    }
    /**
     * @brief Get the default alignment of a Predefined Type by name
     *
     * @param name the name to look for
     * @return dd::OptionalSize
     */
    dd::OptionalSize getDefaultAlignment(const std::string& name) const
    {
        auto found = _defined_types.find(name);
        if (found != _defined_types.end()) {
            return found->second._type->getDefaultAlignment();
        }
        return {};
    }
    /**
     * @brief Get the aliases for this type to determine simalar type (like tBool and bool)
     *
     * @param name the name to look for
     * @return dd::OptionalSize
     */
    std::vector<std::string> getAliasTypes(const std::string& name) const
    {
        auto found = _defined_types.find(name);
        if (found != _defined_types.end()) {
            return found->second._aliases;
        }
        return {};
    }

private:
    struct PredefInfo {
        std::shared_ptr<dd::datamodel::DataType> _type;
        std::vector<std::string> _aliases;
    };
    std::unordered_map<std::string, PredefInfo> _defined_types;
};

} // namespace ddl

#if defined(__GNUC__) && ((__GNUC__ == 5) && (__GNUC_MINOR__ == 2))
#pragma GCC diagnostic pop
#endif // defined(__GNUC__) && ((__GNUC__ == 5) && (__GNUC_MINOR__ == 2))

#endif // DD_PREDEFINED_DATATYPES_H_INCLUDED