/**
 * @file
 * Concurrency APIs
 *
 * Copyright @ 2021 VW Group. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef HAVE_SHARED_MUTEX

#include <a_util/concurrency.h>

#include <cstddef>

namespace a_util {
namespace concurrency {
static const std::int32_t write_lock_active = 1u << 31;     // "-2^31"
static const std::int32_t max_readers = ~write_lock_active; // "+2^31"

struct shared_mutex::impl {
    mutex mtx;
    int32_t flag;
    condition_variable cond1;
    condition_variable cond2;
};

shared_mutex::shared_mutex() : p(new impl())
{
    p->flag = 0;
}

shared_mutex::~shared_mutex()
{
    // finalize any locking operations underway
    unique_lock<mutex> _(p->mtx);
}

void shared_mutex::lock()
{
    unique_lock<mutex> mtx(p->mtx);

    // wait if already write-locked
    while (p->flag & write_lock_active) {
        p->cond1.wait(mtx);
    }

    // set writer-bit
    p->flag |= write_lock_active;

    // wait for remaining reader-locks to unlock
    while (p->flag & max_readers) {
        p->cond2.wait(mtx);
    }
}

bool shared_mutex::try_lock()
{
    unique_lock<mutex> _(p->mtx);

    // no need for bit-fiddling, simply check if no locks are active at all
    if (p->flag == 0) {
        p->flag = write_lock_active;
        return true;
    }
    return false;
}

void shared_mutex::unlock()
{
    unique_lock<mutex> _(p->mtx);
    // simple, since its an exclusive lock
    p->flag = 0;

    // notify any new locks that may be awaiting the unlock
    p->cond1.notify_all();
}

void shared_mutex::lock_shared()
{
    unique_lock<mutex> mtx(p->mtx);
    // wait for any write-locks to unlock and (very improbable) while all reader slots are taken
    while ((p->flag & write_lock_active) || (p->flag & max_readers) == max_readers) {
        p->cond1.wait(mtx);
    }

    /// get new actual number of readers (without the write-lock bit)
    int32_t readers = (p->flag & max_readers) + 1;

    // set new reader count
    p->flag &= ~max_readers;
    p->flag |= readers;
}

bool shared_mutex::try_lock_shared()
{
    unique_lock<mutex> _(p->mtx);

    // get current number of readers
    int32_t readers = p->flag & max_readers;

    // only lock if no write lock is active and there is a reader slot available
    if (!(p->flag & write_lock_active) && readers != max_readers) {
        // increment and set new reader count
        readers++;
        p->flag &= ~max_readers;
        p->flag |= readers;
        return true;
    }
    return false;
}

void shared_mutex::unlock_shared()
{
    unique_lock<mutex> _(p->mtx);

    // get, decrement & set new number of readers
    int32_t readers = (p->flag & max_readers) - 1;
    if (readers >= 0) {
        p->flag &= ~max_readers;
        p->flag |= readers;

        if (p->flag & write_lock_active) {
            // awake the waiting write-lock if we were the last reader
            if (readers == 0) {
                p->cond2.notify_one();
            }
        }
        else {
            // notify any waiting readers that there is a free slot now
            if (readers == max_readers - 1) {
                p->cond1.notify_one();
            }
        }
    }
}

} // namespace concurrency
} // namespace a_util

#else

// provide dummy (with generated namespace to avoid clashes) to silence the linker (4221)
namespace {
char dummy;
};

#endif
