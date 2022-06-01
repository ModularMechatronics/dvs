#ifndef MATRIX_H_
#define MATRIX_H_

#include <stdint.h>
#include <assert.h>
#include <stdlib.h>

typedef enum
{
    DataType_UNDEFINED,
    DataType_DOUBLE,
    DataType_FLOAT,
    DataType_INT8,
    DataType_INT16,
    DataType_INT32,
    DataType_INT64,
    DataType_UINT8,
    DataType_UINT16,
    DataType_UINT32,
    DataType_UINT64
} DataType;

typedef struct
{
    DataType data_type;
    float* const data;
    size_t num_rows;
    size_t num_cols;
} MatrixF;

MatrixF createMatrixF(const size_t num_rows, const size_t num_cols)
{
    const MatrixF mat = {
        .data_type = DataType_FLOAT,
        .data = (float*)malloc(num_rows * num_cols * sizeof(float)),
        .num_rows = num_rows,
        .num_cols = num_cols
    };

    return mat;
}

void matrixFMultiply(const MatrixF m0, const MatrixF m1, const MatrixF mres)
{
    assert(m0.num_cols == m1.num_rows);
    assert(m0.num_rows == mres.num_rows);
    assert(m1.num_cols == mres.num_cols);

    for (size_t r = 0; r < mres.num_rows; r++)
    {
        const size_t m0_row_idx = m0.num_cols * r;

        for (size_t c = 0; c < mres.num_cols; c++)
        {
            float p = 0.0f;
            for (size_t i = 0; i < m0.num_cols; i++)
            {
                p = p + m0.data[m0_row_idx + i] * m1.data[m1.num_cols * i + c];
            }
            mres.data[r * mres.num_cols + c] = p;
        }
    }
}

/*void matrixFAdd(const MatrixF m0, const MatrixF m1, MatrixF mres)
{
    assert(m0.num_rows == m1.num_rows);
    assert(m0.num_cols == m1.num_cols);
    assert(m0.num_rows == mres.num_rows);
    assert(m0.num_cols == mres.num_cols);

    for (size_t r = 0; r < mres.num_rows; r++)
    {
        const size_t m0_row_idx = m0.num_cols * r;

        for (size_t c = 0; c < mres.num_cols; c++)
        {
            mres.data[r * mres.num_cols + c] = m0.data[m0_row_idx + c] + m1.data[m0_row_idx + c];
        }
    }
}

void matrixFSubtract(const MatrixF m0, const MatrixF m1, MatrixF mres)
{
    assert(m0.num_rows == m1.num_rows);
    assert(m0.num_cols == m1.num_cols);
    assert(m0.num_rows == mres.num_rows);
    assert(m0.num_cols == mres.num_cols);

    for (size_t r = 0; r < mres.num_rows; r++)
    {
        const size_t m0_row_idx = m0.num_cols * r;

        for (size_t c = 0; c < mres.num_cols; c++)
        {
            mres.data[r * mres.num_cols + c] = m0.data[m0_row_idx + c] - m1.data[m0_row_idx + c];
        }
    }
}

void matrixFElementWiseMultiply(const MatrixF m0, const MatrixF m1, MatrixF mres)
{
    assert(m0.num_rows == m1.num_rows);
    assert(m0.num_cols == m1.num_cols);
    assert(m0.num_rows == mres.num_rows);
    assert(m0.num_cols == mres.num_cols);

    for (size_t r = 0; r < mres.num_rows; r++)
    {
        const size_t m0_row_idx = m0.num_cols * r;

        for (size_t c = 0; c < mres.num_cols; c++)
        {
            mres.data[r * mres.num_cols + c] = m0.data[m0_row_idx + c] * m1.data[m0_row_idx + c];
        }
    }
}

void matrixFElementWiseDivide(const MatrixF m0, const MatrixF m1, MatrixF mres)
{
    assert(m0.num_rows == m1.num_rows);
    assert(m0.num_cols == m1.num_cols);
    assert(m0.num_rows == mres.num_rows);
    assert(m0.num_cols == mres.num_cols);

    for (size_t r = 0; r < mres.num_rows; r++)
    {
        const size_t m0_row_idx = m0.num_cols * r;

        for (size_t c = 0; c < mres.num_cols; c++)
        {
            mres.data[r * mres.num_cols + c] = m0.data[m0_row_idx + c] / m1.data[m0_row_idx + c];
        }
    }
}

void matrixFScalarFAdd(const MatrixF m0, const float f, MatrixF mres)
{
    assert(m0.num_rows == mres.num_rows);
    assert(m0.num_cols == mres.num_cols);

    for (size_t r = 0; r < mres.num_rows; r++)
    {
        const size_t m0_row_idx = m0.num_cols * r;

        for (size_t c = 0; c < mres.num_cols; c++)
        {
            mres.data[r * mres.num_cols + c] = m0.data[m0_row_idx + c] + f;
        }
    }
}

void matrixFScalarFSubtract(const MatrixF m0, const float f, const MatrixF mres)
{
    assert(m0.num_rows == mres.num_rows);
    assert(m0.num_cols == mres.num_cols);

    for (size_t r = 0; r < mres.num_rows; r++)
    {
        const size_t m0_row_idx = m0.num_cols * r;

        for (size_t c = 0; c < mres.num_cols; c++)
        {
            mres.data[r * mres.num_cols + c] = m0.data[m0_row_idx + c] - f;
        }
    }
}

void matrixFNegate(const MatrixF m0)
{
    for (size_t r = 0; r < m0.num_rows; r++)
    {
        const size_t m0_row_idx = m0.num_cols * r;

        for (size_t c = 0; c < m0.num_cols; c++)
        {
            m0.data[m0_row_idx + c] = m0.data[m0_row_idx + c];
        }
    }
}*/

#define freeMatrix(mat) free(mat.data)


#endif
