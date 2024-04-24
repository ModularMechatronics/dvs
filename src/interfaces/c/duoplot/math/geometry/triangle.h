#ifndef DUOPLOT_MATH_GEOMETRY_TRIANGLE_H_
#define DUOPLOT_MATH_GEOMETRY_TRIANGLE_H_

#include <stdint.h>
#include <string.h>

#include "duoplot/enumerations.h"
#include "duoplot/math/vector_low_dim/vec3.h"
#include "duoplot/pp.h"

typedef struct S_duoplot_Triangle3F
{
    duoplot_Point3F p0;
    duoplot_Point3F p1;
    duoplot_Point3F p2;
} duoplot_Triangle3F;

typedef struct S_duoplot_Triangle3D
{
    duoplot_Point3D p0;
    duoplot_Point3D p1;
    duoplot_Point3D p2;
} duoplot_Triangle3D;

typedef struct S_duoplot_Triangle3DArray
{
    duoplot_Triangle3D* elements;
    size_t num_elements;
} duoplot_Triangle3DArray;

typedef struct S_duoplot_Triangle3FArray
{
    duoplot_Triangle3F* elements;
    size_t num_elements;
} duoplot_Triangle3FArray;

DUOPLOT_WEAK duoplot_Triangle3FArray duoplot_createTriangle3FArray(const size_t num_elements)
{
    duoplot_Triangle3FArray array;
    array.elements = malloc(sizeof(duoplot_Triangle3F) * num_elements);
    array.num_elements = num_elements;

    return array;
}

DUOPLOT_WEAK duoplot_Triangle3DArray duoplot_createTriangle3DArray(const size_t num_elements)
{
    duoplot_Triangle3DArray array;
    array.elements = malloc(sizeof(duoplot_Triangle3D) * num_elements);
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
