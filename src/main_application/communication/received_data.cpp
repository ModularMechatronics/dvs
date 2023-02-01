#include "communication/received_data.h"

#include "dvs/constants.h"

ReceivedData::ReceivedData()
    : hdr_{}, function_{dvs::internal::Function::UNKNOWN}, payload_data_{nullptr}, num_data_bytes_{0U}
{
}

ReceivedData::ReceivedData(const UInt8ArrayView received_array_view)
    : hdr_(received_array_view), function_{hdr_.getFunction()}
{
    const uint64_t transmission_data_offset = hdr_.numBytes() + dvs::internal::kHeaderDataStartOffset;

    if (transmission_data_offset > received_array_view.size())
    {
        throw std::runtime_error("transmission_data_offset can't be bigger than num_received_bytes");
    }

    num_data_bytes_ = received_array_view.size() - transmission_data_offset;

    if (num_data_bytes_ == 0)
    {
        payload_data_ = nullptr;
    }
    else
    {
        payload_data_ = new uint8_t[num_data_bytes_];
        std::memcpy(payload_data_, received_array_view.data() + transmission_data_offset, num_data_bytes_);
    }
}

ReceivedData::ReceivedData(ReceivedData&& other)
    : hdr_{other.hdr_},
      function_{other.function_},
      payload_data_{other.payload_data_},
      num_data_bytes_{other.num_data_bytes_}
{
    other.function_ = dvs::internal::Function::UNKNOWN;
    other.payload_data_ = nullptr;
    other.num_data_bytes_ = 0U;
}

ReceivedData& ReceivedData::operator=(ReceivedData&& other)
{
    if (payload_data_ != nullptr)
    {
        delete[] payload_data_;
    }

    hdr_ = other.hdr_;
    function_ = other.function_;
    payload_data_ = other.payload_data_;
    num_data_bytes_ = other.num_data_bytes_;

    other.function_ = dvs::internal::Function::UNKNOWN;
    other.payload_data_ = nullptr;
    other.num_data_bytes_ = 0U;

    return *this;
}

ReceivedData::~ReceivedData()
{
    delete[] payload_data_;
}

dvs::internal::Function ReceivedData::getFunction() const
{
    return function_;
}

uint8_t* ReceivedData::data() const
{
    return payload_data_;
}

uint64_t ReceivedData::size() const
{
    return num_data_bytes_;
}

const dvs::internal::CommunicationHeader& ReceivedData::getCommunicationHeader() const
{
    return hdr_;
}
