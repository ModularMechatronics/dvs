#ifndef DVS_FILLABLE_UINT8_ARRAY_H_
#define DVS_FILLABLE_UINT8_ARRAY_H_

#include <stdint.h>
#include <assert.h>

class UInt8ArrayView
{
private:
    uint8_t* data_;
    size_t size_;
public:
    UInt8ArrayView() : data_{nullptr}, size_{0U} {}
    UInt8ArrayView(uint8_t* data_in, const size_t size_in) : data_{data_in}, size_{size_in} {}

    uint8_t* data() const
    {
        return data_;
    }

    size_t size() const
    {
        return size_;
    }
};

class FillableUInt8Array
{
private:
    uint8_t* data_;
    size_t size_;
    size_t idx_;

public:
    FillableUInt8Array() : data_{nullptr}, size_{0U}, idx_{0U} {}
    FillableUInt8Array(const size_t array_size) : data_{new uint8_t[array_size]}, size_{array_size}, idx_{0U} {}
    FillableUInt8Array(const FillableUInt8Array& a) = delete;
    ~FillableUInt8Array()
    {
        delete[] data_;
    }

    uint8_t* data() const
    {
        return data_;
    }

    size_t size() const
    {
        return size_;
    }

    template <typename T>
    void fillWithStaticType(const T& d)
    {
        std::memcpy(&(data_[idx_]), &d, sizeof(T));
        idx_ += sizeof(T);

        assert(idx_ <= size_);
    }

    template <typename T>
    void fillWithDataFromPointer(const T* const d, const size_t num_elements)
    {
        std::memcpy(&(data_[idx_]), d, num_elements * sizeof(T));
        idx_ += num_elements * sizeof(T);

        assert(idx_ <= size_);
    }

    UInt8ArrayView view() const
    {
        return UInt8ArrayView{data_, size_};
    }
};

#endif // DVS_FILLABLE_UINT8_ARRAY_H_
