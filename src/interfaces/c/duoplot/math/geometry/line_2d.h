#ifndef DUOPLOT_MATH_GEOMETRY_LINE_2D_H_
#define DUOPLOT_MATH_GEOMETRY_LINE_2D_H_

#include "duoplot/math/vector_low_dim/vec2.h"

typedef struct S_duoplot_duoplot_PLine2DD
{
    duoplot_Point2D p;
    duoplot_Vec2D v;
} duoplot_PLine2DD;

typedef struct S_duoplot_HLine2DD
{
    double a;
    double b;
    double c;
} duoplot_HLine2DD;

#endif  // DUOPLOT_MATH_GEOMETRY_LINE_2D_H_
