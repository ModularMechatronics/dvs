#ifndef DUOPLOT_MATH_IMAGE_IMAGE_TYPES_H_
#define DUOPLOT_MATH_IMAGE_IMAGE_TYPES_H_

#include <stdint.h>

#include "duoplot/enumerations.h"

typedef struct S_duoplot_ImageC1
{
    DataType data_type;
    size_t num_bytes_per_element;
    size_t num_rows;
    size_t num_cols;
    uint8_t* data;

} duoplot_ImageC1;

typedef struct S_duoplot_ImageC1F
{
    DataType data_type;
    size_t num_bytes_per_element;
    size_t num_rows;
    size_t num_cols;
    float* data;

} duoplot_ImageC1F;

typedef struct S_duoplot_ImageC1D
{
    DataType data_type;
    size_t num_bytes_per_element;
    size_t num_rows;
    size_t num_cols;
    double* data;

} duoplot_ImageC1D;

typedef struct S_duoplot_ImageC1S8
{
    DataType data_type;
    size_t num_bytes_per_element;
    size_t num_rows;
    size_t num_cols;
    int8_t* data;

} duoplot_ImageC1S8;

typedef struct S_duoplot_ImageC1S16
{
    DataType data_type;
    size_t num_bytes_per_element;
    size_t num_rows;
    size_t num_cols;
    int16_t* data;

} duoplot_ImageC1S16;

typedef struct S_duoplot_ImageC1S32
{
    DataType data_type;
    size_t num_bytes_per_element;
    size_t num_rows;
    size_t num_cols;
    int32_t* data;

} duoplot_ImageC1S32;

typedef struct S_duoplot_ImageC1S64
{
    DataType data_type;
    size_t num_bytes_per_element;
    size_t num_rows;
    size_t num_cols;
    int64_t* data;

} duoplot_ImageC1S64;

typedef struct S_duoplot_ImageC1U8
{
    DataType data_type;
    size_t num_bytes_per_element;
    size_t num_rows;
    size_t num_cols;
    uint8_t* data;

} duoplot_ImageC1U8;

typedef struct S_duoplot_ImageC1U16
{
    DataType data_type;
    size_t num_bytes_per_element;
    size_t num_rows;
    size_t num_cols;
    uint16_t* data;

} duoplot_ImageC1U16;

typedef struct S_duoplot_ImageC1U32
{
    DataType data_type;
    size_t num_bytes_per_element;
    size_t num_rows;
    size_t num_cols;
    uint32_t* data;

} duoplot_ImageC1U32;

typedef struct S_duoplot_ImageC1U64
{
    DataType data_type;
    size_t num_bytes_per_element;
    size_t num_rows;
    size_t num_cols;
    uint64_t* data;

} duoplot_ImageC1U64;

typedef struct S_duoplot_ImageC3
{
    DataType data_type;
    size_t num_bytes_per_element;
    size_t num_rows;
    size_t num_cols;
    uint8_t* data;

} duoplot_ImageC3;

typedef struct S_duoplot_ImageC3F
{
    DataType data_type;
    size_t num_bytes_per_element;
    size_t num_rows;
    size_t num_cols;
    float* data;

} duoplot_ImageC3F;

typedef struct S_duoplot_ImageC3D
{
    DataType data_type;
    size_t num_bytes_per_element;
    size_t num_rows;
    size_t num_cols;
    double* data;

} duoplot_ImageC3D;

typedef struct S_duoplot_ImageC3S8
{
    DataType data_type;
    size_t num_bytes_per_element;
    size_t num_rows;
    size_t num_cols;
    int8_t* data;

} duoplot_ImageC3S8;

typedef struct S_duoplot_ImageC3S16
{
    DataType data_type;
    size_t num_bytes_per_element;
    size_t num_rows;
    size_t num_cols;
    int16_t* data;

} duoplot_ImageC3S16;

typedef struct S_duoplot_ImageC3S32
{
    DataType data_type;
    size_t num_bytes_per_element;
    size_t num_rows;
    size_t num_cols;
    int32_t* data;

} duoplot_ImageC3S32;

typedef struct S_duoplot_ImageC3S64
{
    DataType data_type;
    size_t num_bytes_per_element;
    size_t num_rows;
    size_t num_cols;
    int64_t* data;

} duoplot_ImageC3S64;

typedef struct S_duoplot_ImageC3U8
{
    DataType data_type;
    size_t num_bytes_per_element;
    size_t num_rows;
    size_t num_cols;
    uint8_t* data;

} duoplot_ImageC3U8;

typedef struct S_duoplot_ImageC3U16
{
    DataType data_type;
    size_t num_bytes_per_element;
    size_t num_rows;
    size_t num_cols;
    uint16_t* data;

} duoplot_ImageC3U16;

typedef struct S_duoplot_ImageC3U32
{
    DataType data_type;
    size_t num_bytes_per_element;
    size_t num_rows;
    size_t num_cols;
    uint32_t* data;

} duoplot_ImageC3U32;

typedef struct S_duoplot_ImageC3U64
{
    DataType data_type;
    size_t num_bytes_per_element;
    size_t num_rows;
    size_t num_cols;
    uint64_t* data;

} duoplot_ImageC3U64;

#endif  // DUOPLOT_MATH_IMAGE_IMAGE_TYPES_H_
