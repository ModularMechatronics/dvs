#ifndef DVS_MATH_LIN_ALG_MATRIX_DYNAMIC_MATRIX_DYNAMIC_H_
#define DVS_MATH_LIN_ALG_MATRIX_DYNAMIC_MATRIX_DYNAMIC_H_

#include <cmath>
#include <cstring>

#include "dvs/math/lin_alg/matrix_dynamic/class_def/matrix_dynamic.h"
#include "dvs/math/misc/math_macros.h"

namespace dvs
{
template <typename T> Matrix<T>& Matrix<T>::operator=(const Matrix<T>& m)
{
    DVS_ASSERT(m.isAllocated()) << "Input matrix not allocated before assignment!";
    if (this != &m)
    {
        if (is_allocated_)
        {
            if ((m.rows() != num_rows_) || (m.cols() != num_cols_))
            {
                delete[] data_;
                DATA_ALLOCATION(data_, m.rows() * m.cols(), T, "Matrix");
            }
        }
        else
        {
            DATA_ALLOCATION(data_, m.rows() * m.cols(), T, "Matrix");
        }

        num_rows_ = m.rows();
        num_cols_ = m.cols();

        for (size_t r = 0; r < m.rows(); r++)
        {
            for (size_t c = 0; c < m.cols(); c++)
            {
                data_[r * num_cols_ + c] = m(r, c);
            }
        }

        is_allocated_ = true;
    }
    return *this;
}

template <typename T> Matrix<T>::Matrix(Matrix<T>&& m)
{
    DVS_ASSERT(m.isAllocated()) << "Input matrix not allocated!";
    data_ = m.data_;
    num_rows_ = m.num_rows_;
    num_cols_ = m.num_cols_;

    is_allocated_ = true;

    m.data_ = nullptr;
    m.num_rows_ = 0;
    m.num_cols_ = 0;
}

template <typename T> template <typename Y> Matrix<T>::Matrix(const Matrix<Y>& m) : is_allocated_(true)
{
    DVS_ASSERT(m.isAllocated()) << "Input matrix not allocated!";
    num_rows_ = m.rows();
    num_cols_ = m.cols();

    DATA_ALLOCATION(data_, m.rows() * m.cols(), T, "Matrix");
    for (size_t r = 0; r < m.rows(); r++)
    {
        for (size_t c = 0; c < m.cols(); c++)
        {
            data_[r * m.cols() + c] = m(r, c);
        }
    }
}

template <typename T> void Matrix<T>::fillBufferWithData(uint8_t* const buffer) const
{
    const uint8_t* const internal_ptr = reinterpret_cast<uint8_t*>(data_);
    const size_t num_bytes = num_rows_ * num_cols_ * sizeof(T);
    std::memcpy(buffer, internal_ptr, num_bytes);
}

template <typename T> Matrix<T>& Matrix<T>::operator=(Matrix<T>&& m)
{
    if (this != &m)
    {
        DVS_ASSERT(m.isAllocated()) << "Input matrix not allocated before assignment!";

        if (is_allocated_)
        {
            delete[] data_;
        }

        num_rows_ = m.rows();
        num_cols_ = m.cols();
        is_allocated_ = true;

        data_ = m.data();

        m.setInternalData(nullptr, 0, 0);
    }

    return *this;
}

template <typename T> Matrix<T>::Matrix() : data_(nullptr), num_rows_(0), num_cols_(0), is_allocated_(false) {}

template <typename T> Matrix<T>::Matrix(const size_t num_rows, const size_t num_cols) : is_allocated_(true)
{
    num_rows_ = num_rows;
    num_cols_ = num_cols;

    DATA_ALLOCATION(data_, num_rows_ * num_cols_, T, "Matrix");
}

template <typename T> Matrix<T>::Matrix(const Matrix<T>& m) : is_allocated_(true)
{
    num_rows_ = m.rows();
    num_cols_ = m.cols();

    DATA_ALLOCATION(data_, m.rows() * m.cols(), T, "Matrix");
    for (size_t r = 0; r < m.rows(); r++)
    {
        for (size_t c = 0; c < m.cols(); c++)
        {
            data_[r * m.cols() + c] = m(r, c);
        }
    }
}

template <typename T> Matrix<T>::Matrix(const std::initializer_list<std::initializer_list<T>>& il)
{
    DVS_ASSERT(il.size() > 0) << "Tried to initialize with empty vector matrix!";
    DVS_ASSERT(il.begin()[0].size() > 0) << "Tried to initialize with empty vector matrix!";

    for (size_t r = 0; r < il.size(); r++)
    {
        DVS_ASSERT(il.begin()[0].size() == il.begin()[r].size())
            << "All row vectors in input std vectors do not have the same size!";
    }

    num_rows_ = il.size();
    num_cols_ = il.begin()[0].size();

    DATA_ALLOCATION(data_, num_cols_ * num_rows_, T, "Matrix");
    is_allocated_ = true;

    for (size_t r = 0; r < il.size(); r++)
    {
        for (size_t c = 0; c < il.begin()[r].size(); c++)
        {
            data_[r * num_cols_ + c] = il.begin()[r].begin()[c];
        }
    }
}

template <typename T> Matrix<T>::~Matrix()
{
    if (is_allocated_)
    {
        delete[] data_;
        is_allocated_ = false;
    }
}

template <typename T> size_t Matrix<T>::size() const
{
    return num_rows_ * num_cols_;
}

template <typename T> void Matrix<T>::resize(const size_t num_rows, const size_t num_cols)
{
    if (is_allocated_)
    {
        delete[] data_;
        is_allocated_ = false;
    }

    num_rows_ = num_rows;
    num_cols_ = num_cols;

    DATA_ALLOCATION(data_, num_rows_ * num_cols_, T, "Matrix");
    is_allocated_ = true;
}

template <typename T> void Matrix<T>::setInternalData(T* const input_ptr, const size_t num_rows, const size_t num_cols)
{
    if (input_ptr == nullptr)
    {
        is_allocated_ = false;
    }
    else
    {
        is_allocated_ = true;
    }

    data_ = input_ptr;
    num_rows_ = num_rows;
    num_cols_ = num_cols;
}

template <typename T> T* Matrix<T>::data() const
{
    return data_;
}

template <typename T> size_t Matrix<T>::lastRowIdx() const
{
    assert(num_rows_ != 0);
    return num_rows_ - 1;
}

template <typename T> size_t Matrix<T>::lastColIdx() const
{
    assert(num_cols_ != 0);
    return num_cols_ - 1;
}

template <typename T> size_t Matrix<T>::rows() const
{
    return num_rows_;
}

template <typename T> size_t Matrix<T>::cols() const
{
    return num_cols_;
}

template <typename T> size_t Matrix<T>::numElements() const
{
    // Returns totalt number of elements in matrix
    return num_rows_ * num_cols_;
}

template <typename T> size_t Matrix<T>::numBytes() const
{
    return num_rows_ * num_cols_ * sizeof(T);
}

template <typename T> bool Matrix<T>::isAllocated() const
{
    return is_allocated_;
}

template <typename T> void Matrix<T>::fill(T val)
{
    assert(is_allocated_ && "Tried to fill unallocated vector!");
    for (size_t k = 0; k < num_rows_ * num_cols_; k++)
    {
        data_[k] = val;
    }
}

template <typename T> T& Matrix<T>::operator()(const size_t r, const size_t c)
{
    assert(r < num_rows_ && "Row index is larger than num_rows_-1!");
    assert(c < num_cols_ && "Column index is larger than num_cols_-1!");

    return data_[r * num_cols_ + c];
}

template <typename T> const T& Matrix<T>::operator()(const size_t r, const size_t c) const
{
    assert(r < num_rows_ && "Row index is larger than num_rows_-1!");
    assert(c < num_cols_ && "Column index is larger than num_cols_-1!");

    return data_[r * num_cols_ + c];
}

template <typename T> Matrix<T> operator*(const Matrix<T>& m0, const Matrix<T>& m1)
{
    assert(m0.cols() == m1.rows());
    Matrix<T> res(m0.rows(), m1.cols());

    for (size_t r = 0; r < res.rows(); r++)
    {
        for (size_t c = 0; c < res.cols(); c++)
        {
            T p = 0.0f;
            for (size_t i = 0; i < m0.cols(); i++)
            {
                p = p + m0(r, i) * m1(i, c);
            }
            res(r, c) = p;
        }
    }
    return res;
}

template <typename T> Matrix<T> operator+(const Matrix<T>& m0, const Matrix<T>& m1)
{
    assert(m0.cols() == m1.cols());
    assert(m0.rows() == m1.rows());
    Matrix<T> res(m0.rows(), m1.cols());

    for (size_t r = 0; r < res.rows(); r++)
    {
        for (size_t c = 0; c < res.cols(); c++)
        {
            res(r, c) = m0(r, c) + m1(r, c);
        }
    }
    return res;
}

template <typename T> Matrix<T> operator-(const Matrix<T>& m0, const Matrix<T>& m1)
{
    assert(m0.cols() == m1.cols());
    assert(m0.rows() == m1.rows());
    Matrix<T> res(m0.rows(), m1.cols());

    for (size_t r = 0; r < res.rows(); r++)
    {
        for (size_t c = 0; c < res.cols(); c++)
        {
            res(r, c) = m0(r, c) - m1(r, c);
        }
    }
    return res;
}

template <typename T> Matrix<T> operator*(const Matrix<T>& m, const T f)
{
    Matrix<T> res(m.rows(), m.cols());

    for (size_t r = 0; r < res.rows(); r++)
    {
        for (size_t c = 0; c < res.cols(); c++)
        {
            res(r, c) = f * m(r, c);
        }
    }
    return res;
}

template <typename T> Matrix<T> operator*(const T f, const Matrix<T>& m)
{
    Matrix<T> res(m.rows(), m.cols());

    for (size_t r = 0; r < res.rows(); r++)
    {
        for (size_t c = 0; c < res.cols(); c++)
        {
            res(r, c) = f * m(r, c);
        }
    }
    return res;
}

template <typename T> Matrix<T> operator^(const Matrix<T>& m0, const Matrix<T>& m1)
{
    DVS_ASSERT(m0.rows() == m1.rows());
    DVS_ASSERT(m0.cols() == m1.cols());

    Matrix<T> res(m0.rows(), m0.cols());

    for (size_t r = 0; r < res.rows(); r++)
    {
        for (size_t c = 0; c < res.cols(); c++)
        {
            res(r, c) = m0(r, c) * m1(r, c);
        }
    }
    return res;
}

template <typename T> Matrix<T> operator/(const Matrix<T>& m0, const Matrix<T>& m1)
{
    DVS_ASSERT(m0.rows() == m1.rows());
    DVS_ASSERT(m0.cols() == m1.cols());

    Matrix<T> res(m0.rows(), m0.cols());

    for (size_t r = 0; r < res.rows(); r++)
    {
        for (size_t c = 0; c < res.cols(); c++)
        {
            res(r, c) = m0(r, c) / m1(r, c);
        }
    }
    return res;
}

template <typename T> Matrix<T> operator/(const Matrix<T>& m, const T f)
{
    Matrix<T> res(m.rows(), m.cols());

    for (size_t r = 0; r < res.rows(); r++)
    {
        for (size_t c = 0; c < res.cols(); c++)
        {
            res(r, c) = m(r, c) / f;
        }
    }
    return res;
}

template <typename T> Matrix<T> operator/(const T f, const Matrix<T>& m)
{
    Matrix<T> res(m.rows(), m.cols());

    for (size_t r = 0; r < res.rows(); r++)
    {
        for (size_t c = 0; c < res.cols(); c++)
        {
            res(r, c) = f / m(r, c);
        }
    }
    return res;
}

template <typename T> Matrix<T> operator-(const Matrix<T>& m, const T f)
{
    Matrix<T> res(m.rows(), m.cols());

    for (size_t r = 0; r < res.rows(); r++)
    {
        for (size_t c = 0; c < res.cols(); c++)
        {
            res(r, c) = m(r, c) - f;
        }
    }
    return res;
}

template <typename T> Matrix<T> operator-(const T f, const Matrix<T>& m)
{
    Matrix<T> res(m.rows(), m.cols());

    for (size_t r = 0; r < res.rows(); r++)
    {
        for (size_t c = 0; c < res.cols(); c++)
        {
            res(r, c) = f - m(r, c);
        }
    }
    return res;
}

template <typename T> Matrix<T> operator+(const Matrix<T>& m, const T f)
{
    Matrix<T> res(m.rows(), m.cols());

    for (size_t r = 0; r < res.rows(); r++)
    {
        for (size_t c = 0; c < res.cols(); c++)
        {
            res(r, c) = m(r, c) + f;
        }
    }
    return res;
}

template <typename T> Matrix<T> operator+(const T f, const Matrix<T>& m)
{
    Matrix<T> res(m.rows(), m.cols());

    for (size_t r = 0; r < res.rows(); r++)
    {
        for (size_t c = 0; c < res.cols(); c++)
        {
            res(r, c) = m(r, c) + f;
        }
    }
    return res;
}

template <typename T> Matrix<T> operator-(const Matrix<T>& m)
{
    Matrix<T> res(m.rows(), m.cols());

    for (size_t r = 0; r < res.rows(); r++)
    {
        for (size_t c = 0; c < res.cols(); c++)
        {
            res(r, c) = -m(r, c);
        }
    }
    return res;
}

template <typename T> Vector<T> operator*(const Matrix<T>& m, const Vector<T>& v)
{
    assert(m.cols() == v.size());
    Vector<T> res(m.rows());

    for (size_t r = 0; r < m.rows(); r++)
    {
        T p = 0.0f;
        for (size_t c = 0; c < m.cols(); c++)
        {
            p = p + m(r, c) * v(c);
        }
        res(r) = p;
    }
    return res;
}

template <typename T> Vector<T> operator*(const Vector<T>& v, const Matrix<T>& m)
{
    assert(m.rows() == v.size());
    Vector<T> res(m.cols());

    for (size_t c = 0; c < m.cols(); c++)
    {
        T p = 0.0f;
        for (size_t r = 0; r < m.rows(); r++)
        {
            p = p + m(r, c) * v(r);
        }
        res(c) = p;
    }
    return res;
}

template <typename T> Matrix<T> Matrix<T>::getTranspose() const
{
    Matrix<T> res(num_cols_, num_rows_);

    for (size_t r = 0; r < res.rows(); r++)
    {
        for (size_t c = 0; c < res.cols(); c++)
        {
            res(r, c) = data_[c * num_cols_ + r];
        }
    }
    return res;
}

template <typename T> Matrix<T> unitMatrix(const size_t rows, const size_t cols)
{
    Matrix<T> unit_matrix(rows, cols);
    for (size_t r = 0; r < rows; r++)
    {
        for (size_t c = 0; c < cols; c++)
        {
            if (r == c)
            {
                unit_matrix(r, c) = 1.0;
            }
            else
            {
                unit_matrix(r, c) = 0.0;
            }
        }
    }
    return unit_matrix;
}

template <typename T> Matrix<T> zerosMatrix(const size_t rows, const size_t cols)
{
    Matrix<T> zero_matrix(rows, cols);
    for (size_t r = 0; r < rows; r++)
    {
        for (size_t c = 0; c < cols; c++)
        {
            zero_matrix(r, c) = 0.0;
        }
    }
    return zero_matrix;
}

template <typename T> Matrix<T> onesMatrix(const size_t rows, const size_t cols)
{
    Matrix<T> ones_matrix(rows, cols);
    for (size_t r = 0; r < rows; r++)
    {
        for (size_t c = 0; c < cols; c++)
        {
            ones_matrix(r, c) = 1.0;
        }
    }
    return ones_matrix;
}

template <typename T> Matrix<T> filledMatrix(const size_t rows, const size_t cols, T val)
{
    Matrix<T> filled_matrix(rows, cols);
    for (size_t r = 0; r < rows; r++)
    {
        for (size_t c = 0; c < cols; c++)
        {
            filled_matrix(r, c) = val;
        }
    }
    return filled_matrix;
}

template <typename T> std::ostream& operator<<(std::ostream& os, const Matrix<T>& m)
{
    std::string s = "";

    for (size_t r = 0; r < m.rows(); r++)
    {
        s = s + "[ ";
        for (size_t c = 0; c < m.cols(); c++)
        {
            s = s + std::to_string(m(r, c));
            if (c != m.cols() - 1)
            {
                s = s + ", ";
            }
        }
        s = s + " ]\n";
    }

    os << s;

    return os;
}

template <typename T> T Matrix<T>::max() const
{
    ASSERT_MAT_VALID_INTERNAL();

    T max_val = data_[0];
    for (size_t c = 1; c < num_cols_; c++)
    {
        max_val = std::max(max_val, data_[c]);
    }

    for (size_t r = 1; r < num_rows_; r++)
    {
        T max_val_internal = data_[r * num_cols_];
        for (size_t c = 1; c < num_cols_; c++)
        {
            max_val_internal = std::max(max_val_internal, data_[r * num_cols_ + c]);
        }
        max_val = std::max(max_val, max_val_internal);
    }

    return max_val;
}

template <typename T> T Matrix<T>::min() const
{
    ASSERT_MAT_VALID_INTERNAL();

    T min_val = data_[0];
    for (size_t c = 1; c < num_cols_; c++)
    {
        min_val = std::min(min_val, data_[c]);
    }

    for (size_t r = 1; r < num_rows_; r++)
    {
        T min_val_internal = data_[r * num_cols_];
        for (size_t c = 1; c < num_cols_; c++)
        {
            min_val_internal = std::min(min_val_internal, data_[r * num_cols_ + c]);
        }
        min_val = std::min(min_val, min_val_internal);
    }

    return min_val;
}

template <typename T> T Matrix<T>::sum() const
{
    T s = 0.0;
    for (size_t r = 0; r < num_rows_; r++)
    {
        for (size_t c = 0; c < num_cols_; c++)
        {
            s = s + data_[r * num_cols_ + c];
        }
    }
    return s;
}

}  // namespace dvs

#endif // DVS_MATH_LIN_ALG_MATRIX_DYNAMIC_MATRIX_DYNAMIC_H_
