#ifndef RECEIVED_DATA_H_
#define RECEIVED_DATA_H_

#include "dvs.h"

class ReceivedData
{
private:
    dvs::internal::FunctionHeader hdr_;
    uint8_t* payload_data_;
    uint64_t num_data_bytes_;

public:
    ReceivedData() = delete;
    ReceivedData(const ReceivedData& other) = delete;
    ReceivedData(ReceivedData&& other) = delete;

    /*ReceivedData(ReceivedData&& other)
    {
        (void)other;

        data_ = other.getDataPointer();
        other.setDataPointer(nullptr);

        num_data_bytes_ = other.getNumDataBytes();
        other.setNumDataBytes(0);

        hdr_ = other.getFunctionHeader();
    }*/

    ReceivedData(const uint8_t* const data, const uint64_t num_received_bytes) : hdr_(&(data[2 * sizeof(uint64_t) + 1]), data[0])
    {
        const uint64_t idx = hdr_.numBytes() + 2 * sizeof(uint64_t) + 1;

        // std::cout << "num_received_bytes: " << num_received_bytes << std::endl;

        if(idx > num_received_bytes)
        {
            throw std::runtime_error("idx can't be bigger than num_received_bytes");
        }

        num_data_bytes_ = num_received_bytes - idx;
        if(num_data_bytes_ == 0)
        {
            payload_data_ = nullptr;
        }
        else
        {
            payload_data_ = new uint8_t[num_data_bytes_];
            for(size_t k = 0; k < num_data_bytes_; k++)
            {
                payload_data_[k] = data[idx + k];
            }
        }
    }

    ~ReceivedData()
    {
        delete[] payload_data_;
    }

    uint8_t* getDataPointer() const
    {
        return payload_data_;
    }

    void setDataPointer(uint8_t* ptr)
    {
        payload_data_ = ptr;
    }

    void setNumDataBytes(const uint64_t ndb)
    {
        num_data_bytes_ = ndb;
    }

    uint64_t getNumDataBytes() const
    {
        return num_data_bytes_;
    }

    dvs::internal::FunctionHeader getFunctionHeader() const
    {
        return hdr_;
    }
};

#endif
