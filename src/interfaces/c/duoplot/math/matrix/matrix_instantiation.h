#ifndef DUOPLOT_MATH_MATRIX_MATRIX_INSTANTIATION_H_
#define DUOPLOT_MATH_MATRIX_MATRIX_INSTANTIATION_H_

#include "duoplot/math/matrix/matrix_types.h"
#include "duoplot/math/utils.h"
#include "duoplot/pp.h"

DUOPLOT_WEAK Matrix createMatrix(const size_t num_rows, const size_t num_cols, const DataType data_type)
{
    Matrix mat;
    mat.data_type = data_type;
    mat.num_bytes_per_element = duoplot_internal_dataTypeToNumBytes(data_type);
    mat.num_rows = num_rows;
    mat.num_cols = num_cols;
    mat.data = (uint8_t*)malloc(num_rows * num_cols * mat.num_bytes_per_element);

    return mat;
}

#define freeMatrix(mat) free(mat.data)

DUOPLOT_WEAK MatrixF createMatrixF(const size_t num_rows, const size_t num_cols)
{
    Matrix mat = createMatrix(num_rows, num_cols, DUOPLOT_INTERNAL_DT_FLOAT);
    MatrixF ret_mat = *(MatrixF*)(&mat);
    return ret_mat;
}

DUOPLOT_WEAK MatrixD createMatrixD(const size_t num_rows, const size_t num_cols)
{
    Matrix mat = createMatrix(num_rows, num_cols, DUOPLOT_INTERNAL_DT_DOUBLE);
    MatrixD ret_mat = *(MatrixD*)(&mat);
    return ret_mat;
}

DUOPLOT_WEAK MatrixS8 createMatrixS8(const size_t num_rows, const size_t num_cols)
{
    Matrix mat = createMatrix(num_rows, num_cols, DUOPLOT_INTERNAL_DT_INT8);
    MatrixS8 ret_mat = *(MatrixS8*)(&mat);
    return ret_mat;
}

DUOPLOT_WEAK MatrixS16 createMatrixS16(const size_t num_rows, const size_t num_cols)
{
    Matrix mat = createMatrix(num_rows, num_cols, DUOPLOT_INTERNAL_DT_INT16);
    MatrixS16 ret_mat = *(MatrixS16*)(&mat);
    return ret_mat;
}

DUOPLOT_WEAK MatrixS32 createMatrixS32(const size_t num_rows, const size_t num_cols)
{
    Matrix mat = createMatrix(num_rows, num_cols, DUOPLOT_INTERNAL_DT_INT32);
    MatrixS32 ret_mat = *(MatrixS32*)(&mat);
    return ret_mat;
}

DUOPLOT_WEAK MatrixS64 createMatrixS64(const size_t num_rows, const size_t num_cols)
{
    Matrix mat = createMatrix(num_rows, num_cols, DUOPLOT_INTERNAL_DT_INT64);
    MatrixS64 ret_mat = *(MatrixS64*)(&mat);
    return ret_mat;
}

DUOPLOT_WEAK MatrixU8 createMatrixU8(const size_t num_rows, const size_t num_cols)
{
    Matrix mat = createMatrix(num_rows, num_cols, DUOPLOT_INTERNAL_DT_UINT8);
    MatrixU8 ret_mat = *(MatrixU8*)(&mat);
    return ret_mat;
}

DUOPLOT_WEAK MatrixU16 createMatrixU16(const size_t num_rows, const size_t num_cols)
{
    Matrix mat = createMatrix(num_rows, num_cols, DUOPLOT_INTERNAL_DT_UINT16);
    MatrixU16 ret_mat = *(MatrixU16*)(&mat);
    return ret_mat;
}

DUOPLOT_WEAK MatrixU32 createMatrixU32(const size_t num_rows, const size_t num_cols)
{
    Matrix mat = createMatrix(num_rows, num_cols, DUOPLOT_INTERNAL_DT_UINT32);
    MatrixU32 ret_mat = *(MatrixU32*)(&mat);
    return ret_mat;
}

DUOPLOT_WEAK MatrixU64 createMatrixU64(const size_t num_rows, const size_t num_cols)
{
    Matrix mat = createMatrix(num_rows, num_cols, DUOPLOT_INTERNAL_DT_UINT64);
    MatrixU64 ret_mat = *(MatrixU64*)(&mat);
    return ret_mat;
}

DUOPLOT_WEAK MatrixF toMatrixF(const Matrix mat)
{
    MatrixF ret_mat = *(MatrixF*)(&mat);
    return ret_mat;
}

DUOPLOT_WEAK MatrixD toMatrixD(const Matrix mat)
{
    MatrixD ret_mat = *(MatrixD*)(&mat);
    return ret_mat;
}

DUOPLOT_WEAK MatrixS8 toMatrixS8(const Matrix mat)
{
    MatrixS8 ret_mat = *(MatrixS8*)(&mat);
    return ret_mat;
}

DUOPLOT_WEAK MatrixS16 toMatrixS16(const Matrix mat)
{
    MatrixS16 ret_mat = *(MatrixS16*)(&mat);
    return ret_mat;
}

DUOPLOT_WEAK MatrixS32 toMatrixS32(const Matrix mat)
{
    MatrixS32 ret_mat = *(MatrixS32*)(&mat);
    return ret_mat;
}

DUOPLOT_WEAK MatrixS64 toMatrixS64(const Matrix mat)
{
    MatrixS64 ret_mat = *(MatrixS64*)(&mat);
    return ret_mat;
}

DUOPLOT_WEAK MatrixU8 toMatrixU8(const Matrix mat)
{
    MatrixU8 ret_mat = *(MatrixU8*)(&mat);
    return ret_mat;
}

DUOPLOT_WEAK MatrixU16 toMatrixU16(const Matrix mat)
{
    MatrixU16 ret_mat = *(MatrixU16*)(&mat);
    return ret_mat;
}

DUOPLOT_WEAK MatrixU32 toMatrixU32(const Matrix mat)
{
    MatrixU32 ret_mat = *(MatrixU32*)(&mat);
    return ret_mat;
}

DUOPLOT_WEAK MatrixU64 toMatrixU64(const Matrix mat)
{
    MatrixU64 ret_mat = *(MatrixU64*)(&mat);
    return ret_mat;
}

#endif  // DUOPLOT_MATH_MATRIX_MATRIX_INSTANTIATION_H_
