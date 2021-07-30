/**
 * @file
 * OO DataDefinition File header
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

#ifndef DDFILE_H_INCLUDED
#define DDFILE_H_INCLUDED

#include "ddl/dd/dd.h"

#include <string>
#include <vector>

namespace ddl {
/**
 * @brief Convenience class to load and validate a Data Definition form a xml file string.
 * <br> This implementation can be used as follows:
 * @code
 * //read the whole file and provide the contained DDL
 * //if there are validation errors, see @ref ddl::dd::Error::problems
 * auto my_dd_1 = DDFile::fromXMLFile("<struct name=\"mystruct\" version=\"1\"></struct>");
 *
 * @endcode
 *
 * @see @ref DDString::fromXMLString, @ref DDString::toXMLString
 *
 */
class DDFile {
public:
    /**
     * @brief Read a file containing a data definiton in XML.
     *
     * @param xml_filepath a valid filesystem path for loading a DataDefinition xmlfile.
     * @param strict       set to true to load the datamodel exactly like defined (no mixture of DDL
     * tag definitions allowed).
     * @throw ddl::dd::Error if the xml file is not valid against the xsd definition
     *                       (depending on the DDL Version).
     * @throw ddl::dd::Error if the validation level of the created DataDefinition is not at least
     *                       good_enough!
     * @return dd::DataDefinition the valid Data Definiton of the file.
     */
    static dd::DataDefinition fromXMLFile(const std::string& xml_filepath, bool strict = false);

    /**
     * @brief Writes DataDefinition to a file containing a data definiton in XML.
     *
     * @param xml_filepath a valid filesystem path for loading a DataDefinition xmlfile.
     * @param ddl_to_write a valid DataDefinition for writing a xmlfile.
     * @throws ddl::dd::Error File could not be written.
     */
    static void toXMLFile(const dd::DataDefinition& ddl_to_write, const std::string& xml_filepath);
};

} // namespace ddl

#endif // DDFILE_H_INCLUDED
