/**
 * @file
 * Condition variable test implementation
 *
 * Copyright @ 2021 VW Group. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <a_util/concurrency/atomic.h>
#include <a_util/concurrency/condition_variable.h>
#include <a_util/concurrency/mutex.h>
#include <a_util/concurrency/thread.h>
#include <a_util/system/system.h>

#include <gtest/gtest.h>
using namespace a_util;

struct CondVarTestStruct {
    concurrency::condition_variable& cond;
    concurrency::mutex& mtx;
    volatile bool& signal;
    CondVarTestStruct(concurrency::condition_variable& cond_,
                      concurrency::mutex& mtx_,
                      bool& signal_)
        : cond(cond_), mtx(mtx_), signal(signal_)
    {
    }

    void Work()
    {
        concurrency::unique_lock<concurrency::mutex> guard(mtx);
        ASSERT_FALSE(signal);
        while (!signal) {
            cond.wait_for(guard, std::chrono::minutes(1));
            concurrency::memoryBarrier();
        }
    }
};

TEST(condvar_test, TestCondVar)
{
    concurrency::condition_variable cond;
    concurrency::mutex mtx;
    bool signal = false;

    CondVarTestStruct test(cond, mtx, signal);
    concurrency::thread th(&CondVarTestStruct::Work, &test);
    system::sleepMilliseconds(500);

    {
        concurrency::unique_lock<concurrency::mutex> guard(mtx);
        signal = true;
        cond.notify_one();
    }

    th.join();
    ASSERT_TRUE(signal);
}
