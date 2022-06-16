#ifndef DVS_MATH_IMAGE_IMAGE_MATH_H_
#define DVS_MATH_IMAGE_IMAGE_MATH_H_

#include "dvs/math/image/image_instantiation.h"

#define AT_IMAGEC1_ROW_COL(img, row, col) img.data[row * img.num_cols + col]
#define AT_IMAGEC3_COL_ROW_COL(img, color, row, col) img.data[color * img.num_rows * img.num_cols + row * img.num_cols + col]

#define PRINT_IMAGEC1(img, fmt)                             \
    {                                                       \
        for (size_t __r = 0; __r < img.num_rows; __r++)     \
        {                                                   \
            printf("[ ");                                   \
            for (size_t __c = 0; __c < img.num_cols; __c++) \
            {                                               \
                printf(fmt, AT_MAT_ROW_COL(img, __r, __c)); \
                if ((__c + 1) != img.num_cols)              \
                {                                           \
                    printf(",");                            \
                }                                           \
                printf(" ");                                \
            }                                               \
            printf("]\n");                                  \
        }                                                   \
    }


#endif // DVS_MATH_IMAGE_IMAGE_MATH_H_
