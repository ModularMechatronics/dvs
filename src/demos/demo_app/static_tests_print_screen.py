import os
import subprocess
from PIL import ImageGrab
import time

if __name__ == "__main__":

    exe_path = "/Users/danielpi/work/dvs/src/build/demos/demos"

    commands = [["small", "basic"],
                ["small", "bump"],
                ["small", "fake_contour"],
                ["small", "lorenz"],
                ["small", "scatter_cluster"],
                ["small", "scatter_color_map"],
                ["small", "scatter_samples"],
                ["small", "scatter_small_points"],
                ["small", "scatter_varying_size"],
                ["small", "stocks"],
                ["small", "transparent_fill_below_plot"]]
    
    time.sleep(2)
    for cm in [commands[0]]:
        subprocess.run([exe_path, "small", "scatter_cluster"])
        ss_region = (0, 0, 600, 600)
        ss_img = ImageGrab.grab(ss_region)
        ss_img.save("SS3.png")
        # subprocess.run([exe_path, cm[0], cm[1]])