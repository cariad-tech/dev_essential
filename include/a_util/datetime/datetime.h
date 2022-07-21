/**
 * @file
 * Public API for @ref a_util::datetime::Date "Date", @ref a_util::datetime::Time "Time" and
 * @ref a_util::datetime::DateTime "DateTime" types and functions
 *
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

    This Source Code Form is subject to the terms of the Mozilla
    Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

If it is not possible or desirable to put the notice in a particular file, then
You may include the notice in a location (such as a LICENSE file in a
relevant directory) where a recipient would be likely to look for such a notice.

You may add additional accurate notices of copyright ownership.
@endverbatim
 */

#ifndef _A_UTILS_UTIL_DATETIME_DATETIME_HEADER_INCLUDED_
#define _A_UTILS_UTIL_DATETIME_DATETIME_HEADER_INCLUDED_

#include <a_util/base/types.h> //timestamp_t

#include <string> //std::string

namespace a_util {
namespace datetime {
/**
 * Date implementation representing a date consisting of year, month and day
 * @warning: This class does not enforce date consistency! E.g. "42.102.20000" is a valid date!
 */
class Date {
public:
    /// Default constructor - initializes the date to 00.00.0000
    Date();

    /**
     * Constructor for presetting the stored date.
     * @param[in] year The year.
     * @param[in] month The month.
     * @param[in] day The day of the month.
     */
    Date(int year, int month, int day);

    /**
     * Sets the stored date.
     * @param[in] year The year.
     * @param[in] month The month.
     * @param[in] day The day of the month.
     */
    void set(int year, int month, int day);

    /**
     * Sets the stored date given a date string
     *
     * This string formats are supported: yyyy.mm.dd or dd.mm.yyyy<br><ul>
     * <li>\c yyyy Year as decimal number (zero-padded 0000..9999) </li>
     * <li>\c mm   Month as decimal number (zero-padded 01..12) </li>
     * <li>\c dd   Day of month as decimal number (zero-padded 01..31) </li>
     * <li>\c .    Separator for year.month.day or day.month.year </li></ul>
     *
     * @param[in] date The string contains the given date as string.
     * @return @c false if the date string is invalid, @c true otherwise
     */
    bool set(const std::string& date);

    /**
     * Sets the day of the month.
     * @param[in] day The day of the month.
     */
    void setDay(int day);

    /**
     * Get the day of the month.
     * @return The day of the month.
     */
    int getDay() const;

    /**
     * Set the month.
     * @param[in] month The month.
     */
    void setMonth(int month);

    /**
     * Get the month.
     * @return The month.
     */
    int getMonth() const;

    /**
     * Set the year.
     * @param[in] year The year.
     */
    void setYear(int year);

    /**
     * Get the year.
     * @return The year.
     */
    int getYear() const;

    /**
     * Creates a string representation of the stored date.
     *
     * This format identifiers are supported: %a %A %b %B %c %d %H %I %j %m %M %p %S %U %w %W %x %X
     * %y %Y Percent signs are escaped by %%
     * %#&lt;character&gt; is not supported because it only makes sense with %Z which is also not
     * supported
     * @param[in] format_str The format of the string representation (see @ref DateTime::format,
     *                       might be empty).
     * @return A string representation of the stored date.
     * @throw std::invalid_argument If the format string is invalid
     */
    std::string format(const std::string& format_str) const;

private:
    int _day;
    int _month;
    int _year;
};

/**
 * Compare two dates for equality
 * @param[in] lhs Left-hand side object
 * @param[in] rhs Right-hand side object
 * @return @c true if both date representations are equal, @c false otherwise
 */
bool operator==(const Date& lhs, const Date& rhs);

/**
 * Compare two dates for inequality
 * @param[in] lhs Left-hand side object
 * @param[in] rhs Right-hand side object
 * @return @c true if the date representations are not equal, @c false otherwise
 */
bool operator!=(const Date& lhs, const Date& rhs);

/**
 * Retrieves the current date (hardware clock with respect to timezone).
 * @return The current date.
 */
Date getCurrentLocalDate();

/**
 * Retrieves the current system date (in UTC format).
 * @return The current date.
 */
Date getCurrentSystemDate();

/**
 * Time implementation representing a time of day (00:00:00,0 - 23:59:59,999999)
 * @warning: This class does not enforce time consistency!
 */
class Time {
public:
    /// Default constructor - initializes the time to 00:00:00,0
    Time();

    /**
     * Constructor for presetting the stored time.
     * @param[in] hour The hour.
     * @param[in] minute The minute.
     * @param[in] second The second.
     * @param[in] microsecond The microsecond.
     */
    Time(int hour, int minute, int second, int microsecond = 0);

    /**
     * Sets the stored time.
     * @param[in] hour The hour.
     * @param[in] minute The minute.
     * @param[in] second The second.
     * @param[in] microsecond The microsecond.
     */
    void set(int hour, int minute, int second, int microsecond = 0);

    /**
     * Sets the time a given time string
     *
     * This string formats are supported: HH:MM:SS <br><ul>
     * <li>\c HH Hour in 24-hour format (zero-padded 00..23) </li>
     * <li>\c MM Minute as decimal number (zero-padded 00..59) </li>
     * <li>\c SS Second as decimal number (zero-padded 00..59) </li>
     * <li>\c :  Separator for hour:minute:second </li></ul>
     *
     * @param[in]  time The string contains the given time as string.
     * @return @c false if the time string format is invalid, @c true otherwise
     */
    bool set(const std::string& time);

    /**
     * Set the hour.
     * @param[in] hour The hour.
     */
    void setHour(int hour);

    /**
     * Get the hour.
     * @return The hour.
     */
    int getHour() const;

    /**
     * Set the minute.
     * @param[in] minute The minute.
     */
    void setMinute(int minute);

    /**
     * Get the minute.
     * @return The minute.
     */
    int getMinute() const;

    /**
     * Set the second.
     * @param[in] second The second.
     */
    void setSecond(int second);

    /**
     * Get the second.
     * @return The second.
     */
    int getSecond() const;

    /**
     * Set the microsecond.
     * @param[in] microsecond The microsecond.
     */
    void setMicrosecond(int microsecond);

    /**
     * Get the microsecond.
     * @return[in] The microsecond.
     */
    int getMicrosecond() const;

    /**
     * Creates a string representation of the stored time.
     * @param[in] format_str The format of the string representation (see @ref DateTime::format,
     *                       might be empty).
     * @return A string representation of the stored date.
     * @throw std::invalid_argument If the format string is invalid
     */
    std::string format(const std::string& format_str) const;

private:
    int _hour;
    int _minute;
    int _second;
    int _microsecond;
};

/**
 * Compare two times for equality
 * @param[in] lhs Left-hand side object
 * @param[in] rhs Right-hand side object
 * @return @c true if both time representations are equal, @c false otherwise
 */
bool operator==(const Time& lhs, const Time& rhs);

/**
 * Compare two times for inequality
 * @param[in] lhs Left-hand side object
 * @param[in] rhs Right-hand side object
 * @return @c true if the time representations are not equal, @c false otherwise
 */
bool operator!=(const Time& lhs, const Time& rhs);

/**
 * Get the current time (hardware clock with respect to timezone).
 * @return The current time.
 */
Time getCurrentLocalTime();

/**
 * Get the current time (in UTC format).
 * @return The current time.
 */
Time getCurrentSystemTime();

/**
 * The reference point in time.
 *
 * With this enum we define the reference point of a timestamp. Since the
 * internal data structure uses Julian day as the starting point when counting
 * the elapsed microseconds, with the help of this enum the reference can be
 * changed when receiving a timestamp (i.e. \c DateTime::toTimestamp())
 * and be converted back to Julian day reference point
 * when setting a timestamp
 * (i.e. \c DateTime::set(timestamp_t, TimestampReference))
 */
enum TimestampReference {
    /**
     * Reference point in time is noon on Monday, January 1, 4713 BC
     * (<a href="https://en.wikipedia.org/wiki/Julian_day">Julian day</a>)
     */
    MicroSecondsSinceJulianDateOrigin = 0,
    /**
     * Reference point in time is midnight on Thursday, 1 January 1970
     * AC (<a href="https://en.wikipedia.org/wiki/Unix_time">Unix time</a>)
     */
    MicroSecondsSinceUnixTimeOrigin = 1
};

/// DateTime class representing both Date and Time
class DateTime : public Date, public Time {
public:
    /// Default constructor - see Date and Time constructors
    DateTime();

    /**
     * Constructor for presetting the stored date and time.
     * @param[in] year The year.
     * @param[in] month The month.
     * @param[in] day The day of the month.
     * @param[in] hour The hour.
     * @param[in] minute The minute.
     * @param[in] second The second.
     * @param[in] microsecond The microsecond.
     */
    DateTime(int year, int month, int day, int hour, int minute, int second, int microsecond = 0);

    /**
     * Set the stored date and time.
     * @param[in] year The year.
     * @param[in] month The month.
     * @param[in] day The day of the month.
     * @param[in] hour The hour.
     * @param[in] minute The minute.
     * @param[in] second The second.
     * @param[in] microsecond The microsecond.
     */
    void set(int year, int month, int day, int hour, int minute, int second, int microsecond = 0);

    /**
     * Set stored data and time from a timestamp.
     *
     * Since the internal data structure uses julian day as the reference point,
     * you have to set the \c timestampReference parameter to the reference point
     * in time that the \c timestamp parameter uses.
     *
     * \remark
     * To convert the number of microseconds that have elapsed since midnight on
     * Thursday, 1 January 1970 AC
     * (<a href="https://en.wikipedia.org/wiki/Unix_time">Unix time</a>) to the
     * number of microseconds that have elapsed since noon on Monday,
     * January 1, 4713 BC (<a href="https://en.wikipedia.org/wiki/Julian_day">
     * Julian day</a>), you have to add the number \c 210866803200000000 to the
     * timestamp
     *
     * @param[in] timestamp The timestamp in number of microseconds that have
     *                       elapsed since the reference point defined in the
     *                       \c timestampReference parameter.
     *
     * @param[in] timestampReference The reference point in time when the count
     *                               in microseconds was started. Default is
     *                               julian day.
     */
    void set(timestamp_t timestamp,
             TimestampReference timestampReference =
                 TimestampReference::MicroSecondsSinceJulianDateOrigin);

    /**
     * Convert stored date and time to a timestamp.
     *
     * Since the internal data structure uses julian day as the reference point,
     * this is the default reference point of the timestamp returned by its
     * member function. You can change the reference point in time by the
     * \c timestampReference parameter.
     *
     * \remark
     * To get the number of microseconds that have elapsed since midnight on
     * Thursday, 1 January 1970 AC
     * (<a href="https://en.wikipedia.org/wiki/Unix_time">Unix time</a>), you
     * have to subtract the number \c 210866803200000000 from the result. This
     * is the number of microseconds that have  elapsed since the Julian day up
     * to the reference point in time in Unix time.
     *
     * @param[in] timestampReference The reference point in time the returned
     * timestamp should refer to.
     * @return The timestamp in number of microseconds that have elapsed since
     *         the point in time defined by the \c timestampReference parameter.
     *         The default reference point int time is julian day.
     */
    timestamp_t toTimestamp(TimestampReference timestampReference =
                                TimestampReference::MicroSecondsSinceJulianDateOrigin) const;

    /**
     * Format the date/time string.
     *
     * Returns a formated string containing date/time information. The format argument consists
     * of one or more codes; as in printf(), the formatting codes are preceded by a percent sign
     * (\c '\%'). Characters that do not begin with \c \% are copied unchanged.
     *
     * The formatting codes are listed below:<br><ul>
     * <li>\c \%a Abbreviated weekday name</li>
     * <li>\c \%A Full weekday name</li>
     * <li>\c \%b Abbreviated month name</li>
     * <li>\c \%B Full month name</li>
     * <li>\c \%c Date and time representation appropriate for locale</li>
     * <li>\c \%d Day of month as decimal number (01  31)</li>
     * <li>\c \%H Hour in 24-hour format (00  23)</li>
     * <li>\c \%I Hour in 12-hour format (01  12)</li>
     * <li>\c \%j Day of year as decimal number (001  366)</li>
     * <li>\c \%m Month as decimal number (01  12)</li>
     * <li>\c \%M Minute as decimal number (00  59)</li>
     * <li>\c \%p Current locale's A.M./P.M. indicator for 12-hour clock</li>
     * <li>\c \%S Second as decimal number (00  59)</li>
     * <li>\c \%U Week of year as decimal number, with Sunday as first day of week (00  53)</li>
     * <li>\c \%w Weekday as decimal number (0  6; Sunday is 0)</li>
     * <li>\c \%W Week of year as decimal number, with Monday as first day of week (00  53)</li>
     * <li>\c \%x Date representation for current locale</li>
     * <li>\c \%X Time representation for current locale</li>
     * <li>\c \%y Year without century, as decimal number (00  99)</li>
     * <li>\c \%Y Year with century, as decimal number</li>
     * <li>\c \%z,%Z Either the time-zone name or time zone abbreviation, depending on registry
     * settings; no characters if time zone is unknown</li>
     * <li>\c \%\% Percent sign</li>
     * </ul>
     *
     * As in the \c printf() function, the \c \# flag may prefix any formatting
     * code. In that case, the meaning of the format code is changed as follows.
     * <br>
     */
#ifdef WIN32
    /**
     * <tt>\%\#a, \%\#A, \%\#b, \%\#B, \%\#p, \%\#X, \%\#z, \%\#Z, \%\#%</tt><br>
     * \c \# flag is ignored.<br>
     *
     * \c \%\#c Long date and time representation, appropriate for current locale.<br>
     * For example: "Tuesday, March 14, 1995, 12:41:29".
     *
     * \c \%\#x Long date representation, appropriate to current locale.<br>
     * For example: "Tuesday, March 14, 1995".
     *
     * <tt>\%\#d, \%\#H, \%\#I, \%\#j, \%\#m, \%\#M, \%\#S, \%\#U, \%\#w, \%\#W, \%\#y,
     * \%\#Y</tt><br> Remove leading zeros (if any).
     */
#else
    /**
     * <tt>\%\#a, \%\#A, \%\#b, \%\#B</tt><br>
     * Characters are converted to uppercase.<br>
     * For example: "TUESDAY", "MARCH".
     */
#endif // WIN32
    /**
     *
     * @param[in] format_str Format-control string.
     * @return The formatted string.
     * @throw std::invalid_argument If the format string is invalid
     */
    std::string format(const std::string& format_str) const;

    /**
     * Sets the date time given a string
     *
     * The formatting codes are listed below: <br><ul>
     * <li>\c \%b Abbreviated month name </li>
     * <li>\c \%B Full month name </li>
     * <li>\c \%m Month as decimal number (space-padded _1..12, zero-padded 01..12, normal 1 -12)
     * </li> <li>\c \%d Day of month as decimal number (space-padded _1..31, zero-padded 01..31,
     * normal 1-31) </li> <li>\c \%y Year without century, as decimal number (00  99) </li> <li>\c
     * \%Y Year with century, as decimal number </li> <li>\c \%H Hour in 24-hour format (00  23)
     * </li> <li>\c \%I Hour in 12-hour format (01  12) </li> <li>\c \%p AM / PM and am / pm
     * detection </li> <li>\c \%M Minute as decimal number (00  59) </li> <li>\c \%S Second as
     * decimal number (00  59) </li></ul>
     *
     * @param[in] format_str format string (see list)
     * @param[in] date_time string containing the date / time string
     * @retval true if the conversion is OK, in this case the local data structures are updated
     *              with the new values
     * @retval false Otherwise
     * @remarks the counterpart is @ref DateTime::format
     */
    bool set(const std::string& format_str, const std::string& date_time);
};

/**
 * Compare two date-time representations for equality
 * @param[in] lhs Left-hand side object
 * @param[in] rhs Right-hand side object
 * @return @c true if both date-time representations are equal, @c false otherwise
 */
bool operator==(const DateTime& lhs, const DateTime& rhs);

/**
 * Compare two date-time representations for inequality
 * @param[in] lhs Left-hand side object
 * @param[in] rhs Right-hand side object
 * @return @c true if the date-time representations are not equal, @c false otherwise
 */
bool operator!=(const DateTime& lhs, const DateTime& rhs);

/**
 * Get the current date and time (hardware clock with respect to timezone).
 * @return The current date and time.
 */
DateTime getCurrentLocalDateTime();

/**
 * Get the current date and time (in UTC format).
 * @return The current date and time.
 */
DateTime getCurrentSystemDateTime();

} // namespace datetime
} // namespace a_util

#endif // _A_UTILS_UTIL_DATETIME_DATETIME_HEADER_INCLUDED_
