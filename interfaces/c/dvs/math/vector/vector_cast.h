#ifndef DVS_VECTOR_CAST_H_
#define DVS_VECTOR_CAST_H_

#include <stdint.h>

#include "dvs/math/vector/vector_instantiation.h"
#include "dvs/math/vector/vector_types.h"

VectorD castVectorFToVectorD(const VectorF input)
{
    VectorD output = createVectorD(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorS8 castVectorFToVectorS8(const VectorF input)
{
    VectorS8 output = createVectorS8(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorS16 castVectorFToVectorS16(const VectorF input)
{
    VectorS16 output = createVectorS16(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorS32 castVectorFToVectorS32(const VectorF input)
{
    VectorS32 output = createVectorS32(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorS64 castVectorFToVectorS64(const VectorF input)
{
    VectorS64 output = createVectorS64(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorU8 castVectorFToVectorU8(const VectorF input)
{
    VectorU8 output = createVectorU8(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorU16 castVectorFToVectorU16(const VectorF input)
{
    VectorU16 output = createVectorU16(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorU32 castVectorFToVectorU32(const VectorF input)
{
    VectorU32 output = createVectorU32(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorU64 castVectorFToVectorU64(const VectorF input)
{
    VectorU64 output = createVectorU64(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorF castVectorDToVectorF(const VectorD input)
{
    VectorF output = createVectorF(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorS8 castVectorDToVectorS8(const VectorD input)
{
    VectorS8 output = createVectorS8(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorS16 castVectorDToVectorS16(const VectorD input)
{
    VectorS16 output = createVectorS16(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorS32 castVectorDToVectorS32(const VectorD input)
{
    VectorS32 output = createVectorS32(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorS64 castVectorDToVectorS64(const VectorD input)
{
    VectorS64 output = createVectorS64(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorU8 castVectorDToVectorU8(const VectorD input)
{
    VectorU8 output = createVectorU8(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorU16 castVectorDToVectorU16(const VectorD input)
{
    VectorU16 output = createVectorU16(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorU32 castVectorDToVectorU32(const VectorD input)
{
    VectorU32 output = createVectorU32(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorU64 castVectorDToVectorU64(const VectorD input)
{
    VectorU64 output = createVectorU64(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorF castVectorS8ToVectorF(const VectorS8 input)
{
    VectorF output = createVectorF(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorD castVectorS8ToVectorD(const VectorS8 input)
{
    VectorD output = createVectorD(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorS16 castVectorS8ToVectorS16(const VectorS8 input)
{
    VectorS16 output = createVectorS16(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorS32 castVectorS8ToVectorS32(const VectorS8 input)
{
    VectorS32 output = createVectorS32(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorS64 castVectorS8ToVectorS64(const VectorS8 input)
{
    VectorS64 output = createVectorS64(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorU8 castVectorS8ToVectorU8(const VectorS8 input)
{
    VectorU8 output = createVectorU8(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorU16 castVectorS8ToVectorU16(const VectorS8 input)
{
    VectorU16 output = createVectorU16(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorU32 castVectorS8ToVectorU32(const VectorS8 input)
{
    VectorU32 output = createVectorU32(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorU64 castVectorS8ToVectorU64(const VectorS8 input)
{
    VectorU64 output = createVectorU64(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorF castVectorS16ToVectorF(const VectorS16 input)
{
    VectorF output = createVectorF(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorD castVectorS16ToVectorD(const VectorS16 input)
{
    VectorD output = createVectorD(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorS8 castVectorS16ToVectorS8(const VectorS16 input)
{
    VectorS8 output = createVectorS8(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorS32 castVectorS16ToVectorS32(const VectorS16 input)
{
    VectorS32 output = createVectorS32(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorS64 castVectorS16ToVectorS64(const VectorS16 input)
{
    VectorS64 output = createVectorS64(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorU8 castVectorS16ToVectorU8(const VectorS16 input)
{
    VectorU8 output = createVectorU8(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorU16 castVectorS16ToVectorU16(const VectorS16 input)
{
    VectorU16 output = createVectorU16(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorU32 castVectorS16ToVectorU32(const VectorS16 input)
{
    VectorU32 output = createVectorU32(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorU64 castVectorS16ToVectorU64(const VectorS16 input)
{
    VectorU64 output = createVectorU64(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorF castVectorS32ToVectorF(const VectorS32 input)
{
    VectorF output = createVectorF(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorD castVectorS32ToVectorD(const VectorS32 input)
{
    VectorD output = createVectorD(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorS8 castVectorS32ToVectorS8(const VectorS32 input)
{
    VectorS8 output = createVectorS8(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorS16 castVectorS32ToVectorS16(const VectorS32 input)
{
    VectorS16 output = createVectorS16(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorS64 castVectorS32ToVectorS64(const VectorS32 input)
{
    VectorS64 output = createVectorS64(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorU8 castVectorS32ToVectorU8(const VectorS32 input)
{
    VectorU8 output = createVectorU8(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorU16 castVectorS32ToVectorU16(const VectorS32 input)
{
    VectorU16 output = createVectorU16(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorU32 castVectorS32ToVectorU32(const VectorS32 input)
{
    VectorU32 output = createVectorU32(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorU64 castVectorS32ToVectorU64(const VectorS32 input)
{
    VectorU64 output = createVectorU64(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorF castVectorS64ToVectorF(const VectorS64 input)
{
    VectorF output = createVectorF(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorD castVectorS64ToVectorD(const VectorS64 input)
{
    VectorD output = createVectorD(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorS8 castVectorS64ToVectorS8(const VectorS64 input)
{
    VectorS8 output = createVectorS8(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorS16 castVectorS64ToVectorS16(const VectorS64 input)
{
    VectorS16 output = createVectorS16(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorS32 castVectorS64ToVectorS32(const VectorS64 input)
{
    VectorS32 output = createVectorS32(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorU8 castVectorS64ToVectorU8(const VectorS64 input)
{
    VectorU8 output = createVectorU8(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorU16 castVectorS64ToVectorU16(const VectorS64 input)
{
    VectorU16 output = createVectorU16(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorU32 castVectorS64ToVectorU32(const VectorS64 input)
{
    VectorU32 output = createVectorU32(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorU64 castVectorS64ToVectorU64(const VectorS64 input)
{
    VectorU64 output = createVectorU64(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorF castVectorU8ToVectorF(const VectorU8 input)
{
    VectorF output = createVectorF(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorD castVectorU8ToVectorD(const VectorU8 input)
{
    VectorD output = createVectorD(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorS8 castVectorU8ToVectorS8(const VectorU8 input)
{
    VectorS8 output = createVectorS8(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorS16 castVectorU8ToVectorS16(const VectorU8 input)
{
    VectorS16 output = createVectorS16(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorS32 castVectorU8ToVectorS32(const VectorU8 input)
{
    VectorS32 output = createVectorS32(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorS64 castVectorU8ToVectorS64(const VectorU8 input)
{
    VectorS64 output = createVectorS64(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorU16 castVectorU8ToVectorU16(const VectorU8 input)
{
    VectorU16 output = createVectorU16(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorU32 castVectorU8ToVectorU32(const VectorU8 input)
{
    VectorU32 output = createVectorU32(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorU64 castVectorU8ToVectorU64(const VectorU8 input)
{
    VectorU64 output = createVectorU64(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorF castVectorU16ToVectorF(const VectorU16 input)
{
    VectorF output = createVectorF(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorD castVectorU16ToVectorD(const VectorU16 input)
{
    VectorD output = createVectorD(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorS8 castVectorU16ToVectorS8(const VectorU16 input)
{
    VectorS8 output = createVectorS8(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorS16 castVectorU16ToVectorS16(const VectorU16 input)
{
    VectorS16 output = createVectorS16(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorS32 castVectorU16ToVectorS32(const VectorU16 input)
{
    VectorS32 output = createVectorS32(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorS64 castVectorU16ToVectorS64(const VectorU16 input)
{
    VectorS64 output = createVectorS64(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorU8 castVectorU16ToVectorU8(const VectorU16 input)
{
    VectorU8 output = createVectorU8(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorU32 castVectorU16ToVectorU32(const VectorU16 input)
{
    VectorU32 output = createVectorU32(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorU64 castVectorU16ToVectorU64(const VectorU16 input)
{
    VectorU64 output = createVectorU64(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorF castVectorU32ToVectorF(const VectorU32 input)
{
    VectorF output = createVectorF(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorD castVectorU32ToVectorD(const VectorU32 input)
{
    VectorD output = createVectorD(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorS8 castVectorU32ToVectorS8(const VectorU32 input)
{
    VectorS8 output = createVectorS8(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorS16 castVectorU32ToVectorS16(const VectorU32 input)
{
    VectorS16 output = createVectorS16(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorS32 castVectorU32ToVectorS32(const VectorU32 input)
{
    VectorS32 output = createVectorS32(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorS64 castVectorU32ToVectorS64(const VectorU32 input)
{
    VectorS64 output = createVectorS64(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorU8 castVectorU32ToVectorU8(const VectorU32 input)
{
    VectorU8 output = createVectorU8(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorU16 castVectorU32ToVectorU16(const VectorU32 input)
{
    VectorU16 output = createVectorU16(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorU64 castVectorU32ToVectorU64(const VectorU32 input)
{
    VectorU64 output = createVectorU64(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorF castVectorU64ToVectorF(const VectorU64 input)
{
    VectorF output = createVectorF(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorD castVectorU64ToVectorD(const VectorU64 input)
{
    VectorD output = createVectorD(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorS8 castVectorU64ToVectorS8(const VectorU64 input)
{
    VectorS8 output = createVectorS8(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorS16 castVectorU64ToVectorS16(const VectorU64 input)
{
    VectorS16 output = createVectorS16(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorS32 castVectorU64ToVectorS32(const VectorU64 input)
{
    VectorS32 output = createVectorS32(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorS64 castVectorU64ToVectorS64(const VectorU64 input)
{
    VectorS64 output = createVectorS64(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorU8 castVectorU64ToVectorU8(const VectorU64 input)
{
    VectorU8 output = createVectorU8(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorU16 castVectorU64ToVectorU16(const VectorU64 input)
{
    VectorU16 output = createVectorU16(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

VectorU32 castVectorU64ToVectorU32(const VectorU64 input)
{
    VectorU32 output = createVectorU32(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

#endif
