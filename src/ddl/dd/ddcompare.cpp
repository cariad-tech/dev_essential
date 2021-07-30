/**
 * @file
 *
 * Copyright @ 2021 VW Group. All rights reserved.
 *
 *     This Source Code Form is subject to the terms of the Mozilla
 *     Public License, v. 2.0. If a copy of the MPL was not distributed
 *     with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * If it is not possible or desirable to put the notice in a particular file, then
 * You may include the notice in a location (such as a LICENSE file in a
 * relevant directory) where a recipient would be likely to look for such a notice.
 *
 * You may add additional accurate notices of copyright ownership.
 */

#include "ddl/dd/ddcompare.h"

#include "../codec/struct_layout.h"
#include "a_util/strings.h"
#include "ddl/dd/dd_predefined_datatypes.h"
#include "ddl/dd/ddstring.h"

#include <algorithm>

namespace ddl {
// define all needed error types and values locally
_MAKE_RESULT(-3, ERR_UNEXPECTED);
_MAKE_RESULT(-5, ERR_INVALID_ARG);
_MAKE_RESULT(-20, ERR_NOT_FOUND);
_MAKE_RESULT(-38, ERR_FAILED);

#define RETURN_DDLERROR_IF_FAILED_DESC(__callterm__, __desc__)                                     \
    {                                                                                              \
        a_util::result::Result ntmpRes = __callterm__;                                             \
        if (isFailed(ntmpRes)) {                                                                   \
            RETURN_ERROR_DESCRIPTION(ntmpRes, __desc__);                                           \
        }                                                                                          \
    }
#define RETURN_DDLERROR_IF_FAILED(__callterm__)                                                    \
    {                                                                                              \
        a_util::result::Result ntmpRes = __callterm__;                                             \
        if (isFailed(ntmpRes)) {                                                                   \
            return ntmpRes;                                                                        \
        }                                                                                          \
    }

#define COMPARE(__name, __method) (&(__name##1))->__method() != (&(__name##2))->__method()
#define COMPARE_VERSION(__name, __method, __flag_prefix)                                           \
    ((flags & DDCompare::__flag_prefix##_versions) &&                                              \
     ((&(__name##1))->__method() != (&(__name##2))->__method()))
#define COMPARE_DESCRIPTION(__name, __flag_prefix)                                                 \
    ((flags & DDCompare::__flag_prefix##_descriptions) &&                                          \
     ((&(__name##1))->getDescription() != (&(__name##2))->getDescription()))
#define COMPARE_COMMENT(__name, __flag_prefix)                                                     \
    ((flags & DDCompare::__flag_prefix##_comments) &&                                              \
     ((&(__name##1))->getComment() != (&(__name##2))->getComment()))
#define CHECK_NAMES(__var, __kind)                                                                 \
    if (flags & DDCompare::icf_names && (&(__var##1))->getName() != (&(__var##2))->getName()) {    \
        RETURN_DDLERROR_IF_FAILED_DESC(                                                            \
            ERR_FAILED,                                                                            \
            a_util::strings::format("The " __kind " '%s' has a different name than '%s'.",         \
                                    (&(__var##1))->getName().c_str(),                              \
                                    (&(__var##2))->getName().c_str())                              \
                .c_str());                                                                         \
    }

#define CHECK_OPTIONAL(__name, __attr) COMPARE(__name, get##__attr)

template <typename T>
static a_util::result::Result CompareSizes(
    const T& vec1, const T& vec2, uint32_t flags, const std::string& name, uint32_t subset_flags)
{
    if (flags & subset_flags) {
        if (vec1.getSize() > vec2.getSize()) {
            RETURN_DDLERROR_IF_FAILED_DESC(
                ERR_FAILED,
                std::string("The first description has more " + name + " then the second.")
                    .c_str());
        }
    }
    else {
        if (vec1.getSize() != vec2.getSize()) {
            RETURN_DDLERROR_IF_FAILED_DESC(
                ERR_FAILED,
                std::string("The descriptions have different amounts of " + name + ".").c_str());
        }
    }

    return a_util::result::SUCCESS;
}

template <typename T>
static a_util::result::Result CompareSizesVec(
    const T& vec1, const T& vec2, uint32_t flags, const std::string& name, uint32_t subset_flags)
{
    if (flags & subset_flags) {
        if (vec1.size() > vec2.size()) {
            RETURN_DDLERROR_IF_FAILED_DESC(
                ERR_FAILED,
                std::string("The first description has more " + name + " then the second.")
                    .c_str());
        }
    }
    else {
        if (vec1.size() != vec2.size()) {
            RETURN_DDLERROR_IF_FAILED_DESC(
                ERR_FAILED,
                std::string("The descriptions have different amounts of " + name + ".").c_str());
        }
    }

    return a_util::result::SUCCESS;
}

a_util::result::Result DDCompare::isBinaryEqual(const std::string& type1,
                                                const std::string& desc1,
                                                const std::string& type2,
                                                const std::string& desc2,
                                                bool is_subset)
{
    return isBinaryEqual(type1,
                         ddl::DDString::fromXMLString(desc1),
                         type2,
                         ddl::DDString::fromXMLString(desc2),
                         is_subset);
}

static a_util::result::Result CompareStaticElements(
    const std::vector<StructLayoutElement>& elements1,
    const std::vector<StructLayoutElement>& elements2)
{
    for (size_t n_element = 0; n_element < elements1.size(); ++n_element) {
        const StructLayoutElement& element1 = elements1[n_element];
        const StructLayoutElement& element2 = elements2[n_element];
        if (element1.deserialized.bit_offset != element2.deserialized.bit_offset ||
            element1.deserialized.bit_size != element2.deserialized.bit_size) {
            RETURN_ERROR_DESCRIPTION(ERR_FAILED,
                                     "The elements '%s' and '%s' differ in size or address.",
                                     element1.name.c_str(),
                                     element2.name.c_str());
        }
        if (element1.type != element2.type) {
            RETURN_ERROR_DESCRIPTION(ERR_FAILED,
                                     "The elements '%s' and '%s' have different types.",
                                     element1.name.c_str(),
                                     element2.name.c_str());
        }
    }

    return a_util::result::SUCCESS;
}

static a_util::result::Result CompareDynamicElements(
    const std::vector<DynamicStructLayoutElement>& elements1,
    const std::vector<DynamicStructLayoutElement>& elements2)
{
    for (size_t n_element = 0; n_element < elements1.size(); ++n_element) {
        const DynamicStructLayoutElement& element1 = elements1[n_element];
        const DynamicStructLayoutElement& element2 = elements2[n_element];

        if (element1.alignment != element2.alignment) {
            RETURN_ERROR_DESCRIPTION(ERR_FAILED,
                                     "The elements '%s' and '%s' have different alignment.",
                                     element1.name.c_str(),
                                     element2.name.c_str());
        }

        if (element1.static_elements.size() != element2.static_elements.size()) {
            RETURN_ERROR_DESCRIPTION(
                ERR_FAILED,
                "The elements '%s' and '%s' have different amounts of child elements.",
                element1.name.c_str(),
                element2.name.c_str());
        }

        RETURN_DDLERROR_IF_FAILED(
            CompareStaticElements(element1.static_elements, element2.static_elements));

        if (element1.dynamic_elements.size() != element2.dynamic_elements.size()) {
            RETURN_ERROR_DESCRIPTION(
                ERR_FAILED,
                "The elements '%s' and '%s' have different amounts of dynamic elements.",
                element1.name.c_str(),
                element2.name.c_str());
        }

        RETURN_DDLERROR_IF_FAILED(
            CompareDynamicElements(element1.dynamic_elements, element2.dynamic_elements));
    }
    return a_util::result::SUCCESS;
}

a_util::result::Result DDCompare::isBinaryEqual(const std::string& type1,
                                                const dd::DataDefinition& desc1,
                                                const std::string& type2,
                                                const dd::DataDefinition& desc2,
                                                bool is_subset)
{
    auto struct1_access = desc1.getStructTypeAccess(type1);
    if (!struct1_access) {
        RETURN_ERROR_DESCRIPTION(ERR_NOT_FOUND,
                                 ("Unable to find definitions for struct " + type1).c_str());
    }
    auto struct2_access = desc2.getStructTypeAccess(type2);
    if (!struct2_access) {
        RETURN_ERROR_DESCRIPTION(ERR_NOT_FOUND,
                                 ("Unable to find definitions for struct " + type2).c_str());
    }

    StructLayout layout1(struct1_access);
    StructLayout layout2(struct2_access);

    RETURN_DDLERROR_IF_FAILED(layout1.isValid());
    RETURN_DDLERROR_IF_FAILED(layout2.isValid());

    if (is_subset) {
        if (layout1.getStaticElements().size() > layout2.getStaticElements().size()) {
            RETURN_ERROR_DESCRIPTION(ERR_FAILED,
                                     "The struct '%s' has more elements than '%s'.",
                                     type1.c_str(),
                                     type2.c_str());
        }
    }
    else {
        if (layout1.getStaticElements().size() != layout2.getStaticElements().size()) {
            RETURN_ERROR_DESCRIPTION(
                ERR_FAILED,
                "The structs '%s' and '%s' have different amounts of elements.",
                type1.c_str(),
                type2.c_str());
        }
    }

    RETURN_DDLERROR_IF_FAILED(
        CompareStaticElements(layout1.getStaticElements(), layout2.getStaticElements()));

    // check dynamic stuff
    if (layout1.getDynamicElements().size() != layout2.getDynamicElements().size()) {
        RETURN_ERROR_DESCRIPTION(
            ERR_FAILED,
            "The structs '%s' and '%s' have different amounts of dynamic elements.",
            type1.c_str(),
            type2.c_str());
    }

    if (layout1.hasDynamicElements()) {
        RETURN_DDLERROR_IF_FAILED(
            CompareDynamicElements(layout1.getDynamicElements(), layout2.getDynamicElements()));
    }

    return a_util::result::SUCCESS;
}

bool areAliasTypes(const dd::DataType& dt1, const dd::DataType& dt2)
{
    if (dt1.getName() != dt2.getName()) {
        auto aliasList = ddl::PredefinedDataTypes::getInstance().getAliasTypes(dt1.getName());
        return (std::find(aliasList.begin(), aliasList.end(), dt2.getName()) != aliasList.end());
    }
    else {
        return true;
    }
}

a_util::result::Result DDCompare::isEqual(const dd::DataType& dt1,
                                          const dd::DataType& dt2,
                                          uint32_t flags)
{
    // Data types have to have the same name
    if (COMPARE(dt, getName)) {
        RETURN_DDLERROR_IF_FAILED_DESC(
            ERR_FAILED,
            a_util::strings::format("The datatypes '%s' and '%s' do not match.",
                                    dt1.getName().c_str(),
                                    dt2.getName().c_str())
                .c_str());
    }

    if (COMPARE_DESCRIPTION(dt, icf) || COMPARE(dt, getArraySize) || COMPARE(dt, getUnitName) ||
        COMPARE(dt, getBitSize)) {
        RETURN_DDLERROR_IF_FAILED_DESC(
            ERR_NOT_FOUND,
            a_util::strings::format("The datatype '%s' is different in the second description.",
                                    dt1.getName().c_str())
                .c_str());
    }

    if (flags & DDCompare::icf_visualizations_attributes) {
        if (CHECK_OPTIONAL(dt, Min) || CHECK_OPTIONAL(dt, Max)) {
            RETURN_DDLERROR_IF_FAILED_DESC(
                ERR_NOT_FOUND,
                a_util::strings::format("The datatype min/max attributes of '%s' are different in "
                                        "the second description.",
                                        dt1.getName().c_str())
                    .c_str());
        }
    }

    return a_util::result::SUCCESS;
}

a_util::result::Result DDCompare::isEqual(const dd::EnumType& enum_type1,
                                          const dd::DataDefinition& source_ddl_of_enum1,
                                          const dd::EnumType& enum_type2,
                                          const dd::DataDefinition& source_ddl_of_enum2,
                                          uint32_t flags)
{
    CHECK_NAMES(enum_type, "enum")

    const auto dt1 = source_ddl_of_enum1.getDataTypes().get(enum_type1.getDataTypeName());
    const auto dt2 = source_ddl_of_enum2.getDataTypes().get(enum_type2.getDataTypeName());
    if (!dt1 || !dt2) {
        RETURN_DDLERROR_IF_FAILED_DESC(
            ERR_FAILED,
            a_util::strings::format("The enum '%s' or '%s' has no defined basetype.",
                                    enum_type1.getName().c_str(),
                                    enum_type2.getName().c_str())
                .c_str());
    }
    if (!areAliasTypes(*dt1, *dt2)) {
        RETURN_DDLERROR_IF_FAILED_DESC(
            ERR_FAILED,
            a_util::strings::format("The enum '%s' has a different type in the second description.",
                                    enum_type1.getName().c_str())
                .c_str());
    }

    if (!(flags & DDCompare::icf_no_enum_values_check)) {
        if (COMPARE(enum_type, getElements)) {
            RETURN_DDLERROR_IF_FAILED_DESC(
                ERR_FAILED,
                a_util::strings::format(
                    "The values of enum '%s' are different in the second description.",
                    enum_type1.getName().c_str())
                    .c_str());
        }
    }

    return a_util::result::SUCCESS;
}

a_util::result::Result DDCompare::isEqual(const dd::UnitPrefix& prefix1,
                                          const dd::UnitPrefix& prefix2,
                                          uint32_t flags)
{
    CHECK_NAMES(prefix, "prefix")

    if (COMPARE(prefix, getPower) || COMPARE(prefix, getSymbol)) {
        RETURN_DDLERROR_IF_FAILED_DESC(
            ERR_FAILED,
            a_util::strings::format("The prefixes '%s' and '%s' do not match.",
                                    prefix1.getName().c_str(),
                                    prefix2.getName().c_str())
                .c_str());
    }

    return a_util::result::SUCCESS;
}

a_util::result::Result DDCompare::isEqual(const dd::BaseUnit& baseunit1,
                                          const dd::BaseUnit& baseunit2,
                                          uint32_t flags)
{
    CHECK_NAMES(baseunit, "base unit")

    if (COMPARE_DESCRIPTION(baseunit, icf) || COMPARE(baseunit, getSymbol)) {
        RETURN_DDLERROR_IF_FAILED_DESC(
            ERR_FAILED,
            a_util::strings::format("The baseunits '%s' and '%s' do not match.",
                                    baseunit1.getName().c_str(),
                                    baseunit2.getName().c_str())
                .c_str());
    }

    return a_util::result::SUCCESS;
}

a_util::result::Result DDCompare::isEqual(const dd::Unit& unit1,
                                          const dd::DataDefinition& source_ddl_of_unit1,
                                          const dd::Unit& unit2,
                                          const dd::DataDefinition& source_ddl_of_unit2,
                                          uint32_t flags)
{
    CHECK_NAMES(unit, "unit")

    if (COMPARE(unit, getDenominator) || COMPARE(unit, getNumerator) || COMPARE(unit, getOffset)) {
        RETURN_DDLERROR_IF_FAILED_DESC(
            ERR_FAILED,
            a_util::strings::format("The units '%s' and '%s' do not match.",
                                    unit1.getName().c_str(),
                                    unit2.getName().c_str())
                .c_str());
    }

    if (!(flags & DDCompare::icf_no_recursion)) {
        const auto& ref_units1 = unit1.getRefUnits();
        const auto& ref_units2 = unit2.getRefUnits();

        RETURN_DDLERROR_IF_FAILED(
            CompareSizesVec(ref_units1, ref_units2, flags, "refunits", DDCompare::icf_subset));

        for (const auto& current_ref_unit1: ref_units1) {
            dd::Unit::RefUnit ref_unit_found2;
            bool found = false;
            for (const auto& current_ref_unit2: ref_units2) {
                if (current_ref_unit2.getUnitName() == current_ref_unit1.getUnitName()) {
                    found = true;
                    ref_unit_found2 = current_ref_unit2;
                }
            }
            if (!found) {
                RETURN_DDLERROR_IF_FAILED_DESC(
                    ERR_NOT_FOUND,
                    a_util::strings::format("The reference unit '%s' of unit '%s' is not available "
                                            "in the second description.",
                                            current_ref_unit1.getUnitName().c_str(),
                                            unit1.getName().c_str())
                        .c_str());
            }

            RETURN_DDLERROR_IF_FAILED(DDCompare::isEqual(current_ref_unit1,
                                                         source_ddl_of_unit1,
                                                         ref_unit_found2,
                                                         source_ddl_of_unit2,
                                                         flags));
        }
    }

    return a_util::result::SUCCESS;
}

a_util::result::Result DDCompare::isEqual(const dd::StructType& ddl_struct1,
                                          const dd::DataDefinition& source_ddl_of_type1,
                                          const dd::StructType& ddl_struct2,
                                          const dd::DataDefinition& source_ddl_of_type2,
                                          uint32_t flags)
{
    CHECK_NAMES(ddl_struct, "struct")

    if ((flags & DDCompare::icf_versions) && ddl_struct1.getVersion() != ddl_struct2.getVersion()) {
        RETURN_DDLERROR_IF_FAILED_DESC(
            ERR_FAILED,
            a_util::strings::format("The versions of '%s' and '%s' do not match.",
                                    ddl_struct1.getName().c_str(),
                                    ddl_struct2.getName().c_str())
                .c_str());
    }

    if ((flags & DDCompare::icf_comments) && ddl_struct1.getComment() != ddl_struct2.getComment()) {
        RETURN_DDLERROR_IF_FAILED_DESC(
            ERR_FAILED,
            a_util::strings::format("The comments of '%s' and '%s' do not match.",
                                    ddl_struct1.getName().c_str(),
                                    ddl_struct2.getName().c_str())
                .c_str());
    }

    if (flags & DDCompare::icf_memory) {
        if (ddl_struct1.getAlignment() != ddl_struct2.getAlignment()) {
            RETURN_DDLERROR_IF_FAILED_DESC(
                ERR_FAILED,
                a_util::strings::format("The alignments of '%s' and '%s' do not match.",
                                        ddl_struct1.getName().c_str(),
                                        ddl_struct2.getName().c_str())
                    .c_str());
        }
    }

    if (!(flags & DDCompare::icf_no_recursion)) {
        const auto& elements1 = ddl_struct1.getElements();
        const auto& elements2 = ddl_struct2.getElements();

        if (flags & DDCompare::icf_subset) {
            if (elements1.getSize() > elements2.getSize()) {
                RETURN_DDLERROR_IF_FAILED_DESC(
                    ERR_FAILED,
                    a_util::strings::format("The there are more elements in '%s' than in '%s'.",
                                            ddl_struct1.getName().c_str(),
                                            ddl_struct2.getName().c_str())
                        .c_str());
            }
        }
        else {
            if (elements1.getSize() != elements2.getSize()) {
                RETURN_DDLERROR_IF_FAILED_DESC(
                    ERR_FAILED,
                    a_util::strings::format("The amount of elements in '%s' and '%s' is not equal.",
                                            ddl_struct1.getName().c_str(),
                                            ddl_struct2.getName().c_str())
                        .c_str());
            }
        }

        auto it_element1 = elements1.begin();
        auto it_element2 = elements2.begin();
        for (; it_element1 != elements1.end(); ++it_element1, ++it_element2) {
            const auto& element1 = *it_element1;
            const auto& element2 = *it_element2;

            // we do this special checks here, since the specialized isEqual method does not have
            // access to the other elements.
            // Dynamic check
            if (element1->getArraySize().isDynamicArraySize() !=
                element2->getArraySize().isDynamicArraySize()) {
                RETURN_DDLERROR_IF_FAILED_DESC(
                    ERR_FAILED,
                    a_util::strings::format(
                        "One of the elements '%s.%s' and '%s.%s' is dynamic the other is not.",
                        ddl_struct1.getName().c_str(),
                        element1->getName().c_str(),
                        ddl_struct2.getName().c_str(),
                        element2->getName().c_str())
                        .c_str());
            }

            // is dynamic
            if (element1->getArraySize().isDynamicArraySize()) {
                // find element positions
                auto found_array_size_element1_pos =
                    elements1.getPosOf(element1->getArraySize().getArraySizeElementName());
                if (!found_array_size_element1_pos) {
                    RETURN_DDLERROR_IF_FAILED_DESC(
                        ERR_INVALID_ARG,
                        a_util::strings::format(
                            "The array size element specified in '%s.%s' could not be found",
                            ddl_struct1.getName().c_str(),
                            element1->getName().c_str())
                            .c_str());
                }
                auto found_array_size_element2_pos =
                    elements2.getPosOf(element2->getArraySize().getArraySizeElementName());
                if (!found_array_size_element2_pos) {
                    RETURN_DDLERROR_IF_FAILED_DESC(
                        ERR_INVALID_ARG,
                        a_util::strings::format(
                            "The array size element specified in '%s.%s' could not be found",
                            ddl_struct2.getName().c_str(),
                            element2->getName().c_str())
                            .c_str());
                }

                if (found_array_size_element1_pos != found_array_size_element2_pos) {
                    RETURN_DDLERROR_IF_FAILED_DESC(
                        ERR_FAILED,
                        a_util::strings::format("The referenced dynamic array size element of the "
                                                "elements '%s.%s' and '%s.%s' do not match.",
                                                ddl_struct1.getName().c_str(),
                                                element1->getName().c_str(),
                                                ddl_struct2.getName().c_str(),
                                                element2->getName().c_str())
                            .c_str());
                }
            }

            // only the last element is allowed to be a subset
            uint32_t sub_flags = flags;
            if ((flags & DDCompare::icf_subset) && (it_element1 + 1 != elements1.end())) {
                sub_flags = flags & ~DDCompare::icf_subset;
            }

            RETURN_DDLERROR_IF_FAILED(DDCompare::isEqual(
                *element1, source_ddl_of_type1, *element2, source_ddl_of_type2, sub_flags));
        }
    }

    return a_util::result::SUCCESS;
}

a_util::result::Result DDCompare::isEqual(const dd::StreamMetaType& stream_meta_type1,
                                          const dd::DataDefinition& source_ddl_of_stream_meta_type1,
                                          const dd::StreamMetaType& stream_meta_type2,
                                          const dd::DataDefinition& source_ddl_of_stream_meta_type2,
                                          uint32_t flags)
{
    CHECK_NAMES(stream_meta_type, "streammetatype")

    if (!(flags & icf_no_recursion)) {
        auto stream_type_parent_name1 = stream_meta_type1.getParent();
        auto stream_type_parent_name2 = stream_meta_type2.getParent();
        if (stream_type_parent_name1.empty() != stream_type_parent_name2.empty()) {
            RETURN_ERROR_DESCRIPTION(
                ERR_FAILED,
                "The streammetatypes '%s' and '%s' are incompatible in parents",
                stream_meta_type1.getName().c_str(),
                stream_meta_type2.getName().c_str());
        }

        auto stream_meta_parent1 =
            source_ddl_of_stream_meta_type1.getStreamMetaTypes().get(stream_type_parent_name1);
        auto stream_meta_parent2 =
            source_ddl_of_stream_meta_type2.getStreamMetaTypes().get(stream_type_parent_name2);
        if (stream_meta_parent1 && stream_meta_parent2) {
            RETURN_DDLERROR_IF_FAILED(isEqual(*stream_meta_parent1,
                                              source_ddl_of_stream_meta_type1,
                                              *stream_meta_parent2,
                                              source_ddl_of_stream_meta_type2,
                                              flags));
        }
        else if ((stream_meta_parent1 && !stream_meta_parent2) ||
                 (stream_meta_parent2 && !stream_meta_parent1)) {
            RETURN_ERROR_DESCRIPTION(
                ERR_FAILED,
                "The streammetatypes '%s' and '%s' are incompatible in parents",
                stream_meta_type1.getName().c_str(),
                stream_meta_type2.getName().c_str());
        }
        else {
            // no parent, nothing to compare
        }
    }

    if ((flags & DDCompare::icf_names) &&
        stream_meta_type1.getParent() != stream_meta_type2.getParent()) {
        RETURN_DDLERROR_IF_FAILED_DESC(
            ERR_FAILED,
            a_util::strings::format("The parents of '%s(%s)' and '%s(%s)' do not match.",
                                    stream_meta_type1.getName().c_str(),
                                    stream_meta_type1.getParent().c_str(),
                                    stream_meta_type2.getName().c_str(),
                                    stream_meta_type2.getParent().c_str())
                .c_str());
    }

    if ((flags & DDCompare::icf_versions) &&
        stream_meta_type1.getVersion() != stream_meta_type2.getVersion()) {
        RETURN_DDLERROR_IF_FAILED_DESC(
            ERR_FAILED,
            a_util::strings::format("The versions of '%s' and '%s' do not match.",
                                    stream_meta_type1.getName().c_str(),
                                    stream_meta_type2.getName().c_str())
                .c_str());
    }

    if (!(flags & icf_no_recursion)) {
        const auto& props1 = stream_meta_type1.getProperties();
        const auto& props2 = stream_meta_type2.getProperties();

        if (flags & DDCompare::icf_subset) {
            if (props1.getSize() > props2.getSize()) {
                RETURN_DDLERROR_IF_FAILED_DESC(
                    ERR_FAILED,
                    a_util::strings::format(
                        "The there are more properties in '%s' as in the second description",
                        stream_meta_type1.getName().c_str())
                        .c_str());
            }
        }
        else {
            if (props1.getSize() != props2.getSize()) {
                RETURN_DDLERROR_IF_FAILED_DESC(
                    ERR_FAILED,
                    a_util::strings::format(
                        "The streammetatype '%s' has a different amount of properties as in "
                        "the second description.",
                        stream_meta_type1.getName().c_str())
                        .c_str());
            }
        }

        for (const auto& current_prop1: props1) {
            const auto& prop2_found = props2.get(current_prop1.second->getName());
            if (prop2_found) {
                RETURN_DDLERROR_IF_FAILED(isEqual(*current_prop1.second, *prop2_found, flags));
            }
            else {
                RETURN_DDLERROR_IF_FAILED_DESC(
                    ERR_FAILED,
                    a_util::strings::format(
                        "The property '%s' in streammetatype '%s' can not be foundin "
                        "the second description.",
                        current_prop1.second->getName().c_str(),
                        stream_meta_type1.getName().c_str())
                        .c_str());
            }
        }
    }
    return a_util::result::SUCCESS;
}

a_util::result::Result DDCompare::isEqual(const dd::Stream& stream1,
                                          const dd::DataDefinition& source_ddl_of_stream1,
                                          const dd::Stream& stream2,
                                          const dd::DataDefinition& source_ddl_of_stream2,
                                          uint32_t flags)
{
    CHECK_NAMES(stream, "stream")

    if (COMPARE_DESCRIPTION(stream, icf)) {
        RETURN_DDLERROR_IF_FAILED_DESC(
            ERR_FAILED,
            a_util::strings::format("The stream '%s' is different in the second description",
                                    stream1.getName().c_str())
                .c_str());
    }

    if (!(flags & icf_no_recursion)) {
        auto stream_type_name1 = stream1.getStreamTypeName();
        auto stream_type_name2 = stream2.getStreamTypeName();
        auto stream_type_of_type1 = source_ddl_of_stream1.getTypeOfType(stream_type_name1);
        auto stream_type_of_type2 = source_ddl_of_stream1.getTypeOfType(stream_type_name2);

        if (stream_type_of_type1 != stream_type_of_type2) {
            RETURN_DDLERROR_IF_FAILED_DESC(
                ERR_FAILED,
                a_util::strings::format("The stream '%s' and stream '%s' are different in type!",
                                        stream1.getName().c_str(),
                                        stream2.getName().c_str())
                    .c_str());
        }
        else {
            if (stream_type_of_type1 == dd::struct_type) {
                auto struct_type1 = source_ddl_of_stream1.getStructTypes().get(stream_type_name1);
                auto struct_type2 = source_ddl_of_stream2.getStructTypes().get(stream_type_name2);
                RETURN_DDLERROR_IF_FAILED(isEqual(*struct_type1,
                                                  source_ddl_of_stream1,
                                                  *struct_type2,
                                                  source_ddl_of_stream2,
                                                  flags));
            }
            else if (stream_type_of_type1 == dd::stream_meta_type) {
                auto stream_meta_type1 =
                    source_ddl_of_stream1.getStreamMetaTypes().get(stream_type_name1);
                auto stream_meta_type2 =
                    source_ddl_of_stream2.getStreamMetaTypes().get(stream_type_name2);
                RETURN_DDLERROR_IF_FAILED(isEqual(*stream_meta_type1,
                                                  source_ddl_of_stream1,
                                                  *stream_meta_type2,
                                                  source_ddl_of_stream2,
                                                  flags));
            }
            else {
                RETURN_ERROR_DESCRIPTION(
                    ERR_UNEXPECTED,
                    a_util::strings::format("Invalid type definition found for stream comparision "
                                            "of stream '%s' and stream '%s'!",
                                            stream1.getName().c_str(),
                                            stream2.getName().c_str())
                        .c_str());
            }
        }
    }

    if (!(flags & icf_no_recursion)) {
        const auto& structs1 = stream1.getStructs();
        const auto& structs2 = stream2.getStructs();

        if (flags & DDCompare::icf_subset) {
            if (structs1.getSize() > structs2.getSize()) {
                RETURN_DDLERROR_IF_FAILED_DESC(
                    ERR_FAILED,
                    a_util::strings::format(
                        "The there are more structs in '%s' in the second description",
                        stream1.getName().c_str())
                        .c_str());
            }
        }
        else {
            if (structs1.getSize() != structs2.getSize()) {
                RETURN_DDLERROR_IF_FAILED_DESC(
                    ERR_FAILED,
                    a_util::strings::format("The stream '%s' has a different amount of structs in "
                                            "the second description.",
                                            stream1.getName().c_str())
                        .c_str());
            }
        }

        auto it_struct1 = structs1.begin();
        auto it_struct2 = structs2.begin();

        for (; it_struct1 != structs1.end(); ++it_struct1, ++it_struct2) {
            const auto& ddl_struct1 = *it_struct1;
            const auto& ddl_struct2 = *it_struct2;

            // only the last element is allowed to be a subset
            uint32_t sub_flags = flags;
            if ((flags & DDCompare::icf_subset) && (it_struct1 + 1 != structs1.end())) {
                sub_flags = flags & ~DDCompare::icf_subset;
            }

            RETURN_DDLERROR_IF_FAILED(DDCompare::isEqual(*ddl_struct1,
                                                         source_ddl_of_stream1,
                                                         *ddl_struct2,
                                                         source_ddl_of_stream2,
                                                         sub_flags));
        }
    }

    return a_util::result::SUCCESS;
}

a_util::result::Result DDCompare::isEqual(const dd::datamodel::KeyValuePair& ext1,
                                          const dd::datamodel::KeyValuePair& ext2,
                                          uint32_t flags)
{
    CHECK_NAMES(ext, "ext declaration") // name == key

    if (COMPARE(ext, getValue)) {
        RETURN_DDLERROR_IF_FAILED_DESC(
            ERR_NOT_FOUND,
            a_util::strings::format(
                "The external declaration '%s' is different in the second description.",
                ext1.getName().c_str())
                .c_str());
    }

    return a_util::result::SUCCESS;
}

a_util::result::Result DDCompare::isEqual(const dd::Unit::RefUnit& ref_unit1,
                                          const dd::DataDefinition& source_ddl_of_ref_unit1,
                                          const dd::Unit::RefUnit& ref_unit2,
                                          const dd::DataDefinition& source_ddl_of_ref_unit2,
                                          uint32_t flags)
{
    if (COMPARE(ref_unit, getPower)) {
        RETURN_DDLERROR_IF_FAILED_DESC(
            ERR_FAILED,
            a_util::strings::format(
                "The reference unit '%s-%s' is different in the second description.",
                ref_unit1.getUnitName().c_str(),
                ref_unit1.getPrefixName().c_str())
                .c_str());
    }

    if (!(flags & icf_no_recursion)) {
        auto base_unit1 = source_ddl_of_ref_unit1.getBaseUnits().get(ref_unit1.getUnitName());
        auto base_unit2 = source_ddl_of_ref_unit2.getBaseUnits().get(ref_unit2.getUnitName());
        if (base_unit1 && base_unit2) {
            RETURN_DDLERROR_IF_FAILED(DDCompare::isEqual(*base_unit1, *base_unit2, flags));
        }
        else {
            if (!base_unit1) {
                RETURN_ERROR_DESCRIPTION(
                    ERR_INVALID_ARG,
                    a_util::strings::format(
                        "The base unit in reference unit '%s-%s' does not exists!",
                        ref_unit1.getUnitName().c_str(),
                        ref_unit1.getPrefixName().c_str())
                        .c_str());
            }
            if (!base_unit2) {
                RETURN_ERROR_DESCRIPTION(
                    ERR_INVALID_ARG,
                    a_util::strings::format(
                        "The base unit in reference unit '%s-%s' does not exists!",
                        ref_unit2.getUnitName().c_str(),
                        ref_unit2.getPrefixName().c_str())
                        .c_str());
            }
        }

        auto prefix_unit1 =
            source_ddl_of_ref_unit1.getUnitPrefixes().get(ref_unit1.getPrefixName());
        auto prefix_unit2 =
            source_ddl_of_ref_unit2.getUnitPrefixes().get(ref_unit2.getPrefixName());
        if (prefix_unit1 && prefix_unit2) {
            RETURN_DDLERROR_IF_FAILED(DDCompare::isEqual(*prefix_unit1, *prefix_unit2, flags));
        }
        else {
            if (!prefix_unit1) {
                RETURN_ERROR_DESCRIPTION(
                    ERR_INVALID_ARG,
                    a_util::strings::format(
                        "The unit prefix in reference unit '%s-%s' does not exists!",
                        ref_unit1.getUnitName().c_str(),
                        ref_unit1.getPrefixName().c_str())
                        .c_str());
            }
            if (!prefix_unit2) {
                RETURN_ERROR_DESCRIPTION(
                    ERR_INVALID_ARG,
                    a_util::strings::format(
                        "The unit prefix in reference unit '%s-%s' does not exists!",
                        ref_unit2.getUnitName().c_str(),
                        ref_unit2.getPrefixName().c_str())
                        .c_str());
            }
        }
    }
    if (flags & icf_names) {
        if (COMPARE(ref_unit, getPrefixName) || COMPARE(ref_unit, getUnitName)) {
            RETURN_ERROR_DESCRIPTION(
                ERR_INVALID_ARG,
                a_util::strings::format("The unit prefix '%s-%s' differ in names from '%s-%s'",
                                        ref_unit1.getUnitName().c_str(),
                                        ref_unit1.getPrefixName().c_str(),
                                        ref_unit2.getUnitName().c_str(),
                                        ref_unit2.getPrefixName().c_str())
                    .c_str());
        }
    }
    return a_util::result::SUCCESS;
}

a_util::result::Result DDCompare::isEqual(const dd::StructType::Element& element1,
                                          const dd::DataDefinition& source_ddl_of_elem1,
                                          const dd::StructType::Element& element2,
                                          const dd::DataDefinition& source_ddl_of_elem2,
                                          uint32_t flags)
{
    if (flags & DDCompare::icf_memory) {
        if (COMPARE(element, getAlignment)) {
            RETURN_DDLERROR_IF_FAILED_DESC(
                ERR_FAILED,
                a_util::strings::format(
                    "The alignments of the elements '%s' and '%s' do not match.",
                    element1.getName().c_str(),
                    element2.getName().c_str())
                    .c_str());
        }
    }

    if (COMPARE(element, getArraySize)) {
        RETURN_DDLERROR_IF_FAILED_DESC(
            ERR_FAILED,
            a_util::strings::format("The arraysizes of the elements '%s' and '%s' do not match.",
                                    element1.getName().c_str(),
                                    element2.getName().c_str())
                .c_str());
    }

    if (COMPARE(element, getValue)) {
        RETURN_DDLERROR_IF_FAILED_DESC(
            ERR_FAILED,
            a_util::strings::format(
                "The constant values of the elements '%s' and '%s' do not match.",
                element1.getName().c_str(),
                element2.getName().c_str())
                .c_str());
    }

    if (flags & icf_visualizations_attributes) {
        if (CHECK_OPTIONAL(element, Default)) {
            RETURN_DDLERROR_IF_FAILED_DESC(
                ERR_FAILED,
                a_util::strings::format(
                    "The default values of the elements '%s' and '%s' do not match.",
                    element1.getName().c_str(),
                    element2.getName().c_str())
                    .c_str());
        }

        if (CHECK_OPTIONAL(element, Scale)) {
            RETURN_DDLERROR_IF_FAILED_DESC(
                ERR_FAILED,
                a_util::strings::format(
                    "The scale values of the elements '%s' and '%s' do not match.",
                    element1.getName().c_str(),
                    element2.getName().c_str())
                    .c_str());
        }

        if (CHECK_OPTIONAL(element, Offset)) {
            RETURN_DDLERROR_IF_FAILED_DESC(
                ERR_FAILED,
                a_util::strings::format(
                    "The scale values of the elements '%s' and '%s' do not match.",
                    element1.getName().c_str(),
                    element2.getName().c_str())
                    .c_str());
        }

        if (CHECK_OPTIONAL(element, Min)) {
            RETURN_DDLERROR_IF_FAILED_DESC(
                ERR_FAILED,
                a_util::strings::format(
                    "The min values of the elements '%s' and '%s' do not match.",
                    element1.getName().c_str(),
                    element2.getName().c_str())
                    .c_str());
        }

        if (CHECK_OPTIONAL(element, Max)) {
            RETURN_DDLERROR_IF_FAILED_DESC(
                ERR_FAILED,
                a_util::strings::format(
                    "The max values of the elements '%s' and '%s' do not match.",
                    element1.getName().c_str(),
                    element2.getName().c_str())
                    .c_str());
        }
    }
    bool elem1_dynamic = !element1.getArraySize().isDynamicArraySize();
    bool elem2_dynamic = !element2.getArraySize().isDynamicArraySize();

    if (elem1_dynamic != elem2_dynamic) {
        RETURN_DDLERROR_IF_FAILED_DESC(
            ERR_FAILED,
            a_util::strings::format("One of the elements '%s' and '%s' is dynamic the other not.",
                                    element1.getName().c_str(),
                                    element2.getName().c_str())
                .c_str());
    }

    if (elem1_dynamic) {
        // here all we can do is compare the names
        if (flags & DDCompare::icf_names &&
            COMPARE(element, getArraySize().getArraySizeElementName)) {
            RETURN_DDLERROR_IF_FAILED_DESC(
                ERR_FAILED,
                a_util::strings::format(
                    "The element '%s' has a different arraysize specifier than '%s'.",
                    element1.getName().c_str(),
                    element2.getName().c_str())
                    .c_str());
        }
    }

    if (flags & DDCompare::icf_serialized) {
        if (COMPARE(element, getBitPos)) {
            RETURN_DDLERROR_IF_FAILED_DESC(
                ERR_FAILED,
                a_util::strings::format("The bitpos of the elements '%s' and '%s' does not match.",
                                        element1.getName().c_str(),
                                        element2.getName().c_str())
                    .c_str());
        }
        if (COMPARE(element, getBytePos)) {
            RETURN_DDLERROR_IF_FAILED_DESC(
                ERR_FAILED,
                a_util::strings::format("The bitpos of the elements '%s' and '%s' does not match.",
                                        element1.getName().c_str(),
                                        element2.getName().c_str())
                    .c_str());
        }
        if (COMPARE(element, getByteOrder)) {
            RETURN_DDLERROR_IF_FAILED_DESC(
                ERR_FAILED,
                a_util::strings::format("The bitpos of the elements '%s' and '%s' does not match.",
                                        element1.getName().c_str(),
                                        element2.getName().c_str())
                    .c_str());
        }
    }

    CHECK_NAMES(element, "element")

    if (!(flags & icf_no_recursion)) {
        if (flags & DDCompare::icf_units) {
            auto elem1_type_of_unit = source_ddl_of_elem1.getTypeOfUnit(element1.getUnitName());
            auto elem2_type_of_unit = source_ddl_of_elem2.getTypeOfUnit(element2.getUnitName());
            if (elem1_type_of_unit != elem2_type_of_unit) {
                RETURN_DDLERROR_IF_FAILED_DESC(
                    ERR_FAILED,
                    a_util::strings::format("The units of the elements '%s' and '%s' do not match.",
                                            element1.getName().c_str(),
                                            element2.getName().c_str())
                        .c_str());
            }
            else {
                if (elem1_type_of_unit == dd::base_unit) {
                    auto base_unit1 =
                        source_ddl_of_elem1.getBaseUnits().get(element1.getUnitName());
                    auto base_unit2 =
                        source_ddl_of_elem2.getBaseUnits().get(element2.getUnitName());
                    RETURN_DDLERROR_IF_FAILED(DDCompare::isEqual(*base_unit1, *base_unit2, flags));
                }
                else if (elem1_type_of_unit == dd::unit) {
                    auto unit1 = source_ddl_of_elem1.getUnits().get(element1.getUnitName());
                    auto unit2 = source_ddl_of_elem2.getUnits().get(element2.getUnitName());
                    RETURN_DDLERROR_IF_FAILED(DDCompare::isEqual(
                        *unit1, source_ddl_of_elem1, *unit2, source_ddl_of_elem2, flags));
                }
            }
        }

        // we handle the special case of basic data types in order to provide a meaningfull error
        // message
        auto dt1 = source_ddl_of_elem1.getDataTypes().get(element1.getTypeName());
        auto dt2 = source_ddl_of_elem2.getDataTypes().get(element2.getTypeName());
        if (dt1 && dt2) {
            if (!areAliasTypes(*dt1, *dt2)) {
                RETURN_DDLERROR_IF_FAILED_DESC(
                    ERR_FAILED,
                    a_util::strings::format(
                        "The data types of the elements '%s(%s)' and '%s(%s)' do not match.",
                        element1.getName().c_str(),
                        dt1->getName().c_str(),
                        element2.getName().c_str(),
                        dt2->getName().c_str())
                        .c_str());
            }
        }
        else {
            const auto et1 = source_ddl_of_elem1.getEnumTypes().get(element1.getTypeName());
            const auto et2 = source_ddl_of_elem2.getEnumTypes().get(element2.getTypeName());
            if (et1 && et2) {
                RETURN_DDLERROR_IF_FAILED(DDCompare::isEqual(
                    *et1, source_ddl_of_elem1, *et2, source_ddl_of_elem2, flags));
            }
            else if (et1 && dt2) {
                dt1 = source_ddl_of_elem1.getDataTypes().get(et1->getDataTypeName());
                if (!dt1) {
                    RETURN_DDLERROR_IF_FAILED_DESC(
                        ERR_FAILED,
                        a_util::strings::format(
                            "The data type of the elements '%s(%s)' and '%s(%s)' do not match.",
                            element1.getName().c_str(),
                            et1->getName().c_str(),
                            element2.getName().c_str(),
                            dt2->getName().c_str())
                            .c_str());
                }
                else {
                    // we compare the datatype names like above
                    if (!areAliasTypes(*dt1, *dt2)) {
                        RETURN_DDLERROR_IF_FAILED_DESC(
                            ERR_FAILED,
                            a_util::strings::format("The data types of the elements '%s(%s:%s)' "
                                                    "and '%s(%s)' do not match.",
                                                    element1.getName().c_str(),
                                                    et1->getName().c_str(),
                                                    dt1->getName().c_str(),
                                                    element2.getName().c_str(),
                                                    dt2->getName().c_str())
                                .c_str());
                    }
                }
            }
            else if (dt1 && et2) {
                dt2 = source_ddl_of_elem2.getDataTypes().get(et2->getDataTypeName());
                if (!dt2) {
                    RETURN_DDLERROR_IF_FAILED_DESC(
                        ERR_FAILED,
                        a_util::strings::format(
                            "The data type of the elements '%s(%s)' and '%s(%s)' do not match.",
                            element1.getName().c_str(),
                            dt1->getName().c_str(),
                            element2.getName().c_str(),
                            et2->getName().c_str())
                            .c_str());
                }
                else {
                    // we compare the datatype names like above
                    if (!areAliasTypes(*dt1, *dt2)) {
                        RETURN_DDLERROR_IF_FAILED_DESC(
                            ERR_FAILED,
                            a_util::strings::format("The data types of the elements '%s(%s)' and "
                                                    "'%s(%s:%s)' do not match.",
                                                    element1.getName().c_str(),
                                                    dt1->getName().c_str(),
                                                    element2.getName().c_str(),
                                                    et2->getName().c_str(),
                                                    dt2->getName().c_str())
                                .c_str());
                    }
                }
            }
            else {
                const auto st1 = source_ddl_of_elem1.getStructTypes().get(element1.getTypeName());
                const auto st2 = source_ddl_of_elem2.getStructTypes().get(element2.getTypeName());
                if (st1 && st2) {
                    RETURN_DDLERROR_IF_FAILED(DDCompare::isEqual(
                        *st1, source_ddl_of_elem1, *st2, source_ddl_of_elem2, flags));
                }
                else {
                    // this is the very last option -> it fails here types are not valid and also
                    // not compareable
                    RETURN_DDLERROR_IF_FAILED_DESC(
                        ERR_FAILED,
                        a_util::strings::format("The set types of the elements '%s(%s)' and "
                                                "'%s(%s)' do not match at all.",
                                                element1.getName().c_str(),
                                                element1.getTypeName().c_str(),
                                                element2.getName().c_str(),
                                                element2.getTypeName().c_str())
                            .c_str());
                }
            }
        }
    }
    else if (flags & icf_names) {
        // compare the names of the types only.
        if (COMPARE(element, getTypeName)) {
            RETURN_DDLERROR_IF_FAILED_DESC(
                ERR_FAILED,
                a_util::strings::format(
                    "The data types of the elements '%s(%s)' and '%s(%s)' do not match.",
                    element1.getName().c_str(),
                    element1.getTypeName().c_str(),
                    element2.getName().c_str(),
                    element2.getTypeName().c_str())
                    .c_str());
        }

        if (flags & DDCompare::icf_units) {
            if (COMPARE(element, getUnitName)) {
                RETURN_DDLERROR_IF_FAILED_DESC(
                    ERR_FAILED,
                    a_util::strings::format(
                        "The units of the elements '%s(%s)' and '%s(%s)' do not match.",
                        element1.getName().c_str(),
                        element1.getUnitName().c_str(),
                        element2.getName().c_str(),
                        element2.getUnitName().c_str())
                        .c_str());
            }
        }
    }

    return a_util::result::SUCCESS;
}

a_util::result::Result DDCompare::isEqual(const dd::Stream::Struct& stream_struct1,
                                          const dd::DataDefinition& source_ddl_of_stream_struct1,
                                          const dd::Stream::Struct& stream_struct2,
                                          const dd::DataDefinition& source_ddl_of_stream_struct2,
                                          uint32_t flags)
{
    if (((flags & DDCompare::icf_serialized) && COMPARE(stream_struct, getBytePos)) ||
        ((flags & DDCompare::icf_names) && COMPARE(stream_struct, getName))) {
        RETURN_DDLERROR_IF_FAILED_DESC(
            ERR_FAILED,
            a_util::strings::format("The stream structs '%s' and '%s' are not equal",
                                    stream_struct1.getName().c_str(),
                                    stream_struct2.getName().c_str())
                .c_str());
    }

    if (!(flags & icf_no_recursion)) {
        const auto st1 =
            source_ddl_of_stream_struct1.getStructTypes().get(stream_struct1.getTypeName());
        const auto st2 =
            source_ddl_of_stream_struct2.getStructTypes().get(stream_struct2.getTypeName());
        if (st1 && st2) {
            RETURN_DDLERROR_IF_FAILED(DDCompare::isEqual(
                *st1, source_ddl_of_stream_struct1, *st2, source_ddl_of_stream_struct2, flags));
        }
        else {
            RETURN_DDLERROR_IF_FAILED_DESC(
                ERR_FAILED,
                a_util::strings::format(
                    "The types stream structs '%s(%s)' and '%s(%s)' are invalid",
                    stream_struct1.getName().c_str(),
                    stream_struct1.getTypeName().c_str(),
                    stream_struct2.getName().c_str(),
                    stream_struct2.getTypeName().c_str())
                    .c_str());
        }
    }
    else if (flags & icf_names) {
        // compare the names of the types only.
        if (COMPARE(stream_struct, getTypeName)) {
            RETURN_DDLERROR_IF_FAILED_DESC(
                ERR_FAILED,
                a_util::strings::format(
                    "The types of the stream structs '%s(%s)' and '%s(%s)' do not match.",
                    stream_struct1.getName().c_str(),
                    stream_struct1.getTypeName().c_str(),
                    stream_struct2.getName().c_str(),
                    stream_struct2.getTypeName().c_str())
                    .c_str());
        }
    }
    return a_util::result::SUCCESS;
}

namespace {

template <typename T_CONTAINER>
a_util::result::Result CompareContent(const T_CONTAINER& cont1,
                                      const T_CONTAINER& cont2,
                                      const std::string& add_error_desc,
                                      uint32_t flags)
{
    for (const auto& val1: cont1) {
        const auto& val2 = cont2.get(val1.second->getName());
        if (!val2) {
            RETURN_DDLERROR_IF_FAILED_DESC(
                ERR_NOT_FOUND,
                a_util::strings::format("The %s '%s' is not available in the second description.",
                                        add_error_desc.c_str(),
                                        val1.second->getName().c_str())
                    .c_str());
        }
        RETURN_DDLERROR_IF_FAILED(DDCompare::isEqual(*val1.second, *val2, flags));
    }
    return a_util::result::SUCCESS;
}

template <typename T_CONTAINER>
a_util::result::Result CompareContentWithDesc(const T_CONTAINER& cont1,
                                              const dd::DataDefinition& desc1,
                                              const T_CONTAINER& cont2,
                                              const dd::DataDefinition& desc2,
                                              const std::string& add_error_desc,
                                              uint32_t flags)
{
    for (const auto& val1: cont1) {
        const auto& val2 = cont2.get(val1.second->getName());
        if (!val2) {
            RETURN_DDLERROR_IF_FAILED_DESC(
                ERR_NOT_FOUND,
                a_util::strings::format("The %s '%s' is not available in the second description.",
                                        add_error_desc.c_str(),
                                        val1.second->getName().c_str())
                    .c_str());
        }
        RETURN_DDLERROR_IF_FAILED(DDCompare::isEqual(*val1.second, desc1, *val2, desc2, flags));
    }
    return a_util::result::SUCCESS;
}
} // namespace

a_util::result::Result DDCompare::isEqual(const dd::DataDefinition& desc1,
                                          const dd::DataDefinition& desc2,
                                          uint32_t flags)
{
    // build flags for the item based comparisions
    uint32_t item_flags = icf_memory | icf_serialized | icf_names;
    if (flags & dcf_versions) {
        item_flags |= icf_versions;
    }
    if (flags & dcf_descriptions) {
        item_flags |= icf_descriptions;
    }
    if (flags & dcf_comments) {
        item_flags |= icf_comments;
    }
    if (flags & dcf_units || flags & dcf_base_units) {
        item_flags |= icf_units;
    }
    if (flags & dcf_visualization_attributes) {
        item_flags |= icf_visualizations_attributes;
    }
    if (flags & dcf_no_enum_values_check) {
        item_flags |= icf_no_enum_values_check;
    }
    if (flags & dcf_no_recursion) {
        item_flags |= icf_no_recursion;
    }

    if (flags & dcf_header) {
        const auto& pHeader1 = desc1.getHeader();
        const auto& pHeader2 = desc2.getHeader();

        if (COMPARE(pHeader, getAuthor) || COMPARE_DESCRIPTION(pHeader, dcf) ||
            COMPARE_VERSION(pHeader, getLanguageVersion, dcf)) {
            RETURN_DDLERROR_IF_FAILED_DESC(ERR_FAILED, "The headers are different.");
        }

        // only check dates if dcf_no_header_dates is not set
        if ((flags & dcf_no_header_dates) == 0 &&
            (COMPARE(pHeader, getDateChange) || COMPARE(pHeader, getDateCreation))) {
            RETURN_DDLERROR_IF_FAILED_DESC(ERR_FAILED, "The headers are different.");
        }

        const auto& exts1 = pHeader1.getExtDeclarations();
        const auto& exts2 = pHeader2.getExtDeclarations();

        RETURN_DDLERROR_IF_FAILED(
            CompareSizes(exts1, exts2, flags, "external declarations", dcf_subset));
        RETURN_DDLERROR_IF_FAILED(CompareContent(exts1, exts2, "external declaration", item_flags));
    }

    if (flags & dcf_data_types) {
        const auto& dts1 = desc1.getDataTypes();
        const auto& dts2 = desc2.getDataTypes();

        RETURN_DDLERROR_IF_FAILED(CompareSizes(dts1, dts2, flags, "datatypes", dcf_subset));
        RETURN_DDLERROR_IF_FAILED(CompareContent(dts1, dts2, "data type", item_flags));
    }

    if (flags & dcf_enums) {
        const auto& enums1 = desc1.getEnumTypes();
        const auto& enums2 = desc2.getEnumTypes();

        RETURN_DDLERROR_IF_FAILED(CompareSizes(enums1, enums2, flags, "enums", dcf_subset));
        RETURN_DDLERROR_IF_FAILED(
            CompareContentWithDesc(enums1, desc1, enums2, desc2, "enum type", item_flags));
    }

    if (flags & dcf_base_units) {
        const auto& units1 = desc1.getBaseUnits();
        const auto& units2 = desc2.getBaseUnits();

        RETURN_DDLERROR_IF_FAILED(CompareSizes(units1, units2, flags, "baseunits", dcf_subset));
        RETURN_DDLERROR_IF_FAILED(CompareContent(units1, units2, "base unit", item_flags));
    }

    if (flags & dcf_units) {
        const auto& units1 = desc1.getUnits();
        const auto& units2 = desc2.getUnits();

        RETURN_DDLERROR_IF_FAILED(CompareSizes(units1, units2, flags, "units", dcf_subset));
        uint32_t unit_flags = item_flags;
        if (flags & dcf_subset) {
            unit_flags |= icf_subset;
        }
        RETURN_DDLERROR_IF_FAILED(
            CompareContentWithDesc(units1, desc1, units2, desc2, "unit", unit_flags));
    }

    if (flags & dcf_prefixes) {
        const auto& prefixes1 = desc1.getUnitPrefixes();
        const auto& prefixes2 = desc2.getUnitPrefixes();

        RETURN_DDLERROR_IF_FAILED(
            CompareSizes(prefixes1, prefixes2, flags, "unit prefixes", dcf_subset));
        RETURN_DDLERROR_IF_FAILED(CompareContent(prefixes1, prefixes2, "unit prefix", item_flags));
    }

    if (flags & dcf_structs) {
        const auto& structs1 = desc1.getStructTypes();
        const auto& structs2 = desc2.getStructTypes();

        RETURN_DDLERROR_IF_FAILED(CompareSizes(structs1, structs2, flags, "structs", dcf_subset));
        RETURN_DDLERROR_IF_FAILED(
            CompareContentWithDesc(structs1, desc1, structs2, desc2, "struct type", item_flags));
    }

    if (flags & dcf_streams) {
        const auto& streams1 = desc1.getStreams();
        const auto& streams2 = desc2.getStreams();

        RETURN_DDLERROR_IF_FAILED(CompareSizes(streams1, streams2, flags, "streams", dcf_subset));
        RETURN_DDLERROR_IF_FAILED(
            CompareContentWithDesc(streams1, desc1, streams2, desc2, "stream", item_flags));
    }

    if (flags & dcf_stream_meta_types) {
        const auto& stream_meta_types1 = desc1.getStreamMetaTypes();
        const auto& stream_meta_types2 = desc2.getStreamMetaTypes();

        RETURN_DDLERROR_IF_FAILED(CompareSizes(
            stream_meta_types1, stream_meta_types2, flags, "stream meta types", dcf_subset));
        RETURN_DDLERROR_IF_FAILED(CompareContentWithDesc(
            stream_meta_types1, desc1, stream_meta_types2, desc2, "stream meta type", item_flags));
    }

    return a_util::result::SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// string based methods
//////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace {
class DDStringReader {
public:
    bool read(const std::string& xml_string)
    {
        try {
            _dd = DDString::fromXMLString(xml_string);
            return true;
        }
        catch (const dd::Error& err) {
            _problems = err.problems();
        }
        return false;
    }
    const ddl::DataDefinition& getDD() const
    {
        return _dd;
    }
    const std::vector<dd::Problem>& getProblems() const
    {
        return _problems;
    }

    std::vector<dd::Problem> _problems;
    ddl::DataDefinition _dd;
};
} // namespace

a_util::result::Result DDCompare::isEqualPrefix(const std::string& prefix_str1,
                                                const std::string& desc1,
                                                const std::string& prefix_str2,
                                                const std::string& desc2,
                                                uint32_t flags)
{
    DDStringReader desc1_string;
    if (!desc1_string.read(desc1)) {
        RETURN_ERROR_DESCRIPTION(
            ERR_INVALID_ARG,
            "desc1 for '%s' is not valid: %s",
            prefix_str1.c_str(),
            a_util::strings::join(dd::transformProblemList(desc1_string.getProblems()), "\n")
                .c_str());
    }
    DDStringReader desc2_string;
    if (!desc2_string.read(desc2)) {
        RETURN_ERROR_DESCRIPTION(
            ERR_INVALID_ARG,
            "desc2 for '%s' is not valid: %s",
            prefix_str2.c_str(),
            a_util::strings::join(dd::transformProblemList(desc2_string.getProblems()), "\n")
                .c_str());
    }

    const auto& prefix1 = desc1_string.getDD().getUnitPrefixes().get(prefix_str1);
    if (!prefix1) {
        RETURN_DDLERROR_IF_FAILED_DESC(
            ERR_NOT_FOUND,
            a_util::strings::format(
                "The prefix '%s' is not definied within the first DataDefinition.",
                prefix_str1.c_str())
                .c_str());
    }

    const auto& prefix2 = desc2_string.getDD().getUnitPrefixes().get(prefix_str2);
    if (!prefix2) {
        RETURN_DDLERROR_IF_FAILED_DESC(
            ERR_NOT_FOUND,
            a_util::strings::format(
                "The prefix '%s' is not definied within the second DataDefinition.",
                prefix_str2.c_str())
                .c_str());
    }

    return isEqual(*prefix1, *prefix2, flags);
}

a_util::result::Result DDCompare::isEqualUnit(const std::string& unit_str1,
                                              const std::string& desc1,
                                              const std::string& unit_str2,
                                              const std::string& desc2,
                                              uint32_t flags)
{
    if (unit_str1 != unit_str2) {
        RETURN_DDLERROR_IF_FAILED_DESC(
            ERR_FAILED,
            a_util::strings::format(
                "The units '%s' and '%s' do not match.", unit_str1.c_str(), unit_str1.c_str())
                .c_str());
    }
    DDStringReader desc1_string;
    if (!desc1_string.read(desc1)) {
        RETURN_ERROR_DESCRIPTION(
            ERR_INVALID_ARG,
            "desc1 for '%s' is not valid: %s",
            unit_str1.c_str(),
            a_util::strings::join(dd::transformProblemList(desc1_string.getProblems()), "\n")
                .c_str());
    }
    DDStringReader desc2_string;
    if (!desc2_string.read(desc2)) {
        RETURN_ERROR_DESCRIPTION(
            ERR_INVALID_ARG,
            "desc2 for '%s' is not valid: %s",
            unit_str2.c_str(),
            a_util::strings::join(dd::transformProblemList(desc2_string.getProblems()), "\n")
                .c_str());
    }

    const auto& baseunit1 = desc1_string.getDD().getBaseUnits().get(unit_str1);
    const auto& baseunit2 = desc2_string.getDD().getBaseUnits().get(unit_str2);
    const auto& unit1 = desc1_string.getDD().getUnits().get(unit_str1);
    const auto& unit2 = desc2_string.getDD().getUnits().get(unit_str2);

    if (!baseunit1 && !unit1) {
        RETURN_DDLERROR_IF_FAILED_DESC(
            ERR_UNEXPECTED,
            a_util::strings::format("The units '%s' is not defined in the first description.",
                                    unit_str1.c_str())
                .c_str());
    }
    if (!baseunit2 && !unit2) {
        RETURN_DDLERROR_IF_FAILED_DESC(
            ERR_UNEXPECTED,
            a_util::strings::format("The units '%s' is not defined in the second description.",
                                    unit_str2.c_str())
                .c_str());
    }

    if (baseunit1 && baseunit2) {
        return isEqual(*baseunit1, *baseunit2, flags);
    }
    else if (unit1 && unit2) {
        return isEqual(*unit1, desc1_string.getDD(), *unit2, desc2_string.getDD(), flags);
    }
    else {
        if (baseunit1) {
            RETURN_DDLERROR_IF_FAILED_DESC(
                ERR_UNEXPECTED,
                a_util::strings::format("The units '%s' is a base unit in the first description "
                                        "and the unit '%s' is a unit in the second.",
                                        unit_str1.c_str(),
                                        unit_str2.c_str())
                    .c_str());
        }
        else {
            RETURN_DDLERROR_IF_FAILED_DESC(
                ERR_UNEXPECTED,
                a_util::strings::format("The units '%s' is a unit in the first description and the "
                                        "unit '%s' is a base unit in the second.",
                                        unit_str1.c_str(),
                                        unit_str2.c_str())
                    .c_str());
        }
    }

    return a_util::result::SUCCESS;
}

a_util::result::Result DDCompare::isEqualType(const std::string& type1,
                                              const std::string& desc1,
                                              const std::string& type2,
                                              const std::string& desc2,
                                              uint32_t flags)
{
    DDStringReader desc1_string;
    if (!desc1_string.read(desc1)) {
        RETURN_ERROR_DESCRIPTION(
            ERR_INVALID_ARG,
            "desc1 for '%s' is not valid: %s",
            type1.c_str(),
            a_util::strings::join(dd::transformProblemList(desc1_string.getProblems()), "\n")
                .c_str());
    }
    DDStringReader desc2_string;
    if (!desc2_string.read(desc2)) {
        RETURN_ERROR_DESCRIPTION(
            ERR_INVALID_ARG,
            "desc2 for '%s' is not valid: %s",
            type2.c_str(),
            a_util::strings::join(dd::transformProblemList(desc2_string.getProblems()), "\n")
                .c_str());
    }

    const auto& dt1 = desc1_string.getDD().getDataTypes().get(type1);
    const auto& dt2 = desc2_string.getDD().getDataTypes().get(type2);
    const auto& ddl_struct1 = desc1_string.getDD().getStructTypes().get(type1);
    const auto& ddl_struct2 = desc2_string.getDD().getStructTypes().get(type2);
    const auto& enum1 = desc1_string.getDD().getEnumTypes().get(type1);
    const auto& enum2 = desc2_string.getDD().getEnumTypes().get(type2);
    const auto& smt1 = desc1_string.getDD().getStreamMetaTypes().get(type1);
    const auto& smt2 = desc2_string.getDD().getStreamMetaTypes().get(type2);

    if (!dt1 && !ddl_struct1 && !enum1 && !smt1) {
        RETURN_DDLERROR_IF_FAILED_DESC(
            ERR_UNEXPECTED,
            a_util::strings::format("The type '%s' is not defined in the first description.",
                                    type1.c_str())
                .c_str());
    }
    if (!dt2 && !ddl_struct2 && !enum2 && !smt2) {
        RETURN_DDLERROR_IF_FAILED_DESC(
            ERR_UNEXPECTED,
            a_util::strings::format("The type '%s' is not defined in the second description.",
                                    type2.c_str())
                .c_str());
    }

    if (dt1 && dt2) {
        return isEqual(*dt1, *dt2, flags);
    }
    else if (ddl_struct1 && ddl_struct2) {
        return isEqual(
            *ddl_struct1, desc1_string.getDD(), *ddl_struct2, desc2_string.getDD(), flags);
    }
    else if (enum1 && enum2) {
        return isEqual(*enum1, desc1_string.getDD(), *enum2, desc2_string.getDD(), flags);
    }
    else if (smt1 && smt2) {
        return isEqual(*smt1, desc1_string.getDD(), *smt2, desc2_string.getDD(), flags);
    }
    else {
        std::string kind1 = "basic data type";
        if (ddl_struct1) {
            kind1 = "struct type";
        }
        else if (enum1) {
            kind1 = "enum type";
        }
        else if (smt1) {
            kind1 = "stream meta type";
        }

        std::string kind2 = "basic data type";
        if (ddl_struct2) {
            kind2 = "struct type";
        }
        else if (enum2) {
            kind2 = "enum type";
        }
        else if (smt2) {
            kind2 = "stream meta type";
        }

        RETURN_DDLERROR_IF_FAILED_DESC(
            ERR_UNEXPECTED,
            a_util::strings::format("The first type '%s' is a %s and the second '%s' is a %s.",
                                    type1.c_str(),
                                    kind1.c_str(),
                                    type2.c_str(),
                                    kind2.c_str())
                .c_str());
    }

    return a_util::result::SUCCESS;
}

a_util::result::Result DDCompare::isEqualStream(const std::string& stream_str1,
                                                const std::string& desc1,
                                                const std::string& stream_str2,
                                                const std::string& desc2,
                                                uint32_t flags)
{
    DDStringReader desc1_string;
    if (!desc1_string.read(desc1)) {
        RETURN_ERROR_DESCRIPTION(
            ERR_INVALID_ARG,
            "desc1 for '%s' is not valid: %s",
            stream_str1.c_str(),
            a_util::strings::join(dd::transformProblemList(desc1_string.getProblems()), "\n")
                .c_str());
    }
    DDStringReader desc2_string;
    if (!desc2_string.read(desc2)) {
        RETURN_ERROR_DESCRIPTION(
            ERR_INVALID_ARG,
            "desc2 for '%s' is not valid: %s",
            stream_str2.c_str(),
            a_util::strings::join(dd::transformProblemList(desc2_string.getProblems()), "\n")
                .c_str());
    }

    const auto& stream1 = desc1_string.getDD().getStreams().get(stream_str1);
    if (!stream1) {
        RETURN_DDLERROR_IF_FAILED_DESC(
            ERR_NOT_FOUND,
            a_util::strings::format(
                "The stream '%s' is not definied within the first DataDefinition.",
                stream_str1.c_str())
                .c_str());
    }

    const auto& stream2 = desc2_string.getDD().getStreams().get(stream_str2);
    if (!stream2) {
        RETURN_DDLERROR_IF_FAILED_DESC(
            ERR_NOT_FOUND,
            a_util::strings::format(
                "The stream '%s' is not definied within the second DataDefinition.",
                stream_str2.c_str())
                .c_str());
    }

    return isEqual(*stream1, desc1_string.getDD(), *stream2, desc2_string.getDD(), flags);
}

a_util::result::Result DDCompare::isEqual(const std::string& desc1,
                                          const std::string& desc2,
                                          uint32_t flags)
{
    DDStringReader desc1_string;
    if (!desc1_string.read(desc1)) {
        RETURN_ERROR_DESCRIPTION(
            ERR_INVALID_ARG,
            "desc1 is not valid: %s",
            a_util::strings::join(dd::transformProblemList(desc1_string.getProblems()), "\n")
                .c_str());
    }
    DDStringReader desc2_string;
    if (!desc2_string.read(desc2)) {
        RETURN_ERROR_DESCRIPTION(
            ERR_INVALID_ARG,
            "desc2 is not valid: %s",
            a_util::strings::join(dd::transformProblemList(desc2_string.getProblems()), "\n")
                .c_str());
    }

    return isEqual(desc1_string.getDD(), desc2_string.getDD(), flags);
}

} // namespace ddl
