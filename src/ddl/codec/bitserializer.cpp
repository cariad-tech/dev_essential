/**
 * @file
 * Raw memory related functionality
 *
 * Copyright @ 2021 VW Group. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <a_util/memory.h>
#include <ddl/codec/bitserializer.h>

#include <stdexcept>

using namespace a_util;
using namespace a_util::memory;

a_util::memory::Endianess a_util::memory::get_platform_endianess()
{
    uint32_t value = 0x01020304;
    if (((unsigned char*)&value)[0] == 0x04 && ((unsigned char*)&value)[2] == 0x02) {
        return bit_little_endian;
    }
    else if (((unsigned char*)&value)[0] == 0x01 && ((unsigned char*)&value)[2] == 0x03) {
        return bit_big_endian;
    }
    else {
        throw std::runtime_error("unsupported endianess");
    }
}

std::string a_util::memory::detail::formatBits(uint64_t value)
{
    std::string bit_string;

    for (int i = 0; i < (int)sizeof(uint64_t) * 8; i++) {
        if (i % 8 == 0) {
            bit_string += " | ";
        }
        bit_string += a_util::strings::toString((value >> (63 - i)) & 1);
    }

    return bit_string;
}

a_util::result::Result a_util::memory::detail::convertSignalEndianess(uint64_t* signal,
                                                                      Endianess endianess,
                                                                      size_t bit_length)
{
    if (bit_length > 8) // Signal is bigger than one byte
    {
        if (endianess != get_platform_endianess()) // Byteorder was different on writing platform
        {
            if (bit_length > (4 * 8)) // 64 Bit
            {
                // If the BE signal is shorter than 8 byte, make sure the value is aligned
                // at the LSB end, seen from a BE viewpoint.
                if (endianess == bit_big_endian) {
                    *signal <<= (((8 * 8) - (bit_length)) / 8) * 8;
                }
                *reinterpret_cast<uint64_t*>(signal) =
                    a_util::memory::swapEndianess(*reinterpret_cast<uint64_t*>(signal));
            }
            else if (bit_length > (2 * 8)) // 32 Bit
            {
                // If the BE signal is shorter than 4 byte, make sure the value is aligned
                // at the LSB end, seen from a BE viewpoint.
                if (endianess == bit_big_endian) {
                    *signal <<= (((4 * 8) - (bit_length)) / 8) * 8;
                }
                *reinterpret_cast<uint32_t*>(signal) =
                    a_util::memory::swapEndianess(*reinterpret_cast<uint32_t*>(signal));
            }
            else // 16 Bit
            {
                *reinterpret_cast<uint16_t*>(signal) =
                    a_util::memory::swapEndianess(*reinterpret_cast<uint16_t*>(signal));
            }
        }
    }

    return a_util::result::SUCCESS;
}
