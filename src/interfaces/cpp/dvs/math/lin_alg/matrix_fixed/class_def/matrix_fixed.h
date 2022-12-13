#ifndef DVS_MATH_LIN_ALG_MATRIX_FIXED_CLASS_DEF_MATRIX_FIXED_H_
#define DVS_MATH_LIN_ALG_MATRIX_FIXED_CLASS_DEF_MATRIX_FIXED_H_

#include <assert.h>

#include "dvs/logging.h"
#include "dvs/math/misc/forward_decl.h"

namespace dvs
{

template <typename T, uint16_t R, uint16_t C> class MatrixFixed
{
    // protected:
public:
    T data_[R * C];

    MatrixFixed();
    template <typename Y> MatrixFixed(const MatrixFixed<Y, R, C>& m);

    void fillBufferWithData(uint8_t* const buffer) const;

    T& operator()(const size_t r, const size_t c);
    const T& operator()(const size_t r, const size_t c) const;

    size_t numRows() const;
    size_t numCols() const;
    size_t size() const;
    size_t numElements() const;
    size_t numBytes() const;

    void fill(const T val);
    T* data() const;
    // Matrix<T, C, R> getTranspose() const;

    T max() const;
    T min() const;
    T sum() const;

    MatrixFixed<T, R, C> transposed() const;
};

}  // namespace dvs

#endif  // DVS_MATH_LIN_ALG_MATRIX_DYNAMIC_CLASS_DEF_MATRIX_DYNAMIC_H_
