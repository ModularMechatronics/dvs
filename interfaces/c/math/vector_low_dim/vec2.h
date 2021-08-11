#ifndef DVS_VEC2_H_
#define DVS_VEC2_H_

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

#endif
