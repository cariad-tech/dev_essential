/**
 * @file
 * OO DataDefinition Header - Observable Data Defintion Header class
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

#ifndef DD_DATA_MODEL_HEADER_H_INCLUDED
#define DD_DATA_MODEL_HEADER_H_INCLUDED

#include <ddl/datamodel/datamodel_base.h>
#include <ddl/datamodel/datamodel_keyvalue.h>
#include <ddl/datamodel/infomodel_base.h>
#include <ddl/dd/dd_common_types.h>
#include <ddl/utilities/dd_access_map.h>

#include <string>
#include <unordered_map>
#include <vector>

namespace ddl {

namespace dd {

namespace datamodel {
/**
 * @brief Data Definition datamodel for the Header.
 *
 */
class Header : public ModelSubject<Header>, public InfoMap {
public:
    /**
     * @brief no default CTOR.
     */
    Header() = delete;
    /**
     * @brief copy CTOR.
     *
     * @param other other header.
     */
    Header(const Header& other);
    /**
     * @brief copy assignment.
     *
     * @param other other header.
     */
    Header& operator=(const Header& other);
    /**
     * @brief move CTOR.
     */
    Header(Header&&) = default;
    /**
     * @brief move assignment.
     */
    Header& operator=(Header&&) = default;
    /**
     * @brief Destroys the Header
     */
    virtual ~Header() = default;

    /**
     * @brief Use of the key value pair to declare the ext declaration.
     *
     */
    using ExtDeclaration = KeyValuePair;
    /**
     * @brief map container for a set of ext declarations.
     */
    using ExtDeclarations = utility::TypeAccessMap<ExtDeclaration, Header>;
    /// @cond nodoc
    friend ExtDeclarations;
    /// @endcond nodoc

    /**
     * @brief Construct a new Header object
     *
     * @param ddl_version The Language version
     * @param author the author
     * @param date_creation the date of creation
     * @param date_change the date of last change
     * @param description the description
     * @param ext_declarations a vector of ext declations that are added immediatelly.
     * @throws throws if ext_declarations contains ext declarations with the same key.
     */
    Header(const Version& ddl_version,
           const std::string& author = {},
           const std::string& date_creation = {},
           const std::string& date_change = {},
           const std::string& description = {},
           const std::vector<ExtDeclaration>& ext_declarations = {});

    /**
     * @brief Equality operator.
     *
     * @param other the other header to compare to
     * @return true if every single content is the same
     * @return false if content is different
     */
    bool operator==(const Header& other) const;
    /**
     * @brief Non-Equality operator.
     *
     * @param other the other header to compare to
     * @return false if every single content is the same
     * @return true if content is different
     */
    bool operator!=(const Header& other) const;

    /**
     * @brief Get the Language Version
     *
     * @return Version
     */
    Version getLanguageVersion() const;
    /**
     * @brief Set the Language Version
     *
     * @param ddl_version The language version
     * @remark This change is observable.
     */
    void setLanguageVersion(const Version& ddl_version);

    /**
     * @brief Get the Author
     *
     * @return const std::string&
     */
    const std::string& getAuthor() const;
    /**
     * @brief Set the Author
     *
     * @param author the author
     * @remark This change is observable.
     */
    void setAuthor(const std::string& author);

    /**
     * @brief Get the Date of Creation
     *
     * @return const std::string&
     */
    const std::string& getDateCreation() const;
    /**
     * @brief Set the Date of Creation
     *
     * @param date_creation the date of creation.
     * @remark This change is observable.
     */
    void setDateCreation(const std::string& date_creation);

    /**
     * @brief Get the Date of last Change
     *
     * @return const std::string&
     */
    const std::string& getDateChange() const;
    /**
     * @brief Set the Date of last Change
     *
     * @param date_change the date of last change
     * @remark This change is observable.
     */
    void setDateChange(const std::string& date_change);

    /**
     * @brief Get the Description
     *
     * @return const std::string&
     */
    const std::string& getDescription() const;
    /**
     * @brief Set the Description object
     *
     * @param description the description
     * @remark This change is observable.
     */
    void setDescription(const std::string& description);

    /**
     * @brief Get the Ext Declarations object
     *
     * @return const ExtDeclarations&
     */
    const ExtDeclarations& getExtDeclarations() const;
    /**
     * @brief Get the Ext Declarations object
     *
     * @return ExtDeclarations&
     */
    ExtDeclarations& getExtDeclarations();

private:
    bool validateContains(const ExtDeclarations::access_type& ext_declarations_name) const;
    void notifyChangedMapContent(utility::TypeAccessMapEventCode code,
                                 ExtDeclarations::access_type& ext_declaration,
                                 const std::string& additional_info);

    Version _ddl_version = {};
    std::string _author;
    std::string _date_creation;
    std::string _date_change;
    std::string _description;
    ExtDeclarations _ext_declarations;
};

} // namespace datamodel
} // namespace dd
} // namespace ddl

#endif // DD_DATA_MODEL_HEADER_H_INCLUDED
