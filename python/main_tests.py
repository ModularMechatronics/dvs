import dvs
import numpy as np
from structures import *


def setup_dvs_view():
    dvs.set_current_element("view_00")
    dvs.clear_view()


def test_scatter():
    dvs.set_current_element("view_00")
    dvs.clear_view()

    for i in range(0, 100):
        dvs.soft_clear_view()
        x = np.linspace(0, 3, 100, dtype=np.float32)
        y = np.sin(x * 5.0 + 0.1 * i)

        dvs.view(0, 90)
        dvs.scatter(x, y)


def test_plot():
    dvs.set_current_element("view_00")
    dvs.clear_view()

    for i in range(0, 100):
        dvs.soft_clear_view()
        x = np.linspace(0, 3, 100, dtype=np.float32)
        y = np.sin(x * 5.0 + 0.1 * i)

        dvs.axis([-10, -10, -10], [10, 10, 10])
        dvs.view(0, 90)
        dvs.plot(x, y)  # ,color=dvs.Color(23, 25, 66) alpha=100, line_width=13


def test_plot3():
    dvs.set_current_element("view_00")

    dvs.clear_view()
    x = np.linspace(0, 3, 100, dtype=np.float32)
    y = np.sin(x * 5.0)
    z = np.cos(x * 5.0)

    dvs.axis([-10, -10, -10], [10, 10, 10])
    dvs.view(0, 90)
    dvs.plot3(x, y, z)


def test_scatter3():
    dvs.set_current_element("view_00")

    dvs.clear_view()
    x = np.linspace(0, 3, 100, dtype=np.float32)
    y = np.sin(x * 5.0)
    z = np.cos(x * 5.0)
    dvs.scatter3(x, y, z)


def test_surf():
    dvs.set_current_element("view_00")

    dvs.clear_view()
    x = np.linspace(-0.1, 3, 100, dtype=np.float32)
    y = np.linspace(-0.1, 3, 100, dtype=np.float32)
    x, y = np.meshgrid(x, y)
    r = np.sqrt(x * x + y * y)
    z = np.sin(r) / r

    dvs.axis([-10, -10, -10], [10, 10, 10])
    dvs.view(0, 90)
    dvs.surf(x, y, z)


def test_imshow():
    dvs.set_current_element("view_00")

    dvs.clear_view()
    x = np.linspace(-0.1, 3, 100, dtype=np.float32)
    y = np.linspace(-0.1, 3, 100, dtype=np.float32)
    x, y = np.meshgrid(x, y)
    r = np.sqrt(x * x + y * y)
    zr = np.sin(r) / r
    zg = np.sin(r)
    zb = np.sin(r) / (r * r)

    zr = (zr - np.min(zr)) / (np.max(zr) - np.min(zr))
    zg = (zg - np.min(zg)) / (np.max(zg) - np.min(zg))
    zb = (zb - np.min(zb)) / (np.max(zb) - np.min(zb))

    q = np.zeros((3, 100, 100), dtype=np.float32)
    q[0, :, :] = zr
    q[1, :, :] = zg
    q[2, :, :] = zb

    dvs.axis([-10, -10, -10], [10, 10, 10])
    dvs.view(0, 90)
    dvs.imshow(q)


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


def test_draw_mesh():
    setup_dvs_view()
    points = [Point3D(1.0, 1.0),
              Point3D(2.0, 4.0),
              Point3D(3.0, 2.0),
              Point3D(4.0, 3.0),
              Point3D(4.0, 1.5)]
    indices = [IndexTriplet(0, 1, 2),
               IndexTriplet(1, 2, 3),
               IndexTriplet(2, 3, 4),
               IndexTriplet(0, 2, 4)]
    draw_mesh(points, indices)


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
