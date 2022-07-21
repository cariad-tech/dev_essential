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

#ifndef DD_DD_FROM_XML_FACTORY_H_INCLUDED
#define DD_DD_FROM_XML_FACTORY_H_INCLUDED

#include <ddl/datamodel/datamodel_datadefinition.h>
#include <ddl/dd/dd_error.h>
#include <ddl/dd/dd_predefined_datatypes.h>
#include <ddl/utilities/dd_access_optional.h>
#include <ddl/utilities/std_to_string.h>

#include <list>
#include <regex>
#include <stdexcept>
#include <string>

namespace ddl {
namespace dd {
/**
 * @brief Factory to create a DataDefinition out of a XML based description.
 * This factory uses a template type DOM_NODE_TYPE which is design to follow the a concept class
 * type definition of the a_util::DOMElement.
 *
 * @tparam DOM_NODE_TYPE a DOMElement class must implement following funtions:
 *                       \li string getAttribute(const std::string& name) const;
 *                       \li string getData() const;
 *                       \li bool findNode(const std::string& node_name, DOM_NODE_TYPE& result_node)
 * const; \li bool findNodes(const std::string& node_names, std::list<DOM_NODE_TYPE>& result_nodes)
 * const;
 */
template <typename DOM_NODE_TYPE>
struct DDFromXMLFactory {
    /**
     * check if the given string is an integer value.
     * @return true/false
     */
    static bool isInteger(const std::string& stringToCheck)
    {
        const std::regex numeric_integer_check("^-{0,1}(\\d)+");
        return std::regex_match(stringToCheck, numeric_integer_check);
    } // namespace utility
    /**
     * @brief specialized conversion to the Optional<int32_t>
     *
     * @param from_string string representation of the value
     * @param default_value the optional value to set if not set in \p from_string
     * @return utility::Optional<int>
     */
    static utility::Optional<int> toType(const std::string& from_string,
                                         const utility::Optional<int>& default_value = {})
    {
        try {
            int i_converted = std::stoi(from_string);
            return i_converted;
        }
        catch (const std::invalid_argument&) {
            return default_value;
        }
        catch (const std::exception&) {
            return default_value;
        }
    }

    /**
     * @brief specialized conversion to the Optional<size_t>
     *
     * @param from_string string representation of the value
     * @param default_value the optional value to set if not set in \p from_string
     * @return utility::Optional<size_t>
     */
    static utility::Optional<size_t> toType(const std::string& from_string,
                                            const utility::Optional<size_t>& default_value = {})
    {
        try {
            int size_converted = std::stoi(from_string);
            if (size_converted < 0) {
                // this is if we set i.e. the bytepos to -1 if it is after a dynamic array
                return default_value;
            }
            else {
                return size_converted;
            }
        }
        catch (const std::invalid_argument&) {
            return default_value;
        }
        catch (const std::exception&) {
            return default_value;
        }
    }

    /**
     * @brief specialized function to convert a string to an optional string which is only valid if
     * set.
     *
     * @param from_string the string to convert from
     * @param default_value the optional value to set if \p from_string is empty
     * @return utility::Optional<std::string>
     */
    static utility::Optional<std::string> toType(
        const std::string& from_string, const utility::Optional<std::string>& default_value = {})
    {
        if (from_string.empty()) {
            return default_value;
        }
        else {
            return from_string;
        }
    }

    /**
     * @brief Get the Attribute from the \p dom_node. Return a valid Optional<T> if set, otherwise
     * return \p default_value or throw.
     *
     * @tparam T the value type of the returned Optional
     * @param dom_node the dom node (see also @ref DDFromXMLFactory)
     * @param attribute_name the attribute name
     * @param default_value the default value if the attribute was not set. (this will be only used
     * if \p is_mandatory is false)
     * @param is_mandatory if set to true this function will throw if the attribute is not set in
     * DOM
     * @param parse_error_message the message in the thrown exception if \p is_mandatory was set to
     * true
     * @return utility::Optional<T>
     * @throws dd::Error The function will throw only if \p is_mandatory is set to true.
     */
    template <typename T>
    static utility::Optional<T> getAttribute(const DOM_NODE_TYPE& dom_node,
                                             const char* attribute_name,
                                             const utility::Optional<T>& default_value = {},
                                             bool is_mandatory = false,
                                             const std::string& parse_error_message = {})
    {
        auto value = dom_node.getAttribute(attribute_name);
        if (value.empty()) {
            if (is_mandatory) {
                throw Error(
                    "DDFromXMLFactory::getAttribute", {"...", attribute_name}, parse_error_message);
            }
            return default_value;
        }
        else {
            return toType(value, default_value);
        }
    }

    /**
     * @brief Get the data content of a subnode of \p dom_node. Return a valid Optional<T> if set,
     * otherwise return \p default_value or throw.
     *
     * @tparam T the value type of the returned Optional
     * @param dom_node the dom node (see also @ref DDFromXMLFactory)
     * @param sub_node_name the sub node name
     * @param default_value the default value if the attribute was not set. (this will be only used
     * if \p is_mandatory is false)
     * @param is_mandatory if set to true this function will throw if the data area is not set in
     * sub node
     * @param parse_error_message the message in the thrown exception if \p is_mandatory was set to
     * true
     * @return utility::Optional<T>
     * @throws dd::Error The function will throw only if \p is_mandatory is set to true.
     */
    template <typename T>
    static utility::Optional<T> getData(const DOM_NODE_TYPE& dom_node,
                                        const std::string& sub_node_name,
                                        const utility::Optional<T>& default_value = {},
                                        bool is_mandatory = false,
                                        const char* parse_error_message = "")
    {
        DOM_NODE_TYPE sub_node;
        std::string value = {};
        if (dom_node.findNode(sub_node_name, sub_node)) {
            value = sub_node.getData();
        }
        if (value.empty()) {
            if (is_mandatory) {
                throw Error(
                    "DDFromXMLFactory::getData", {"...", sub_node_name}, parse_error_message);
            }
            return default_value;
        }
        else {
            return toType(value, default_value);
        }
    }

    /**
     * @brief Create a Header Ext Declaration object
     *
     * @param dom_node the node to create the ExDeclaration from (should be "ext_declaration" tag
     * name).
     * @return datamodel::Header::ExtDeclaration
     */
    static datamodel::Header::ExtDeclaration createHeaderExtDeclaration(
        const DOM_NODE_TYPE& dom_node)
    {
        auto key_attrib = getAttribute<std::string>(
            dom_node,
            "key",
            {},
            true,
            "header/ext_declaration tag has no mandatory 'key' attribute");
        auto value_attrib = getAttribute<std::string>(
            dom_node,
            "value",
            {},
            true,
            "header/ext_declaration tag has no mandatory 'value' attribute");
        return datamodel::Header::ExtDeclaration(*key_attrib, *value_attrib);
    }

    /**
     * @brief Create a Header object
     *
     * @param dom_node the node to create the header from (should be "header" tag name).
     * @return datamodel::Header
     */
    static datamodel::Header createHeader(const DOM_NODE_TYPE& dom_node)
    {
        // read header exts
        std::vector<datamodel::Header::ExtDeclaration> exts;
        std::list<DOM_NODE_TYPE> header_ext_nodes;
        if (dom_node.findNodes("ext_declaration", header_ext_nodes)) {
            for (const auto& current_header_ext_node: header_ext_nodes) {
                exts.emplace_back(createHeaderExtDeclaration(current_header_ext_node));
            }
        }
        // read the header itself
        Version version_to_set = {};
        auto file_ddl_version = getData<std::string>(dom_node, "language_version", {});
        if (file_ddl_version) {
            version_to_set = VersionConversion::fromString(*file_ddl_version);
        }
        datamodel::Header created_header(version_to_set, {}, {}, {}, {}, exts);

        auto author = getData<std::string>(dom_node, "author", {});
        if (author) {
            created_header.setAuthor(author);
        }
        auto date_crea = getData<std::string>(dom_node, "date_creation", {});
        if (date_crea) {
            created_header.setDateCreation(date_crea);
        }
        auto date_cha = getData<std::string>(dom_node, "date_change", {});
        if (date_cha) {
            created_header.setDateChange(date_cha);
        }
        auto description = getData<std::string>(dom_node, "description", {});
        if (description) {
            created_header.setDescription(description);
        }
        return created_header;
    }

    /**
     * @brief Create a Base Unit object
     *
     * @param dom_node the node to create the BaseUnit from (should be "baseunit" tag name).
     * @return datamodel::BaseUnit
     */
    static datamodel::BaseUnit createBaseUnit(const DOM_NODE_TYPE& dom_node)
    {
        auto name = getAttribute<std::string>(
            dom_node, "name", {}, true, "baseunit tag has no mandatory 'name' attribute");
        auto symbol = getAttribute<std::string>(dom_node, "symbol", std::string(""));
        auto description = getAttribute<std::string>(dom_node, "description", std::string(""));
        return datamodel::BaseUnit(*name, *symbol, *description);
    }

    /**
     * @brief Create a Unit Prefix object
     *
     * @param dom_node the node to create the UnitPrefix from (should be "prefixes" tag name).
     * @return datamodel::UnitPrefix
     */
    static datamodel::UnitPrefix createUnitPrefix(const DOM_NODE_TYPE& dom_node)
    {
        auto name = getAttribute<std::string>(
            dom_node, "name", {}, true, "prefixes tag has no mandatory 'name' attribute");
        auto symbol = getAttribute<std::string>(
            dom_node, "symbol", {}, true, "prefixes tag has no mandatory 'symbol' attribute");
        auto power = getAttribute<int32_t>(
            dom_node, "power", {}, true, "prefixes tag has no mandatory 'power' attribute");
        return datamodel::UnitPrefix(*name, *symbol, *power);
    }

    /**
     * @brief Create a Ref Unit object
     *
     * @param dom_node the node to create the RefUnit from (should be "refUnit" tag name).
     * @return datamodel::Unit::RefUnit
     */
    static datamodel::Unit::RefUnit createRefUnit(const DOM_NODE_TYPE& dom_node)
    {
        auto unit_name = getAttribute<std::string>(
            dom_node, "name", {}, true, "unit/refUnit tag has no mandatory 'name' attribute");
        auto power = getAttribute<int32_t>(
            dom_node, "power", {}, true, "unit/refUnit tag has no mandatory 'power' attribute");
        auto prefix_name = getAttribute<std::string>(
            dom_node, "prefix", {}, true, "unit/refUnit tag has no mandatory 'prefix' attribute");
        return datamodel::Unit::RefUnit(*unit_name, *power, *prefix_name);
    }

    /**
     * @brief Create a Unit object
     *
     * @param dom_node the node to create the Unit from (should be "unit" tag name).
     * @return datamodel::Unit
     */
    static datamodel::Unit createUnit(const DOM_NODE_TYPE& dom_node)
    {
        // read u exts
        std::vector<datamodel::Unit::RefUnit> ref_units;
        std::list<DOM_NODE_TYPE> ref_unit_nodes;
        if (dom_node.findNodes("refUnit", ref_unit_nodes)) {
            for (const auto& current_ref_unit_node: ref_unit_nodes) {
                ref_units.emplace_back(createRefUnit(current_ref_unit_node));
            }
        }
        auto name = getAttribute<std::string>(
            dom_node, "name", {}, true, "unit tag has no mandatory 'name' attribute");
        auto numerator = getData<std::string>(
            dom_node, "numerator", {}, true, "unit tag has no mandatory 'numerator' tag");
        auto denominator = getData<std::string>(
            dom_node, "denominator", {}, true, "unit tag has no mandatory 'denominator' tag");
        auto offset = getData<std::string>(
            dom_node, "offset", {}, true, "unit tag has no mandatory 'offset' tag");
        return datamodel::Unit(*name, *numerator, *denominator, *offset, ref_units);
    }

    /**
     * @brief Create a Data Type object
     *
     * @param dom_node the node to create the DataType from (should be "datatype" tag name).
     * @param file_ddl_version the set filename language verion (from header)
     * @param strict set to true to load the datamodel exactly like defined (no mixture of DDL tag
     * definitions allowed).
     * @return datamodel::DataType
     */
    static datamodel::DataType createDataType(const DOM_NODE_TYPE& dom_node,
                                              const Version& file_ddl_version,
                                              bool strict)
    {
        // changes between 2.0 and 3.0
        // attribute name changed to "name" from "type"
        std::string name;
        // mandatory
        if (strict && (file_ddl_version >= Version::ddl_version_30)) {
            auto name_attrib = getAttribute<std::string>(
                dom_node, "name", {}, true, "datatype tag has no mandatory 'name' attribute");
            name = *name_attrib;
        }
        else {
            // this must be mandatory, but there are wrong files in the world that uses name instead
            // of type !
            auto name_attrib = getAttribute<std::string>(dom_node, "name", {});
            auto type_attrib = getAttribute<std::string>(dom_node, "type", {});
            if (!type_attrib && !name_attrib) {
                type_attrib = getAttribute<std::string>(
                    dom_node, "type", {}, true, "datatype tag has no mandatory 'type' attribute");
            }
            else {
                if (type_attrib) {
                    name = *type_attrib;
                }
                else {
                    name = *name_attrib;
                }
            }
        }
        auto type_size = getAttribute<size_t>(
            dom_node, "size", {}, true, "datatype tag has no mandatory 'size' attribute");
        // create the type
        datamodel::DataType created_type(
            name,
            type_size,
            {},
            {},
            {},
            {},
            {},
            ddl::PredefinedDataTypes::getInstance().getDefaultAlignment(name));

        // optional
        auto description = getAttribute<std::string>(dom_node, "description");
        if (description) {
            created_type.setDescription(*description);
        }
        auto array_size = getAttribute<size_t>(dom_node, "arraysize");
        if (array_size) {
            created_type.setArraySize(*array_size);
        }
        auto unit = getAttribute<std::string>(dom_node, "unit");
        if (unit) {
            created_type.setUnitName(unit);
        }
        // min / max introduced in DataDefinition 3.0
        if (file_ddl_version >= Version::ddl_version_30 ||
            file_ddl_version == Version::ddl_version_notset) {
            auto min_val = getAttribute<std::string>(dom_node, "min");
            if (min_val) {
                created_type.setMin(*min_val);
            }
            auto max_val = getAttribute<std::string>(dom_node, "max");
            if (max_val) {
                created_type.setMax(*max_val);
            }
        }
        return created_type;
    }

    /**
     * @brief Create a Enum Type Element object
     *
     * @param dom_node the node to create the EnumType::Element from (should be "element" tag name).
     * @return datamodel::EnumType::Element
     */
    static datamodel::EnumType::Element createEnumTypeElement(const DOM_NODE_TYPE& dom_node)
    {
        auto name_attrib = getAttribute<std::string>(
            dom_node, "name", {}, true, "enum/element tag has no mandatory 'name' attribute");
        auto value_attrib = getAttribute<std::string>(
            dom_node, "value", {}, true, "enum/element tag has no mandatory 'value' attribute");
        return datamodel::EnumType::Element(*name_attrib, *value_attrib);
    }

    /**
     * @brief Create a Enum Type object
     *
     * @param dom_node the node to create the EnumType from (should be "enum" tag name).
     * @return datamodel::EnumType
     */
    static datamodel::EnumType createEnumType(const DOM_NODE_TYPE& dom_node)
    {
        auto name_attrib = getAttribute<std::string>(
            dom_node, "name", {}, true, "enum tag has no mandatory 'name' attribute");
        auto type_attrib = getAttribute<std::string>(
            dom_node, "type", {}, true, "type tag has no mandatory 'type' attribute");

        datamodel::EnumType created_enum(*name_attrib, *type_attrib);
        std::list<DOM_NODE_TYPE> enum_element_nodes;
        if (dom_node.findNodes("element", enum_element_nodes)) {
            for (const auto& current_enum_element_node: enum_element_nodes) {
                created_enum.getElements().emplace(
                    createEnumTypeElement(current_enum_element_node));
            }
        }
        return created_enum;
    }

    /**
     * @brief Create a Struct Type Element object
     *
     * @param dom_node the node to create the StructType::Element from (should be "element" tag
     * name).
     * @param file_ddl_version the set filename language verion (from header)
     * @param strict set to true to load the datamodel exactly like defined (no mixture of DDL tag
     * definitions allowed).
     * @return datamodel::StructType::Element
     */
    static datamodel::StructType::Element createStructTypeElement(const DOM_NODE_TYPE& dom_node,
                                                                  const Version& file_ddl_version,
                                                                  bool strict)
    {
        auto name_attrib = getAttribute<std::string>(
            dom_node, "name", {}, true, "struct/element tag has no mandatory 'name' attribute");
        auto type_attrib = getAttribute<std::string>(dom_node,
                                                     "type",
                                                     {},
                                                     true,
                                                     "struct/element tag '" + *name_attrib +
                                                         "' has no mandatory 'type' attribute");

        utility::Optional<size_t> byte_pos;
        ByteOrder byte_order(ByteOrder::e_le);
        utility::Optional<size_t> bit_pos;
        utility::Optional<size_t> num_bits;
        Alignment alignment_to_set = Alignment::e0;

        Version used_file_version = file_ddl_version;
        if (file_ddl_version == Version::ddl_version_notset || !strict) {
            // we need to figure out the version because it i.e. a string parsing where no header is
            // set
            DOM_NODE_TYPE serialized_node;
            if (dom_node.findNode("serialized", serialized_node)) {
                used_file_version = Version::ddl_version_40;
            }
            else {
                // this is a guess!
                used_file_version = Version::ddl_version_30;
            }
        }

        // From version 4.0 on this information is specified within the <serialized> tag.
        // From version 4.0 on this information is specified within the <deserialized> tag.
        if (used_file_version >= Version::ddl_version_40) {
            DOM_NODE_TYPE serialized_node;
            if (dom_node.findNode("serialized", serialized_node)) {
                // mandatory serialized info
                auto byte_pos_int = getAttribute<int>(serialized_node,
                                                      "bytepos",
                                                      {},
                                                      true,
                                                      "struct/element tag '" + *name_attrib +
                                                          "' has no mandatory 'bytepos' attribute");
                if (*byte_pos_int == -1) {
                    byte_pos = {};
                }
                else {
                    byte_pos = static_cast<size_t>(*byte_pos_int);
                }
                auto byte_order_attrib =
                    getAttribute<std::string>(serialized_node,
                                              "byteorder",
                                              {},
                                              true,
                                              "struct/element/serialized tag '" + *name_attrib +
                                                  "' no mandatory 'byte_order' attribute");
                byte_order = ByteOrderConversion::fromString(*byte_order_attrib, byte_order);

                bit_pos = getAttribute<size_t>(serialized_node, "bitpos", {});
                num_bits = getAttribute<size_t>(serialized_node, "numbits", {});
            }
            else {
                throw Error("DDFromXMLFactory::getAttribute",
                            "struct/element/deserialized tag was not found");
            }

            DOM_NODE_TYPE deserialized_node;
            if (dom_node.findNode("deserialized", deserialized_node)) {
                auto alignment_attrib =
                    getAttribute<std::string>(deserialized_node,
                                              "alignment",
                                              {},
                                              true,
                                              "struct/element/deserialized tag '" + *name_attrib +
                                                  "' has no mandatory 'alignment' attribute");
                alignment_to_set =
                    AlignmentConversion::fromString(*alignment_attrib, alignment_to_set);
            }
            else {
                throw Error("DDFromXMLFactory::getAttribute",
                            "struct/element/serialized tag was not found");
            }
        }
        else {
            // mandatory serialized info
            auto byte_pos_int = getAttribute<int>(dom_node,
                                                  "bytepos",
                                                  {},
                                                  true,
                                                  "struct/element tag '" + *name_attrib +
                                                      "' has no mandatory 'bytepos' attribute");
            if (*byte_pos_int == -1) {
                byte_pos = {};
            }
            else {
                byte_pos = static_cast<size_t>(*byte_pos_int);
            }
            auto byte_order_attrib =
                getAttribute<std::string>(dom_node,
                                          "byteorder",
                                          {},
                                          true,
                                          "struct/element tag '" + *name_attrib +
                                              "' has no mandatory 'byte_order' attribute");
            byte_order = ByteOrderConversion::fromString(*byte_order_attrib, byte_order);

            bit_pos = getAttribute<size_t>(dom_node, "bitpos", {});
            num_bits = getAttribute<size_t>(dom_node, "numbits", {});

            // mandatory deserialized info
            auto alignment_attrib = getAttribute<std::string>(
                dom_node,
                "alignment",
                {},
                true,
                "struct/element tag '" + *name_attrib + "' has no mandatory 'alignment' attribute");
            alignment_to_set = AlignmentConversion::fromString(*alignment_attrib, alignment_to_set);
        }

        datamodel::StructType::Element created_element(
            *name_attrib,
            *type_attrib,
            datamodel::StructType::DeserializedInfo(alignment_to_set),
            datamodel::StructType::SerializedInfo(byte_pos, byte_order, bit_pos, num_bits));

        // optional
        auto description = getAttribute<std::string>(dom_node, "description");
        if (description) {
            created_element.setDescription(*description);
        }
        auto unit_name = getAttribute<std::string>(dom_node, "unit");
        if (unit_name) {
            created_element.setUnitName(*unit_name);
        }
        auto comment = getAttribute<std::string>(dom_node, "comment");
        if (comment) {
            created_element.setComment(*comment);
        }
        auto arraysize_attrib =
            getAttribute<std::string>(dom_node, "arraysize"); // introduced in DataDefinition 2.0
        dd::OptionalSize arraysize_value = {};
        // we check for an integrer to prevent an excpetion doin this for us
        if (arraysize_attrib && isInteger(*arraysize_attrib)) {
            arraysize_value = getAttribute<size_t>(dom_node, "arraysize");
        }
        if (arraysize_value) // the array size is set to a numeric value for size
        {
            created_element.setArraySize(*arraysize_value);
        }
        else if (arraysize_attrib) // array size is set to a string (or an invalid value) and
                                   // introduced in DataDefinition 2.0 //Dynamic array support
        {
            if (file_ddl_version >= Version::ddl_version_20 ||
                file_ddl_version == Version::ddl_version_notset || !strict) {
                created_element.setArraySize(*arraysize_attrib);
            }
        }
        // introduced in DataDefinition 2.0
        if (used_file_version >= Version::ddl_version_20 ||
            file_ddl_version == Version::ddl_version_notset || !strict) {
            auto value_attrib = getAttribute<std::string>(dom_node, "value");
            if (value_attrib) {
                created_element.setValue(*value_attrib);
            }
        }
        // introduced in DataDefinition 3.0
        if (used_file_version >= Version::ddl_version_30 ||
            file_ddl_version == Version::ddl_version_notset || !strict) {
            auto min_attrib = getAttribute<std::string>(dom_node, "min");
            if (min_attrib) {
                created_element.setMin(*min_attrib);
            }
            auto max_attrib = getAttribute<std::string>(dom_node, "max");
            if (max_attrib) {
                created_element.setMax(*max_attrib);
            }
            auto default_attrib = getAttribute<std::string>(dom_node, "default");
            if (default_attrib) {
                created_element.setDefault(*default_attrib);
            }
            auto scale_attrib = getAttribute<std::string>(dom_node, "scale");
            if (scale_attrib) {
                created_element.setScale(*scale_attrib);
            }
            auto offset_attrib = getAttribute<std::string>(dom_node, "offset");
            if (offset_attrib) {
                created_element.setOffset(*offset_attrib);
            }
        }

        return created_element;
    }

    /**
     * @brief Create a Struct Type object
     *
     * @param dom_node node to create the StructType from (should be "struct" tag name).
     * @param file_ddl_version the set filename language verion (from header)
     * @param strict set to true to load the datamodel exactly like defined (no mixture of DDL tag
     * definitions allowed).
     * @return datamodel::StructType
     */
    static datamodel::StructType createStructType(const DOM_NODE_TYPE& dom_node,
                                                  const Version& file_ddl_version,
                                                  bool strict)
    {
        auto name_attrib = getAttribute<std::string>(dom_node, "name", {});
        if (!name_attrib) {
            // this is very old compatibility where structs where named with type attribute
            auto type_attrib = getAttribute<std::string>(dom_node, "type", {});
            if (type_attrib) {
                name_attrib = type_attrib;
            }
            else {
                // this is to set error
                name_attrib = getAttribute<std::string>(
                    dom_node, "name", {}, true, "struct tag has no mandatory 'name' attribute");
            }
        }
        // usually the version is mandatory, but the regression test says, there are description,
        // where it is not set :-(
        auto version_attrib = getAttribute<std::string>(dom_node, "version", {});
        if (!version_attrib) {
            version_attrib = "1";
        }

        auto alignment_attrib = getAttribute<std::string>(dom_node, "alignment", {});
        OptionalSize alignment_to_set;
        if (alignment_attrib) {
            alignment_to_set = AlignmentConversion::fromString(*alignment_attrib);
        }
        auto comment_attrib = getAttribute<std::string>(dom_node, "comment", {});
        std::string comment_to_set = "";
        if (comment_attrib) {
            comment_to_set = *comment_attrib;
        }
        auto ddl_version_attrib = getAttribute<std::string>(dom_node, "ddlversion", {});
        auto ddl_version_to_set = Version::ddl_version_notset;
        if (ddl_version_attrib) {
            ddl_version_to_set = VersionConversion::fromString(*ddl_version_attrib);
        }

        datamodel::StructType struct_type(
            *name_attrib, *version_attrib, alignment_to_set, comment_to_set, ddl_version_to_set);
        // read elements
        std::list<DOM_NODE_TYPE> struct_element_nodes;
        if (dom_node.findNodes("element", struct_element_nodes)) {
            for (const auto& current_struct_element_node: struct_element_nodes) {
                struct_type.getElements().emplace(
                    createStructTypeElement(current_struct_element_node, file_ddl_version, strict));
            }
        }
        return struct_type;
    }

    /**
     * @brief Create a Stream Meta Type Property object
     *
     * @param dom_node node to create the StreamMetaType::Property from (should be "property" tag
     * name).
     * @return datamodel::StreamMetaType::Property
     */
    static datamodel::StreamMetaType::Property createStreamMetaTypeProperty(
        const DOM_NODE_TYPE& dom_node)
    {
        auto name_attrib = getAttribute<std::string>(
            dom_node,
            "name",
            {},
            true,
            "streammetatypename/property tag has no mandatory 'name' attribute");
        auto type_attrib = getAttribute<std::string>(
            dom_node,
            "type",
            {},
            true,
            "streammetatypename/property tag has no mandatory 'type' attribute");
        return datamodel::StreamMetaType::Property(*name_attrib, *type_attrib);
    }

    /**
     * @brief Create a Stream Meta Type object
     *
     * @param dom_node node to create the StreamMetaType from (should be "streammetatype" tag name).
     * @return datamodel::StreamMetaType
     */
    static datamodel::StreamMetaType createStreamMetaType(const DOM_NODE_TYPE& dom_node)
    {
        auto name_attrib = getAttribute<std::string>(
            dom_node, "name", {}, true, "streammetatype tag has no mandatory 'name' attribute");
        auto version_attrib =
            getAttribute<std::string>(dom_node,
                                      "version",
                                      {},
                                      true,
                                      "streammetatype tag has no mandatory 'version' attribute");

        datamodel::StreamMetaType created_type =
            datamodel::StreamMetaType(*name_attrib, *version_attrib, {});

        auto parent_attrib = getAttribute<std::string>(dom_node, "parent", {});
        if (parent_attrib) {
            created_type.setParent(*parent_attrib);
        }
        // read properties
        std::list<DOM_NODE_TYPE> smt_prop_nodes;
        if (dom_node.findNodes("property", smt_prop_nodes)) {
            for (const auto& current_smt_prop_node: smt_prop_nodes) {
                created_type.getProperties().emplace(
                    createStreamMetaTypeProperty(current_smt_prop_node));
            }
        }
        return created_type;
    }

    /**
     * @brief Create a Stream Struct object
     *
     * @param dom_node node to create the Stream::Struct from (should be "struct" tag name).
     * @return datamodel::Stream::Struct
     */
    static datamodel::Stream::Struct createStreamStruct(const DOM_NODE_TYPE& dom_node)
    {
        auto name_attrib = getAttribute<std::string>(dom_node, "name", {});
        auto type_attrib = getAttribute<std::string>(
            dom_node, "type", {}, true, "stream/struct tag has no mandatory 'type' attribute");
        auto byte_pos = getAttribute<size_t>(dom_node, "bytepos", {});
        return datamodel::Stream::Struct(*name_attrib, *type_attrib, byte_pos);
    }

    /**
     * @brief Create a Stream object
     *
     * @param dom_node node to create the Stream from (should be "stream" tag name).
     * @return datamodel::Stream
     */
    static datamodel::Stream createStream(const DOM_NODE_TYPE& dom_node)
    {
        auto name_attrib = getAttribute<std::string>(dom_node, "name", {});
        auto type_attrib = getAttribute<std::string>(
            dom_node, "type", {}, true, "stream tag has no mandatory 'type' attribute");
        // we need to "try" because, there are stream definitions "in the world" where only "type"
        // was used (also as name)!
        if (!name_attrib) {
            // we use type as name
            name_attrib = *type_attrib;
        }

        datamodel::Stream created_stream = datamodel::Stream(*name_attrib, *type_attrib);

        auto desc_attrib = getAttribute<std::string>(dom_node, "description", {});
        if (desc_attrib) {
            created_stream.setDescription(*desc_attrib);
        }
        // read the structs
        std::list<DOM_NODE_TYPE> ss_nodes;
        if (dom_node.findNodes("struct", ss_nodes)) {
            for (const auto& current_stream_struct_node: ss_nodes) {
                created_stream.getStructs().emplace(createStreamStruct(current_stream_struct_node));
            }
        }
        return created_stream;
    }

    /**
     * @brief Creates a whole DataDefinition and read all sub node
     *
     * @param dom_node node to create the DataDefinition from.
     * @param language_version forced language version is no header-tag is found within the dom
     * definition
     * @param strict set to true to load the datamodel exactly like defined (no mixture of DDL tag
     * definitions allowed).
     * @return datamodel::DataDefinition
     */
    static datamodel::DataDefinition createDD(const DOM_NODE_TYPE& dom_node,
                                              const dd::Version& language_version,
                                              bool strict)
    {
        // we use the version given, but if header exists we use the headers version
        datamodel::DataDefinition new_ddl(language_version);
        DOM_NODE_TYPE header_element;
        if (dom_node.findNode("header", header_element)) {
            // read header
            new_ddl.setHeader(createHeader(header_element));
        }
        // read BaseUnits
        std::list<DOM_NODE_TYPE> baseunit_nodes;
        if (dom_node.findNodes("//units/baseunit", baseunit_nodes)) {
            for (const auto& current_base_unit_node: baseunit_nodes) {
                new_ddl.getBaseUnits().emplace(createBaseUnit(current_base_unit_node));
            }
        }
        // read UnitPrefix
        std::list<DOM_NODE_TYPE> prefix_nodes;
        if (dom_node.findNodes("//units/prefixes", prefix_nodes)) {
            for (const auto& current_prefix_node: prefix_nodes) {
                new_ddl.getUnitPrefixes().emplace(createUnitPrefix(current_prefix_node));
            }
        }
        // read Unit
        std::list<DOM_NODE_TYPE> unit_nodes;
        if (dom_node.findNodes("//units/unit", unit_nodes)) {
            for (const auto& current_unit_node: unit_nodes) {
                new_ddl.getUnits().emplace(
                    dd::DDFromXMLFactory<DOM_NODE_TYPE>::createUnit(current_unit_node));
            }
        }
        // read datatypes
        std::list<DOM_NODE_TYPE> datatype_nodes;
        if (dom_node.findNodes("//datatypes/datatype", datatype_nodes)) {
            for (const auto& current_datatype_node: datatype_nodes) {
                new_ddl.getDataTypes().emplace(
                    createDataType(current_datatype_node, new_ddl.getVersion(), strict));
            }
        }
        // read enum types
        std::list<DOM_NODE_TYPE> enum_nodes;
        if (dom_node.findNodes("//enums/enum", enum_nodes)) {
            for (const auto& current_enum_node: enum_nodes) {
                new_ddl.getEnumTypes().emplace(
                    dd::DDFromXMLFactory<DOM_NODE_TYPE>::createEnumType(current_enum_node));
            }
        }
        // read struct types
        std::list<DOM_NODE_TYPE> struct_nodes;
        if (dom_node.findNodes("//structs/struct", struct_nodes)) {
            for (const auto& current_struct_node: struct_nodes) {
                new_ddl.getStructTypes().emplace(
                    createStructType(current_struct_node, new_ddl.getVersion(), strict));
            }
        }
        // read struct types very special (only <struct></struct> is possible)
        if (dom_node.getName() == "struct") {
            new_ddl.getStructTypes().emplace(
                createStructType(dom_node, new_ddl.getVersion(), strict));
        }
        // read streammetatypes
        if ((new_ddl.getVersion() >= dd::Version::ddl_version_40) || !strict) {
            std::list<DOM_NODE_TYPE> stream_meta_type_nodes;
            if (dom_node.findNodes("//streammetatypes/streammetatype", stream_meta_type_nodes)) {
                for (const auto& current_smt_node: stream_meta_type_nodes) {
                    new_ddl.getStreamMetaTypes().emplace(createStreamMetaType(current_smt_node));
                }
            }
        }

        std::list<DOM_NODE_TYPE> stream_nodes;
        if (dom_node.findNodes("//streams/stream", stream_nodes)) {
            for (const auto& current_stream_node: stream_nodes) {
                new_ddl.getStreams().emplace(createStream(current_stream_node));
            }
        }
        return new_ddl;
    }
};

} // namespace dd
} // namespace ddl

#endif // DD_FROM_XML_FACTORY_H_INCLUDED
