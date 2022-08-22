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

template <typename T> Matrix<T> Vector<T>::outerProduct(const Vector<T>& v) const
{
    assert(vector_length_ == v.size());
    Matrix<T> mat(vector_length_, vector_length_);

    for (size_t r = 0; r < vector_length_; r++)
    {
        for (size_t c = 0; c < vector_length_; c++)
        {
            mat(r, c) = data_[r] * v(c);
        }
    }

    return mat;
}

template <typename T> Matrix<T> Vector<T>::toColumnVectorMat() const
{
    Matrix<T> mres(vector_length_, 1);

    for (size_t r = 0; r < vector_length_; r++)
    {
        mres(r, 0) = data_[r];
    }

    return mres;
}

template <typename T> Matrix<T> Vector<T>::toRowVectorMat() const
{
    Matrix<T> mres(1, vector_length_);

    for (size_t c = 0; c < vector_length_; c++)
    {
        mres(0, c) = data_[c];
    }

    return mres;
}

// Vec234D

template <typename T> Matrix<T> Vec2<T>::outerProduct(const Vec2<T>& v) const
{
    Vector<T> vv0 = toVector();
    Vector<T> vv1 = v.toVector();

    return vv0.outerProduct(vv1);
}

template <typename T> Matrix<T> Vec3<T>::outerProduct(const Vec3<T>& v) const
{
    Vector<T> vv0 = toVector();
    Vector<T> vv1 = v.toVector();

    return vv0.outerProduct(vv1);
}

template <typename T> Vec4<T> Vector<T>::toVec4() const
{
    DVS_ASSERT(is_allocated_) << "Error, parent vector not allocated!";
    DVS_ASSERT(vector_length_ >= 4) << "Error, parent vector not of size 4 or larger!";
    Vec4<T> v(data_[0], data_[1], data_[2], data_[3]);
    return v;
}

template <typename T> Vec3<T> Vector<T>::toVec3() const
{
    DVS_ASSERT(is_allocated_) << "Error, parent vector not allocated!";
    DVS_ASSERT(vector_length_ >= 3) << "Error, parent vector not of size 3 or larger!";
    Vec3<T> v(data_[0], data_[1], data_[2]);
    return v;
}

template <typename T> Vec2<T> Vector<T>::toVec2() const
{
    DVS_ASSERT(is_allocated_) << "Error, parent vector not allocated!";
    DVS_ASSERT(vector_length_ >= 2) << "Error, parent vector not of size 2 or larger!";
    Vec2<T> v(data_[0], data_[1]);
    return v;
}

template <typename T> Matrix<T> Vec4<T>::outerProduct(const Vec4<T>& v) const
{
    Vector<T> vv0 = toVector();
    Vector<T> vv1 = v.toVector();

    return vv0.outerProduct(vv1);
}

template <typename T> Vector<T> Vec2<T>::toVector() const
{
    const Vector<T> v({x, y});
    return v;
}

template <typename T> Vector<T> Vec3<T>::toVector() const
{
    const Vector<T> v({x, y, z});
    return v;
}

template <typename T> Vector<T> Vec4<T>::toVector() const
{
    const Vector<T> v({x, y, z, w});
    return v;
}

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

template <typename T> Vec3<T> crossProductMatrixToVec3(const Matrix<T>& m)
{
    return Vec3<T>(m(2, 1), m(0, 2), m(1, 0));
}

template <typename T>
AxisAngle<T> estimateRotationFromTwoVectors(const Vec3<T>& v_unrotated, const Vec3<T>& v_rotated)
{
    Vec3<T> rotation_vector = v_unrotated.crossProduct(v_rotated);
    T phi = v_unrotated.angleBetweenVectors(v_rotated);

    return AxisAngle<T>(phi, rotation_vector.x, rotation_vector.y, rotation_vector.z);
}

}  // namespace dvs

#endif // DVS_MATH_LIN_ALG_MATRIX_VECTOR_DYNAMIC_H_
