#ifndef DVS_MATH_IMAGE_IMAGE_C1_H_
#define DVS_MATH_IMAGE_IMAGE_C1_H_

#include <assert.h>
#include <stdint.h>

#include <cstring>
#include <iostream>

namespace dvs
{
template <typename T> class ImageC1
{
private:
    T* data_;
    size_t num_rows_;
    size_t num_cols_;
    bool is_allocated_;

public:
    ImageC1();
    ImageC1(const size_t num_rows, const size_t num_cols);
    ~ImageC1();

    T& operator()(const size_t r, const size_t c);
    const T& operator()(const size_t r, const size_t c) const;

    bool isAllocated() const;
    size_t numRows() const;
    size_t numCols() const;
    size_t numBytes() const;
    void fillBufferWithData(uint8_t* const buffer) const;
    void setInternalData(T* const input_ptr, const size_t num_rows, const size_t num_cols);

    void resize(const size_t num_rows_, const size_t num_cols_);

    size_t numElements() const;
    T* data() const;
};

template <typename T> ImageC1<T>::~ImageC1()
{
    if (is_allocated_)
    {
        delete[] data_;
    }
}

template <typename T> ImageC1<T>::ImageC1() : num_rows_(0), num_cols_(0), is_allocated_(false) {}

template <typename T> ImageC1<T>::ImageC1(const size_t num_rows, const size_t num_cols)
{
    data_ = new T[num_rows * num_cols];
    num_rows_ = num_rows;
    num_cols_ = num_cols;
    is_allocated_ = true;
}

template <typename T> void ImageC1<T>::setInternalData(T* const input_ptr, const size_t num_rows, const size_t num_cols)
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
    if (is_allocated_)
    {
        delete[] data_;
        is_allocated_ = false;
        num_rows_ = 0;
        num_cols_ = 0;
    }

    data_ = new T[num_rows * num_cols];
    num_rows_ = num_rows;
    num_cols_ = num_cols;
    is_allocated_ = true;
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
    assert(r < num_rows_ && "Row index is larger than num_rows_-1!");
    assert(c < num_cols_ && "Column index is larger than num_cols_-1!");

    return data_[r * num_cols_ + c];
}

template <typename T> const T& ImageC1<T>::operator()(const size_t r, const size_t c) const
{
    assert(r < num_rows_ && "Row index is larger than num_rows_-1!");
    assert(c < num_cols_ && "Column index is larger than num_cols_-1!");

    return data_[r * num_cols_ + c];
}

template <typename T> bool ImageC1<T>::isAllocated() const
{
    return is_allocated_;
}

}  // namespace dvs

#endif // DVS_MATH_IMAGE_IMAGE_C1_H_
