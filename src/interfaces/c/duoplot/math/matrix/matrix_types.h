#ifndef DUOPLOT_MATH_MATRIX_MATRIX_TYPES_H_
#define DUOPLOT_MATH_MATRIX_MATRIX_TYPES_H_

#include <stdint.h>

#include "duoplot/enumerations.h"

typedef struct S_duoplot_Matrix
{
    duoplot_internal_DataType data_type;
    size_t num_bytes_per_element;
    size_t num_rows;
    size_t num_cols;
    uint8_t* data;

} duoplot_Matrix;

typedef struct S_duoplot_MatrixF
{
    duoplot_internal_DataType data_type;
    size_t num_bytes_per_element;
    size_t num_rows;
    size_t num_cols;
    float* data;

} duoplot_MatrixF;

typedef struct S_duoplot_MatrixD
{
    duoplot_internal_DataType data_type;
    size_t num_bytes_per_element;
    size_t num_rows;
    size_t num_cols;
    double* data;

} duoplot_MatrixD;

typedef struct S_duoplot_MatrixS8
{
    duoplot_internal_DataType data_type;
    size_t num_bytes_per_element;
    size_t num_rows;
    size_t num_cols;
    int8_t* data;

} duoplot_MatrixS8;

typedef struct S_duoplot_MatrixS16
{
    duoplot_internal_DataType data_type;
    size_t num_bytes_per_element;
    size_t num_rows;
    size_t num_cols;
    int16_t* data;

} duoplot_MatrixS16;

typedef struct S_duoplot_MatrixS32
{
    duoplot_internal_DataType data_type;
    size_t num_bytes_per_element;
    size_t num_rows;
    size_t num_cols;
    int32_t* data;

} duoplot_MatrixS32;

typedef struct S_duoplot_MatrixS64
{
    duoplot_internal_DataType data_type;
    size_t num_bytes_per_element;
    size_t num_rows;
    size_t num_cols;
    int64_t* data;

} duoplot_MatrixS64;

typedef struct S_duoplot_MatrixU8
{
    duoplot_internal_DataType data_type;
    size_t num_bytes_per_element;
    size_t num_rows;
    size_t num_cols;
    uint8_t* data;

} duoplot_MatrixU8;

typedef struct S_duoplot_MatrixU16
{
    duoplot_internal_DataType data_type;
    size_t num_bytes_per_element;
    size_t num_rows;
    size_t num_cols;
    uint16_t* data;

} duoplot_MatrixU16;

typedef struct S_duoplot_MatrixU32
{
    duoplot_internal_DataType data_type;
    size_t num_bytes_per_element;
    size_t num_rows;
    size_t num_cols;
    uint32_t* data;

} duoplot_MatrixU32;

typedef struct S_duoplot_MatrixU64
{
    duoplot_internal_DataType data_type;
    size_t num_bytes_per_element;
    size_t num_rows;
    size_t num_cols;
    uint64_t* data;

} duoplot_MatrixU64;

typedef struct S_duoplot_MatrixPair
{
    duoplot_Matrix m0;
    duoplot_Matrix m1;
} MatrixPair;

typedef struct S_duoplot_MatrixPairD
{
    duoplot_MatrixD m0;
    duoplot_MatrixD m1;
} duoplot_MatrixPairD;

#endif  // DUOPLOT_MATH_MATRIX_MATRIX_TYPES_H_
