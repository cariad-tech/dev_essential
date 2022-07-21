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

#include <a_util/xml.h>
#include <ddl/datamodel/xml_datamodel.h>
#include <ddl/datamodel/xml_ddfromxml_factory.h>
#include <ddl/datamodel/xml_ddtoxml_factory.h>

#include <exception>
#include <utility>

namespace ddl {

namespace dd {

namespace datamodel {

// static reading
DataDefinition fromXMLString(const std::string& xml_string,
                             const dd::Version& dd_language_version,
                             bool strict)
{
    DataDefinition new_dd;
    // parse the DataDefinition
    // the most important thing of performant reading is: We validate and re-calculate internals at
    // the very end!! this will just read and fill in the datamodel
    a_util::xml::DOM dd_string;

    if (dd_string.fromString(xml_string)) {
        a_util::xml::DOMElement root = dd_string.getRoot();
        new_dd = dd::DDFromXMLFactory<a_util::xml::DOMElement>::createDD(
            root, dd_language_version, strict);
    }
    else {
        throw dd::Error("readFromXMLString", {"..."}, dd_string.getLastError());
    }

    return new_dd;
}

std::string toXMLString(const dd::datamodel::DataDefinition& dd)
{
    a_util::xml::DOM dom;
    dom.fromString("<?xml version=\"1.0\" encoding=\"iso-8859-1\" standalone=\"no\"?>\n"
                   "<ddl:ddl xmlns:ddl=\"ddl\"> \n"
                   "</ddl:ddl>");

    a_util::xml::DOMElement root = dom.getRoot();
    dd::DDToXMLFactory<a_util::xml::DOMElement>::createNode(root, dd);

    return dom.toString();
}

// static reading
DataDefinition fromXMLFile(const std::string& xml_filepath, bool strict)
{
    dd::datamodel::DataDefinition new_ddl;
    // parse the DataDefinition
    // the most important thing of performant reading is: We validate and re-calculate internals at
    // the very end!! this will just read and fill in the datamodel
    a_util::xml::DOM ddl_file;

    if (ddl_file.load(xml_filepath)) {
        a_util::xml::DOMElement root = ddl_file.getRoot();
        // we use the version from the file
        new_ddl = dd::DDFromXMLFactory<a_util::xml::DOMElement>::createDD(
            root, dd::Version::ddl_version_notset, strict);
    }
    else {
        throw dd::Error("readFromFile", {xml_filepath}, ddl_file.getLastError());
    }

    return new_ddl;
}

void prepareXMLFile(const dd::datamodel::DataDefinition& ddl, a_util::xml::DOM& dom)
{
    dom.fromString("<?xml version=\"1.0\" encoding=\"iso-8859-1\" standalone=\"no\"?>\n"
                   "<ddl:ddl xmlns:ddl=\"ddl\"> \n"
                   "</ddl:ddl>");

    a_util::xml::DOMElement root = dom.getRoot();

    dd::DDToXMLFactory<a_util::xml::DOMElement>::createNode(root, ddl);
}

void saveXMLFile(const a_util::xml::DOM& dom, const std::string& xml_filepath)
{
    if (!dom.save(xml_filepath)) {
        throw dd::Error("writeToFile", {"xml_filepath"}, dom.getLastError());
    }
}

void toXMLFile(const dd::datamodel::DataDefinition& ddl, const std::string& xml_filepath)
{
    a_util::xml::DOM dom;
    prepareXMLFile(ddl, dom);
    saveXMLFile(dom, xml_filepath);
}

void toXMLFile(const dd::datamodel::DataDefinition& ddl,
               const std::string& xml_filepath,
               const a_util::SortingOrder order)
{
    a_util::xml::DOM dom;
    prepareXMLFile(ddl, dom);

    // sort nodes in description
    size_t nodes = {};
    a_util::xml::DOMElement root = dom.getRoot();
    root.sortNodes("header/ext_declaration", "key", order);
    root.sortNodes("units/baseunit", "name", order);
    root.sortNodes("units/prefixes", "name", order);
    nodes = root.sortNodes("units/unit", "name", order);
    root.sortNodes("units/unit[*]/refUnit", "name", order, nodes);
    root.sortNodes("datatypes/datatype", "name", order);
    nodes = root.sortNodes("enums/enum", "name", order);
    root.sortNodes("enums/enum[*]/element", "name", order, nodes);
    root.sortNodes("structs/struct", "name", order);
    root.sortNodes("streams/stream", "name", order);
    nodes = root.sortNodes("streammetatypes/streammetatype", "name", order);
    root.sortNodes("streammetatypes/streammetatype[*]/property", "name", order, nodes);

    saveXMLFile(dom, xml_filepath);
}

} // namespace datamodel
} // namespace dd
} // namespace ddl
