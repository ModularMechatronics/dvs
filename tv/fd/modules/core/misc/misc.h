#ifndef UTILITIES_MISC_H_
#define UTILITIES_MISC_H_

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "arl/utilities/logging.h"

namespace arl
{
struct Time
{
    int year;
    int month;
    int day;
    int hour;
    int minute;
    double second;
    Time() = default;
    Time(const std::string& ts);

    std::string toString() const;

    static Time timeNow();
};

bool operator<(const Time& t0, const Time& t1);
bool operator>(const Time& t0, const Time& t1);
bool operator==(const Time& t0, const Time& t1);

std::ostream& operator<<(std::ostream& os, const Time& ti);

enum class Month
{
    Jan,
    Feb,
    Mar,
    Apr,
    May,
    Jun,
    Jul,
    Aug,
    Sep,
    Oct,
    Nov,
    Dec
};

std::string monthEnumToString(const Month m);

void printStringLines(const std::vector<std::string>& string_lines);

template <typename T> void printVectorOfVectors(std::vector<std::vector<T>> data)
{
    for (size_t r = 0; r < data.size(); r++)
    {
        std::vector<T>& data_row = data[r];
        for (size_t c = 0; c < data_row.size(); c++)
        {
            std::cout << data_row[c];
            if (c != (data_row.size() - 1))
            {
                std::cout << ",";
            }
        }
        std::cout << std::endl;
    }
}

}  // namespace arl

#endif
