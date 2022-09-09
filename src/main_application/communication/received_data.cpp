#include "communication/received_data.h"

#include "dvs/constants.h"

ReceivedData::ReceivedData(const UInt8ArrayView received_array_view)
        : hdr_(received_array_view)
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

ReceivedData::~ReceivedData()
{
    delete[] payload_data_;
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
