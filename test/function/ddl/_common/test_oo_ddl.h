/**
 * @file
 * Test DataDefinition Helper
 *
 * Copyright @ 2021 VW Group. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef TEST_DDL_H_INCLUDED
#define TEST_DDL_H_INCLUDED

#include <a_util/filesystem.h>
#include <a_util/result.h>
#include <ddl/dd/dd.h>
#include <ddl/dd/dd_predefined_datatypes.h>
#include <ddl/dd/ddcompare.h>

#include <gtest/gtest.h>

_MAKE_RESULT(-38, ERR_FAILED);

namespace test_ddl {

#define EXPECT_EQ_WITH_RETVAL(retval_, expleft_, expright_)                                        \
    if (!(expleft_ == expright_)) {                                                                \
        retval_ = false;                                                                           \
        EXPECT_EQ(expleft_, expright_);                                                            \
    }

#define EXPECT_TRUE_WITH_RETVAL(retval_, exp_)                                                     \
    if (exp_) {                                                                                    \
    }                                                                                              \
    else {                                                                                         \
        retval_ = false;                                                                           \
        EXPECT_TRUE(exp_);                                                                         \
    }

inline bool containsDataType(const ddl::dd::DataDefinition& oo_ddl,
                             const ddl::dd::DataType& data_type)
{
    auto found_data_type = oo_ddl.getDataTypes().get(data_type.getName());
    bool retval = true;
    EXPECT_TRUE_WITH_RETVAL(retval, found_data_type);
    if (found_data_type) {
        EXPECT_EQ_WITH_RETVAL(retval, found_data_type->getUnitName(), data_type.getUnitName());
        EXPECT_EQ_WITH_RETVAL(
            retval, found_data_type->getDescription(), data_type.getDescription());
        EXPECT_EQ_WITH_RETVAL(retval, found_data_type->getBitSize(), data_type.getBitSize());
        EXPECT_EQ_WITH_RETVAL(retval, found_data_type->getArraySize(), data_type.getArraySize());
        EXPECT_EQ_WITH_RETVAL(retval, found_data_type->getMin(), data_type.getMin());
        EXPECT_EQ_WITH_RETVAL(retval, found_data_type->getMax(), data_type.getMax());
    }
    return retval;
}

inline bool containsEnumType(const ddl::dd::DataDefinition& oo_ddl,
                             const ddl::dd::EnumType& enum_type)
{
    auto found_enum_type = oo_ddl.getEnumTypes().get(enum_type.getName());
    bool retval = true;
    EXPECT_TRUE_WITH_RETVAL(retval, found_enum_type);
    EXPECT_TRUE_WITH_RETVAL(retval, oo_ddl.getDataTypes().contains(enum_type.getDataTypeName()));

    if (found_enum_type) {
        EXPECT_EQ_WITH_RETVAL(retval, found_enum_type->getName(), enum_type.getName());
        EXPECT_EQ_WITH_RETVAL(
            retval, found_enum_type->getDataTypeName(), enum_type.getDataTypeName());
        EXPECT_EQ_WITH_RETVAL(retval, found_enum_type->getElements(), enum_type.getElements());
    }
    return retval;
}

inline bool containsStructType(const ddl::dd::datamodel::DataDefinition& oo_ddl,
                               const std::string& name,
                               const std::vector<std::pair<std::string, std::string>> elements)
{
    auto found_struct_type = oo_ddl.getStructTypes().get(name);
    bool retval = true;
    EXPECT_TRUE_WITH_RETVAL(retval, found_struct_type);

    if (found_struct_type) {
        EXPECT_EQ_WITH_RETVAL(retval, elements.size(), found_struct_type->getElements().getSize());
        for (const auto& current_elem: elements) {
            auto has_element = found_struct_type->getElements().contains(current_elem.first);
            EXPECT_TRUE_WITH_RETVAL(retval, has_element);
            if (has_element) {
                EXPECT_EQ_WITH_RETVAL(
                    retval,
                    found_struct_type->getElements().get(current_elem.first)->getTypeName(),
                    current_elem.second);
            }
        }
    }
    return retval;
}

inline bool containsBaseUnit(const ddl::dd::DataDefinition& oo_ddl,
                             const ddl::dd::BaseUnit& base_unit)
{
    auto found_base_unit = oo_ddl.getBaseUnits().get(base_unit.getName());
    bool retval = true;
    EXPECT_TRUE_WITH_RETVAL(retval, found_base_unit);
    if (found_base_unit) {
        EXPECT_EQ_WITH_RETVAL(retval, *found_base_unit, base_unit);
    }
    return retval;
}

inline bool containsUnitPrefix(const ddl::dd::DataDefinition& oo_ddl,
                               const ddl::dd::UnitPrefix& unit_prefix)
{
    auto found_unit_prefix = oo_ddl.getUnitPrefixes().get(unit_prefix.getName());
    bool retval = true;
    EXPECT_TRUE_WITH_RETVAL(retval, found_unit_prefix);
    if (found_unit_prefix) {
        EXPECT_EQ_WITH_RETVAL(retval, *found_unit_prefix, unit_prefix);
    }
    return retval;
}

inline bool containsUnit(const ddl::dd::DataDefinition& oo_ddl, const ddl::dd::Unit& unit)
{
    auto found_unit = oo_ddl.getUnits().get(unit.getName());
    bool retval = true;
    EXPECT_TRUE_WITH_RETVAL(retval, found_unit);
    if (found_unit) {
        EXPECT_EQ_WITH_RETVAL(retval, *found_unit, unit);
    }
    return retval;
}

inline bool containsASpecificProblem(const std::vector<ddl::dd::Problem>& problems,
                                     const std::string& problem)
{
    for (const auto& current: problems) {
        if (current.problem_message.find(problem) != std::string::npos) {
            return true;
        }
    }
    return false;
}

inline a_util::result::Result CompOutput(const std::string& strFile1, const std::string& strFile2)
{
    using namespace ddl;
    std::string strContent1;
    std::string strContent2;

    if (a_util::filesystem::readTextFile(strFile1, strContent1) != a_util::filesystem::OK ||
        a_util::filesystem::readTextFile(strFile2, strContent2) != a_util::filesystem::OK) {
        return ERR_FAILED;
    }

    a_util::result::Result res = DDCompare::isEqual(strContent1,
                                                    strContent2,
                                                    DDCompare::dcf_all | DDCompare::dcf_subset |
                                                        DDCompare::dcf_no_header_dates);
    return res;
}

#if defined(__GNUC__) && ((__GNUC__ == 5) && (__GNUC_MINOR__ == 2))
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
#endif // defined(__GNUC__) && ((__GNUC__ == 5) && (__GNUC_MINOR__ == 2))

using bool_data_type = ddl::DataType<bool>;
using int8_data_type = ddl::DataType<int8_t>;
using uint8_data_type = ddl::DataType<uint8_t>;
using int16_data_type = ddl::DataType<int16_t>;
using uint16_data_type = ddl::DataType<uint16_t>;
using int32_data_type = ddl::DataType<int32_t>;
using uint32_data_type = ddl::DataType<uint32_t>;
using int64_data_type = ddl::DataType<int64_t>;
using uint64_data_type = ddl::DataType<uint64_t>;
using float_data_type = ddl::DataType<float>;
using double_data_type = ddl::DataType<double>;

#if defined(__GNUC__) && ((__GNUC__ == 5) && (__GNUC_MINOR__ == 2))
#pragma GCC diagnostic pop
#endif // defined(__GNUC__) && ((__GNUC__ == 5) && (__GNUC_MINOR__ == 2))

} // namespace test_ddl

#endif // TEST_DDL_H_INCLUDED
