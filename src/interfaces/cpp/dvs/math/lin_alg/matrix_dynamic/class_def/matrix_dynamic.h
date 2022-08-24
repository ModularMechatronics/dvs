#ifndef DVS_MATH_LIN_ALG_MATRIX_DYNAMIC_CLASS_DEF_MATRIX_DYNAMIC_H_
#define DVS_MATH_LIN_ALG_MATRIX_DYNAMIC_CLASS_DEF_MATRIX_DYNAMIC_H_

#include "dvs/math/misc/forward_decl.h"

namespace dvs
{

template <typename T> class MatrixInitializer
{
private:
    T* data_;
    size_t num_rows_;
    size_t num_cols_;
    friend class Matrix<T>;

public:
    MatrixInitializer() = delete;
    MatrixInitializer(const MatrixInitializer<T>& v) = delete;
    MatrixInitializer(MatrixInitializer<T>&& v);
    MatrixInitializer<T>& operator=(const MatrixInitializer<T>& v) = delete;
    MatrixInitializer<T>& operator=(MatrixInitializer<T>&& v) = delete;
    explicit MatrixInitializer(const std::initializer_list<std::initializer_list<T>>& il)
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

        DATA_ALLOCATION(data_, num_cols_ * num_rows_, T, "MatrixInitializer");

        for (size_t r = 0; r < il.size(); r++)
        {
            for (size_t c = 0; c < il.begin()[r].size(); c++)
            {
                data_[r * num_cols_ + c] = il.begin()[r].begin()[c];
            }
        }
    }

    ~MatrixInitializer()
    {
        delete[] data_;
    }
};

template <typename T> class Matrix
{
protected:
    T* data_;
    size_t num_rows_;
    size_t num_cols_;
    bool is_allocated_;

public:
    Matrix();
    Matrix(const size_t num_rows, const size_t num_cols);
    Matrix(const Matrix<T>& m);
    template <typename Y> Matrix(const Matrix<Y>& m);
    // Matrix(const std::initializer_list<std::initializer_list<T>>& il);
    Matrix(MatrixInitializer<T>&& m)
    {
        data_ = m.data_;
        num_rows_ = m.num_rows_;
        num_cols_ = m.num_cols_;
        is_allocated_ = true;

        m.data_ = nullptr;
        m.num_rows_ = 0U;
        m.num_cols_ = 0U;
    }
    Matrix(Matrix<T>&& m);
    ~Matrix();

    void fillBufferWithData(uint8_t* const buffer) const;

    T& operator()(const size_t r, const size_t c);
    const T& operator()(const size_t r, const size_t c) const;

    Matrix<T>& operator=(const Matrix<T>& m);
    Matrix<T>& operator=(Matrix<T>&& m);

    void resize(const size_t num_rows, const size_t num_cols);
    size_t rows() const;
    size_t cols() const;
    size_t size() const;
    size_t numElements() const;
    size_t numBytes() const;

    bool isAllocated() const;
    void fill(const T val);
    T* data() const;
    void setInternalData(T* const input_ptr, const size_t num_rows, const size_t num_cols);
    Matrix<T> getTranspose() const;

    size_t lastRowIdx() const;
    size_t lastColIdx() const;

    T max() const;
    T min() const;
    T sum() const;
};

}  // namespace dvs

#endif // DVS_MATH_LIN_ALG_MATRIX_DYNAMIC_CLASS_DEF_MATRIX_DYNAMIC_H_
