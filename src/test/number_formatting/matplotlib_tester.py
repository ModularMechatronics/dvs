import matplotlib.pyplot as plt
import numpy as np


if __name__ == "__main__":

    d = np.linspace(0.0, 1.0, num=100)
    y = np.sin(2 * np.pi * d)

    # y = y * 1e-47
    # y = y * 1e300
    y = y * 0.000001 + 11396

    plt.plot(d, y, "r")
    plt.show()
