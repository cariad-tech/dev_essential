/**
 * @file
 * Named Codec Index
 *
 * Copyright @ 2022 VW Group. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef DDL_PREVIEW_NAMED_CODEC_INDEX_CLASS_HEADER
#define DDL_PREVIEW_NAMED_CODEC_INDEX_CLASS_HEADER

#include <ddl/codec/codec_index.h>

#include <string>
#include <vector>

namespace ddl {
namespace codec {

class StructAccess;
class NamedCodecIndex {
public:
    struct NamedElementIndex {
        NamedElementIndex(const std::string& access_name);
        NamedElementIndex(std::string name, size_t pos);

        bool operator==(const NamedElementIndex& other) const;
        bool operator!=(const NamedElementIndex& other) const;

        size_t getArrayPos() const;
        const std::string& getName() const;
        bool resolvedWithArrayPos() const;

    private:
        size_t _array_pos = CodecIndex::ElementIndex::_invalid_array_pos;
        std::string _name = {};
        bool _resolved_with_array_pos = false;
    };

    NamedCodecIndex() = default;

    NamedCodecIndex(const NamedCodecIndex& codec_index, NamedCodecIndex::NamedElementIndex index);
    NamedCodecIndex(const NamedCodecIndex& codec_index,
                    size_t count,
                    NamedCodecIndex::NamedElementIndex index);
    NamedCodecIndex(const std::string& element_name);
    NamedCodecIndex(const std::vector<std::string>& element_name);

    const NamedElementIndex& operator[](size_t pos) const;
    size_t getSize() const;
    bool lastResolvedWithArrayPos() const;

private:
    friend class StructAccess;
    std::vector<NamedElementIndex> _indices = {};
};

std::string toString(const NamedCodecIndex& index);

} // namespace codec
} // namespace ddl

#endif // DDL_PREVIEW_NAMED_CODEC_INDEX_CLASS_HEADER
