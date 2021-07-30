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

#ifndef DDLSTRING_H_INCLUDED
#define DDLSTRING_H_INCLUDED

#include "ddl/datamodel/xml_datamodel.h"
#include "ddl/dd/dd.h"

#include <string>
#include <vector>

namespace ddl {

/**
 * @brief Convenience class to load and validate a Data Definition xml string.
 * <br> This implementation can be used as follows:
 * @code
 * //(1) read the whole string and provide the contained DDL
 * //    if there are validation errors, see @ref ddl::dd::Error::problems
 * auto my_dd_1 = DDString::fromXMLString("<struct name=\"mystruct\" version=\"1\"></struct>");
 *
 *
 * //(2) read the whole string and extract the contained DDL for a given struct.
 * //    if there are validation errors, see @ref ddl::dd::Error::problems
 * auto my_dd_2 = DDString::fromXMLString("mystruct",
 *                                        "<struct name=\"mystruct\" version=\"1\"></struct>");
 *
 * @endcode
 *
 * @see @ref DDString::fromXMLString, @ref DDString::toXMLString
 *
 */
class DDString {
public:
    /**
     * @brief Read a string containing a data definiton in XML.
     *
     * @param xml_string The data defintion may contain a fully XML defined or parts of it. (struct
     * tag only, structs tag only, datatype only ... etc. but valid!)
     * @param ddl_language_version if not set the parser will have a look, if the \p xml_string
     * contains a header or it guesses.
     * @param strict set to true to load the datamodel exactly like defined (no mixture of DDL tag
     * definitions allowed).
     * @throw ddl::dd::Error if the xml string is not valid against the xsd definition! (depending
     * on the DDL Version).
     * @throw ddl::dd::Error if the validation level of the created DataDefinition is not at least
     * good_enough!
     * @return dd::DataDefinition the valid Data Definiton of the string.
     */
    static dd::DataDefinition fromXMLString(
        const std::string& xml_string,
        const dd::Version& ddl_language_version = dd::Version::ddl_version_notset,
        bool strict = false);
    /**
     * @brief Read a string containing a data definiton in XML.
     *
     * @param struct_name The data defintion will determine if the \p struct_name extists or not.
     * @param xml_string The data defintion may contain a fully XML defined or parts of it. (struct
     * tag only, structs tag only, datatype only ... etc. but valid!)
     * @param ddl_language_version if not set the parser will have a look, if the \p xml_string
     * contains a header or it guesses.
     * @param strict set to true to load the datamodel exactly like defined (no mixture of DDL tag
     * definitions allowed).
     * @throw ddl::dd::Error if the xml string is not valid against the xsd definition! (depending
     * on the DDL Version).
     * @throw ddl::dd::Error if the validation level of the created DataDefinition is not at least
     * good_enough!
     * @throw ddl::dd::Error if the given struct_name is not part of this description.
     * @return dd::DataDefinition the valid Data Definiton of the string.
     */
    static dd::DataDefinition fromXMLString(
        const std::string& struct_name,
        const std::string& xml_string,
        const dd::Version& ddl_language_version = dd::Version::ddl_version_notset,
        bool strict = false);
    /**
     * @brief This will write the given valid Data Definiton into a string as xml.
     *
     * @param dd_to_write the DataDefinition to write into a string.
     * @return std::string the string of the DataDefinition as xml
     */
    static std::string toXMLString(const dd::DataDefinition& dd_to_write);
    /**
     * @brief This will write the given valid Data Definiton into a string as xml, but only vith all
     * depending Definition of the StructType with the name \p struct_name.
     *
     * @param struct_name name of the struct that is written into the string.
     * @param dd_to_write the DataDefinition where a StructType with the name \p struct_name is
     * defined.
     * @return std::string the string of the DataDefinition as xml
     */
    static std::string toXMLString(const std::string& struct_name,
                                   const dd::DataDefinition& dd_to_write);
};

} // namespace ddl

#endif // DDLSTRING_H_INCLUDED
