#ifndef DUOPLOT_MATH_VECTOR_LOW_DIM_VEC3_H_
#define DUOPLOT_MATH_VECTOR_LOW_DIM_VEC3_H_

#include <stdint.h>

typedef struct S_Vec3d
{
    double x;
    double y;
    double z;
} Vec3d;

typedef struct S_Vec3f
{
    float x;
    float y;
    float z;
} Vec3f;

typedef struct S_Vec3s8
{
    int8_t x;
    int8_t y;
    int8_t z;
} Vec3s8;

typedef struct S_Vec3s16
{
    int16_t x;
    int16_t y;
    int16_t z;
} Vec3s16;

typedef struct S_Vec3s32
{
    int32_t x;
    int32_t y;
    int32_t z;
} Vec3s32;

typedef struct S_Vec3s64
{
    int64_t x;
    int64_t y;
    int64_t z;
} Vec3s64;

typedef struct S_Vec3u8
{
    uint8_t x;
    uint8_t y;
    uint8_t z;
} Vec3u8;

typedef struct S_Vec3u16
{
    uint16_t x;
    uint16_t y;
    uint16_t z;
} Vec3u16;

typedef struct S_Vec3u32
{
    uint32_t x;
    uint32_t y;
    uint32_t z;
} Vec3u32;

typedef struct S_Vec3u64
{
    uint64_t x;
    uint64_t y;
    uint64_t z;
} Vec3u64;

#define Point3d Vec3d
#define Point3f Vec3f
#define Point3s8 Vec3s8
#define Point3s16 Vec3s16
#define Point3s32 Vec3s32
#define Point3s64 Vec3s64
#define Point3u8 Vec3u8
#define Point3u16 Vec3u16
#define Point3u32 Vec3u32
#define Point3u64 Vec3u64

Vec3d createVec3d(const double x, const double y, const double z)
{
    const Vec3d v = {x, y, z};
    return v;
}

Vec3f createVec3f(const float x, const float y, const float z)
{
    const Vec3f v = {x, y, z};
    return v;
}

Vec3s8 createVec3s8(const int8_t x, const int8_t y, const int8_t z)
{
    const Vec3s8 v = {x, y, z};
    return v;
}

Vec3s16 createVec3s16(const int16_t x, const int16_t y, const int16_t z)
{
    const Vec3s16 v = {x, y, z};
    return v;
}

Vec3s32 createVec3s32(const int32_t x, const int32_t y, const int32_t z)
{
    const Vec3s32 v = {x, y, z};
    return v;
}

Vec3s64 createVec3s64(const int64_t x, const int64_t y, const int64_t z)
{
    const Vec3s64 v = {x, y, z};
    return v;
}

Vec3u8 createVec3u8(const uint8_t x, const uint8_t y, const uint8_t z)
{
    const Vec3u8 v = {x, y, z};
    return v;
}

Vec3u16 createVec3u16(const uint16_t x, const uint16_t y, const uint16_t z)
{
    const Vec3u16 v = {x, y, z};
    return v;
}

Vec3u32 createVec3u32(const uint32_t x, const uint32_t y, const uint32_t z)
{
    const Vec3u32 v = {x, y, z};
    return v;
}

Vec3u64 createVec3u64(const uint64_t x, const uint64_t y, const uint64_t z)
{
    const Vec3u64 v = {x, y, z};
    return v;
}

#define createPoint3d createVec3d
#define createPoint3f createVec3f
#define createPoint3s8 createVec3s8
#define createPoint3s16 createVec3s16
#define createPoint3s32 createVec3s32
#define createPoint3s64 createVec3s64
#define createPoint3u8 createVec3u8
#define createPoint3u16 createVec3u16
#define createPoint3u32 createVec3u32
#define createPoint3u64 createVec3u64

typedef struct S_Vec3dArray
{
    Vec3d* elements;
    size_t num_elements;
} Vec3dArray;

typedef struct S_Vec3fArray
{
    Vec3f* elements;
    size_t num_elements;
} Vec3fArray;

Vec3dArray createVec3dArray(const size_t num_elements)
{
    Vec3dArray array;
    array.elements = malloc(sizeof(Vec3d) * num_elements);
    array.num_elements = num_elements;

    return array;
}

Vec3fArray createVec3fArray(const size_t num_elements)
{
    Vec3fArray array;
    array.elements = malloc(sizeof(Vec3f) * num_elements);
    array.num_elements = num_elements;

    return array;
}

#define Point3dArray Vec3dArray
#define Point3fArray Vec3fArray

#endif  // DUOPLOT_MATH_VECTOR_LOW_DIM_VEC3_H_
