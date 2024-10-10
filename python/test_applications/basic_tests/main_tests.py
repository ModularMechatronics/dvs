import duoplot
import numpy as np

import pylas
import time
import sys
import surf_functions
import plot_2d_functions
import plot_3d_functions
import pywavefront
import os


def setup_duoplot_view():
    duoplot.set_current_element("p_view_0")
    duoplot.clear_view()


def test_scatter():
    duoplot.set_current_element("p_view_0")
    duoplot.clear_view()

    for i in range(0, 100):
        duoplot.soft_clear_view()
        x = np.linspace(0, 3, 100, dtype=np.float32)
        y = np.sin(x * 5.0 + 0.1 * i)

        duoplot.view(0, 90)
        duoplot.scatter(x, y)


def test_plot():
    duoplot.set_current_element("p_view_0")
    duoplot.clear_view()

    x = np.linspace(0, 3, 500, dtype=np.float32)
    y = np.sin(x * 5.0)

    """duoplot.plot(
        x,
        y,
        # name="Hello", # TODO: Name is probably broken, name size is not included in SIZE_OF_PROPERTY
        alpha=0.1,
        line_width=13,
        point_size=13,
        buffer_size=500,
        z_offset=0.1,
        edge_color=duoplot.properties.EdgeColor(0.1, 0.2, 0.3),
        face_color=duoplot.properties.FaceColor(0.1, 0.2, 0.3),
        color=duoplot.properties.Color(0.1, 0.2, 0.3),
        distance_from=duoplot.properties.DistanceFrom.x(0.1, 0.2, 0.3),
        color_map=duoplot.properties.ColorMap.JET,
        scatter_style=duoplot.properties.ScatterStyle.CIRCLE,
        line_style=duoplot.properties.LineStyle.DASHED,
        transform=duoplot.properties.Transform(np.eye(3), np.eye(3), np.zeros(3)),
    )"""

    """duoplot.plot(
        x,
        y,
        color_map=duoplot.properties.ColorMap.JET,
        point_size=13,
        buffer_size=500,
        z_offset=0.1,
    )"""
    duoplot.plot(x, y, line_width=13, color=duoplot.properties.Color.CYAN)
    duoplot.scatter(x, y, point_size=23, color=duoplot.properties.Color.BLACK)


def test_plot3():
    duoplot.set_current_element("p_view_0")

    duoplot.clear_view()
    x = np.linspace(0, 3, 100, dtype=np.float32)
    y = np.sin(x * 5.0)
    z = np.cos(x * 5.0)

    duoplot.axis([-10, -10, -10], [10, 10, 10])
    duoplot.view(0, 90)
    duoplot.plot3(x, y, z)


def test_scatter():
    duoplot.set_current_element("p_view_0")

    duoplot.clear_view()
    x = np.linspace(0, 3, 100, dtype=np.float32)
    y = np.sin(x * 5.0)
    duoplot.scatter(x, y)


def test_scatter3():
    duoplot.set_current_element("p_view_0")

    duoplot.clear_view()
    x = np.linspace(0, 3, 100, dtype=np.float32)
    y = np.sin(x * 5.0)
    z = np.cos(x * 5.0)
    duoplot.scatter3(x, y, z)


def test_surf():
    duoplot.set_current_element("p_view_0")

    duoplot.clear_view()
    x = np.linspace(-0.1, 3, 100, dtype=np.float32)
    y = np.linspace(-0.1, 3, 100, dtype=np.float32)
    x, y = np.meshgrid(x, y)
    r = np.sqrt(x * x + y * y)
    z = np.sin(r * 10.0) / (r * 10.0)

    duoplot.view(0, 90)
    duoplot.surf(x, y, z)


def test_plot2_demo():
    duoplot.set_current_element("p_view_0")
    duoplot.clear_view()

    theta = np.linspace(-10.0, 10.0, 500, dtype=np.float32)

    PLOT2D_FUNCTION_NAMES = [
        q
        for q in plot_2d_functions.__dir__()
        if not q.startswith("__") and not q.startswith("np")
    ]

    for idx, plot_fun in enumerate(PLOT2D_FUNCTION_NAMES):
        x, y = eval("plot_2d_functions." + plot_fun + "(theta)")
        duoplot.clear_view()
        duoplot.set_title(plot_fun)

        duoplot.plot(x, y)
        input("Press Enter to continue...")


def test_plot3_demo():
    duoplot.set_current_element("p_view_0")
    duoplot.clear_view()

    theta = np.linspace(-10.0, 10.0, 500, dtype=np.float32)

    PLOT2D_FUNCTION_NAMES = [
        q
        for q in plot_3d_functions.__dir__()
        if not q.startswith("__") and not q.startswith("np")
    ]

    for idx, plot_fun in enumerate(PLOT2D_FUNCTION_NAMES):
        x, y, z = eval("plot_3d_functions." + plot_fun + "(theta)")
        duoplot.clear_view()

        duoplot.plot3(x, y, z)
        input("Press Enter to continue...")


def test_series_demo():
    # duoplot.open_project_file("./project_files/series.duoplot") # TODO: Open this file for this test

    for i in range(0, 15):
        duoplot.set_current_element("p" + str(i))
        duoplot.clear_view()
        # duoplot.disable_scale_on_rotation()
        duoplot.view(30, 33)
        duoplot.axes_square()
        duoplot.axis(duoplot.Vec3D(1.0, 1.0, -1.0), duoplot.Vec3D(3.0, 3.0, 1.0))

    x = np.linspace(-4.0, 4.0, 100, dtype=np.float32)
    y = np.linspace(-4.0, 4.0, 100, dtype=np.float32)
    x, y = np.meshgrid(x, y)

    SURF_FUNCTION_NAMES = [
        q
        for q in surf_functions.__dir__()
        if not q.startswith("__") and not q.startswith("np")
    ]

    SURF_FUNCTION_NAMES = [q for q in reversed(SURF_FUNCTION_NAMES)]

    color_maps = [
        duoplot.properties.ColorMap.MAGMA,
        duoplot.properties.ColorMap.VIRIDIS,
        duoplot.properties.ColorMap.JET_SOFT,
        duoplot.properties.ColorMap.JET_BRIGHT,
        duoplot.properties.ColorMap.PASTEL,
        duoplot.properties.ColorMap.HSV,
        duoplot.properties.ColorMap.JET,
    ]

    for idx, surf_fun in enumerate(SURF_FUNCTION_NAMES):
        if idx >= 15:
            break

        print(surf_fun)
        duoplot.set_current_element("p" + str(idx))
        z = eval("surf_functions." + surf_fun + "(x, y)")
        duoplot.soft_clear_view()

        cm = color_maps[idx % len(color_maps)]
        if idx < 2:
            duoplot.surf(x, y, z, color_map=cm)
        else:
            duoplot.surf(x, y, z, color_map=cm, edge_color=duoplot.properties.EdgeColor.NONE)

        # for _ in range(40):
        #     duoplot.view(curr_angle, 40)
        #     time.sleep(0.01)
        #     curr_angle = next_angle(curr_angle)

    """x = np.linspace(-4.0, 4.0, 100, dtype=np.float32)
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
        duoplot.properties.ColorMap.JET,
        duoplot.properties.ColorMap.HSV,
        duoplot.properties.ColorMap.MAGMA,
        duoplot.properties.ColorMap.VIRIDIS,
        duoplot.properties.ColorMap.PASTEL,
        duoplot.properties.ColorMap.JET_SOFT,
        duoplot.properties.ColorMap.JET_BRIGHT,
    ]

    for idx, surf_fun in enumerate(SURF_FUNCTION_NAMES):
        z = eval("surf_functions." + surf_fun + "(x, y)")
        duoplot.soft_clear_view()

        # duoplot.surf(x, y, z, color_map=properties.ColorMap.JET_SOFT)
        cm = color_maps[idx % len(color_maps)]
        duoplot.surf(x, y, z, color_map=cm)

        for _ in range(40):
            duoplot.view(curr_angle, 40)
            time.sleep(0.01)
            curr_angle = next_angle(curr_angle)"""


def test_surf_demo():
    duoplot.set_current_element("p_view_0")
    duoplot.clear_view()
    duoplot.disable_scale_on_rotation()

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
        duoplot.properties.ColorMap.JET,
        duoplot.properties.ColorMap.HSV,
        duoplot.properties.ColorMap.MAGMA,
        duoplot.properties.ColorMap.VIRIDIS,
        duoplot.properties.ColorMap.PASTEL,
        duoplot.properties.ColorMap.JET_SOFT,
        duoplot.properties.ColorMap.JET_BRIGHT,
    ]

    for idx, surf_fun in enumerate(SURF_FUNCTION_NAMES):
        z = eval("surf_functions." + surf_fun + "(x, y)")
        duoplot.soft_clear_view()

        # duoplot.surf(x, y, z, color_map=properties.ColorMap.JET_SOFT)
        cm = color_maps[idx % len(color_maps)]
        duoplot.surf(x, y, z, color_map=cm)

        for _ in range(40):
            duoplot.view(curr_angle, 40)
            time.sleep(0.01)
            curr_angle = next_angle(curr_angle)


def test_imshow():
    duoplot.set_current_element("p_view_0")

    duoplot.clear_view()
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

    duoplot.axis([-10, -10, -10], [10, 10, 10])
    duoplot.view(0, 90)
    duoplot.imshow(q)


def test_draw_mesh():
    setup_duoplot_view()

    vertices = [
        duoplot.Point3D(0.0, 0.0, 0.0),
        duoplot.Point3D(1.0, 0.0, 0.0),
        duoplot.Point3D(0.5, -1.0, 1.0),
        duoplot.Point3D(0.0, 0.0, 0.0),
        duoplot.Point3D(-1.0, 0.0, 0.0),
        duoplot.Point3D(0.0, 1.0, 1.0),
        duoplot.Point3D(0.0, 0.0, 0.0),
        duoplot.Point3D(-1.0, 0.0, 0.0),
        duoplot.Point3D(0.0, 1.0, -1.0),
        duoplot.Point3D(0.0, 0.0, 0.0),
        duoplot.Point3D(1.0, 0.0, 0.0),
        duoplot.Point3D(0.5, -1.0, -1.0),
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
        duoplot.IndexTriplet(0, 1, 2),
        duoplot.IndexTriplet(3, 4, 5),
        duoplot.IndexTriplet(6, 7, 8),
        duoplot.IndexTriplet(9, 10, 11),
    ]

    duoplot.draw_mesh(vertices, indices)

    vertices2[:, 0] = vertices2[:, 0] + 2.0
    duoplot.draw_mesh(
        vertices2,
        indices,
        edge_color=duoplot.properties.EdgeColor(0, 0, 0),
        face_color=duoplot.properties.FaceColor(1.0, 0, 0.9),
    )

    vertices2[:, 2] = vertices2[:, 2] - 2.0
    duoplot.draw_mesh(
        vertices2,
        indices,
        edge_color=duoplot.properties.EdgeColor(0, 0, 0),
        face_color=duoplot.properties.FaceColor(0.0, 0, 0.9),
    )


def test_line_collection():
    setup_duoplot_view()

    x = [0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0]
    y = [0.0, 0.0, 1.0, 1.0, 2.0, 2.0, 3.0, 3.0]
    z = [0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0]

    duoplot.line_collection(x, y, color=duoplot.properties.Color(1, 0, 1))

    duoplot.line_collection3(x, y, z, color=duoplot.properties.Color(1, 0, 1))


def test_plot_collection():
    duoplot.set_current_element("p_view_0")
    duoplot.clear_view()
    duoplot.set_current_element("p1")
    duoplot.clear_view()

    x = []
    y = []
    z = []
    xl = []
    yl = []
    zl = []

    duoplot.soft_clear_view()

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

        duoplot.set_current_element("p_view_0")

        duoplot.scatter(
            xp,
            yp,
            color=duoplot.properties.Color(
                np.random.rand(), np.random.rand(), np.random.rand()
            ),
        )

        duoplot.scatter(
            xp + 1.0,
            yp + 1.0,
            color=duoplot.properties.Color(
                np.random.rand(), np.random.rand(), np.random.rand()
            ),
        )

        duoplot.set_current_element("p1")
        duoplot.scatter3(
            xp,
            yp,
            zp,
            color=duoplot.properties.Color(
                np.random.rand(), np.random.rand(), np.random.rand()
            ),
        )

        duoplot.scatter3(
            xp + 1.0,
            yp + 1.0,
            zp + 1.0,
            color=duoplot.properties.Color(
                np.random.rand(), np.random.rand(), np.random.rand()
            ),
        )

    duoplot.set_current_element("p_view_0")

    duoplot.plot_collection(x, y, color=duoplot.properties.Color.RED)
    duoplot.plot_collection(xl, yl, color=duoplot.properties.Color.RED)

    duoplot.set_current_element("p1")

    duoplot.plot_collection3(x, y, z, color=duoplot.properties.Color.RED)
    duoplot.plot_collection3(xl, yl, zl, color=duoplot.properties.Color.RED)


def test_stairs():
    duoplot.set_current_element("p_view_0")
    duoplot.clear_view()

    x = np.linspace(0, 3, 500, dtype=np.float32)
    y = np.sin(x * 5.0)

    duoplot.plot(x, y, color=duoplot.properties.Color.CYAN)
    duoplot.scatter(x, y, color=duoplot.properties.Color.BLACK)
    duoplot.stairs(x, y, color=duoplot.properties.Color.RED)


def test_stem():
    duoplot.set_current_element("p_view_0")
    duoplot.clear_view()

    x = np.linspace(0, 3, 500, dtype=np.float32)
    y = np.sin(x * 5.0)

    duoplot.plot(x, y, color=duoplot.properties.Color.CYAN)
    duoplot.scatter(x, y, point_size=13, color=duoplot.properties.Color.BLACK)
    duoplot.stem(x, y, color=duoplot.properties.Color.RED)


def color_map_jet(d, colors):
    value = np.float32(d)
    value[value < 0.0] = 0.0
    value[value >= 1.0] = 0.99999

    full_range_value = value * np.float32(11.0)
    integer_part = np.floor(full_range_value).astype(np.int16)
    fraction_part = full_range_value - integer_part

    colors[integer_part == 0, 0] = (
        (0.3686274509803922 - fraction_part[integer_part == 0] * 0.17254901960784316)
        * 255.0
    ).astype(np.uint8)
    colors[integer_part == 0, 1] = (
        (0.30980392156862746 + fraction_part[integer_part == 0] * 0.22352941176470587)
        * 255.0
    ).astype(np.uint8)
    colors[integer_part == 0, 2] = (
        (0.6352941176470588 + fraction_part[integer_part == 0] * 0.10588235294117654)
        * 255.0
    ).astype(np.uint8)

    colors[integer_part == 1, 0] = (
        (0.19607843137254902 + fraction_part[integer_part == 1] * 0.203921568627451)
        * 255.0
    ).astype(np.uint8)
    colors[integer_part == 1, 1] = (
        (0.5333333333333333 + fraction_part[integer_part == 1] * 0.22745098039215683)
        * 255.0
    ).astype(np.uint8)
    colors[integer_part == 1, 2] = (
        (0.7411764705882353 - fraction_part[integer_part == 1] * 0.09411764705882353)
        * 255.0
    ).astype(np.uint8)

    colors[integer_part == 2, 0] = (
        (0.4 + fraction_part[integer_part == 2] * 0.2705882352941176) * 255.0
    ).astype(np.uint8)
    colors[integer_part == 2, 1] = (
        (0.7607843137254902 + fraction_part[integer_part == 2] * 0.10588235294117654)
        * 255.0
    ).astype(np.uint8)
    colors[integer_part == 2, 2] = (
        (0.6470588235294118 - fraction_part[integer_part == 2] * 0.0039215686274509665)
        * 255.0
    ).astype(np.uint8)

    colors[integer_part == 3, 0] = (
        (0.6705882352941176 + fraction_part[integer_part == 3] * 0.2313725490196079)
        * 255.0
    ).astype(np.uint8)
    colors[integer_part == 3, 1] = (
        (0.8666666666666667 + fraction_part[integer_part == 3] * 0.09411764705882353)
        * 255.0
    ).astype(np.uint8)
    colors[integer_part == 3, 2] = (
        (0.6431372549019608 - fraction_part[integer_part == 3] * 0.04705882352941182)
        * 255.0
    ).astype(np.uint8)

    colors[integer_part == 4, 0] = (
        (0.9019607843137255 + fraction_part[integer_part == 4] * 0.0980392156862745)
        * 255.0
    ).astype(np.uint8)
    colors[integer_part == 4, 1] = (
        (0.9607843137254902 + fraction_part[integer_part == 4] * 0.039215686274509776)
        * 255.0
    ).astype(np.uint8)
    colors[integer_part == 4, 2] = (
        (0.596078431372549 + fraction_part[integer_part == 4] * 0.15294117647058825)
        * 255.0
    ).astype(np.uint8)

    colors[integer_part == 5, 0] = (
        (1.0 - fraction_part[integer_part == 5] * 0.0039215686274509665) * 255.0
    ).astype(np.uint8)
    colors[integer_part == 5, 1] = (
        (1.0 - fraction_part[integer_part == 5] * 0.1215686274509804) * 255.0
    ).astype(np.uint8)
    colors[integer_part == 5, 2] = (
        (0.7490196078431373 - fraction_part[integer_part == 5] * 0.20392156862745103)
        * 255.0
    ).astype(np.uint8)

    colors[integer_part == 6, 0] = (
        (0.996078431372549 - fraction_part[integer_part == 6] * 0.0039215686274509665)
        * 255.0
    ).astype(np.uint8)
    colors[integer_part == 6, 1] = (
        (0.8784313725490196 - fraction_part[integer_part == 6] * 0.196078431372549)
        * 255.0
    ).astype(np.uint8)
    colors[integer_part == 6, 2] = (
        (0.5450980392156862 - fraction_part[integer_part == 6] * 0.16470588235294115)
        * 255.0
    ).astype(np.uint8)

    colors[integer_part == 7, 0] = (
        (0.9921568627450981 - fraction_part[integer_part == 7] * 0.03529411764705881)
        * 255.0
    ).astype(np.uint8)
    colors[integer_part == 7, 1] = (
        (0.6823529411764706 - fraction_part[integer_part == 7] * 0.25490196078431376)
        * 255.0
    ).astype(np.uint8)
    colors[integer_part == 7, 2] = (
        (0.3803921568627451 - fraction_part[integer_part == 7] * 0.11764705882352938)
        * 255.0
    ).astype(np.uint8)

    colors[integer_part == 8, 0] = (
        (0.9568627450980393 - fraction_part[integer_part == 8] * 0.1215686274509804)
        * 255.0
    ).astype(np.uint8)
    colors[integer_part == 8, 1] = (
        (0.42745098039215684 - fraction_part[integer_part == 8] * 0.18431372549019606)
        * 255.0
    ).astype(np.uint8)
    colors[integer_part == 8, 2] = (
        (0.2627450980392157 + fraction_part[integer_part == 8] * 0.047058823529411764)
        * 255.0
    ).astype(np.uint8)

    colors[integer_part == 9, 0] = (
        (0.8352941176470589 - fraction_part[integer_part == 9] * 0.21568627450980393)
        * 255.0
    ).astype(np.uint8)
    colors[integer_part == 9, 1] = (
        (0.24313725490196078 - fraction_part[integer_part == 9] * 0.2392156862745098)
        * 255.0
    ).astype(np.uint8)
    colors[integer_part == 9, 2] = (
        (0.30980392156862746 - fraction_part[integer_part == 9] * 0.05098039215686273)
        * 255.0
    ).astype(np.uint8)

    colors[integer_part == 10, 0] = (
        (0.6196078431372549 - fraction_part[integer_part == 10] * 0.25098039215686274)
        * 255.0
    ).astype(np.uint8)
    colors[integer_part == 10, 1] = (
        (0.00392156862745098 + fraction_part[integer_part == 10] * 0.3058823529411765)
        * 255.0
    ).astype(np.uint8)
    colors[integer_part == 10, 2] = (
        (0.25882352941176473 + fraction_part[integer_part == 10] * 0.37647058823529406)
        * 255.0
    ).astype(np.uint8)


def test_3d_obj():
    duoplot.set_current_element("p_view_0")
    duoplot.clear_view()
    duoplot.wait_for_flush()

    x = np.linspace(0, 3, 500, dtype=np.float32)
    y = np.sin(x * 5.0)

    duck_path = "../../../misc_data/RubberDuck/RubberDuck.obj"

    if not os.path.exists(duck_path):
        duck_path = "misc_data/RubberDuck/RubberDuck.obj"

    bb = pywavefront.Wavefront(duck_path, create_materials=False, collect_faces=True)
    vertices = np.array(bb.vertices, dtype=np.float32)

    duoplot.axes_square()
    z = 2
    duoplot.axis([-2, -2, z - 2], [2, 2, z + 2])
    duoplot.view(-30, 30)
    duoplot.disable_scale_on_rotation()
    duoplot.global_illumination(duoplot.Vec3D(2.0, 2.0, 2.0))

    num_faces = len(bb.mesh_list[0].faces)

    indices = num_faces * [duoplot.IndexTriplet(0, 0, 0)]
    faces = bb.mesh_list[0].faces

    for i in range(0, num_faces):
        indices[i] = duoplot.IndexTriplet(
            faces[i][0],
            faces[i][1],
            faces[i][2],
        )

    vertices_original = vertices.copy()

    phase = 0.0

    t_elevation = np.pi / 2.0 + np.pi / 4.0

    n_its = 1000
    azimuth = -160
    amplitude = 1.0

    decay = 0.997

    colors = (np.random.rand(num_faces, 3) * 255).astype(np.uint8)
    colors[:, 0] = 0
    colors[:, 1] = 0
    colors[:, 2] = 0

    num_points = vertices.shape[0]

    point_colors = (np.random.rand(num_points, 3) * 255).astype(np.uint8)
    point_colors[:, 0] = 0
    point_colors[:, 1] = 0
    point_colors[:, 2] = 0

    x_interval = [np.min(vertices[:, 0]), np.max(vertices[:, 0])]

    dx = x_interval[1] - x_interval[0]

    offset = 0.0

    x_distances = np.zeros(num_faces, dtype=np.float32)

    point_x_distances = np.zeros(num_points, dtype=np.float32)

    for k in range(0, num_faces):
        idx_triplet = indices[k]
        p0 = vertices[idx_triplet.i0][0]
        p1 = vertices[idx_triplet.i1][0]
        p2 = vertices[idx_triplet.i2][0]

        p_mean = (p0 + p1 + p2) / 3.0

        d = (p_mean - x_interval[0]) / dx

        x_distances[k] = d

    for k in range(0, num_points):
        d = (vertices[k, 0] - x_interval[0]) / dx
        point_x_distances[k] = d

    for i in range(0, 1500):
        y_vec = np.sin(vertices[:, 0] * 5.0 + phase) * 0.5 * amplitude
        vertices[:, 1] = vertices_original[:, 1] + y_vec

        amplitude = amplitude * decay

        do = x_distances + offset
        color_map_jet(do - np.floor(do), colors)

        offset = offset + 0.01

        if i == n_its / 2:
            decay = 0.95

        if i > 1000:
            # Draw with only point cloud
            do = point_x_distances + offset
            color_map_jet(do - np.floor(do), point_colors)
            decay = 1.01
            amplitude = 0.3

            duoplot.scatter3(
                vertices[:, 0],
                vertices[:, 1],
                vertices[:, 2],
                colors=point_colors,
                scatter_style=duoplot.properties.ScatterStyle.DISC,
                point_size=5,
            )

        elif i > 800:
            # Draw with only edges, and point cloud
            do = point_x_distances + offset
            color_map_jet(do - np.floor(do), point_colors)

            duoplot.draw_mesh(
                vertices,
                indices,
                face_color=duoplot.properties.FaceColor.NONE,
            )
            duoplot.scatter3(
                vertices[:, 0],
                vertices[:, 1],
                vertices[:, 2],
                colors=point_colors,
                scatter_style=duoplot.properties.ScatterStyle.DISC,
                point_size=5,
            )

        elif i > 600:
            # Draw with only edges
            duoplot.draw_mesh(
                vertices,
                indices,
                face_color=duoplot.properties.FaceColor.NONE,
            )
        elif i > 375:
            # Draw with edges
            duoplot.draw_mesh(vertices, indices, colors=colors)
        else:
            # Draw without edges
            duoplot.draw_mesh(
                vertices,
                indices,
                colors=colors,
                edge_color=duoplot.properties.EdgeColor.NONE,
            )

        duoplot.flush_current_element()

        phase = phase + 0.1
        duoplot.view(azimuth, np.sin(t_elevation) * 10)
        t_elevation += 0.01

        azimuth -= 0.5
        if azimuth < -180.0:
            azimuth = 180.0

        duoplot.soft_clear_view()


def test_las_file():
    f_name = "Africa_Palace.las"
    # f_name = "laman_mahkota.laz"

    las_path = "../../../misc_data/" + f_name

    if not os.path.exists(las_path):
        las_path = "misc_data/" + f_name

    with pylas.open(las_path) as fh:
        las = fh.read()

        stride = 50

        x = las.x[0::stride]
        y = las.y[0::stride]
        z = las.z[0::stride]

        x = x - np.mean(x)
        y = y - np.mean(y)
        z = z - np.mean(z)

        red = (las.red[0::stride] / 255).astype(np.uint8)
        green = (las.green[0::stride] / 255).astype(np.uint8)
        blue = (las.blue[0::stride] / 255).astype(np.uint8)

        colors = np.array([red, green, blue]).T

        x_interval = [np.min(x), np.max(x)]
        y_interval = [np.min(y), np.max(y)]
        z_interval = [np.min(z), np.max(z)]

        min_val = np.min([x_interval[0], y_interval[0], z_interval[0]])
        max_val = np.max([x_interval[1], y_interval[1], z_interval[1]])

        duoplot.set_current_element("p_view_0")
        duoplot.clear_view()
        duoplot.axes_square()
        duoplot.disable_scale_on_rotation()

        duoplot.axis([min_val, min_val, min_val], [max_val, max_val, max_val])

        duoplot.scatter3(
            x,
            y,
            z,
            colors=colors,
            color=duoplot.properties.Color(0, 0, 0),
            scatter_style=duoplot.properties.ScatterStyle.DISC,
            point_size=3,
        )


def test_3d_map():
    import trimesh

    map_path = "/Users/danielpi/work/dvs/map_data/map3d.glb"
    glbd = trimesh.load(map_path)

    vertices = None
    faces = np.array([])

    for k, v in glbd.geometry.items():
        va = np.array(v.vertices.T)
        fs = np.array(v.faces)

        # if vertices is None:
        #     vertices_1 = va
        #     faces_1 = fs
        # else:
        a = 1
        # vertices = np.concatenate((vertices, v.vertices.T), axis=1)
        # faces = np.concatenate((faces, np.array(v.faces) - 3), axis=0)
        if k == "GLTF":
            continue

        vertices = v.vertices.T
        faces = np.array(v.faces)
        colors = v.visual.to_color().vertex_colors
        break

    vertices = vertices.T
    indices = [
        duoplot.IndexTriplet(np.uint32(q[0]), np.uint32(q[1]), np.uint32(q[2]))
        for q in faces
    ]

    # indices = [indices[0], indices[1], indices[2], indices[3]]
    # vertices i (X, 3) in shape
    # indices is list of IndexTriplet
    import copy

    vertices_new = copy.deepcopy(vertices)
    vertices_new[:, 1] = vertices[:, 2]
    vertices_new[:, 2] = vertices[:, 1]

    # colors is (X, 3), np.uint8, same size as indices

    colors_new = []

    for idx in indices:
        c0 = np.float32(colors[idx.i0][0:3])
        c1 = np.float32(colors[idx.i1][0:3])
        c2 = np.float32(colors[idx.i2][0:3])
        c = np.round((c0 + c1 + c2) / 3.0).astype(np.uint8)
        colors_new.append(c)

    colors_new = np.array(colors_new)

    duoplot.set_current_element("p_view_0")
    duoplot.clear_view()

    duoplot.axes_square()
    duoplot.disable_scale_on_rotation()

    duoplot.global_illumination(duoplot.Vec3D(2.0, 2.0, 2.0))

    zo = -10.0
    s = 1.0
    duoplot.axis(
        duoplot.Vec3D(0.0, 0.0, 0.0 + zo), duoplot.Vec3D(80.0 * s, 80.0 * s, 20.0 * s + zo)
    )

    duoplot.draw_mesh(
        vertices_new,
        indices,
        colors=colors_new,
        edge_color=duoplot.properties.EdgeColor.NONE,
    )

    duoplot.soft_clear_view()
    azimuth = -180.0

    for i in range(0, 1000):
        time.sleep(0.01)
        duoplot.view(azimuth, 21.0)

        azimuth += 0.4
        if azimuth > 180.0:
            azimuth = -180.0

        if i == 300:
            duoplot.draw_mesh(
                vertices_new,
                indices,
                colors=colors_new,
                edge_color=duoplot.properties.EdgeColor.BLACK,
            )

            duoplot.soft_clear_view()
        elif i == 500:
            duoplot.draw_mesh(
                vertices_new,
                indices,
                edge_color=duoplot.properties.EdgeColor.BLACK,
                color_map=duoplot.properties.ColorMap.JET_SOFT,
            )

            duoplot.soft_clear_view()
        elif i == 900:

            duoplot.draw_mesh(
                vertices_new,
                indices,
                edge_color=duoplot.properties.EdgeColor.NONE,
                color_map=duoplot.properties.ColorMap.JET_SOFT,
            )

            duoplot.soft_clear_view()
