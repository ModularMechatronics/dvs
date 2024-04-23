#ifndef DUOPLOT_MATH_MATRIX_MATRIX_MATH_H_
#define DUOPLOT_MATH_MATRIX_MATRIX_MATH_H_

#include <math.h>

#include "duoplot/math/matrix/matrix_instantiation.h"
#include "duoplot/pp.h"

#define DUOPLOT_AT_MAT_ROW_COL(mat, row, col) mat.data[row * mat.num_cols + col]

#define DUOPLOT_PRINT_MAT(mat, fmt)                                 \
    {                                                               \
        for (size_t __r = 0; __r < mat.num_rows; __r++)             \
        {                                                           \
            printf("[ ");                                           \
            for (size_t __c = 0; __c < mat.num_cols; __c++)         \
            {                                                       \
                printf(fmt, DUOPLOT_AT_MAT_ROW_COL(mat, __r, __c)); \
                if ((__c + 1) != mat.num_cols)                      \
                {                                                   \
                    printf(",");                                    \
                }                                                   \
                printf(" ");                                        \
            }                                                       \
            printf("]\n");                                          \
        }                                                           \
    }

DUOPLOT_WEAK duoplot_MatrixPairD
duoplot_meshgridD(const double x0, const double x1, const double y0, const double y1, const size_t xn, const size_t yn)
{
    duoplot_MatrixD xm = duoplot_createMatrixD(yn, xn);
    duoplot_MatrixD ym = duoplot_createMatrixD(yn, xn);

    const size_t num_rows = yn;
    const size_t num_cols = xn;

    double xval = x0;
    double yval = y0;
    const double dx = (x1 - x0) / ((double)(xn - 1));
    const double dy = (y1 - y0) / ((double)(yn - 1));

    for (size_t r = 0; r < num_rows; r++)
    {
        xval = x0;
        for (size_t c = 0; c < num_cols; c++)
        {
            DUOPLOT_AT_MAT_ROW_COL(xm, r, c) = xval;
            DUOPLOT_AT_MAT_ROW_COL(ym, r, c) = yval;
            xval += dx;
        }
        yval += dy;
    }

    const duoplot_MatrixPairD mp = {xm, ym};

    return mp;
}

DUOPLOT_WEAK duoplot_MatrixD duoplot_matrix_sinD(const duoplot_MatrixD mat)
{
    duoplot_MatrixD mat_out = duoplot_createMatrixD(mat.num_rows, mat.num_cols);

    for (size_t r = 0; r < mat.num_rows; r++)
    {
        for (size_t c = 0; c < mat.num_cols; c++)
        {
            DUOPLOT_AT_MAT_ROW_COL(mat_out, r, c) = sin(DUOPLOT_AT_MAT_ROW_COL(mat, r, c));
        }
    }

    return mat_out;
}

DUOPLOT_WEAK duoplot_MatrixD duoplot_matrix_cosD(const duoplot_MatrixD mat)
{
    duoplot_MatrixD mat_out = duoplot_createMatrixD(mat.num_rows, mat.num_cols);

    for (size_t r = 0; r < mat.num_rows; r++)
    {
        for (size_t c = 0; c < mat.num_cols; c++)
        {
            DUOPLOT_AT_MAT_ROW_COL(mat_out, r, c) = cos(DUOPLOT_AT_MAT_ROW_COL(mat, r, c));
        }
    }

    return mat_out;
}

DUOPLOT_WEAK duoplot_MatrixD duoplot_matrix_elementWiseMultiplyD(const duoplot_MatrixD m0, const duoplot_MatrixD m1)
{
    duoplot_MatrixD mat_out = duoplot_createMatrixD(m0.num_rows, m0.num_cols);

    for (size_t r = 0; r < m0.num_rows; r++)
    {
        for (size_t c = 0; c < m0.num_cols; c++)
        {
            DUOPLOT_AT_MAT_ROW_COL(mat_out, r, c) = DUOPLOT_AT_MAT_ROW_COL(m0, r, c) * DUOPLOT_AT_MAT_ROW_COL(m1, r, c);
        }
    }

    return mat_out;
}

#endif  // DUOPLOT_MATH_MATRIX_MATRIX_MATH_H_
