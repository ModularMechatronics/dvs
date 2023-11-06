#ifndef MAIN_APPLICATION_COMMUNICATION_RECEIVED_DATA_H_
#define MAIN_APPLICATION_COMMUNICATION_RECEIVED_DATA_H_

#include "dvs/internal.h"
#include "dvs/math/math.h"
#include "dvs/fillable_uint8_array.h"

class ReceivedData
{
private:
    dvs::internal::CommunicationHeader hdr_;
    dvs::internal::Function function_;
    uint8_t* payload_data_;
    uint8_t* raw_data_;
    uint64_t num_data_bytes_;
    uint64_t total_num_bytes_;

public:
    ReceivedData();
    ReceivedData(const ReceivedData& other) = delete;
    ReceivedData(ReceivedData&& other);
    ReceivedData& operator=(const ReceivedData& other) = delete;
    ReceivedData& operator=(ReceivedData&& other);

    ReceivedData(const size_t size_to_allocate);
    ~ReceivedData();

    void parseHeader();

    uint8_t* payloadData() const;
    uint8_t* rawData() const;
    uint64_t size() const;
    dvs::internal::Function getFunction() const;
    const dvs::internal::CommunicationHeader& getCommunicationHeader() const;
};

#endif  // MAIN_APPLICATION_COMMUNICATION_RECEIVED_DATA_H_
