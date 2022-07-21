/**
 * @file
 * Public API for @ref a_util::xml::DOM "DOM" and @ref a_util::xml::DOMElement "DOMElement" types
 * and functions
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

#ifndef A_UTILS_UTIL_XML_DOM_HEADER_INCLUDED_
#define A_UTILS_UTIL_XML_DOM_HEADER_INCLUDED_

#include <a_util/memory/stack_ptr.h>

#include <list>
#include <map>
#include <stdexcept>

namespace a_util {
namespace xml {
class DOM;
class DOMElement;

/// Exception class used by the DOM classes
class DOMException : public std::runtime_error {
public:
    /**
     * CTOR
     * @param[in] what Error message to distribute with the exception
     */
    DOMException(const std::string& what);
};

/// Tyoe alias for DOMElement lists
typedef std::list<DOMElement> DOMElementList;
/// Type alias for DOM attributes
typedef std::map<std::string, std::string> DOMAttributes;

/// Representation for an element in the DOM
class DOMElement {
public:
    /// CTOR
    DOMElement();

    /// DTOR
    ~DOMElement();

    /**
     * Copy constructor.
     * @param[in] element The object to copy from
     */
    DOMElement(const DOMElement& element);

    /**
     * Copy assignment operator.
     * @param[in] element The element to copy from
     * @return *this
     */
    DOMElement& operator=(const DOMElement& element);

    /**
     * Returns the value of an attribute with @c name
     * @param[in] name The name of the attribute
     * @param[in] def_value A default value that is returned if the attribute does not exist
     * @return The value of the attribute or the default value
     */
    std::string getAttribute(const std::string& name,
                             const std::string& def_value = std::string()) const;

    /**
     * Get a map of all attributes of this element
     * @return A map of all attributes of the element
     */
    DOMAttributes getAttributes() const;

    /**
     * Get the first child mathing the @c name of this element
     * @param[in] name The name of the child element
     * @return The child element or a @c nullptr if not found
     */
    DOMElement getChild(const std::string& name) const;

    /**
     * Get a list of all child elements of the element
     * @return The element list of all children
     */
    DOMElementList getChildren() const;

    /**
     * Get the data of the element.
     * @return The data
     */
    std::string getData() const;

    /**
     * Get the DOM document this element belongs to
     * @return The document this element belongs to
     */
    DOM getDocument() const;

    /**
     * Get the name of the element
     * @return The name of the element
     */
    std::string getName() const;

    /**
     * Get this elements parent element
     * @return The parent element - empty if no parent element exists
     */
    DOMElement getParent() const;

    /**
     * Get current path of the element
     * @return Returns the path
     */
    std::string getPath() const;

    /**
     * Sets an attribute.
     * Setting an attribute on an DOMElement object not created from an existing DOM will fail as
     * a DOMElement needs to be linked to a valid DOM
     * @param[in] name The attribute name
     * @param[in] strValue The value of the attribute
     * @return @c false if its an invalid attribute name, @c true otherwise
     */
    bool setAttribute(const std::string& name, const std::string& strValue);

    /**
     * Sets the data of the element.
     * Setting the data on an DOMElement object not created from an existing DOM will fail as
     * a DOMElement needs to be linked to a valid DOM
     * @param[in] data The new data
     * @return @c true if the data was set, @c false otherwise
     */
    bool setData(const std::string& data);

    /**
     * Sets the name of the element.
     * Setting the name on an DOMElement object not created from an existing DOM will fail as
     * a DOMElement needs to be linked to a valid DOM
     * @param[in] name The new name
     * @return @c true if the name was set, @c false otherwise.
     */
    bool setName(const std::string& name);

    /**
     * Checks if an attribute exists
     * @param[in] name The name of the attribute
     * @return @c true if attribute exists, otherwise @c false
     */
    bool hasAttribute(const std::string& name) const;

    /**
     * Removes an attribute
     * @param [in] name The name of the attribute
     * @return @c true if the attribute was erased, @c false otherwise
     */
    bool eraseAttribute(const std::string& name);

    /// Finds a node based on a query.
    ///
    /// Some basic syntax:
    ///  - queries starting with '/' are starting at the root node
    ///  - use '*' to search all child nodes in the path
    ///  - if you search with absolute paths, don't forget to use the root node in the path.
    ///  - to search for certain attributes on nodes use [\@property0='value'] or [\@property0] to
    /// check the existence. Keep in mind that attributes search method will include parent (root)
    /// node in the elements list if search value match. In this case parent node will be the first
    /// element in the list.
    ///  - attributes can be concatenated to make them behave like an AND:
    /// [\@property0][\@property1='value']
    ///  - recursive search from root starts with '//nodes_to_search'. The whole tree is being
    /// searched.
    ///  - recursive search from current node starts with './/nodes_to_search'. The whole subtree is
    /// being searched.
    ///  - use 'and' or 'or' to connect multiple properties logically (USE ONLY ONE OPERATOR!)
    ///
    /// Here are some examples of search queries.
    ///  - /root/path/to/node                :absolute path to nodes
    ///  - path/to/node                      :search starting on current node. Same as above, if
    /// searched in root node.
    ///  - /root/node/*                      :find all children of all 'node' nodes
    ///  - *                                 :find all children of current node
    ///  - /root/node[\@prop='2']/*        :find all children of 'node' nodes with matching
    ///  attribute
    ///
    ///  - //node                            :finds all 'node' nodes in the tree (recursive search)
    ///  - .//node                           :recursive search starting from current node
    ///  - //*[\@prop='2']                 :find all nodes in the tree with matching attributes
    ///  - /root/path/to[\@prop='2']//node :find all 'node' nodes in the subtree of the sub paths
    /// '/root/path/to'
    ///
    ///  - child/*[\@prop1='2']            :find all nodes in 'child' nodes with matching attributes
    ///
    ///  - *[\@prop1='3' and \@prop2='4']     :logical AND comparison for attributes
    ///
    /// @param[in] query The query string
    /// @param[out] element_ptr This will point to the found element
    /// @return @c false if no matching node is found or the query is invalid, @c true otherwise
    bool findNode(const std::string& query, DOMElement& element_ptr) const;

    /**
     * Finds nodes based on a query.
     *
     * See @ref findNode() for the syntax.
     * @param[in] query The query string
     * @param[out] elements This list will be filled with the found elements
     * @return @c false if no matching node is found or the query is invalid, @c true otherwise
     */
    bool findNodes(const std::string& query, DOMElementList& elements) const;

    /**
     * Creates a new child element
     * @param[in] name The name of the child
     * @return The new child
     */
    DOMElement createChild(const std::string& name);

    /**
     * Creates a new child element before an existing child element
     * @param[in] name The name of the child
     * @param[in] before The existing child element
     * @return The new child
     */
    DOMElement createChildBefore(const std::string& name, const DOMElement& before);

    /**
     * Erases the first child of that name in this element
     * @param name [in] The name of the child element
     * @return @c true if the child was removed, @c false otherwise
     */
    bool removeChild(const std::string& name);

    /**
     * Check whether this DOMElement is empty
     * @return @c true if empty, @c false otherwise
     */
    bool isNull() const;

private:
    /**
     * Comparison operator.
     * @param lhs [in] Left instance
     * @param rhs [in] Right instance
     * @return @c true if the two instances are equal (deep comparison)
     */
    friend bool operator==(const DOMElement& lhs, const DOMElement& rhs);

private:
    class Implementation;
    memory::StackPtr<Implementation, 16> _impl;
    friend class DOM;
};

/**
 * Incomparison operator.
 * @param lhs [in] Left instance
 * @param rhs [in] Right instance
 * @return @c true if the two instances are not equal (deep comparison), @c false otherwise
 */
bool operator!=(const DOMElement& lhs, const DOMElement& rhs);

/// XML parser to read and write standard XML files
class DOM {
public:
    /// CTOR
    DOM();

    /// DTOR
    ~DOM();

    /**
     * Copy constructor
     * @param[in] dom The object to copy from
     */
    DOM(const DOM& dom);

    /**
     * Copy assignment
     * @param[in] dom The element to copy from
     * @return Reference to *this
     */
    DOM& operator=(const DOM& dom);

    /**
     * Loads an xml file
     * @param[in] file_path Path to the filename
     * @return @c false if file could not be loaded (check DOM::getLastError()), otherwise @c true
     */
    bool load(const std::string& file_path);

    /**
     * save document to an xml file
     * @param[in] file_path Path to the file to save
     * @return @c false if file could not be safed (check DOM::getLastError()), otherwise @c true
     */
    bool save(const std::string& file_path) const;

    /**
     * Create the dom from an input string
     * @param[in] xml String which must have a valid XML syntax
     * @return @c false string is invalid (check DOM::getLastError()), otherwise @c true
     */
    bool fromString(const std::string& xml);

    /**
     * Creates an xml string representation of the dom
     * @return XML string representation of the dom
     */
    std::string toString() const;

    /**
     * Resets the instance to the empty state
     * @post @c this->IsNull()
     */
    void reset();

    /**
     * Get root element of the dom
     * @return The root element of the dom
     */
    DOMElement getRoot() const;

    /**
     * Finds a node based on a query
     * See DOMElement::findNode() for the syntax.
     *
     * @param[in] query The query string
     * @param[out] element_ptr This will point to the found element.
     * @return Whether or not the node was found
     */
    bool findNode(const std::string& query, DOMElement& element_ptr) const;

    /**
     * Finds nodes based on a query.
     * See DOMElement::findNode() for the syntax.
     *
     * @param[in] query The query string
     * @param[out] elements This list will be filled with the found elements
     * @return Whether or not any nodes where found
     */
    bool findNodes(const std::string& query, DOMElementList& elements) const;

    /**
     * Get the last error description occurred
     * @return The last occurred error
     */
    std::string getLastError() const;

    /**
     * Check whether this DOM is empty
     * @return @c true if empty, @c false otherwise
     */
    bool isNull() const;

private:
    class Implementation;
    memory::StackPtr<Implementation, 280> _impl;
    friend class DOMElement;
};

/**
 * Comparison operator.
 * @param lhs [in] Left instance
 * @param rhs [in] Right instance
 * @return @c true if the two instances are equal (deep comparison), @c false otherwise
 */
bool operator==(const DOM& lhs, const DOM& rhs);

/**
 * Incomparison operator.
 * @param lhs [in] Left instance
 * @param rhs [in] Right instance
 * @return @c false if the two instances are equal (deep comparison), @c true otherwise
 */
bool operator!=(const DOM& lhs, const DOM& rhs);

} // namespace xml
} // namespace a_util

#endif // A_UTILS_UTIL_XML_DOM_HEADER_INCLUDED_
