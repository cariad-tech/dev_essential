/**
 * @file
 * String convert functions
 *
 * Copyright @ 2021 VW Group. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef A_UTIL_UTIL_DETAIL_STD_TO_DETAIL_HEADER_INCLUDED
#define A_UTIL_UTIL_DETAIL_STD_TO_DETAIL_HEADER_INCLUDED

#include <cstdint>
#include <cstdio> //std::sprintf
#include <cstdlib>

#if (defined(_MSC_VER) && (_MSC_VER >= 1800)) ||                                                   \
    (defined(__clang_major__) && (__cplusplus != 201103L)) ||                                      \
    (defined(__GNUC__) && (__GNUC__ >= 5) && (__cplusplus >= 201103L) && !defined(__QNX__)) ||     \
    (defined(__GNUC__) && (__GNUC__ == 4) && (__GNUC_MINOR__ > 7) &&                               \
     defined(__GXX_EXPERIMENTAL_CXX0X__))

inline std::int64_t a_util_strtoll(const char* str, char** str_end, int base)
{
    return std::strtoll(str, str_end, base);
}

inline std::uint64_t a_util_strtoull(const char* str, char** str_end, int base)
{
    return std::strtoull(str, str_end, base);
}

#define HAS_STD_STRTOULL 1
#define HAS_STD_STRTOLL 1

#elif (defined(_MSC_VER) && (_MSC_VER < 1800))

inline std::int64_t a_util_strtoll(const char* str, char** str_end, int base)
{
    return _strtoi64(str, str_end, base);
}

inline std::uint64_t a_util_strtoull(const char* str, char** str_end, int base)
{
    return _strtoui64(str, str_end, base);
}

#elif defined(__GNUC__) && (__GNUC__ == 5)
// Bug in g++ 5.x.x header file, not having std::strto(u)ll for C++11

inline std::int64_t a_util_strtoll(const char* str, char** str_end, int base)
{
    return strtoll(str, str_end, base);
}

inline std::uint64_t a_util_strtoull(const char* str, char** str_end, int base)
{
    return strtoull(str, str_end, base);
}

#elif defined(__GNUC__) && (!__GXX_EXPERIMENTAL_CXX0X__)

inline std::int64_t a_util_strtoll(const char* str, char** str_end, int base)
{
    return strtoll(str, str_end, base);
}

inline std::uint64_t a_util_strtoull(const char* str, char** str_end, int base)
{
    return strtoull(str, str_end, base);
}

#else

#error Compiler not supported

#endif

#ifndef HAS_STD_STRTOLL
#define HAS_STD_STRTOLL 0
#endif // !HAS_STD_STRTOLL

#ifndef HAS_STD_STRTOULL
#define HAS_STD_STRTOULL 0
#endif // !HAS_STD_STRTOULL

namespace a_util {
namespace strings {
namespace detail {
inline std::int64_t strtoll(const char* str, char** str_end, int base)
{
    return ::a_util_strtoll(str, str_end, base);
}

inline std::uint64_t strtoull(const char* str, char** str_end, int base)
{
    return ::a_util_strtoull(str, str_end, base);
}

using std::sprintf;

} // namespace detail
} // namespace strings
} // namespace a_util

#endif // A_UTIL_UTIL_DETAIL_STD_TO_DETAIL_HEADER_INCLUDED
