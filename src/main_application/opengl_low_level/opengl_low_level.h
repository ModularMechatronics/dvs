#ifndef MAIN_APPLICATION_OPENGL_LOW_LEVEL_OPENGL_LOW_LEVEL_H_
#define MAIN_APPLICATION_OPENGL_LOW_LEVEL_OPENGL_LOW_LEVEL_H_

#include <assert.h>

#include <cmath>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <vector>

#include "opengl_low_level/data_structures.h"
#include "opengl_low_level/opengl_header.h"
#include "dvs/math/math.h"

inline void setColor(const float r, const float g, const float b)
{
    glColor3f(r, g, b);
}

inline void setColor(const RGBTripletf& pc)
{
    glColor3f(pc.red, pc.green, pc.blue);
}

inline void setLinewidth(const float line_width)
{
    glLineWidth(line_width);
}

inline void setPointSize(const float point_size)
{
    glPointSize(point_size);
}

#endif // MAIN_APPLICATION_OPENGL_LOW_LEVEL_OPENGL_LOW_LEVEL_H_
