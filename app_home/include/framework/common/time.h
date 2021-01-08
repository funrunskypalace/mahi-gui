#pragma once

#include <chrono>

#include "framework/common/common.h"

namespace framework
{

/**
 * Less typing for common system time and steady time type.
 *
 * SystemTime should be used when getting a time to present to the user, e.g.
 * for logging. MonotonicTime should be used when tracking time for computing an
 * interval.
 */
using SystemTime = std::chrono::time_point<std::chrono::system_clock>;
using MonotonicTime = std::chrono::time_point<std::chrono::steady_clock>;

#define DATETIME_FORMAT_DEFAULT "%F %T.%N"

class GWSDK_API AppClockUnit
{
public:
    static const int64_t MILLISECONDS_PER_SECOND = 1000;
    static const int64_t MICROSECONDS_PER_MILLISECOND = 1000;
    static const int64_t NANOSECONDS_PER_MICROSECOND = 1000;

    static const int64_t MICROSECONDS_PER_SECOND =
        MICROSECONDS_PER_MILLISECOND * MILLISECONDS_PER_SECOND;
    static const int64_t NANOSECONDS_PER_MILLISECOND =
        NANOSECONDS_PER_MICROSECOND * MICROSECONDS_PER_MILLISECOND;
    static const int64_t NANOSECONDS_PER_SECOND =
        NANOSECONDS_PER_MILLISECOND * MILLISECONDS_PER_SECOND;

    static const int64_t SECONDS_PER_MINUTE = 60;
    static const int64_t MINUTES_PER_HOUR = 60;
    static const int64_t HOURS_PER_DAY = 24;
    static const int64_t SECONDS_PER_HOUR =
        SECONDS_PER_MINUTE * MINUTES_PER_HOUR;

    static const int64_t MILLISECONDS_PER_MINUTE =
        MILLISECONDS_PER_SECOND * SECONDS_PER_MINUTE;
    static const int64_t NANOSECONDS_PER_MINUTE =
        NANOSECONDS_PER_SECOND * SECONDS_PER_MINUTE;
    static const int64_t NANOSECONDS_PER_HOUR =
        NANOSECONDS_PER_SECOND * SECONDS_PER_HOUR;
    static const int64_t NANOSECONDS_PER_DAY =
        NANOSECONDS_PER_HOUR * HOURS_PER_DAY;
};

class AppClock
{
public:
    /**
     * @return current nano time in int64_t (unix-timestamp * 1e9 + nano-part)
     */
    static int64_t nowInNano();

    static int64_t nowInMicro();

    /**
     * parse string time to nano time
     * @param timestr string-formatted time
     * @param format ref: https://en.cppreference.com/w/cpp/io/manip/put_time +
     * %N for nanoseconds {:09d}
     * @return nano time in int64_t
     */
    static int64_t
    strptime(const std::string& timestr,
             const std::string& format = DATETIME_FORMAT_DEFAULT);

    /**
     * dump int64_t time to string with format
     * @param nanotime nano time in int64_t
     * @param format ref: https://en.cppreference.com/w/cpp/io/manip/put_time +
     * %N for nanoseconds {:09d}
     * @return string-formatted time
     */
    static const std::string
    strftime(const int64_t nanotime,
             const std::string& format = DATETIME_FORMAT_DEFAULT);

    static inline const std::string
    strfnow(const std::string& format = DATETIME_FORMAT_DEFAULT)
    {
        return strftime(nowInNano(), format);
    }

    static inline int64_t nextMinuteNano(int64_t nanotime)
    {
        return nanotime - nanotime % AppClockUnit::NANOSECONDS_PER_MINUTE +
               AppClockUnit::NANOSECONDS_PER_MINUTE;
    }

    static inline int64_t nextDayNano(int64_t nanotime)
    {
        int64_t day = nanotime - nanotime % AppClockUnit::NANOSECONDS_PER_DAY +
                      7 * AppClockUnit::NANOSECONDS_PER_HOUR +
                      30 * AppClockUnit::NANOSECONDS_PER_MINUTE;
        if (day < nowInNano())
        {
            day += AppClockUnit::NANOSECONDS_PER_DAY;
        }
        return day;
    }

private:
    AppClock();
    static const AppClock& getInstance();

    int64_t start_time_since_epoch_;
    int64_t start_time_steady_;
};
} // namespace framework
