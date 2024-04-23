#ifndef DUOPLOT_MATH_VECTOR_VECTOR_TYPES_H
#define DUOPLOT_MATH_VECTOR_VECTOR_TYPES_H

#include <stdint.h>

#include "duoplot/enumerations.h"

typedef struct S_duoplot_Vector
{
    DataType data_type;
    size_t num_bytes_per_element;
    size_t num_elements;
    uint8_t* data;

} duoplot_Vector;

typedef struct S_duoplot_VectorF
{
    DataType data_type;
    size_t num_bytes_per_element;
    size_t num_elements;
    float* data;

} duoplot_VectorF;

typedef struct S_duoplot_VectorD
{
    DataType data_type;
    size_t num_bytes_per_element;
    size_t num_elements;
    double* data;

} duoplot_VectorD;

typedef struct S_duoplot_VectorS8
{
    DataType data_type;
    size_t num_bytes_per_element;
    size_t num_elements;
    int8_t* data;

} duoplot_VectorS8;

typedef struct S_duoplot_VectorS16
{
    DataType data_type;
    size_t num_bytes_per_element;
    size_t num_elements;
    int16_t* data;

} duoplot_VectorS16;

typedef struct S_duoplot_VectorS32
{
    DataType data_type;
    size_t num_bytes_per_element;
    size_t num_elements;
    int32_t* data;

} duoplot_VectorS32;

typedef struct S_duoplot_VectorS64
{
    DataType data_type;
    size_t num_bytes_per_element;
    size_t num_elements;
    int64_t* data;

} duoplot_VectorS64;

//
typedef struct S_duoplot_VectorU8
{
    DataType data_type;
    size_t num_bytes_per_element;
    size_t num_elements;
    uint8_t* data;

} duoplot_VectorU8;

typedef struct S_duoplot_VectorU16
{
    DataType data_type;
    size_t num_bytes_per_element;
    size_t num_elements;
    uint16_t* data;

} duoplot_VectorU16;

typedef struct S_duoplot_VectorU32
{
    DataType data_type;
    size_t num_bytes_per_element;
    size_t num_elements;
    uint32_t* data;

} duoplot_VectorU32;

typedef struct S_duoplot_VectorU64
{
    DataType data_type;
    size_t num_bytes_per_element;
    size_t num_elements;
    uint64_t* data;

} duoplot_VectorU64;

#endif  // DUOPLOT_MATH_VECTOR_VECTOR_TYPES_H
