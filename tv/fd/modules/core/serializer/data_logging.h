#ifndef DATA_LOGGING_H_
#define DATA_LOGGING_H_

#include <stddef.h>
#include <stdint.h>
#include <cstring>

#include <vector>
#include <mutex>
#include <string>
#include <iostream>

#include "enumerations.h"

namespace data_logging
{

constexpr size_t kMaxNumBuffers = 1024;

namespace internal
{

inline size_t sizeOfType(const char* const c)
{
    std::cout << "sizeOf const char* const" << std::endl;
    // +2 to encode string length
    return strlen(c) + 2;
}

inline size_t sizeOfType(const std::string& s)
{
    std::cout << "sizeOf string" << std::endl;
    // +2 to encode string length
    return s.length() + 2;
}

template <typename T>
size_t sizeOfType(const T& v)
{
    std::cout << "sizeOf template type" << std::endl;
    return sizeof(v);
}

template <typename T>
void serialize(const T& t, uint8_t* output_buffer)
{
    const uint8_t* const tmp_ptr = reinterpret_cast<const uint8_t* const>(&t);

    for(size_t k = 0; k < sizeof(T); k++)
    {
        output_buffer[k] = tmp_ptr[k];
    }
}

inline void serialize(const std::string& s, uint8_t* output_buffer)
{
    const uint16_t string_len = s.length();
    size_t idx = 0U;

    std::memcpy(output_buffer, &string_len, sizeof(uint16_t));
    idx += sizeof(uint16_t);

    for(size_t k = 0; k < s.length(); k++)
    {
        output_buffer[idx] = s[k];
        idx += 1;
    }
}


class Buffer
{
private:
    uint8_t* data_;
    size_t buffer_size_;
    size_t idx_;

public:
    Buffer() = delete;
    Buffer(const size_t buffer_size) : 
        data_(new uint8_t[buffer_size]),
        buffer_size_(buffer_size),
        idx_(0U)
    {}

    ~Buffer()
    {
        delete[] data_;
    }

    void writeBytes(const uint8_t* const input_data, const size_t num_bytes)
    {
        if((idx_ + num_bytes) > buffer_size_)
        {
            buffer_size_ = buffer_size_ * 2U;
            uint8_t* new_ptr = new uint8_t[buffer_size_];

            memcpy(new_ptr, data_, idx_);
            delete[] data_;
            data_ = new_ptr;
        }

        memcpy(&(data_[idx_]), input_data, num_bytes);
        idx_ += num_bytes;
    }

    uint8_t* getDataPointer() const
    {
        return data_;
    }

    size_t fullSize() const
    {
        return buffer_size_;
    }

    size_t usedSize() const
    {
        return idx_;
    }
};

class DataBuffer
{
private:
    size_t buffer_size_;
    size_t num_buffers_;

    Buffer** data_; 

public:

    size_t bufferSize() const
    {
        return buffer_size_;
    }

    size_t numBuffers() const
    {
        return num_buffers_;
    }

    DataBuffer();
    DataBuffer(const size_t buffer_size);
    DataBuffer(const size_t buffer_size, const size_t num_buffers);

    DataBuffer(const DataBuffer& other) = delete;
    DataBuffer& operator=(const DataBuffer& other) = delete;
    DataBuffer& operator=(DataBuffer&& other) = delete;

    void reinit(const size_t buffer_size, const size_t num_buffers);

    template <typename T>
    void append(const int data_id, const T& data)
    {
        const size_t num_bytes_to_write = sizeOfType(data);
    }

    ~DataBuffer();

};

extern DataBuffer global_instance_data_buffer_;
extern std::mutex global_instance_mtx_;
}

void setBufferSize(const size_t buffer_size);
void setBufferSizeAndNumBuffers(const size_t buffer_size, const size_t num_buffers);

template <typename T>
void append(const int data_id, const T& data)
{
    std::lock_guard l(internal::global_instance_mtx_);
    internal::global_instance_data_buffer_.append(data_id, data);
}

void saveToFile(const int data_id, const std::string& file_name);

/*
float f;
std::string s;
Vector3f v;

append(0, v);

*/

}



// #if __has_include("custom_data_logging_names.h")
// #include "custom_data_logging_names.h"
// #endif

#endif
