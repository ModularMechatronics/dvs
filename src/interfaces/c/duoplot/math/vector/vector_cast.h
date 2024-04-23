#ifndef DUOPLOT_MATH_VECTOR_VECTOR_CAST_H_
#define DUOPLOT_MATH_VECTOR_VECTOR_CAST_H_

#include <stdint.h>

#include "duoplot/math/vector/vector_instantiation.h"
#include "duoplot/math/vector/vector_types.h"
#include "duoplot/pp.h"

DUOPLOT_WEAK duoplot_VectorD duoplot_castVectorFToVectorD(const duoplot_VectorF input)
{
    duoplot_VectorD output = duoplot_createVectorD(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorS8 duoplot_castVectorFToVectorS8(const duoplot_VectorF input)
{
    duoplot_VectorS8 output = duoplot_createVectorS8(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorS16 duoplot_castVectorFToVectorS16(const duoplot_VectorF input)
{
    duoplot_VectorS16 output = duoplot_createVectorS16(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorS32 duoplot_castVectorFToVectorS32(const duoplot_VectorF input)
{
    duoplot_VectorS32 output = duoplot_createVectorS32(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorS64 duoplot_castVectorFToVectorS64(const duoplot_VectorF input)
{
    duoplot_VectorS64 output = duoplot_createVectorS64(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorU8 duoplot_castVectorFToVectorU8(const duoplot_VectorF input)
{
    duoplot_VectorU8 output = duoplot_createVectorU8(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorU16 duoplot_castVectorFToVectorU16(const duoplot_VectorF input)
{
    duoplot_VectorU16 output = duoplot_createVectorU16(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorU32 duoplot_castVectorFToVectorU32(const duoplot_VectorF input)
{
    duoplot_VectorU32 output = duoplot_createVectorU32(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorU64 duoplot_castVectorFToVectorU64(const duoplot_VectorF input)
{
    duoplot_VectorU64 output = duoplot_createVectorU64(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorF duoplot_castVectorDToVectorF(const duoplot_VectorD input)
{
    duoplot_VectorF output = duoplot_createVectorF(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorS8 duoplot_castVectorDToVectorS8(const duoplot_VectorD input)
{
    duoplot_VectorS8 output = duoplot_createVectorS8(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorS16 duoplot_castVectorDToVectorS16(const duoplot_VectorD input)
{
    duoplot_VectorS16 output = duoplot_createVectorS16(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorS32 duoplot_castVectorDToVectorS32(const duoplot_VectorD input)
{
    duoplot_VectorS32 output = duoplot_createVectorS32(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorS64 duoplot_castVectorDToVectorS64(const duoplot_VectorD input)
{
    duoplot_VectorS64 output = duoplot_createVectorS64(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorU8 duoplot_castVectorDToVectorU8(const duoplot_VectorD input)
{
    duoplot_VectorU8 output = duoplot_createVectorU8(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorU16 duoplot_castVectorDToVectorU16(const duoplot_VectorD input)
{
    duoplot_VectorU16 output = duoplot_createVectorU16(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorU32 duoplot_castVectorDToVectorU32(const duoplot_VectorD input)
{
    duoplot_VectorU32 output = duoplot_createVectorU32(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorU64 duoplot_castVectorDToVectorU64(const duoplot_VectorD input)
{
    duoplot_VectorU64 output = duoplot_createVectorU64(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorF duoplot_castVectorS8ToVectorF(const duoplot_VectorS8 input)
{
    duoplot_VectorF output = duoplot_createVectorF(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorD duoplot_castVectorS8ToVectorD(const duoplot_VectorS8 input)
{
    duoplot_VectorD output = duoplot_createVectorD(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorS16 duoplot_castVectorS8ToVectorS16(const duoplot_VectorS8 input)
{
    duoplot_VectorS16 output = duoplot_createVectorS16(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorS32 duoplot_castVectorS8ToVectorS32(const duoplot_VectorS8 input)
{
    duoplot_VectorS32 output = duoplot_createVectorS32(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorS64 duoplot_castVectorS8ToVectorS64(const duoplot_VectorS8 input)
{
    duoplot_VectorS64 output = duoplot_createVectorS64(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorU8 duoplot_castVectorS8ToVectorU8(const duoplot_VectorS8 input)
{
    duoplot_VectorU8 output = duoplot_createVectorU8(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorU16 duoplot_castVectorS8ToVectorU16(const duoplot_VectorS8 input)
{
    duoplot_VectorU16 output = duoplot_createVectorU16(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorU32 duoplot_castVectorS8ToVectorU32(const duoplot_VectorS8 input)
{
    duoplot_VectorU32 output = duoplot_createVectorU32(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorU64 duoplot_castVectorS8ToVectorU64(const duoplot_VectorS8 input)
{
    duoplot_VectorU64 output = duoplot_createVectorU64(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorF duoplot_castVectorS16ToVectorF(const duoplot_VectorS16 input)
{
    duoplot_VectorF output = duoplot_createVectorF(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorD duoplot_castVectorS16ToVectorD(const duoplot_VectorS16 input)
{
    duoplot_VectorD output = duoplot_createVectorD(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorS8 duoplot_castVectorS16ToVectorS8(const duoplot_VectorS16 input)
{
    duoplot_VectorS8 output = duoplot_createVectorS8(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorS32 duoplot_castVectorS16ToVectorS32(const duoplot_VectorS16 input)
{
    duoplot_VectorS32 output = duoplot_createVectorS32(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorS64 duoplot_castVectorS16ToVectorS64(const duoplot_VectorS16 input)
{
    duoplot_VectorS64 output = duoplot_createVectorS64(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorU8 duoplot_castVectorS16ToVectorU8(const duoplot_VectorS16 input)
{
    duoplot_VectorU8 output = duoplot_createVectorU8(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorU16 duoplot_castVectorS16ToVectorU16(const duoplot_VectorS16 input)
{
    duoplot_VectorU16 output = duoplot_createVectorU16(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorU32 duoplot_castVectorS16ToVectorU32(const duoplot_VectorS16 input)
{
    duoplot_VectorU32 output = duoplot_createVectorU32(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorU64 duoplot_castVectorS16ToVectorU64(const duoplot_VectorS16 input)
{
    duoplot_VectorU64 output = duoplot_createVectorU64(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorF duoplot_castVectorS32ToVectorF(const duoplot_VectorS32 input)
{
    duoplot_VectorF output = duoplot_createVectorF(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorD duoplot_castVectorS32ToVectorD(const duoplot_VectorS32 input)
{
    duoplot_VectorD output = duoplot_createVectorD(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorS8 duoplot_castVectorS32ToVectorS8(const duoplot_VectorS32 input)
{
    duoplot_VectorS8 output = duoplot_createVectorS8(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorS16 duoplot_castVectorS32ToVectorS16(const duoplot_VectorS32 input)
{
    duoplot_VectorS16 output = duoplot_createVectorS16(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorS64 duoplot_castVectorS32ToVectorS64(const duoplot_VectorS32 input)
{
    duoplot_VectorS64 output = duoplot_createVectorS64(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorU8 duoplot_castVectorS32ToVectorU8(const duoplot_VectorS32 input)
{
    duoplot_VectorU8 output = duoplot_createVectorU8(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorU16 duoplot_castVectorS32ToVectorU16(const duoplot_VectorS32 input)
{
    duoplot_VectorU16 output = duoplot_createVectorU16(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorU32 duoplot_castVectorS32ToVectorU32(const duoplot_VectorS32 input)
{
    duoplot_VectorU32 output = duoplot_createVectorU32(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorU64 duoplot_castVectorS32ToVectorU64(const duoplot_VectorS32 input)
{
    duoplot_VectorU64 output = duoplot_createVectorU64(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorF duoplot_castVectorS64ToVectorF(const duoplot_VectorS64 input)
{
    duoplot_VectorF output = duoplot_createVectorF(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorD duoplot_castVectorS64ToVectorD(const duoplot_VectorS64 input)
{
    duoplot_VectorD output = duoplot_createVectorD(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorS8 duoplot_castVectorS64ToVectorS8(const duoplot_VectorS64 input)
{
    duoplot_VectorS8 output = duoplot_createVectorS8(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorS16 duoplot_castVectorS64ToVectorS16(const duoplot_VectorS64 input)
{
    duoplot_VectorS16 output = duoplot_createVectorS16(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorS32 duoplot_castVectorS64ToVectorS32(const duoplot_VectorS64 input)
{
    duoplot_VectorS32 output = duoplot_createVectorS32(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorU8 duoplot_castVectorS64ToVectorU8(const duoplot_VectorS64 input)
{
    duoplot_VectorU8 output = duoplot_createVectorU8(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorU16 duoplot_castVectorS64ToVectorU16(const duoplot_VectorS64 input)
{
    duoplot_VectorU16 output = duoplot_createVectorU16(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorU32 duoplot_castVectorS64ToVectorU32(const duoplot_VectorS64 input)
{
    duoplot_VectorU32 output = duoplot_createVectorU32(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorU64 duoplot_castVectorS64ToVectorU64(const duoplot_VectorS64 input)
{
    duoplot_VectorU64 output = duoplot_createVectorU64(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorF duoplot_castVectorU8ToVectorF(const duoplot_VectorU8 input)
{
    duoplot_VectorF output = duoplot_createVectorF(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorD duoplot_castVectorU8ToVectorD(const duoplot_VectorU8 input)
{
    duoplot_VectorD output = duoplot_createVectorD(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorS8 duoplot_castVectorU8ToVectorS8(const duoplot_VectorU8 input)
{
    duoplot_VectorS8 output = duoplot_createVectorS8(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorS16 duoplot_castVectorU8ToVectorS16(const duoplot_VectorU8 input)
{
    duoplot_VectorS16 output = duoplot_createVectorS16(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorS32 duoplot_castVectorU8ToVectorS32(const duoplot_VectorU8 input)
{
    duoplot_VectorS32 output = duoplot_createVectorS32(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorS64 duoplot_castVectorU8ToVectorS64(const duoplot_VectorU8 input)
{
    duoplot_VectorS64 output = duoplot_createVectorS64(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorU16 duoplot_castVectorU8ToVectorU16(const duoplot_VectorU8 input)
{
    duoplot_VectorU16 output = duoplot_createVectorU16(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorU32 duoplot_castVectorU8ToVectorU32(const duoplot_VectorU8 input)
{
    duoplot_VectorU32 output = duoplot_createVectorU32(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorU64 duoplot_castVectorU8ToVectorU64(const duoplot_VectorU8 input)
{
    duoplot_VectorU64 output = duoplot_createVectorU64(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorF duoplot_castVectorU16ToVectorF(const duoplot_VectorU16 input)
{
    duoplot_VectorF output = duoplot_createVectorF(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorD duoplot_castVectorU16ToVectorD(const duoplot_VectorU16 input)
{
    duoplot_VectorD output = duoplot_createVectorD(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorS8 duoplot_castVectorU16ToVectorS8(const duoplot_VectorU16 input)
{
    duoplot_VectorS8 output = duoplot_createVectorS8(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorS16 duoplot_castVectorU16ToVectorS16(const duoplot_VectorU16 input)
{
    duoplot_VectorS16 output = duoplot_createVectorS16(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorS32 duoplot_castVectorU16ToVectorS32(const duoplot_VectorU16 input)
{
    duoplot_VectorS32 output = duoplot_createVectorS32(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorS64 duoplot_castVectorU16ToVectorS64(const duoplot_VectorU16 input)
{
    duoplot_VectorS64 output = duoplot_createVectorS64(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorU8 duoplot_castVectorU16ToVectorU8(const duoplot_VectorU16 input)
{
    duoplot_VectorU8 output = duoplot_createVectorU8(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorU32 duoplot_castVectorU16ToVectorU32(const duoplot_VectorU16 input)
{
    duoplot_VectorU32 output = duoplot_createVectorU32(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorU64 duoplot_castVectorU16ToVectorU64(const duoplot_VectorU16 input)
{
    duoplot_VectorU64 output = duoplot_createVectorU64(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorF duoplot_castVectorU32ToVectorF(const duoplot_VectorU32 input)
{
    duoplot_VectorF output = duoplot_createVectorF(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorD duoplot_castVectorU32ToVectorD(const duoplot_VectorU32 input)
{
    duoplot_VectorD output = duoplot_createVectorD(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorS8 duoplot_castVectorU32ToVectorS8(const duoplot_VectorU32 input)
{
    duoplot_VectorS8 output = duoplot_createVectorS8(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorS16 duoplot_castVectorU32ToVectorS16(const duoplot_VectorU32 input)
{
    duoplot_VectorS16 output = duoplot_createVectorS16(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorS32 duoplot_castVectorU32ToVectorS32(const duoplot_VectorU32 input)
{
    duoplot_VectorS32 output = duoplot_createVectorS32(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorS64 duoplot_castVectorU32ToVectorS64(const duoplot_VectorU32 input)
{
    duoplot_VectorS64 output = duoplot_createVectorS64(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorU8 duoplot_castVectorU32ToVectorU8(const duoplot_VectorU32 input)
{
    duoplot_VectorU8 output = duoplot_createVectorU8(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorU16 duoplot_castVectorU32ToVectorU16(const duoplot_VectorU32 input)
{
    duoplot_VectorU16 output = duoplot_createVectorU16(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorU64 duoplot_castVectorU32ToVectorU64(const duoplot_VectorU32 input)
{
    duoplot_VectorU64 output = duoplot_createVectorU64(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorF duoplot_castVectorU64ToVectorF(const duoplot_VectorU64 input)
{
    duoplot_VectorF output = duoplot_createVectorF(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorD duoplot_castVectorU64ToVectorD(const duoplot_VectorU64 input)
{
    duoplot_VectorD output = duoplot_createVectorD(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorS8 duoplot_castVectorU64ToVectorS8(const duoplot_VectorU64 input)
{
    duoplot_VectorS8 output = duoplot_createVectorS8(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorS16 duoplot_castVectorU64ToVectorS16(const duoplot_VectorU64 input)
{
    duoplot_VectorS16 output = duoplot_createVectorS16(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorS32 duoplot_castVectorU64ToVectorS32(const duoplot_VectorU64 input)
{
    duoplot_VectorS32 output = duoplot_createVectorS32(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorS64 duoplot_castVectorU64ToVectorS64(const duoplot_VectorU64 input)
{
    duoplot_VectorS64 output = duoplot_createVectorS64(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorU8 duoplot_castVectorU64ToVectorU8(const duoplot_VectorU64 input)
{
    duoplot_VectorU8 output = duoplot_createVectorU8(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorU16 duoplot_castVectorU64ToVectorU16(const duoplot_VectorU64 input)
{
    duoplot_VectorU16 output = duoplot_createVectorU16(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

DUOPLOT_WEAK duoplot_VectorU32 duoplot_castVectorU64ToVectorU32(const duoplot_VectorU64 input)
{
    duoplot_VectorU32 output = duoplot_createVectorU32(input.num_elements);
    for (size_t k = 0; k < input.num_elements; k++)
    {
        output.data[k] = input.data[k];
    }

    return output;
}

#endif  // DUOPLOT_MATH_VECTOR_VECTOR_CAST_H_
