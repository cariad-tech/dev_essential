/**
 * @file
 * Mutex test implementation
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

#include "a_util/concurrency/mutex.h"
#include "a_util/concurrency/shared_mutex.h"
#include "a_util/concurrency/thread.h"
#include "a_util/system/system.h" //system::sleepMilliseconds

#include <gtest/gtest.h>

struct MutexTestStruct {
    a_util::concurrency::mutex mtx;
    void Work()
    {
        mtx.lock();
        mtx.unlock();
    }
};

TEST(mutex_test, TestMutex)
{
    MutexTestStruct t;
    ASSERT_TRUE(t.mtx.try_lock());

    {
        a_util::concurrency::thread th(&MutexTestStruct::Work, &t);
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
