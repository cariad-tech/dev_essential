/**
 * @file
 * Implementation of the CodecIterator.
 *
 * @copyright
 * @verbatim
Copyright @ 2022 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */

#ifndef DDL_CODEC_ITERATOR_CLASS_HEADER
#define DDL_CODEC_ITERATOR_CLASS_HEADER

#include <a_util/variant.h>
#include <ddl/codec/codec_index.h>

#include <functional>

namespace ddl {
namespace codec {

// forward declarations
template <typename ElementAccessType, typename ChildElementsType>
class FactoryElement;
template <typename ElementAccessType, typename ChildElementsType>
class DecoderElement;
template <typename ElementAccessType, typename ChildElementsType>
class CodecElement;

/**
 * @brief The element iterator.
 * @see @ref ChildElements.
 * @tparam ElementAccessType The element access concept type.
 *                           (see concept templates @ref FactoryElementAccess, @ref
 * DecoderElementAccess, @ref CodecElementAccess)
 */
template <typename ElementAccessType>
class ElementIterator {
public:
    using element_type = typename ElementAccessType::element_type; //!< The element type in use
    using value_type = element_type;                               //!< Value type of the iterator
    using reference = value_type&;                                 //!< Reference value type
    using const_reference = const value_type&;                     //!< const reference value type
    using pointer = value_type*;                                   //!< Pointer type
    using const_pointer = const value_type*;             //!< pointer to const data value type
    using difference_type = int;                         //!< difference value type
    using iterator_category = std::forward_iterator_tag; //!< iterator type

    /**
     * @brief default CTOR
     */
    ElementIterator() = delete;
    /**
     * @brief CTOR for one element
     *
     * @param element the element.
     */
    ElementIterator(value_type&& element) : _element(std::move(element))
    {
    }
    /**
     * @brief increasing operator to the next position.
     *
     * @return self_type
     */
    ElementIterator& operator++()
    {
        next();
        return *this;
    }
    /**
     * @brief iterator increasing.
     *
     * @return self_type
     */
    ElementIterator operator++(int)
    {
        const auto old_index = _element.getIndex();
        next();
        return ElementIterator(old_index, _element.getAccess());
    }
    /**
     * @brief checks for equality (refering the same element!)
     *
     * @param rhs the other iterator
     * @return true is equal
     * @return false is not equal
     */
    bool operator==(const ElementIterator& rhs) const
    {
        return _element.getIndex() == rhs._element.getIndex();
    }
    /**
     * @brief checks for not equality (does not refer the same element!)
     *
     * @param rhs the other iterator
     * @return true is not equal
     * @return false is equal
     */
    bool operator!=(const ElementIterator& rhs) const
    {
        return _element.getIndex() != rhs._element.getIndex();
    }
    /**
     * @brief defering access (check for end!)
     *
     * @return const reference
     */
    const_reference operator*() const
    {
        return _element;
    }
    /**
     * @brief pointer access (check for end!)
     *
     * @return const pointer
     */
    const_pointer operator->() const
    {
        return &_element;
    }
    /**
     * @brief defering access (check for end!)
     *
     * @return const reference
     */
    reference operator*()
    {
        return _element;
    }
    /**
     * @brief pointer access (check for end!)
     *
     * @return const pointer
     */
    pointer operator->()
    {
        return &_element;
    }

private:
    void next()
    {
        _element.next();
    }
    element_type _element;
};

/**
 * @brief The const element iterator.
 * @see @ref ChildElements.
 * @tparam ElementAccessType The element access concept type.
 *                           (see concept templates @ref FactoryElementAccess, @ref
 * DecoderElementAccess, @ref CodecElementAccess)
 */
template <typename ElementAccessType>
class ElementIteratorConst {
public:
    using element_type = typename ElementAccessType::element_type; //!< The element type in use
    using value_type = const element_type;               //!< const value type of the iterator
    using const_reference = value_type&;                 //!< const reference value type
    using const_pointer = value_type*;                   //!< pointer to const data value type
    using difference_type = int;                         //!< difference value type
    using iterator_category = std::forward_iterator_tag; //!< iterator type

    /**
     * @brief default CTOR
     */
    ElementIteratorConst() = delete;
    /**
     * @brief CTOR for one element
     * @param element the element.
     */
    ElementIteratorConst(value_type&& element) : _element(std::move(element))
    {
    }
    /**
     * @brief increasing operator to the next position.
     * @return self_type
     */
    ElementIteratorConst& operator++()
    {
        next();
        return *this;
    }
    /**
     * @brief iterator increasing.
     * @return self_type
     */
    ElementIteratorConst operator++(int)
    {
        const auto old_index = _element.getIndex();
        next();
        return self_type(old_index, _element.getAccess());
    }

    /**
     * @brief checks for equality (refering the same element!)
     *
     * @param rhs the other iterator
     * @return true is equal
     * @return false is not equal
     */
    bool operator==(const ElementIteratorConst& rhs) const
    {
        return _element.getIndex() == rhs._element.getIndex();
    }
    /**
     * @brief checks for not equality (does not refer the same element!)
     *
     * @param rhs the other iterator
     * @return true is not equal
     * @return false is equal
     */
    bool operator!=(const ElementIteratorConst& rhs) const
    {
        return _element.getIndex() != rhs._element.getIndex();
    }
    /**
     * @brief defering access (check for end!)
     *
     * @return const reference
     */
    const_reference operator*() const
    {
        return _element;
    }
    /**
     * @brief pointer access (check for end!)
     *
     * @return const pointer
     */
    const_pointer operator->() const
    {
        return &_element;
    }

private:
    void next()
    {
        _element.next();
    }
    element_type _element;
};

/**
 * @brief Iteratable container type for the given Element type in ElementAccessType::element_type
 * @tparam ElementAccessType The element access concept type.
 *                           (see concept templates @ref FactoryElementAccess, @ref
 * DecoderElementAccess, @ref CodecElementAccess)
 */
template <typename ElementAccessType>
class ChildElements {
public:
    /// The supported access type (from ElementAccessType::access_type)
    using access_type = typename ElementAccessType::access_type;
    /// The supported element type (from ElementAccessType::element_type)
    using element_type = typename ElementAccessType::element_type;
    using const_iterator = ElementIteratorConst<ElementAccessType>; //!< const iterator type
    using iterator = ElementIterator<ElementAccessType>;            //!< iterator type

    /**
     * @brief friend declaration for element type
     */
    friend element_type;
    /**
     * @brief friend declaration for access type
     */
    friend access_type;
    /**
     * @brief friend declaration for factory element
     */
    friend FactoryElement<ElementAccessType, ChildElements<ElementAccessType>>;
    /**
     * @brief friend declaration for decoder element
     */
    friend DecoderElement<ElementAccessType, ChildElements<ElementAccessType>>;
    /**
     * @brief friend declaration for codec element
     */
    friend CodecElement<ElementAccessType, ChildElements<ElementAccessType>>;

    /**
     * @brief move CTOR
     */
    ChildElements(ChildElements&&) = default;
    /**
     * @brief move operator
     * @return ChildElements&
     */
    ChildElements& operator=(ChildElements&&) = default;
    /**
     * @brief no copy CTOR
     */
    ChildElements(const ChildElements&) = delete;
    /**
     * @brief no copy operator
     * @return ChildElements&
     */
    ChildElements& operator=(const ChildElements&) = delete;
    /**
     * @brief DTOR
     * */
    ~ChildElements() = default;

    /**
     * @brief cbegin
     * @return const_iterator
     */
    const_iterator cbegin() const
    {
        if (_child_element_count > 0) {
            CodecIndex begin_index(_codec_base_index, {0, 0});
            ElementAccessType::resolve(_access, begin_index);
            size_t element_child_count = begin_index.getLayout().child_element_count;
            return const_iterator(element_type(
                std::move(begin_index), _child_element_count, element_child_count, _access));
        }
        else {
            return const_iterator(element_type(CodecIndex(), 0, 0, _access));
        }
    }

    /**
     * @brief begin
     * @return const_iterator
     */
    const_iterator begin() const
    {
        return cbegin();
    }
    /**
     * @brief cend
     * @return const_iterator
     */
    const_iterator cend() const
    {
        if (_child_element_count > 0) {
            return const_iterator(
                element_type(CodecIndex(_codec_base_index, {_child_element_count, 0}),
                             _child_element_count,
                             0,
                             _access));
        }
        else {
            return const_iterator(element_type(CodecIndex(), 0, 0, _access));
        }
    }
    /**
     * @brief end
     * @return const_iterator
     */
    const_iterator end() const
    {
        return cend();
    }
    /**
     * @brief begin
     * @return iterator
     */
    iterator begin()
    {
        if (_child_element_count > 0) {
            CodecIndex begin_index(_codec_base_index, {0, 0});
            ElementAccessType::resolve(_access, begin_index);
            size_t element_child_count = begin_index.getLayout().child_element_count;
            return iterator(element_type(
                std::move(begin_index), _child_element_count, element_child_count, _access));
        }
        else {
            return iterator(element_type(CodecIndex(), 0, 0, _access));
        }
    }
    /**
     * @brief end
     * @return iterator
     */
    iterator end()
    {
        if (_child_element_count > 0) {
            return iterator(element_type(CodecIndex(_codec_base_index, {_child_element_count, 0}),
                                         _child_element_count,
                                         0,
                                         _access));
        }
        else {
            return iterator(element_type(CodecIndex(), 0, 0, _access));
        }
    }
    /**
     * @brief Size of the container (child count)
     * @return size_t
     */
    size_t size() const
    {
        return _child_element_count;
    }

private:
    ChildElements(const CodecIndex& base_index, access_type& access)
        : _codec_base_index(base_index),
          _child_element_count(ElementAccessType::getChildCount(access, base_index)),
          _access(access)
    {
    }

    ChildElements(const CodecIndex& base_index, size_t child_element_count, access_type& access)
        : _codec_base_index(base_index), _child_element_count(child_element_count), _access(access)
    {
    }
    void resetChildCount(size_t child_element_count)
    {
        _child_element_count = child_element_count;
    }
    void resetChildCount()
    {
        resetChildCount(ElementAccessType::getChildCount(_access, _codec_base_index));
    }

    const CodecIndex& _codec_base_index;
    size_t _child_element_count;
    access_type& _access;
};

/**
 * @brief A factory element access type concept template to retrieve element information from the @p
 * AccessType. This class is to retrieve common information of the element.
 * @see @ref FactoryElement.
 *
 * @tparam AccessType The AccessType for the element (CodecFactory, Codec, Decoder, etc. )
 */
template <typename AccessType>
class FactoryElementAccess {
public:
    using access_type = AccessType; //!< The access type (CodecFactory, Codec, Decoder, etc.)
    using self_type = FactoryElementAccess<AccessType>;                       //!< self type
    using element_type = FactoryElement<self_type, ChildElements<self_type>>; //!< element type

    /**
     * @brief Get a element object for the given @p full_element_name.
     *
     * @param access defined access type.
     * @param full_element_name The full name of the element in point notation (i.e. \p
     * "element1.child_element[4].element_value").
     * @return element_type
     */
    static element_type getElement(access_type& access, const std::string& full_element_name)
    {
        return access.getElement(full_element_name);
    };
    /**
     * @brief Get a element object for the given @p index.
     *
     * @param access defined access type.
     * @param index The codec index.
     * @return element_type
     */
    static element_type getElement(access_type& access, const CodecIndex& index)
    {
        return access.getElement(index);
    };
    /**
     * @brief Get the Child Count
     *
     * @param access defined access type.
     * @param index The codec index
     * @return size_t
     */
    static size_t getChildCount(access_type& access, const CodecIndex& index)
    {
        return access.getElementChildCount(index);
    };
    /**
     * @brief Get the full name of the element within its main structure.
     * If the element is an array you get the elements array name representation:
     * "main_element.element_name[0]" or main_element.element_name[1] ... etc. If the element is not
     * an array you get the elements name representation: "main_element.element_name".
     *
     * @param access defined access type.
     * @param index The codec index
     * @return std::string
     */
    static std::string getFullName(access_type& access, const CodecIndex& index)
    {
        return access.getElementFullName(index);
    };
    /**
     * @brief Get the name of the element within its level structure.
     * If the element is an array you get the elements array name representation: "element_name[0]"
     * or element_name[1] ... etc. If the element is not an array you get the elements name
     * representation: "element_name".
     *
     * @param access defined access type.
     * @param index The codec index
     * @return std::string
     */
    static std::string getName(access_type& access, const CodecIndex& index)
    {
        return access.getElementName(index);
    };
    /**
     * @brief Get the name of the element within its level structure.
     * If the element is an array you get the elements base name representation : "element_name"
     * for each array element. If the element is not an array you get the elements name
     * representation is also the base name representation: "element_name".
     *
     * @param access defined access type.
     * @param index The codec index
     * @return std::string
     */
    static std::string getBaseName(access_type& access, const CodecIndex& index)
    {
        return access.getElementBaseName(index);
    };

    /**
     * @brief Resolves the given CodecIndex and set the layout information
     *
     * @param access defined access type.
     * @param index The codec index to resolve
     * @throws throws if not found
     */
    static void resolve(access_type& access, CodecIndex& index)
    {
        return access.resolve(index);
    };
};

/**
 * @brief A FactoryElement
 *
 * @tparam ElementAccessType The element access concept type.
 *                           (see concept templates @ref FactoryElementAccess, @ref
 * DecoderElementAccess, @ref CodecElementAccess)
 * @tparam ChildElementsType The child elements type
 */
template <typename ElementAccessType, typename ChildElementsType = ChildElements<ElementAccessType>>
class FactoryElement {
public:
    using self_type = FactoryElement<ElementAccessType, ChildElementsType>; //!< self type
    using access_type = typename ElementAccessType::access_type;            //!< access type
    using child_elements_type = ChildElementsType;                //!< allowed container type
    using iterator_type = typename child_elements_type::iterator; //!< allowed iterator type
    /// allowed const iterator type
    using const_iterator_type = typename child_elements_type::const_iterator;

    /**
     * @brief friend declaration for child_elements_type
     */
    friend child_elements_type;
    /**
     * @brief friend declaration for iterator_type
     */
    friend iterator_type;
    /**
     * @brief friend declaration for child_elements_type
     */
    friend const_iterator_type;
    /**
     * @brief friend declaration for access_type
     */
    friend access_type;
    /**
     * @brief friend declaration for DecoderElement
     */
    friend DecoderElement<ElementAccessType, ChildElementsType>;
    /**
     * @brief friend declaration for CodecElement
     */
    friend CodecElement<ElementAccessType, ChildElementsType>;

    /**
     * @brief no CTOR
     */
    FactoryElement() = delete;
    /**
     * @brief move CTOR
     * @param other the element to move
     */
    FactoryElement(FactoryElement&& other)
        : _codec_index(std::move(other._codec_index)),
          _end_element_index(std::move(other._end_element_index)),
          _child_elements(_codec_index, other._child_elements.size(), other._access),
          _access(other._access)
    {
    }
    /**
     * @brief copy assignment operator
     * @param other the element to move
     * @return FactoryElement&
     */
    FactoryElement& operator=(FactoryElement&& other)
    {
        _codec_index = std::move(other._codec_index);
        _end_element_index = std::move(other._end_element_index);
        _child_elements = child_elements_type(_codec_index, other._child_elements.size(), _access);
    }
    /**
     * @brief copy CTOR
     * @param other the element to copy
     */
    FactoryElement(const FactoryElement& other)
        : _codec_index(other._codec_index),
          _end_element_index(other._end_element_index),
          _child_elements(_codec_index, other._child_elements.size(), other._access),
          _access(other._access)
    {
    }
    /**
     * @brief move assignment operator
     * @param other the element to copy
     * @return FactoryElement&
     */
    FactoryElement& operator=(const FactoryElement& other)
    {
        _codec_index = other._codec_index;
        _end_element_index = other._end_element_index;
        _child_elements = child_elements_type(_codec_index, other._child_elements.size(), _access);
    }

    /**
     * @brief DTOR
     *
     */
    ~FactoryElement() = default;
    /**
     * @brief Get the codec index of the element.
     * @return const CodecIndex&
     */
    const CodecIndex& getIndex() const
    {
        return _codec_index;
    }
    /**
     * @brief Get the array size
     * @return size_t
     */
    size_t getArraySize() const
    {
        return _codec_index.getLayout().array_size;
    }
    /**
     * @brief Get the type of the element
     * @return ddl::codec::ElementType
     */
    ddl::codec::ElementType getType() const
    {
        return _codec_index.getType();
    }
    /**
     * @brief Get the full name of the element.
     * If the element is an array you get the elements array name representation:
     * "main_element.element_name[0]" or main_element.element_name[1] ... etc. If the element is not
     * an array you get the elements name representation: "main_element.element_name".
     * @return std::string
     */
    std::string getFullName() const
    {
        return ElementAccessType::getFullName(_access, _codec_index);
    }
    /**
     * @brief Get the name of the element
     * If the element is an array you get the elements array name representation: "element_name[0]"
     * or element_name[1] ... etc. If the element is not an array you get the elements name
     * representation: "element_name".
     * @return std::string
     */
    std::string getName() const
    {
        return ElementAccessType::getName(_access, _codec_index);
    }
    /**
     * @brief Get the base name of the element.
     * If the element is an array you get the elements base name representation : "element_name"
     * for each array element. If the element is not an array you get the elements name
     * representation is also the base name representation: "element_name" .
     * @return std::string
     */
    std::string getBaseName() const
    {
        return ElementAccessType::getBaseName(_access, _codec_index);
    }
    /**
     * @brief Get array information.
     * @return true is an array
     * @return false is no array
     */
    bool isArray() const
    {
        return _codec_index.getLayout().array_size > 1;
    }
    /**
     * @brief Get children information.
     * @return true has children
     * @return false has no children
     */
    bool hasChildren() const
    {
        return _child_elements.size() > 0;
    }
    /**
     * @brief Get the ChildElements.
     * @return child_elements_type
     * @throw std::runtime_error if index is invalid
     */
    const child_elements_type& getChildElements() const
    {
        return _child_elements;
    }
    /**
     * @brief Get the child element with the given name.
     * @param name Name of the child element.
     * @return self_type
     */
    self_type getChildElement(const std::string& name) const
    {
        auto full_name = getFullName();
        full_name.append(".");
        full_name.append(name);
        return self_type(full_name, getAccess());
    }
    /**
     * @brief Get the array element of the given @p array_pos
     * @param array_pos The array pos of the element to retrieve
     * @return self_type
     */
    self_type getArrayElement(size_t array_pos = 0) const
    {
        return self_type(_codec_index.getIndexForArrayPos(array_pos), getAccess());
    }
    /**
     * @brief Get validation indormation
     * @return true is valid
     * @return false is not valid
     */
    bool isValid() const
    {
        return _codec_index != CodecIndex();
    }

private:
    FactoryElement(CodecIndex&& index,
                   size_t end_element_index,
                   size_t child_elements_count,
                   access_type& access)
        : _codec_index(std::move(index)),
          _end_element_index(end_element_index),
          _child_elements(_codec_index, child_elements_count, access),
          _access(access)
    {
    }
    FactoryElement(CodecIndex&& index, size_t end_element_index, access_type& access)
        : _codec_index(std::move(index)),
          _end_element_index(end_element_index),
          _child_elements(
              _codec_index, ElementAccessType::getChildCount(access, _codec_index), access),
          _access(access)
    {
    }
    FactoryElement(const std::string& element_name, access_type& access)
        : _codec_index(std::move(ElementAccessType::getElement(access, element_name)._codec_index)),
          _end_element_index(CodecIndex::ElementIndex::_invalid_element_index),
          _child_elements(_codec_index, access),
          _access(access)
    {
    }

    FactoryElement(CodecIndex&& index, access_type& access)
        : _codec_index(std::move(index)),
          _end_element_index(CodecIndex::ElementIndex::_invalid_element_index),
          _child_elements(_codec_index, access),
          _access(access)
    {
    }
    FactoryElement(access_type& access)
        : _codec_index(CodecIndex()),
          _end_element_index(CodecIndex::ElementIndex::_invalid_element_index),
          _child_elements(_codec_index, 0, access),
          _access(access)
    {
    }
    void next()
    {
        auto break_increasing = false;
        auto index_valid = false;
        while (!break_increasing) {
            ++_codec_index;
            const auto element_index = _codec_index.back();
            if (element_index.getIndex() != CodecIndex::ElementIndex::_invalid_element_index &&
                element_index.getIndex() < _end_element_index) {
                ElementAccessType::resolve(_access, _codec_index);
                if (_codec_index.getLayout().array_size != 0) {
                    break_increasing = true;
                    index_valid = true;
                }
            }
            else {
                break_increasing = true;
            }
        }
        if (index_valid) {
            _child_elements.resetChildCount(_codec_index.getLayout().child_element_count);
        }
    }
    access_type& getAccess() const
    {
        return _access;
    }

    void resetIndex(CodecIndex&& codec_index)
    {
        _codec_index = std::move(codec_index);
        _child_elements.resetChildCount();
    }
    CodecIndex _codec_index;
    size_t _end_element_index;
    child_elements_type _child_elements;
    access_type& _access;
};

/**
 * @brief A element access type concept template to retrieve element information from the @p
 * AccessType and get the element value. This class is to retrieve common information of the element
 * and get the element value content.
 * @see @ref DecoderElement.
 *
 * @tparam AccessType The AccessType for the element (Codec, Decoder, etc. )
 */
template <typename AccessType>
class DecoderElementAccess : public FactoryElementAccess<AccessType> {
public:
    using access_type = AccessType;                     //!< access type in use
    using self_type = DecoderElementAccess<AccessType>; //!< self type
    /// supported element type
    using element_type = DecoderElement<self_type, ChildElements<self_type>>;

    /**
     * @brief Get a element object for the given @p full_element_name.
     *
     * @param access defined access type.
     * @param full_element_name The full name of the element in point notation (i.e. \p
     * "element1.child_element[4].element_value").
     * @return element_type
     */
    static element_type getElement(access_type& access, const std::string& full_element_name)
    {
        return access.getElement(full_element_name);
    };
    /**
     * @brief Get a element object for the given @p index.
     *
     * @param access defined access type.
     * @param index The codec index.
     * @return element_type
     */
    static element_type getElement(access_type& access, const CodecIndex& index)
    {
        return access.getElement(index);
    };
    /**
     * @brief Get the value as type T
     *
     * @param access defined access type.
     * @param index The codec index.
     * @tparam T Type of the value to return
     * @return value in type T (supported types: all int types, bool, float, double, std::string)
     * @remark for T = std::string the value is returned as element name in case of enum type
     * @see @ref StaticCodec::getElementValue
     */
    template <typename T>
    static T getValue(access_type& access, const CodecIndex& index)
    {
        return access.template getElementValue<T>(index);
    }
    /**
     * @brief Get the value as variant
     *
     * @param access defined access type.
     * @param index The codec index.
     * @return value as variant converted from elements type
     * @see @ref StaticDecoder::getElementVariantValue
     */
    static a_util::variant::Variant getVariantValue(access_type& access, const CodecIndex& index)
    {
        return access.getElementVariantValue(index);
    };
    /**
     * @brief Get the value as type string
     *
     * @param access defined access type.
     * @param index The codec index.
     * @return value as string, it is set to the enums element name in case of enum type
     * @see @ref StaticDecoder::getElementStringValue
     */
    static std::string getStringValue(access_type& access, const CodecIndex& index)
    {
        return access.getElementStringValue(index);
    };
    /**
     * @brief Get the value by copy to the given @p value buffer
     *
     * @param access defined access type.
     * @param index The codec index.
     * @param value the value buffer pointer.
     * @param value_size the size of the value buffer in bytes.
     */
    static void getRawValue(access_type& access,
                            const CodecIndex& index,
                            void* value,
                            size_t value_size)
    {
        access.getElementRawValue(index, value, value_size);
    };
    /**
     * @brief Get the address of the element
     *
     * @param access defined access type.
     * @param index The codec index.
     * @return const void*
     */
    static const void* getAddress(access_type& access, const CodecIndex& index)
    {
        return access.getElementAddress(index);
    };
};

/**
 * @brief A DecoderElement to get values.
 * @tparam ElementAccessType The element access concept type.
 *                           (see concept templates @ref DecoderElementAccess, @ref
 * CodecElementAccess)
 */
template <typename ElementAccessType, typename ChildElementsType = ChildElements<ElementAccessType>>
class DecoderElement : public FactoryElement<ElementAccessType, ChildElementsType> {
public:
    using base_type = FactoryElement<ElementAccessType, ChildElementsType>; //!< base type
    using self_type = DecoderElement<ElementAccessType, ChildElementsType>; //!< self type
    /// access type from concept template
    using access_type = typename ElementAccessType::access_type;
    using child_elements_type = typename base_type::child_elements_type; //!< allowed container type
    using iterator_type = typename child_elements_type::iterator;        //!< allowed iterator type
    /// allowed const iterator type
    using const_iterator_type = typename child_elements_type::const_iterator;

    /**
     * @brief friend declaration for child_elements_type
     */
    friend child_elements_type;
    /**
     * @brief friend declaration for iterator_type
     */
    friend iterator_type;
    /**
     * @brief friend declaration for const_iterator_type
     */
    friend const_iterator_type;
    /**
     * @brief friend declaration for access_type
     */
    friend access_type;
    /**
     * @brief friend declaration for base_type
     */
    friend base_type;
    /**
     * @brief friend declaration for CodecElement
     */
    friend CodecElement<ElementAccessType, ChildElementsType>;

    /**
     * @brief CTOR
     */
    DecoderElement() = delete;
    /**
     * @brief Get the value as type T
     * @tparam T Type of the value to return
     * @return value in type T (supported types: all int types, bool, float, double, std::string)
     * @remark for T = std::string the value is returned as element name in case of enum type
     * @see @ref StaticCodec::getElementValue
     */
    template <typename T>
    T getValue() const
    {
        return ElementAccessType::template getValue<T>(base_type::getAccess(),
                                                       base_type::getIndex());
    }
    /**
     * @brief Get the value as variant
     * @return value as variant (supported types: all int types, bool, float, double, std::string)
     */
    a_util::variant::Variant getVariantValue() const
    {
        return ElementAccessType::getVariantValue(base_type::getAccess(), base_type::getIndex());
    };
    /**
     * @brief Get the value as string
     * @return value as string (the value is returned as enum element name in case of enum type)
     */
    std::string getStringValue() const
    {
        return ElementAccessType::getStringValue(base_type::getAccess(), base_type::getIndex());
    };
    /**
     * @brief Get the as copy to the @p value buffer
     * @param value The value buffer to copy to
     * @param value_size The size of the value buffer (if 0 its unsafe usage without size check!,
     *                   the size is only retrieved from the codec information)
     */
    void getRawValue(void* value, size_t value_size) const
    {
        ElementAccessType::getRawValue(
            base_type::getAccess(), base_type::getIndex(), value, value_size);
    }
    /**
     * @brief Get the address of the element.
     * @return const void*
     */
    const void* getAddress() const
    {
        return ElementAccessType::getAddress(base_type::getAccess(), base_type::getIndex());
    }
    /**
     * @brief Get the child element with the given name.
     * @param name Name of the child element.
     * @return self_type
     */
    self_type getChildElement(const std::string& name) const
    {
        auto full_name = base_type::getFullName();
        full_name.append(".");
        full_name.append(name);
        return self_type(full_name, base_type::getAccess());
    }
    /**
     * @brief Get the array element of the given @p array_pos
     * @param array_pos The array pos of the element to retrieve
     * @return self_type
     */
    self_type getArrayElement(size_t array_pos = 0) const
    {
        return self_type(base_type::getIndex().getIndexForArrayPos(array_pos),
                         base_type::getAccess());
    }

private:
    DecoderElement(CodecIndex&& index,
                   size_t end_element_index,
                   size_t child_elements_count,
                   access_type& access)
        : base_type(std::move(index), end_element_index, child_elements_count, access)
    {
    }
    DecoderElement(CodecIndex&& index, size_t end_element_index, access_type& access)
        : base_type(std::move(index), end_element_index, access)
    {
    }
    DecoderElement(const std::string& element_name, access_type& access)
        : base_type(element_name, access)
    {
    }
    DecoderElement(CodecIndex&& index, access_type& access) : base_type(std::move(index), access)
    {
    }
    DecoderElement(access_type& access) : base_type(access)
    {
    }
    void next()
    {
        base_type::next();
    }
};

/**
 * @brief A element access type concept template to retrieve element information from the @p
 * AccessType, get and set the element value. This class is to retrieve common information of the
 * element, get and set the element value content.
 * @see @ref CodecElement.
 *
 * @tparam AccessType The AccessType for the element (Codec, StaticCodec, etc. )
 */
template <typename AccessType>
class CodecElementAccess : public DecoderElementAccess<AccessType> {
public:
    using access_type = AccessType;                   //!< access type
    using self_type = CodecElementAccess<AccessType>; //!< self type
    /// supported element type
    using element_type = CodecElement<self_type, ChildElements<self_type>>;

    /**
     * @brief Get a element object for the given @p full_element_name.
     *
     * @param access defined access type.
     * @param full_element_name The full name of the element in point notation (i.e. \p
     * "element1.child_element[4].element_value").
     * @return element_type
     */
    static element_type getElement(access_type& access, const std::string& full_element_name)
    {
        return access.getElement(full_element_name);
    };
    /**
     * @brief Get a element object for the given @p index.
     *
     * @param access defined access type.
     * @param index The codec index.
     * @return element_type
     */
    static element_type getElement(access_type& access, const CodecIndex& index)
    {
        return access.getElement(index);
    };
    /**
     * @brief Sets the value from \p value of type \p T
     * @tparam T Type of the \p value
     * @param[in] access The access type
     * @param[in] index The codec index of the value
     * @param[in] value The value in type \p T
     * @remark for T = std::string the value can be set as valid element name in case of enum type
     * otherwise the string is converted
     * @see @ref StaticCodec::setElementValue
     */
    template <typename T>
    static void setValue(access_type& access, const CodecIndex& index, const T& value)
    {
        access.template setElementValue<T>(index, value);
    }
    /**
     * @brief Sets the value from \p value as variant
     * @param[in] access The access type
     * @param[in] index The codec index of the value
     * @param[in] value The value to set as variant
     * @see @ref StaticCodec::setElementVariantValue
     */
    static void setVariantValue(access_type& access,
                                const CodecIndex& index,
                                const a_util::variant::Variant& value)
    {
        access.setElementVariantValue(index, value);
    };
    /**
     * @brief Sets the value from \p value as string
     * @param[in] access The access type
     * @param[in] index The codec index of the value
     * @param[in] value The value to set as string. string will be converted to the elements type.
     * @see @ref StaticCodec::setElementStringValue
     */
    static void setStringValue(access_type& access,
                               const CodecIndex& index,
                               const std::string& value)
    {
        access.setElementStringValue(index, value);
    };
    /**
     * @brief Set the value by copying from a @p value buffer
     * @param access defined access type.
     * @param index The codec index.
     * @param value The value buffer
     * @param value_size the size of the value buffer in bytes.
     * @see @ref StaticCodec::setElementRawValue
     */
    static void setRawValue(access_type& access,
                            const CodecIndex& index,
                            const void* value,
                            size_t value_size)
    {
        access.setElementRawValue(index, value, value_size);
    };
    /**
     * @brief Get the address (with write access)
     *
     * @param access defined access type.
     * @param index The codec index.
     * @return void*
     */
    static void* getAddress(access_type& access, const CodecIndex& index)
    {
        return access.getElementAddress(index);
    };
};

/**
 * @brief A CodecElement to get and set values.
 * @tparam ElementAccessType The element access concept type.
 *                           (see concept templates @ref CodecElementAccess)
 */
template <typename ElementAccessType, typename ChildElementsType = ChildElements<ElementAccessType>>
class CodecElement : public DecoderElement<ElementAccessType, ChildElementsType> {
public:
    /// factory base type
    using factory_base_type = FactoryElement<ElementAccessType, ChildElementsType>;
    using base_type = DecoderElement<ElementAccessType, ChildElementsType>; //!< base type
    using self_type = CodecElement<ElementAccessType, ChildElementsType>;   //!< self type
    using access_type = typename ElementAccessType::access_type; //!< access type from the concept
    /// allowed container type
    using child_elements_type = typename factory_base_type::child_elements_type;
    using iterator_type = typename child_elements_type::iterator; //!< allowed iterator type
    /// allowed const iterator type
    using const_iterator_type = typename child_elements_type::const_iterator;

    /**
     * @brief friend declaration for child_elements_type
     */
    friend child_elements_type;
    /**
     * @brief friend declaration for iterator_type
     */
    friend iterator_type;
    /**
     * @brief friend declaration for const_iterator_type
     */
    friend const_iterator_type;
    /**
     * @brief friend declaration for access_type
     */
    friend access_type;
    /**
     * @brief friend declaration for base_type
     */
    friend base_type;
    /**
     * @brief friend declaration for factory_base_type
     */
    friend factory_base_type;
    /**
     * @brief CTOR
     */
    CodecElement() = delete;
    /**
     * @brief Sets the value from \p value of type \p T
     * @tparam T Type of the \p value
     * @param[in] value The value in type \p T
     * @remark for T = std::string the value can be set as valid element name in case of enum type
     * otherwise the string is converted
     * @see @ref StaticCodec::setElementValue
     */
    template <typename T>
    void setValue(const T& value)
    {
        return ElementAccessType::template setValue<T>(
            base_type::getAccess(), base_type::getIndex(), value);
    }
    /**
     * @brief Sets the value from \p value as variant
     * @param[in] value The value to set as variant
     * @see @ref ddl::codec::StaticCodec::setElementVariantValue
     */
    void setVariantValue(const a_util::variant::Variant& value)
    {
        return ElementAccessType::setVariantValue(
            base_type::getAccess(), base_type::getIndex(), value);
    };
    /**
     * @brief Sets the value from \p value as string
     * @param[in] value The value to set as string. string will be converted to the elements type.
     * @see @ref ddl::codec::StaticCodec::setElementStringValue
     */
    void setStringValue(const std::string& value)
    {
        return ElementAccessType::setStringValue(
            base_type::getAccess(), base_type::getIndex(), value);
    };
    /**
     * @brief Set the value by copying from a @p value buffer
     * @param[in] value The value buffer
     * @param[in] value_size the size of the value buffer in bytes.
     * @see @ref ddl::codec::StaticCodec::setElementRawValue
     */
    void setRawValue(const void* value, size_t value_size)
    {
        return ElementAccessType::setRawValue(
            base_type::getAccess(), base_type::getIndex(), value, value_size);
    };

    /**
     * @brief Reset the elements value to their default values, constant values defined in the data
     * definition or zero if \p zero_value is set.
     * @param[in] zero_value_of_known_type set the value to 0 if no constant or default value set in
     * data definition.
     */
    void reset(bool zero_value_of_known_type = false)
    {
        const auto& layout = base_type::getIndex().getLayout();
        if (layout.constant_info->isConstant()) {
            return ElementAccessType::setVariantValue(base_type::getAccess(),
                                                      base_type::getIndex(),
                                                      layout.constant_info->getConstantValue());
        }
        else if (layout.default_value_info->hasDefaultValue()) {
            return ElementAccessType::setVariantValue(base_type::getAccess(),
                                                      base_type::getIndex(),
                                                      layout.default_value_info->getDefaultValue());
        }
        else {
            // we can only reset known types
            if (zero_value_of_known_type) {
                auto known_type = base_type::getType();
                if (known_type != ElementType::cet_empty &&
                    known_type != ElementType::cet_sub_codec &&
                    known_type != ElementType::cet_user_type) {
                    uint64_t null_value = 0;
                    return ElementAccessType::setRawValue(base_type::getAccess(),
                                                          base_type::getIndex(),
                                                          &null_value,
                                                          sizeof(null_value));
                }
            }
        }
    }

    using base_type::getAddress;
    /**
     * @brief Get the address (with writing access)
     * @return void*
     */
    void* getAddress()
    {
        return ElementAccessType::getAddress(base_type::getAccess(), base_type::getIndex());
    }

    using factory_base_type::getChildElements;
    /**
     * @brief Gets an reference to the child elements
     *
     * @return child_elements_type&
     */
    child_elements_type& getChildElements()
    {
        return factory_base_type::_child_elements;
    }
    /**
     * @brief Get a dedicated ChildElement by name
     * @param name The name if point notation.
     * @return self_type
     * @throw std::runtime_error if name is not valid or not found
     */
    self_type getChildElement(const std::string& name) const
    {
        auto full_name = base_type::getFullName();
        full_name.append(".");
        full_name.append(name);
        return self_type(full_name, base_type::getAccess());
    }

    /**
     * @brief Get a dedicated ChildElement by name
     * @param name The name if point notation.
     * @return self_type
     * @throw std::runtime_error if name is not valid or not found
     */
    self_type getChildElement(const std::string& name)
    {
        auto full_name = base_type::getFullName();
        full_name.append(".");
        full_name.append(name);
        return self_type(full_name, base_type::getAccess());
    }
    /**
     * @brief Get the array element of the given @p array_pos
     * @param array_pos The array pos of the element to retrieve
     * @return self_type
     */
    self_type getArrayElement(size_t array_pos = 0) const
    {
        return self_type(base_type::getIndex().getIndexForArrayPos(array_pos),
                         base_type::getAccess());
    }

    /**
     * @brief Get the array element of the given @p array_pos
     * @param array_pos The array pos of the element to retrieve
     * @return self_type
     */
    self_type getArrayElement(size_t array_pos = 0)
    {
        return self_type(base_type::getIndex().getIndexForArrayPos(array_pos),
                         base_type::getAccess());
    }

private:
    CodecElement(CodecIndex&& index,
                 size_t end_element_index,
                 size_t child_elements_count,
                 access_type& access)
        : base_type(std::move(index), end_element_index, child_elements_count, access)
    {
    }
    CodecElement(CodecIndex&& index, size_t end_element_index, access_type& access)
        : base_type(std::move(index), end_element_index, access)
    {
    }
    CodecElement(const std::string& element_name, access_type& access)
        : base_type(element_name, access)
    {
    }
    CodecElement(CodecIndex&& index, access_type& access) : base_type(std::move(index), access)
    {
    }
    CodecElement(access_type& access) : base_type(access)
    {
    }
    void next()
    {
        base_type::next();
    }
};

/**
 * @brief Iterates ALL leaf elements within ALL array elements.
 *
 * Usage:
 * @code {.cpp}
 * //description contains a data description where "my_struct" is defined
 * struct my_sub_struct
 * {
 *     uint32_t sub_element[3];
 *     uint32_t sub_element_after;
 * }
 * struct my_struct
 * {
 *     my_sub_struct element[2];
 * }
 * CodecFactory factory("my_struct", strDescription);
 * forEachLeafElement(factory.getElements(),
 *                    [](const auto& element) {
 *                            std::cout << element.getFullName() << std::endl;
 *                       });
 * //following output:
 * //"element[0].sub_element[0]"
 * //"element[0].sub_element[1]"
 * //"element[0].sub_element[2]"
 * //"element[0].sub_element_after"
 * //"element[1].sub_element[0]"
 * //"element[1].sub_element[1]"
 * //"element[1].sub_element[2]"
 * //"element[1].sub_element_after"
 * @endcode
 *
 * @tparam ElementsType The type of elements.
 * @param elements The elements
 * @param func The function to call per leaf element
 * @see @ref CodecFactory::getElements, @ref Codec::getElements, @ref Decoder::getElements,
 *      @ref StaticCodec::getElements, @ref StaticDecoder::getElements
 */
template <typename ElementsType>
void forEachLeafElement(
    ElementsType& elements,
    const std::function<void(std::conditional_t<std::is_const<ElementsType>::value,
                                                const typename ElementsType::element_type,
                                                typename ElementsType::element_type>&)>& func)
{
    for (auto& element: elements) {
        if (element.hasChildren()) {
            if (element.isArray()) {
                for (size_t array_pos = 0; array_pos < element.getArraySize(); ++array_pos) {
                    auto array_element = element.getArrayElement(array_pos);
                    auto& children = array_element.getChildElements();
                    forEachLeafElement(children, func);
                }
            }
            else {
                auto& children = element.getChildElements();
                forEachLeafElement(children, func);
            }
        }
        else {
            if (element.isArray()) {
                for (size_t array_pos = 0; array_pos < element.getArraySize(); ++array_pos) {
                    auto array_element = element.getArrayElement(array_pos);
                    func(array_element);
                }
            }
            else {
                func(element);
            }
        }
    }
}

/**
 * @brief Iterates elements without array elements (also structures).
 *
 * Usage:
 * @code {.cpp}
 * //description contains a data description where "my_struct" is defined
 * struct my_sub_struct
 * {
 *     uint32_t sub_element[3];
 *     uint32_t sub_element_after;
 * }
 * struct my_struct
 * {
 *     my_sub_struct element[2];
 * }
 * CodecFactory factory("my_struct", description);
 * forEachElement(factory.getElements(),
 *                [](const auto& element) {
 *                       std::cout << element.getFullName() << std::endl;
 *                       std::cout << element.getName() << std::endl;
 *                       std::cout << element.getBaseName() << std::endl;
 *                       std::cout << element.getArraySize() << std::endl;
 *                  });
 * //following output:
 * //"element[0]"
 * //"element[0]"
 * //"element"
 * //"2"
 * //"element[0].sub_element[0]"
 * //"sub_element[0]"
 * //"sub_element"
 * //"3"
 * //"element[0].sub_element_after"
 * //"sub_element_after"
 * //"sub_element_after"
 * //"1"
 * @endcode
 *
 * @tparam ElementsType The type of elements.
 * @param elements The elements
 * @param func The function to call per leaf element
 * @see @ref CodecFactory::getElements, @ref Codec::getElements, @ref Decoder::getElements,
 *      @ref StaticCodec::getElements, @ref StaticDecoder::getElements
 */
template <typename ElementsType>
void forEachElement(
    ElementsType& elements,
    const std::function<void(std::conditional_t<std::is_const<ElementsType>::value,
                                                const typename ElementsType::element_type,
                                                typename ElementsType::element_type>&)>& func)
{
    for (auto& element: elements) {
        func(element);
        if (element.hasChildren()) {
            auto& children = element.getChildElements();
            forEachElement(children, func);
        }
    }
}

} // namespace codec
} // namespace ddl

#endif // DDL_CODEC_ITERATOR_CLASS_HEADER
