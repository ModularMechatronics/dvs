#ifndef DVS_MATH_H_
#define DVS_MATH_H_

#include <math.h>

#include "enumerations.h"
#include "math/matrix_types.h"
#include "math/vector_types.h"

size_t dataTypeToNumBytes(const DataType dt)
{
    switch (dt)
    {
        case DT_FLOAT:
            return sizeof(float);
            break;
        case DT_DOUBLE:
            return sizeof(double);
            break;
        case DT_UINT8:
            return sizeof(uint8_t);
            break;
        case DT_UINT16:
            return sizeof(uint16_t);
            break;
        case DT_UINT32:
            return sizeof(uint32_t);
            break;
        case DT_UINT64:
            return sizeof(uint64_t);
            break;
        case DT_INT8:
            return sizeof(int8_t);
            break;
        case DT_INT16:
            return sizeof(int16_t);
            break;
        case DT_INT32:
            return sizeof(int32_t);
            break;
        case DT_INT64:
            return sizeof(int64_t);
            break;
        default:
            exit(-1);  // Invalid data type
            break;
    }
}

Vector createVector(const size_t num_elements, const DataType data_type)
{
    Vector vec;
    vec.data_type = data_type;
    vec.num_bytes_per_element = dataTypeToNumBytes(data_type);
    vec.num_elements = num_elements;
    vec.data = (uint8_t*)malloc(num_elements * vec.num_bytes_per_element);

    return vec;
}

#define freeVector(vec)                \
    {                                  \
        free(vec.data);                \
        vec.num_elements = 0;          \
        vec.num_bytes_per_element = 0; \
    }

VectorF createVectorF(const size_t num_elements)
{
    Vector vec = createVector(num_elements, DT_FLOAT);
    VectorF vec_f = *(VectorF*)(&vec);

    return vec_f;
}

VectorF toVectorF(const Vector vec)
{
    VectorF vec_f = *(VectorF*)(&vec);
    return vec_f;
}

VectorF linspaceFromPointsAndCountF(const double x0, const double x1, const size_t num_elements)
{
    VectorF vec = createVectorF(num_elements);
    const double dx = (x1 - x0) / ((double)(num_elements - 1));

    vec.data[0] = x0;

    for (size_t k = 1; k < num_elements; k++)
    {
        vec.data[k] = vec.data[k - 1] + dx;
    }

    return vec;
}

VectorF dvsSinF(const VectorF vec)
{
    VectorF vec_out = createVectorF(vec.num_elements);
    for (size_t k = 0; k < vec.num_elements; k++)
    {
        vec_out.data[k] = sin(vec.data[k]);
    }

    return vec_out;
}

VectorF dvsCosF(const VectorF vec)
{
    VectorF vec_out = createVectorF(vec.num_elements);
    for (size_t k = 0; k < vec.num_elements; k++)
    {
        vec_out.data[k] = cos(vec.data[k]);
    }

    return vec_out;
}

typedef struct S_Vec3DD
{
    double x;
    double y;
    double z;
} Vec3DD;

typedef struct S_Vec2DD
{
    double x;
    double y;
} Vec2DD;

#endif