#ifndef DVS_MATH_LIN_ALG_MATRIX_DYNAMIC_CLASS_DEF_MATRIX_DYNAMIC_H_
#define DVS_MATH_LIN_ALG_MATRIX_DYNAMIC_CLASS_DEF_MATRIX_DYNAMIC_H_

#include "dvs/math/misc/forward_decl.h"

namespace dvs
{
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
    Matrix(const MatrixView<T>& mv);
    template <typename Y> Matrix(const Matrix<Y>& m);
    Matrix(const std::vector<std::vector<T>>& vm);
    Matrix(const std::initializer_list<std::initializer_list<T>>& il);
    Matrix(Matrix<T>&& m);
    ~Matrix();

    void fillBufferWithData(uint8_t* const buffer) const;

    Matrix<T>&& move();

    T& operator()(const size_t r, const size_t c);
    const T& operator()(const size_t r, const size_t c) const;

    Matrix<T>& operator=(const Matrix<T>& m);
    Matrix<T>& operator=(Matrix<T>&& m);
    Matrix<T>& operator=(const MatrixView<T>& m);

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

template <typename T> class MatrixView
{
private:
    T* data_;
    size_t num_rows_parent_;
    size_t num_cols_parent_;
    size_t num_rows_;
    size_t num_cols_;
    size_t start_row_;
    size_t start_col_;

    size_t start_idx_;

public:
    MatrixView() = delete;
    MatrixView(const MatrixView<T>& m);
    MatrixView(MatrixView&& m) = delete;
    MatrixView(T* data,
               const size_t start_row,
               const size_t start_col,
               const size_t num_rows_parent,
               const size_t num_cols_parent,
               const size_t num_rows,
               const size_t num_cols);
    // operator=(const T val);
    MatrixView<T>& operator=(const MatrixView<T>& other);
    T& operator()(const size_t r, const size_t c);
    const T& operator()(const size_t r, const size_t c) const;

    size_t rows() const;
    size_t cols() const;
};

}  // namespace dvs

#endif // DVS_MATH_LIN_ALG_MATRIX_DYNAMIC_CLASS_DEF_MATRIX_DYNAMIC_H_
