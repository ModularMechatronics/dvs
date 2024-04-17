#ifndef DUOPLOT_MATH_VECTOR_VECTOR_INSTANTIATION_H_
#define DUOPLOT_MATH_VECTOR_VECTOR_INSTANTIATION_H_

#include "duoplot/math/utils.h"
#include "duoplot/math/vector/vector_types.h"

Vector createVector(const size_t num_elements, const DataType data_type)
{
    Vector vec;
    vec.data_type = data_type;
    vec.num_bytes_per_element = dataTypeToNumBytes(data_type);
    vec.num_elements = num_elements;
    vec.data = (uint8_t*)malloc(num_elements * vec.num_bytes_per_element);

    return vec;
}

#define freeVector(vec) free(vec.data)

VectorF createVectorF(const size_t num_elements)
{
    Vector vec = createVector(num_elements, DT_FLOAT);
    VectorF ret_vec = *(VectorF*)(&vec);

    return ret_vec;
}

VectorD createVectorD(const size_t num_elements)
{
    Vector vec = createVector(num_elements, DT_DOUBLE);
    VectorD ret_vec = *(VectorD*)(&vec);

    return ret_vec;
}

VectorS8 createVectorS8(const size_t num_elements)
{
    Vector vec = createVector(num_elements, DT_INT8);
    VectorS8 ret_vec = *(VectorS8*)(&vec);

    return ret_vec;
}

VectorS16 createVectorS16(const size_t num_elements)
{
    Vector vec = createVector(num_elements, DT_INT16);
    VectorS16 ret_vec = *(VectorS16*)(&vec);

    return ret_vec;
}

VectorS32 createVectorS32(const size_t num_elements)
{
    Vector vec = createVector(num_elements, DT_INT32);
    VectorS32 ret_vec = *(VectorS32*)(&vec);

    return ret_vec;
}

VectorS64 createVectorS64(const size_t num_elements)
{
    Vector vec = createVector(num_elements, DT_INT64);
    VectorS64 ret_vec = *(VectorS64*)(&vec);

    return ret_vec;
}

VectorU8 createVectorU8(const size_t num_elements)
{
    Vector vec = createVector(num_elements, DT_UINT8);
    VectorU8 ret_vec = *(VectorU8*)(&vec);

    return ret_vec;
}

VectorU16 createVectorU16(const size_t num_elements)
{
    Vector vec = createVector(num_elements, DT_UINT16);
    VectorU16 ret_vec = *(VectorU16*)(&vec);

    return ret_vec;
}

VectorU32 createVectorU32(const size_t num_elements)
{
    Vector vec = createVector(num_elements, DT_UINT32);
    VectorU32 ret_vec = *(VectorU32*)(&vec);

    return ret_vec;
}

VectorU64 createVectorU64(const size_t num_elements)
{
    Vector vec = createVector(num_elements, DT_UINT64);
    VectorU64 ret_vec = *(VectorU64*)(&vec);

    return ret_vec;
}

VectorF toVectorF(const Vector vec)
{
    VectorF ret_vec = *(VectorF*)(&vec);
    return ret_vec;
}

VectorD toVectorD(const Vector vec)
{
    VectorD ret_vec = *(VectorD*)(&vec);
    return ret_vec;
}

VectorS8 toVectorS8(const Vector vec)
{
    VectorS8 ret_vec = *(VectorS8*)(&vec);
    return ret_vec;
}

VectorS16 toVectorS16(const Vector vec)
{
    VectorS16 ret_vec = *(VectorS16*)(&vec);
    return ret_vec;
}

VectorS32 toVectorS32(const Vector vec)
{
    VectorS32 ret_vec = *(VectorS32*)(&vec);
    return ret_vec;
}

VectorS64 toVectorS64(const Vector vec)
{
    VectorS64 ret_vec = *(VectorS64*)(&vec);
    return ret_vec;
}

VectorU8 toVectorU8(const Vector vec)
{
    VectorU8 ret_vec = *(VectorU8*)(&vec);
    return ret_vec;
}

VectorU16 toVectorU16(const Vector vec)
{
    VectorU16 ret_vec = *(VectorU16*)(&vec);
    return ret_vec;
}

VectorU32 toVectorU32(const Vector vec)
{
    VectorU32 ret_vec = *(VectorU32*)(&vec);
    return ret_vec;
}

VectorU64 toVectorU64(const Vector vec)
{
    VectorU64 ret_vec = *(VectorU64*)(&vec);
    return ret_vec;
}

#endif  // DUOPLOT_MATH_VECTOR_VECTOR_INSTANTIATION_H_
