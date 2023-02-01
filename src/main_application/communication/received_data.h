#ifndef MAIN_APPLICATION_COMMUNICATION_RECEIVED_DATA_H_
#define MAIN_APPLICATION_COMMUNICATION_RECEIVED_DATA_H_

#include "dvs/dvs.h"
#include "dvs/fillable_uint8_array.h"

class ReceivedData
{
private:
    dvs::internal::CommunicationHeader hdr_;
    dvs::internal::Function function_;
    uint8_t* payload_data_;
    uint64_t num_data_bytes_;

public:
    ReceivedData() = delete;
    ReceivedData(const ReceivedData& other) = delete;
    ReceivedData(ReceivedData&& other) = delete;
    ReceivedData operator=(const ReceivedData& other) = delete;
    ReceivedData operator=(ReceivedData&& other) = delete;

    ReceivedData(const UInt8ArrayView array_view);
    ~ReceivedData();

    uint8_t* data() const;
    uint64_t size() const;
    dvs::internal::Function getFunction() const;
    const dvs::internal::CommunicationHeader& getCommunicationHeader() const;
};

#endif  // MAIN_APPLICATION_COMMUNICATION_RECEIVED_DATA_H_
