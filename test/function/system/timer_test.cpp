/**
 * @file
 * Timer test implementation
 *
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

#include "a_util/system/system.h"
#include "a_util/system/timer.h"

#include <gtest/gtest.h>

using namespace a_util;

static int invocations_glob = 0;
struct TimerTestStruct {
    static void Function()
    {
        invocations_glob++;
    }

    int invocations;
    TimerTestStruct() : invocations(0)
    {
    }
    void Method()
    {
        invocations++;
    }
};

TEST(timer_test, TestTimerOneShot)
{
    TimerTestStruct test;
    system::Timer timer(0, &TimerTestStruct::Method, test);
    ASSERT_FALSE(timer.isRunning());

    system::sleepMilliseconds(150);
    ASSERT_EQ(test.invocations, 0);
    ASSERT_TRUE(timer.start());
    ASSERT_TRUE(timer.isRunning());
    system::sleepMilliseconds(100);
    ASSERT_EQ(test.invocations, 1);
    ASSERT_FALSE(timer.isRunning());
    ASSERT_FALSE(timer.stop());

    ASSERT_TRUE(timer.start());
    ASSERT_TRUE(timer.isRunning());
    system::sleepMilliseconds(100);
    ASSERT_EQ(test.invocations, 2);
    ASSERT_FALSE(timer.isRunning());

    timer.setPeriod(50000);
    ASSERT_TRUE(timer.start());
    ASSERT_TRUE(timer.stop());
    ASSERT_EQ(test.invocations, 2);
}

TEST(timer_test, TestTimerPeriodic)
{
    // Test whether period is correct and timer can be restarted
    system::Timer timer1;
    timer1.setCallback(&TimerTestStruct::Function);
    timer1.setPeriod(50000);
    EXPECT_EQ(timer1.getPeriod(), 50000);
    ASSERT_TRUE(timer1.start());
    EXPECT_TRUE(timer1.isRunning());
    system::sleepMilliseconds(120 /*ms*/);
    ASSERT_TRUE(timer1.stop());
    EXPECT_EQ(invocations_glob, 2);

    ASSERT_FALSE(timer1.isRunning());
    invocations_glob = 0;
    ASSERT_TRUE(timer1.start());
    system::sleepMilliseconds(120 /*ms*/);
    ASSERT_TRUE(timer1.stop());
    EXPECT_EQ(invocations_glob, 2);
    ASSERT_FALSE(timer1.isRunning());
    invocations_glob = 0;

    // test whether it runs only after start(), test whether it actually stop()s
    system::Timer timer_func(100000, &TimerTestStruct::Function);
    EXPECT_EQ(timer_func.getPeriod(), 100000);
    system::sleepMilliseconds(150 /*ms*/);
    EXPECT_EQ(invocations_glob, 0);
    EXPECT_FALSE(timer_func.isRunning());

    EXPECT_TRUE(timer_func.start());
    EXPECT_TRUE(timer_func.isRunning());
    system::sleepMilliseconds(220 /*ms*/);
    EXPECT_TRUE(timer_func.stop());
    EXPECT_FALSE(timer_func.isRunning());
    system::sleepMilliseconds(120 /*ms*/);
    EXPECT_EQ(invocations_glob, 2);

    // same with a member function
    TimerTestStruct test;
    {
        system::Timer timer_meth(50000, &TimerTestStruct::Method, test);
        EXPECT_TRUE(timer_meth.start());
        EXPECT_TRUE(timer_meth.isRunning());
        EXPECT_FALSE(timer_meth.start());
        system::sleepMilliseconds(500 /*ms*/);
        EXPECT_TRUE(timer_meth.stop());
        EXPECT_FALSE(timer_meth.isRunning());
        EXPECT_NEAR(test.invocations, 10, 2);

        // restart
        EXPECT_FALSE(timer_meth.stop());
        EXPECT_TRUE(timer_meth.start());
        EXPECT_TRUE(timer_meth.isRunning());
        system::sleepMilliseconds(500 /*ms*/);
    } // let go out of scope
    system::sleepMilliseconds(500 /*ms*/);
    EXPECT_NEAR(test.invocations, 20, 4);
}

TEST(timer_test, TestTimerNoCallback)
{
    system::Timer timer;
    timer.setPeriod(1);
    timer.start();
    system::sleepMilliseconds(200);
    timer.stop();
}
