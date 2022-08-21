#ifndef DVS_MATH_VECTOR_LOW_DIM_VEC3_H_
#define DVS_MATH_VECTOR_LOW_DIM_VEC3_H_

#include <stdint.h>

typedef struct S_Vec3DD
{
    double x;
    double y;
    double z;
} Vec3DD;

typedef struct S_Vec3DF
{
    float x;
    float y;
    float z;
} Vec3DF;

typedef struct S_Vec3DS8
{
    int8_t x;
    int8_t y;
    int8_t z;
} Vec3DS8;

typedef struct S_Vec3DS16
{
    int16_t x;
    int16_t y;
    int16_t z;
} Vec3DS16;

typedef struct S_Vec3DS32
{
    int32_t x;
    int32_t y;
    int32_t z;
} Vec3DS32;

typedef struct S_Vec3DS64
{
    int64_t x;
    int64_t y;
    int64_t z;
} Vec3DS64;

typedef struct S_Vec3DU8
{
    uint8_t x;
    uint8_t y;
    uint8_t z;
} Vec3DU8;

typedef struct S_Vec3DU16
{
    uint16_t x;
    uint16_t y;
    uint16_t z;
} Vec3DU16;

typedef struct S_Vec3DU32
{
    uint32_t x;
    uint32_t y;
    uint32_t z;
} Vec3DU32;

typedef struct S_Vec3DU64
{
    uint64_t x;
    uint64_t y;
    uint64_t z;
} Vec3DU64;

#define Point3DD Vec3DD
#define Point3DF Vec3DF
#define Point3DS8 Vec3DS8
#define Point3DS16 Vec3DS16
#define Point3DS32 Vec3DS32
#define Point3DS64 Vec3DS64
#define Point3DU8 Vec3DU8
#define Point3DU16 Vec3DU16
#define Point3DU32 Vec3DU32
#define Point3DU64 Vec3DU64

Vec3DD createVec3DD(const double x, const double y, const double z)
{
    const Vec3DD v = {x, y, z};
    return v;
}

Vec3DF createVec3DF(const float x, const float y, const float z)
{
    const Vec3DF v = {x, y, z};
    return v;
}

Vec3DS8 createVec3DS8(const int8_t x, const int8_t y, const int8_t z)
{
    const Vec3DS8 v = {x, y, z};
    return v;
}

Vec3DS16 createVec3DS16(const int16_t x, const int16_t y, const int16_t z)
{
    const Vec3DS16 v = {x, y, z};
    return v;
}

Vec3DS32 createVec3DS32(const int32_t x, const int32_t y, const int32_t z)
{
    const Vec3DS32 v = {x, y, z};
    return v;
}

Vec3DS64 createVec3DS64(const int64_t x, const int64_t y, const int64_t z)
{
    const Vec3DS64 v = {x, y, z};
    return v;
}

Vec3DU8 createVec3DU8(const uint8_t x, const uint8_t y, const uint8_t z)
{
    const Vec3DU8 v = {x, y, z};
    return v;
}

Vec3DU16 createVec3DU16(const uint16_t x, const uint16_t y, const uint16_t z)
{
    const Vec3DU16 v = {x, y, z};
    return v;
}

Vec3DU32 createVec3DU32(const uint32_t x, const uint32_t y, const uint32_t z)
{
    const Vec3DU32 v = {x, y, z};
    return v;
}

Vec3DU64 createVec3DU64(const uint64_t x, const uint64_t y, const uint64_t z)
{
    const Vec3DU64 v = {x, y, z};
    return v;
}

#define createPoint3DD createVec3DD
#define createPoint3DF createVec3DF
#define createPoint3DS8 createVec3DS8
#define createPoint3DS16 createVec3DS16
#define createPoint3DS32 createVec3DS32
#define createPoint3DS64 createVec3DS64
#define createPoint3DU8 createVec3DU8
#define createPoint3DU16 createVec3DU16
#define createPoint3DU32 createVec3DU32
#define createPoint3DU64 createVec3DU64

typedef struct S_Vec3DDArray
{
    Vec3DD* elements;
    size_t num_elements;
} Vec3DDArray;

typedef struct S_Vec3DFArray
{
    Vec3DF* elements;
    size_t num_elements;
} Vec3DFArray;

Vec3DDArray createVec3DDArray(const size_t num_elements)
{
    Vec3DDArray array;
    array.elements = malloc(sizeof(Vec3DD) * num_elements);
    array.num_elements = num_elements;

    return array;
}

Vec3DFArray createVec3DFArray(const size_t num_elements)
{
    Vec3DFArray array;
    array.elements = malloc(sizeof(Vec3DF) * num_elements);
    array.num_elements = num_elements;

    return array;
}

#define Point3DDArray Vec3DDArray
#define Point3DFArray Vec3DFArray

#endif // DVS_MATH_VECTOR_LOW_DIM_VEC3_H_
