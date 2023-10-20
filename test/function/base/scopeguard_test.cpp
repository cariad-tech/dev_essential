/**
 * @file
 * scopeguard test
 *
 * Copyright @ 2021 VW Group. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <a_util/base/scopeguard.h>

#include <gtest/gtest.h>

namespace {

using a_util::scopeExit;
#ifdef A_UTIL_HAVE_UNCAUGHT_EXCEPTIONS
using a_util::scopeFailure;
using a_util::scopeSuccess;
#endif

int g_on_exit_called = 0;

void onExit()
{
    ++g_on_exit_called;
}

struct OnExitClass {
    void onExit()
    {
        ++_on_exit_called;
    }

    static int _on_exit_called;
};

int OnExitClass::_on_exit_called = 0;

struct OnExitCallable {
    OnExitCallable()
    {
    }
    OnExitCallable& operator=(const OnExitCallable&)
    {
        ++_copy_assigned;
        return *this;
    }
    OnExitCallable& operator=(OnExitCallable&&)
    {
        ++_move_assigned;
        return *this;
    }
    OnExitCallable(const OnExitCallable&)
    {
        ++_copy_constructed;
    }
    OnExitCallable(OnExitCallable&&)
    {
        ++_move_constructed;
    }
    void operator()()
    {
        ++_called;
    }

    static void clearGlobals()
    {
        _called = 0;
        _copy_assigned = 0;
        _move_assigned = 0;
        _copy_constructed = 0;
        _move_constructed = 0;
    }

    static int _called, _copy_assigned, _move_assigned, _copy_constructed, _move_constructed;
};

int OnExitCallable::_called = 0;
int OnExitCallable::_copy_assigned = 0;
int OnExitCallable::_move_assigned = 0;
int OnExitCallable::_copy_constructed = 0;
int OnExitCallable::_move_constructed = 0;

TEST(scopeguard_test, scopeExitFreeFunction)
{
    g_on_exit_called = 0;
    // No anonymous name macro because of ODR concerns.
    // See https://stackoverflow.com/questions/37268686/how-can-counter-cause-a-odr-violation-here
    {
        EXPECT_EQ(g_on_exit_called, 0);
        auto g = scopeExit(onExit);
        EXPECT_EQ(g_on_exit_called, 0);
    }
    EXPECT_EQ(g_on_exit_called, 1);
}

TEST(scopeguard_test, scopeExitLambda)
{
    int lambda_called = 0;
    {
        auto g = scopeExit([&]() { ++lambda_called; });
    }
    EXPECT_EQ(lambda_called, 1);
}

TEST(scopeguard_test, scopeExitBind)
{
    OnExitClass::_on_exit_called = 0;
    {
        OnExitClass c;
        auto g = scopeExit(std::bind(&OnExitClass::onExit, c));
    }
    EXPECT_EQ(OnExitClass::_on_exit_called, 1);
}

TEST(scopeguard_test, scopeExitFunction)
{
    g_on_exit_called = 0;
    {
        std::function<void()> f = onExit;
        auto g = scopeExit(f);
    }
    EXPECT_EQ(g_on_exit_called, 1);
}

TEST(scopeguard_test, scopeExitCallableRValue)
{
    OnExitCallable::clearGlobals();
    {
        auto g = scopeExit(OnExitCallable());
    }
    EXPECT_EQ(OnExitCallable::_called, 1);
    EXPECT_EQ(OnExitCallable::_copy_assigned, 0);
    EXPECT_EQ(OnExitCallable::_move_assigned, 0);
    EXPECT_EQ(OnExitCallable::_copy_constructed, 0);
    EXPECT_EQ(OnExitCallable::_move_constructed, 1);
}

TEST(scopeguard_test, scopeExitCallableLValue)
{
    OnExitCallable::clearGlobals();
    {
        OnExitCallable o;
        auto g = scopeExit(o);
    }
    EXPECT_EQ(OnExitCallable::_called, 1);
    EXPECT_EQ(OnExitCallable::_copy_assigned, 0);
    EXPECT_EQ(OnExitCallable::_move_assigned, 0);
    EXPECT_EQ(OnExitCallable::_copy_constructed, 1);
    EXPECT_EQ(OnExitCallable::_move_constructed, 0);
}

TEST(scopeguard_test, scopeExitCallableMove)
{
    OnExitCallable::clearGlobals();
    {
        OnExitCallable o;
        auto g = scopeExit(std::move(o));
    }
    EXPECT_EQ(OnExitCallable::_called, 1);
    EXPECT_EQ(OnExitCallable::_copy_assigned, 0);
    EXPECT_EQ(OnExitCallable::_move_assigned, 0);
    EXPECT_EQ(OnExitCallable::_copy_constructed, 0);
    EXPECT_EQ(OnExitCallable::_move_constructed, 1);
}

TEST(scopeguard_test, scopeExitEnabling)
{
    int scope_exit = 0;
    {
        auto g = scopeExit([&]() { ++scope_exit; });
        g.dismiss();
    }
    EXPECT_EQ(scope_exit, 0);
    {
        auto g = scopeExit([&]() { ++scope_exit; });
        g.dismiss();
        g.rehire();
    }
    EXPECT_EQ(scope_exit, 1);
}

#ifdef A_UTIL_HAVE_UNCAUGHT_EXCEPTIONS

template <bool go_boom>
struct DtorBoom {
    ~DtorBoom()
    {
        try {
            auto g1 = scopeSuccess([&]() { ++_scope_success; });
            auto g2 = scopeFailure([&]() { ++_scope_failure; });
            if constexpr (go_boom)
                throw std::runtime_error("boom");
        }
        catch (...) {
        }
    }

    static int _scope_success;
    static int _scope_failure;
};

template <bool go_boom>
int DtorBoom<go_boom>::_scope_success = 0;
template <bool go_boom>
int DtorBoom<go_boom>::_scope_failure = 0;

TEST(scopeguard_test, scopeSuccess)
{
    try {
        DtorBoom<true> b;
        // This is why we need std::uncaught_exceptions(). With std::uncaught_exception() only,
        // we cannot decide in the D'tor of DtorBoom if an exception occurred locally, because the
        // uncaught exception outside would mask the local one.
        throw std::runtime_error("Run dtor while exception uncaught");
    }
    catch (...) {
    }
    EXPECT_EQ(DtorBoom<true>::_scope_failure, 1);
    EXPECT_EQ(DtorBoom<true>::_scope_success, 0);
}

TEST(scopeguard_test, scopeFailure)
{
    try {
        DtorBoom<false> b;
        throw std::runtime_error("Run dtor while exception uncaught");
    }
    catch (...) {
    }
    EXPECT_EQ(DtorBoom<false>::_scope_failure, 0);
    EXPECT_EQ(DtorBoom<false>::_scope_success, 1);
}

TEST(scopeguard_test, scopeSuccessFailureEnabling)
{
    int scope_success = 0;
    {
        auto g = scopeSuccess([&]() { ++scope_success; });
        g.dismiss();
    }
    EXPECT_EQ(scope_success, 0);
    {
        auto g = scopeSuccess([&]() { ++scope_success; });
        g.dismiss();
        g.rehire();
    }
    EXPECT_EQ(scope_success, 1);
}

#endif // A_UTIL_HAVE_UNCAUGHT_EXCEPTIONS

} // namespace