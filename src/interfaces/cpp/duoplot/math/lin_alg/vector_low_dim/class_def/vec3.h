#ifndef DUOPLOT_MATH_LIN_ALG_VECTOR_LOW_DIM_CLASS_DEF_VEC3_H_
#define DUOPLOT_MATH_LIN_ALG_VECTOR_LOW_DIM_CLASS_DEF_VEC3_H_

#include "duoplot/math/misc/forward_decl.h"

namespace duoplot
{
template <typename T> struct Vec3
{
    T x;
    T y;
    T z;

    Vec3(const T x_, const T y_, const T z_);
    template <typename Y> Vec3(const Vec3<Y>& v);
    Vec3();

    Vec3<T> normalized() const;
    Vec3<T> vectorBetweenPoints(const Point3<T>& end_point) const;
    Vec3<T> normalizedVectorBetweenPoints(const Point3<T>& end_point) const;
    T squaredNorm() const;
    T norm() const;
    Vec3<T> elementWiseMultiply(const Vec3<T>& factor_vector) const;
    Vec3<T> elementWiseDivide(const Vec3<T>& numerator_vector) const;
    Vec3<T> crossProduct(const Vec3<T>& right_vector) const;
    Matrix<T> toCrossProductMatrix() const;
    T angleBetweenVectors(const Vec3<T>& v) const;
};

}  // namespace duoplot

#endif  // DUOPLOT_MATH_LIN_ALG_VECTOR_LOW_DIM_CLASS_DEF_VEC3_H_
