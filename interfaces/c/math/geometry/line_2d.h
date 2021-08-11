#ifndef DVS_LINE_2D_H_
#define DVS_LINE_2D_H_

#include "math/vector_low_dim/vec2.h"

typedef struct S_PLine2DD
{
    Point2DD p;
    Vec2DD v;
} PLine2DD;

typedef struct S_HLine2DD
{
    double a;
    double b;
    double c;
} HLine2DD;

#endif
