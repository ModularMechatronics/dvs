
import dvs
import numpy as np

if __name__ == "__main__":
    x = np.linspace(0, 3, 100, dtype=np.float32)
    y = np.sin(x * 5.0)

    dvs.plot(x, y, color=dvs.Color(23, 25, 66)) # , alpha=100, line_width=13
    # dvs.plot(x, y)
