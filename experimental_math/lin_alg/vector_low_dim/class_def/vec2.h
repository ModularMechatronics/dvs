#ifndef DVS_MATH_LIN_ALG_VECTOR_LOW_DIM_CLASS_DEF_VEC2_H_
#define DVS_MATH_LIN_ALG_VECTOR_LOW_DIM_CLASS_DEF_VEC2_H_

#include "dvs/math/misc/forward_decl.h"

namespace dvs
{
template <typename T> struct Vec2
{
    T x;
    T y;

    Vec2(const T x_, const T y_);
    Vec2();
    Vec2(const T v_array[2]);
    template <typename Y> Vec2(const Vec2<Y>& v);
    Vector<T> toVector() const;

    ComplexCoord<T> toComplex() const;
    PolarCoord<T> toPolar() const;

    Vec2<T> normalized() const;
    Vec2<T> vectorBetweenPoints(const Point2<T>& end_point) const;
    Vec2<T> normalizedVectorBetweenPoints(const Point2<T>& end_point) const;
    T squaredNorm() const;
    T norm() const;
    Vec2<T> elementWiseMultiply(const Vec2<T>& factor_vector) const;
    Vec2<T> elementWiseDivide(const Vec2<T>& numerator_vector) const;
    Matrix<T> outerProduct(const Vec2<T>& v) const;
    Point2<T> rotatePointAroundThis(const Point2<T>& point_to_rotate, const T angle) const;
    Point2<T> mirrorPointInThis(const Point2<T>& point_to_mirror) const;
    T angleBetweenVectors(const Vec2<T>& v) const;
    T angle() const;

    bool areAllNan() const;
    bool isAnyNan() const;
};

template <typename T> struct VecXY
{
    T x;
    T y;
    VecXY() = default;
    VecXY(const T x_, const T y_) : x(x_), y(y_) {}
    template <typename Y> VecXY(const VecXY<Y>& v) : x(v.x), y(v.y) {}
};

template <typename T> struct VecXZ
{
    T x;
    T z;
    VecXZ() = default;
    VecXZ(const T x_, const T z_) : x(x_), z(z_) {}
    template <typename Y> VecXZ(const VecXZ<Y>& v) : x(v.x), z(v.z) {}
};

template <typename T> struct VecYZ
{
    T y;
    T z;
    VecYZ() = default;
    VecYZ(const T y_, const T z_) : y(y_), z(z_) {}
    template <typename Y> VecYZ(const VecYZ<Y>& v) : y(v.y), z(v.z) {}
};

}  // namespace dvs

#endif // DVS_MATH_LIN_ALG_VECTOR_LOW_DIM_CLASS_DEF_VEC2_H_
