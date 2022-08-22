#ifndef DVS_MATH_GEOMETRY_LINE_2D_H_
#define DVS_MATH_GEOMETRY_LINE_2D_H_

#include "dvs/math/vector_low_dim/vec2.h"

typedef struct S_PLine2DD
{
    Point2d p;
    Vec2d v;
} PLine2DD;

typedef struct S_HLine2DD
{
    double a;
    double b;
    double c;
} HLine2DD;

#endif // DVS_MATH_GEOMETRY_LINE_2D_H_
