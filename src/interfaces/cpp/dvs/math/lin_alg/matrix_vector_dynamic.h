#ifndef DVS_MATH_LIN_ALG_MATRIX_VECTOR_DYNAMIC_H_
#define DVS_MATH_LIN_ALG_MATRIX_VECTOR_DYNAMIC_H_

#include <cmath>

#include "dvs/math/lin_alg/matrix_dynamic/matrix_dynamic.h"
#include "dvs/math/lin_alg/vector_dynamic/vector_dynamic.h"
#include "dvs/math/lin_alg/vector_low_dim/vec2.h"
#include "dvs/math/lin_alg/vector_low_dim/vec3.h"
#include "dvs/math/lin_alg/vector_low_dim/vec4.h"
#include "dvs/math/misc/math_macros.h"

namespace dvs
{

template <typename T> Matrix<T> Vec3<T>::toCrossProductMatrix() const
{
    Matrix<T> m(3, 3);
    m(0, 0) = 0.0;
    m(0, 1) = -z;
    m(0, 2) = y;

    m(1, 0) = z;
    m(1, 1) = 0.0;
    m(1, 2) = -x;

    m(2, 0) = -y;
    m(2, 1) = x;
    m(2, 2) = 0.0;
    return m;
}

}  // namespace dvs

#endif  // DVS_MATH_LIN_ALG_MATRIX_VECTOR_DYNAMIC_H_
