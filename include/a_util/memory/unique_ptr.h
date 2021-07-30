/**
 * @file
 * Public API for compatibility to C++11
 * <a href="https://en.cppreference.com/w/cpp/memory/unique_ptr">std::unique_ptr</a> and C++14
 * <a href="https://en.cppreference.com/w/cpp/memory/unique_ptr/make_unique">std::make_unique</a>
 * from C++ standard header <a href="https://en.cppreference.com/w/cpp/header/memory">\<memory\>
 * </a>
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

#ifndef A_UTIL_UTIL_MEMORY_UNIQUE_PTR_HEADER_INCLUDED
#define A_UTIL_UTIL_MEMORY_UNIQUE_PTR_HEADER_INCLUDED

#include <memory>

namespace a_util {
namespace memory {
/**
 * Compatibility to C++11
 * <a href="https://en.cppreference.com/w/cpp/memory/unique_ptr">std::unique_ptr</a>
 */
using ::std::unique_ptr;

#if (!defined(_MSC_VER) && !(__cplusplus > 201103L)) ||                                            \
    (defined(_MSC_VER) && (_MSC_VER < 1800)) || defined(A_UTIL_ENABLE_MAKE_UNIQUE)

/** @cond INTERNAL_DOCUMENTATION */
template <class T>
struct _Unique_if {
    typedef unique_ptr<T> _Single_object;
};

template <class T>
struct _Unique_if<T[]> {
    typedef unique_ptr<T[]> _Unknown_bound;
};

template <class T, size_t N>
struct _Unique_if<T[N]> {
    typedef void _Known_bound;
};
/** @endcond */

/**
 * Compatibility to C++14
 * <a href="https://en.cppreference.com/w/cpp/memory/unique_ptr/make_unique">std::make_unique</a>
 * for non-array types
 *
 * @tparam T Type to create
 * @tparam Args Arguments types matching constructor argument types of type T
 * @param[in] args List of arguments passed to the constructor of type T
 * @return Object of non-array type T
 * @copyright Stephan T. Lavavej (<stl@microsoft.com>) via C++ standard proposal
 * <a href="http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3656.htm">JTC1/SC22/WG21
 * N3656</a>
 * @note Define @c A_UTIL_ENABLE_MAKE_UNIQUE in case the platform check mistakenly identifies the
 *       existance of @c std::make_unique
 */
template <class T, class... Args>
typename _Unique_if<T>::_Single_object make_unique(Args&&... args)
{
    return unique_ptr<T>(new T(std::forward<Args>(args)...));
}

/**
 * Compatibility to C++14
 * <a href="https://en.cppreference.com/w/cpp/memory/unique_ptr/make_unique">std::make_unique</a>
 * for array types
 *
 * @tparam T Type to create an array of unknown bound
 * @param[in] n Size of the array
 * @return Array of unknown bound type T
 * @copyright Stephan T. Lavavej (<stl@microsoft.com>) via C++ standard proposal
 * <a href="http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3656.htm">JTC1/SC22/WG21
 * N3656</a>
 * @note Define @c A_UTIL_ENABLE_MAKE_UNIQUE in case the platform check mistakenly identifies the
 *       existance of @c std::make_unique
 */
template <class T>
typename _Unique_if<T>::_Unknown_bound make_unique(size_t n)
{
    typedef typename std::remove_extent<T>::type U;
    return unique_ptr<T>(new U[n]());
}

/**
 * Compatibility to C++14
 * <a href="https://en.cppreference.com/w/cpp/memory/unique_ptr/make_unique">std::make_unique</a>
 * to disallow construction of arrays of known bound
 *
 * @tparam T Type of array of known bound
 * @tparam Args Ignored
 * @copyright Stephan T. Lavavej (<stl@microsoft.com>) via C++ standard proposal
 * <a href="http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3656.htm">JTC1/SC22/WG21
 * N3656</a>
 * @note Define @c A_UTIL_ENABLE_MAKE_UNIQUE in case the platform check mistakenly identifies the
 *       existance of @c std::make_unique
 */
template <class T, class... Args>
typename _Unique_if<T>::_Known_bound make_unique(Args&&...) = delete;

#else

/**
 * Compatibility to C++14
 * <a href="https://en.cppreference.com/w/cpp/memory/unique_ptr/make_unique">std::make_unique</a>
 */

using ::std::make_unique;

#endif // defined(A_UTIL_ENABLE_MAKE_UNIQUE)

} // namespace memory
} // namespace a_util

#endif // A_UTIL_UTIL_MEMORY_UNIQUE_PTR_HEADER_INCLUDED
