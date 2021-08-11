#ifndef DVS_TRIANGLE_H_
#define DVS_TRIANGLE_H_

#include <stdint.h>
#include <string.h>

#include "math/vector_low_dim/vec3.h"
#include "enumerations.h"

typedef struct S_Triangle3DF
{
    Point3DF p0;
    Point3DF p1;
    Point3DF p2;
} Triangle3DF;

typedef struct S_Triangle3DD
{
    Point3DD p0;
    Point3DD p1;
    Point3DD p2;
} Triangle3DD;

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

Triangle3DFArray createTriangle3DFArray(const size_t num_elements)
{
    Triangle3DFArray array;
    array.elements = malloc(sizeof(Triangle3DF) * num_elements);
    array.num_elements = num_elements;

    return array;
}

Triangle3DDArray createTriangle3DDArray(const size_t num_elements)
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

IndexTripletArray createIndexTripletArray(const size_t num_elements)
{
    IndexTripletArray array;
    array.elements = malloc(sizeof(IndexTriplet) * num_elements);
    array.num_elements = num_elements;

    return array;
}


#endif
