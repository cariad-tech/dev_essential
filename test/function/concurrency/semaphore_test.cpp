/**
 * @file
 * Condition variable test implementation
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

#include "a_util/concurrency/semaphore.h"
#include "a_util/concurrency/thread.h"
#include "a_util/system.h"

#include <gtest/gtest.h>
using namespace a_util;

struct SemaphoreTestStruct {
    concurrency::semaphore sema;
    bool done;

    SemaphoreTestStruct() : done(false)
    {
    }

    void Work()
    {
        if (sema.wait_for(std::chrono::seconds(1))) {
            done = true;
        }
    }
};

TEST(semaphore_test, TestSemaphore)
{
    SemaphoreTestStruct test;
    concurrency::thread th(&SemaphoreTestStruct::Work, &test);
    system::sleepMilliseconds(500);

    ASSERT_EQ(test.done, false);
    test.sema.notify();
    th.join();
    ASSERT_EQ(test.done, true);
    ASSERT_FALSE(test.sema.is_set());
    ASSERT_FALSE(test.sema.wait_for(std::chrono::milliseconds(50)));
    ASSERT_FALSE(test.sema.is_set());
    test.sema.notify();
    ASSERT_TRUE(test.sema.is_set());
    test.sema.reset();
    ASSERT_FALSE(test.sema.is_set());
    ASSERT_FALSE(test.sema.try_wait());
    ASSERT_FALSE(test.sema.is_set());
    test.sema.notify();
    ASSERT_TRUE(test.sema.try_wait());
    ASSERT_FALSE(test.sema.is_set());
}

template <int TIMEOUT>
static void DoTestSemaphoreForDeadlock()
{
    a_util::system::Timer timer;
    timer.setPeriod(100 * 1000); // every 100ms
    timer.start();

    a_util::concurrency::semaphore _semaphore;

    const timestamp_t start_time = a_util::system::getCurrentMicroseconds();
    _semaphore.wait_for(std::chrono::seconds(TIMEOUT));
    const timestamp_t running_time = a_util::system::getCurrentMicroseconds() - start_time;

    timer.stop();

    // Semaphore wait should last about 30 s
    ASSERT_GT(running_time, (TIMEOUT - 1) * 1000 * 1000);
    ASSERT_LT(running_time, (TIMEOUT + 1) * 1000 * 1000);
}

// ATTENTION: On Failure this Test will loop forever !!!
TEST(semaphore_test, TestSemaphoreForDeadlock)
{
    DoTestSemaphoreForDeadlock<1>();
    DoTestSemaphoreForDeadlock<2>();
    DoTestSemaphoreForDeadlock<5>();
    DoTestSemaphoreForDeadlock<10>();
    DoTestSemaphoreForDeadlock<20>();
}

template <int TIMEOUT>
class UnlockSemaphoreThread {
public:
    UnlockSemaphoreThread(a_util::concurrency::semaphore& a_semaphore)
        : _semaphore(a_semaphore), _thread(&UnlockSemaphoreThread::ThreadFunc, this)
    {
    }

public:
    void join()
    {
        _thread.join();
    }

private:
    void ThreadFunc()
    {
        a_util::system::sleepMilliseconds(TIMEOUT * 1000);
        _semaphore.notify();
    }

private:
    a_util::concurrency::semaphore& _semaphore;
    a_util::concurrency::thread _thread;
};

template <int TIMEOUT>
static void DoTestSemaphoreForUnlock()
{
    a_util::system::Timer timer;
    timer.setPeriod(100 * 1000);
    timer.start();

    a_util::concurrency::semaphore _semaphore;
    UnlockSemaphoreThread<TIMEOUT> unlock_sempahore_thread(_semaphore);

    const timestamp_t start_time = a_util::system::getCurrentMicroseconds();
    _semaphore.wait_for(std::chrono::seconds(TIMEOUT * 2));
    const timestamp_t runtime = a_util::system::getCurrentMicroseconds() - start_time;

    timer.stop();
    unlock_sempahore_thread.join();

    // Semaphore wait should last about 30 s
    ASSERT_GT(runtime, (TIMEOUT - 1) * 1000 * 1000);
    ASSERT_LT(runtime, (TIMEOUT + 1) * 1000 * 1000);
}

// ATTENTION: On Failure this Test will loop forever !!!
TEST(semaphore_test, TestSemaphoreForUnlock)
{
    DoTestSemaphoreForUnlock<1>();
    DoTestSemaphoreForUnlock<2>();
    DoTestSemaphoreForUnlock<5>();
    DoTestSemaphoreForUnlock<10>();
    DoTestSemaphoreForUnlock<20>();
}
