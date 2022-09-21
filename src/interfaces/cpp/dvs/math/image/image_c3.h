#ifndef DVS_MATH_IMAGE_IMAGE_C3_H_
#define DVS_MATH_IMAGE_IMAGE_C3_H_

#include <stdint.h>
#include <assert.h>

#include <cstring>
#include <iostream>

#include "dvs/logging.h"

namespace dvs
{

template <typename T> class ImageC3View
{
private:
    T* data_;
    size_t num_rows_;
    size_t num_cols_;
    size_t num_element_per_channel_;

public:
    ImageC3View() : data_{nullptr}, num_rows_{0U}, num_cols_{0U}, num_element_per_channel_{0U}
    {

    }

    ImageC3View(T* const data_ptr_in, const size_t num_rows, const size_t num_cols) :
        data_{data_ptr_in}, num_rows_{num_rows}, num_cols_{num_cols}, num_element_per_channel_{num_rows_ * num_cols_}
    {}

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
        return 3 * num_rows_ * num_cols_ * sizeof(T);
    }

    T& operator()(const size_t r, const size_t c, const size_t ch)
    {
        assert((r < num_rows_) && "Row index is larger than num_rows_ - 1!");
        assert((c < num_cols_) && "Column index is larger than num_cols_ - 1!");
        assert((ch < 3) && "Channel index is larger than 2!");

        return data_[ch * num_element_per_channel_ + r * num_cols_ + c];
    }

    const T& operator()(const size_t r, const size_t c, const size_t ch) const
    {
        assert((r < num_rows_) && "Row index is larger than num_rows_ - 1!");
        assert((c < num_cols_) && "Column index is larger than num_cols_ - 1!");
        assert((ch < 3) && "Channel index is larger than 2!");

        return data_[ch * num_element_per_channel_ + r * num_cols_ + c];
    }
};

template <typename T> class ImageC3
{
private:
    T* data_;
    size_t num_rows_;
    size_t num_cols_;
    size_t num_element_per_channel_;

public:
    ImageC3();
    ImageC3(const size_t num_rows, const size_t num_cols);
    ~ImageC3();

    T& operator()(const size_t r, const size_t c, const size_t ch);
    const T& operator()(const size_t r, const size_t c, const size_t ch) const;

    size_t numRows() const;
    size_t numCols() const;
    size_t numBytes() const;
    size_t numElements() const;
    void fillBufferWithData(uint8_t* const buffer) const;

    T* data() const;
};

template <typename T> ImageC3<T>::~ImageC3()
{
    if (num_rows_ > 0U)
    {
        delete[] data_;
    }
}

template <typename T>
ImageC3<T>::ImageC3() : data_{nullptr}, num_rows_(0U), num_cols_(0U), num_element_per_channel_(0U)
{
}

template <typename T> ImageC3<T>::ImageC3(const size_t num_rows, const size_t num_cols)
{
    DVS_ASSERT(num_rows > 0U) << "Cannot initialize with number of rows to 0!";
    DVS_ASSERT(num_cols > 0U) << "Cannot initialize with number of columns to 0!";

    data_ = new T[num_rows * num_cols * 3];
    num_rows_ = num_rows;
    num_cols_ = num_cols;
    num_element_per_channel_ = num_rows_ * num_cols_;
}

template <typename T> void ImageC3<T>::fillBufferWithData(uint8_t* const buffer) const
{
    const uint8_t* const internal_ptr = reinterpret_cast<uint8_t*>(data_);
    const size_t num_bytes = num_rows_ * num_cols_ * 3 * sizeof(T);
    std::memcpy(buffer, internal_ptr, num_bytes);
}

template <typename T> size_t ImageC3<T>::numBytes() const
{
    return 3 * num_rows_ * num_cols_ * sizeof(T);
}

template <typename T> size_t ImageC3<T>::numElements() const
{
    return 3 * num_rows_ * num_cols_;
}

template <typename T> size_t ImageC3<T>::numRows() const
{
    return num_rows_;
}

template <typename T> size_t ImageC3<T>::numCols() const
{
    return num_cols_;
}

template <typename T> T* ImageC3<T>::data() const
{
    return data_;
}

template <typename T> T& ImageC3<T>::operator()(const size_t r, const size_t c, const size_t ch)
{
    assert((r < num_rows_) && "Row index is larger than num_rows_ - 1!");
    assert((c < num_cols_) && "Column index is larger than num_cols_ - 1!");
    assert((ch < 3) && "Channel index is larger than 2!");

    return data_[ch * num_element_per_channel_ + r * num_cols_ + c];
}

template <typename T> const T& ImageC3<T>::operator()(const size_t r, const size_t c, const size_t ch) const
{
    assert((r < num_rows_) && "Row index is larger than num_rows_ - 1!");
    assert((c < num_cols_) && "Column index is larger than num_cols_ - 1!");
    assert((ch < 3) && "Channel index is larger than 2!");

    return data_[ch * num_element_per_channel_ + r * num_cols_ + c];
}

}  // namespace dvs

#endif // DVS_MATH_IMAGE_IMAGE_C3_H_
