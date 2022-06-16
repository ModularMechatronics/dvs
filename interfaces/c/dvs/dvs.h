#ifndef DVS_H_
#define DVS_H_

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

    sendHeaderAndByteArray(getSendFunction(), (uint8_t*)points, sizeof(Point3DD) * 4, &hdr);
}

void drawTriangleFunction(const Triangle3DD triangle, const FunctionHeaderObject first_prop, ...)
{
    FunctionHeader hdr;
    initFunctionHeader(&hdr);

    APPEND_VAL(&hdr, FHOT_FUNCTION, F_DRAW_TRIANGLES_3D, uint8_t);
    APPEND_VAL(&hdr, FHOT_DATA_TYPE, DT_DOUBLE, uint8_t);
    APPEND_VAL(&hdr, FHOT_NUM_ELEMENTS, 1, uint32_t);

    APPEND_PROPERTIES(hdr, first_prop);

    sendHeaderAndByteArray(getSendFunction(), (uint8_t*)(&triangle), sizeof(Triangle3DD), &hdr);
}

void drawTrianglesFunction(const Triangle3DFArray triangles, const FunctionHeaderObject first_prop, ...)
{
    FunctionHeader hdr;
    initFunctionHeader(&hdr);

    APPEND_VAL(&hdr, FHOT_FUNCTION, F_DRAW_TRIANGLES_3D, uint8_t);
    APPEND_VAL(&hdr, FHOT_DATA_TYPE, DT_FLOAT, uint8_t);
    APPEND_VAL(&hdr, FHOT_NUM_ELEMENTS, triangles.num_elements, uint32_t);

    APPEND_PROPERTIES(hdr, first_prop);

    sendHeaderAndByteArray(
        getSendFunction(), (uint8_t*)(triangles.elements), triangles.num_elements * sizeof(Triangle3DF), &hdr);
}

void drawMeshFunction(const Point3DDArray vertices,
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

void drawLineBetweenPointsFunction(const Point3DD p0, const Point3DD p1, const FunctionHeaderObject first_prop, ...)
{
    FunctionHeader hdr;
    initFunctionHeader(&hdr);

    APPEND_VAL(&hdr, FHOT_FUNCTION, F_DRAW_LINE_BETWEEN_POINTS_3D, uint8_t);
    APPEND_VAL(&hdr, FHOT_DATA_TYPE, DT_DOUBLE, uint8_t);
    APPEND_VAL(&hdr, FHOT_NUM_ELEMENTS, 0, uint32_t);

    APPEND_PROPERTIES(hdr, first_prop);
    const Point3DD points[2] = {p0, p1};

    sendHeaderAndByteArray(getSendFunction(), (uint8_t*)points, 2 * sizeof(Point3DD), &hdr);
}

void drawLineBetween2DPointsFunction(const Point2DD p0, const Point2DD p1, const FunctionHeaderObject first_prop, ...)
{
    FunctionHeader hdr;
    initFunctionHeader(&hdr);

    APPEND_VAL(&hdr, FHOT_FUNCTION, F_DRAW_LINE_BETWEEN_POINTS_3D, uint8_t);
    APPEND_VAL(&hdr, FHOT_DATA_TYPE, DT_DOUBLE, uint8_t);
    APPEND_VAL(&hdr, FHOT_NUM_ELEMENTS, 0, uint32_t);

    APPEND_PROPERTIES(hdr, first_prop);
    const Point3DD p0_e = {p0.x, p0.y, 0.0}, p1_e = {p1.x, p1.y, 0.0};
    const Point3DD points[2] = {p0_e, p1_e};

    sendHeaderAndByteArray(getSendFunction(), (uint8_t*)points, 2 * sizeof(Point3DD), &hdr);
}

void drawLine3DFunction(const Line3DD line, const double t0, const double t1, const FunctionHeaderObject first_prop, ...)
{
    FunctionHeader hdr;
    initFunctionHeader(&hdr);

    APPEND_VAL(&hdr, FHOT_FUNCTION, F_DRAW_LINE3D, uint8_t);
    APPEND_VAL(&hdr, FHOT_DATA_TYPE, DT_DOUBLE, uint8_t);
    APPEND_VAL(&hdr, FHOT_NUM_ELEMENTS, 0, uint32_t);

    APPEND_PROPERTIES(hdr, first_prop);

    Point3DD p0, p1;
    p0.x = line.p.x + line.v.x * t0;
    p0.y = line.p.y + line.v.y * t0;
    p0.z = line.p.z + line.v.z * t0;

    p1.x = line.p.x + line.v.x * t1;
    p1.y = line.p.y + line.v.y * t1;
    p1.z = line.p.z + line.v.z * t1;

    const Point3DD points[2] = {p0, p1};

    sendHeaderAndByteArray(getSendFunction(), (uint8_t*)points, 2 * sizeof(Point3DD), &hdr);
}

void drawLine2DFunction(
    const PLine2DD line, const double t0, const double t1, const FunctionHeaderObject first_prop, ...)
{
    FunctionHeader hdr;
    initFunctionHeader(&hdr);

    APPEND_VAL(&hdr, FHOT_FUNCTION, F_DRAW_LINE3D, uint8_t);
    APPEND_VAL(&hdr, FHOT_DATA_TYPE, DT_DOUBLE, uint8_t);
    APPEND_VAL(&hdr, FHOT_NUM_ELEMENTS, 0, uint32_t);

    APPEND_PROPERTIES(hdr, first_prop);

    Point3DD p0, p1;
    p0.x = line.p.x + line.v.x * t0;
    p0.y = line.p.y + line.v.y * t0;
    p0.z = 0.0;

    p1.x = line.p.x + line.v.x * t1;
    p1.y = line.p.y + line.v.y * t1;
    p1.z = 0.0;

    const Point3DD points[2] = {p0, p1};

    sendHeaderAndByteArray(getSendFunction(), (uint8_t*)points, 2 * sizeof(Point3DD), &hdr);
}

void drawPlaneXYFunction(
    const PlaneD plane, const PointXYD p0, const PointXYD p1, const FunctionHeaderObject first_prop, ...)
{
    FunctionHeader hdr;
    initFunctionHeader(&hdr);

    APPEND_VAL(&hdr, FHOT_FUNCTION, F_PLANE_XY, uint8_t);
    APPEND_VAL(&hdr, FHOT_DATA_TYPE, DT_DOUBLE, uint8_t);
    APPEND_VAL(&hdr, FHOT_NUM_ELEMENTS, 2, uint32_t);

    APPEND_PROPERTIES(hdr, first_prop);

    struct PlanePointsStruct
    {
        PointXYD p0;
        PointXYD p1;
        PlaneD plane;
    } pps;

    pps.p0 = p0;
    pps.p1 = p1;
    pps.plane = plane;

    sendHeaderAndByteArray(getSendFunction(), (uint8_t*)(&pps), sizeof(struct PlanePointsStruct), &hdr);
}

void drawPlaneXZFunction(
    const PlaneD plane, const PointXZD p0, const PointXZD p1, const FunctionHeaderObject first_prop, ...)
{
    FunctionHeader hdr;
    initFunctionHeader(&hdr);

    APPEND_VAL(&hdr, FHOT_FUNCTION, F_PLANE_XZ, uint8_t);
    APPEND_VAL(&hdr, FHOT_DATA_TYPE, DT_DOUBLE, uint8_t);
    APPEND_VAL(&hdr, FHOT_NUM_ELEMENTS, 2, uint32_t);

    APPEND_PROPERTIES(hdr, first_prop);

    struct PlanePointsStruct
    {
        PointXZD p0;
        PointXZD p1;
        PlaneD plane;
    } pps;

    pps.p0 = p0;
    pps.p1 = p1;
    pps.plane = plane;

    sendHeaderAndByteArray(getSendFunction(), (uint8_t*)(&pps), sizeof(struct PlanePointsStruct), &hdr);
}

void drawPlaneYZFunction(
    const PlaneD plane, const PointYZD p0, const PointYZD p1, const FunctionHeaderObject first_prop, ...)
{
    FunctionHeader hdr;
    initFunctionHeader(&hdr);

    APPEND_VAL(&hdr, FHOT_FUNCTION, F_PLANE_YZ, uint8_t);
    APPEND_VAL(&hdr, FHOT_DATA_TYPE, DT_DOUBLE, uint8_t);
    APPEND_VAL(&hdr, FHOT_NUM_ELEMENTS, 2, uint32_t);

    APPEND_PROPERTIES(hdr, first_prop);

    struct PlanePointsStruct
    {
        PointYZD p0;
        PointYZD p1;
        PlaneD plane;
    } pps;

    pps.p0 = p0;
    pps.p1 = p1;
    pps.plane = plane;

    sendHeaderAndByteArray(getSendFunction(), (uint8_t*)(&pps), sizeof(struct PlanePointsStruct), &hdr);
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

#define drawPlaneXY(plane, p0, p1, ...) drawPlaneXYFunction(plane, p0, p1, __VA_ARGS__, getLastFuncHdrObj())

#define drawPlaneXZ(plane, p0, p1, ...) drawPlaneXZFunction(plane, p0, p1, __VA_ARGS__, getLastFuncHdrObj())

#define drawPlaneYZ(plane, p0, p1, ...) drawPlaneYZFunction(plane, p0, p1, __VA_ARGS__, getLastFuncHdrObj())

#define drawLine2D(line, t0, t1, ...) drawLine2DFunction(line, t0, t1, __VA_ARGS__, getLastFuncHdrObj())

#define drawLine3D(line, t0, t1, ...) drawLine3DFunction(line, t0, t1, __VA_ARGS__, getLastFuncHdrObj())

#define drawLineBetweenPoints(p0, p1, ...) drawLineBetweenPointsFunction(p0, p1, __VA_ARGS__, getLastFuncHdrObj())

#define drawLineBetween2DPoints(p0, p1, ...) drawLineBetween2DPointsFunction(p0, p1, __VA_ARGS__, getLastFuncHdrObj())

#define drawMesh(vertices, indices, ...) drawMeshFunction(vertices, indices, __VA_ARGS__, getLastFuncHdrObj())

#define drawTriangle(triangle, ...) drawTriangleFunction(triangle, __VA_ARGS__, getLastFuncHdrObj())

#define drawTriangles(triangles, ...) drawTrianglesFunction(triangles, __VA_ARGS__, getLastFuncHdrObj())

#define drawPolygonFrom4Points(p0, p1, p2, p3, ...) \
    drawPolygonFrom4PointsFunction(p0, p1, p2, p3, __VA_ARGS__, getLastFuncHdrObj())

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
