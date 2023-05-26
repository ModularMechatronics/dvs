#ifndef DVS_INTERNAL_H_
#define DVS_INTERNAL_H_

#include <stdint.h>

#include <functional>

#include "dvs/communication.h"
#include "dvs/communication_header.h"
#include "dvs/constants.h"
#include "dvs/fillable_uint8_array.h"
#include "dvs/property_set.h"
#include "dvs/timing.h"
#include "dvs/utils.h"
#include "math/math.h"

namespace dvs
{
namespace internal
{

using SendFunctionType = std::function<void(const UInt8ArrayView& input_array)>;

inline void sendThroughTcpInterface(const UInt8ArrayView& input_array)
{
    int tcp_sockfd;
    struct sockaddr_in tcp_servaddr;

    tcp_sockfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&tcp_servaddr, sizeof(tcp_servaddr));

    tcp_servaddr.sin_family = AF_INET;
    tcp_servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    tcp_servaddr.sin_port = htons(kTcpPortNum);

    if (connect(tcp_sockfd, (struct sockaddr*)&tcp_servaddr, sizeof(tcp_servaddr)) == (-1))
    {
        DVS_LOG_INFO() << "Failed to connect!";
    }

    const uint64_t num_bytes_to_send = input_array.size();

    write(tcp_sockfd, &num_bytes_to_send, sizeof(uint64_t));
    write(tcp_sockfd, input_array.data(), input_array.size());

    close(tcp_sockfd);
}

inline void sendThroughQueryUdpInterface(const UInt8ArrayView& input_array)
{
    const uint8_t* const data_to_send = input_array.data();
    const uint64_t total_num_bytes_to_send = input_array.size();

    constexpr int kNumReceiveBytes = 10;
    const UdpClient udp_client(kUdpQueryPortNum);
    char received_data[kNumReceiveBytes];

    if (total_num_bytes_to_send >= kMaxNumBytesForOneTransmission)
    {
        throw std::runtime_error("Tried to send too much data through query interface!");
    }

    udp_client.sendData(data_to_send, total_num_bytes_to_send);

    std::cout << "Before receive" << std::endl;
    const int num_received_bytes = udp_client.receiveData<kNumReceiveBytes>(received_data);
    std::cout << "After receive" << std::endl;

    if (!ackValid(received_data))
    {
        throw std::runtime_error("No valid ack received!");
    }
    else if (num_received_bytes != 5)
    {
        throw std::runtime_error("Ack received but number of bytes was " + std::to_string(num_received_bytes));
    }
}

inline size_t receiveFromQueryUdpInterface()
{
    constexpr int kNumReceiveBytes = 100;
    const UdpClient udp_client(kUdpQueryPortNum);
    char received_data[kNumReceiveBytes];
    std::cout << "Before" << std::endl;
    udp_client.receiveData<kNumReceiveBytes>(received_data);
    std::cout << "After" << std::endl;

    size_t received_size;

    std::memcpy(&received_size, received_data, sizeof(size_t));

    return received_size;
}

inline SendFunctionType getSendFunction()
{
    return sendThroughTcpInterface;
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

template <typename U> void fillBuffer(FillableUInt8Array& fillable_array, const U& data_to_be_sent)
{
    fillable_array.fillWithDataFromPointer(data_to_be_sent.data(), data_to_be_sent.numElements());
}

template <typename U, typename... Us>
void fillBuffer(FillableUInt8Array& fillable_array, const U& data_to_be_sent, const Us&... other_elements)
{
    fillable_array.fillWithDataFromPointer(data_to_be_sent.data(), data_to_be_sent.numElements());
    fillBuffer(fillable_array, other_elements...);
}

template <typename U>
void sendHeaderAndData(const SendFunctionType& send_function, const CommunicationHeader& hdr, const U& first_element)
{
    const uint64_t num_bytes_hdr = hdr.numBytes();
    const uint64_t num_bytes_first = first_element.numBytes();

    const uint64_t num_bytes_from_object = num_bytes_hdr + num_bytes_first;

    // + 1 bytes for endianness byte
    // + 2 * sizeof(uint64_t) for magic number and number of bytes in transfer
    const uint64_t num_bytes = num_bytes_from_object + 1 + 2 * sizeof(uint64_t);

    FillableUInt8Array fillable_array{num_bytes};

    fillable_array.fillWithStaticType(isBigEndian());

    fillable_array.fillWithStaticType(kMagicNumber);

    fillable_array.fillWithStaticType(num_bytes);

    hdr.fillBufferWithData(fillable_array);

    fillable_array.fillWithDataFromPointer(first_element.data(), first_element.numElements());

    send_function(fillable_array.view());
}

template <typename U, typename... Us>
void sendHeaderAndData(const SendFunctionType& send_function,
                       const CommunicationHeader& hdr,
                       const U& first_element,
                       const Us&... other_elements)
{
    const uint64_t num_bytes_hdr = hdr.numBytes();
    const uint64_t num_bytes_first = first_element.numBytes();

    const uint64_t num_bytes_from_object = countNumBytes(other_elements...) + num_bytes_hdr + num_bytes_first;

    // + 1 bytes for endianness byte
    // + 2 * sizeof(uint64_t) for magic number and number of bytes in transfer
    const uint64_t num_bytes = num_bytes_from_object + 1 + 2 * sizeof(uint64_t);

    FillableUInt8Array fillable_array{num_bytes};

    fillable_array.fillWithStaticType(isBigEndian());

    fillable_array.fillWithStaticType(kMagicNumber);

    fillable_array.fillWithStaticType(num_bytes);

    hdr.fillBufferWithData(fillable_array);

    fillable_array.fillWithDataFromPointer(first_element.data(), first_element.numElements());

    fillBuffer(fillable_array, other_elements...);

    send_function(fillable_array.view());
}

template <typename U> void fillBufferWithCollection(FillableUInt8Array& fillable_array, const U& data_to_be_sent)
{
    for (size_t k = 0; k < data_to_be_sent.size(); k++)
    {
        fillable_array.fillWithDataFromPointer(data_to_be_sent[k].data(), data_to_be_sent[k].numElements());
    }
}

template <typename U, typename... Us>
void fillBufferWithCollection(FillableUInt8Array& fillable_array, const U& data_to_be_sent, const Us&... other_elements)
{
    for (size_t k = 0; k < data_to_be_sent.size(); k++)
    {
        fillable_array.fillWithDataFromPointer(data_to_be_sent[k].data(), data_to_be_sent[k].numElements());
    }

    fillBufferWithCollection(fillable_array, other_elements...);
}

template <typename U, typename... Us>
void sendHeaderAndVectorCollection(const SendFunctionType& send_function,
                                   const CommunicationHeader& hdr,
                                   const Vector<uint16_t>& vector_lengths,
                                   const size_t num_bytes_to_send,
                                   const U& first_element,
                                   const Us&... other_elements)
{
    const uint64_t num_bytes_hdr = hdr.numBytes();
    const uint64_t num_bytes_from_object = num_bytes_to_send + num_bytes_hdr;

    // + 1 bytes for endianness byte
    // + 2 * sizeof(uint64_t) for magic number and number of bytes in transfer
    const uint64_t num_bytes = num_bytes_from_object + 1 + 2 * sizeof(uint64_t) + vector_lengths.numBytes();

    FillableUInt8Array fillable_array{num_bytes};

    fillable_array.fillWithStaticType(isBigEndian());

    fillable_array.fillWithStaticType(kMagicNumber);

    fillable_array.fillWithStaticType(num_bytes);

    hdr.fillBufferWithData(fillable_array);

    fillable_array.fillWithDataFromPointer(vector_lengths.data(), vector_lengths.numElements());

    for (size_t k = 0; k < first_element.size(); k++)
    {
        fillable_array.fillWithDataFromPointer(first_element[k].data(), first_element[k].numElements());
    }

    fillBufferWithCollection(fillable_array, other_elements...);

    send_function(fillable_array.view());
}

inline void sendHeaderOnly(const SendFunctionType& send_function, const CommunicationHeader& hdr)
{
    const uint64_t num_bytes_hdr = hdr.numBytes();

    // + 1 bytes for endianness byte
    // + 2 * sizeof(uint64_t) for magic number and number of bytes in transfer
    const uint64_t num_bytes = num_bytes_hdr + 1 + 2 * sizeof(uint64_t);

    FillableUInt8Array fillable_array{num_bytes};

    fillable_array.fillWithStaticType(isBigEndian());

    fillable_array.fillWithStaticType(kMagicNumber);

    fillable_array.fillWithStaticType(num_bytes);

    hdr.fillBufferWithData(fillable_array);

    send_function(fillable_array.view());
}

}  // namespace internal
}  // namespace dvs

#endif  // DVS_INTERNAL_H_
