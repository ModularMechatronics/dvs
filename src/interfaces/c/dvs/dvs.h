#ifndef DVS_DVS_H_
#define DVS_DVS_H_

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "dvs/communication_header.h"
#include "dvs/internal.h"
#include "dvs/math/math.h"
#include "dvs/structures.h"

void plotFunction3D(const Vector* const x,
                    const Vector* const y,
                    const Vector* const z,
                    const Function fcn,
                    const CommunicationHeaderObject first_prop,
                    ...)
{
    CommunicationHeader hdr;
    initCommunicationHeader(&hdr, fcn);

    APPEND_OBJ(&hdr, CHOT_DATA_TYPE, x->data_type, uint8_t);
    APPEND_OBJ(&hdr, CHOT_NUM_ELEMENTS, x->num_elements, uint32_t);

    APPEND_PROPERTIES(hdr, first_prop);

    sendHeaderAndThreeVectors(getSendFunction(), x, y, z, &hdr);
}

void plotFunction2D(
    const Vector* const x, const Vector* const y, const Function fcn, const CommunicationHeaderObject first_prop, ...)
{
    CommunicationHeader hdr;
    initCommunicationHeader(&hdr, fcn);

    APPEND_OBJ(&hdr, CHOT_DATA_TYPE, x->data_type, uint8_t);
    APPEND_OBJ(&hdr, CHOT_NUM_ELEMENTS, x->num_elements, uint32_t);

    APPEND_PROPERTIES(hdr, first_prop);

    sendHeaderAndTwoVectors(getSendFunction(), x, y, &hdr);
}

void surfFunction(const Matrix* const x,
                  const Matrix* const y,
                  const Matrix* const z,
                  const CommunicationHeaderObject first_prop,
                  ...)
{
    CommunicationHeader hdr;
    initCommunicationHeader(&hdr, F_SURF);

    Dimension2D dims = {x->num_rows, x->num_cols};

    APPEND_OBJ(&hdr, CHOT_DATA_TYPE, x->data_type, uint8_t);
    APPEND_OBJ(&hdr, CHOT_NUM_ELEMENTS, x->num_rows * x->num_cols, uint32_t);  // Needed?
    appendDims(&hdr, CHOT_DIMENSION_2D, dims);

    APPEND_PROPERTIES(hdr, first_prop);

    sendHeaderAndThreeMatrices(getSendFunction(), x, y, z, &hdr);
}

void drawMeshFunction(const Point3dArray vertices,
                      const IndexTripletArray indices,
                      const CommunicationHeaderObject first_prop,
                      ...)
{
    CommunicationHeader hdr;
    initCommunicationHeader(&hdr, F_DRAW_MESH);

    APPEND_OBJ(&hdr, CHOT_DATA_TYPE, DT_DOUBLE, uint8_t);
    APPEND_OBJ(&hdr, CHOT_NUM_ELEMENTS, indices.num_elements, uint32_t);
    APPEND_OBJ(&hdr, CHOT_NUM_VERTICES, vertices.num_elements, uint32_t);
    APPEND_OBJ(&hdr, CHOT_NUM_INDICES, indices.num_elements, uint32_t);

    APPEND_PROPERTIES(hdr, first_prop);

    sendHeaderAndTwoByteArrays(getSendFunction(),
                               (uint8_t*)(vertices.elements),
                               vertices.num_elements * sizeof(Triangle3DD),
                               (uint8_t*)(indices.elements),
                               indices.num_elements * sizeof(IndexTriplet),
                               &hdr);
}

void imShowFunction(const ImageC3* const img, const CommunicationHeaderObject first_prop, ...)
{
    CommunicationHeader hdr;
    initCommunicationHeader(&hdr, F_IM_SHOW);

    Dimension2D dims = {img->num_rows, img->num_cols};

    APPEND_OBJ(&hdr, CHOT_DATA_TYPE, img->data_type, uint8_t);
    APPEND_OBJ(&hdr, CHOT_NUM_CHANNELS, 3, uint8_t);
    APPEND_OBJ(&hdr, CHOT_NUM_ELEMENTS, img->num_rows * img->num_cols, uint32_t);  // Needed?
    appendDims(&hdr, CHOT_DIMENSION_2D, dims);

    APPEND_PROPERTIES(hdr, first_prop);

    sendHeaderAndByteArray(getSendFunction(),
                           (uint8_t*)(img->data),
                           img->num_rows * img->num_cols * 3 * dataTypeToNumBytes(img->data_type),
                           &hdr);
}

#define imShow(img, ...) imShowFunction((ImageC3*)&img, ##__VA_ARGS__, getLastCommHdrObj())

#define drawMesh(vertices, indices, ...) drawMeshFunction(vertices, indices, ##__VA_ARGS__, getLastCommHdrObj())

#define surf(x, y, z, ...) surfFunction((Matrix*)&x, (Matrix*)&y, (Matrix*)&z, ##__VA_ARGS__, getLastCommHdrObj())

#define plot(x, y, ...) plotFunction2D((Vector*)&x, (Vector*)&y, F_PLOT2, ##__VA_ARGS__, getLastCommHdrObj())

#define scatter(x, y, ...) plotFunction2D((Vector*)&x, (Vector*)&y, F_SCATTER2, ##__VA_ARGS__, getLastCommHdrObj())

#define plot3(x, y, z, ...) \
    plotFunction3D((Vector*)&x, (Vector*)&y, (Vector*)&z, F_PLOT3, ##__VA_ARGS__, getLastCommHdrObj())

#define scatter3(x, y, z, ...) \
    plotFunction3D((Vector*)&x, (Vector*)&y, (Vector*)&z, F_SCATTER3, ##__VA_ARGS__, getLastCommHdrObj())

void setCurrentElement(const char* const name)
{
    CommunicationHeader hdr;
    initCommunicationHeader(&hdr, F_SET_CURRENT_ELEMENT);

    CommunicationHeaderObject* const current_obj = hdr.objects + hdr.obj_idx;

    current_obj->type = CHOT_ELEMENT_NAME;

    const size_t name_length = strnlen(name, 100U);
    current_obj->num_bytes = sizeof(uint8_t) + sizeof(uint8_t) + (uint8_t)name_length;

    memset(current_obj->data, 0, kMaxNumFunctionHeaderBytes);

    current_obj->data[0U] = PT_NAME;
    current_obj->data[1U] = name_length;
    memcpy(current_obj->data + 2U, name, name_length);

    appendObjectIndexToCommunicationHeaderObjectLookupTable(&(hdr.objects_lut), CHOT_ELEMENT_NAME, hdr.obj_idx);

    hdr.obj_idx += 1;

    sendHeader(getSendFunction(), &hdr);
}

void clearView()
{
    CommunicationHeader hdr;
    initCommunicationHeader(&hdr, F_CLEAR);

    sendHeader(getSendFunction(), &hdr);
}

void softClearView()
{
    CommunicationHeader hdr;
    initCommunicationHeader(&hdr, F_SOFT_CLEAR);

    sendHeader(getSendFunction(), &hdr);
}

void view(const float azimuth, const float elevation)
{
    CommunicationHeader hdr;
    initCommunicationHeader(&hdr, F_VIEW);

    APPEND_OBJ(&hdr, CHOT_AZIMUTH, azimuth, float);
    APPEND_OBJ(&hdr, CHOT_ELEVATION, elevation, float);

    sendHeader(getSendFunction(), &hdr);
}

void axis(const Vec3d min_bound, const Vec3d max_bound)
{
    CommunicationHeader hdr;
    initCommunicationHeader(&hdr, F_AXES_3D);

    typedef struct S_Bnd3D
    {
        Vec3d min_bnd;
        Vec3d max_bnd;
    } Bnd3D;

    const Bnd3D bnd = {min_bound, max_bound};

    APPEND_OBJ(&hdr, CHOT_AXIS_MIN_MAX_VEC, bnd, Bnd3D);

    sendHeader(getSendFunction(), &hdr);
}

void axis2D(const Vec2d min_bound, const Vec2d max_bound)
{
    CommunicationHeader hdr;
    initCommunicationHeader(&hdr, F_AXES_3D);

    typedef struct S_Bnd3D
    {
        Vec3d min_bnd;
        Vec3d max_bnd;
    } Bnd3D;
    const Vec3d v0 = {min_bound.x, min_bound.y, 0.0};
    const Vec3d v1 = {max_bound.x, max_bound.y, 0.0};

    const Bnd3D bnd = {v0, v1};

    APPEND_OBJ(&hdr, CHOT_AXIS_MIN_MAX_VEC, bnd, Bnd3D);

    sendHeader(getSendFunction(), &hdr);
}

#endif  // DVS_DVS_H_
