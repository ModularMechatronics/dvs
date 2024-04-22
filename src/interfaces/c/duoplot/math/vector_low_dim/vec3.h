#ifndef DUOPLOT_MATH_VECTOR_LOW_DIM_VEC3_H_
#define DUOPLOT_MATH_VECTOR_LOW_DIM_VEC3_H_

#include <stdint.h>

#include "duoplot/pp.h"

typedef struct S_Vec3D
{
    double x;
    double y;
    double z;
} Vec3D;

typedef struct S_Vec3F
{
    float x;
    float y;
    float z;
} Vec3F;

typedef struct S_Vec3S8
{
    int8_t x;
    int8_t y;
    int8_t z;
} Vec3S8;

typedef struct S_Vec3S16
{
    int16_t x;
    int16_t y;
    int16_t z;
} Vec3S16;

typedef struct S_Vec3S32
{
    int32_t x;
    int32_t y;
    int32_t z;
} Vec3S32;

typedef struct S_Vec3S64
{
    int64_t x;
    int64_t y;
    int64_t z;
} Vec3S64;

typedef struct S_Vec3U8
{
    uint8_t x;
    uint8_t y;
    uint8_t z;
} Vec3U8;

typedef struct S_Vec3U16
{
    uint16_t x;
    uint16_t y;
    uint16_t z;
} Vec3U16;

typedef struct S_Vec3U32
{
    uint32_t x;
    uint32_t y;
    uint32_t z;
} Vec3U32;

typedef struct S_Vec3U64
{
    uint64_t x;
    uint64_t y;
    uint64_t z;
} Vec3U64;

#define Point3D Vec3D
#define Point3F Vec3F
#define Point3S8 Vec3S8
#define Point3S16 Vec3S16
#define Point3S32 Vec3S32
#define Point3S64 Vec3S64
#define Point3U8 Vec3U8
#define Point3U16 Vec3U16
#define Point3U32 Vec3U32
#define Point3U64 Vec3U64

DUOPLOT_WEAK Vec3D createVec3D(const double x, const double y, const double z)
{
    const Vec3D v = {x, y, z};
    return v;
}

DUOPLOT_WEAK Vec3F createVec3F(const float x, const float y, const float z)
{
    const Vec3F v = {x, y, z};
    return v;
}

DUOPLOT_WEAK Vec3S8 createVec3S8(const int8_t x, const int8_t y, const int8_t z)
{
    const Vec3S8 v = {x, y, z};
    return v;
}

DUOPLOT_WEAK Vec3S16 createVec3S16(const int16_t x, const int16_t y, const int16_t z)
{
    const Vec3S16 v = {x, y, z};
    return v;
}

DUOPLOT_WEAK Vec3S32 createVec3S32(const int32_t x, const int32_t y, const int32_t z)
{
    const Vec3S32 v = {x, y, z};
    return v;
}

DUOPLOT_WEAK Vec3S64 createVec3S64(const int64_t x, const int64_t y, const int64_t z)
{
    const Vec3S64 v = {x, y, z};
    return v;
}

DUOPLOT_WEAK Vec3U8 createVec3U8(const uint8_t x, const uint8_t y, const uint8_t z)
{
    const Vec3U8 v = {x, y, z};
    return v;
}

DUOPLOT_WEAK Vec3U16 createVec3U16(const uint16_t x, const uint16_t y, const uint16_t z)
{
    const Vec3U16 v = {x, y, z};
    return v;
}

DUOPLOT_WEAK Vec3U32 createVec3U32(const uint32_t x, const uint32_t y, const uint32_t z)
{
    const Vec3U32 v = {x, y, z};
    return v;
}

DUOPLOT_WEAK Vec3U64 createVec3U64(const uint64_t x, const uint64_t y, const uint64_t z)
{
    const Vec3U64 v = {x, y, z};
    return v;
}

#define createPoint3D createVec3D
#define createPoint3F createVec3F
#define createPoint3S8 createVec3S8
#define createPoint3S16 createVec3S16
#define createPoint3S32 createVec3S32
#define createPoint3S64 createVec3S64
#define createPoint3U8 createVec3U8
#define createPoint3U16 createVec3U16
#define createPoint3U32 createVec3U32
#define createPoint3U64 createVec3U64

typedef struct S_Vec3DArray
{
    Vec3D* elements;
    size_t num_elements;
} Vec3DArray;

typedef struct S_Vec3FArray
{
    Vec3F* elements;
    size_t num_elements;
} Vec3FArray;

DUOPLOT_WEAK Vec3DArray createVec3DArray(const size_t num_elements)
{
    Vec3DArray array;
    array.elements = malloc(sizeof(Vec3D) * num_elements);
    array.num_elements = num_elements;

    return array;
}

DUOPLOT_WEAK Vec3FArray createVec3FArray(const size_t num_elements)
{
    Vec3FArray array;
    array.elements = malloc(sizeof(Vec3F) * num_elements);
    array.num_elements = num_elements;

    return array;
}

#define Point3DArray Vec3DArray
#define Point3FArray Vec3FArray

#endif  // DUOPLOT_MATH_VECTOR_LOW_DIM_VEC3_H_
