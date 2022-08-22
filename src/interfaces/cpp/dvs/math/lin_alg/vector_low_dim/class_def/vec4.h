#ifndef DVS_MATH_LIN_ALG_VECTOR_LOW_DIM_CLASS_DEFS_VEC4D_CLASS_DEF_H_
#define DVS_MATH_LIN_ALG_VECTOR_LOW_DIM_CLASS_DEFS_VEC4D_CLASS_DEF_H_

#include "dvs/math/misc/forward_decl.h"

namespace dvs
{
template <typename T> struct Vec4
{
    T x;
    T y;
    T z;
    T w;

    Vec4(const T x_, const T y_, const T z_, const T w_);
    Vec4();
    Vec4(const T v_array[4]);
    template <typename Y> Vec4(const Vec4<Y>& v);

    Vector<T> toVector() const;
    Vec4<T> normalized() const;
    Vec4<T> vectorBetweenPoints(const Point4<T>& end_point) const;
    Vec4<T> normalizedVectorBetweenPoints(const Point4<T>& end_point) const;
    T squaredNorm() const;
    T norm() const;
    Vec4<T> elementWiseMultiply(const Vec4<T>& factor_vector) const;
    Vec4<T> elementWiseDivide(const Vec4<T>& numerator_vector) const;
    Matrix<T> outerProduct(const Vec4<T>& v) const;
    Point4<T> mirrorPointInThis(const Point4<T>& point_to_mirror) const;
    T angleBetweenVectors(const Vec4<T>& v) const;

    bool areAllNan() const;
    bool isAnyNan() const;
};
}  // namespace dvs

#endif // DVS_MATH_LIN_ALG_VECTOR_LOW_DIM_CLASS_DEFS_VEC4D_CLASS_DEF_H_
