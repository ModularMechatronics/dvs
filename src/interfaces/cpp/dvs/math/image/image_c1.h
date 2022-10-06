#ifndef DVS_MATH_IMAGE_IMAGE_C1_H_
#define DVS_MATH_IMAGE_IMAGE_C1_H_

#include <assert.h>
#include <stdint.h>

#include <cstring>
#include <iostream>

#include "dvs/logging.h"

namespace dvs
{

template <typename T> class ImageC1View
{
private:
    T* data_;
    size_t num_rows_;
    size_t num_cols_;

public:
    ImageC1View() : data_{nullptr}, num_rows_{0U}, num_cols_{0U} {}

    ImageC1View(T* const data_ptr_in, const size_t num_rows, const size_t num_cols)
        : data_{data_ptr_in}, num_rows_{num_rows}, num_cols_{num_cols}
    {
    }

    T* data() const
    {
        return data_;
    }

    size_t numRows() const
    {
        return num_rows_;
    }

    size_t numCols() const
    {
        return num_cols_;
    }

    size_t numBytes() const
    {
        return num_rows_ * num_cols_ * sizeof(T);
    }

    size_t numElements() const
    {
        return num_rows_ * num_cols_;
    }

    T& operator()(const size_t r, const size_t c)
    {
        assert((r < num_rows_) && "Row index is larger than num_rows_ - 1!");
        assert((c < num_cols_) && "Column index is larger than num_cols_ - 1!");

        return data_[r * num_cols_ + c];
    }

    const T& operator()(const size_t r, const size_t c) const
    {
        assert((r < num_rows_) && "Row index is larger than num_rows_ - 1!");
        assert((c < num_cols_) && "Column index is larger than num_cols_ - 1!");

        return data_[r * num_cols_ + c];
    }
};

template <typename T> class ImageC1
{
private:
    T* data_;
    size_t num_rows_;
    size_t num_cols_;

public:
    ImageC1();
    ImageC1(const size_t num_rows, const size_t num_cols);
    ~ImageC1();

    T& operator()(const size_t r, const size_t c);
    const T& operator()(const size_t r, const size_t c) const;

    size_t numRows() const;
    size_t numCols() const;
    size_t numBytes() const;
    void fillBufferWithData(uint8_t* const buffer) const;

    void resize(const size_t num_rows_, const size_t num_cols_);

    size_t numElements() const;
    T* data() const;
};

template <typename T> ImageC1<T>::~ImageC1()
{
    if (num_rows_ > 0U)
    {
        delete[] data_;
    }
}

template <typename T> ImageC1<T>::ImageC1() : data_{nullptr}, num_rows_{0}, num_cols_{0} {}

template <typename T> ImageC1<T>::ImageC1(const size_t num_rows, const size_t num_cols)
{
    DVS_ASSERT(num_rows > 0U) << "Cannot initialize with number of rows to 0!";
    DVS_ASSERT(num_cols > 0U) << "Cannot initialize with number of columns to 0!";
    data_ = new T[num_rows * num_cols];
    num_rows_ = num_rows;
    num_cols_ = num_cols;
}

template <typename T> void ImageC1<T>::fillBufferWithData(uint8_t* const buffer) const
{
    const uint8_t* const internal_ptr = reinterpret_cast<uint8_t*>(data_);
    const size_t num_bytes = num_rows_ * num_cols_ * sizeof(T);
    std::memcpy(buffer, internal_ptr, num_bytes);
}

template <typename T> size_t ImageC1<T>::numBytes() const
{
    return num_rows_ * num_cols_ * sizeof(T);
}

template <typename T> void ImageC1<T>::resize(const size_t num_rows, const size_t num_cols)
{
    DVS_ASSERT(num_rows > 0U) << "Cannot initialize with number of rows to 0!";
    DVS_ASSERT(num_cols > 0U) << "Cannot initialize with number of columns to 0!";

    if (num_rows_ > 0U)
    {
        delete[] data_;
    }

    data_ = new T[num_rows * num_cols];
    num_rows_ = num_rows;
    num_cols_ = num_cols;
}

template <typename T> size_t ImageC1<T>::numRows() const
{
    return num_rows_;
}

template <typename T> size_t ImageC1<T>::numCols() const
{
    return num_cols_;
}

template <typename T> size_t ImageC1<T>::numElements() const
{
    return num_rows_ * num_cols_;
}

template <typename T> T* ImageC1<T>::data() const
{
    return data_;
}

template <typename T> T& ImageC1<T>::operator()(const size_t r, const size_t c)
{
    assert((r < num_rows_) && "Row index is larger than num_rows_ - 1!");
    assert((c < num_cols_) && "Column index is larger than num_cols_ - 1!");

    return data_[r * num_cols_ + c];
}

template <typename T> const T& ImageC1<T>::operator()(const size_t r, const size_t c) const
{
    assert((r < num_rows_) && "Row index is larger than num_rows_ - 1!");
    assert((c < num_cols_) && "Column index is larger than num_cols_ - 1!");

    return data_[r * num_cols_ + c];
}

}  // namespace dvs

#endif  // DVS_MATH_IMAGE_IMAGE_C1_H_
