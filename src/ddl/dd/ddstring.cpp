/**
 * @file
 * OO DataDefinition Redesign
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

#include "dd_fromxmlelement.h"

#include <a_util/strings.h>
#include <ddl/dd/ddstring.h>

namespace ddl {

void fromXMLString(dd::datamodel::DataDefinition& dd,
                   const std::string& xml_string,
                   const dd::Version& ddl_language_version,
                   bool strict)
{
    std::vector<ddl::dd::Problem> problem_list;
    using namespace a_util::xml;
    DOM ddl_dom_string;
    if (ddl_dom_string.fromString(xml_string)) {
        DOMElement root = ddl_dom_string.getRoot();
        if (!detail::fromXMLElement(dd, root, problem_list, ddl_language_version, strict)) {
            throw dd::Error("fromXMLString",
                            {"xml_string"},
                            " is not a valid DDL string. See problem list!",
                            problem_list);
        }
    }
    else {
        throw dd::Error("fromXMLString", {"xml_string"}, ddl_dom_string.getLastError());
    }
}

dd::DataDefinition DDString::fromXMLString(const std::string& xml_string,
                                           const dd::Version& ddl_language_version,
                                           bool strict)
{
    DataDefinition created_dd;
    dd::datamodel::DataDefinition created_datamodel;
    ddl::fromXMLString(created_datamodel, xml_string, ddl_language_version, strict);
    // this will throw if we have XML problems
    created_dd.setModel(
        std::make_shared<dd::datamodel::DataDefinition>(std::move(created_datamodel)));
    // setModel validates
    // we throw if invalid
    if (!created_dd.isValid(dd::ValidationInfo::ValidationLevel::good_enough)) {
        throw dd::Error("DDString::fromXMLString",
                        {"xml_string"},
                        "is not valid. See validation protocol!",
                        created_dd.getValidationProtocol());
    };
    return created_dd;
}

dd::DataDefinition DDString::fromXMLString(const std::string& struct_name,
                                           const std::string& xml_string,
                                           const dd::Version& ddl_language_version,
                                           bool strict)
{
    // this will throw if there are problems
    auto complete_dd = fromXMLString(xml_string, ddl_language_version, strict);
    // check if struct is in there
    auto struct_extracted = complete_dd.getStructTypes().get(struct_name);
    if (!struct_extracted) {
        throw dd::Error("DDString::DDString",
                        {struct_name, "xml_string"},
                        "The xml_string does not contain the struct_type '" + struct_name + "'!");
    }
    else {
        // We do not extract, this will slow down performance
        return complete_dd;
    }
}

std::string DDString::toXMLString(const dd::DataDefinition& dd_to_write)
{
    return dd::datamodel::toXMLString(*dd_to_write.getModel());
}

std::string DDString::toXMLString(const std::string& struct_name,
                                  const dd::DataDefinition& dd_to_write)
{
    dd::DataDefinition tmp_dd(dd_to_write.getVersion());
    if (!addTypeByName(struct_name, dd_to_write, tmp_dd)) {
        throw dd::Error(
            "toXMLString", {struct_name}, "The struct does not exist in given DataDefinition");
    }
    else {
        return dd::datamodel::toXMLString(*tmp_dd.getModel());
    }
}

} // namespace ddl
