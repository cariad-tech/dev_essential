/**
 * @file
 * Thread test implementation
 *
 * Copyright @ 2021 VW Group. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <a_util/concurrency/atomic.h>
#include <a_util/concurrency/thread.h>
#include <a_util/system.h>

#include <gtest/gtest.h>

#include <cstddef>

using namespace a_util;

namespace TestThreadUtils {
struct foo {
    bool b;
    foo() : b(false)
    {
    }

    void work()
    {
        b = true;
    }
};

static bool global_b = false;
static void GlobalWork()
{
    global_b = true;
}
} // namespace TestThreadUtils

struct ThreadTestStruct {
    static void ReturnUnjoined()
    {
        concurrency::thread t3(TestThreadUtils::GlobalWork);
        ASSERT_TRUE(t3.joinable());
    }
};

TEST(thread_test, TestThread)
{
    TestThreadUtils::foo f;
    ASSERT_FALSE(f.b);
    concurrency::thread t1(&TestThreadUtils::foo::work, &f);

    ASSERT_TRUE(t1.joinable());
    t1.join();
    concurrency::memoryBarrier();
    ASSERT_TRUE(f.b);

    concurrency::thread t2(TestThreadUtils::GlobalWork);
    // dont join immediately but sleep, to ensure the threadfunc returns
    // this tests whether the thread stays joinable despite being finished
    system::sleepMilliseconds(250);
    ASSERT_TRUE(t2.joinable());
    t2.join();
    concurrency::memoryBarrier();
    ASSERT_TRUE(TestThreadUtils::global_b);

    ASSERT_GE(concurrency::thread::hardware_concurrency(), 1u);
    ASSERT_NO_THROW(std::this_thread::yield());
    ASSERT_NE(std::this_thread::get_id(), t2.get_id());

#ifndef __QNX__ // Google test's death test mechanism causes a core file on QNX
    ASSERT_DEATH_IF_SUPPORTED(ThreadTestStruct::ReturnUnjoined(), ".*");
#endif
}

namespace TestThreadUtils {
static void doWork()
{
    system::sleepMilliseconds(3 * 1000);
}
} // namespace TestThreadUtils

TEST(thread_test, TestDetach)
{
    concurrency::thread t1(TestThreadUtils::doWork);
    ASSERT_TRUE(t1.joinable());

    t1.detach();
    ASSERT_FALSE(t1.joinable());
}
