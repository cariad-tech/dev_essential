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

#include <a_util/concurrency/chrono.h>

#include <gtest/gtest.h>

using namespace a_util;

TEST(chrono_test, TestDurationCast)
{
    // From/To: microseconds
    {
        // nanoseconds <-> microseconds
        {
            chrono::nanoseconds nsx2(25 * 1000 * 1000);
            chrono::microseconds us1(25 * 1000);
            chrono::microseconds us2 = chrono::duration_cast<chrono::microseconds>(nsx2);
            ASSERT_EQ(us1.count(), us2.count());

            chrono::nanoseconds nsx1 = chrono::duration_cast<chrono::nanoseconds>(us1);
            ASSERT_EQ(nsx1.count(), nsx2.count());
        }

        // microseconds <-> microseconds
        {
            chrono::microseconds usx2(25 * 1000);
            chrono::microseconds us1(25 * 1000);
            chrono::microseconds us2 = chrono::duration_cast<chrono::microseconds>(usx2);
            ASSERT_EQ(us1.count(), us2.count());

            chrono::microseconds usx1 = chrono::duration_cast<chrono::microseconds>(us1);
            ASSERT_EQ(usx1.count(), usx2.count());
        }

        // milliseconds <-> microseconds
        {
            chrono::milliseconds msx2(25);
            chrono::microseconds us1(25 * 1000);
            chrono::microseconds us2 = chrono::duration_cast<chrono::microseconds>(msx2);
            ASSERT_EQ(us1.count(), us2.count());

            chrono::milliseconds msx1 = chrono::duration_cast<chrono::milliseconds>(us1);
            ASSERT_EQ(msx1.count(), msx2.count());
        }

        // seconds <-> microseconds
        {
            chrono::seconds sx2(2);
            chrono::microseconds us1(2 * 1000 * 1000);
            chrono::microseconds us2 = chrono::duration_cast<chrono::microseconds>(sx2);
            ASSERT_EQ(us1.count(), us2.count());

            chrono::seconds sx1 = chrono::duration_cast<chrono::seconds>(us1);
            ASSERT_EQ(sx1.count(), sx2.count());
        }

        // minutes <-> microseconds
        {
            chrono::minutes mx2(2);
            chrono::microseconds us1(2 * 60 * 1000 * 1000);
            chrono::microseconds us2 = chrono::duration_cast<chrono::microseconds>(mx2);
            ASSERT_EQ(us1.count(), us2.count());

            chrono::minutes mx1 = chrono::duration_cast<chrono::minutes>(us1);
            ASSERT_EQ(mx1.count(), mx2.count());
        }

        // minutes <-> microseconds
        {
            chrono::minutes mx2(2);
            chrono::microseconds us1(2 * 60 * 1000 * 1000);
            chrono::microseconds us2 = chrono::duration_cast<chrono::microseconds>(mx2);
            ASSERT_EQ(us1.count(), us2.count());

            chrono::minutes mx1 = chrono::duration_cast<chrono::minutes>(us1);
            ASSERT_EQ(mx1.count(), mx2.count());
        }

        // hours <-> microseconds
        {
            chrono::hours hx2(2);
            chrono::microseconds us1(static_cast<intmax_t>(1) * 2 * 60 * 60 * 1000 * 1000);
            chrono::microseconds us2 = chrono::duration_cast<chrono::microseconds>(hx2);
            ASSERT_EQ(us1.count(), us2.count());

            chrono::hours hx1 = chrono::duration_cast<chrono::hours>(us1);
            ASSERT_EQ(hx1.count(), hx2.count());
        }
    }
}
