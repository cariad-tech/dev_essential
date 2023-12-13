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

#ifndef DDLFROMXMLELEMENT_H_INCLUDED
#define DDLFROMXMLELEMENT_H_INCLUDED

#include <a_util/xml.h>
#include <ddl/datamodel/xml_datamodel.h>

#include <string>
#include <vector>

namespace ddl {
namespace detail {

bool fromXMLElement(ddl::dd::datamodel::DataDefinition& dd,
                    a_util::xml::DOMElement& root_element,
                    std::vector<ddl::dd::Problem>& problem_list,
                    const dd::Version& ddl_language_version,
                    bool strict);

} // namespace detail
} // namespace ddl

#endif // DDLFROMXMLELEMENT_H_INCLUDED