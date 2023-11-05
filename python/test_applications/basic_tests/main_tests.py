import dvs
import numpy as np

# from structures import *
# import properties
import time
import surf_functions
import plot_2d_functions
import plot_3d_functions


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

    x = np.linspace(0, 3, 500, dtype=np.float32)
    y = np.sin(x * 5.0)

    """dvs.plot(
        x,
        y,
        # name="Hello", # TODO: Name is probably broken, name size is not included in SIZE_OF_PROPERTY
        alpha=0.1,
        line_width=13,
        point_size=13,
        buffer_size=500,
        z_offset=0.1,
        edge_color=dvs.properties.EdgeColor(0.1, 0.2, 0.3),
        face_color=dvs.properties.FaceColor(0.1, 0.2, 0.3),
        color=dvs.properties.Color(0.1, 0.2, 0.3),
        distance_from=dvs.properties.DistanceFrom.x(0.1, 0.2, 0.3),
        color_map=dvs.properties.ColorMap.JET,
        scatter_style=dvs.properties.ScatterStyle.CIRCLE,
        line_style=dvs.properties.LineStyle.DASHED,
        transform=dvs.properties.Transform(np.eye(3), np.eye(3), np.zeros(3)),
    )"""

    """dvs.plot(
        x,
        y,
        color_map=dvs.properties.ColorMap.JET,
        point_size=13,
        buffer_size=500,
        z_offset=0.1,
    )"""
    dvs.plot(x, y, line_width=13, color=dvs.properties.Color.CYAN)
    dvs.scatter(x, y, point_size=23, color=dvs.properties.Color.BLACK)


def test_plot3():
    dvs.set_current_element("p_view_0")

    dvs.clear_view()
    x = np.linspace(0, 3, 100, dtype=np.float32)
    y = np.sin(x * 5.0)
    z = np.cos(x * 5.0)

    dvs.axis([-10, -10, -10], [10, 10, 10])
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


def test_plot2_demo():
    dvs.set_current_element("p_view_0")
    dvs.clear_view()

    theta = np.linspace(-10.0, 10.0, 500, dtype=np.float32)

    PLOT2D_FUNCTION_NAMES = [
        q
        for q in plot_2d_functions.__dir__()
        if not q.startswith("__") and not q.startswith("np")
    ]

    for idx, plot_fun in enumerate(PLOT2D_FUNCTION_NAMES):
        x, y = eval("plot_2d_functions." + plot_fun + "(theta)")
        dvs.clear_view()

        dvs.plot(x, y)
        input("Press Enter to continue...")


def test_plot3_demo():
    dvs.set_current_element("p_view_0")
    dvs.clear_view()

    theta = np.linspace(-10.0, 10.0, 500, dtype=np.float32)

    PLOT2D_FUNCTION_NAMES = [
        q
        for q in plot_3d_functions.__dir__()
        if not q.startswith("__") and not q.startswith("np")
    ]

    for idx, plot_fun in enumerate(PLOT2D_FUNCTION_NAMES):
        x, y, z = eval("plot_3d_functions." + plot_fun + "(theta)")
        dvs.clear_view()

        dvs.plot3(x, y, z)
        input("Press Enter to continue...")


def test_surf_demo():
    dvs.set_current_element("p_view_0")
    dvs.clear_view()
    dvs.disable_scale_on_rotation()

    x = np.linspace(-4.0, 4.0, 100, dtype=np.float32)
    y = np.linspace(-4.0, 4.0, 100, dtype=np.float32)
    x, y = np.meshgrid(x, y)

    SURF_FUNCTION_NAMES = [
        q
        for q in surf_functions.__dir__()
        if not q.startswith("__") and not q.startswith("np")
    ]

    def next_angle(current_angle):
        if current_angle >= 0:
            if current_angle + 1 > 179:
                return -179
            else:
                return current_angle + 1
        else:
            return current_angle + 1

    curr_angle = 0

    color_maps = [
        dvs.properties.ColorMap.JET,
        dvs.properties.ColorMap.HSV,
        dvs.properties.ColorMap.MAGMA,
        dvs.properties.ColorMap.VIRIDIS,
        dvs.properties.ColorMap.PASTEL,
        dvs.properties.ColorMap.JET_SOFT,
        dvs.properties.ColorMap.JET_BRIGHT,
    ]

    for idx, surf_fun in enumerate(SURF_FUNCTION_NAMES):
        z = eval("surf_functions." + surf_fun + "(x, y)")
        dvs.soft_clear_view()

        # dvs.surf(x, y, z, color_map=properties.ColorMap.JET_SOFT)
        cm = color_maps[idx % len(color_maps)]
        dvs.surf(x, y, z, color_map=cm)

        for _ in range(40):
            dvs.view(curr_angle, 40)
            time.sleep(0.01)
            curr_angle = next_angle(curr_angle)


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

    vertices = [
        dvs.Point3D(0.0, 0.0, 0.0),
        dvs.Point3D(1.0, 0.0, 0.0),
        dvs.Point3D(0.5, -1.0, 1.0),
        dvs.Point3D(0.0, 0.0, 0.0),
        dvs.Point3D(-1.0, 0.0, 0.0),
        dvs.Point3D(0.0, 1.0, 1.0),
        dvs.Point3D(0.0, 0.0, 0.0),
        dvs.Point3D(-1.0, 0.0, 0.0),
        dvs.Point3D(0.0, 1.0, -1.0),
        dvs.Point3D(0.0, 0.0, 0.0),
        dvs.Point3D(1.0, 0.0, 0.0),
        dvs.Point3D(0.5, -1.0, -1.0),
    ]

    vertices2 = np.array(
        [
            [0.0, 0.0, 0.0],
            [1.0, 0.0, 0.0],
            [0.5, -1.0, 1.0],
            [0.0, 0.0, 0.0],
            [-1.0, 0.0, 0.0],
            [0.0, 1.0, 1.0],
            [0.0, 0.0, 0.0],
            [-1.0, 0.0, 0.0],
            [0.0, 1.0, -1.0],
            [0.0, 0.0, 0.0],
            [1.0, 0.0, 0.0],
            [0.5, -1.0, -1.0],
        ]
    )

    indices = [
        dvs.IndexTriplet(0, 1, 2),
        dvs.IndexTriplet(3, 4, 5),
        dvs.IndexTriplet(6, 7, 8),
        dvs.IndexTriplet(9, 10, 11),
    ]

    dvs.draw_mesh(vertices, indices)

    vertices2[:, 0] = vertices2[:, 0] + 2.0
    dvs.draw_mesh(
        vertices2,
        indices,
        edge_color=dvs.properties.EdgeColor(0, 0, 0),
        face_color=dvs.properties.FaceColor(1.0, 0, 0.9),
    )

    vertices2[:, 2] = vertices2[:, 2] - 2.0
    dvs.draw_mesh(
        vertices2,
        indices,
        edge_color=dvs.properties.EdgeColor(0, 0, 0),
        face_color=dvs.properties.FaceColor(0.0, 0, 0.9),
    )


def test_line_collection():
    setup_dvs_view()

    x = [0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0]
    y = [0.0, 0.0, 1.0, 1.0, 2.0, 2.0, 3.0, 3.0]
    z = [0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0]

    dvs.line_collection(x, y, color=dvs.properties.Color(1, 0, 1))

    dvs.line_collection3(x, y, z, color=dvs.properties.Color(1, 0, 1))


def test_plot_collection():
    dvs.set_current_element("p_view_0")
    dvs.clear_view()
    dvs.set_current_element("p1")
    dvs.clear_view()

    x = []
    y = []
    z = []
    xl = []
    yl = []
    zl = []

    dvs.soft_clear_view()

    for _ in range(0, 5):
        t = np.linspace(0, 3, 100, dtype=np.float32)
        xp = np.sin(t * 5.0 * np.random.rand() + np.random.rand())
        yp = np.cos(t * 5.0 * np.random.rand() + np.random.rand())
        zp = np.sin(t * 5.0 * np.random.rand() + np.random.rand())
        x.append(xp)
        y.append(yp)
        z.append(zp)
        xl.append((xp + 1.0).tolist())
        yl.append((yp + 1.0).tolist())
        zl.append((zp + 1.0).tolist())

        dvs.set_current_element("p_view_0")

        dvs.scatter(
            xp,
            yp,
            color=dvs.properties.Color(
                np.random.rand(), np.random.rand(), np.random.rand()
            ),
        )

        dvs.scatter(
            xp + 1.0,
            yp + 1.0,
            color=dvs.properties.Color(
                np.random.rand(), np.random.rand(), np.random.rand()
            ),
        )

        dvs.set_current_element("p1")
        dvs.scatter3(
            xp,
            yp,
            zp,
            color=dvs.properties.Color(
                np.random.rand(), np.random.rand(), np.random.rand()
            ),
        )

        dvs.scatter3(
            xp + 1.0,
            yp + 1.0,
            zp + 1.0,
            color=dvs.properties.Color(
                np.random.rand(), np.random.rand(), np.random.rand()
            ),
        )

    dvs.set_current_element("p_view_0")

    dvs.plot_collection(x, y, color=dvs.properties.Color.RED)
    dvs.plot_collection(xl, yl, color=dvs.properties.Color.RED)

    dvs.set_current_element("p1")

    dvs.plot_collection3(x, y, z, color=dvs.properties.Color.RED)
    dvs.plot_collection3(xl, yl, zl, color=dvs.properties.Color.RED)


def test_stairs():
    dvs.set_current_element("p_view_0")
    dvs.clear_view()

    x = np.linspace(0, 3, 500, dtype=np.float32)
    y = np.sin(x * 5.0)

    dvs.plot(x, y, color=dvs.properties.Color.CYAN)
    dvs.scatter(x, y, color=dvs.properties.Color.BLACK)
    dvs.stairs(x, y, color=dvs.properties.Color.RED)


def test_stem():
    dvs.set_current_element("p_view_0")
    dvs.clear_view()

    x = np.linspace(0, 3, 500, dtype=np.float32)
    y = np.sin(x * 5.0)

    dvs.plot(x, y, color=dvs.properties.Color.CYAN)
    dvs.scatter(x, y, point_size=13, color=dvs.properties.Color.BLACK)
    dvs.stem(x, y, color=dvs.properties.Color.RED)
