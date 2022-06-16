#ifndef DVS_VECTOR_TYPES_H_
#define DVS_VECTOR_TYPES_H_

#include <stdint.h>

#include "dvs/enumerations.h"

typedef struct S_Vector
{
    DataType data_type;
    size_t num_bytes_per_element;
    size_t num_elements;
    uint8_t* data;

} Vector;

typedef struct S_VectorF
{
    DataType data_type;
    size_t num_bytes_per_element;
    size_t num_elements;
    float* data;

} VectorF;

typedef struct S_VectorD
{
    DataType data_type;
    size_t num_bytes_per_element;
    size_t num_elements;
    double* data;

} VectorD;

typedef struct S_VectorS8
{
    DataType data_type;
    size_t num_bytes_per_element;
    size_t num_elements;
    int8_t* data;

} VectorS8;

typedef struct S_VectorS16
{
    DataType data_type;
    size_t num_bytes_per_element;
    size_t num_elements;
    int16_t* data;

} VectorS16;

typedef struct S_VectorS32
{
    DataType data_type;
    size_t num_bytes_per_element;
    size_t num_elements;
    int32_t* data;

} VectorS32;

typedef struct S_VectorS64
{
    DataType data_type;
    size_t num_bytes_per_element;
    size_t num_elements;
    int64_t* data;

} VectorS64;

//
typedef struct S_VectorU8
{
    DataType data_type;
    size_t num_bytes_per_element;
    size_t num_elements;
    uint8_t* data;

} VectorU8;

typedef struct S_VectorU16
{
    DataType data_type;
    size_t num_bytes_per_element;
    size_t num_elements;
    uint16_t* data;

} VectorU16;

typedef struct S_VectorU32
{
    DataType data_type;
    size_t num_bytes_per_element;
    size_t num_elements;
    uint32_t* data;

} VectorU32;

typedef struct S_VectorU64
{
    DataType data_type;
    size_t num_bytes_per_element;
    size_t num_elements;
    uint64_t* data;

} VectorU64;

#endif
