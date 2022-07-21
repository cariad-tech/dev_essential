/**
 * @file
 * OO DataDefinition Redesign
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

#ifndef DD_DATA_MODEL_XML_H_INCLUDED
#define DD_DATA_MODEL_XML_H_INCLUDED

#include <a_util/base/enums.h>
#include <ddl/datamodel/datamodel_datadefinition.h>

#include <string>

namespace ddl {

namespace dd {

namespace datamodel {
/**
 * @brief creates a datamodel from a xml string
 *
 * @param xml_string the string as xml
 * @param ddl_language_version the language to use if no header is in the defintion
 * @param strict set to true to load the datamodel exactly like defined (no mixture of DDL tag
 * definitions allowed).
 * @return DataDefinition
 * @throws throws dd::Error if the XML given is not valid. More information on exception.
 */
DataDefinition fromXMLString(const std::string& xml_string,
                             const dd::Version& ddl_language_version = {},
                             bool strict = false);

/**
 * @brief creates a XML from the DD datamodel.
 *
 * @param dd the Data Defintion the xml string is to create
 * @return std::string the xml string
 */
std::string toXMLString(const DataDefinition& dd);

/**
 * @brief creates a datamodel from a xml file
 *
 * @param xml_filepath a valid file path (if relative the current working directory is base)
 * @param strict set to true to load the datamodel exactly like defined (no mixture of DDL tag
 * definitions allowed).
 * @return DataDefinition
 * @throws throws dd::Error if the XML given is not valid. More information on exception.
 */
DataDefinition fromXMLFile(const std::string& xml_filepath, bool strict = false);

/**
 * @brief creates a XML file from the given DD datamodel.
 *
 * @param dd           the Data Defintion the xml file is to create
 * @param xml_filepath a valid file path (if relative the current working directory is base)
 */
void toXMLFile(const DataDefinition& dd, const std::string& xml_filepath);

/**
 * @brief creates a XML file from the given DD datamodel.
 *
 * @param dd           the Data Defintion the xml file is to create
 * @param xml_filepath a valid file path (if relative the current working directory is base)
 * @param order        Sort nodes either in ascending or in descending order
 */
void toXMLFile(const DataDefinition& dd,
               const std::string& xml_filepath,
               a_util::SortingOrder order);

} // namespace datamodel
} // namespace dd
} // namespace ddl

#endif // DD_DATA_MODEL_H_INCLUDED
