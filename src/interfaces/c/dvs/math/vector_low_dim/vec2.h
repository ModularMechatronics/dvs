#ifndef DVS_MATH_VECTOR_LOW_DIM_VEC2_H_
#define DVS_MATH_VECTOR_LOW_DIM_VEC2_H_

#include <stdint.h>

typedef struct S_Vec2f
{
    float x;
    float y;
} Vec2f;

typedef struct S_Vec2d
{
    double x;
    double y;
} Vec2d;

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

#define Point2f Vec2f
#define Point2d Vec2d

#define PointXYd VecXYd
#define PointXZd VecXZd
#define PointYZd VecYZd

#endif  // DVS_MATH_VECTOR_LOW_DIM_VEC2_H_
