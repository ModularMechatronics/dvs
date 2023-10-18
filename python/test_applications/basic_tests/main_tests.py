import dvs
import numpy as np
from structures import *


def setup_dvs_view():
    dvs.set_current_element("p_view_0")
    dvs.clear_view()


def test_scatter():
    dvs.set_current_element("p_view_0")
    dvs.clear_view()

    for i in range(0, 100):
        dvs.soft_clear_view()
        x = np.linspace(0, 3, 100, dtype=np.float32)
        y = np.sin(x * 5.0 + 0.1 * i)

        dvs.view(0, 90)
        dvs.scatter(x, y)


def test_plot():
    dvs.set_current_element("p_view_0")
    dvs.clear_view()

    # for i in range(0, 100):
    # dvs.soft_clear_view()
    x = np.linspace(0, 3, 100, dtype=np.float32)
    y = np.sin(x * 5.0)

    # dvs.axis([-10, -10, -10], [10, 10, 10])
    # dvs.view(0, 90)
    dvs.plot(x, y)  # ,color=dvs.Color(23, 25, 66) alpha=100, line_width=13


def test_plot3():
    dvs.set_current_element("p_view_0")

    dvs.clear_view()
    x = np.linspace(0, 3, 100, dtype=np.float32)
    y = np.sin(x * 5.0)
    z = np.cos(x * 5.0)

    dvs.axis([-10, 1, -10, -10], [10, 10, 10])
    dvs.view(0, 90)
    dvs.plot3(x, y, z)


def test_scatter():
    dvs.set_current_element("p_view_0")

    dvs.clear_view()
    x = np.linspace(0, 3, 100, dtype=np.float32)
    y = np.sin(x * 5.0)
    dvs.scatter(x, y)


def test_scatter3():
    dvs.set_current_element("p_view_0")

    dvs.clear_view()
    x = np.linspace(0, 3, 100, dtype=np.float32)
    y = np.sin(x * 5.0)
    z = np.cos(x * 5.0)
    dvs.scatter3(x, y, z)


def test_surf():
    dvs.set_current_element("p_view_0")

    dvs.clear_view()
    x = np.linspace(-0.1, 3, 100, dtype=np.float32)
    y = np.linspace(-0.1, 3, 100, dtype=np.float32)
    x, y = np.meshgrid(x, y)
    r = np.sqrt(x * x + y * y)
    z = np.sin(r * 10.0) / (r * 10.0)

    dvs.view(0, 90)
    dvs.surf(x, y, z)


def test_imshow():
    dvs.set_current_element("p_view_0")

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

