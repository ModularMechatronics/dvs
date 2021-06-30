#ifndef DVS_MATRIX_DYNAMIC_CLASS_H_
#define DVS_MATRIX_DYNAMIC_CLASS_H_

#include "math/misc/forward_decl.h"

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
    T& operator()(const size_t idx);
    const T& operator()(const size_t idx) const;

    Matrix<T> operator()(const size_t row, const IndexSpan& col_idx_span) const;
    Matrix<T> operator()(const IndexSpan& row_idx_span, const size_t col) const;
    MatrixView<T> operator()(const IndexSpan& row_idx_span, const IndexSpan& col_idx_span) const;

    MatrixView<T> operator()(const EndIndex& row_end_idx, const IndexSpan& col_idx_span) const;
    MatrixView<T> operator()(const IndexSpan& row_idx_span, const EndIndex& col_end_idx) const;

    MatrixView<T> operator()(const AllIndices& all_indices, const IndexSpan& col_idx_span) const;
    MatrixView<T> operator()(const IndexSpan& row_idx_span, const AllIndices& all_indices) const;

    MatrixView<T> operator()(const AllIndices& all_indices, const size_t col) const;
    MatrixView<T> operator()(const size_t row, const AllIndices& all_indices) const;

    T& operator()(const EndIndex& row_end_idx, const size_t c);
    const T& operator()(const EndIndex& row_end_idx, const size_t c) const;
    T& operator()(const size_t r, const EndIndex& col_end_idx);
    const T& operator()(const size_t r, const EndIndex& col_end_idx) const;
    Matrix<T>& operator=(const Matrix<T>& m);
    Matrix<T>& operator=(Matrix<T>&& m);
    Matrix<T>& operator=(const MatrixView<T>& m);

    void removeRowAtIndex(const size_t row_idx);
    void removeRowsAtIndices(const IndexSpan& idx_span);
    void removeColAtIndex(const size_t col_idx);
    void removeColsAtIndices(const IndexSpan& idx_span);
    void resize(const size_t num_rows, const size_t num_cols);
    size_t rows() const;
    size_t cols() const;
    size_t size() const;
    size_t numElements() const;
    size_t numBytes() const;
    void hCat(const Vector<T>& v);
    void vCat(const Vector<T>& v);

    void hCat(const Matrix<T>& m);
    void vCat(const Matrix<T>& m);

    void hCat(const Vec2D<T>& v);
    void vCat(const Vec2D<T>& v);
    void hCat(const Vec3D<T>& v);
    void vCat(const Vec3D<T>& v);
    void hCat(const Vec4D<T>& v);
    void vCat(const Vec4D<T>& v);

    bool isAllocated() const;
    void fill(const T val);
    void switchRows(size_t r0, size_t r1);
    void switchColumns(size_t c0, size_t c1);
    T* getDataPointer() const;
    void setInternalData(T* const input_ptr, const size_t num_rows, const size_t num_cols);
    Matrix<T> getTranspose() const;
    void transpose();

    void addToAllCols(const Vector<T>& v);
    void addToAllRows(const Vector<T>& v);
    void addToAllCols(const Matrix<T>& m);
    void addToAllRows(const Matrix<T>& m);

    void addToCol(const size_t col_idx, const Vector<T>& v);
    void addToRow(const size_t row_idx, const Vector<T>& v);
    void addToCol(const size_t col_idx, const Matrix<T>& m);
    void addToRow(const size_t row_idx, const Matrix<T>& m);

    Vector<T> toVector() const;

    Vector<T> getColumnAsVector(const size_t column_idx) const;
    Vector<T> getRowAsVector(const size_t row_idx) const;
    Matrix<T> getColumn(const size_t column_idx) const;
    Matrix<T> getRow(const size_t row_idx) const;
    size_t lastRowIdx() const;
    size_t lastColIdx() const;

    T max() const;
    T min() const;
    Matrix<T> minAlongCols() const;
    Matrix<T> minAlongRows() const;
    Matrix<T> maxAlongCols() const;
    Matrix<T> maxAlongRows() const;
    T sum() const;
    Matrix<T> sumAlongRows() const;
    Matrix<T> sumAlongCols() const;
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

#endif
