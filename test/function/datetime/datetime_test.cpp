/**
 * @file
 * Date and time test implementation
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

#include <a_util/datetime.h>
#include <a_util/system.h>

#include <gtest/gtest.h>

#include <chrono>

#if (defined(_MSC_VER) && (_MSC_VER <= 1600)) // vc100, C++03

timestamp_t getMicroSecondsSinceUnixTimeOrigin()
{
    // C++03 Windows, see: https://stackoverflow.com/a/1695332
    FILETIME file_time = {};
    GetSystemTimeAsFileTime(&file_time);
    timestamp_t now_ut = // to nanoseconds
        (LONGLONG)file_time.dwLowDateTime + ((LONGLONG)(file_time.dwHighDateTime) << 32LL);
    now_ut -= 116444736000000000LL; // to unix_time
    now_ut /= 10;                   // to microseconds
    return now_ut;
}

#elif (!defined(_MSC_VER) && (__cplusplus < 201103L)) // gcc, clang or so, C++03

#include <cmath>
#include <ctime>

timestamp_t getMicroSecondsSinceUnixTimeOrigin()
{
    // C++03 probably linux, see: https://stackoverflow.com/a/17371925
    struct timespec spec = {};
    clock_gettime(CLOCK_REALTIME, &spec);       // to seconds and nanoseconds
    timestamp_t now_ut = spec.tv_sec * 1000000; // seconds to microseconds
    now_ut += round(spec.tv_nsec / 1.0e3);      // nanoseconds to microseconds
    return now_ut;
}

#else //> C++11
timestamp_t getMicroSecondsSinceUnixTimeOrigin()
{
    timestamp_t now_ut = std::chrono::duration_cast<std::chrono::microseconds>(
                             std::chrono::system_clock::now().time_since_epoch())
                             .count();
    return now_ut;
}
#endif

using namespace a_util;
using namespace a_util::datetime;

TEST(datetime_test, TestTimestampConversion)
{
    /*------------------------------------------------------------------------
     *  compare chrono and a_util timestamps
     *------------------------------------------------------------------------*/
    DateTime system_date_time = a_util::datetime::getCurrentSystemDateTime();
    const timestamp_t now_ut = getMicroSecondsSinceUnixTimeOrigin();

    const timestamp_t system_timestamp_ut =
        system_date_time.toTimestamp(TimestampReference::MicroSecondsSinceUnixTimeOrigin);

    /*------------------------------------------------------------------------
     *  check if the DateTime object retrieved with the
     *  getCurrentSystemDateTime() function outputs (nearly) the same point in
     *  time when converted to unix time, like the unix time based timestamp we
     *  get with the help of the std::chrono library
     *------------------------------------------------------------------------*/
    EXPECT_NEAR((double)system_timestamp_ut, (double)now_ut, 2000);

    DateTime now_date_time;
    now_date_time.set(now_ut, TimestampReference::MicroSecondsSinceUnixTimeOrigin);

    /*------------------------------------------------------------------------
     *  check if we get (nearly) the same DateTime object, if we set its member
     *  variables with a unix time based timestamp, and initiate a time
     *  reference conversion with the TimestampReference parameter
     *------------------------------------------------------------------------*/
    ASSERT_EQ(system_date_time.getYear(), now_date_time.getYear());
    ASSERT_EQ(system_date_time.getMonth(), now_date_time.getMonth());
    ASSERT_EQ(system_date_time.getDay(), now_date_time.getDay());
    ASSERT_EQ(system_date_time.getHour(), now_date_time.getHour());
    ASSERT_EQ(system_date_time.getMinute(), now_date_time.getMinute());
    EXPECT_NEAR(system_date_time.getSecond(), now_date_time.getSecond(), 1);
    EXPECT_NEAR(system_date_time.getMicrosecond(), now_date_time.getMicrosecond(), 2000);

    /*------------------------------------------------------------------------
     *  test conversion with the beginning of unix time measurement
     *------------------------------------------------------------------------*/
    DateTime ut_date = a_util::datetime::DateTime(1970, 1, 1, 0, 0, 0, 0);
    timestamp_t ut_date_jd_timestamp = ut_date.toTimestamp();
    ASSERT_EQ(ut_date_jd_timestamp, 210866803200000000LL);

    timestamp_t ut_date_ut_timestamp =
        ut_date.toTimestamp(TimestampReference::MicroSecondsSinceUnixTimeOrigin);
    ASSERT_EQ(ut_date_ut_timestamp, 0);

    DateTime ut_date_converted;
    ut_date_converted.set(ut_date_ut_timestamp,
                          TimestampReference::MicroSecondsSinceUnixTimeOrigin);

    ASSERT_EQ(ut_date_converted.getYear(), ut_date.getYear());
    ASSERT_EQ(ut_date_converted.getMonth(), ut_date.getMonth());
    ASSERT_EQ(ut_date_converted.getDay(), ut_date.getDay());
    ASSERT_EQ(ut_date_converted.getHour(), ut_date.getHour());
    ASSERT_EQ(ut_date_converted.getMinute(), ut_date.getMinute());
    ASSERT_EQ(ut_date_converted.getSecond(), ut_date.getSecond());
    ASSERT_EQ(ut_date_converted.getMicrosecond(), ut_date.getMicrosecond());

    /*------------------------------------------------------------------------
     *  test conversion with an arbitrary date
     *------------------------------------------------------------------------*/
    DateTime date = a_util::datetime::DateTime(2017, 12, 12, 13, 11, 4, 432);

    const timestamp_t date_ut_timestamp =
        date.toTimestamp(TimestampReference::MicroSecondsSinceUnixTimeOrigin);

    DateTime date_converted;
    date_converted.set(date_ut_timestamp, TimestampReference::MicroSecondsSinceUnixTimeOrigin);

    ASSERT_EQ(date_converted.getYear(), date.getYear());
    ASSERT_EQ(date_converted.getMonth(), date.getMonth());
    ASSERT_EQ(date_converted.getDay(), date.getDay());
    ASSERT_EQ(date_converted.getHour(), date.getHour());
    ASSERT_EQ(date_converted.getMinute(), date.getMinute());
    ASSERT_EQ(date_converted.getSecond(), date.getSecond());
    ASSERT_EQ(date_converted.getMicrosecond(), date.getMicrosecond());
}

TEST(datetime_test, TestConstruction)
{
    DateTime date_time1;
    ASSERT_EQ(date_time1.getMicrosecond(), 0);
    ASSERT_EQ(date_time1.getYear(), 0);
    ASSERT_EQ(date_time1.getSecond(), 0);
    ASSERT_EQ(date_time1.getMonth(), 0);
    ASSERT_EQ(date_time1.getMinute(), 0);
    ASSERT_EQ(date_time1.getHour(), 0);
    ASSERT_EQ(date_time1.getDay(), 0);

    DateTime date_time2(1, 2, 3, 4, 5, 6, 7);
    ASSERT_EQ(date_time2.getMicrosecond(), 7);
    ASSERT_EQ(date_time2.getYear(), 1);
    ASSERT_EQ(date_time2.getSecond(), 6);
    ASSERT_EQ(date_time2.getMonth(), 2);
    ASSERT_EQ(date_time2.getMinute(), 5);
    ASSERT_EQ(date_time2.getHour(), 4);
    ASSERT_EQ(date_time2.getDay(), 3);

    DateTime date_time3(date_time2);
    ASSERT_EQ(date_time3.getMicrosecond(), 7);
    ASSERT_EQ(date_time3.getYear(), 1);
    ASSERT_EQ(date_time3.getSecond(), 6);
    ASSERT_EQ(date_time3.getMonth(), 2);
    ASSERT_EQ(date_time3.getMinute(), 5);
    ASSERT_EQ(date_time3.getHour(), 4);
    ASSERT_EQ(date_time3.getDay(), 3);
}

TEST(datetime_test, TestFunction)
{
    // test setter and getter
    {
        DateTime date_time;
        date_time.set(1, 2, 3, 4, 5, 6, 7);
        ASSERT_EQ(date_time.getMicrosecond(), 7);
        ASSERT_EQ(date_time.getYear(), 1);
        ASSERT_EQ(date_time.getSecond(), 6);
        ASSERT_EQ(date_time.getMonth(), 2);
        ASSERT_EQ(date_time.getMinute(), 5);
        ASSERT_EQ(date_time.getHour(), 4);
        ASSERT_EQ(date_time.getDay(), 3);

        date_time.set(0, 0, 0, 0, 0, 0, 0);
        ASSERT_EQ(date_time.getMicrosecond(), 0);
        ASSERT_EQ(date_time.getYear(), 0);
        ASSERT_EQ(date_time.getSecond(), 0);
        ASSERT_EQ(date_time.getMonth(), 0);
        ASSERT_EQ(date_time.getMinute(), 0);
        ASSERT_EQ(date_time.getHour(), 0);
        ASSERT_EQ(date_time.getDay(), 0);

        // check whether timestamp of zero is zero
        ASSERT_EQ(date_time.toTimestamp(), 0);
    }

    // Julian Day Number to Gregorian date check and conversion
    // since nobody ever will need this stuff it's just enough to compare
    // whether the conversion is reversible or not.
    {
        timestamp_t tsT1 = timestamp_t((2299161LL + 3) * 86400000000LL);
        DateTime date_time;
        date_time.set(tsT1);
        ASSERT_EQ(date_time.toTimestamp(), tsT1);
    }

    // test FromTimeStamp
    {
        DateTime date_time(1, 2, 3, 4, 5, 6, 7);
        date_time.set(date_time.toTimestamp());
        ASSERT_EQ(date_time.getMicrosecond(), 7);
        ASSERT_EQ(date_time.getYear(), 1);
        ASSERT_EQ(date_time.getSecond(), 6);
        ASSERT_EQ(date_time.getMonth(), 2);
        ASSERT_EQ(date_time.getMinute(), 5);
        ASSERT_EQ(date_time.getHour(), 4);
        ASSERT_EQ(date_time.getDay(), 3);
    }

    {
        {
            DateTime date_time(2009, 03, 04, 16, 30, 60, 999);
            std::string test =
                date_time.format("%a %A %b %B %d %H %I %j %m %M %S %U %w %W %x %X %y %Y");
            ASSERT_EQ(
                test,
                "Wed Wednesday Mar March 04 16 04 063 03 31 00 09 3 09 03/04/09 16:31:00 09 2009");
        }
#ifndef __STRICT_ANSI__
        {
            DateTime date_time(2009, 03, 04, 06, 01, 60, 999);
            std::string test = date_time.format(
                "%#a %#A %#b %#B %#d %#H %#I %#j %#m %#M %#S %#U %#w %#W %#x %#X %#y %#Y");
#ifdef WIN32
            ASSERT_EQ(test,
                      "Wed Wednesday Mar March 4 6 6 63 3 2 0 9 3 9 Wednesday, March 04, 2009 "
                      "06:02:00 9 2009");
#else
            // Linux
            ASSERT_EQ(
                test,
                "WED WEDNESDAY MAR MARCH 04 06 06 063 03 02 00 09 3 09 03/04/09 06:02:00 09 2009");
#endif // WIN32
        }
#endif // __STRICT_ANSI__
    }
}

TEST(datetime_test, TestDate)
{
    {
        Date date;
        ASSERT_EQ(0, date.getDay());
        ASSERT_EQ(0, date.getMonth());
        ASSERT_EQ(0, date.getYear());

        Date date2 = date;
        ASSERT_EQ(date, date2);
        date2.setDay(1);
        ASSERT_TRUE(date != date2);
    }

    {
        Date date;
        date.setDay(1);
        ASSERT_EQ(1, date.getDay());
        date.setMonth(2);
        ASSERT_EQ(2, date.getMonth());
        date.setYear(3);
        ASSERT_EQ(3, date.getYear());
    }

    // just check whether this methods return anything useful
    {
        Date date1 = datetime::getCurrentSystemDate();
        Date date2 = datetime::getCurrentLocalDate();
        ASSERT_TRUE(date1.getYear() != 0);
        ASSERT_TRUE(date2.getYear() != 0);
        ASSERT_EQ(date1.getYear(), date2.getYear());
    }

    // test format
    {
        Date date(2012, 9, 12);
        std::string date_str = date.format("%Y-%m-%d");
        ASSERT_EQ(date_str, "2012-09-12");

        date = Date(1969, 12, 31);
        date_str = date.format("%Y-%m-%d");
        ASSERT_EQ(date_str, "1969-12-31");

        // Date doesn't check the validity of the date
        // but Format will adjust it and creates a valid date
        date.set(2000, 13, 32);
        date_str = date.format("%Y-%m-%d");
        ASSERT_EQ(date_str, "2001-02-01");
    }

    {
        // functional tests: format dd.mm.yyyy
        Date date;
        ASSERT_TRUE(date.set("14.02.2013"));
        ASSERT_EQ(date.getDay(), 14);
        ASSERT_EQ(date.getMonth(), 2);
        ASSERT_EQ(date.getYear(), 2013);

        // functional tests: format yyyy.mm.dd
        date.set("2013.02.14");
        ASSERT_EQ(date.getDay(), 14);
        ASSERT_EQ(date.getMonth(), 2);
        ASSERT_EQ(date.getYear(), 2013);

        // error test: the date string length is 0
        ASSERT_FALSE(date.set(""));

        // error test: the date string includes any unsupported character
        ASSERT_FALSE(date.set("14:02:2013"));

        // error test: the date string is not zero-padded
        ASSERT_FALSE(date.set("14.2.13"));

        // error test: the date string format is not supported
        ASSERT_FALSE(date.set("14.02.13"));

        // error test: the date string format is not supported
        ASSERT_FALSE(date.set("14.02.2013."));

        // error test: month out of range
        ASSERT_FALSE(date.set("14.00.2013"));
        ASSERT_FALSE(date.set("14.13.2013"));

        // error test: day out of range
        ASSERT_FALSE(date.set("00.02.2013"));
        ASSERT_FALSE(date.set("39.02.2013"));
    }
}

bool IsTimeDifferenceInExpectedRange(Time& time1,
                                     Time& time2,
                                     int64_t range_start,
                                     int64_t range_end)
{
    int64_t nUS1 =
        int64_t((time1.getHour() * 3600 + time1.getMinute()) * 60 + time1.getSecond()) * 1000000 +
        time1.getMicrosecond();
    int64_t nUS2 =
        int64_t((time2.getHour() * 3600 + time2.getMinute()) * 60 + time2.getSecond()) * 1000000 +
        time2.getMicrosecond();
    int64_t nUSDiff = nUS2 - nUS1;
    if (nUSDiff < 0)
        nUSDiff += 24 * 60 * 60 *
                   1000000LL; //???
                              // should be anywhere in the range between range_start and range_end
    return nUSDiff >= range_start && nUSDiff <= range_end;
}

TEST(datetime_test, TestTime)
{
    {
        Time time;
        ASSERT_EQ(0, time.getHour());
        ASSERT_EQ(0, time.getMinute());
        ASSERT_EQ(0, time.getSecond());

        Time time2 = time;
        ASSERT_EQ(time, time2);
        time2.setHour(1);
        ASSERT_TRUE(time != time2);
    }

    {
        Time time(1, 2, 3, 4);
        ASSERT_EQ(1, time.getHour());
        ASSERT_EQ(2, time.getMinute());
        ASSERT_EQ(3, time.getSecond());
        Time oOtherTime(time);
        ASSERT_EQ(1, oOtherTime.getHour());
        ASSERT_EQ(2, oOtherTime.getMinute());
        ASSERT_EQ(3, oOtherTime.getSecond());
        Time oCopyTime = time;
        ASSERT_EQ(1, oCopyTime.getHour());
        ASSERT_EQ(2, oCopyTime.getMinute());
        ASSERT_EQ(3, oCopyTime.getSecond());
    }

    // test assignment operator
    {
        Time time1(1, 2, 3, 4);
        Time time2;
        time2 = time1;
        ASSERT_EQ(time1.getHour(), time2.getHour());
        ASSERT_EQ(time1.getMinute(), time2.getMinute());
        ASSERT_EQ(time1.getSecond(), time2.getSecond());
        ASSERT_EQ(time1.getMicrosecond(), time2.getMicrosecond());
    }

    // test Time::Format
    {
        Time time(12, 30, 45, 0);
        std::string time_str = time.format("%H:%M:%S");
        ASSERT_EQ(time_str, "12:30:45");

        time.set(0, 0, 0, 0);
        time_str = time.format("%H:%M:%S");
        ASSERT_EQ(time_str, "00:00:00");

        // Time doesn't check the validy of the parameter
        // but Format will adjust them anyway to a valid time format
        time.set(0, 0, 3661, 0);
        time_str = time.format("%H:%M:%S");
        ASSERT_EQ(time_str, "01:01:01");
    }

    // at least some plausibility tests for GetCurrentTime() and GetSystemTime()
    {
        Time time1 = datetime::getCurrentLocalTime();
        a_util::system::sleepMilliseconds(2000);
        Time time2 = datetime::getCurrentLocalTime();
        ASSERT_TRUE(IsTimeDifferenceInExpectedRange(time1, time2, 1000000, 3000000));

        Time time3 = datetime::getCurrentSystemTime();
        a_util::system::sleepMilliseconds(2000);
        Time time4 = datetime::getCurrentSystemTime();
        ASSERT_TRUE(IsTimeDifferenceInExpectedRange(time3, time4, 1000000, 3000000));
    }

    // test Time::GetTimeFromString(..)
    {
        // functional tests: format dd.mm.yyyy
        Time time;
        ASSERT_TRUE(time.set("16:05:42"));
        ASSERT_EQ(time.getHour(), 16);
        ASSERT_EQ(time.getMinute(), 5);
        ASSERT_EQ(time.getSecond(), 42);

        // error test: the time string length is 0
        ASSERT_FALSE(time.set(""));

        // error test: the time string includes any unsupported character
        ASSERT_FALSE(time.set("16.05.42"));

        // error test: the time string is not zero-padded
        ASSERT_FALSE(time.set("16:5:42"));

        // error test: the time string format is not supported
        ASSERT_FALSE(time.set("16:05:421"));

        // error test: the time string format is not supported
        ASSERT_FALSE(time.set("16:05:42:"));

        // error test: hour out of range
        ASSERT_FALSE(time.set("24:05:42"));

        // error test: minute out of range
        ASSERT_FALSE(time.set("16:60:42"));

        // error test: second out of range
        ASSERT_FALSE(time.set("16:60:62"));
    }
}

TEST(datetime_test, TestParsing)
{
    DateTime dt;
    std::string date_str1 = "Dec 22 2009, 10:52:01";
    std::string strFmt1 = "%b %d %Y, %H %M %S";
    dt.set(strFmt1, date_str1);

    ASSERT_EQ(dt.getYear(), 2009);
    ASSERT_EQ(dt.getMonth(), 12);
    ASSERT_EQ(dt.getDay(), 22);
    ASSERT_EQ(dt.getHour(), 10);
    ASSERT_EQ(dt.getMinute(), 52);
    ASSERT_EQ(dt.getSecond(), 1);

    DateTime oSysDateTime = datetime::getCurrentSystemDateTime();
    std::string strSysDateTime = oSysDateTime.format(strFmt1);
    dt.set(strFmt1, strSysDateTime);

    ASSERT_EQ(dt.getYear(), oSysDateTime.getYear());
    ASSERT_EQ(dt.getMonth(), oSysDateTime.getMonth());
    ASSERT_EQ(dt.getDay(), oSysDateTime.getDay());
    ASSERT_EQ(dt.getHour(), oSysDateTime.getHour());
    ASSERT_EQ(dt.getMinute(), oSysDateTime.getMinute());
    ASSERT_EQ(dt.getSecond(), oSysDateTime.getSecond());

    DateTime oCurDateTime = datetime::getCurrentLocalDateTime();
    std::string strCurDateTime = oCurDateTime.format(strFmt1);
    dt.set(strFmt1, strCurDateTime);

    ASSERT_EQ(dt.getYear(), oCurDateTime.getYear());
    ASSERT_EQ(dt.getMonth(), oCurDateTime.getMonth());
    ASSERT_EQ(dt.getDay(), oCurDateTime.getDay());
    ASSERT_EQ(dt.getHour(), oCurDateTime.getHour());
    ASSERT_EQ(dt.getMinute(), oCurDateTime.getMinute());
    ASSERT_EQ(dt.getSecond(), oCurDateTime.getSecond());
}

TEST(datetime_test, TestAssignment)
{
    DateTime dt1;
    DateTime dt2;

    dt1 = dt2;

    ASSERT_EQ(dt1.getYear(), dt2.getYear());
    ASSERT_EQ(dt1.getMonth(), dt2.getMonth());
    ASSERT_EQ(dt1.getDay(), dt2.getDay());
    ASSERT_EQ(dt1.getHour(), dt2.getHour());
    ASSERT_EQ(dt1.getMinute(), dt2.getMinute());
    ASSERT_EQ(dt1.getSecond(), dt2.getSecond());

    ASSERT_EQ(dt1, dt2);
    dt2.setDay(1);
    ASSERT_TRUE(dt1 != dt2);
}

TEST(datetime_test, TestInvalidFormatDirective)
{
    DateTime dateTime = datetime::getCurrentLocalDateTime();
    // an empty format string should return an empty result
    ASSERT_TRUE(dateTime.format("").empty());

    ASSERT_THROW(dateTime.format("%s"), std::invalid_argument);
    ASSERT_EQ(dateTime.format("%%"), "%");
}
