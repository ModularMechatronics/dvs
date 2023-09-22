
void drawPolygonFrom4PointsFunction(const Point3d p0,
                                    const Point3d p1,
                                    const Point3d p2,
                                    const Point3d p3,
                                    const CommunicationHeaderObject first_prop,
                                    ...)
{
    CommunicationHeader hdr;
    initCommunicationHeader(&hdr);

    APPEND_VAL(&hdr, CHOT_FUNCTION, F_POLYGON_FROM_4_POINTS, uint8_t);
    APPEND_VAL(&hdr, CHOT_DATA_TYPE, DT_DOUBLE, uint8_t);
    APPEND_VAL(&hdr, CHOT_NUM_ELEMENTS, 4, uint32_t);

    APPEND_PROPERTIES(hdr, first_prop);
    const Point3d points[4] = {p0, p1, p2, p3};

    sendHeaderAndByteArray(getSendFunction(), (uint8_t*)points, sizeof(Point3d) * 4, &hdr);
}

void drawTriangleFunction(const Triangle3DD triangle, const CommunicationHeaderObject first_prop, ...)
{
    CommunicationHeader hdr;
    initCommunicationHeader(&hdr);

    APPEND_VAL(&hdr, CHOT_FUNCTION, F_DRAW_TRIANGLES_3D, uint8_t);
    APPEND_VAL(&hdr, CHOT_DATA_TYPE, DT_DOUBLE, uint8_t);
    APPEND_VAL(&hdr, CHOT_NUM_ELEMENTS, 1, uint32_t);

    APPEND_PROPERTIES(hdr, first_prop);

    sendHeaderAndByteArray(getSendFunction(), (uint8_t*)(&triangle), sizeof(Triangle3DD), &hdr);
}

void drawTrianglesFunction(const Triangle3DFArray triangles, const CommunicationHeaderObject first_prop, ...)
{
    CommunicationHeader hdr;
    initCommunicationHeader(&hdr);

    APPEND_VAL(&hdr, CHOT_FUNCTION, F_DRAW_TRIANGLES_3D, uint8_t);
    APPEND_VAL(&hdr, CHOT_DATA_TYPE, DT_FLOAT, uint8_t);
    APPEND_VAL(&hdr, CHOT_NUM_ELEMENTS, triangles.num_elements, uint32_t);

    APPEND_PROPERTIES(hdr, first_prop);

    sendHeaderAndByteArray(
        getSendFunction(), (uint8_t*)(triangles.elements), triangles.num_elements * sizeof(Triangle3DF), &hdr);
}

void drawLineBetweenPointsFunction(const Point3d p0, const Point3d p1, const CommunicationHeaderObject first_prop, ...)
{
    CommunicationHeader hdr;
    initCommunicationHeader(&hdr);

    APPEND_VAL(&hdr, CHOT_FUNCTION, F_DRAW_LINE_BETWEEN_POINTS_3D, uint8_t);
    APPEND_VAL(&hdr, CHOT_DATA_TYPE, DT_DOUBLE, uint8_t);
    APPEND_VAL(&hdr, CHOT_NUM_ELEMENTS, 0, uint32_t);

    APPEND_PROPERTIES(hdr, first_prop);
    const Point3d points[2] = {p0, p1};

    sendHeaderAndByteArray(getSendFunction(), (uint8_t*)points, 2 * sizeof(Point3d), &hdr);
}

void drawLineBetween2DPointsFunction(const Point2d p0,
                                     const Point2d p1,
                                     const CommunicationHeaderObject first_prop,
                                     ...)
{
    CommunicationHeader hdr;
    initCommunicationHeader(&hdr);

    APPEND_VAL(&hdr, CHOT_FUNCTION, F_DRAW_LINE_BETWEEN_POINTS_3D, uint8_t);
    APPEND_VAL(&hdr, CHOT_DATA_TYPE, DT_DOUBLE, uint8_t);
    APPEND_VAL(&hdr, CHOT_NUM_ELEMENTS, 0, uint32_t);

    APPEND_PROPERTIES(hdr, first_prop);
    const Point3d p0_e = {p0.x, p0.y, 0.0}, p1_e = {p1.x, p1.y, 0.0};
    const Point3d points[2] = {p0_e, p1_e};

    sendHeaderAndByteArray(getSendFunction(), (uint8_t*)points, 2 * sizeof(Point3d), &hdr);
}

void drawLine3DFunction(
    const Line3DD line, const double t0, const double t1, const CommunicationHeaderObject first_prop, ...)
{
    CommunicationHeader hdr;
    initCommunicationHeader(&hdr);

    APPEND_VAL(&hdr, CHOT_FUNCTION, F_DRAW_LINE3D, uint8_t);
    APPEND_VAL(&hdr, CHOT_DATA_TYPE, DT_DOUBLE, uint8_t);
    APPEND_VAL(&hdr, CHOT_NUM_ELEMENTS, 0, uint32_t);

    APPEND_PROPERTIES(hdr, first_prop);

    Point3d p0, p1;
    p0.x = line.p.x + line.v.x * t0;
    p0.y = line.p.y + line.v.y * t0;
    p0.z = line.p.z + line.v.z * t0;

    p1.x = line.p.x + line.v.x * t1;
    p1.y = line.p.y + line.v.y * t1;
    p1.z = line.p.z + line.v.z * t1;

    const Point3d points[2] = {p0, p1};

    sendHeaderAndByteArray(getSendFunction(), (uint8_t*)points, 2 * sizeof(Point3d), &hdr);
}

void drawLine2DFunction(
    const PLine2DD line, const double t0, const double t1, const CommunicationHeaderObject first_prop, ...)
{
    CommunicationHeader hdr;
    initCommunicationHeader(&hdr);

    APPEND_VAL(&hdr, CHOT_FUNCTION, F_DRAW_LINE3D, uint8_t);
    APPEND_VAL(&hdr, CHOT_DATA_TYPE, DT_DOUBLE, uint8_t);
    APPEND_VAL(&hdr, CHOT_NUM_ELEMENTS, 0, uint32_t);

    APPEND_PROPERTIES(hdr, first_prop);

    Point3d p0, p1;
    p0.x = line.p.x + line.v.x * t0;
    p0.y = line.p.y + line.v.y * t0;
    p0.z = 0.0;

    p1.x = line.p.x + line.v.x * t1;
    p1.y = line.p.y + line.v.y * t1;
    p1.z = 0.0;

    const Point3d points[2] = {p0, p1};

    sendHeaderAndByteArray(getSendFunction(), (uint8_t*)points, 2 * sizeof(Point3d), &hdr);
}

void drawPlaneXYFunction(
    const PlaneD plane, const PointXYd p0, const PointXYd p1, const CommunicationHeaderObject first_prop, ...)
{
    CommunicationHeader hdr;
    initCommunicationHeader(&hdr);

    APPEND_VAL(&hdr, CHOT_FUNCTION, F_PLANE_XY, uint8_t);
    APPEND_VAL(&hdr, CHOT_DATA_TYPE, DT_DOUBLE, uint8_t);
    APPEND_VAL(&hdr, CHOT_NUM_ELEMENTS, 2, uint32_t);

    APPEND_PROPERTIES(hdr, first_prop);

    struct PlanePointsStruct
    {
        PointXYd p0;
        PointXYd p1;
        PlaneD plane;
    } pps;

    pps.p0 = p0;
    pps.p1 = p1;
    pps.plane = plane;

    sendHeaderAndByteArray(getSendFunction(), (uint8_t*)(&pps), sizeof(struct PlanePointsStruct), &hdr);
}

void drawPlaneXZFunction(
    const PlaneD plane, const PointXZd p0, const PointXZd p1, const CommunicationHeaderObject first_prop, ...)
{
    CommunicationHeader hdr;
    initCommunicationHeader(&hdr);

    APPEND_VAL(&hdr, CHOT_FUNCTION, F_PLANE_XZ, uint8_t);
    APPEND_VAL(&hdr, CHOT_DATA_TYPE, DT_DOUBLE, uint8_t);
    APPEND_VAL(&hdr, CHOT_NUM_ELEMENTS, 2, uint32_t);

    APPEND_PROPERTIES(hdr, first_prop);

    struct PlanePointsStruct
    {
        PointXZd p0;
        PointXZd p1;
        PlaneD plane;
    } pps;

    pps.p0 = p0;
    pps.p1 = p1;
    pps.plane = plane;

    sendHeaderAndByteArray(getSendFunction(), (uint8_t*)(&pps), sizeof(struct PlanePointsStruct), &hdr);
}

void drawPlaneYZFunction(
    const PlaneD plane, const PointYZd p0, const PointYZd p1, const CommunicationHeaderObject first_prop, ...)
{
    CommunicationHeader hdr;
    initCommunicationHeader(&hdr);

    APPEND_VAL(&hdr, CHOT_FUNCTION, F_PLANE_YZ, uint8_t);
    APPEND_VAL(&hdr, CHOT_DATA_TYPE, DT_DOUBLE, uint8_t);
    APPEND_VAL(&hdr, CHOT_NUM_ELEMENTS, 2, uint32_t);

    APPEND_PROPERTIES(hdr, first_prop);

    struct PlanePointsStruct
    {
        PointYZd p0;
        PointYZd p1;
        PlaneD plane;
    } pps;

    pps.p0 = p0;
    pps.p1 = p1;
    pps.plane = plane;

    sendHeaderAndByteArray(getSendFunction(), (uint8_t*)(&pps), sizeof(struct PlanePointsStruct), &hdr);
}

#define drawPlaneXY(plane, p0, p1, ...) drawPlaneXYFunction(plane, p0, p1, __VA_ARGS__, getLastCommHdrObj())

#define drawPlaneXZ(plane, p0, p1, ...) drawPlaneXZFunction(plane, p0, p1, __VA_ARGS__, getLastCommHdrObj())

#define drawPlaneYZ(plane, p0, p1, ...) drawPlaneYZFunction(plane, p0, p1, __VA_ARGS__, getLastCommHdrObj())

#define drawLine2D(line, t0, t1, ...) drawLine2DFunction(line, t0, t1, __VA_ARGS__, getLastCommHdrObj())

#define drawLine3D(line, t0, t1, ...) drawLine3DFunction(line, t0, t1, __VA_ARGS__, getLastCommHdrObj())

#define drawLineBetweenPoints(p0, p1, ...) drawLineBetweenPointsFunction(p0, p1, __VA_ARGS__, getLastCommHdrObj())

#define drawLineBetween2DPoints(p0, p1, ...) drawLineBetween2DPointsFunction(p0, p1, __VA_ARGS__, getLastCommHdrObj())

#define drawTriangle(triangle, ...) drawTriangleFunction(triangle, __VA_ARGS__, getLastCommHdrObj())

#define drawTriangles(triangles, ...) drawTrianglesFunction(triangles, __VA_ARGS__, getLastCommHdrObj())

#define drawPolygonFrom4Points(p0, p1, p2, p3, ...) \
    drawPolygonFrom4PointsFunction(p0, p1, p2, p3, __VA_ARGS__, getLastCommHdrObj())
