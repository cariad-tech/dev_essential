/**
 * @file
 * static_assert() fallback for compilers without the feature
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

#ifndef A_UTIL_UTIL_BASE_STATIC_ASSERT_FALLBACK_HEADER_INCLUDED
#define A_UTIL_UTIL_BASE_STATIC_ASSERT_FALLBACK_HEADER_INCLUDED

#include <a_util/base/config.h>

#if A_UTIL_ENABLE_COMPILER_CXX_STATIC_ASSERT

// use c++-11 keyword 'static_assert'

#else

namespace a_util {
namespace base {
namespace detail {

/// Helper type for backported @c static_assert() - incomplete on purpose
template <bool>
struct StaticAssert;
/// Helper type for backported @c static_assert() if the given condition is evaluated to @c true
template <>
struct StaticAssert<true> {
    typedef bool type;
};

} // namespace detail
} // namespace base
} // namespace a_util

// full disclosure: Stolen from googletest framework (see GTEST_ATTRIBUTE_UNUSED_ for details)
#if defined(__GNUC__) && !defined(COMPILER_ICC)
#define A_UTIL_ATTRIBUTE_UNUSED_ __attribute__((unused))
#elif defined(__clang__)
#if __has_attribute(unused)
#define A_UTIL_ATTRIBUTE_UNUSED_ __attribute__((unused))
#endif
#endif
#ifndef A_UTIL_ATTRIBUTE_UNUSED_
#define A_UTIL_ATTRIBUTE_UNUSED_
#endif

// Two phase macro expansion to get the macro expansion of __LINE__
// Also see stackoverflow: http://tinyurl.com/ybehbeza
#define STATIC_ASSERT_PASTE_TOKENS(x, y) x##y
#define STATIC_ASSERT_TYPE(x, y) STATIC_ASSERT_PASTE_TOKENS(x, y)

/**
 * Replacement for 'static_assert' feature for compilers not supporting it
 * @details Uses a_util::base::detail::StaticAssert<bool> under the hood. If @c expr evaluates to
 *          @c true, the compilation succeeds by using the provided class template specialization.
 *          If @c expr evaluates to @c false, compilation fails due to the missing class template
 *          specialization for const expression @c false. In case compilation succeeds, a type
 *          is created with helper macro @c STATIC_ASSERT_TYPE. The intention of the type is to
 *          make sure, compilation stops in case of an invalid condition and succeeds otherwise. It
 *          consists of 'static_assert_succeeded_in_line_' plus the stringified @c __LINE__ in
 *          which the @c static_assert() macro was expanded. The @c __LINE__ is appended to make
 *          sure, the macro expansion can be used more than once within the same scope.
 * @param[in] expr The expression - must evaluate to a @c bool type. To workaround macro expansion
 *                 errors raised by types containing a comma, parenthesis might be used.
 * @param[in] msg Ignored, purely to match the signature of @c static_assert() feature
 */
#define static_assert(expr, msg)                                                                   \
    typedef typename ::a_util::base::detail::StaticAssert<(static_cast<bool>(expr))>::type         \
        STATIC_ASSERT_TYPE(static_assert_succeeded_in_line_, __LINE__) A_UTIL_ATTRIBUTE_UNUSED_

#endif // A_UTIL_ENABLE_COMPILER_CXX_STATIC_ASSERT

#endif // A_UTIL_UTIL_BASE_STATIC_ASSERT_FALLBACK_HEADER_INCLUDED
