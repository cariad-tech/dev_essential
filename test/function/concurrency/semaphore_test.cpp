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

#include <a_util/concurrency/semaphore.h>
#include <a_util/system.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <atomic>
#include <thread>

class SemaphoreTestFixture : public ::testing::Test {
protected:
    a_util::concurrency::semaphore _sema;
    std::atomic<bool> _done = {};

public:
    void work()
    {
        if (_sema.wait_for(std::chrono::milliseconds(100))) {
            _done = true;
        }
    }
};

TEST_F(SemaphoreTestFixture, notifyWaitForInWorkerThreadSucceeds)
{
    std::thread th(&SemaphoreTestFixture::work, this);
    ASSERT_EQ(_done, false);
    _sema.notify();
    th.join();
    ASSERT_EQ(_done, true);
    ASSERT_FALSE(_sema.is_set());
}

TEST_F(SemaphoreTestFixture, waitForThrowsForNegativeTimeout)
{
    EXPECT_THROW(_sema.wait_for(std::chrono::milliseconds(-50)), std::invalid_argument);
    EXPECT_FALSE(_sema.is_set());
}

TEST_F(SemaphoreTestFixture, waitFor__successful)
{
    _sema.notify();
    ASSERT_TRUE(_sema.wait_for(std::chrono::milliseconds(50)));
}
TEST_F(SemaphoreTestFixture, waitFor__failing)
{
    ASSERT_FALSE(_sema.wait_for(std::chrono::milliseconds(50)));
}

TEST_F(SemaphoreTestFixture, resetLockCountSucceeds)
{
    _sema.notify();
    ASSERT_TRUE(_sema.is_set());
    _sema.reset();
    ASSERT_FALSE(_sema.is_set());
}

TEST_F(SemaphoreTestFixture, tryWaitSucceeds)
{
    ASSERT_FALSE(_sema.try_wait());
    ASSERT_FALSE(_sema.is_set());
    _sema.notify();
    ASSERT_TRUE(_sema.try_wait());
    ASSERT_FALSE(_sema.is_set());
}

TEST_F(SemaphoreTestFixture, waitForReturnsAfterSpecifiedTime)
{
    using namespace ::testing; // gmocks AllOf(), Ge() and Le()

    const auto start_time = a_util::system::getCurrentMilliseconds();
    ASSERT_FALSE(_sema.wait_for(std::chrono::milliseconds(50)));
    const auto running_time = a_util::system::getCurrentMilliseconds() - start_time;

    // Taking the lag of the time measurement into account, the running time is estimated ...
    EXPECT_THAT(running_time, AllOf(Ge(30), Le(150)));
}

class NotifySemaphoreThread {
public:
    NotifySemaphoreThread(a_util::concurrency::semaphore& semaphore)
        : _semaphore(semaphore), _thread(&NotifySemaphoreThread::threadFunc, this)
    {
    }

    ~NotifySemaphoreThread()
    {
        _thread.join();
    }

private:
    void threadFunc()
    {
        a_util::system::sleepMilliseconds(50);
        _semaphore.notify();
    }

private:
    a_util::concurrency::semaphore& _semaphore;
    std::thread _thread;
};

TEST_F(SemaphoreTestFixture, notifyFromWorkerThreadSucceeds)
{
    using namespace ::testing; // gmocks AllOf(), Ge() and Le()

    NotifySemaphoreThread unlock_sempahore_thread(_sema);
    const auto start_time = a_util::system::getCurrentMilliseconds();
    // notify() fires after ~50ms, so this should return after ~50ms, not 1s
    // test timeout is set by ctest to 1s, so this test would abort for this waiting time
    EXPECT_TRUE(_sema.wait_for(std::chrono::seconds(1)));
    const auto running_time = a_util::system::getCurrentMilliseconds() - start_time;

    // Taking the lag of the time measurement into account, the running time is estimated ...
    EXPECT_THAT(running_time, AllOf(Ge(30), Le(150)));
}

TEST_F(SemaphoreTestFixture, waitAfterNotifySucceeds)
{
    _sema.notify();
    _sema.wait();
    ASSERT_FALSE(_sema.is_set());
}

TEST_F(SemaphoreTestFixture, waitBlocksUntilNotify)
{
    std::atomic<bool> wait_stopped{false};
    std::thread t{[&]() {
        _sema.wait();
        wait_stopped = true;
    }};

    ASSERT_FALSE(wait_stopped);
    _sema.notify();
    t.join();
    ASSERT_TRUE(wait_stopped);
    ASSERT_FALSE(_sema.is_set());
}
