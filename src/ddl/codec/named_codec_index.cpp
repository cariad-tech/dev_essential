/**
 * @file
 * Implementation of Named Coding index parser
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

#include "named_codec_index.h"

#include <a_util/strings/strings_convert.h>
#include <a_util/strings/strings_functions.h>
#include <ddl/utilities/std_to_string.h>

#include <algorithm>
#include <iterator>
#include <stdexcept>

namespace ddl {
namespace codec {

NamedCodecIndex::NamedElementIndex::NamedElementIndex(const std::string& access_name)
    : _array_pos(0), _name(std::string())
{
    if (access_name.empty()) {
        throw std::runtime_error("CodecIndex: name parsing error : name is empty");
    }
    auto array_begin_pos = access_name.find_first_of('[');
    if (array_begin_pos != std::string::npos) {
        auto array_end_pos = access_name.length() - 1;
        auto array_end_char = access_name[array_end_pos];
        if (array_end_char == ']' && array_end_pos > array_begin_pos) {
            _array_pos = a_util::strings::toUInt64(
                access_name.substr(array_begin_pos + 1, array_end_pos - array_begin_pos - 1));
            _name = access_name.substr(0, array_begin_pos);
            _resolved_with_array_pos = true;
        }
        else {
            throw std::runtime_error("NamedCodecIndex: array name parsing error for name : " +
                                     access_name);
        }
    }
    else {
        _name = access_name;
    }
}

NamedCodecIndex::NamedElementIndex::NamedElementIndex(std::string name, size_t pos)
    : _array_pos(pos), _name(std::move(name))
{
}

bool NamedCodecIndex::NamedElementIndex::NamedElementIndex::operator==(
    const NamedElementIndex& other) const
{
    return (other._name == _name && other._array_pos == _array_pos);
}

bool NamedCodecIndex::NamedElementIndex::NamedElementIndex::operator!=(
    const NamedElementIndex& other) const
{
    return !operator==(other);
}

size_t NamedCodecIndex::NamedElementIndex::getArrayPos() const
{
    return _array_pos;
}

const std::string& NamedCodecIndex::NamedElementIndex::getName() const
{
    return _name;
}

bool NamedCodecIndex::NamedElementIndex::resolvedWithArrayPos() const
{
    return _resolved_with_array_pos;
}

NamedCodecIndex::NamedCodecIndex(const std::vector<std::string>& element_name)
{
    for (const auto& value: element_name) {
        _indices.emplace_back(value);
    }
}

NamedCodecIndex::NamedCodecIndex(const std::string& element_name)
    : NamedCodecIndex(a_util::strings::split(element_name, ".", false))
{
}

NamedCodecIndex::NamedCodecIndex(const NamedCodecIndex& codec_index, NamedElementIndex index)
    : _indices(codec_index._indices)
{
    _indices.emplace_back(std::move(index));
}

NamedCodecIndex::NamedCodecIndex(const NamedCodecIndex& index,
                                 size_t count,
                                 NamedElementIndex element_index)
{
    const auto copy_size = std::min(count, index._indices.size());
    _indices.reserve(copy_size + 1);
    std::copy(
        index._indices.begin(), index._indices.begin() + copy_size, std::back_inserter(_indices));
    _indices.emplace_back(std::move(element_index));
}

const NamedCodecIndex::NamedElementIndex& NamedCodecIndex::operator[](size_t pos) const
{
    return _indices[pos];
}

size_t NamedCodecIndex::getSize() const
{
    return _indices.size();
}

bool NamedCodecIndex::lastResolvedWithArrayPos() const
{
    if (getSize() > 0) {
        return _indices.back().resolvedWithArrayPos();
    }
    return false;
}

// for good error messages
std::string toString(const NamedCodecIndex::NamedElementIndex& element_index)
{
    if (element_index.getArrayPos() > 0) {
        return std::string(element_index.getName() + "[" +
                           std::to_string(element_index.getArrayPos()) + "]");
    }
    else {
        return element_index.getName();
    }
} // namespace

std::string toString(const NamedCodecIndex& index)
{
    std::string result;
    for (size_t index_pos = 0; index_pos < index.getSize(); ++index_pos) {
        if (index_pos != 0) {
            result += ".";
        }
        result += toString(index[index_pos]);
    }
    return result;
}

} // namespace codec
} // namespace ddl
