#ifndef DUOPLOT_MATH_IMAGE_IMAGE_MATH_H_
#define DUOPLOT_MATH_IMAGE_IMAGE_MATH_H_

#include "duoplot/math/image/image_instantiation.h"

#define DUOPLOT_AT_IMAGEC1_ROW_COL(img, row, col) img.data[row * img.num_cols + col]
#define DUOPLOT_AT_IMAGEC3_COLOR_ROW_COL(img, color, row, col) \
    img.data[color * img.num_rows * img.num_cols + row * img.num_cols + col]

#define DUOPLOT_PRINT_IMAGEC1(img, fmt)                                 \
    {                                                                   \
        for (size_t __r = 0; __r < img.num_rows; __r++)                 \
        {                                                               \
            printf("[ ");                                               \
            for (size_t __c = 0; __c < img.num_cols; __c++)             \
            {                                                           \
                printf(fmt, DUOPLOT_AT_IMAGEC1_ROW_COL(img, __r, __c)); \
                if ((__c + 1) != img.num_cols)                          \
                {                                                       \
                    printf(",");                                        \
                }                                                       \
                printf(" ");                                            \
            }                                                           \
            printf("]\n");                                              \
        }                                                               \
    }

#endif  // DUOPLOT_MATH_IMAGE_IMAGE_MATH_H_
