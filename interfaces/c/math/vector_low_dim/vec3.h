#ifndef DVS_VECTOR_LOW_DIM_H_
#define DVS_VECTOR_LOW_DIM_H_

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

#endif
