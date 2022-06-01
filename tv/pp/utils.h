#ifndef UTILS_H_
#define UTILS_H_

#include <time.h>
#include <cstdlib>
#include <stdint.h>

#include <vector>
#include <utility>
#include <chrono>

double randNum()
{
    return static_cast<double>(rand() % 1001) / 1000.0;
}

double randNumInDomain(const double min_val, const double max_val)
{
    const double r = static_cast<double>(rand() % 1001) / 1000.0;

    const double d = max_val - min_val;

    return r * d + min_val;
}

double randNumZeroMean()
{
    return 2.0 * (static_cast<double>(rand() % 1001) / 1000.0 - 1.0);
}

double sqDist(const double x0, const double x1, const double y0, const double y1)
{
    const double xd = x1 - x0;
    const double yd = y1 - y0;
    return xd * xd + yd * yd;
}

#endif
