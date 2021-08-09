#ifndef DVS_VECTOR_MATH_H_
#define DVS_VECTOR_MATH_H_

#include "math/utils.h"
#include "math/vector/vector_instantiation.h"

VectorF vector_linspaceFromPointsAndCountF(const double x0, const double x1, const size_t num_elements)
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

VectorF vector_sinF(const VectorF vec)
{
    VectorF vec_out = createVectorF(vec.num_elements);
    for (size_t k = 0; k < vec.num_elements; k++)
    {
        vec_out.data[k] = sin(vec.data[k]);
    }

    return vec_out;
}

VectorF vector_cosF(const VectorF vec)
{
    VectorF vec_out = createVectorF(vec.num_elements);
    for (size_t k = 0; k < vec.num_elements; k++)
    {
        vec_out.data[k] = cos(vec.data[k]);
    }

    return vec_out;
}

#endif
