#ifndef DVS_INTERNAL_H_
#define DVS_INTERNAL_H_

#include <stdint.h>

#include <functional>

#include "dvs/communication.h"
#include "dvs/function_header.h"
#include "dvs/utils.h"
#include "math/math.h"

namespace dvs
{
namespace internal
{
constexpr uint64_t kMagicNumber = 0xdeadbeefcafebabe;
using SendFunctionType = std::function<void(const uint8_t* const data_blob, const uint64_t num_bytes)>;

inline void sendThroughUdpInterface(const uint8_t* const data_blob, const uint64_t num_bytes)
{
    const size_t max_bytes_for_one_msg = 1380;
    UdpClient udp_client(9752);
    char data[256];

    if (num_bytes > max_bytes_for_one_msg)
    {
        size_t num_sent_bytes = 0;

        while (num_sent_bytes < num_bytes)
        {
            const size_t num_bytes_to_send =
                std::min(max_bytes_for_one_msg, static_cast<size_t>(num_bytes) - num_sent_bytes);

            udp_client.sendData(&(data_blob[num_sent_bytes]), num_bytes_to_send);
            num_sent_bytes += num_bytes_to_send;

            const int num_received_bytes = udp_client.receiveData(data);

            bool ack_received = checkAck(data);

            if (!ack_received)
            {
                throw std::runtime_error("No ack received!");
            }
            else if (num_received_bytes != 5)
            {
                throw std::runtime_error("Ack received but number of bytes was " + std::to_string(num_received_bytes));
            }
        }
    }
    else
    {
        udp_client.sendData(data_blob, num_bytes);

        const int num_received_bytes = udp_client.receiveData(data);

        bool ack_received = checkAck(data);

        if (!ack_received)
        {
            throw std::runtime_error("No ack received!");
        }
        else if (num_received_bytes != 5)
        {
            throw std::runtime_error("Ack received but number of bytes was " + std::to_string(num_received_bytes));
        }
    }
}

inline SendFunctionType getSendFunction()
{
    return sendThroughUdpInterface;
}

template <typename U> void countNumBytes(uint64_t& num_bytes, const U& data_to_be_sent)
{
    num_bytes += data_to_be_sent.numBytes();
}

template <typename U, typename... Us>
void countNumBytes(uint64_t& num_bytes, const U& data_to_be_sent, const Us&... datas)
{
    num_bytes += data_to_be_sent.numBytes();
    countNumBytes(num_bytes, datas...);
}

template <typename... Us> uint64_t countNumBytes(const Us&... datas)
{
    uint64_t num_bytes = 0;
    countNumBytes(num_bytes, datas...);
    return num_bytes;
}

template <typename U> void fillBuffer(uint8_t* const data_blob, const U& data_to_be_sent)
{
    data_to_be_sent.fillBufferWithData(data_blob);
}

template <typename U, typename... Us>
void fillBuffer(uint8_t* const data_blob, const U& data_to_be_sent, const Us&... other_elements)
{
    const uint64_t num_bytes = data_to_be_sent.numBytes();

    data_to_be_sent.fillBufferWithData(data_blob);
    fillBuffer(&(data_blob[num_bytes]), other_elements...);
}

template <typename U>
void sendHeaderAndData(const SendFunctionType& send_function, const FunctionHeader& hdr, const U& first_element)
{
    const uint64_t num_bytes_hdr = hdr.numBytes();
    const uint64_t num_bytes_first = first_element.numBytes();

    const uint64_t num_bytes_from_object = num_bytes_hdr + num_bytes_first;

    // + 1 bytes for endianness byte
    // + 2 * sizeof(uint64_t) for magic number and number of bytes in transfer
    const uint64_t num_bytes = num_bytes_from_object + 1 + 2 * sizeof(uint64_t);

    uint8_t* const data_blob = new uint8_t[num_bytes];

    uint64_t idx = 0;
    data_blob[idx] = isBigEndian();
    idx += 1;

    std::memcpy(&(data_blob[idx]), &kMagicNumber, sizeof(uint64_t));
    idx += sizeof(uint64_t);

    std::memcpy(&(data_blob[sizeof(uint64_t) + 1]), &num_bytes, sizeof(uint64_t));
    idx += sizeof(uint64_t);

    hdr.fillBufferWithData(&(data_blob[idx]));
    idx += num_bytes_hdr;

    first_element.fillBufferWithData(&(data_blob[idx]));

    send_function(data_blob, num_bytes);

    delete[] data_blob;
}

template <typename U, typename... Us>
void sendHeaderAndData(const SendFunctionType& send_function,
                       const FunctionHeader& hdr,
                       const U& first_element,
                       const Us&... other_elements)
{
    const uint64_t num_bytes_hdr = hdr.numBytes();
    const uint64_t num_bytes_first = first_element.numBytes();

    const uint64_t num_bytes_from_object = countNumBytes(other_elements...) + num_bytes_hdr + num_bytes_first;

    // + 1 bytes for endianness byte
    // + 2 * sizeof(uint64_t) for magic number and number of bytes in transfer
    const uint64_t num_bytes = num_bytes_from_object + 1 + 2 * sizeof(uint64_t);

    uint8_t* const data_blob = new uint8_t[num_bytes];

    uint64_t idx = 0;
    data_blob[idx] = isBigEndian();
    idx += 1;

    std::memcpy(&(data_blob[idx]), &kMagicNumber, sizeof(uint64_t));
    idx += sizeof(uint64_t);

    std::memcpy(&(data_blob[sizeof(uint64_t) + 1]), &num_bytes, sizeof(uint64_t));
    idx += sizeof(uint64_t);

    hdr.fillBufferWithData(&(data_blob[idx]));
    idx += num_bytes_hdr;

    first_element.fillBufferWithData(&(data_blob[idx]));
    idx += num_bytes_first;

    fillBuffer(&(data_blob[idx]), other_elements...);

    send_function(data_blob, num_bytes);

    delete[] data_blob;
}

template <typename U> void fillBufferWithCollection(uint8_t* const data_blob, const U& data_to_be_sent)
{
    size_t total_num_bytes = 0;
    for (size_t k = 0; k < data_to_be_sent.size(); k++)
    {
        data_to_be_sent[k].fillBufferWithData(&(data_blob[total_num_bytes]));
        total_num_bytes += data_to_be_sent[k].numBytes();
    }
}

template <typename U, typename... Us>
void fillBufferWithCollection(uint8_t* const data_blob, const U& data_to_be_sent, const Us&... other_elements)
{
    size_t total_num_bytes = 0;
    for (size_t k = 0; k < data_to_be_sent.size(); k++)
    {
        data_to_be_sent[k].fillBufferWithData(&(data_blob[total_num_bytes]));
        total_num_bytes += data_to_be_sent[k].numBytes();
    }
    fillBufferWithCollection(&(data_blob[total_num_bytes]), other_elements...);
}

template <typename U, typename... Us>
void sendHeaderAndVectorCollection(const SendFunctionType& send_function,
                                   const FunctionHeader& hdr,
                                   const Vector<uint8_t>& vector_lengths,
                                   const size_t num_bytes_to_send,
                                   const U& first_element,
                                   const Us&... other_elements)
{
    const uint64_t num_bytes_hdr = hdr.numBytes();
    const uint64_t num_bytes_from_object = num_bytes_to_send + num_bytes_hdr;

    // + 1 bytes for endianness byte
    // + 2 * sizeof(uint64_t) for magic number and number of bytes in transfer
    const uint64_t num_bytes = num_bytes_from_object + 1 + 2 * sizeof(uint64_t) + vector_lengths.numBytes();

    uint8_t* const data_blob = new uint8_t[num_bytes];

    uint64_t idx = 0;
    data_blob[idx] = isBigEndian();
    idx += 1;

    std::memcpy(&(data_blob[idx]), &kMagicNumber, sizeof(uint64_t));
    idx += sizeof(uint64_t);

    std::memcpy(&(data_blob[sizeof(uint64_t) + 1]), &num_bytes, sizeof(uint64_t));
    idx += sizeof(uint64_t);

    hdr.fillBufferWithData(&(data_blob[idx]));
    idx += num_bytes_hdr;

    vector_lengths.fillBufferWithData(&(data_blob[idx]));
    idx += vector_lengths.numBytes();

    for (size_t k = 0; k < first_element.size(); k++)
    {
        first_element[k].fillBufferWithData(&(data_blob[idx]));
        idx += first_element[k].numBytes();
    }

    fillBufferWithCollection(&(data_blob[idx]), other_elements...);

    send_function(data_blob, num_bytes + 8);

    delete[] data_blob;
}

inline void sendHeaderOnly(const SendFunctionType& send_function, const FunctionHeader& hdr)
{
    const uint64_t num_bytes_hdr = hdr.numBytes();

    // + 1 bytes for endianness byte
    // + 2 * sizeof(uint64_t) for magic number and number of bytes in transfer
    const uint64_t num_bytes = num_bytes_hdr + 1 + 2 * sizeof(uint64_t);

    uint8_t* const data_blob = new uint8_t[num_bytes];

    uint64_t idx = 0;
    data_blob[idx] = isBigEndian();
    idx += 1;

    std::memcpy(&(data_blob[idx]), &kMagicNumber, sizeof(uint64_t));
    idx += sizeof(uint64_t);

    std::memcpy(&(data_blob[sizeof(uint64_t) + 1]), &num_bytes, sizeof(uint64_t));
    idx += sizeof(uint64_t);

    hdr.fillBufferWithData(&(data_blob[idx]));

    send_function(data_blob, num_bytes);

    delete[] data_blob;
}

}  // namespace internal
}  // namespace dvs

#endif