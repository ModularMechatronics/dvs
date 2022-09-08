#ifndef DVS_INTERNAL_H_
#define DVS_INTERNAL_H_

#include <stdint.h>

#include <functional>

#include "dvs/communication.h"
#include "dvs/transmission_header.h"
#include "dvs/fillable_uint8_array.h"
#include "dvs/utils.h"
#include "math/math.h"

namespace dvs
{
namespace internal
{
constexpr uint64_t kMagicNumber = 0xdeadbeefcafebabe;
using SendFunctionType = std::function<void(const uint8_t* const data_blob, const uint64_t num_bytes)>;

// TODO: Take UInt8ArrayView as input argument instead
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

            const bool ack_received = checkAck(data);

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

        const bool ack_received = checkAck(data);

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
void sendHeaderAndData(const SendFunctionType& send_function, const TransmissionHeader& hdr, const U& first_element)
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

    send_function(fillable_array.data(), fillable_array.size());
}

template <typename U, typename... Us>
void sendHeaderAndData(const SendFunctionType& send_function,
                       const TransmissionHeader& hdr,
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

    send_function(fillable_array.data(), fillable_array.size());
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
                                   const TransmissionHeader& hdr,
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

    send_function(fillable_array.data(), fillable_array.size());
}

template <typename T> void fillBufferWithRefCollection(FillableUInt8Array& fillable_array, const std::vector<std::reference_wrapper<Vector<T>>>& data_to_be_sent)
{
    for (size_t k = 0; k < data_to_be_sent.size(); k++)
    {
        const Vector<T>& data_ref = data_to_be_sent[k];
        fillable_array.fillWithDataFromPointer(data_ref.data(), data_ref.numElements());
    }
}

template <typename T, typename... Us>
void fillBufferWithRefCollection(FillableUInt8Array& fillable_array, const std::vector<std::reference_wrapper<Vector<T>>>& data_to_be_sent, const Us&... other_elements)
{
    for (size_t k = 0; k < data_to_be_sent.size(); k++)
    {
        const Vector<T>& data_ref = data_to_be_sent[k];
        fillable_array.fillWithDataFromPointer(data_ref.data(), data_ref.numElements());
    }
    fillBufferWithRefCollection(fillable_array, other_elements...);
}

template <typename T, typename... Us>
void sendHeaderAndRefVectorCollection(const SendFunctionType& send_function,
                                      const TransmissionHeader& hdr,
                                      const Vector<uint16_t>& vector_lengths,
                                      const size_t num_bytes_to_send,
                                      const std::vector<std::reference_wrapper<Vector<T>>>& first_element,
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
        const Vector<T>& fe_ref = first_element[k];
        fillable_array.fillWithDataFromPointer(fe_ref.data(), fe_ref.numElements());
    }

    fillBufferWithRefCollection(fillable_array, other_elements...);

    send_function(fillable_array.data(), fillable_array.size());
}

inline void sendHeaderOnly(const SendFunctionType& send_function, const TransmissionHeader& hdr)
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

    send_function(fillable_array.data(), fillable_array.size());
}

}  // namespace internal
}  // namespace dvs

#endif // DVS_INTERNAL_H_
