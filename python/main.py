
import dvs
import numpy as np


def test_plot():
    dvs.set_current_element("view_00")
    dvs.hard_clear()

    for i in range(0, 100):
        dvs.clear()
        x = np.linspace(0, 3, 100, dtype=np.float32)
        y = np.sin(x * 5.0 + 0.1 * i)

        dvs.axis([-10, -10, -10], [10, 10, 10])
        dvs.view(0, 90)
        dvs.plot(x, y)  # ,color=dvs.Color(23, 25, 66) alpha=100, line_width=13


def test_plot3():
    dvs.set_current_element("view_00")

    dvs.hard_clear()
    x = np.linspace(0, 3, 100, dtype=np.float32)
    y = np.sin(x * 5.0)
    z = np.cos(x * 5.0)

    dvs.axis([-10, -10, -10], [10, 10, 10])
    dvs.view(0, 90)
    dvs.plot3(x, y, z)


def test_surf():
    dvs.set_current_element("view_00")

    dvs.hard_clear()
    x = np.linspace(-0.1, 3, 10, dtype=np.float32)
    y = np.linspace(-0.1, 3, 10, dtype=np.float32)
    x, y = np.meshgrid(x, y)
    r = np.sqrt(x * x + y * y)
    z = np.sin(r) / r

    dvs.axis([-10, -10, -10], [10, 10, 10])
    dvs.view(0, 90)
    dvs.surf(x, y, z)


if __name__ == "__main__":
    test_surf()
