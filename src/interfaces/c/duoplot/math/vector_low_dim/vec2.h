#ifndef DUOPLOT_MATH_VECTOR_LOW_DIM_VEC2_H_
#define DUOPLOT_MATH_VECTOR_LOW_DIM_VEC2_H_

#include <stdint.h>

typedef struct S_duoplot_Vec2F
{
    float x;
    float y;
} duoplot_Vec2F;

typedef struct S_duoplot_Vec2D
{
    double x;
    double y;
} duoplot_Vec2D;

typedef struct S_duoplot_VecXYD
{
    double x;
    double y;
} duoplot_VecXYD;

typedef struct S_duoplot_VecXZD
{
    double x;
    double z;
} duoplot_VecXZD;

typedef struct S_duoplot_VecYZD
{
    double y;
    double z;
} duoplot_VecYZD;

#define duoplot_Point2F duoplot_Vec2F
#define duoplot_Point2D duoplot_Vec2D

#define duoplot_PointXYD duoplot_VecXYD
#define duoplot_PointXZD duoplot_VecXZD
#define duoplot_PointYZD duoplot_VecYZD

#endif  // DUOPLOT_MATH_VECTOR_LOW_DIM_VEC2_H_
