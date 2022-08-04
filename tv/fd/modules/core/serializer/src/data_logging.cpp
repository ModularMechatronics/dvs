#include "data_logging.h"

#include <iostream>

namespace data_logging
{
namespace
{

constexpr size_t kDefaultNumBuffers = 10;
constexpr size_t kDefaultBufferSize = 1024;

}

namespace internal
{
DataBuffer global_instance_data_buffer_{};
std::mutex global_instance_mtx_{};

void DataBuffer::reinit(const size_t buffer_size, const size_t num_buffers)
{
    for(size_t k = 0; k < num_buffers_; k++)
    {
        delete[] data_[k];
    }
    delete[] data_;

    buffer_size_ = buffer_size;
    num_buffers_ = num_buffers;

    data_ = new Buffer*[num_buffers_];

    for(size_t k = 0; k < num_buffers_; k++)
    {
        data_[k] = new Buffer(buffer_size);
    }
}

DataBuffer::DataBuffer(const size_t buffer_size) : DataBuffer(buffer_size, kDefaultNumBuffers)
{
    std::cout << "Constructor 1 arg" << std::endl;
}

DataBuffer::DataBuffer(const size_t buffer_size, const size_t num_buffers) : buffer_size_(buffer_size), num_buffers_(num_buffers)
{
    std::cout << "Constructor 2 args" << std::endl;

    data_ = new Buffer*[num_buffers_];

    for(size_t k = 0; k < num_buffers_; k++)
    {
        data_[k] = new Buffer(buffer_size);
    }
}

DataBuffer::DataBuffer() : DataBuffer(kDefaultBufferSize, kDefaultNumBuffers)
{
    std::cout << "Constructor" << std::endl;
}

DataBuffer::~DataBuffer()
{
    std::cout << "Destructor" << std::endl;

    for(size_t k = 0; k < num_buffers_; k++)
    {
        delete data_[k];
    }
    delete[] data_;
}

}

void setBufferSize(const size_t buffer_size)
{
    std::lock_guard l(internal::global_instance_mtx_);
    internal::global_instance_data_buffer_.reinit(buffer_size, internal::global_instance_data_buffer_.numBuffers());
}

void setBufferSizeAndNumBuffers(const size_t buffer_size, const size_t num_buffers)
{
    std::lock_guard l(internal::global_instance_mtx_);
    internal::global_instance_data_buffer_.reinit(buffer_size, num_buffers);
}

}

