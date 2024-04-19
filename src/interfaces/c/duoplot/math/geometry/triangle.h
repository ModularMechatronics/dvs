#ifndef DUOPLOT_MATH_GEOMETRY_TRIANGLE_H_
#define DUOPLOT_MATH_GEOMETRY_TRIANGLE_H_

#include <stdint.h>
#include <string.h>

#include "duoplot/enumerations.h"
#include "duoplot/math/vector_low_dim/vec3.h"
#include "duoplot/pp.h"

typedef struct S_Triangle3DF
{
    Point3f p0;
    Point3f p1;
    Point3f p2;
} Triangle3DF;

typedef struct S_Triangle3DD
{
    Point3d p0;
    Point3d p1;
    Point3d p2;
} Triangle3DD;

DUOPLOT_WEAK Triangle3DD createTriangle3DD(const Point3d p0, const Point3d p1, const Point3d p2)
{
    const Triangle3DD t = {p0, p1, p2};
    return t;
}

DUOPLOT_WEAK Triangle3DF createTriangle3DF(const Point3f p0, const Point3f p1, const Point3f p2)
{
    const Triangle3DF t = {p0, p1, p2};
    return t;
}

typedef struct S_Triangle3DDArray
{
    Triangle3DD* elements;
    size_t num_elements;
} Triangle3DDArray;

typedef struct S_Triangle3DFArray
{
    Triangle3DF* elements;
    size_t num_elements;
} Triangle3DFArray;

DUOPLOT_WEAK Triangle3DFArray createTriangle3DFArray(const size_t num_elements)
{
    Triangle3DFArray array;
    array.elements = malloc(sizeof(Triangle3DF) * num_elements);
    array.num_elements = num_elements;

    return array;
}

DUOPLOT_WEAK Triangle3DDArray createTriangle3DDArray(const size_t num_elements)
{
    Triangle3DDArray array;
    array.elements = malloc(sizeof(Triangle3DD) * num_elements);
    array.num_elements = num_elements;

    return array;
}

typedef struct S_IndexTriplet
{
    size_t i0;
    size_t i1;
    size_t i2;
} IndexTriplet;

typedef struct S_IndexTripletArray
{
    IndexTriplet* elements;
    size_t num_elements;
} IndexTripletArray;

DUOPLOT_WEAK IndexTripletArray createIndexTripletArray(const size_t num_elements)
{
    IndexTripletArray array;
    array.elements = malloc(sizeof(IndexTriplet) * num_elements);
    array.num_elements = num_elements;

    return array;
}

#endif  // DUOPLOT_MATH_GEOMETRY_TRIANGLE_H_
