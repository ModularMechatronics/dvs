#ifndef OPENGL_FUNCTIONS_H_
#define OPENGL_FUNCTIONS_H_

#include <cstddef>
#include <vector>

#include "dvs/math/math.h"
#include "opengl_low_level/data_structures.h"

using namespace dvs;

void setColor(const RGBTripletf& pc);
void setColor(const float r, const float g, const float b);

void setLinewidth(const float line_width);
void setPointSize(const float point_size);


#endif
