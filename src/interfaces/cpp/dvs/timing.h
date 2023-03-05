#ifndef DVS_TIMING_H_
#define DVS_TIMING_H_

#include <chrono>
#include <string>

namespace dvs
{

namespace timing
{
using tp = std::chrono::high_resolution_clock::time_point;

inline std::chrono::high_resolution_clock::time_point getTimeNow()
{
    return std::chrono::high_resolution_clock::now();
}

inline double timePointsToString(const std::chrono::high_resolution_clock::time_point& t0,
                                 const std::chrono::high_resolution_clock::time_point& t1)
{
    std::chrono::duration<double, std::milli> ms_double = t1 - t0;

    return ms_double.count();
}

}  // namespace timing
}  // namespace dvs

#endif
