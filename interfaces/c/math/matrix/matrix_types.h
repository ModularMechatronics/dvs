#ifndef DVS_MATRIX_TYPES_H_
#define DVS_MATRIX_TYPES_H_

#include <stdint.h>

#include "enumerations.h"

typedef struct S_Matrix
{
    DataType data_type;
    size_t num_bytes_per_element;
    size_t num_rows;
    size_t num_cols;
    uint8_t* data;

} Matrix;

typedef struct S_MatrixF
{
    DataType data_type;
    size_t num_bytes_per_element;
    size_t num_rows;
    size_t num_cols;
    float* data;

} MatrixF;

typedef struct S_MatrixD
{
    DataType data_type;
    size_t num_bytes_per_element;
    size_t num_rows;
    size_t num_cols;
    double* data;

} MatrixD;

typedef struct S_MatrixS8
{
    DataType data_type;
    size_t num_bytes_per_element;
    size_t num_rows;
    size_t num_cols;
    int8_t* data;

} MatrixS8;

typedef struct S_MatrixS16
{
    DataType data_type;
    size_t num_bytes_per_element;
    size_t num_rows;
    size_t num_cols;
    int16_t* data;

} MatrixS16;

typedef struct S_MatrixS32
{
    DataType data_type;
    size_t num_bytes_per_element;
    size_t num_rows;
    size_t num_cols;
    int32_t* data;

} MatrixS32;

typedef struct S_MatrixS64
{
    DataType data_type;
    size_t num_bytes_per_element;
    size_t num_rows;
    size_t num_cols;
    int64_t* data;

} MatrixS64;

typedef struct S_MatrixU8
{
    DataType data_type;
    size_t num_bytes_per_element;
    size_t num_rows;
    size_t num_cols;
    uint8_t* data;

} MatrixU8;

typedef struct S_MatrixU16
{
    DataType data_type;
    size_t num_bytes_per_element;
    size_t num_rows;
    size_t num_cols;
    uint16_t* data;

} MatrixU16;

typedef struct S_MatrixU32
{
    DataType data_type;
    size_t num_bytes_per_element;
    size_t num_rows;
    size_t num_cols;
    uint32_t* data;

} MatrixU32;

typedef struct S_MatrixU64
{
    DataType data_type;
    size_t num_bytes_per_element;
    size_t num_rows;
    size_t num_cols;
    uint64_t* data;

} MatrixU64;

typedef struct S_MatrixPair
{
    Matrix m0;
    Matrix m1;
} MatrixPair;

typedef struct S_MatrixPairD
{
    MatrixD m0;
    MatrixD m1;
} MatrixPairD;

#endif
