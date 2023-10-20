/**
 * @file
 * Mutex test implementation
 *
 * Copyright @ 2021 VW Group. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <a_util/concurrency/fast_mutex.h>
#include <a_util/concurrency/mutex.h>
#include <a_util/concurrency/shared_mutex.h>
#include <a_util/concurrency/thread.h>
#include <a_util/system/system.h> //system::sleepMilliseconds

#include <gtest/gtest.h>

template <typename Mutex>
struct MutexTestStruct {
    Mutex mtx;
    void Work()
    {
        mtx.lock();
        mtx.unlock();
    }
};

TEST(mutex_test, TestFastMutex)
{
    using mutex_test_struct = MutexTestStruct<a_util::concurrency::fast_mutex>;
    mutex_test_struct t;
    ASSERT_TRUE(t.mtx.try_lock());
    ASSERT_NE(nullptr, t.mtx.native_handle()); // handle initialized during construction

    {
        a_util::concurrency::thread th(&mutex_test_struct::Work, &t);
        a_util::system::sleepMilliseconds(100);
        ASSERT_TRUE(th.joinable());

        t.mtx.unlock();
        th.join();
    }
}

TEST(mutex_test, TestMutex)
{
    using mutex_test_struct = MutexTestStruct<a_util::concurrency::mutex>;
    mutex_test_struct t;
    ASSERT_TRUE(t.mtx.try_lock());

    {
        a_util::concurrency::thread th(&mutex_test_struct::Work, &t);
        a_util::system::sleepMilliseconds(100);
        ASSERT_TRUE(th.joinable());
        t.mtx.unlock();
        th.join();
    }
}

TEST(mutex_test, TestRecursiveMutex)
{
    a_util::concurrency::recursive_mutex m;
    m.lock();
    m.lock();
    m.unlock();
    m.unlock();
}

TEST(mutex_test, TestSharedMutex)
{
    a_util::concurrency::shared_mutex m;
    ASSERT_TRUE(m.try_lock());
    ASSERT_FALSE(m.try_lock());
    ASSERT_FALSE(m.try_lock_shared());
    m.unlock();
    m.lock();
    ASSERT_FALSE(m.try_lock());
    ASSERT_FALSE(m.try_lock_shared());
    m.unlock();

    m.lock_shared();
    ASSERT_TRUE(m.try_lock_shared());
    m.lock_shared(); // 3 readers now

    ASSERT_FALSE(m.try_lock());
    m.unlock_shared(); // 2
    ASSERT_FALSE(m.try_lock());
    m.unlock_shared(); // 1
    m.unlock_shared(); // 0

    ASSERT_TRUE(m.try_lock());
    ASSERT_FALSE(m.try_lock());
    ASSERT_FALSE(m.try_lock_shared());
}
