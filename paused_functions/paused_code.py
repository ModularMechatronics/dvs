
def draw_polygon_from_4_points(p0: Point3D, p1: Point3D, p2: Point3D, p3: Point3D, **properties):
    hdr = CommunicationHeader()
    hdr.append(CommunicationHeaderObjectType.FUNCTION,
               Function.POLYGON_FROM_4_POINTS)
    hdr.append(CommunicationHeaderObjectType.NUM_ELEMENTS, 4)

    hdr.append(CommunicationHeaderObjectType.DATA_TYPE, DataType.DOUBLE)

    hdr.append_properties(properties)

    send_header_and_data(send_with_udp, hdr, p0, p1, p2, p3)


def draw_triangle(triangle: Triangle3D, **properties):

    hdr = CommunicationHeader()
    hdr.append(CommunicationHeaderObjectType.FUNCTION,
               Function.DRAW_TRIANGLES_3D)
    hdr.append(CommunicationHeaderObjectType.NUM_ELEMENTS, 1)
    hdr.append(CommunicationHeaderObjectType.DATA_TYPE, DataType.DOUBLE)

    hdr.append_properties(properties)

    send_header_and_data(send_with_udp, hdr, triangle)


def draw_triangles(triangles: List[Triangle3D], **properties):
    hdr = CommunicationHeader()
    hdr.append(CommunicationHeaderObjectType.FUNCTION, Function.DRAW_TRIANGLES_3D)
    hdr.append(CommunicationHeaderObjectType.NUM_ELEMENTS, len(triangles))
    # hdr.append(CommunicationHeaderObjectType.DATA_TYPE, ) TODO

    hdr.append_properties(properties)

    send_header_and_data(send_with_udp, hdr, triangles)



def draw_plane_xy(plane: Plane, p0: PointXY, p1: PointXY, **properties):
    hdr = CommunicationHeader()
    hdr.append(CommunicationHeaderObjectType.FUNCTION,
               Function.PLANE_XY)
    hdr.append(CommunicationHeaderObjectType.NUM_ELEMENTS, 2)
    hdr.append(CommunicationHeaderObjectType.DATA_TYPE, DataType.DOUBLE)

    hdr.append_properties(properties)

    send_header_and_data(send_with_udp, hdr, p0, p1, plane)


def draw_plane_xz(plane: Plane, p0: PointXZ, p1: PointXZ, **properties):
    hdr = CommunicationHeader()
    hdr.append(CommunicationHeaderObjectType.FUNCTION,
               Function.PLANE_XZ)
    hdr.append(CommunicationHeaderObjectType.NUM_ELEMENTS, 2)
    hdr.append(CommunicationHeaderObjectType.DATA_TYPE, DataType.DOUBLE)

    hdr.append_properties(properties)

    send_header_and_data(send_with_udp, hdr, p0, p1, plane)


def draw_plane_yz(plane: Plane, p0: PointYZ, p1: PointYZ, **properties):
    hdr = CommunicationHeader()
    hdr.append(CommunicationHeaderObjectType.FUNCTION,
               Function.PLANE_YZ)
    hdr.append(CommunicationHeaderObjectType.NUM_ELEMENTS, 2)
    hdr.append(CommunicationHeaderObjectType.DATA_TYPE, DataType.DOUBLE)

    hdr.append_properties(properties)

    send_header_and_data(send_with_udp, hdr, p0, p1, plane)


def draw_line(line: Line3D, t0: np.float64, t1: np.float64, **properties):
    hdr = CommunicationHeader()
    hdr.append(CommunicationHeaderObjectType.FUNCTION,
               Function.DRAW_LINE3D)
    hdr.append(CommunicationHeaderObjectType.NUM_ELEMENTS, 0)
    hdr.append(CommunicationHeaderObjectType.DATA_TYPE, DataType.DOUBLE)

    hdr.append_properties(properties)

    p0 = line.eval(t0)
    p1 = line.eval(t1)

    send_header_and_data(send_with_udp, hdr, p0, p1)


def draw_line_2d(line: PLine2D, t0: np.float64, t1: np.float64, **properties):

    hdr = CommunicationHeader()
    hdr.append(CommunicationHeaderObjectType.FUNCTION,
               Function.DRAW_LINE3D)
    hdr.append(CommunicationHeaderObjectType.NUM_ELEMENTS, 0)
    hdr.append(CommunicationHeaderObjectType.DATA_TYPE, DataType.DOUBLE)

    hdr.append_properties(properties)

    p0_2d = line.eval(t0)
    p1_2d = line.eval(t1)
    p0 = Point3D(p0_2d.x, p0_2d.y, 0.0)
    p1 = Point3D(p1_2d.x, p1_2d.y, 0.0)

    send_header_and_data(send_with_udp, hdr, p0, p1)


def draw_line_between_points(p0: Union[Point2D, Point3D], p1: Union[Point2D, Point3D], **properties):

    assert(type(p0) == type(p1))

    hdr = CommunicationHeader()

    if type(p0) == Point3D:
        p0_u = p0
        p1_u = p1
    else:
        p0_u = Point3D(p0.x, p0.y, 0.0)
        p1_u = Point3D(p1.x, p1.y, 0.0)
        
    hdr.append(CommunicationHeaderObjectType.FUNCTION,
               Function.DRAW_LINE3D)
    hdr.append(CommunicationHeaderObjectType.NUM_ELEMENTS, 0)
    hdr.append(CommunicationHeaderObjectType.DATA_TYPE, DataType.DOUBLE)

    hdr.append_properties(properties)

    send_header_and_data(send_with_udp, hdr, p0_u, p1_u)



def test_draw_polygon_from_4_points():
    setup_dvs_view()
    p0 = Point3D(0.0, 0.0, 0.0)
    p1 = Point3D(1.0, 0.0, 1.0)
    p2 = Point3D(1.0, 1.0, 2.0)
    p3 = Point3D(0.0, 1.0, 3.0)

    dvs.draw_polygon_from_4_points(p0, p1, p2, p3)


def test_draw_triangle():
    setup_dvs_view()
    triangle = Triangle(Point3D(0.0, 0.0, 0.0), Point3D(
        1.0, 1.0, 0.0), Point3D(1.0, 0.0, 2.0))


def test_draw_triangles():
    setup_dvs_view()

    def rm(phi): return np.array(
        [[np.cos(phi), -np.sin(phi), 0.0], [np.sin(phi), np.cos(phi), 0.0], [0.0, 0.0, 1.0]])

    coords = np.array([[0.0, 1.0, 1.0],
                       [0.0, 1.0, 0.0],
                       [0.0, 0.0, 2.0]])

    triangles = []

    for x in [0, 0.1, 0.2, 0.3, 0.4]:
        z = 2.0 * x
        phi = x

        rp = rm(phi) @ coords
        triangles.append(Triangle3D(Point3D(rp[0][0], rp[1][0], rp[2][0]),
                                    Point3D(rp[0][1], rp[1][1], rp[2][1]),
                                    Point3D(rp[0][2], rp[1][2], rp[2][2])))

    dvs.draw_triangles(triangles)



def test_draw_plane_xy():
    setup_dvs_view()

    plane = Plane(0.0, 0.2, 1.0, 0.5)
    p0 = PointXY(0.0, 0.0)
    p1 = PointXY(1.0, 1.0)


def test_draw_plane_xz():
    setup_dvs_view()
    plane = Plane(0.0, 1.0, 0.2, 0.5)
    p0 = PointXZ(0.0, 0.0)
    p1 = PointXZ(1.0, 1.0)


def test_draw_plane_yz():
    setup_dvs_view()

    plane = Plane(1.0, 0.0, 0.2, 0.5)
    p0 = PointYZ(0.0, 0.0)
    p1 = PointYZ(1.0, 1.0)


def test_draw_line():
    setup_dvs_view()  # TODO: <- Move to main.py
    line = Line3D(Point3D(0.1, 0.2, 0.3), Vec3D(1.1, 1.2, 1.3))

    dvs.draw_line(line, 0.1, 5.7)


def test_draw_line_2d():
    setup_dvs_view()

    line = PLine2D(Point2D(0.1, 0.2), Vec2D(1.1, 1.2))

    dvs.draw_line_2d(line, 0.1, 5.7)


def test_draw_line_2d_between_x_values():
    setup_dvs_view()

    line = HLine2D(0.1, 0.2, 0.3)
    dvs.draw_line_2d_between_x_values(line, 0.1, 4.0)


def test_draw_line_2d_between_y_values():
    setup_dvs_view()

    line = HLine2D(0.1, 0.2, 0.3)
    dvs.draw_line_2d_between_y_values(line, 0.5, 3.0)


def test_draw_line_between_points():
    setup_dvs_view()

    p0 = Point3D(0.1, 0.1, 0.1)
    p1 = Point3D(1.0, 2.0, 3.0)

    p0_2 = Point3D(p0.x, p0.y)
    p1_2 = Point3D(p1.x, p1.y)

    dvs.draw_line_between_points(p0, p1)

    dvs.draw_line_between_points(p0_2, p1_2)


def hold_on(name: str):

    hdr = CommunicationHeader()
    hdr.append(CommunicationHeaderObjectType.FUNCTION, Function.HOLD_ON)
    send_header(send_with_udp, hdr)


def hold_off(name: str):

    hdr = CommunicationHeader()
    hdr.append(CommunicationHeaderObjectType.FUNCTION, Function.HOLD_OFF)
    send_header(send_with_udp, hdr)
