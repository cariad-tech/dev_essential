/**
 * @file
 * Date and time handling classes
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
#include <a_util/memory/memory.h>
#include <a_util/memory/memorybuffer.h>
#include <a_util/regex.h>
#include <a_util/strings/strings_convert_decl.h>
#include <a_util/strings/strings_functions.h>

#ifdef WIN32

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

#else

#include <sys/time.h>

#endif

#include <algorithm>
#include <cctype>
#include <cmath>
#include <ctime>
#include <map>
#include <stdexcept>

namespace a_util {
namespace datetime {
namespace detail {
namespace {
/// Number of microseconds that have elapsed between julian day and unix time.
const timestamp_t timeDifferenceJulian2Unix = 210866803200000000LL;

static const timestamp_t usecs_per_day = static_cast<timestamp_t>(86400000000LL);
static const std::string digit_chars = "0123456789";

static std::string FormatTimeInfo(tm time_info, const std::string& format)
{
    if (format.empty()) {
        return strings::empty_string;
    }

    // Check the format of the string. If any unsupported character preceeded
    // by % was found return an error string and assert in debug mode
#if defined(__STRICT_ANSI__) && defined(__QNX__)
    regex::RegularExpression regex("\\%[^aAbBcdHIjmMpSUwWxXyY%]");
#else
    regex::RegularExpression regex("\\%[^aAbBcdHIjmMpSUwWxXyY%#]");
#endif
    int consumed = 0;

    if (regex.match(format, regex::RegularExpression::AT_Unanchored, consumed)) {
        throw std::invalid_argument((std::string("Invalid format string: ") + format).c_str());
    }

    time_info.tm_year -= 1900;
    time_info.tm_mon -= 1;

    // paste in additional info, take care of 64 bit compatibility
#ifdef _MSC_VER
    _mktime64(&time_info);
#else
    mktime(&time_info);
#endif

    std::string date_time;
    size_t buffer_size = format.size();
    size_t char_count = 0;

    while (char_count == 0) {
        buffer_size += 512; // append buffer for date/time macros

        memory::MemoryBuffer buffer(buffer_size);
        char_count = strftime((char*)buffer.getPtr(), buffer_size, format.c_str(), &time_info);
        if (char_count > 0) {
            date_time = (const char*)buffer.getPtr();
        }
    }

    date_time.erase(char_count);
    return date_time;
}

int ParseMonth(const char*& it, const char* end)
{
    static std::map<std::string, int> months;
    if (months.empty()) {
        months["January"] = 1;
        months["February"] = 2;
        months["March"] = 3;
        months["April"] = 4;
        months["May"] = 5;
        months["June"] = 6;
        months["July"] = 7;
        months["August"] = 8;
        months["September"] = 9;
        months["October"] = 10;
        months["November"] = 11;
        months["December"] = 12;

        months["Jan"] = 1;
        months["Feb"] = 2;
        months["Mar"] = 3;
        months["Apr"] = 4;

        months["Jun"] = 6;
        months["Jul"] = 7;
        months["Aug"] = 8;
        months["Sep"] = 9;
        months["Oct"] = 10;
        months["Nov"] = 11;
        months["Dec"] = 12;
    }

    std::string month;
    while ((it != end && std::isspace(*it)) || std::ispunct(*it)) {
        ++it;
    }

    bool is_first = true;
    while (it != end && std::isalpha(*it)) {
        const char ch = (*it++);
        if (is_first) {
            month += static_cast<char>(std::toupper(ch));
            is_first = false;
        }
        else {
            month += static_cast<char>(std::tolower(ch));
        }
    }

    return months[month];
}

int ParseAMPM(const char*& it, const char* end, int hour)
{
    std::string am_pm;
    while ((it != end && std::isspace(*it)) || std::ispunct(*it)) {
        ++it;
    };

    while (it != end && std::isalpha(*it)) {
        const char ch = (*it++);
        am_pm += static_cast<char>(std::toupper(ch));
    }
    if (am_pm == "AM") {
        if (hour == 12) {
            return 0;
        }
        else {
            return hour;
        }
    }
    else if (am_pm == "PM") {
        if (hour < 12) {
            return hour + 12;
        }
        else {
            return hour;
        }
    }
    return -1;
}

void ParseDateTime(const std::string& format, const std::string& str, DateTime& dt)
{
#define IS_NUMBER(c) (digit_chars.find((c), 0) != std::string::npos)

#define SKIP_JUNK()                                                                                \
    while (it != end && !IS_NUMBER(*it))                                                           \
    ++it

#define PARSE_NUMBER(var)                                                                          \
    while (it != end && IS_NUMBER(*it))                                                            \
    var = var * 10 + ((*it++) - '0')

#define PARSE_NUMBER_N(var, n)                                                                     \
    {                                                                                              \
        int i = 0;                                                                                 \
        while (i++ < n && it != end && IS_NUMBER(*it))                                             \
            var = var * 10 + ((*it++) - '0');                                                      \
    }

    dt = DateTime();

    const char* it = str.c_str();
    const char* end = it + str.size();
    const char* itf = format.c_str();
    const char* endf = itf + format.size();

    int year = 0, month = 0, day = 0, hour = 0, minute = 0, second = 0;

    while (itf != endf && it != end) {
        if (*itf == '%') {
            if (++itf != endf) {
                switch (*itf) {
                case 'b':
                case 'B':
                    month = ParseMonth(it, end);
                    break;
                case 'm':
                    SKIP_JUNK();
                    PARSE_NUMBER_N(month, 2);
                    break;
                case 'd':
                    SKIP_JUNK();
                    PARSE_NUMBER_N(day, 2);
                    break;
                case 'y':
                    SKIP_JUNK();
                    PARSE_NUMBER_N(year, 2);
                    if (year >= 69) {
                        year += 1900;
                    }
                    else {
                        year += 2000;
                    }
                    break;
                case 'Y':
                    SKIP_JUNK();
                    PARSE_NUMBER_N(year, 4);
                    break;
                case 'H':
                case 'I':
                    SKIP_JUNK();
                    PARSE_NUMBER_N(hour, 2);
                    break;
                case 'p':
                case 'P':
                    hour = ParseAMPM(it, end, hour);
                    break;
                case 'M':
                    SKIP_JUNK();
                    PARSE_NUMBER_N(minute, 2);
                    break;
                case 'S':
                    SKIP_JUNK();
                    PARSE_NUMBER_N(second, 2);
                    break;
                }
                ++itf;
            }
        }
        else {
            ++itf;
        }
    }

    dt.setYear(year);
    dt.setMonth(month);
    dt.setDay(day);
    dt.setHour(hour);
    dt.setMinute(minute);
    dt.setSecond(second);
    dt.setMicrosecond(0);
}

} // namespace
} // namespace detail

Date::Date() : _day(0), _month(0), _year(0)
{
}

Date::Date(int year, int month, int day) : _day(day), _month(month), _year(year)
{
}

void Date::set(int year, int month, int day)
{
    _year = year;
    _month = month;
    _day = day;
}

void Date::setDay(int day)
{
    _day = day;
}

int Date::getDay() const
{
    return _day;
}

void Date::setMonth(int month)
{
    _month = month;
}

int Date::getMonth() const
{
    return _month;
}

void Date::setYear(int year)
{
    _year = year;
}

int Date::getYear() const
{
    return _year;
}

std::string Date::format(const std::string& format) const
{
    // fill ANSI time structure
    struct tm time_info;
    memory::set(&time_info, sizeof(time_info), 0, sizeof(time_info));
    time_info.tm_isdst = -1;
    time_info.tm_year = _year;
    time_info.tm_mon = _month;
    time_info.tm_mday = _day;
    time_info.tm_hour = 0;
    time_info.tm_min = 0;
    time_info.tm_sec = 0;

    return detail::FormatTimeInfo(time_info, format);
}

bool Date::set(const std::string& date)
{
    if (date.empty()) {
        return false;
    }

    // check the date string, if any unsupported character are included
    a_util::regex::RegularExpression regex("[^.0123456789]");
    int consumed = 0;
    if (regex.match(date, a_util::regex::RegularExpression::AT_Unanchored, consumed)) {
        return false;
    }

    // check the format of the date string
    size_t num_sep = std::count(date.begin(), date.end(), '.');
    if (num_sep != 2) {
        // format of the date string is invalid
        return false;
    }

    std::vector<std::string> vec_parts;
    vec_parts = strings::split(date, ".");
    if (vec_parts.size() == 3) {
        std::string day, month, year;
        if (vec_parts[0].size() == 2 && vec_parts[1].size() == 2 && vec_parts[2].size() == 4) {
            day = vec_parts[0];
            month = vec_parts[1];
            year = vec_parts[2];
        }
        else if (vec_parts[0].size() == 4 && vec_parts[1].size() == 2 && vec_parts[2].size() == 2) {
            year = vec_parts[0];
            month = vec_parts[1];
            day = vec_parts[2];
        }
        else {
            return false;
        }

        std::uint32_t y, m, d;
        if (!strings::toUInt32(year, y) || !strings::toUInt32(month, m) ||
            !strings::toUInt32(day, d)) {
            return false;
        }

        if (y > 9999 || m < 1 || m > 12 || d < 1 || d > 31) {
            return false;
        }

        set(y, m, d);
    }
    else {
        // format of the date string is invalid
        return false;
    }

    return true;
}

Date getCurrentLocalDate()
{
    Date date;

#ifdef WIN32
    SYSTEMTIME system_time;
    ::GetLocalTime(&system_time);

    date.setYear(system_time.wYear);
    date.setMonth(system_time.wMonth);
    date.setDay(system_time.wDay);
#else
    struct timeval current_time;
    ::gettimeofday(&current_time, nullptr);
    struct tm system_time;
    ::localtime_r(&current_time.tv_sec, &system_time);

    date.setYear(system_time.tm_year + 1900);
    date.setMonth(system_time.tm_mon + 1);
    date.setDay(system_time.tm_mday);
#endif

    return date;
}

Date getCurrentSystemDate()
{
    Date date;

#ifdef WIN32
    SYSTEMTIME system_time;
    ::GetSystemTime(&system_time);

    date.setYear(system_time.wYear);
    date.setMonth(system_time.wMonth);
    date.setDay(system_time.wDay);
#else
    struct timeval currrent_time;
    ::gettimeofday(&currrent_time, nullptr);
    struct tm system_time;
    ::gmtime_r(&currrent_time.tv_sec, &system_time);

    date.setYear(system_time.tm_year + 1900);
    date.setMonth(system_time.tm_mon + 1);
    date.setDay(system_time.tm_mday);
#endif

    return date;
}

bool operator==(const Date& a, const Date& b)
{
    return a.getYear() == b.getYear() && a.getMonth() == b.getMonth() && a.getDay() == b.getDay();
}

bool operator!=(const Date& a, const Date& b)
{
    return !(a == b);
}

Time::Time() : _hour(0), _minute(0), _second(0), _microsecond(0)
{
}

void Time::set(int hour, int minute, int second, int microseconds /*= 0*/)
{
    _hour = hour;
    _minute = minute;
    _second = second;
    _microsecond = microseconds;
}

void Time::setHour(int hour)
{
    _hour = hour;
}

int Time::getHour() const
{
    return _hour;
}

void Time::setMinute(int minute)
{
    _minute = minute;
}

int Time::getMinute() const
{
    return _minute;
}

void Time::setSecond(int second)
{
    _second = second;
}

int Time::getSecond() const
{
    return _second;
}

void Time::setMicrosecond(int microseconds)
{
    _microsecond = microseconds;
}

int Time::getMicrosecond() const
{
    return _microsecond;
}

std::string Time::format(const std::string& format) const
{
    if (format.empty()) {
        return strings::empty_string;
    }

    // fill ANSI time structure
    struct tm time_info;
    memory::set(&time_info, sizeof(time_info), 0, sizeof(time_info));
    time_info.tm_isdst = -1;
    time_info.tm_hour = _hour;
    time_info.tm_min = _minute;
    time_info.tm_sec = _second;

    // fill rest using current date
    Date date = getCurrentLocalDate();
    time_info.tm_year = date.getYear();
    time_info.tm_mon = date.getMonth();
    time_info.tm_mday = date.getDay();

    return detail::FormatTimeInfo(time_info, format);
}

bool Time::set(const std::string& time)
{
    if (time.empty()) {
        return false;
    }

    // check the time string, if any unsupported character are included
    a_util::regex::RegularExpression regex("[^:0123456789]");
    int consumed = 0;

    if (regex.match(time, a_util::regex::RegularExpression::AT_Unanchored, consumed)) {
        return false;
    }

    // check the format of the time string
    // on error, then return error code ERR_UNKNOWN_FORMAT
    size_t num_sep = std::count(time.begin(), time.end(), ':');
    if (num_sep != 2) {
        // format of the time string is invalid
        return false;
    }
    std::vector<std::string> vec_parts;
    vec_parts = strings::split(time, ":");
    if (vec_parts.size() == 3) {
        if (vec_parts[0].size() != 2 || vec_parts[1].size() != 2 || vec_parts[2].size() != 2) {
            return false;
        }

        std::uint32_t h, m, s;
        if (!strings::toUInt32(vec_parts[0], h) || !strings::toUInt32(vec_parts[1], m) ||
            !strings::toUInt32(vec_parts[2], s)) {
            return false;
        }

        if (h > 23 || m > 59 || s > 59) {
            return false;
        }

        set(h, m, s);
    }
    else {
        // format of the date string is invalid
        return false;
    }

    return true;
}

Time::Time(int hour, int minute, int second, int microsecond /*= 0*/)
    : _hour(hour), _minute(minute), _second(second), _microsecond(microsecond)
{
}

Time getCurrentLocalTime()
{
    Time time;

#ifdef WIN32
    SYSTEMTIME system_time;
    ::GetLocalTime(&system_time);

    time.setHour(system_time.wHour);
    time.setMinute(system_time.wMinute);
    time.setSecond(system_time.wSecond);
    time.setMicrosecond(system_time.wMilliseconds * 1000);
#else
    struct timeval current_time;
    ::gettimeofday(&current_time, nullptr);
    struct tm system_time;
    ::localtime_r(&current_time.tv_sec, &system_time);

    time.setHour(system_time.tm_hour);
    time.setMinute(system_time.tm_min);
    time.setSecond(system_time.tm_sec);
    time.setMicrosecond(current_time.tv_usec);
#endif

    return time;
}

Time getCurrentSystemTime()
{
    Time time;

#ifdef WIN32
    SYSTEMTIME system_time;
    ::GetSystemTime(&system_time);

    time.setHour(system_time.wHour);
    time.setMinute(system_time.wMinute);
    time.setSecond(system_time.wSecond);
    time.setMicrosecond(system_time.wMilliseconds * 1000);
#else
    struct timeval current_time;
    ::gettimeofday(&current_time, nullptr);
    struct tm system_time;
    ::gmtime_r(&current_time.tv_sec, &system_time);

    time.setHour(system_time.tm_hour);
    time.setMinute(system_time.tm_min);
    time.setSecond(system_time.tm_sec);
    time.setMicrosecond(current_time.tv_usec);
#endif

    return time;
}

bool operator==(const Time& a, const Time& b)
{
    return a.getHour() == b.getHour() && a.getMinute() == b.getMinute() &&
           a.getSecond() == b.getSecond() && a.getMicrosecond() == b.getMicrosecond();
}

bool operator!=(const Time& a, const Time& b)
{
    return !(a == b);
}

DateTime::DateTime() : Date(), Time()
{
}

DateTime::DateTime(
    int year, int month, int day, int hour, int minute, int second, int microseconds /*= 0*/)
    : Date(year, month, day), Time(hour, minute, second, microseconds)
{
}

void DateTime::set(
    int year, int month, int day, int hour, int minute, int second, int microseconds /*= 0*/)
{
    Date::set(year, month, day);
    Time::set(hour, minute, second, microseconds);
}

timestamp_t DateTime::toTimestamp(TimestampReference timestampReference) const
{
    timestamp_t timestamp =
        ((timestamp_t)(getHour() * 3600 + getMinute() * 60 + getSecond()) * 1000000) +
        (timestamp_t)getMicrosecond();

    // convert the timestamp to use the reference point set by the parameter timestampReference.
    switch (timestampReference) {
    case TimestampReference::MicroSecondsSinceUnixTimeOrigin: {
        timestamp -= a_util::datetime::detail::timeDifferenceJulian2Unix;
        break;
    }
    case TimestampReference::MicroSecondsSinceJulianDateOrigin:
        break;
    }

    int year = getYear();
    int month = getMonth();
    int day = getDay();
    if (year == 0 && month == 0 && day == 0) {
        return timestamp;
    }

    month -= 3;
    if (month < 0) {
        month += 12;
        year--;
    }

    int day_count = (int)floor(year * 365.25) + (int)floor(month * 30.6 + 0.5) + day + 1721117;
    if (day_count > 2299170) {
        day_count -= ((int)floor((float)year / 100.0) - (int)floor((float)year / 400.0) - 2);
    }

    return (timestamp_t)day_count * detail::usecs_per_day + timestamp;
}

std::string DateTime::format(const std::string& format) const
{
    if (format.empty()) {
        return strings::empty_string;
    }

    // fill ANSI time structure
    struct tm time_info;
    memory::set(&time_info, sizeof(time_info), 0, sizeof(time_info));
    time_info.tm_isdst = -1;
    time_info.tm_hour = getHour();
    time_info.tm_min = getMinute();
    time_info.tm_sec = getSecond();
    time_info.tm_year = getYear();
    time_info.tm_mon = getMonth();
    time_info.tm_mday = getDay();

    return detail::FormatTimeInfo(time_info, format);
}

bool DateTime::set(const std::string& format, const std::string& date_time)
{
    detail::ParseDateTime(format, date_time, *this);
    return true;
}

void DateTime::set(timestamp_t date_time, TimestampReference timestampReference)
{
    // convert the timestamp to use Julian day as the reference point.
    switch (timestampReference) {
    case TimestampReference::MicroSecondsSinceUnixTimeOrigin: {
        date_time += a_util::datetime::detail::timeDifferenceJulian2Unix;
        break;
    }
    case TimestampReference::MicroSecondsSinceJulianDateOrigin:
        break;
    }

    // Julian Day Number to Gregorian date check and conversion
    int days = (int)(date_time / detail::usecs_per_day);
    if (days >= 2299161) {
        int days2 = days - 1721119;
        days += (int)floor((float)days2 / 36524.25) - (int)floor((float)days2 / 146097.0) - 2;
    }

    int tz = days - 1721117;
    int y = (int)floor((tz - 0.2) / 365.25);
    int r = tz - (int)floor(y * 365.25);
    int m = (int)floor((r - 0.5) / 30.6);
    int d = r - (int)floor(m * 30.6 + 0.5);

    m += 3;
    if (m > 12) {
        m -= 12;
        y++;
    }

    timestamp_t timestamp = (date_time % detail::usecs_per_day);
    int hours = (int)(timestamp / 3600000000LL);
    int minutes = (int)(timestamp / 60000000LL) % 60;
    int seconds = (int)(timestamp / 1000000LL) % 60;
    int microseconds = (int)(timestamp % 1000000LL);

    set(y, m, d, hours, minutes, seconds, microseconds);
}

DateTime getCurrentLocalDateTime()
{
    DateTime date_time;

#ifdef WIN32
    SYSTEMTIME system_time;
    ::GetLocalTime(&system_time);

    date_time.setHour(system_time.wHour);
    date_time.setMinute(system_time.wMinute);
    date_time.setSecond(system_time.wSecond);
    date_time.setMicrosecond(system_time.wMilliseconds * 1000);
    date_time.setYear(system_time.wYear);
    date_time.setMonth(system_time.wMonth);
    date_time.setDay(system_time.wDay);
#else
    struct timeval current_time;
    ::gettimeofday(&current_time, nullptr);
    struct tm system_time;
    ::localtime_r(&current_time.tv_sec, &system_time);

    date_time.setHour(system_time.tm_hour);
    date_time.setMinute(system_time.tm_min);
    date_time.setSecond(system_time.tm_sec);
    date_time.setMicrosecond(current_time.tv_usec);
    date_time.setYear(system_time.tm_year + 1900);
    date_time.setMonth(system_time.tm_mon + 1);
    date_time.setDay(system_time.tm_mday);
#endif

    return date_time;
}

DateTime getCurrentSystemDateTime()
{
    DateTime date_time;

#ifdef WIN32
    SYSTEMTIME system_time;
    ::GetSystemTime(&system_time);

    date_time.setHour(system_time.wHour);
    date_time.setMinute(system_time.wMinute);
    date_time.setSecond(system_time.wSecond);
    date_time.setMicrosecond(system_time.wMilliseconds * 1000);
    date_time.setYear(system_time.wYear);
    date_time.setMonth(system_time.wMonth);
    date_time.setDay(system_time.wDay);
#else
    struct timeval current_time;
    ::gettimeofday(&current_time, nullptr);
    struct tm system_time;
    ::gmtime_r(&current_time.tv_sec, &system_time);

    date_time.setHour(system_time.tm_hour);
    date_time.setMinute(system_time.tm_min);
    date_time.setSecond(system_time.tm_sec);
    date_time.setMicrosecond(current_time.tv_usec);
    date_time.setYear(system_time.tm_year + 1900);
    date_time.setMonth(system_time.tm_mon + 1);
    date_time.setDay(system_time.tm_mday);
#endif

    return date_time;
}

bool operator==(const DateTime& a, const DateTime& b)
{
    return static_cast<Date>(a) == static_cast<Date>(b) &&
           static_cast<Time>(a) == static_cast<Time>(b);
}

bool operator!=(const DateTime& a, const DateTime& b)
{
    return !(a == b);
}

} // namespace datetime
} // namespace a_util
