/**
 * @file
 * Contains code for scope guard classes. These classes facilitate use of the
 * RAII idiom to perform cleanup work or resource management. They are based on
 * Andrei Alexandrescu's talk “Declarative Control Flow"
 * (https://www.youtube.com/watch?v=WjTrfoiB0MQ)
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

#ifndef A_UTIL_SCOPEGUARD_HEADER_INCLUDED
#define A_UTIL_SCOPEGUARD_HEADER_INCLUDED

#include <exception>
#include <type_traits>
#include <utility>

#if ((defined _MSC_VER) && (_MSVC_LANG >= 201703L)) ||                                             \
    ((!defined _MSC_VER) && (__cplusplus >= 201703L))
// C++17 is required for std::uncaught_exceptions. Code using it is enabled conditionally.
// MSVC does not correctly set the value of __cplusplus. Therefore there is an extra test for
// MSVC in the preprocessor directive.
// See https://devblogs.microsoft.com/cppblog/msvc-now-correctly-reports-__cplusplus/
#define A_UTIL_HAVE_UNCAUGHT_EXCEPTIONS
#define A_UTIL_HAVE_GUARANTEED_COPY_ELISION
#endif

namespace a_util {

/**
 * @addtogroup a_util_core
 * @{
 */

/**
 * Scope guard executing a callable object of type @c F on scope exit.
 *
 * The callable execution can be disabled/re-enabled by using @ref dismiss() or @ref
 * rehire() respectively (default: enabled).
 *
 * To actually create scope guards objects, the following convenience functions are provided:
 * - @ref scopeExit()
 * - @ref scopeSuccess()
 * - @ref scopeFailure()
 *
 * @tparam F Type of the callable object (pointer to function, lambda, etc.)
 */
template <class F>
class ScopeGuard {
public:
    ScopeGuard() = delete;
    ScopeGuard(const ScopeGuard&) = delete;
    ScopeGuard& operator=(const ScopeGuard&) = delete;
    ScopeGuard& operator=(ScopeGuard&& other) = delete;

#ifdef A_UTIL_HAVE_GUARANTEED_COPY_ELISION
    ScopeGuard(ScopeGuard&&) = delete;
#else
    ScopeGuard(ScopeGuard&& other) : _enabled(other._enabled), _f(other._f)
    {
        other.dismiss();
    }
#endif // A_UTIL_HAVE_GUARANTEED_COPY_ELISION

    /**
     * Ctor
     * @param[in] f The callable object to execute on scope exit
     */
    explicit ScopeGuard(F&& f) : _f(std::forward<F>(f))
    {
    }

    /**
     * Destroy the scope guard object and execute the callable if execution is enabled
     */
    ~ScopeGuard()
    {
        if (_enabled) {
            _f();
        }
    }

    /**
     * Dismiss the execution of the callable on scope exit
     */
    void dismiss() noexcept
    {
        _enabled = false;
    }

    /**
     * Rehire the execution of the callable on scope exit
     */
    void rehire() noexcept
    {
        _enabled = true;
    }

private:
    bool _enabled = true;
    typename std::decay<F>::type _f;
};

/**
 * Creates a scope guard executing callable @c f on scope exit.
 *
 * Usage example:
 * @code{.cpp}
{
    auto needs_cleanup = create_something();
    auto onexit = scopeExit([&]() { cleanup(needs_cleanup); });
    could_throw();
}
@endcode
 * This can help simplify C-style resource handling and control flow for exception handling.
 *
 * @param[in] f The callable object to execute.
 * @return An instance of @ref ScopeGuard
 */
template <class F>
inline auto scopeExit(F&& f)
{
    return ScopeGuard<F>(std::forward<F>(f));
}

#ifdef A_UTIL_HAVE_UNCAUGHT_EXCEPTIONS

/** @cond INTERNAL_DOCUMENTATION */
namespace detail {

template <class F, bool ExecuteOnFailure>
class ScopeGuard final : public ::a_util::ScopeGuard<F> {
public:
    explicit ScopeGuard(F&& f) : ::a_util::ScopeGuard<F>(std::forward<F>(f))
    {
    }

    ~ScopeGuard()
    {
        const auto uncaught_exceptions_on_exit = std::uncaught_exceptions();
        const auto success = _uncaught_exceptions_on_entry == uncaught_exceptions_on_exit;
        if (ExecuteOnFailure == success) {
            this->dismiss();
        }
    }

private:
    const int _uncaught_exceptions_on_entry = std::uncaught_exceptions();
};

} // namespace detail
/** @endcond */

/**
 * Creates a scope guard executing callable @c f on scope exit if no uncaught exceptions occurred.
 *
 * @copydetails scopeExit
 * @see https://en.cppreference.com/w/cpp/error/uncaught_exception
 *
 */
template <class F>
inline auto scopeSuccess(F&& f)
{
    return detail::ScopeGuard<F, /*ExecuteOnFailure=*/false>(std::forward<F>(f));
}

/**
 * Creates a scope guard executing callable @c f on scope exit if uncaught exceptions occurred.
 *
 * @copydetails scopeSuccess
 */
template <class F>
inline auto scopeFailure(F&& f)
{
    return detail::ScopeGuard<F, /*ExecuteOnFailure=*/true>(std::forward<F>(f));
}

#endif // A_UTIL_HAVE_UNCAUGHT_EXCEPTIONS

/** @} */

} // namespace a_util

#endif // A_UTIL_SCOPEGUARD_HEADER_INCLUDED