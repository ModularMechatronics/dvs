#ifndef DVS_MATH_IMAGE_IMAGE_RGBA_H_
#define DVS_MATH_IMAGE_IMAGE_RGBA_H_

#include <assert.h>
#include <stdint.h>

#include <cstring>
#include <iostream>

#include "dvs/logging.h"

namespace dvs
{

template <typename T> class ImageRGBAView
{
private:
    T* data_;
    size_t num_rows_;
    size_t num_cols_;
    size_t num_element_per_channel_;

public:
    ImageRGBAView() : data_{nullptr}, num_rows_{0U}, num_cols_{0U}, num_element_per_channel_{0U} {}

    ImageRGBAView(T* const data_ptr_in, const size_t num_rows, const size_t num_cols)
        : data_{data_ptr_in}, num_rows_{num_rows}, num_cols_{num_cols}, num_element_per_channel_{num_rows_ * num_cols_}
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

    size_t numElements() const
    {
        return num_rows_ * num_cols_;
    }

    size_t numBytes() const
    {
        return 4 * num_rows_ * num_cols_ * sizeof(T);
    }

    T& operator()(const size_t r, const size_t c, const size_t ch)
    {
        assert((r < num_rows_) && "Row index is larger than num_rows_ - 1!");
        assert((c < num_cols_) && "Column index is larger than num_cols_ - 1!");
        assert((ch < 4) && "Channel index is larger than 3!");

        return data_[ch * num_element_per_channel_ + r * num_cols_ + c];
    }

    const T& operator()(const size_t r, const size_t c, const size_t ch) const
    {
        assert((r < num_rows_) && "Row index is larger than num_rows_ - 1!");
        assert((c < num_cols_) && "Column index is larger than num_cols_ - 1!");
        assert((ch < 4) && "Channel index is larger than 3!");

        return data_[ch * num_element_per_channel_ + r * num_cols_ + c];
    }
};

template <typename T> class ImageRGBAConstView
{
private:
    const T* data_;
    size_t num_rows_;
    size_t num_cols_;
    size_t num_element_per_channel_;

public:
    ImageRGBAConstView() : data_{nullptr}, num_rows_{0U}, num_cols_{0U}, num_element_per_channel_{0U} {}

    ImageRGBAConstView(const T* const data_ptr_in, const size_t num_rows, const size_t num_cols)
        : data_{data_ptr_in}, num_rows_{num_rows}, num_cols_{num_cols}, num_element_per_channel_{num_rows_ * num_cols_}
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
        return 4 * num_rows_ * num_cols_ * sizeof(T);
    }

    const T& operator()(const size_t r, const size_t c, const size_t ch) const
    {
        assert((r < num_rows_) && "Row index is larger than num_rows_ - 1!");
        assert((c < num_cols_) && "Column index is larger than num_cols_ - 1!");
        assert((ch < 4) && "Channel index is larger than 3!");

        return data_[ch * num_element_per_channel_ + r * num_cols_ + c];
    }
};

template <typename T> class ImageRGBA
{
private:
    T* data_;
    size_t num_rows_;
    size_t num_cols_;
    size_t num_element_per_channel_;

public:
    ImageRGBA();
    ImageRGBA(const size_t num_rows, const size_t num_cols);
    ~ImageRGBA();

    T& operator()(const size_t r, const size_t c, const size_t ch);
    const T& operator()(const size_t r, const size_t c, const size_t ch) const;

    size_t numRows() const;
    size_t numCols() const;
    size_t numBytes() const;
    size_t numElements() const;
    void fillBufferWithData(uint8_t* const buffer) const;
    void fill(const T fill_value, const size_t channel);

    T* data() const;
};

template <typename T> ImageRGBA<T>::~ImageRGBA()
{
    if (num_rows_ > 0U)
    {
        delete[] data_;
    }
}

template <typename T>
ImageRGBA<T>::ImageRGBA() : data_{nullptr}, num_rows_(0U), num_cols_(0U), num_element_per_channel_(0U)
{
}

template <typename T> ImageRGBA<T>::ImageRGBA(const size_t num_rows, const size_t num_cols)
{
    DVS_ASSERT(num_rows > 0U) << "Cannot initialize with number of rows to 0!";
    DVS_ASSERT(num_cols > 0U) << "Cannot initialize with number of columns to 0!";

    data_ = new T[num_rows * num_cols * 4];
    num_rows_ = num_rows;
    num_cols_ = num_cols;
    num_element_per_channel_ = num_rows_ * num_cols_;
}

template <typename T> void ImageRGBA<T>::fillBufferWithData(uint8_t* const buffer) const
{
    const uint8_t* const internal_ptr = reinterpret_cast<uint8_t*>(data_);
    const size_t num_bytes = num_rows_ * num_cols_ * 4 * sizeof(T);
    std::memcpy(buffer, internal_ptr, num_bytes);
}

template <typename T> size_t ImageRGBA<T>::numBytes() const
{
    return 4 * num_rows_ * num_cols_ * sizeof(T);
}

template <typename T> size_t ImageRGBA<T>::numElements() const
{
    return 4 * num_rows_ * num_cols_;
}

template <typename T> size_t ImageRGBA<T>::numRows() const
{
    return num_rows_;
}

template <typename T> size_t ImageRGBA<T>::numCols() const
{
    return num_cols_;
}

template <typename T> T* ImageRGBA<T>::data() const
{
    return data_;
}

template <typename T> void ImageRGBA<T>::fill(const T fill_value, const size_t channel)
{
    for (size_t k = 0; k < (num_rows_ * num_cols_); k++)
    {
        data_[channel * num_element_per_channel_ + k] = fill_value;
    }
}

template <typename T> T& ImageRGBA<T>::operator()(const size_t r, const size_t c, const size_t ch)
{
    assert((r < num_rows_) && "Row index is larger than num_rows_ - 1!");
    assert((c < num_cols_) && "Column index is larger than num_cols_ - 1!");
    assert((ch < 4) && "Channel index is larger than 3!");

    return data_[ch * num_element_per_channel_ + r * num_cols_ + c];
}

template <typename T> const T& ImageRGBA<T>::operator()(const size_t r, const size_t c, const size_t ch) const
{
    assert((r < num_rows_) && "Row index is larger than num_rows_ - 1!");
    assert((c < num_cols_) && "Column index is larger than num_cols_ - 1!");
    assert((ch < 4) && "Channel index is larger than 3!");

    return data_[ch * num_element_per_channel_ + r * num_cols_ + c];
}

}  // namespace dvs

#endif  // DVS_MATH_IMAGE_IMAGE_RGBA_H_
