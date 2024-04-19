#ifndef DUOPLOT_MATH_MATRIX_MATRIX_MATH_H_
#define DUOPLOT_MATH_MATRIX_MATRIX_MATH_H_

#include <math.h>

#include "duoplot/math/matrix/matrix_instantiation.h"
#include "duoplot/pp.h"

#define AT_MAT_ROW_COL(mat, row, col) mat.data[row * mat.num_cols + col]

#define PRINT_MAT(mat, fmt)                                 \
    {                                                       \
        for (size_t __r = 0; __r < mat.num_rows; __r++)     \
        {                                                   \
            printf("[ ");                                   \
            for (size_t __c = 0; __c < mat.num_cols; __c++) \
            {                                               \
                printf(fmt, AT_MAT_ROW_COL(mat, __r, __c)); \
                if ((__c + 1) != mat.num_cols)              \
                {                                           \
                    printf(",");                            \
                }                                           \
                printf(" ");                                \
            }                                               \
            printf("]\n");                                  \
        }                                                   \
    }

DUOPLOT_WEAK MatrixPairD
meshgridD(const double x0, const double x1, const double y0, const double y1, const size_t xn, const size_t yn)
{
    MatrixD xm = createMatrixD(yn, xn);
    MatrixD ym = createMatrixD(yn, xn);

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
            AT_MAT_ROW_COL(xm, r, c) = xval;
            AT_MAT_ROW_COL(ym, r, c) = yval;
            xval += dx;
        }
        yval += dy;
    }

    const MatrixPairD mp = {xm, ym};

    return mp;
}

DUOPLOT_WEAK MatrixD matrix_sinD(const MatrixD mat)
{
    MatrixD mat_out = createMatrixD(mat.num_rows, mat.num_cols);

    for (size_t r = 0; r < mat.num_rows; r++)
    {
        for (size_t c = 0; c < mat.num_cols; c++)
        {
            AT_MAT_ROW_COL(mat_out, r, c) = sin(AT_MAT_ROW_COL(mat, r, c));
        }
    }

    return mat_out;
}

DUOPLOT_WEAK MatrixD matrix_cosD(const MatrixD mat)
{
    MatrixD mat_out = createMatrixD(mat.num_rows, mat.num_cols);

    for (size_t r = 0; r < mat.num_rows; r++)
    {
        for (size_t c = 0; c < mat.num_cols; c++)
        {
            AT_MAT_ROW_COL(mat_out, r, c) = cos(AT_MAT_ROW_COL(mat, r, c));
        }
    }

    return mat_out;
}

DUOPLOT_WEAK MatrixD matrix_elementWiseMultiplyD(const MatrixD m0, const MatrixD m1)
{
    MatrixD mat_out = createMatrixD(m0.num_rows, m0.num_cols);

    for (size_t r = 0; r < m0.num_rows; r++)
    {
        for (size_t c = 0; c < m0.num_cols; c++)
        {
            AT_MAT_ROW_COL(mat_out, r, c) = AT_MAT_ROW_COL(m0, r, c) * AT_MAT_ROW_COL(m1, r, c);
        }
    }

    return mat_out;
}

#endif  // DUOPLOT_MATH_MATRIX_MATRIX_MATH_H_
