#include "point.h"

#include <math.h>

#include <iostream>

namespace sphere
{

Point::Point() {}

Point::Point(float x_, float y_, float z_)
{
    x = x_;
    y = y_;
    z = z_;
}

void Point::printPoint()
{
    std::cout << "[ " << x << ", " << y << ", " << z << " ]" << std::endl;
}

void Point::normalize()
{
    float s = sqrt(x * x + y * y + z * z);
    x = x / s;
    y = y / s;
    z = z / s;
}
}  // namespace sphere
