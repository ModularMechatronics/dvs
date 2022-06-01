import os
import numpy as np
import matplotlib.pyplot as plt

data_path = "build/output.txt"


if __name__ == "__main__":

    with open(data_path, "r") as f:
        data = f.readlines()
    data_fields = data[0].rstrip("\n").split(",")
    data_array = np.array([[float(q) for q in d.rstrip("\n").split(",")] for d in data[1:]])

    data = {}
    for i, f in enumerate(data_fields):
        data[f] = data_array[:, i]

    I = np.sqrt(data["iu"] ** 2.0 + data["iv"] ** 2.0 + data["iw"] ** 2.0)

    fig, (ax1, ax2, ax3) = plt.subplots(3, 1)
    fig.suptitle('A tale of 2 subplots')

    ax1.plot(data["t"], data["theta"])
    ax1.set_ylabel('theta')

    ax2.plot(data["t"], data["omega"])
    ax2.set_xlabel('time (s)')
    ax2.set_ylabel('omega')

    ax3.plot(data["t"], data["iu"])
    ax3.plot(data["t"], data["iv"])
    ax3.plot(data["t"], data["iw"])
    ax3.set_xlabel('time (s)')
    ax3.set_ylabel('current')

    plt.show()