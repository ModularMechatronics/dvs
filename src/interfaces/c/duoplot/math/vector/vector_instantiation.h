#ifndef DUOPLOT_MATH_VECTOR_VECTOR_INSTANTIATION_H_
#define DUOPLOT_MATH_VECTOR_VECTOR_INSTANTIATION_H_

#include "duoplot/math/utils.h"
#include "duoplot/math/vector/vector_types.h"

DUOPLOT_WEAK duoplot_Vector duoplot_createVector(const size_t num_elements, const duoplot_internal_DataType data_type)
{
    duoplot_Vector vec;
    vec.data_type = data_type;
    vec.num_bytes_per_element = duoplot_internal_dataTypeToNumBytes(data_type);
    vec.num_elements = num_elements;
    vec.data = (uint8_t*)malloc(num_elements * vec.num_bytes_per_element);

    return vec;
}

#define freeVector(vec) free(vec.data)

DUOPLOT_WEAK duoplot_VectorF duoplot_createVectorF(const size_t num_elements)
{
    duoplot_Vector vec = duoplot_createVector(num_elements, DUOPLOT_INTERNAL_DT_FLOAT);
    duoplot_VectorF ret_vec = *(duoplot_VectorF*)(&vec);

    return ret_vec;
}

DUOPLOT_WEAK duoplot_VectorD duoplot_createVectorD(const size_t num_elements)
{
    duoplot_Vector vec = duoplot_createVector(num_elements, DUOPLOT_INTERNAL_DT_DOUBLE);
    duoplot_VectorD ret_vec = *(duoplot_VectorD*)(&vec);

    return ret_vec;
}

DUOPLOT_WEAK duoplot_VectorS8 duoplot_createVectorS8(const size_t num_elements)
{
    duoplot_Vector vec = duoplot_createVector(num_elements, DUOPLOT_INTERNAL_DT_INT8);
    duoplot_VectorS8 ret_vec = *(duoplot_VectorS8*)(&vec);

    return ret_vec;
}

DUOPLOT_WEAK duoplot_VectorS16 duoplot_createVectorS16(const size_t num_elements)
{
    duoplot_Vector vec = duoplot_createVector(num_elements, DUOPLOT_INTERNAL_DT_INT16);
    duoplot_VectorS16 ret_vec = *(duoplot_VectorS16*)(&vec);

    return ret_vec;
}

DUOPLOT_WEAK duoplot_VectorS32 duoplot_createVectorS32(const size_t num_elements)
{
    duoplot_Vector vec = duoplot_createVector(num_elements, DUOPLOT_INTERNAL_DT_INT32);
    duoplot_VectorS32 ret_vec = *(duoplot_VectorS32*)(&vec);

    return ret_vec;
}

DUOPLOT_WEAK duoplot_VectorS64 duoplot_createVectorS64(const size_t num_elements)
{
    duoplot_Vector vec = duoplot_createVector(num_elements, DUOPLOT_INTERNAL_DT_INT64);
    duoplot_VectorS64 ret_vec = *(duoplot_VectorS64*)(&vec);

    return ret_vec;
}

DUOPLOT_WEAK duoplot_VectorU8 duoplot_createVectorU8(const size_t num_elements)
{
    duoplot_Vector vec = duoplot_createVector(num_elements, DUOPLOT_INTERNAL_DT_UINT8);
    duoplot_VectorU8 ret_vec = *(duoplot_VectorU8*)(&vec);

    return ret_vec;
}

DUOPLOT_WEAK duoplot_VectorU16 duoplot_createVectorU16(const size_t num_elements)
{
    duoplot_Vector vec = duoplot_createVector(num_elements, DUOPLOT_INTERNAL_DT_UINT16);
    duoplot_VectorU16 ret_vec = *(duoplot_VectorU16*)(&vec);

    return ret_vec;
}

DUOPLOT_WEAK duoplot_VectorU32 duoplot_createVectorU32(const size_t num_elements)
{
    duoplot_Vector vec = duoplot_createVector(num_elements, DUOPLOT_INTERNAL_DT_UINT32);
    duoplot_VectorU32 ret_vec = *(duoplot_VectorU32*)(&vec);

    return ret_vec;
}

DUOPLOT_WEAK duoplot_VectorU64 duoplot_createVectorU64(const size_t num_elements)
{
    duoplot_Vector vec = duoplot_createVector(num_elements, DUOPLOT_INTERNAL_DT_UINT64);
    duoplot_VectorU64 ret_vec = *(duoplot_VectorU64*)(&vec);

    return ret_vec;
}

DUOPLOT_WEAK duoplot_VectorF duoplot_toVectorF(const duoplot_Vector vec)
{
    duoplot_VectorF ret_vec = *(duoplot_VectorF*)(&vec);
    return ret_vec;
}

DUOPLOT_WEAK duoplot_VectorD duoplot_toVectorD(const duoplot_Vector vec)
{
    duoplot_VectorD ret_vec = *(duoplot_VectorD*)(&vec);
    return ret_vec;
}

DUOPLOT_WEAK duoplot_VectorS8 duoplot_toVectorS8(const duoplot_Vector vec)
{
    duoplot_VectorS8 ret_vec = *(duoplot_VectorS8*)(&vec);
    return ret_vec;
}

DUOPLOT_WEAK duoplot_VectorS16 duoplot_toVectorS16(const duoplot_Vector vec)
{
    duoplot_VectorS16 ret_vec = *(duoplot_VectorS16*)(&vec);
    return ret_vec;
}

DUOPLOT_WEAK duoplot_VectorS32 duoplot_toVectorS32(const duoplot_Vector vec)
{
    duoplot_VectorS32 ret_vec = *(duoplot_VectorS32*)(&vec);
    return ret_vec;
}

DUOPLOT_WEAK duoplot_VectorS64 duoplot_toVectorS64(const duoplot_Vector vec)
{
    duoplot_VectorS64 ret_vec = *(duoplot_VectorS64*)(&vec);
    return ret_vec;
}

DUOPLOT_WEAK duoplot_VectorU8 duoplot_toVectorU8(const duoplot_Vector vec)
{
    duoplot_VectorU8 ret_vec = *(duoplot_VectorU8*)(&vec);
    return ret_vec;
}

DUOPLOT_WEAK duoplot_VectorU16 duoplot_toVectorU16(const duoplot_Vector vec)
{
    duoplot_VectorU16 ret_vec = *(duoplot_VectorU16*)(&vec);
    return ret_vec;
}

DUOPLOT_WEAK duoplot_VectorU32 duoplot_toVectorU32(const duoplot_Vector vec)
{
    duoplot_VectorU32 ret_vec = *(duoplot_VectorU32*)(&vec);
    return ret_vec;
}

DUOPLOT_WEAK duoplot_VectorU64 duoplot_toVectorU64(const duoplot_Vector vec)
{
    duoplot_VectorU64 ret_vec = *(duoplot_VectorU64*)(&vec);
    return ret_vec;
}

#endif  // DUOPLOT_MATH_VECTOR_VECTOR_INSTANTIATION_H_
