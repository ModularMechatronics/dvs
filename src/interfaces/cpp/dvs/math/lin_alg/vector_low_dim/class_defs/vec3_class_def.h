#ifndef DVS_MATH_LIN_ALG_VECTOR_LOW_DIM_CLASS_DEFS_VEC3D_CLASS_DEF_H_
#define DVS_MATH_LIN_ALG_VECTOR_LOW_DIM_CLASS_DEFS_VEC3D_CLASS_DEF_H_

#include "dvs/math/misc/forward_decl.h"

namespace dvs
{
template <typename T> struct Vec3
{
    T x;
    T y;
    T z;

    Vec3(const T x_, const T y_, const T z_);
    template <typename Y> Vec3(const Vec3<Y>& v);
    Vec3();
    Vec3(const T v_array[3]);
    Vector<T> toVector() const;

    CylindricalCoord<T> toCylindricalVec() const;
    SphericalCoord<T> toSphericalCoord() const;

    Vec3<T> normalized() const;
    Vec3<T> vectorBetweenPoints(const Point3<T>& end_point) const;
    Vec3<T> normalizedVectorBetweenPoints(const Point3<T>& end_point) const;
    T squaredNorm() const;
    T norm() const;
    Vec3<T> elementWiseMultiply(const Vec3<T>& factor_vector) const;
    Vec3<T> elementWiseDivide(const Vec3<T>& numerator_vector) const;
    Vec3<T> crossProduct(const Vec3<T>& right_vector) const;
    Matrix<T> toCrossProductMatrix() const;
    Matrix<T> outerProduct(const Vec3<T>& v) const;
    Point3<T> mirrorPointInThis(const Point3<T>& point_to_mirror) const;
    T angleBetweenVectors(const Vec3<T>& v) const;
    Vec3<T> perpendicularVector() const;

    bool areAllNan() const;
    bool isAnyNan() const;
};

}  // namespace dvs

#endif // DVS_MATH_LIN_ALG_VECTOR_LOW_DIM_CLASS_DEFS_VEC3D_CLASS_DEF_H_
