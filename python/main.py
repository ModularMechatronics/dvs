
import dvs
import numpy as np

if __name__ == "__main__":

    dvs.set_current_element("view_00")

    for i in range(0, 100):
        dvs.clear_view()
        x = np.linspace(0, 3, 100, dtype=np.float32)
        y = np.sin(x * 5.0 + 0.1 * i)

        dvs.axis([-10, -10, -10], [10, 10, 10])
        dvs.view(0, 90)
        dvs.plot(x, y)  # ,color=dvs.Color(23, 25, 66) alpha=100, line_width=13
