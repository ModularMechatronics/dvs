#include <gtest/gtest.h>

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "core/serializer.h"
#include "core/types.h"

template <typename T>
class Matrix
{
private:
    T* data_;

public:
    Matrix() = default;
};

template <typename T>
class Vector
{
private:
    T* data_;

public:
    Vector() = default;
};

template <typename T>
class Vec3
{
private:
    T* data_;

public:
    Vec3() = default;
};

F32 serializeMe(const U32 u, const F64Ptr data, const Matrix<F32> mat, const Vector<S16> vec, const Vec3<F64> v3)
{

}

/*F32 serializedFunction(const U32 u, const F64Ptr data, const Matrix<F32> mat, const Vector<S16> vec, const Vec3<F64> v3)
{
    data_logging::logFunctionData(__FILE__, __FUNCTION__, u, mat, vec, v3);
}

F32 functionCallFromDeserializedData(const U32 _u, const F64Ptr _data, const Matrix<F32> _mat, const Vector<S16> _vec, const Vec3<F64> _v3)
{
    data_logging::readFromLogData(__FILE__, __FUNCTION__, u, mat, vec, v3);
}
*/

TEST(TestInstrumentation, TestBasic)
{
    

}

