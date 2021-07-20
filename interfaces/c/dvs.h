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

    memcpy(&(data_blob[idx]), &magic_num, sizeof(uint64_t));
    idx += sizeof(uint64_t);

    memcpy(&(data_blob[sizeof(uint64_t) + 1]), &num_bytes, sizeof(uint64_t));
    idx += sizeof(uint64_t);

    fillBufferWithHeader(hdr, &(data_blob[idx]));
    idx += num_bytes_hdr;

    send_function(data_blob, num_bytes);

    free(data_blob);
}

void plotFunction3D(const Vector* const x,
                    const Vector* const y,
                    const Vector* const z,
                    const Function fcn,
                    const FunctionHeaderObject first_prop,
                    ...)
{
    FunctionHeader hdr;
    initFunctionHeader(&hdr);

    APPEND_VAL(&hdr, FHOT_FUNCTION, fcn, uint8_t);
    APPEND_VAL(&hdr, FHOT_DATA_TYPE, x->data_type, uint8_t);
    APPEND_VAL(&hdr, FHOT_NUM_ELEMENTS, x->num_elements, uint32_t);

    APPEND_PROPERTIES(hdr, first_prop);

    sendHeaderAndThreeVectors(getSendFunction(), x, y, z, &hdr);
}

void plotFunction2D(
    const Vector* const x, const Vector* const y, const Function fcn, const FunctionHeaderObject first_prop, ...)
{
    FunctionHeader hdr;
    initFunctionHeader(&hdr);

    APPEND_VAL(&hdr, FHOT_FUNCTION, fcn, uint8_t);
    APPEND_VAL(&hdr, FHOT_DATA_TYPE, x->data_type, uint8_t);
    APPEND_VAL(&hdr, FHOT_NUM_ELEMENTS, x->num_elements, uint32_t);

    APPEND_PROPERTIES(hdr, first_prop);

    sendHeaderAndTwoVectors(getSendFunction(), x, y, &hdr);
}

#define plot(x, y, ...) plotFunction2D((Vector*)&x, (Vector*)&y, F_PLOT2, __VA_ARGS__, getLastFHO())

#define scatter(x, y, ...) plotFunction2D((Vector*)&x, (Vector*)&y, F_SCATTER2, __VA_ARGS__, getLastFHO())

#define plot3(x, y, z, ...) plotFunction3D((Vector*)&x, (Vector*)&y, (Vector*)&z, F_PLOT3, __VA_ARGS__, getLastFHO())

#define scatter3(x, y, z, ...) \
    plotFunction3D((Vector*)&x, (Vector*)&y, (Vector*)&z, F_SCATTER3, __VA_ARGS__, getLastFHO())

void setCurrentElement(const char* name)
{
    FunctionHeader hdr;
    initFunctionHeader(&hdr);
    const size_t kNumBytesForName = 23;

    APPEND_VAL(&hdr, FHOT_FUNCTION, F_SET_CURRENT_ELEMENT, uint8_t);
    hdr.values[hdr.num_objects].type = FHOT_ELEMENT_NAME;
    hdr.values[hdr.num_objects].num_bytes = 1 + sizeof(size_t) + kNumBytesForName;
    hdr.values[hdr.num_objects].data[0] = (uint8_t)PT_NAME;

    size_t tmp_var = kNumBytesForName;
    memcpy(&(hdr.values[hdr.num_objects].data[1]), &tmp_var, sizeof(size_t));
    memcpy(&(hdr.values[hdr.num_objects].data[1 + sizeof(size_t)]), name, kNumBytesForName);

    hdr.num_objects += 1;

    sendHeader(getSendFunction(), &hdr);
}

void clearView()
{
    FunctionHeader hdr;
    initFunctionHeader(&hdr);
    APPEND_VAL(&hdr, FHOT_FUNCTION, F_CLEAR, uint8_t);

    sendHeader(getSendFunction(), &hdr);
}

void softClearView()
{
    FunctionHeader hdr;
    initFunctionHeader(&hdr);
    APPEND_VAL(&hdr, FHOT_FUNCTION, F_SOFT_CLEAR, uint8_t);

    sendHeader(getSendFunction(), &hdr);
}

void holdOn()
{
    FunctionHeader hdr;
    initFunctionHeader(&hdr);
    APPEND_VAL(&hdr, FHOT_FUNCTION, F_HOLD_ON, uint8_t);

    sendHeader(getSendFunction(), &hdr);
}

void holdOff()
{
    FunctionHeader hdr;
    initFunctionHeader(&hdr);
    APPEND_VAL(&hdr, FHOT_FUNCTION, F_HOLD_OFF, uint8_t);

    sendHeader(getSendFunction(), &hdr);
}

void view(const float azimuth, const float elevation)
{
    FunctionHeader hdr;
    initFunctionHeader(&hdr);
    APPEND_VAL(&hdr, FHOT_FUNCTION, F_VIEW, uint8_t);
    APPEND_VAL(&hdr, FHOT_AZIMUTH, azimuth, float);
    APPEND_VAL(&hdr, FHOT_ELEVATION, elevation, float);

    sendHeader(getSendFunction(), &hdr);
}

void axis(const Vec3DD min_bound, const Vec3DD max_bound)
{
    FunctionHeader hdr;
    initFunctionHeader(&hdr);
    APPEND_VAL(&hdr, FHOT_FUNCTION, F_AXES_3D, uint8_t);

    typedef struct S_Bnd3D
    {
        Vec3DD min_bnd;
        Vec3DD max_bnd;
    } Bnd3D;

    Bnd3D bnd = {min_bound, max_bound};

    APPEND_VAL(&hdr, FHOT_AXIS_MIN_MAX_VEC, bnd, Bnd3D);

    sendHeader(getSendFunction(), &hdr);
}

void axis2D(const Vec2DD min_bound, const Vec2DD max_bound)
{
    FunctionHeader hdr;
    initFunctionHeader(&hdr);
    APPEND_VAL(&hdr, FHOT_FUNCTION, F_AXES_3D, uint8_t);

    typedef struct S_Bnd3D
    {
        Vec3DD min_bnd;
        Vec3DD max_bnd;
    } Bnd3D;
    Vec3DD v0 = {min_bound.x, min_bound.y, 0.0};
    Vec3DD v1 = {max_bound.x, max_bound.y, 0.0};

    Bnd3D bnd = {v0, v1};

    APPEND_VAL(&hdr, FHOT_AXIS_MIN_MAX_VEC, bnd, Bnd3D);

    sendHeader(getSendFunction(), &hdr);
}

#endif
