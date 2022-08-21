#ifndef DVS_MATH_VECTOR_LOW_DIM_VEC2_H_
#define DVS_MATH_VECTOR_LOW_DIM_VEC2_H_

#include <stdint.h>

typedef struct S_Vec2DD
{
    double x;
    double y;
} Vec2DD;

typedef struct S_VecXYD
{
    double x;
    double y;
} VecXYD;

typedef struct S_VecXZD
{
    double x;
    double z;
} VecXZD;

typedef struct S_VecYZD
{
    double y;
    double z;
} VecYZD;

#define Point2DF Vec2DF
#define Point2DD Vec2DD

#define PointXYD VecXYD
#define PointXZD VecXZD
#define PointYZD VecYZD

#endif // DVS_MATH_VECTOR_LOW_DIM_VEC2_H_
