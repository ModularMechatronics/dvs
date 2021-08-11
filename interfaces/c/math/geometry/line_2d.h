#ifndef DVS_LINE_2D_H_
#define DVS_LINE_2D_H_

#include "math/vector_low_dim/vec2.h"

typedef struct S_PLine2D
{
    Point2DD p;
    Vec2DD v;
} PLine2D;

typedef struct S_HLine2D
{
    double a;
    double b;
    double c;
} HLine2D;

#endif
