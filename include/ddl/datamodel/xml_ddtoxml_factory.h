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

#ifndef DD_DD_TO_XML_FACTORY_H_INCLUDED
#define DD_DD_TO_XML_FACTORY_H_INCLUDED

#include <ddl/datamodel/datamodel_datadefinition.h>
#include <ddl/dd/dd_error.h>
#include <ddl/utilities/dd_access_optional.h>
#include <ddl/utilities/std_to_string.h>

#include <stdexcept>
#include <string>

namespace ddl {
namespace dd {

/**
 * @brief Template class to create DD DOM nodes with the help of the type DOM_NODE_TYPE.
 *
 * @tparam DOM_NODE_TYPE The type for the DOM Node which must follow the concept of implementing
 * functions: \li setAttribute(const std::string& name, const std::string& value); \li
 * DOM_NODE_TYPE& createChild(const std::string& name); \li setData(const std::string& name, const
 * std::string& data);
 */
template <typename DOM_NODE_TYPE>
struct DDToXMLFactory {
    /**
     * @brief Set the Optional Attribute. This will only set the attribute if the value is valid.
     *
     * @tparam T The value_type of the optional
     * @param dom_node the dom node where to set the attribute
     * @param attribute_name the attribute name
     * @param value the value of the attribute
     * @return true the value was set
     * @return false the value was not set
     */
    template <typename T>
    static bool setOptionalAttribute(DOM_NODE_TYPE& dom_node,
                                     const char* attribute_name,
                                     const utility::Optional<T>& value)
    {
        if (value) {
            dom_node.setAttribute(attribute_name, std::to_string(*value));
            return true;
        }
        return false;
    }

    /**
     * @brief Set the Optional Attribute. This will only set the attribute if the value is not
     * empty.
     *
     * @param dom_node the dom node where to set the attribute
     * @param attribute_name the attribute name
     * @param value the value of the attribute
     * @return true the value was set
     * @return false the value was not set
     */
    static bool setOptionalAttribute(DOM_NODE_TYPE& dom_node,
                                     const char* attribute_name,
                                     const std::string& value)
    {
        if (!value.empty()) {
            dom_node.setAttribute(attribute_name, value);
            return true;
        }
        return false;
    }

    /**
     * @brief Set the data of the new created tag with the name \p sub_node_name.
     *
     * @param dom_node the dom node where to set the attribute
     * @param sub_node_name the subnode tag name
     * @param value the data content
     */
    static void setData(DOM_NODE_TYPE& dom_node,
                        const std::string& sub_node_name,
                        const std::string& value)
    {
        DOM_NODE_TYPE sub_node = dom_node.createChild(sub_node_name);
        sub_node.setData(value);
    }

    /**
     * @brief Create a Node for the header.
     *
     * @param parent_node the parent node where to create the node
     * @param header the datamodel of the header
     */
    static void createNode(DOM_NODE_TYPE& parent_node, const datamodel::Header& header)
    {
        DOM_NODE_TYPE sub_node = parent_node.createChild("header");
        setData(
            sub_node, "language_version", VersionConversion::toString(header.getLanguageVersion()));
        setData(sub_node, "author", header.getAuthor());
        setData(sub_node, "date_creation", header.getDateCreation());
        setData(sub_node, "date_change", header.getDateChange());
        setData(sub_node, "description", header.getDescription());
        const auto& declarations = header.getExtDeclarations();
        for (auto ext = declarations.cbegin(); ext != declarations.cend(); ++ext) {
            DOM_NODE_TYPE sub_ext_node = sub_node.createChild("ext_declaration");
            sub_ext_node.setAttribute("key", ext->second->getKey());
            sub_ext_node.setAttribute("value", ext->second->getValue());
        }
    }

    /**
     * @brief Create a Node for the base_unit.
     *
     * @param parent_node the parent node where to create the node
     * @param base_unit the datamodel of the base_unit
     */
    static void createNode(DOM_NODE_TYPE& parent_node, const datamodel::BaseUnit& base_unit)
    {
        DOM_NODE_TYPE sub_node = parent_node.createChild("baseunit");
        sub_node.setAttribute("name", base_unit.getName());
        sub_node.setAttribute("symbol", base_unit.getSymbol());
        sub_node.setAttribute("description", base_unit.getDescription());
    }

    /**
     * @brief Create a Node for the unit_prefix.
     *
     * @param parent_node the parent node where to create the node
     * @param unit_prefix the datamodel of the unit_prefix
     */
    static void createNode(DOM_NODE_TYPE& parent_node, const datamodel::UnitPrefix& unit_prefix)
    {
        DOM_NODE_TYPE sub_node = parent_node.createChild("prefixes");
        sub_node.setAttribute("name", unit_prefix.getName());
        auto power = std::to_string(unit_prefix.getPower());
        sub_node.setAttribute("power", power);
        sub_node.setAttribute("symbol", unit_prefix.getSymbol());
    }

    /**
     * @brief Create a Node for the unit.
     *
     * @param parent_node the parent node where to create the node
     * @param unit the datamodel of the unit
     */
    static void createNode(DOM_NODE_TYPE& parent_node, const datamodel::Unit& unit)
    {
        DOM_NODE_TYPE sub_node = parent_node.createChild("unit");
        sub_node.setAttribute("name", unit.getName());
        setData(sub_node, "numerator", unit.getNumerator());
        setData(sub_node, "denominator", unit.getDenominator());
        setData(sub_node, "offset", unit.getOffset());
        const auto& ref_units = unit.getRefUnits();
        for (auto ref_unit = ref_units.cbegin(); ref_unit != ref_units.cend(); ++ref_unit) {
            DOM_NODE_TYPE sub_node_elem = sub_node.createChild("refUnit");
            sub_node_elem.setAttribute("name", (*ref_unit).getUnitName());
            sub_node_elem.setAttribute("power", std::to_string((*ref_unit).getPower()));
            sub_node_elem.setAttribute("prefix", (*ref_unit).getPrefixName());
        }
    }

    /**
     * @brief Create a Node for the data_type.
     *
     * @param parent_node the parent node where to create the node
     * @param data_type the datamodel of the unit
     * @param file_ddl_version the file version (this influence some attribute names and content)
     */
    static void createNode(DOM_NODE_TYPE& parent_node,
                           const datamodel::DataType& data_type,
                           const Version& file_ddl_version)
    {
        DOM_NODE_TYPE sub_node = parent_node.createChild("datatype");
        // changes between 2.0 and 3.0
        // attribute name changed to "name" from "type"
        // mandatory
        if (file_ddl_version >= Version::ddl_version_30) {
            sub_node.setAttribute("name", data_type.getName());
        }
        else {
            sub_node.setAttribute("type", data_type.getName());
        }
        sub_node.setAttribute("size", std::to_string(data_type.getBitSize()));
        // optional
        setOptionalAttribute(sub_node, "description", data_type.getDescription());
        setOptionalAttribute<size_t>(sub_node, "arraysize", data_type.getArraySize());
        setOptionalAttribute(sub_node, "unit", data_type.getUnitName());
        // min / max introduced in DataDefinition 3.0
        if (file_ddl_version >= Version::ddl_version_30) {
            setOptionalAttribute(sub_node, "min", data_type.getMin());
            setOptionalAttribute(sub_node, "max", data_type.getMax());
        }
    }

    /**
     * @brief Create a Node for the enum_type.
     *
     * @param parent_node the parent node where to create the node
     * @param enum_type the datamodel of enum_type
     */
    static void createNode(DOM_NODE_TYPE& parent_node, const datamodel::EnumType& enum_type)
    {
        DOM_NODE_TYPE sub_node = parent_node.createChild("enum");
        sub_node.setAttribute("name", enum_type.getName());
        sub_node.setAttribute("type", enum_type.getDataTypeName());
        const auto& elems = enum_type.getElements();
        for (auto elem = elems.cbegin(); elem != elems.cend(); ++elem) {
            DOM_NODE_TYPE sub_node_elem = sub_node.createChild("element");
            sub_node_elem.setAttribute("name", elem->second->getName());
            sub_node_elem.setAttribute("value", elem->second->getValue());
        }
    }

    /**
     * @brief Create a Node for the struct_type element.
     *
     * @param parent_node the parent node where to create the node
     * @param element the datamodel of element
     * @param file_ddl_version the file version (this influence some attribute names and content)
     */
    static void createNode(DOM_NODE_TYPE& parent_node,
                           const datamodel::StructType::Element& element,
                           const Version& file_ddl_version)
    {
        DOM_NODE_TYPE sub_node = parent_node.createChild("element");
        sub_node.setAttribute("name", element.getName());
        sub_node.setAttribute("type", element.getTypeName());

        // From version 4.0 on this information is specified within the <deserialized> tag.
        DOM_NODE_TYPE* serialized_node = &sub_node;
        DOM_NODE_TYPE serialized_node_version_40;
        if (file_ddl_version >= Version::ddl_version_40) {
            serialized_node_version_40 = sub_node.createChild("serialized");
            serialized_node = &serialized_node_version_40;
        }
        serialized_node->setAttribute("bytepos", std::to_string(*element.getBytePos()));
        serialized_node->setAttribute("byteorder",
                                      dd::ByteOrderConversion::toString(element.getByteOrder()));
        setOptionalAttribute<size_t>(*serialized_node, "bitpos", element.getBitPos());
        setOptionalAttribute<size_t>(*serialized_node, "numbits", element.getNumBits());
        // From version 4.0 on this information is specified within the <serialized> tag.
        DOM_NODE_TYPE* deserialized_node = &sub_node;
        DOM_NODE_TYPE deserialized_node_version_40;
        if (file_ddl_version >= Version::ddl_version_40) {
            deserialized_node_version_40 = sub_node.createChild("deserialized");
            deserialized_node = &deserialized_node_version_40;
        }
        deserialized_node->setAttribute("alignment", std::to_string(element.getAlignment()));
        // optional
        setOptionalAttribute(sub_node, "description", element.getDescription());
        setOptionalAttribute(sub_node, "unit", element.getUnitName());
        setOptionalAttribute(sub_node, "comment", element.getComment());

        // in DataDefinition 2.0 dynamic arrays are introduced
        if (file_ddl_version >= Version::ddl_version_20 &&
            element.getArraySize().isDynamicArraySize()) {
            setOptionalAttribute(
                sub_node, "arraysize", element.getArraySize().getArraySizeElementName());
        }
        else {
            auto array_size = element.getArraySize().getArraySizeValue();
            if (array_size == 0) {
                array_size = 1;
            }
            sub_node.setAttribute("arraysize", std::to_string(array_size));
        }
        // introduced in DataDefinition 2.0
        if (file_ddl_version >= Version::ddl_version_20) {
            setOptionalAttribute(sub_node, "value", element.getValue());
        }
        // introduced in DataDefinition 3.0
        if (file_ddl_version >= Version::ddl_version_30) {
            setOptionalAttribute(sub_node, "min", element.getMin());
            setOptionalAttribute(sub_node, "max", element.getMax());
            setOptionalAttribute(sub_node, "default", element.getDefault());
            setOptionalAttribute(sub_node, "scale", element.getScale());
            setOptionalAttribute(sub_node, "offset", element.getOffset());
        }
    }
    /**
     * @brief Create a Node for the struct_type.
     *
     * @param parent_node the parent node where to create the node
     * @param struct_type the datamodel of struct_type
     * @param file_ddl_version the file version (this influence some attribute names and content)
     */
    static void createNode(DOM_NODE_TYPE& parent_node,
                           const datamodel::StructType& struct_type,
                           const Version& file_ddl_version)
    {
        DOM_NODE_TYPE sub_node = parent_node.createChild("struct");
        sub_node.setAttribute("name", struct_type.getName());
        sub_node.setAttribute("version", struct_type.getVersion());

        setOptionalAttribute<size_t>(sub_node, "alignment", struct_type.getAlignment());
        setOptionalAttribute(sub_node, "comment", struct_type.getComment());
        if (struct_type.getLanguageVersion() != Version::ddl_version_notset) {
            sub_node.setAttribute("ddlversion",
                                  VersionConversion::toString(struct_type.getLanguageVersion()));
        }
        const auto& elements = struct_type.getElements();
        for (auto elem = elements.cbegin(); elem != elements.cend(); ++elem) {
            auto current_elem = *elem;
            createNode(sub_node, *current_elem, file_ddl_version);
        }
    }
    /**
     * @brief Create a Node for the stream_meta_type.
     *
     * @param parent_node the parent node where to create the node
     * @param stream_meta_type the datamodel of stream_meta_type
     */
    static void createNode(DOM_NODE_TYPE& parent_node,
                           const datamodel::StreamMetaType& stream_meta_type)
    {
        DOM_NODE_TYPE sub_node = parent_node.createChild("streammetatype");
        sub_node.setAttribute("name", stream_meta_type.getName());
        sub_node.setAttribute("version", stream_meta_type.getVersion());
        setOptionalAttribute(sub_node, "parent", stream_meta_type.getParent());

        const auto& props = stream_meta_type.getProperties();
        for (auto prop = props.cbegin(); prop != props.cend(); ++prop) {
            DOM_NODE_TYPE sub_prop_node = sub_node.createChild("property");
            sub_prop_node.setAttribute("name", prop->second->getName());
            sub_prop_node.setAttribute("type", prop->second->getType());
        }
    }

    /**
     * @brief Create a Node for the stream.
     *
     * @param parent_node the parent node where to create the node
     * @param stream the datamodel of stream
     */
    static void createNode(DOM_NODE_TYPE& parent_node, const datamodel::Stream& stream)
    {
        DOM_NODE_TYPE sub_node = parent_node.createChild("stream");
        sub_node.setAttribute("name", stream.getName());
        sub_node.setAttribute("type", stream.getStreamTypeName());
        setOptionalAttribute(sub_node, "description", stream.getDescription());

        const auto& stream_structs = stream.getStructs();
        for (auto stream_struct = stream_structs.cbegin(); stream_struct != stream_structs.cend();
             ++stream_struct) {
            DOM_NODE_TYPE sub_struct_node = sub_node.createChild("struct");
            setOptionalAttribute(sub_struct_node, "name", (*stream_struct)->getName());
            sub_struct_node.setAttribute("type", (*stream_struct)->getTypeName());
            auto byte_pos = (*stream_struct)->getBytePos();
            if (!byte_pos) {
                byte_pos = 0;
            }
            sub_struct_node.setAttribute("bytepos", std::to_string(*byte_pos));
        }
    }

    /**
     * @brief Create a Node for the DD.
     *
     * @param parent_node the parent node where to create the node
     * @param dd the datamodel of DD
     */
    static void createNode(DOM_NODE_TYPE& parent_node, const datamodel::DataDefinition& dd)
    {
        createNode(parent_node, *(dd.getHeader()));

        DOM_NODE_TYPE units_node = parent_node.createChild("units");
        // write base unit types
        const auto& base_units = dd.getBaseUnits();
        for (auto base_unit = base_units.cbegin(); base_unit != base_units.cend(); ++base_unit) {
            createNode(units_node, *(base_unit->second));
        }
        // write unit prefix types
        const auto& unit_prefixes = dd.getUnitPrefixes();
        for (auto unit_prefix = unit_prefixes.cbegin(); unit_prefix != unit_prefixes.cend();
             ++unit_prefix) {
            createNode(units_node, *(unit_prefix->second));
        }
        // write unit types
        const auto& units = dd.getUnits();
        for (auto unit = units.cbegin(); unit != units.cend(); ++unit) {
            createNode(units_node, *(unit->second));
        }

        // write data types
        DOM_NODE_TYPE data_types_node = parent_node.createChild("datatypes");
        const auto& data_types = dd.getDataTypes();
        for (auto data_type = data_types.cbegin(); data_type != data_types.cend(); ++data_type) {
            createNode(data_types_node, *(data_type->second), dd.getVersion());
        }
        // write enum types
        DOM_NODE_TYPE enum_types_node = parent_node.createChild("enums");
        const auto& enum_types = dd.getEnumTypes();
        for (auto enum_type = enum_types.cbegin(); enum_type != enum_types.cend(); ++enum_type) {
            createNode(enum_types_node, *(enum_type->second));
        }
        // write struct types
        DOM_NODE_TYPE struct_types_node = parent_node.createChild("structs");
        const auto& struct_types = dd.getStructTypes();
        for (auto struct_type = struct_types.cbegin(); struct_type != struct_types.cend();
             ++struct_type) {
            createNode(struct_types_node, *(struct_type->second), dd.getVersion());
        }

        // write stream meta types types
        if (dd.getVersion() >= dd::Version::ddl_version_40) {
            DOM_NODE_TYPE stream_meta_types_node = parent_node.createChild("streammetatypes");
            const auto& stream_meta_types_types = dd.getStreamMetaTypes();
            for (auto stream_meta_type = stream_meta_types_types.cbegin();
                 stream_meta_type != stream_meta_types_types.cend();
                 ++stream_meta_type) {
                createNode(stream_meta_types_node, *(stream_meta_type->second));
            }
        }

        DOM_NODE_TYPE streams_node = parent_node.createChild("streams");
        const auto& streams = dd.getStreams();
        for (auto stream = streams.cbegin(); stream != streams.cend(); ++stream) {
            createNode(streams_node, *(stream->second));
        }
    }
};
} // namespace dd
} // namespace ddl

#endif // DD_DD_TO_XML_FACTORY_H_INCLUDED