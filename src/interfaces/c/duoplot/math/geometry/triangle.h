#ifndef DUOPLOT_MATH_GEOMETRY_TRIANGLE_H_
#define DUOPLOT_MATH_GEOMETRY_TRIANGLE_H_

#include <stdint.h>
#include <string.h>

#include "duoplot/enumerations.h"
#include "duoplot/math/vector_low_dim/vec3.h"
#include "duoplot/pp.h"

typedef struct S_duoplot_Triangle3DF
{
    duoplot_Point3F p0;
    duoplot_Point3F p1;
    duoplot_Point3F p2;
} duoplot_Triangle3DF;

typedef struct S_duoplot_Triangle3DD
{
    duoplot_Point3D p0;
    duoplot_Point3D p1;
    duoplot_Point3D p2;
} duoplot_Triangle3DD;

typedef struct S_duoplot_Triangle3DDArray
{
    duoplot_Triangle3DD* elements;
    size_t num_elements;
} duoplot_Triangle3DDArray;

typedef struct S_duoplot_Triangle3DFArray
{
    duoplot_Triangle3DF* elements;
    size_t num_elements;
} duoplot_Triangle3DFArray;

DUOPLOT_WEAK duoplot_Triangle3DFArray duoplot_createTriangle3DFArray(const size_t num_elements)
{
    duoplot_Triangle3DFArray array;
    array.elements = malloc(sizeof(duoplot_Triangle3DF) * num_elements);
    array.num_elements = num_elements;

    return array;
}

DUOPLOT_WEAK duoplot_Triangle3DDArray duoplot_createTriangle3DDArray(const size_t num_elements)
{
    duoplot_Triangle3DDArray array;
    array.elements = malloc(sizeof(duoplot_Triangle3DD) * num_elements);
    array.num_elements = num_elements;

    return array;
}

typedef struct S_duoplot_IndexTriplet
{
    size_t i0;
    size_t i1;
    size_t i2;
} duoplot_IndexTriplet;

typedef struct S_duoplot_IndexTripletArray
{
    duoplot_IndexTriplet* elements;
    size_t num_elements;
} duoplot_IndexTripletArray;

DUOPLOT_WEAK duoplot_IndexTripletArray duoplot_createIndexTripletArray(const size_t num_elements)
{
    duoplot_IndexTripletArray array;
    array.elements = malloc(sizeof(duoplot_IndexTriplet) * num_elements);
    array.num_elements = num_elements;

    return array;
}

#endif  // DUOPLOT_MATH_GEOMETRY_TRIANGLE_H_
