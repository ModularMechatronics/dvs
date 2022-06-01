#include "arl/utilities/misc.h"

#include <vector>

namespace arl
{
std::string Time::toString() const
{
    auto f = [](const int i) {
        if (i < 10)
        {
            return "0" + std::to_string(i);
        }
        else
        {
            return std::to_string(i);
        }
    };
    auto g = [](const double d) {
        if (d < 10.0)
        {
            return "0" + std::to_string(d);
        }
        else
        {
            return std::to_string(d);
        }
    };

    return f(year) + "-" + f(month) + "-" + f(day) + "T" + f(hour) + ":" + f(minute) + ":" +
           g(second) + "Z";
}

Time Time::timeNow()
{
    const std::time_t t = std::time(0);
    const std::tm* now = std::localtime(&t);

    Time time_now;

    time_now.year = now->tm_year + 1900;

    time_now.month = now->tm_mon + 1;
    time_now.day = now->tm_mday;
    time_now.hour = now->tm_hour;
    time_now.minute = now->tm_min;
    time_now.second = now->tm_sec;

    return time_now;
}

bool operator>(const Time& t0, const Time& t1)
{
    if (t0 == t1)
    {
        return false;
    }
    else
    {
        return !(t0 < t1);
    }
}

bool operator==(const Time& t0, const Time& t1)
{
    return (t0.year == t1.year) && (t0.month == t1.month) && (t0.day == t1.day) &&
           (t0.hour == t1.hour) && (t0.minute == t1.minute) && (t0.second == t1.second);
}

bool operator<(const Time& t0, const Time& t1)
{
    if (t0.year == t1.year)
    {
        if (t0.month == t1.month)
        {
            if (t0.day == t1.day)
            {
                if (t0.hour == t1.hour)
                {
                    if (t0.minute == t1.minute)
                    {
                        if (t0.second == t1.second)
                        {
                            return false;
                        }
                        else
                        {
                            return t0.second < t1.second ? true : false;
                        }
                    }
                    else
                    {
                        return t0.minute < t1.minute ? true : false;
                    }
                }
                else
                {
                    return t0.hour < t1.hour ? true : false;
                }
            }
            else
            {
                return t0.day < t1.day ? true : false;
            }
        }
        else
        {
            return t0.month < t1.month ? true : false;
        }
    }
    else
    {
        return t0.year < t1.year ? true : false;
    }
}

std::ostream& operator<<(std::ostream& os, const Time& ti)
{
    auto f = [](const int i) {
        if (i < 10)
        {
            return "0" + std::to_string(i);
        }
        else
        {
            return std::to_string(i);
        }
    };

    auto g = [](const double d) {
        if (d < 10.0)
        {
            return "0" + std::to_string(d);
        }
        else
        {
            return std::to_string(d);
        }
    };

    os << f(ti.year) << "-" << f(ti.month) << "-" << f(ti.day) << " " << f(ti.hour) << ":"
       << f(ti.minute) << ":" << g(ti.second);

    return os;
}

std::string monthEnumToString(const Month m)
{
    std::string s;
    switch (m)
    {
        case Month::Jan:
            s = "Jan";
            break;
        case Month::Feb:
            s = "Feb";
            break;
        case Month::Mar:
            s = "Mar";
            break;
        case Month::Apr:
            s = "Apr";
            break;
        case Month::May:
            s = "May";
            break;
        case Month::Jun:
            s = "Jun";
            break;
        case Month::Jul:
            s = "Jul";
            break;
        case Month::Aug:
            s = "Aug";
            break;
        case Month::Sep:
            s = "Sep";
            break;
        case Month::Oct:
            s = "Oct";
            break;
        case Month::Nov:
            s = "Nov";
            break;
        case Month::Dec:
            s = "Dec";
            break;
        default:
            EXIT() << "Invalid month!";
            break;
    }

    return s;
}

}