/**
 * @file
 * OO DataDefinition Redesign
 *
 * Copyright @ 2021 VW Group. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "dd_fromxmlelement.h"

#include <a_util/strings.h>
#include <ddl/datamodel/xml_datamodel.h>
#include <ddl/dd/ddfile.h>

namespace ddl {
namespace {

void fromXMLFile(dd::datamodel::DataDefinition& dd, const std::string& xml_filepath, bool strict)
{
    using namespace a_util::xml;
    a_util::xml::DOM ddl_dom_file;
    std::vector<ddl::dd::Problem> problem_list;

    if (ddl_dom_file.load(xml_filepath)) {
        DOMElement root = ddl_dom_file.getRoot();
        if (!detail::fromXMLElement(dd, root, problem_list, {}, strict)) {
            throw dd::Error("fromXMLFile",
                            {xml_filepath},
                            " is not a valid DDL File. See problem list!",
                            problem_list);
        }
    }
    else {
        throw dd::Error("fromXMLFile", {xml_filepath}, ddl_dom_file.getLastError());
    }
}

} // namespace

/**
 * DataDefinition File.
 *
 */
dd::DataDefinition DDFile::fromXMLFile(const std::string& xml_filepath, bool strict)
{
    dd::DataDefinition created_dd;
    dd::datamodel::DataDefinition created_datamodel;
    // this will throw for xml errors
    ddl::fromXMLFile(created_datamodel, xml_filepath, strict);
    // this will validate
    created_dd.setModel(
        std::make_shared<dd::datamodel::DataDefinition>(std::move(created_datamodel)));
    if (!created_dd.isValid(dd::ValidationLevel::good_enough)) {
        throw dd::Error("DDFile::fromXMLFile",
                        {xml_filepath},
                        "is not valid. See validation protocol!",
                        created_dd.getValidationProtocol());
    }

    return created_dd;
}

void DDFile::toXMLFile(const dd::DataDefinition& ddl_to_write, const std::string& xml_filepath)
{
    dd::datamodel::toXMLFile(*ddl_to_write.getModel(), xml_filepath);
}

void DDFile::toXMLFile(const dd::DataDefinition& ddl_to_write,
                       const std::string& xml_filepath,
                       const a_util::SortingOrder order)
{
    dd::datamodel::toXMLFile(*ddl_to_write.getModel(), xml_filepath, order);
}

} // namespace ddl
