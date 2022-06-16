#ifndef DVS_INTERNAL_H_
#define DVS_INTERNAL_H_

#include <stdint.h>

#include "dvs/communication.h"
#include "dvs/function_header.h"
#include "dvs/internal.h"
#include "dvs/math/math.h"

const uint64_t kMagicNumber = 0xdeadbeefcafebabe;
const uint64_t max_bytes_for_one_msg = 1380;

int checkAck(char data[256])
{
    const int ar = data[0] == 'a' && data[1] == 'c' && data[2] == 'k' && data[3] == '#' && data[4] == '\0';
    return ar;
}

uint8_t isBigEndian()
{
    const uint32_t x = 1;
    const uint8_t* const ptr = (uint8_t*)(&x);

    if (ptr[0] == '\x01')
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

uint64_t minVal(const uint64_t v0, const uint64_t v1)
{
    if (v0 < v1)
    {
        return v0;
    }
    else
    {
        return v1;
    }
}

void sendThroughUdpInterface(const uint8_t* const data_blob, const uint64_t num_bytes)
{
    SocketStructure sock_struct = createSocket(9752);
    char data[256];

    if (num_bytes > max_bytes_for_one_msg)
    {
        uint64_t num_sent_bytes = 0;

        while (num_sent_bytes < num_bytes)
        {
            const uint64_t num_bytes_to_send = minVal(max_bytes_for_one_msg, num_bytes - num_sent_bytes);

            sendData(&sock_struct, &(data_blob[num_sent_bytes]), num_bytes_to_send);
            num_sent_bytes += num_bytes_to_send;

            const int num_received_bytes = receiveData(&sock_struct, data);

            const int ack_received = checkAck(data);

            if (!ack_received)
            {
                printf("Error receiving!\n");
                exit(-1);
            }
            else if (num_received_bytes != 5)
            {
                printf("Error receiving, too many bytes!\n");
                exit(-1);
            }
        }
    }
    else
    {
        sendData(&sock_struct, data_blob, num_bytes);

        const int num_received_bytes = receiveData(&sock_struct, data);

        int ack_received = checkAck(data);

        if (!ack_received)
        {
            printf("Error receiving!\n");
            exit(-1);
        }
        else if (num_received_bytes != 5)
        {
            printf("Error receiving, too many bytes!\n");
            exit(-1);
        }
    }
}

#define APPEND_PROPERTIES(__hdr, __first_prop)            \
    {                                                     \
        va_list __args;                                   \
        va_start(__args, __first_prop);                   \
                                                          \
        FunctionHeaderObject __prp = __first_prop;        \
        while (__prp.type != FHOT_UNKNOWN)                \
        {                                                 \
            appendFunctionHeaderObject(&__hdr, &__prp);   \
            __prp = va_arg(__args, FunctionHeaderObject); \
        }                                                 \
        va_end(__args);                                   \
    }

#define APPEND_VAL(__hdr, __type, __val, __target_data_type)                          \
    {                                                                                 \
        (__hdr)->values[(__hdr)->num_objects].type = __type;                          \
        (__hdr)->values[(__hdr)->num_objects].num_bytes = sizeof(__target_data_type); \
        __target_data_type __tmp_val = __val;                                         \
        uint8_t* __obj_ptr = (uint8_t*)(&__tmp_val);                                  \
        for (size_t __k = 0; __k < sizeof(__target_data_type); __k++)                 \
        {                                                                             \
            (__hdr)->values[(__hdr)->num_objects].data[__k] = __obj_ptr[__k];         \
        }                                                                             \
        (__hdr)->num_objects += 1;                                                    \
    }

typedef void (*SendFunction)(const uint8_t* const, const uint64_t);

SendFunction getSendFunction()
{
    return sendThroughUdpInterface;
}

int countNumHeaderBytes(const FunctionHeader* const hdr)
{
    // 1 for first byte, that indicates how many attributes in buffer, which is
    // same as hdr->num_objects
    size_t s = 1;

    for (size_t k = 0; k < hdr->num_objects; k++)
    {
        s = s + sizeof(uint16_t) + sizeof(uint8_t) + hdr->values[k].num_bytes;
    }

    return s;
}

void fillBufferWithHeader(const FunctionHeader* const hdr, uint8_t* const buffer)
{
    size_t idx = 1;
    buffer[0] = (uint8_t)(hdr->num_objects);
    const FunctionHeaderObject* const values = hdr->values;

    for (size_t k = 0; k < hdr->num_objects; k++)
    {
        memcpy(&(buffer[idx]), &(values[k].type), sizeof(uint16_t));
        idx += sizeof(uint16_t);

        memcpy(&(buffer[idx]), &(values[k].num_bytes), sizeof(uint8_t));
        idx += sizeof(uint8_t);

        memcpy(&(buffer[idx]), values[k].data, values[k].num_bytes);
        idx += values[k].num_bytes;
    }
}

void sendHeaderAndByteArray(SendFunction send_function,
                            const uint8_t* const array,
                            const size_t num_bytes_from_array,
                            FunctionHeader* hdr)
{
    const uint64_t num_bytes_hdr = countNumHeaderBytes(hdr);

    // + 1 bytes for endianness byte
    // + 2 * sizeof(uint64_t) for magic number and number of bytes in transfer
    const uint64_t num_bytes = num_bytes_from_array + num_bytes_hdr + 1 + 2 * sizeof(uint64_t);

    uint8_t* const data_blob = malloc(num_bytes);

    uint64_t idx = 0;
    data_blob[idx] = isBigEndian();
    idx += 1;

    memcpy(&(data_blob[idx]), &kMagicNumber, sizeof(uint64_t));
    idx += sizeof(uint64_t);

    memcpy(&(data_blob[sizeof(uint64_t) + 1]), &num_bytes, sizeof(uint64_t));
    idx += sizeof(uint64_t);

    fillBufferWithHeader(hdr, &(data_blob[idx]));
    idx += num_bytes_hdr;

    memcpy(&(data_blob[idx]), array, num_bytes_from_array);
    idx += num_bytes_from_array;

    send_function(data_blob, num_bytes);

    free(data_blob);
}

void sendHeaderAndTwoByteArrays(SendFunction send_function,
                                const uint8_t* const array0,
                                const size_t num_bytes_from_array0,
                                const uint8_t* const array1,
                                const size_t num_bytes_from_array1,
                                FunctionHeader* hdr)
{
    const uint64_t num_bytes_hdr = countNumHeaderBytes(hdr);

    // + 1 bytes for endianness byte
    // + 2 * sizeof(uint64_t) for magic number and number of bytes in transfer
    const uint64_t num_bytes = num_bytes_from_array0 + num_bytes_from_array1 + num_bytes_hdr + 1 + 2 * sizeof(uint64_t);

    uint8_t* const data_blob = malloc(num_bytes);

    uint64_t idx = 0;
    data_blob[idx] = isBigEndian();
    idx += 1;

    memcpy(&(data_blob[idx]), &kMagicNumber, sizeof(uint64_t));
    idx += sizeof(uint64_t);

    memcpy(&(data_blob[sizeof(uint64_t) + 1]), &num_bytes, sizeof(uint64_t));
    idx += sizeof(uint64_t);

    fillBufferWithHeader(hdr, &(data_blob[idx]));
    idx += num_bytes_hdr;

    memcpy(&(data_blob[idx]), array0, num_bytes_from_array0);
    idx += num_bytes_from_array0;

    memcpy(&(data_blob[idx]), array1, num_bytes_from_array1);
    idx += num_bytes_from_array1;

    send_function(data_blob, num_bytes);

    free(data_blob);
}

void sendHeaderAndTwoVectors(SendFunction send_function,
                             const Vector* const x,
                             const Vector* const y,
                             FunctionHeader* hdr)
{
    const uint64_t num_bytes_hdr = countNumHeaderBytes(hdr);
    const uint64_t num_bytes_one_vector = x->num_elements * x->num_bytes_per_element;

    const uint64_t num_bytes_from_object = 2 * num_bytes_one_vector + num_bytes_hdr;

    // + 1 bytes for endianness byte
    // + 2 * sizeof(uint64_t) for magic number and number of bytes in transfer
    const uint64_t num_bytes = num_bytes_from_object + 1 + 2 * sizeof(uint64_t);

    uint8_t* const data_blob = malloc(num_bytes);

    uint64_t idx = 0;
    data_blob[idx] = isBigEndian();
    idx += 1;

    memcpy(&(data_blob[idx]), &kMagicNumber, sizeof(uint64_t));
    idx += sizeof(uint64_t);

    memcpy(&(data_blob[sizeof(uint64_t) + 1]), &num_bytes, sizeof(uint64_t));
    idx += sizeof(uint64_t);

    fillBufferWithHeader(hdr, &(data_blob[idx]));
    idx += num_bytes_hdr;

    memcpy(&(data_blob[idx]), x->data, num_bytes_one_vector);
    idx += num_bytes_one_vector;

    memcpy(&(data_blob[idx]), y->data, num_bytes_one_vector);
    idx += num_bytes_one_vector;

    send_function(data_blob, num_bytes);

    free(data_blob);
}

void sendHeaderAndThreeMatrices(SendFunction send_function,
                                const Matrix* const x,
                                const Matrix* const y,
                                const Matrix* const z,
                                FunctionHeader* hdr)
{
    const uint64_t num_bytes_hdr = countNumHeaderBytes(hdr);
    const uint64_t num_bytes_one_matrix = x->num_rows * x->num_cols * x->num_bytes_per_element;

    const uint64_t num_bytes_from_object = 3 * num_bytes_one_matrix + num_bytes_hdr;

    // + 1 bytes for endianness byte
    // + 2 * sizeof(uint64_t) for magic number and number of bytes in transfer
    const uint64_t num_bytes = num_bytes_from_object + 1 + 2 * sizeof(uint64_t);

    uint8_t* const data_blob = malloc(num_bytes);

    uint64_t idx = 0;
    data_blob[idx] = isBigEndian();
    idx += 1;

    memcpy(&(data_blob[idx]), &kMagicNumber, sizeof(uint64_t));
    idx += sizeof(uint64_t);

    memcpy(&(data_blob[sizeof(uint64_t) + 1]), &num_bytes, sizeof(uint64_t));
    idx += sizeof(uint64_t);

    fillBufferWithHeader(hdr, &(data_blob[idx]));
    idx += num_bytes_hdr;

    memcpy(&(data_blob[idx]), x->data, num_bytes_one_matrix);
    idx += num_bytes_one_matrix;

    memcpy(&(data_blob[idx]), y->data, num_bytes_one_matrix);
    idx += num_bytes_one_matrix;

    memcpy(&(data_blob[idx]), z->data, num_bytes_one_matrix);
    idx += num_bytes_one_matrix;

    send_function(data_blob, num_bytes);

    free(data_blob);
}

void sendHeaderAndThreeVectors(SendFunction send_function,
                               const Vector* const x,
                               const Vector* const y,
                               const Vector* const z,
                               FunctionHeader* hdr)
{
    const uint64_t num_bytes_hdr = countNumHeaderBytes(hdr);
    const uint64_t num_bytes_one_vector = x->num_elements * x->num_bytes_per_element;

    const uint64_t num_bytes_from_object = 3 * num_bytes_one_vector + num_bytes_hdr;

    // + 1 bytes for endianness byte
    // + 2 * sizeof(uint64_t) for magic number and number of bytes in transfer
    const uint64_t num_bytes = num_bytes_from_object + 1 + 2 * sizeof(uint64_t);

    uint8_t* const data_blob = malloc(num_bytes);

    uint64_t idx = 0;
    data_blob[idx] = isBigEndian();
    idx += 1;

    memcpy(&(data_blob[idx]), &kMagicNumber, sizeof(uint64_t));
    idx += sizeof(uint64_t);

    memcpy(&(data_blob[sizeof(uint64_t) + 1]), &num_bytes, sizeof(uint64_t));
    idx += sizeof(uint64_t);

    fillBufferWithHeader(hdr, &(data_blob[idx]));
    idx += num_bytes_hdr;

    memcpy(&(data_blob[idx]), x->data, num_bytes_one_vector);
    idx += num_bytes_one_vector;

    memcpy(&(data_blob[idx]), y->data, num_bytes_one_vector);
    idx += num_bytes_one_vector;

    memcpy(&(data_blob[idx]), z->data, num_bytes_one_vector);
    idx += num_bytes_one_vector;

    send_function(data_blob, num_bytes);

    free(data_blob);
}

void sendHeader(SendFunction send_function, FunctionHeader* hdr)
{
    const uint64_t num_bytes_hdr = countNumHeaderBytes(hdr);

    // + 1 bytes for endianness byte
    // + 2 * sizeof(uint64_t) for magic number and number of bytes in transfer
    const uint64_t num_bytes = num_bytes_hdr + 1 + 2 * sizeof(uint64_t);

    uint8_t* const data_blob = malloc(num_bytes);

    uint64_t idx = 0;
    data_blob[idx] = isBigEndian();
    idx += 1;

    memcpy(&(data_blob[idx]), &kMagicNumber, sizeof(uint64_t));
    idx += sizeof(uint64_t);

    memcpy(&(data_blob[sizeof(uint64_t) + 1]), &num_bytes, sizeof(uint64_t));
    idx += sizeof(uint64_t);

    fillBufferWithHeader(hdr, &(data_blob[idx]));
    idx += num_bytes_hdr;

    send_function(data_blob, num_bytes);

    free(data_blob);
}

#endif
