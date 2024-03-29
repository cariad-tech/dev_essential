/**
 * @file
 * Implementation of the serialization via ddl.
 * Copyright @ 2021 VW Group. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <a_util/memory.h>
#include <a_util/result/error_def.h>
#include <ddl/serialization/serialization.h>

namespace ddl {
namespace codec {
// define all needed error types and values locally
_MAKE_RESULT(-12, ERR_MEMORY);

a_util::result::Result transformToBuffer(const codec::Decoder& decoder,
                                         a_util::memory::MemoryBuffer& buffer,
                                         bool zero)
{
    DataRepresentation target_rep =
        decoder.getRepresentation() == deserialized ? serialized : deserialized;
    size_t needed_size = decoder.getBufferSize(target_rep);
    if (buffer.getSize() < needed_size) {
        if (!buffer.allocate(needed_size)) {
            return ERR_MEMORY;
        }
    }

    if (zero) {
        a_util::memory::set(buffer.getPtr(), buffer.getSize(), 0, buffer.getSize());
    }
    codec::Codec codec = decoder.makeCodecFor(buffer.getPtr(), buffer.getSize(), target_rep);
    return transform(decoder, codec);
}

} // namespace codec

} // namespace ddl
