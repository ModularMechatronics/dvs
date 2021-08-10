#ifndef DVS_H_
#define DVS_H_

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "function_header.h"
#include "internal.h"
#include "math/math.h"
#include "structures.h"

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

void drawPolygonFrom4PointsFunction(const Point3DD p0,
                                    const Point3DD p1,
                                    const Point3DD p2,
                                    const Point3DD p3,
                                    const FunctionHeaderObject first_prop,
                                    ...)
{
    FunctionHeader hdr;
    initFunctionHeader(&hdr);

    APPEND_VAL(&hdr, FHOT_FUNCTION, F_POLYGON_FROM_4_POINTS, uint8_t);
    APPEND_VAL(&hdr, FHOT_DATA_TYPE, DT_DOUBLE, uint8_t);
    APPEND_VAL(&hdr, FHOT_NUM_ELEMENTS, 4, uint32_t);

    APPEND_PROPERTIES(hdr, first_prop);
    const Point3DD points[4] = {p0, p1, p2, p3};

    sendHeaderAndByteArray(getSendFunction(), (uint8_t*)(&points), sizeof(Point3DD) * 4, &hdr);
}

#define drawPolygonFrom4Points(p0, p1, p2, p3, ...) \
    drawPolygonFrom4PointsFunction(p0, p1, p2, p3, __VA_ARGS__, getLastFHO())

#define surf(x, y, z, ...) surfFunction((Matrix*)&x, (Matrix*)&y, (Matrix*)&z, __VA_ARGS__, getLastFHO())

#define plot(x, y, ...) plotFunction2D((Vector*)&x, (Vector*)&y, F_PLOT2, __VA_ARGS__, getLastFHO())

#define scatter(x, y, ...) plotFunction2D((Vector*)&x, (Vector*)&y, F_SCATTER2, __VA_ARGS__, getLastFHO())

#define plot3(x, y, z, ...) plotFunction3D((Vector*)&x, (Vector*)&y, (Vector*)&z, F_PLOT3, __VA_ARGS__, getLastFHO())

#define scatter3(x, y, z, ...) \
    plotFunction3D((Vector*)&x, (Vector*)&y, (Vector*)&z, F_SCATTER3, __VA_ARGS__, getLastFHO())

/*void drawPolygonFrom4Points(const Point3D<double>& p0,
                            const Point3D<double>& p1,
                            const Point3D<double>& p2,
                            const Point3D<double>& p3,
                            const Us&... settings)*/

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

void axis(const Vec3DD min_bound, const Vec3DD max_bound)
{
    FunctionHeader hdr;
    initFunctionHeader(&hdr);
    APPEND_VAL(&hdr, FHOT_FUNCTION, F_AXES_3D, uint8_t);

    typedef struct S_Bnd3D
    {
        Vec3DD min_bnd;
        Vec3DD max_bnd;
    } Bnd3D;

    const Bnd3D bnd = {min_bound, max_bound};

    APPEND_VAL(&hdr, FHOT_AXIS_MIN_MAX_VEC, bnd, Bnd3D);

    sendHeader(getSendFunction(), &hdr);
}

void axis2D(const Vec2DD min_bound, const Vec2DD max_bound)
{
    FunctionHeader hdr;
    initFunctionHeader(&hdr);
    APPEND_VAL(&hdr, FHOT_FUNCTION, F_AXES_3D, uint8_t);

    typedef struct S_Bnd3D
    {
        Vec3DD min_bnd;
        Vec3DD max_bnd;
    } Bnd3D;
    const Vec3DD v0 = {min_bound.x, min_bound.y, 0.0};
    const Vec3DD v1 = {max_bound.x, max_bound.y, 0.0};

    const Bnd3D bnd = {v0, v1};

    APPEND_VAL(&hdr, FHOT_AXIS_MIN_MAX_VEC, bnd, Bnd3D);

    sendHeader(getSendFunction(), &hdr);
}

#endif
