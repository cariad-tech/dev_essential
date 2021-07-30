/**
 * Copyright @ 2021 VW Group. All rights reserved.
 *
 *     This Source Code Form is subject to the terms of the Mozilla
 *     Public License, v. 2.0. If a copy of the MPL was not distributed
 *     with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * If it is not possible or desirable to put the notice in a particular file, then
 * You may include the notice in a location (such as a LICENSE file in a
 * relevant directory) where a recipient would be likely to look for such a notice.
 *
 * You may add additional accurate notices of copyright ownership.
 */

#ifndef _TESTER_DEF_SERIALIZATION_HEADER
#define _TESTER_DEF_SERIALIZATION_HEADER

#include <stdint.h>

struct tExample00 {
    uint8_t a;
    uint16_t b;
    uint32_t c;
    uint64_t d;
};
struct tExample01 {
    uint8_t a;
    uint16_t b;
    uint64_t c;
    uint32_t d;
};
struct tExample02 {
    uint8_t a;
    uint32_t b;
    uint16_t c;
    uint64_t d;
};
struct tExample03 {
    uint8_t a;
    uint32_t b;
    uint64_t c;
    uint16_t d;
};
struct tExample04 {
    uint8_t a;
    uint64_t b;
    uint16_t c;
    uint32_t d;
};
struct tExample05 {
    uint8_t a;
    uint64_t b;
    uint32_t c;
    uint16_t d;
};
struct tExample06 {
    uint16_t a;
    uint8_t b;
    uint32_t c;
    uint64_t d;
};
struct tExample07 {
    uint16_t a;
    uint8_t b;
    uint64_t c;
    uint32_t d;
};
struct tExample08 {
    uint16_t a;
    uint32_t b;
    uint8_t c;
    uint64_t d;
};
struct tExample09 {
    uint16_t a;
    uint32_t b;
    uint64_t c;
    uint8_t d;
};
struct tExample10 {
    uint16_t a;
    uint64_t b;
    uint8_t c;
    uint32_t d;
};
struct tExample11 {
    uint16_t a;
    uint64_t b;
    uint32_t c;
    uint8_t d;
};
struct tExample12 {
    uint32_t a;
    uint8_t b;
    uint16_t c;
    uint64_t d;
};
struct tExample13 {
    uint32_t a;
    uint8_t b;
    uint64_t c;
    uint16_t d;
};
struct tExample14 {
    uint32_t a;
    uint16_t b;
    uint8_t c;
    uint64_t d;
};
struct tExample15 {
    uint32_t a;
    uint16_t b;
    uint64_t c;
    uint8_t d;
};
struct tExample16 {
    uint32_t a;
    uint64_t b;
    uint8_t c;
    uint16_t d;
};
struct tExample17 {
    uint32_t a;
    uint64_t b;
    uint16_t c;
    uint8_t d;
};
struct tExample18 {
    uint64_t a;
    uint8_t b;
    uint16_t c;
    uint32_t d;
};
struct tExample19 {
    uint64_t a;
    uint8_t b;
    uint32_t c;
    uint16_t d;
};
struct tExample20 {
    uint64_t a;
    uint16_t b;
    uint8_t c;
    uint32_t d;
};
struct tExample21 {
    uint64_t a;
    uint16_t b;
    uint32_t c;
    uint8_t d;
};
struct tExample22 {
    uint64_t a;
    uint32_t b;
    uint8_t c;
    uint16_t d;
};
struct tExample23 {
    uint64_t a;
    uint32_t b;
    uint16_t c;
    uint8_t d;
};

namespace pack_1 {
#pragma pack(push, 1)
struct tExample00 {
    uint8_t a;
    uint16_t b;
    uint32_t c;
    uint64_t d;
};
struct tExample01 {
    uint8_t a;
    uint16_t b;
    uint64_t c;
    uint32_t d;
};
struct tExample02 {
    uint8_t a;
    uint32_t b;
    uint16_t c;
    uint64_t d;
};
struct tExample03 {
    uint8_t a;
    uint32_t b;
    uint64_t c;
    uint16_t d;
};
struct tExample04 {
    uint8_t a;
    uint64_t b;
    uint16_t c;
    uint32_t d;
};
struct tExample05 {
    uint8_t a;
    uint64_t b;
    uint32_t c;
    uint16_t d;
};
struct tExample06 {
    uint16_t a;
    uint8_t b;
    uint32_t c;
    uint64_t d;
};
struct tExample07 {
    uint16_t a;
    uint8_t b;
    uint64_t c;
    uint32_t d;
};
struct tExample08 {
    uint16_t a;
    uint32_t b;
    uint8_t c;
    uint64_t d;
};
struct tExample09 {
    uint16_t a;
    uint32_t b;
    uint64_t c;
    uint8_t d;
};
struct tExample10 {
    uint16_t a;
    uint64_t b;
    uint8_t c;
    uint32_t d;
};
struct tExample11 {
    uint16_t a;
    uint64_t b;
    uint32_t c;
    uint8_t d;
};
struct tExample12 {
    uint32_t a;
    uint8_t b;
    uint16_t c;
    uint64_t d;
};
struct tExample13 {
    uint32_t a;
    uint8_t b;
    uint64_t c;
    uint16_t d;
};
struct tExample14 {
    uint32_t a;
    uint16_t b;
    uint8_t c;
    uint64_t d;
};
struct tExample15 {
    uint32_t a;
    uint16_t b;
    uint64_t c;
    uint8_t d;
};
struct tExample16 {
    uint32_t a;
    uint64_t b;
    uint8_t c;
    uint16_t d;
};
struct tExample17 {
    uint32_t a;
    uint64_t b;
    uint16_t c;
    uint8_t d;
};
struct tExample18 {
    uint64_t a;
    uint8_t b;
    uint16_t c;
    uint32_t d;
};
struct tExample19 {
    uint64_t a;
    uint8_t b;
    uint32_t c;
    uint16_t d;
};
struct tExample20 {
    uint64_t a;
    uint16_t b;
    uint8_t c;
    uint32_t d;
};
struct tExample21 {
    uint64_t a;
    uint16_t b;
    uint32_t c;
    uint8_t d;
};
struct tExample22 {
    uint64_t a;
    uint32_t b;
    uint8_t c;
    uint16_t d;
};
struct tExample23 {
    uint64_t a;
    uint32_t b;
    uint16_t c;
    uint8_t d;
};
#pragma pack(pop)
} // namespace pack_1

#endif //_TESTER_DEF_SERIALIZATION_HEADER
