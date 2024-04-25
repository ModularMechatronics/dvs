#ifndef DUOPLOT_MATH_IMAGE_IMAGE_TYPES_H_
#define DUOPLOT_MATH_IMAGE_IMAGE_TYPES_H_

#include <stdint.h>

#include "duoplot/enumerations.h"

typedef struct S_duoplot_ImageGray
{
    duoplot_internal_DataType data_type;
    size_t num_bytes_per_element;
    size_t num_rows;
    size_t num_cols;
    uint8_t* data;

} duoplot_ImageGray;

typedef struct S_duoplot_ImageGrayF
{
    duoplot_internal_DataType data_type;
    size_t num_bytes_per_element;
    size_t num_rows;
    size_t num_cols;
    float* data;

} duoplot_ImageGrayF;

typedef struct S_duoplot_ImageGrayD
{
    duoplot_internal_DataType data_type;
    size_t num_bytes_per_element;
    size_t num_rows;
    size_t num_cols;
    double* data;

} duoplot_ImageGrayD;

typedef struct S_duoplot_ImageGrayS8
{
    duoplot_internal_DataType data_type;
    size_t num_bytes_per_element;
    size_t num_rows;
    size_t num_cols;
    int8_t* data;

} duoplot_ImageGrayS8;

typedef struct S_duoplot_ImageGrayS16
{
    duoplot_internal_DataType data_type;
    size_t num_bytes_per_element;
    size_t num_rows;
    size_t num_cols;
    int16_t* data;

} duoplot_ImageGrayS16;

typedef struct S_duoplot_ImageGrayS32
{
    duoplot_internal_DataType data_type;
    size_t num_bytes_per_element;
    size_t num_rows;
    size_t num_cols;
    int32_t* data;

} duoplot_ImageGrayS32;

typedef struct S_duoplot_ImageGrayS64
{
    duoplot_internal_DataType data_type;
    size_t num_bytes_per_element;
    size_t num_rows;
    size_t num_cols;
    int64_t* data;

} duoplot_ImageGrayS64;

typedef struct S_duoplot_ImageGrayU8
{
    duoplot_internal_DataType data_type;
    size_t num_bytes_per_element;
    size_t num_rows;
    size_t num_cols;
    uint8_t* data;

} duoplot_ImageGrayU8;

typedef struct S_duoplot_ImageGrayU16
{
    duoplot_internal_DataType data_type;
    size_t num_bytes_per_element;
    size_t num_rows;
    size_t num_cols;
    uint16_t* data;

} duoplot_ImageGrayU16;

typedef struct S_duoplot_ImageGrayU32
{
    duoplot_internal_DataType data_type;
    size_t num_bytes_per_element;
    size_t num_rows;
    size_t num_cols;
    uint32_t* data;

} duoplot_ImageGrayU32;

typedef struct S_duoplot_ImageGrayU64
{
    duoplot_internal_DataType data_type;
    size_t num_bytes_per_element;
    size_t num_rows;
    size_t num_cols;
    uint64_t* data;

} duoplot_ImageGrayU64;

typedef struct S_duoplot_ImageRGB
{
    duoplot_internal_DataType data_type;
    size_t num_bytes_per_element;
    size_t num_rows;
    size_t num_cols;
    uint8_t* data;

} duoplot_ImageRGB;

typedef struct S_duoplot_ImageRGBF
{
    duoplot_internal_DataType data_type;
    size_t num_bytes_per_element;
    size_t num_rows;
    size_t num_cols;
    float* data;

} duoplot_ImageRGBF;

typedef struct S_duoplot_ImageRGBD
{
    duoplot_internal_DataType data_type;
    size_t num_bytes_per_element;
    size_t num_rows;
    size_t num_cols;
    double* data;

} duoplot_ImageRGBD;

typedef struct S_duoplot_ImageRGBS8
{
    duoplot_internal_DataType data_type;
    size_t num_bytes_per_element;
    size_t num_rows;
    size_t num_cols;
    int8_t* data;

} duoplot_ImageRGBS8;

typedef struct S_duoplot_ImageRGBS16
{
    duoplot_internal_DataType data_type;
    size_t num_bytes_per_element;
    size_t num_rows;
    size_t num_cols;
    int16_t* data;

} duoplot_ImageRGBS16;

typedef struct S_duoplot_ImageRGBS32
{
    duoplot_internal_DataType data_type;
    size_t num_bytes_per_element;
    size_t num_rows;
    size_t num_cols;
    int32_t* data;

} duoplot_ImageRGBS32;

typedef struct S_duoplot_ImageRGBS64
{
    duoplot_internal_DataType data_type;
    size_t num_bytes_per_element;
    size_t num_rows;
    size_t num_cols;
    int64_t* data;

} duoplot_ImageRGBS64;

typedef struct S_duoplot_ImageRGBU8
{
    duoplot_internal_DataType data_type;
    size_t num_bytes_per_element;
    size_t num_rows;
    size_t num_cols;
    uint8_t* data;

} duoplot_ImageRGBU8;

typedef struct S_duoplot_ImageRGBU16
{
    duoplot_internal_DataType data_type;
    size_t num_bytes_per_element;
    size_t num_rows;
    size_t num_cols;
    uint16_t* data;

} duoplot_ImageRGBU16;

typedef struct S_duoplot_ImageRGBU32
{
    duoplot_internal_DataType data_type;
    size_t num_bytes_per_element;
    size_t num_rows;
    size_t num_cols;
    uint32_t* data;

} duoplot_ImageRGBU32;

typedef struct S_duoplot_ImageRGBU64
{
    duoplot_internal_DataType data_type;
    size_t num_bytes_per_element;
    size_t num_rows;
    size_t num_cols;
    uint64_t* data;

} duoplot_ImageRGBU64;

#endif  // DUOPLOT_MATH_IMAGE_IMAGE_TYPES_H_
