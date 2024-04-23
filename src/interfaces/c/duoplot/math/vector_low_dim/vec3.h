#ifndef DUOPLOT_MATH_VECTOR_LOW_DIM_VEC3_H_
#define DUOPLOT_MATH_VECTOR_LOW_DIM_VEC3_H_

#include <stdint.h>

#include "duoplot/pp.h"

typedef struct S_duoplot_Vec3D
{
    double x;
    double y;
    double z;
} duoplot_Vec3D;

typedef struct S_duoplot_Vec3F
{
    float x;
    float y;
    float z;
} duoplot_Vec3F;

typedef struct S_duoplot_Vec3S8
{
    int8_t x;
    int8_t y;
    int8_t z;
} duoplot_Vec3S8;

typedef struct S_duoplot_Vec3S16
{
    int16_t x;
    int16_t y;
    int16_t z;
} duoplot_Vec3S16;

typedef struct S_duoplot_Vec3S32
{
    int32_t x;
    int32_t y;
    int32_t z;
} duoplot_Vec3S32;

typedef struct S_duoplot_Vec3S64
{
    int64_t x;
    int64_t y;
    int64_t z;
} duoplot_Vec3S64;

typedef struct S_duoplot_Vec3U8
{
    uint8_t x;
    uint8_t y;
    uint8_t z;
} duoplot_Vec3U8;

typedef struct S_duoplot_Vec3U16
{
    uint16_t x;
    uint16_t y;
    uint16_t z;
} duoplot_Vec3U16;

typedef struct S_duoplot_Vec3U32
{
    uint32_t x;
    uint32_t y;
    uint32_t z;
} duoplot_Vec3U32;

typedef struct S_duoplot_Vec3U64
{
    uint64_t x;
    uint64_t y;
    uint64_t z;
} duoplot_Vec3U64;

#define duoplot_Point3D duoplot_Vec3D
#define duoplot_Point3F duoplot_Vec3F
#define duoplot_Point3S8 duoplot_Vec3S8
#define duoplot_Point3S16 duoplot_Vec3S16
#define duoplot_Point3S32 duoplot_Vec3S32
#define duoplot_Point3S64 duoplot_Vec3S64
#define duoplot_Point3U8 duoplot_Vec3U8
#define duoplot_Point3U16 duoplot_Vec3U16
#define duoplot_Point3U32 duoplot_Vec3U32
#define duoplot_Point3U64 duoplot_Vec3U64

typedef struct S_duoplot_Vec3DArray
{
    duoplot_Vec3D* elements;
    size_t num_elements;
} duoplot_Vec3DArray;

typedef struct S_duoplot_Vec3FArray
{
    duoplot_Vec3F* elements;
    size_t num_elements;
} duoplot_Vec3FArray;

DUOPLOT_WEAK duoplot_Vec3DArray duoplot_createVec3DArray(const size_t num_elements)
{
    duoplot_Vec3DArray array;
    array.elements = malloc(sizeof(duoplot_Vec3D) * num_elements);
    array.num_elements = num_elements;

    return array;
}

DUOPLOT_WEAK duoplot_Vec3FArray duoplot_createVec3FArray(const size_t num_elements)
{
    duoplot_Vec3FArray array;
    array.elements = malloc(sizeof(duoplot_Vec3F) * num_elements);
    array.num_elements = num_elements;

    return array;
}

#define duoplot_Point3DArray duoplot_Vec3DArray
#define duoplot_Point3FArray duoplot_Vec3FArray

#define duoplot_createPoint3DArray duoplot_createVec3DArray
#define duoplot_createPoint3FArray duoplot_createVec3FArray

#endif  // DUOPLOT_MATH_VECTOR_LOW_DIM_VEC3_H_
