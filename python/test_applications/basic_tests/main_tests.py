import dvs
import numpy as np

import pylas
import time
import sys
import surf_functions
import plot_2d_functions
import plot_3d_functions
import pywavefront
import os


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


def color_map_jet(d, colors):
    value = np.float32(d)
    value[value < 0.0] = 0.0
    value[value >= 1.0] = 0.99999

    full_range_value = value * np.float32(11.0)
    integer_part = np.floor(full_range_value).astype(np.int16)
    fraction_part = full_range_value - integer_part

    colors[integer_part == 0, 0] = ((0.3686274509803922 - fraction_part[integer_part == 0] * 0.17254901960784316) * 255.0).astype(np.uint8)
    colors[integer_part == 0, 1] = ((0.30980392156862746 + fraction_part[integer_part == 0] * 0.22352941176470587) * 255.0).astype(np.uint8)
    colors[integer_part == 0, 2] = ((0.6352941176470588 + fraction_part[integer_part == 0] * 0.10588235294117654) * 255.0).astype(np.uint8)

    colors[integer_part == 1, 0] = ((0.19607843137254902 + fraction_part[integer_part == 1] * 0.203921568627451) * 255.0).astype(np.uint8)
    colors[integer_part == 1, 1] = ((0.5333333333333333 + fraction_part[integer_part == 1] * 0.22745098039215683) * 255.0).astype(np.uint8)
    colors[integer_part == 1, 2] = ((0.7411764705882353 - fraction_part[integer_part == 1] * 0.09411764705882353) * 255.0).astype(np.uint8)

    colors[integer_part == 2, 0] = ((0.4 + fraction_part[integer_part == 2] * 0.2705882352941176) * 255.0).astype(np.uint8)
    colors[integer_part == 2, 1] = ((0.7607843137254902 + fraction_part[integer_part == 2] * 0.10588235294117654) * 255.0).astype(np.uint8)
    colors[integer_part == 2, 2] = ((0.6470588235294118 - fraction_part[integer_part == 2] * 0.0039215686274509665) * 255.0).astype(np.uint8)

    colors[integer_part == 3, 0] = ((0.6705882352941176 + fraction_part[integer_part == 3] * 0.2313725490196079) * 255.0).astype(np.uint8)
    colors[integer_part == 3, 1] = ((0.8666666666666667 + fraction_part[integer_part == 3] * 0.09411764705882353) * 255.0).astype(np.uint8)
    colors[integer_part == 3, 2] = ((0.6431372549019608 - fraction_part[integer_part == 3] * 0.04705882352941182) * 255.0).astype(np.uint8)

    colors[integer_part == 4, 0] = ((0.9019607843137255 + fraction_part[integer_part == 4] * 0.0980392156862745) * 255.0).astype(np.uint8)
    colors[integer_part == 4, 1] = ((0.9607843137254902 + fraction_part[integer_part == 4] * 0.039215686274509776) * 255.0).astype(np.uint8)
    colors[integer_part == 4, 2] = ((0.596078431372549 + fraction_part[integer_part == 4] * 0.15294117647058825) * 255.0).astype(np.uint8)

    colors[integer_part == 5, 0] = ((1.0 - fraction_part[integer_part == 5] * 0.0039215686274509665) * 255.0).astype(np.uint8)
    colors[integer_part == 5, 1] = ((1.0 - fraction_part[integer_part == 5] * 0.1215686274509804) * 255.0).astype(np.uint8)
    colors[integer_part == 5, 2] = ((0.7490196078431373 - fraction_part[integer_part == 5] * 0.20392156862745103) * 255.0).astype(np.uint8)

    colors[integer_part == 6, 0] = ((0.996078431372549 - fraction_part[integer_part == 6] * 0.0039215686274509665) * 255.0).astype(np.uint8)
    colors[integer_part == 6, 1] = ((0.8784313725490196 - fraction_part[integer_part == 6] * 0.196078431372549) * 255.0).astype(np.uint8)
    colors[integer_part == 6, 2] = ((0.5450980392156862 - fraction_part[integer_part == 6] * 0.16470588235294115) * 255.0).astype(np.uint8)

    colors[integer_part == 7, 0] = ((0.9921568627450981 - fraction_part[integer_part == 7] * 0.03529411764705881) * 255.0).astype(np.uint8)
    colors[integer_part == 7, 1] = ((0.6823529411764706 - fraction_part[integer_part == 7] * 0.25490196078431376) * 255.0).astype(np.uint8)
    colors[integer_part == 7, 2] = ((0.3803921568627451 - fraction_part[integer_part == 7] * 0.11764705882352938) * 255.0).astype(np.uint8)

    colors[integer_part == 8, 0] = ((0.9568627450980393 - fraction_part[integer_part == 8] * 0.1215686274509804) * 255.0).astype(np.uint8)
    colors[integer_part == 8, 1] = ((0.42745098039215684 - fraction_part[integer_part == 8] * 0.18431372549019606) * 255.0).astype(np.uint8)
    colors[integer_part == 8, 2] = ((0.2627450980392157 + fraction_part[integer_part == 8] * 0.047058823529411764) * 255.0).astype(np.uint8)

    colors[integer_part == 9, 0] = ((0.8352941176470589 - fraction_part[integer_part == 9] * 0.21568627450980393) * 255.0).astype(np.uint8)
    colors[integer_part == 9, 1] = ((0.24313725490196078 - fraction_part[integer_part == 9] * 0.2392156862745098) * 255.0).astype(np.uint8)
    colors[integer_part == 9, 2] = ((0.30980392156862746 - fraction_part[integer_part == 9] * 0.05098039215686273) * 255.0).astype(np.uint8)

    colors[integer_part == 10, 0] = ((0.6196078431372549 - fraction_part[integer_part == 10] * 0.25098039215686274) * 255.0).astype(np.uint8)
    colors[integer_part == 10, 1] = ((0.00392156862745098 + fraction_part[integer_part == 10] * 0.3058823529411765) * 255.0).astype(np.uint8)
    colors[integer_part == 10, 2] = ((0.25882352941176473 + fraction_part[integer_part == 10] * 0.37647058823529406) * 255.0).astype(np.uint8)


def test_3d_obj():
    dvs.set_current_element("p_view_0")
    dvs.clear_view()

    x = np.linspace(0, 3, 500, dtype=np.float32)
    y = np.sin(x * 5.0)

    duck_path = "../../../misc_data/RubberDuck/RubberDuck.obj"

    if not os.path.exists(duck_path):
        duck_path = "misc_data/RubberDuck/RubberDuck.obj"

    bb = pywavefront.Wavefront(duck_path, create_materials=False, collect_faces=True)
    # create_materials=True,collect_faces=True)
    vertices = np.array(bb.vertices, dtype=np.float32)
    # vertices_t = vertices.T

    dvs.axes_square()
    z = 2
    dvs.axis([-2, -2, z - 2], [2, 2, z + 2])
    dvs.view(-30, 30)
    dvs.disable_scale_on_rotation()

    num_faces = len(bb.mesh_list[0].faces)

    indices = num_faces * [dvs.IndexTriplet(0, 0, 0)]
    faces = bb.mesh_list[0].faces

    for i in range(0, num_faces):
        indices[i] = dvs.IndexTriplet(
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

    x_interval = [np.min(vertices[:, 0]), np.max(vertices[:, 0])]

    dx = x_interval[1] - x_interval[0]

    offset = 0.0

    x_distances = np.zeros(num_faces, dtype=np.float32)

    for k in range(0, num_faces):
        idx_triplet = indices[k]
        p0 = vertices[idx_triplet.i0][0]
        p1 = vertices[idx_triplet.i1][0]
        p2 = vertices[idx_triplet.i2][0]

        p_mean = (p0 + p1 + p2) / 3.0

        d = (p_mean - x_interval[0]) / dx

        x_distances[k] = d

    for i in range(0, 1000):
        y_vec = np.sin(vertices[:, 0] * 5.0 + phase) * 0.5 * amplitude
        vertices[:, 1] = vertices_original[:, 1] + y_vec

        amplitude = amplitude * decay

        do = x_distances + offset
        color_map_jet(do - np.floor(do), colors)

        offset = offset + 0.01

        if i > (n_its / 4):
            dvs.draw_mesh(vertices, indices, colors=colors)
        else:
            dvs.draw_mesh(
                vertices,
                indices,
                colors=colors,
                edge_color=dvs.properties.EdgeColor.NONE,
            )
        if i == n_its / 2:
            decay = 0.95

        phase = phase + 0.1
        dvs.view(azimuth, np.sin(t_elevation) * 10)
        t_elevation += 0.01

        azimuth -= 0.5
        if azimuth < -180.0:
            azimuth = 180.0

        dvs.soft_clear_view()



def test_las_file():
    f_name = "Africa_Palace.las"
    f_name = "laman_mahkota.laz"

    las_path = "../../../misc_data/" + f_name

    if not os.path.exists(las_path):
        las_path = "misc_data/" + f_name

    with pylas.open(las_path) as fh:
        las = fh.read()

        stride = 10

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

        dvs.set_current_element("p_view_0")
        dvs.clear_view()
        dvs.axes_square()
        dvs.disable_scale_on_rotation()

        dvs.axis([min_val, min_val, min_val], [max_val, max_val, max_val])

        dvs.scatter3(x, y, z, colors=colors, color=dvs.properties.Color(0, 0, 0), scatter_style=dvs.properties.ScatterStyle.DISC, point_size=3)
