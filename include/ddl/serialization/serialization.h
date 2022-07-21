/**
 * @file
 * Implementation of ddl base serialization.
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

#ifndef DDL_SERIALIZER_CLASS_HEADER
#define DDL_SERIALIZER_CLASS_HEADER

#include <a_util/result.h>
#include <ddl/codec/codec.h>

#include <assert.h>

namespace ddl {
namespace codec {

/**
 * @brief Creates an a_util error ERR_AUTIL_UNEXPECTED
 */
_MAKE_RESULT(-3, ERR_AUTIL_UNEXPECTED);
/**
 * Copies all elements from a decoder to a codec
 * @param[in] decoder The source decoder.
 * @param[out] encoder The destination codec.
 * @return Standard result.
 */
template <typename DECODER, typename ENCODER>
a_util::result::Result transform(const DECODER& decoder, ENCODER& encoder)
{
    try {
        forEachLeafElement(decoder.getElements(), [&encoder](const auto& element) {
            uint64_t value_pointer; // this is the max possible size of a data type at the moment!
                                    // Usertypes are not allowed to be greater!
            element.getRawValue(&value_pointer, sizeof(value_pointer));
            encoder.setElementRawValue(element.getIndex(), &value_pointer, sizeof(value_pointer));
        });
    }
    catch (const std::exception&) {
        return ERR_AUTIL_UNEXPECTED;
    }
    return {};
}

/**
 * Tranforms the data from a given decoder into the opposite data representation.
 * Allocates the buffer accordingly.
 * @param[in] decoder The source decoder.
 * @param[out] buffer The destination buffer object.
 * @param[in] zero Whether or not to memzero the buffer before writing the elements to it.
 * @return Standard result.
 */
a_util::result::Result transformToBuffer(const codec::Decoder& decoder,
                                         a_util::memory::MemoryBuffer& buffer,
                                         bool zero = false);

} // namespace codec
} // namespace ddl

#include <ddl/serialization/serialization_legacy.h>

#endif // DDL_SERIALIZER_CLASS_HEADER
