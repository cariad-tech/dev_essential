/**
 * @file
 * Timer test implementation
 *
 * Copyright @ 2021 VW Group. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <a_util/system/system.h>
#include <a_util/system/timer.h>

#include <gtest/gtest.h>

using namespace a_util;

int& invocationsGlob() noexcept
{
    static int invocations = 0;
    return invocations;
}

void function()
{
    auto& invocations = invocationsGlob();
    ++invocations;
}

class TimerTestFixture : public ::testing::Test {
public:
    void method()
    {
        _method_invocations++;
    }

protected: // access
    auto invocations() -> int
    {
        return _method_invocations;
    }

private: // functions
    void TearDown() override
    {
        invocationsGlob() = 0;
    }

private: // data
    int _method_invocations = {};
};

TEST_F(TimerTestFixture, oneShot)
{
    system::Timer timer(0, &TimerTestFixture::method, *this);
    ASSERT_FALSE(timer.isRunning());

    system::sleepMilliseconds(150);
    ASSERT_EQ(invocations(), 0);
    ASSERT_TRUE(timer.start());
    ASSERT_TRUE(timer.isRunning());
    system::sleepMilliseconds(100);
    ASSERT_EQ(invocations(), 1);
    ASSERT_FALSE(timer.isRunning());
    ASSERT_FALSE(timer.stop());

    ASSERT_TRUE(timer.start());
    ASSERT_TRUE(timer.isRunning());
    system::sleepMilliseconds(100);
    ASSERT_EQ(invocations(), 2);
    ASSERT_FALSE(timer.isRunning());

    timer.setPeriod(50000);
    ASSERT_TRUE(timer.start());
    ASSERT_TRUE(timer.stop());
    ASSERT_EQ(invocations(), 2);
}

TEST_F(TimerTestFixture, periodicallyCallFunction)
{
    // Test whether period is correct and timer can be restarted
    system::Timer timer1;
    timer1.setCallback(&function);
    timer1.setPeriod(50000 /*us*/);
    EXPECT_EQ(timer1.getPeriod(), 50000U /*us*/);
    ASSERT_TRUE(timer1.start());
    EXPECT_TRUE(timer1.isRunning());
    system::sleepMilliseconds(120 /*ms*/);
    ASSERT_TRUE(timer1.stop());
    EXPECT_EQ(invocationsGlob(), 2);

    ASSERT_FALSE(timer1.isRunning());
    invocationsGlob() = 0;
    ASSERT_TRUE(timer1.start());
    system::sleepMilliseconds(120 /*ms*/);
    ASSERT_TRUE(timer1.stop());
    EXPECT_EQ(invocationsGlob(), 2);
    ASSERT_FALSE(timer1.isRunning());
    invocationsGlob() = 0;

    // test whether it runs only after start(), test whether it actually stop()s
    system::Timer timer_func(100000, &function);
    EXPECT_EQ(timer_func.getPeriod(), 100000U);
    system::sleepMilliseconds(150 /*ms*/);
    EXPECT_EQ(invocationsGlob(), 0);
    EXPECT_FALSE(timer_func.isRunning());

    EXPECT_TRUE(timer_func.start());
    EXPECT_TRUE(timer_func.isRunning());
    system::sleepMilliseconds(220 /*ms*/);
    EXPECT_TRUE(timer_func.stop());
    EXPECT_FALSE(timer_func.isRunning());
    system::sleepMilliseconds(120 /*ms*/);
    EXPECT_EQ(invocationsGlob(), 2);
}

TEST_F(TimerTestFixture, periodicallyCallMethod)
{
    {
        system::Timer timer_meth(50000 /*us*/, &TimerTestFixture::method, *this);
        EXPECT_TRUE(timer_meth.start());
        EXPECT_TRUE(timer_meth.isRunning());
        EXPECT_FALSE(timer_meth.start());
        system::sleepMilliseconds(500 /*ms*/);
        EXPECT_TRUE(timer_meth.stop());
        EXPECT_FALSE(timer_meth.isRunning());
        EXPECT_NEAR(invocations(), 10, 2);

        // restart
        EXPECT_FALSE(timer_meth.stop());
        EXPECT_TRUE(timer_meth.start());
        EXPECT_TRUE(timer_meth.isRunning());
        system::sleepMilliseconds(500 /*ms*/);
    } // let go out of scope
    system::sleepMilliseconds(500 /*ms*/);
    EXPECT_NEAR(invocations(), 20, 4);
}

TEST(TimerTest, noCallback)
{
    system::Timer timer;
    timer.setPeriod(1 /*us*/);
    timer.start();
    system::sleepMilliseconds(200);
    timer.stop();
}
