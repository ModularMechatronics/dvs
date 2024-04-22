#ifndef DUOPLOT_MATH_VECTOR_LOW_DIM_VEC2_H_
#define DUOPLOT_MATH_VECTOR_LOW_DIM_VEC2_H_

#include <stdint.h>

typedef struct S_Vec2F
{
    float x;
    float y;
} Vec2F;

typedef struct S_Vec2D
{
    double x;
    double y;
} Vec2D;

typedef struct S_VecXYd
{
    double x;
    double y;
} VecXYd;

typedef struct S_VecXZd
{
    double x;
    double z;
} VecXZd;

typedef struct S_VecYZd
{
    double y;
    double z;
} VecYZd;

#define Point2f Vec2F
#define Point2d Vec2D

#define PointXYd VecXYd
#define PointXZd VecXZd
#define PointYZd VecYZd

#endif  // DUOPLOT_MATH_VECTOR_LOW_DIM_VEC2_H_
