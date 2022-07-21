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

#include <a_util/xml.h>
#include <ddl/datamodel/xml_ddfromxml_factory.h>
#include <ddl/datamodel/xml_ddtoxml_factory.h>

namespace ddl {
namespace detail {

bool fromXMLElement(ddl::dd::datamodel::DataDefinition& dd,
                    a_util::xml::DOMElement& root_element,
                    std::vector<ddl::dd::Problem>& problem_list,
                    const dd::Version& ddl_language_version,
                    bool strict)
{
    using namespace a_util::xml;
    auto version_to_init = ddl_language_version;
    if (version_to_init == dd::Version::ddl_version_notset) {
        version_to_init = dd::Version::ddl_version_current;
    }
    ddl::dd::datamodel::DataDefinition new_ddl(version_to_init);
    bool error_while_reading = false;

    DOMElement header_element;
    bool header_was_set = false;
    if (root_element.findNode("header", header_element)) {
        // read header
        try {
            new_ddl.setHeader(dd::DDFromXMLFactory<DOMElement>::createHeader(header_element));
        }
        // we catch only dd errors
        catch (const dd::Error& dd_error) {
            // we need to break opening at all! Without  aheader we do not know the version i.e.
            problem_list.push_back({"xml - header", dd_error.what()});
            return false;
        }
        header_was_set = true;
    }
    // if we have no header within the string we need to "guess" the DataDefinition language
    // version
    dd::Version file_version_to_use = new_ddl.getVersion();
    if (!header_was_set) {
        file_version_to_use = ddl_language_version;
    }

    // read BaseUnits
    DOMElementList baseunit_nodes;
    if (root_element.findNodes("//units/baseunit", baseunit_nodes)) {
        for (const auto& current_base_unit_node: baseunit_nodes) {
            try {
                new_ddl.getBaseUnits().emplace(
                    ddl::dd::DDFromXMLFactory<DOMElement>::createBaseUnit(current_base_unit_node));
            }
            // we catch only dd errors
            catch (const dd::Error& dd_error) {
                problem_list.push_back({"xml - baseunit", dd_error.what()});
                error_while_reading = true;
            }
        }
    }
    // read UnitPrefix
    DOMElementList prefix_nodes;
    if (root_element.findNodes("//units/prefixes", prefix_nodes)) {
        for (const auto& current_prefix_node: prefix_nodes) {
            try {
                new_ddl.getUnitPrefixes().emplace(
                    dd::DDFromXMLFactory<DOMElement>::createUnitPrefix(current_prefix_node));
            }
            // we catch only dd errors
            catch (const dd::Error& dd_error) {
                problem_list.push_back({"xml - prefix", dd_error.what()});
                error_while_reading = true;
            }
        }
    }
    // read Unit
    DOMElementList unit_nodes;
    if (root_element.findNodes("//units/unit", unit_nodes)) {
        for (const auto& current_unit_node: unit_nodes) {
            try {
                new_ddl.getUnits().emplace(
                    dd::DDFromXMLFactory<DOMElement>::createUnit(current_unit_node));
            }
            // we catch only dd errors
            catch (const dd::Error& dd_error) {
                problem_list.push_back({"xml - unit", dd_error.what()});
                error_while_reading = true;
            }
        }
    }
    // read datatypes
    DOMElementList datatype_nodes;
    if (root_element.findNodes("//datatypes/datatype", datatype_nodes)) {
        for (const auto& current_datatype_node: datatype_nodes) {
            try {
                new_ddl.getDataTypes().emplace(dd::DDFromXMLFactory<DOMElement>::createDataType(
                    current_datatype_node, file_version_to_use, strict));
            }
            // we catch only dd errors
            catch (const dd::Error& dd_error) {
                problem_list.push_back({"xml - datatype", dd_error.what()});
                error_while_reading = true;
            }
        }
    }
    // read enum types
    DOMElementList enum_nodes;
    if (root_element.findNodes("//enums/enum", enum_nodes)) {
        for (const auto& current_enum_node: enum_nodes) {
            try {
                new_ddl.getEnumTypes().emplace(
                    dd::DDFromXMLFactory<DOMElement>::createEnumType(current_enum_node));
            }
            // we catch only dd errors
            catch (const dd::Error& dd_error) {
                problem_list.push_back({"xml - enum", dd_error.what()});
                error_while_reading = true;
            }
        }
    }
    // read struct types
    DOMElementList struct_nodes;
    if (root_element.findNodes("//structs/struct", struct_nodes)) {
        for (const auto& current_struct_node: struct_nodes) {
            try {
                new_ddl.getStructTypes().emplace(dd::DDFromXMLFactory<DOMElement>::createStructType(
                    current_struct_node, file_version_to_use, strict));
            }
            // we catch only dd errors
            catch (const dd::Error& dd_error) {
                problem_list.push_back({"xml - struct", dd_error.what()});
                error_while_reading = true;
            }
        }
    }
    // special read for definition of struct tag only <struct>
    if (root_element.getName() == "struct") {
        try {
            new_ddl.getStructTypes().emplace(dd::DDFromXMLFactory<DOMElement>::createStructType(
                root_element, file_version_to_use, strict));
        }
        // we catch only dd errors
        catch (const dd::Error& dd_error) {
            problem_list.push_back({"xml - struct", dd_error.what()});
            error_while_reading = true;
        }
    }
    // read streammetatypes
    if (file_version_to_use >= dd::Version::ddl_version_40 ||
        file_version_to_use == dd::Version::ddl_version_notset || !strict) {
        DOMElementList stream_meta_type_nodes;
        if (root_element.findNodes("//streammetatypes/streammetatype", stream_meta_type_nodes)) {
            for (const auto& current_smt_node: stream_meta_type_nodes) {
                try {
                    new_ddl.getStreamMetaTypes().emplace(
                        dd::DDFromXMLFactory<DOMElement>::createStreamMetaType(current_smt_node));
                }
                // we catch only dd errors
                catch (const dd::Error& dd_error) {
                    problem_list.push_back({"xml - streammetatype", dd_error.what()});
                    error_while_reading = true;
                }
            }
        }
    }

    DOMElementList stream_nodes;
    if (root_element.findNodes("//streams/stream", stream_nodes)) {
        for (const auto& current_stream_node: stream_nodes) {
            try {
                new_ddl.getStreams().emplace(
                    dd::DDFromXMLFactory<DOMElement>::createStream(current_stream_node));
            }
            // we catch only dd errors
            catch (const dd::Error& dd_error) {
                problem_list.push_back({"xml - stream", dd_error.what()});
                error_while_reading = true;
            }
        }
    }
    if (!error_while_reading) {
        dd = std::move(new_ddl);
        return true;
    }
    else {
        return false;
    }
}
} // namespace detail
} // namespace ddl
