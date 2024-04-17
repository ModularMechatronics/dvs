#ifndef DUOPLOT_MATH_LIN_ALG_MATRIX_VECTOR_FIXED_H_
#define DUOPLOT_MATH_LIN_ALG_MATRIX_VECTOR_FIXED_H_

#include <cmath>

#include "duoplot/math/lin_alg/matrix_fixed/matrix_fixed.h"
#include "duoplot/math/lin_alg/vector_low_dim/vec2.h"
#include "duoplot/math/lin_alg/vector_low_dim/vec3.h"
#include "duoplot/math/lin_alg/vector_low_dim/vec4.h"
#include "duoplot/math/misc/math_macros.h"

namespace duoplot
{

template <typename T> MatrixFixed<T, 3, 3> diagMatrixFixed(const Vec3<T>& v)
{
    MatrixFixed<T, 3, 3> res = unitMatrixFixed<T, 3, 3>();

    res(0, 0) = v.x;
    res(1, 1) = v.y;
    res(2, 2) = v.z;

    return res;
}

}  // namespace duoplot

#endif  // DUOPLOT_MATH_LIN_ALG_MATRIX_VECTOR_FIXED_H_
