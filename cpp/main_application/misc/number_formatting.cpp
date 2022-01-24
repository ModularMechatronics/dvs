#include <cmath>
#include <iostream>
#include <sstream>

#include "misc/misc.h"

int getIntExponent(const double d)
{
    return std::floor(std::log10(std::fabs(d)));
}

double getBase(const double num)
{
    const double exp_val = getIntExponent(num);

    return num / std::pow(10.0, exp_val);
}

std::string getBaseAsString(const double num)
{
    const double exp_val = getIntExponent(num);

    return toStringWithNumDecimalPlaces(num / std::pow(10.0, exp_val), 5);
}

std::string getIntExponentAsString(const double num)
{
    return std::to_string(getIntExponent(num));
}

std::string toStringWithNumDecimalPlaces(const double input_val, const size_t n)
{
    std::ostringstream out;
    out.precision(n);
    out << std::fixed << input_val;
    return out.str();
}

std::string formatNumberInternal(const double num, const size_t n)
{
    const double abs_num = std::fabs(num);
    if (abs_num == 0.0)
    {
        return "0.0";
    }
    else if (abs_num > 1.0e6)
    {
        return toStringWithNumDecimalPlaces(getBase(num), n) + "e" + "+" +
               std::to_string(getIntExponent(num));
    }
    else if (abs_num < 1.0e-4)
    {
        return toStringWithNumDecimalPlaces(getBase(num), n) + "e" +
               std::to_string(getIntExponent(num));
    }
    else
    {
        const int exp_num = getIntExponent(num);
        if (abs_num < 1.0)
        {
            return toStringWithNumDecimalPlaces(num, 5 - exp_num);
        }
        else
        {
            return toStringWithNumDecimalPlaces(num, 5 - exp_num);
        }
    }
}

std::string formatNumber(const double num, const size_t n)
{
    const std::string s = formatNumberInternal(num, n);

    if((s.length() > 1) && (s.find('.') != std::string::npos))
    {
        int idx = s.length() - 1;
        while(idx > 1)
        {
            if((s[idx] == '0') && s[idx - 1] == '0')
            {
                idx--;
            }
            else
            {
                if((s[idx] == '0') && (s[idx - 1] == '.'))
                {
                    idx--;
                }
                break;
            }
        }
        return s.substr(0, idx);
    }
    else
    {
        return s;
    }
}
