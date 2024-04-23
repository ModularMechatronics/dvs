#ifndef DUOPLOT_MATH_MATRIX_MATRIX_INSTANTIATION_H_
#define DUOPLOT_MATH_MATRIX_MATRIX_INSTANTIATION_H_

#include "duoplot/math/matrix/matrix_types.h"
#include "duoplot/math/utils.h"
#include "duoplot/pp.h"

DUOPLOT_WEAK duoplot_Matrix duoplot_createMatrix(const size_t num_rows, const size_t num_cols, const DataType data_type)
{
    duoplot_Matrix mat;
    mat.data_type = data_type;
    mat.num_bytes_per_element = duoplot_internal_dataTypeToNumBytes(data_type);
    mat.num_rows = num_rows;
    mat.num_cols = num_cols;
    mat.data = (uint8_t*)malloc(num_rows * num_cols * mat.num_bytes_per_element);

    return mat;
}

#define freeMatrix(mat) free(mat.data)

DUOPLOT_WEAK duoplot_MatrixF duoplot_createMatrixF(const size_t num_rows, const size_t num_cols)
{
    duoplot_Matrix mat = duoplot_createMatrix(num_rows, num_cols, DUOPLOT_INTERNAL_DT_FLOAT);
    duoplot_MatrixF ret_mat = *(duoplot_MatrixF*)(&mat);
    return ret_mat;
}

DUOPLOT_WEAK duoplot_MatrixD duoplot_createMatrixD(const size_t num_rows, const size_t num_cols)
{
    duoplot_Matrix mat = duoplot_createMatrix(num_rows, num_cols, DUOPLOT_INTERNAL_DT_DOUBLE);
    duoplot_MatrixD ret_mat = *(duoplot_MatrixD*)(&mat);
    return ret_mat;
}

DUOPLOT_WEAK duoplot_MatrixS8 duoplot_createMatrixS8(const size_t num_rows, const size_t num_cols)
{
    duoplot_Matrix mat = duoplot_createMatrix(num_rows, num_cols, DUOPLOT_INTERNAL_DT_INT8);
    duoplot_MatrixS8 ret_mat = *(duoplot_MatrixS8*)(&mat);
    return ret_mat;
}

DUOPLOT_WEAK duoplot_MatrixS16 duoplot_createMatrixS16(const size_t num_rows, const size_t num_cols)
{
    duoplot_Matrix mat = duoplot_createMatrix(num_rows, num_cols, DUOPLOT_INTERNAL_DT_INT16);
    duoplot_MatrixS16 ret_mat = *(duoplot_MatrixS16*)(&mat);
    return ret_mat;
}

DUOPLOT_WEAK duoplot_MatrixS32 duoplot_createMatrixS32(const size_t num_rows, const size_t num_cols)
{
    duoplot_Matrix mat = duoplot_createMatrix(num_rows, num_cols, DUOPLOT_INTERNAL_DT_INT32);
    duoplot_MatrixS32 ret_mat = *(duoplot_MatrixS32*)(&mat);
    return ret_mat;
}

DUOPLOT_WEAK duoplot_MatrixS64 duoplot_createMatrixS64(const size_t num_rows, const size_t num_cols)
{
    duoplot_Matrix mat = duoplot_createMatrix(num_rows, num_cols, DUOPLOT_INTERNAL_DT_INT64);
    duoplot_MatrixS64 ret_mat = *(duoplot_MatrixS64*)(&mat);
    return ret_mat;
}

DUOPLOT_WEAK duoplot_MatrixU8 duoplot_createMatrixU8(const size_t num_rows, const size_t num_cols)
{
    duoplot_Matrix mat = duoplot_createMatrix(num_rows, num_cols, DUOPLOT_INTERNAL_DT_UINT8);
    duoplot_MatrixU8 ret_mat = *(duoplot_MatrixU8*)(&mat);
    return ret_mat;
}

DUOPLOT_WEAK duoplot_MatrixU16 duoplot_createMatrixU16(const size_t num_rows, const size_t num_cols)
{
    duoplot_Matrix mat = duoplot_createMatrix(num_rows, num_cols, DUOPLOT_INTERNAL_DT_UINT16);
    duoplot_MatrixU16 ret_mat = *(duoplot_MatrixU16*)(&mat);
    return ret_mat;
}

DUOPLOT_WEAK duoplot_MatrixU32 duoplot_createMatrixU32(const size_t num_rows, const size_t num_cols)
{
    duoplot_Matrix mat = duoplot_createMatrix(num_rows, num_cols, DUOPLOT_INTERNAL_DT_UINT32);
    duoplot_MatrixU32 ret_mat = *(duoplot_MatrixU32*)(&mat);
    return ret_mat;
}

DUOPLOT_WEAK duoplot_MatrixU64 duoplot_createMatrixU64(const size_t num_rows, const size_t num_cols)
{
    duoplot_Matrix mat = duoplot_createMatrix(num_rows, num_cols, DUOPLOT_INTERNAL_DT_UINT64);
    duoplot_MatrixU64 ret_mat = *(duoplot_MatrixU64*)(&mat);
    return ret_mat;
}

DUOPLOT_WEAK duoplot_MatrixF duoplot_toMatrixF(const duoplot_Matrix mat)
{
    duoplot_MatrixF ret_mat = *(duoplot_MatrixF*)(&mat);
    return ret_mat;
}

DUOPLOT_WEAK duoplot_MatrixD duoplot_toMatrixD(const duoplot_Matrix mat)
{
    duoplot_MatrixD ret_mat = *(duoplot_MatrixD*)(&mat);
    return ret_mat;
}

DUOPLOT_WEAK duoplot_MatrixS8 duoplot_toMatrixS8(const duoplot_Matrix mat)
{
    duoplot_MatrixS8 ret_mat = *(duoplot_MatrixS8*)(&mat);
    return ret_mat;
}

DUOPLOT_WEAK duoplot_MatrixS16 duoplot_toMatrixS16(const duoplot_Matrix mat)
{
    duoplot_MatrixS16 ret_mat = *(duoplot_MatrixS16*)(&mat);
    return ret_mat;
}

DUOPLOT_WEAK duoplot_MatrixS32 duoplot_toMatrixS32(const duoplot_Matrix mat)
{
    duoplot_MatrixS32 ret_mat = *(duoplot_MatrixS32*)(&mat);
    return ret_mat;
}

DUOPLOT_WEAK duoplot_MatrixS64 duoplot_toMatrixS64(const duoplot_Matrix mat)
{
    duoplot_MatrixS64 ret_mat = *(duoplot_MatrixS64*)(&mat);
    return ret_mat;
}

DUOPLOT_WEAK duoplot_MatrixU8 duoplot_toMatrixU8(const duoplot_Matrix mat)
{
    duoplot_MatrixU8 ret_mat = *(duoplot_MatrixU8*)(&mat);
    return ret_mat;
}

DUOPLOT_WEAK duoplot_MatrixU16 duoplot_toMatrixU16(const duoplot_Matrix mat)
{
    duoplot_MatrixU16 ret_mat = *(duoplot_MatrixU16*)(&mat);
    return ret_mat;
}

DUOPLOT_WEAK duoplot_MatrixU32 duoplot_toMatrixU32(const duoplot_Matrix mat)
{
    duoplot_MatrixU32 ret_mat = *(duoplot_MatrixU32*)(&mat);
    return ret_mat;
}

DUOPLOT_WEAK duoplot_MatrixU64 duoplot_toMatrixU64(const duoplot_Matrix mat)
{
    duoplot_MatrixU64 ret_mat = *(duoplot_MatrixU64*)(&mat);
    return ret_mat;
}

#endif  // DUOPLOT_MATH_MATRIX_MATRIX_INSTANTIATION_H_
