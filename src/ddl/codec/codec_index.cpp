/**
 * @file
 * Implementation of CodecIndex class.
 *
 * @copyright
 * @verbatim
Copyright @ 2022 VW Group. All rights reserved.

    This Source Code Form is subject to the terms of the Mozilla
    Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

If it is not possible or desirable to put the notice in a particular file, then
You may include the notice in a location (such as a LICENSE file in a
relevant directory) where a recipient would be likely to look for such a notice.

You may add additional accurate notices of copyright ownership.
@endverbatim
 */

#include <ddl/codec/codec_index.h>
#include <ddl/utilities/std_to_string.h>

#include <algorithm>
#include <iterator>
#include <stdexcept>

namespace ddl {
namespace codec {

CodecIndex::ElementIndex::ElementIndex(size_t element_index)
    : _element_index(element_index), _array_pos(0)
{
}

CodecIndex::ElementIndex::ElementIndex(size_t element_index, size_t array_pos)
    : _element_index(element_index), _array_pos(array_pos)
{
}

bool CodecIndex::ElementIndex::operator==(const ElementIndex& other) const
{
    return (other._element_index == _element_index && other._array_pos == _array_pos);
}

bool CodecIndex::ElementIndex::operator!=(const ElementIndex& other) const
{
    return !operator==(other);
}

size_t CodecIndex::ElementIndex::getIndex() const
{
    return _element_index;
}

size_t CodecIndex::ElementIndex::getArrayPos() const
{
    return _array_pos;
}

CodecIndex::ElementIndex& CodecIndex::ElementIndex::operator++()
{
    if (_element_index != _invalid_element_index) {
        ++_element_index;
        return *this;
    }
    throw std::runtime_error("can not increment invalid index");
}

CodecIndex::CodecIndex(ElementIndex index) : _indices({{std::move(index)}})
{
}

CodecIndex::CodecIndex(const std::vector<ElementIndex>& indices) : _indices(indices)
{
}

CodecIndex::CodecIndex(const CodecIndex& index, size_t count, ElementIndex element_index)
{
    std::copy(index._indices.begin(),
              index._indices.begin() + std::min(count, index._indices.size()),
              std::back_inserter(_indices));
    _indices.emplace_back(std::move(element_index));
}

CodecIndex::ElementIndex CodecIndex::back() const noexcept
{
    if (_indices.empty()) {
        return CodecIndex::ElementIndex();
    }
    return _indices.back();
}

CodecIndex CodecIndex::getIndexForArrayPos(size_t array_pos) const
{
    const auto elem_index = back();
    const auto old_array_pos = elem_index.getArrayPos();
    if (old_array_pos == array_pos) {
        return *this;
    }
    else if (hasLayout() && _indices.size() > 0 && array_pos < _layout.array_size) {
        CodecIndex new_index = *this;

        // update array pos
        new_index._indices[new_index.getSize() - 1] =
            ElementIndex(_indices[new_index.getSize() - 1].getIndex(), array_pos);
        new_index._layout.array_pos = array_pos;

        // calculate the bit pos
        new_index._layout.deserialized.bit_offset =
            _layout.deserialized.bit_offset +
            ((array_pos - old_array_pos) * (_layout.deserialized.type_bit_size_aligned));
        new_index._layout.serialized.bit_offset =
            _layout.serialized.bit_offset +
            ((array_pos - old_array_pos) * (_layout.serialized.type_bit_size));
        return new_index;
    }
    throw std::runtime_error("invalid array pos request " + std::to_string(array_pos) +
                             " for CodecIndex " + toString(*this));
}

CodecIndex::CodecIndex(const CodecIndex& codec_index, ElementIndex index)
    : _indices(codec_index._indices)
{
    _indices.emplace_back(std::move(index));
}

CodecIndex::CodecIndex(ElementIndex index, const ElementLayout& layout) : CodecIndex(index)
{
    setLayout(layout);
}

CodecIndex::CodecIndex(const CodecIndex& codec_index,
                       size_t count,
                       ElementIndex index,
                       const ElementLayout& layout)
    : CodecIndex(codec_index, count, index)
{
    setLayout(layout);
}

size_t CodecIndex::getSize() const
{
    return _indices.size();
}

size_t CodecIndex::size() const
{
    return _indices.size();
}

const CodecIndex::ElementIndex& CodecIndex::operator[](size_t pos) const
{
    return _indices[pos];
}

bool CodecIndex::operator==(const CodecIndex& other) const
{
    if (other.getSize() == getSize()) {
        for (size_t current_pos = 0; current_pos < getSize(); ++current_pos) {
            if (operator[](current_pos) != other[current_pos]) {
                return false;
            }
        }
        return true;
    }
    else {
        return false;
    }
}

bool CodecIndex::operator!=(const CodecIndex& other) const
{
    return !operator==(other);
}

bool CodecIndex::hasLayout() const
{
    return _layout_valid;
}

ElementType CodecIndex::getType() const
{
    return getLayout().type_info->getType();
}

const ElementLayout& CodecIndex::getLayout() const
{
    if (!_layout_valid) {
        throw std::runtime_error("CodecIndex has no valid layout");
    }
    return _layout;
}

CodecIndex& CodecIndex::operator++()
{
    if (!_indices.empty()) {
        ++_indices[_indices.size() - 1];
        _layout_valid = false;
    }
    return *this;
}

CodecIndex CodecIndex::operator++(int)
{
    auto old_value = *this;
    ++*this;
    return old_value;
}

void CodecIndex::setLayout(const ElementLayout& layout)
{
    _layout_valid = true;
    _layout = layout;
}

void CodecIndex::addElementIndex(ElementIndex index)
{
    _indices.emplace_back(std::move(index));
}

namespace {
// for good error messages
std::string toString(const CodecIndex::ElementIndex& element_index)
{
    return std::string("{" + std::to_string(element_index.getIndex()) + ", " +
                       std::to_string(element_index.getArrayPos()) + "}");
}
} // namespace

std::string toString(const CodecIndex& index)
{
    std::string result = "{";
    for (size_t index_pos = 0; index_pos < index.getSize(); ++index_pos) {
        if (index_pos != 0) {
            result += ", ";
        }
        result += toString(index[index_pos]);
    }
    result += "}";
    return result;
}

} // namespace codec
} // namespace ddl
