/**
 * @file
 * XML library / DOM classes
 *
 * Copyright @ 2021 VW Group. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <a_util/strings.h>
#include <a_util/xml.h>

#include <pugixml-1.8/src/pugixml.hpp>

#include <algorithm> //std::equal
#include <functional>

namespace a_util {
namespace xml {
// using this in member initialization list
#pragma warning(disable : 4355)

// XML line indendation used in toString/save
static const char* const xml_indentation = "    ";

DOMException::DOMException(const std::string& what) : std::runtime_error(what.c_str())
{
}

// DOMElement implementation
class DOMElement::Implementation {
public:
    pugi::xml_node _node;
    DOM* _document = {};

    void initFromNode(const pugi::xml_node node)
    {
        this->_node = node;
    }

    DOMElement createChildFromNode(pugi::xml_node new_node)
    {
        DOMElementList children;
        children.push_back(DOMElement());

        DOMElement& child = children.back();
        child._impl->_document = _document;
        child._impl->initFromNode(new_node);

        return child;
    }
};

DOMElement::DOMElement() : _impl(Implementation())
{
}

DOMElement::~DOMElement()
{
}

DOMElement::DOMElement(const DOMElement& element) : _impl(Implementation(*element._impl))
{
}

DOMElement& DOMElement::operator=(const DOMElement& element)
{
    if (&element != this) {
        _impl.reset(Implementation(*element._impl));
    }

    return *this;
}

std::string DOMElement::getName() const
{
    return _impl->_node.name();
}

bool DOMElement::setName(const std::string& name)
{
    return _impl->_node.set_name(name.c_str());
}

DOMElement DOMElement::getParent() const
{
    DOMElement parent(*this);
    parent._impl->_node = _impl->_node.parent();

    return parent;
}

DOM DOMElement::getDocument() const
{
    return *_impl->_document;
}

DOMElementList DOMElement::getChildren() const
{
    using pugi::xml_node_iterator;

    DOMElementList children;
    for (xml_node_iterator it = _impl->_node.children().begin();
         _impl->_node.children().end() != it;
         ++it) {
        children.push_back(*this);
        children.back()._impl->_node = *it;
    }
    return children;
}

std::string DOMElement::getData() const
{
    return _impl->_node.child_value();
}

bool DOMElement::setData(const std::string& data)
{
    pugi::xml_node child = _impl->_node.first_child();
    if (!child) {
        child = _impl->_node.append_child(pugi::node_pcdata);
    }
    return child.set_value(data.c_str());
}

DOMAttributes DOMElement::getAttributes() const
{
    DOMAttributes attributes;
    for (pugi::xml_attribute_iterator it = _impl->_node.attributes_begin();
         it != _impl->_node.attributes_end();
         ++it) {
        attributes[it->name()] = it->value();
    }

    return attributes;
}

bool DOMElement::hasAttribute(const std::string& name) const
{
    pugi::xml_attribute attr = _impl->_node.attribute(name.c_str());
    return !attr.empty();
}

std::string DOMElement::getAttribute(const std::string& name, const std::string& def_value) const
{
    pugi::xml_attribute attr = _impl->_node.attribute(name.c_str());
    if (attr) {
        return attr.as_string(def_value.c_str());
    }

    return def_value;
}

bool DOMElement::setAttribute(const std::string& name, const std::string& value)
{
    if (name.empty()) {
        return false;
    }

    pugi::xml_attribute attr = _impl->_node.attribute(name.c_str());
    if (!attr) {
        attr = _impl->_node.append_attribute(name.c_str());
    }

    if (attr) {
        attr.set_value(value.c_str());
        return true;
    }

    return false;
}

bool DOMElement::eraseAttribute(const std::string& name)
{
    return _impl->_node.remove_attribute(name.c_str());
}

std::string DOMElement::getPath() const
{
    return _impl->_node.path();
}

bool DOMElement::findNode(const std::string& query, DOMElement& element_ptr) const
{
    try {
        pugi::xpath_node oRes = _impl->_node.select_node(query.c_str());
        if (!oRes) {
            element_ptr = DOMElement();
            return false;
        }
        else {
            element_ptr._impl->initFromNode(oRes.node());
        }
    }
    catch (pugi::xpath_exception&) {
        return false;
    }

    return true;
}

bool DOMElement::findNodes(const std::string& query, DOMElementList& elements) const
{
    elements.clear();
    try {
        pugi::xpath_node_set oRes = _impl->_node.select_nodes(query.c_str());
        if (oRes.empty()) {
            return false;
        }

        for (pugi::xpath_node_set::const_iterator it = oRes.begin(); it != oRes.end(); ++it) {
            elements.push_back(DOMElement());
            elements.back()._impl->initFromNode(it->node());
        }
    }
    catch (pugi::xpath_exception&) {
        return false;
    }

    return true;
}

bool DOMElement::sortNodes(const std::string& query, const SortingOrder order)
{
    DOMElementList elements;
    if (findNodes(query, elements)) {
        // sort list of nodes by node name
        elements.sort([](const auto& element_a, const auto& element_b) {
            return element_a.getName() < element_b.getName();
        });

        // get parent node for reordering
        xml_node parent = elements.front()._impl->_node.parent();

        // append_move: move each node of list to end
        // prepend_move: move each node of list to begin
        const auto move_node = order == SortingOrder::ascending ?
                                   std::mem_fn(&xml_node::append_move) :
                                   std::mem_fn(&xml_node::prepend_move);
        // reorder nodes in xml parent node
        std::for_each(
            elements.cbegin(), elements.cend(), [&move_node, &parent](const auto& element) {
                move_node(parent, element._impl->_node);
            });
    }
    return !elements.empty();
}

bool DOMElement::sortNodes(const std::string& query,
                           const SortingOrder order,
                           const std::size_t number_of_nodes)
{
    return sortNodes(query, std::string(), order, number_of_nodes);
}

std::size_t DOMElement::sortNodes(const std::string& query,
                                  const std::string& attribute,
                                  const SortingOrder order)
{
    DOMElementList elements;
    if (findNodes(query, elements)) {
        // sort list of nodes by attribute name
        elements.sort([&attribute](const auto& element_a, const auto& element_b) {
            return element_a.getAttribute(attribute) < element_b.getAttribute(attribute);
        });

        // get anchor and parent node for reordering
        xml_node anchor = elements.front()._impl->_node;
        xml_node parent = anchor.parent();

        // insert_move_after: move each node of list behind the last moved node
        // insert_move_before: move each node of list before the last moved node
        const auto move_node = order == SortingOrder::ascending ?
                                   std::mem_fn(&xml_node::insert_move_after) :
                                   std::mem_fn(&xml_node::insert_move_before);
        // reorder nodes in xml parent node
        std::for_each(std::next(elements.cbegin()),
                      elements.cend(),
                      [&move_node, &anchor, &parent](const auto& element) {
                          anchor = move_node(parent, element._impl->_node, anchor);
                      });
    }
    return elements.size();
}

bool DOMElement::sortNodes(const std::string& query,
                           const std::string& attribute,
                           const SortingOrder order,
                           const std::size_t number_of_nodes)
{
    bool result = true;
    for (std::size_t i = 1; i <= number_of_nodes; ++i) {
        // replace [*] in query by a concrete number
        const auto sub_query = strings::replace(query, "[*]", "[" + strings::toString(i) + "]");
        if (attribute.empty()) {
            result &= sortNodes(sub_query, order);
        }
        else {
            result &= sortNodes(sub_query, attribute, order) > 0;
        }
    }
    return result;
}

DOMElement DOMElement::createChild(const std::string& name)
{
    pugi::xml_node node = _impl->_node.append_child(name.c_str());
    return _impl->createChildFromNode(node);
}

DOMElement DOMElement::createChildBefore(const std::string& name, const DOMElement& before)
{
    pugi::xml_node node = _impl->_node.insert_child_before(name.c_str(), before._impl->_node);
    return _impl->createChildFromNode(node);
}

DOMElement DOMElement::getChild(const std::string& name) const
{
    const DOMElementList children = getChildren();
    for (DOMElementList::const_iterator it = children.begin(); it != children.end(); ++it) {
        if (it->getName() == name) {
            return *it;
        }
    }

    return DOMElement();
}

bool DOMElement::removeChild(const std::string& name)
{
    DOMElementList children = getChildren();
    for (DOMElementList::iterator it = children.begin(); it != children.end(); ++it) {
        if (it->getName() == name) {
            return it->_impl->_node.parent().remove_child(it->_impl->_node);
        }
    }

    return false;
}

bool DOMElement::isNull() const
{
    return *this == DOMElement();
}

bool operator==(const DOMElement& lhs, const DOMElement& rhs)
{
    struct Local {
        static bool hasEqualAttributesNameAndData(const DOMElement& left, const DOMElement& right)
        {
            return (left.getName() == right.getName()) &&
                   (left.getAttributes() == right.getAttributes()) &&
                   (left.getData() == right.getData());
        }
    };

    bool equal = lhs._impl->_node.empty() && rhs._impl->_node.empty() ?
                     true :
                     lhs._impl->_node == rhs._impl->_node;

    if (!equal && Local::hasEqualAttributesNameAndData(lhs, rhs)) { // traverse through the tree
        DOMElementList lhs_nodes;
        DOMElementList rhs_nodes;
        lhs.findNodes(".//*", lhs_nodes);
        lhs.findNodes(".//*", rhs_nodes);

        if (lhs_nodes.size() == rhs_nodes.size()) {
            equal = std::equal(lhs_nodes.begin(),
                               lhs_nodes.end(),
                               rhs_nodes.begin(),
                               Local::hasEqualAttributesNameAndData);
        }
    }
    return equal;
}

bool operator!=(const DOMElement& lhs, const DOMElement& rhs)
{
    return !(lhs == rhs);
}

// DOM implementation
class DOM::Implementation {
public:
    pugi::xml_document _document;
    DOMElement _root;
    mutable std::string _last_error;

    Implementation() : _document(), _root(), _last_error()
    {
    }

    Implementation(const Implementation& other)
        : _document(), _root(other._root), _last_error(other._last_error)
    {
        _document.reset(other._document);
    }
};

DOM::~DOM()
{
}

DOM::DOM() : _impl()
{
    reset();
}

DOM::DOM(const DOM& dom) : _impl()
{
    fromString(dom.toString());
}

DOM& DOM::operator=(const DOM& dom)
{
    if (this != &dom) {
        fromString(dom.toString());
    }
    return *this;
}

bool DOM::load(const std::string& file_path)
{
    {
        pugi::xml_document temp;
        pugi::xml_parse_result res = temp.load_file(file_path.c_str());
        if (!res) {
            _impl->_last_error = res.description();
            return false;
        }

        reset();
        _impl->_document.reset(temp);
    }

    _impl->_root._impl->initFromNode(_impl->_document.root().first_child());

    return true;
}

bool DOM::save(const std::string& file_path) const
{
    if (!_impl->_document.save_file(file_path.c_str(), xml_indentation)) {
        _impl->_last_error = "Failed to save dom to file";
        return false;
    }

    return true;
}

bool DOM::fromString(const std::string& xml)
{
    {
        pugi::xml_document temp;
        pugi::xml_parse_result res = temp.load_string(xml.c_str());
        if (!res) {
            _impl->_last_error = res.description();
            return false;
        }

        reset();
        _impl->_document.reset(temp);
    }

    _impl->_root._impl->initFromNode(_impl->_document.root().first_child());

    return true;
}

std::string DOM::toString() const
{
    struct xml_string_writer : pugi::xml_writer {
        std::string strResult;
        virtual void write(const void* data, size_t size)
        {
            strResult.append(static_cast<const char*>(data), size);
        }
    };

    xml_string_writer oWriter;
    _impl->_document.save(oWriter, xml_indentation);
    return oWriter.strResult;
}

void DOM::reset()
{
    _impl->_document.reset();
    _impl->_root._impl->_document = this;
    _impl->_root._impl->initFromNode(_impl->_document.append_child(""));
    _impl->_last_error.clear();
}

DOMElement DOM::getRoot() const
{
    return _impl->_root;
}

bool DOM::findNode(const std::string& query, DOMElement& element_ptr) const
{
    return getRoot().findNode(query, element_ptr);
}

bool DOM::findNodes(const std::string& query, DOMElementList& elements) const
{
    return getRoot().findNodes(query, elements);
}

std::string DOM::getLastError() const
{
    return _impl->_last_error;
}

bool DOM::isNull() const
{
    return *this == DOM();
}

bool operator==(const DOM& lhs, const DOM& rhs)
{
    return lhs.getRoot() == rhs.getRoot();
}

bool operator!=(const DOM& lhs, const DOM& rhs)
{
    return !(lhs == rhs);
}

} // namespace xml
} // namespace a_util
