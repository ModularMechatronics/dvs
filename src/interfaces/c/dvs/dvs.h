#ifndef DVS_DVS_H_
#define DVS_DVS_H_

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "dvs/function_header.h"
#include "dvs/internal.h"
#include "dvs/math/math.h"
#include "dvs/structures.h"

void plotFunction3D(const Vector* const x,
                    const Vector* const y,
                    const Vector* const z,
                    const Function fcn,
                    const FunctionHeaderObject first_prop,
                    ...)
{
    FunctionHeader hdr;
    initFunctionHeader(&hdr);

    APPEND_VAL(&hdr, FHOT_FUNCTION, fcn, uint8_t);
    APPEND_VAL(&hdr, FHOT_DATA_TYPE, x->data_type, uint8_t);
    APPEND_VAL(&hdr, FHOT_NUM_ELEMENTS, x->num_elements, uint32_t);

    APPEND_PROPERTIES(hdr, first_prop);

    sendHeaderAndThreeVectors(getSendFunction(), x, y, z, &hdr);
}

void plotFunction2D(
    const Vector* const x, const Vector* const y, const Function fcn, const FunctionHeaderObject first_prop, ...)
{
    FunctionHeader hdr;
    initFunctionHeader(&hdr);

    APPEND_VAL(&hdr, FHOT_FUNCTION, fcn, uint8_t);
    APPEND_VAL(&hdr, FHOT_DATA_TYPE, x->data_type, uint8_t);
    APPEND_VAL(&hdr, FHOT_NUM_ELEMENTS, x->num_elements, uint32_t);

    APPEND_PROPERTIES(hdr, first_prop);

    sendHeaderAndTwoVectors(getSendFunction(), x, y, &hdr);
}

void surfFunction(
    const Matrix* const x, const Matrix* const y, const Matrix* const z, const FunctionHeaderObject first_prop, ...)
{
    FunctionHeader hdr;
    initFunctionHeader(&hdr);

    Dimension2D dims = {x->num_rows, x->num_cols};

    APPEND_VAL(&hdr, FHOT_FUNCTION, F_SURF, uint8_t);
    APPEND_VAL(&hdr, FHOT_DATA_TYPE, x->data_type, uint8_t);
    APPEND_VAL(&hdr, FHOT_NUM_ELEMENTS, x->num_rows * x->num_cols, uint32_t);  // Needed?
    APPEND_VAL(&hdr, FHOT_DIMENSION_2D, dims, Dimension2D);

    APPEND_PROPERTIES(hdr, first_prop);

    sendHeaderAndThreeMatrices(getSendFunction(), x, y, z, &hdr);
}

void drawMeshFunction(const Point3dArray vertices,
                      const IndexTripletArray indices,
                      const FunctionHeaderObject first_prop,
                      ...)
{
    FunctionHeader hdr;
    initFunctionHeader(&hdr);

    APPEND_VAL(&hdr, FHOT_FUNCTION, F_DRAW_MESH, uint8_t);
    APPEND_VAL(&hdr, FHOT_DATA_TYPE, DT_DOUBLE, uint8_t);
    APPEND_VAL(&hdr, FHOT_NUM_ELEMENTS, indices.num_elements, uint32_t);
    APPEND_VAL(&hdr, FHOT_NUM_VERTICES, vertices.num_elements, uint32_t);
    APPEND_VAL(&hdr, FHOT_NUM_INDICES, indices.num_elements, uint32_t);

    APPEND_PROPERTIES(hdr, first_prop);

    sendHeaderAndTwoByteArrays(getSendFunction(),
                               (uint8_t*)(vertices.elements),
                               vertices.num_elements * sizeof(Triangle3DD),
                               (uint8_t*)(indices.elements),
                               indices.num_elements * sizeof(IndexTriplet),
                               &hdr);
}

void imShowFunction(const ImageC3* const img, const FunctionHeaderObject first_prop, ...)
{
    FunctionHeader hdr;
    initFunctionHeader(&hdr);

    Dimension2D dims = {img->num_rows, img->num_cols};

    APPEND_VAL(&hdr, FHOT_FUNCTION, F_IM_SHOW, uint8_t);
    APPEND_VAL(&hdr, FHOT_DATA_TYPE, img->data_type, uint8_t);
    APPEND_VAL(&hdr, FHOT_NUM_CHANNELS, 3, uint8_t);
    APPEND_VAL(&hdr, FHOT_NUM_ELEMENTS, img->num_rows * img->num_cols, uint32_t);  // Needed?
    APPEND_VAL(&hdr, FHOT_DIMENSION_2D, dims, Dimension2D);

    APPEND_PROPERTIES(hdr, first_prop);

    sendHeaderAndByteArray(getSendFunction(),
                           (uint8_t*)(img->data),
                           img->num_rows * img->num_cols * 3 * dataTypeToNumBytes(img->data_type),
                           &hdr);
}

#define imShow(img, ...) imShowFunction((ImageC3*)&img, __VA_ARGS__, getLastFuncHdrObj())

#define drawMesh(vertices, indices, ...) drawMeshFunction(vertices, indices, __VA_ARGS__, getLastFuncHdrObj())

#define surf(x, y, z, ...) surfFunction((Matrix*)&x, (Matrix*)&y, (Matrix*)&z, __VA_ARGS__, getLastFuncHdrObj())

#define plot(x, y, ...) plotFunction2D((Vector*)&x, (Vector*)&y, F_PLOT2, __VA_ARGS__, getLastFuncHdrObj())

#define scatter(x, y, ...) plotFunction2D((Vector*)&x, (Vector*)&y, F_SCATTER2, __VA_ARGS__, getLastFuncHdrObj())

#define plot3(x, y, z, ...) \
    plotFunction3D((Vector*)&x, (Vector*)&y, (Vector*)&z, F_PLOT3, __VA_ARGS__, getLastFuncHdrObj())

#define scatter3(x, y, z, ...) \
    plotFunction3D((Vector*)&x, (Vector*)&y, (Vector*)&z, F_SCATTER3, __VA_ARGS__, getLastFuncHdrObj())

void setCurrentElement(const char* name)
{
    FunctionHeader hdr;
    initFunctionHeader(&hdr);
    const size_t kNumBytesForName = 23;

    APPEND_VAL(&hdr, FHOT_FUNCTION, F_SET_CURRENT_ELEMENT, uint8_t);
    hdr.values[hdr.num_objects].type = FHOT_ELEMENT_NAME;
    hdr.values[hdr.num_objects].num_bytes = 1 + sizeof(size_t) + kNumBytesForName;
    hdr.values[hdr.num_objects].data[0] = (uint8_t)PT_NAME;

    memcpy(&(hdr.values[hdr.num_objects].data[1]), &kNumBytesForName, sizeof(size_t));
    memcpy(&(hdr.values[hdr.num_objects].data[1 + sizeof(size_t)]), name, kNumBytesForName);

    hdr.num_objects += 1;

    sendHeader(getSendFunction(), &hdr);
}

void clearView()
{
    FunctionHeader hdr;
    initFunctionHeader(&hdr);
    APPEND_VAL(&hdr, FHOT_FUNCTION, F_CLEAR, uint8_t);

    sendHeader(getSendFunction(), &hdr);
}

void softClearView()
{
    FunctionHeader hdr;
    initFunctionHeader(&hdr);
    APPEND_VAL(&hdr, FHOT_FUNCTION, F_SOFT_CLEAR, uint8_t);

    sendHeader(getSendFunction(), &hdr);
}

void holdOn()
{
    FunctionHeader hdr;
    initFunctionHeader(&hdr);
    APPEND_VAL(&hdr, FHOT_FUNCTION, F_HOLD_ON, uint8_t);

    sendHeader(getSendFunction(), &hdr);
}

void holdOff()
{
    FunctionHeader hdr;
    initFunctionHeader(&hdr);
    APPEND_VAL(&hdr, FHOT_FUNCTION, F_HOLD_OFF, uint8_t);

    sendHeader(getSendFunction(), &hdr);
}

void view(const float azimuth, const float elevation)
{
    FunctionHeader hdr;
    initFunctionHeader(&hdr);
    APPEND_VAL(&hdr, FHOT_FUNCTION, F_VIEW, uint8_t);
    APPEND_VAL(&hdr, FHOT_AZIMUTH, azimuth, float);
    APPEND_VAL(&hdr, FHOT_ELEVATION, elevation, float);

    sendHeader(getSendFunction(), &hdr);
}

void axis(const Vec3d min_bound, const Vec3d max_bound)
{
    FunctionHeader hdr;
    initFunctionHeader(&hdr);
    APPEND_VAL(&hdr, FHOT_FUNCTION, F_AXES_3D, uint8_t);

    typedef struct S_Bnd3D
    {
        Vec3d min_bnd;
        Vec3d max_bnd;
    } Bnd3D;

    const Bnd3D bnd = {min_bound, max_bound};

    APPEND_VAL(&hdr, FHOT_AXIS_MIN_MAX_VEC, bnd, Bnd3D);

    sendHeader(getSendFunction(), &hdr);
}

void axis2D(const Vec2d min_bound, const Vec2d max_bound)
{
    FunctionHeader hdr;
    initFunctionHeader(&hdr);
    APPEND_VAL(&hdr, FHOT_FUNCTION, F_AXES_3D, uint8_t);

    typedef struct S_Bnd3D
    {
        Vec3d min_bnd;
        Vec3d max_bnd;
    } Bnd3D;
    const Vec3d v0 = {min_bound.x, min_bound.y, 0.0};
    const Vec3d v1 = {max_bound.x, max_bound.y, 0.0};

    const Bnd3D bnd = {v0, v1};

    APPEND_VAL(&hdr, FHOT_AXIS_MIN_MAX_VEC, bnd, Bnd3D);

    sendHeader(getSendFunction(), &hdr);
}

#endif  // DVS_DVS_H_
