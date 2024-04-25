#ifndef DUOPLOT_MATH_VECTOR_VECTOR_MATH_H_
#define DUOPLOT_MATH_VECTOR_VECTOR_MATH_H_

#include "duoplot/math/utils.h"
#include "duoplot/math/vector/vector_cast.h"
#include "duoplot/math/vector/vector_instantiation.h"

DUOPLOT_WEAK duoplot_VectorF duoplot_Vector_linspaceFromPointsAndCountF(const double x0,
                                                                        const double x1,
                                                                        const size_t num_elements)
{
    duoplot_VectorF vec = duoplot_createVectorF(num_elements);
    const double dx = (x1 - x0) / ((double)(num_elements - 1));

    vec.data[0] = x0;

    for (size_t k = 1; k < num_elements; k++)
    {
        vec.data[k] = vec.data[k - 1] + dx;
    }

    return vec;
}

DUOPLOT_WEAK duoplot_VectorF duoplot_Vector_sinF(const duoplot_VectorF vec)
{
    duoplot_VectorF vec_out = duoplot_createVectorF(vec.num_elements);
    for (size_t k = 0; k < vec.num_elements; k++)
    {
        vec_out.data[k] = sin(vec.data[k]);
    }

    return vec_out;
}

DUOPLOT_WEAK duoplot_VectorF duoplot_Vector_cosF(const duoplot_VectorF vec)
{
    duoplot_VectorF vec_out = duoplot_createVectorF(vec.num_elements);
    for (size_t k = 0; k < vec.num_elements; k++)
    {
        vec_out.data[k] = cos(vec.data[k]);
    }

    return vec_out;
}

#endif  // DUOPLOT_MATH_VECTOR_VECTOR_MATH_H_
