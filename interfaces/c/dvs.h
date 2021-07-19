#ifndef DVS_H_
#define DVS_H_

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "function_header.h"
#include "internal.h"
#include "math/math.h"

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

    memcpy(&(data_blob[idx]), &magic_num, sizeof(uint64_t));
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

void plotInternal(const Vector* const x, const Vector* const y, const FunctionHeaderObject first_prop, ...)
{
    FunctionHeader hdr;
    initFunctionHeader(&hdr);

    APPEND_VAL(&hdr, FHOT_FUNCTION, F_PLOT2, uint8_t);
    APPEND_VAL(&hdr, FHOT_DATA_TYPE, x->data_type, uint8_t);
    APPEND_VAL(&hdr, FHOT_NUM_ELEMENTS, x->num_elements, uint32_t);

    APPEND_PROPERTIES(hdr, first_prop);

    sendHeaderAndTwoVectors(getSendFunction(), x, y, &hdr);

    // printf("Num properties: %i\n", hdr.num_objects);
}

// Use as plot(const Vector x, const Vector y, Property p0, Property p1...)
#define plot(x, y, ...) plotInternal((Vector*)&x, (Vector*)&y, __VA_ARGS__, getLastFHO())

#endif
