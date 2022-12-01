# from cgi import test
# import matplotlib.pyplot as plt
import numpy as np

rainbow = [
        [1.0, 1.0, 0.0],
        [0.0, 1.0, 0.0],
        [0.0, 1.0, 1.0],
        [0.0, 0.0, 1.0],
        [1.0, 0.0, 1.0],
        [1.0, 0.0, 0.0]
]

test_table = [
        [0.0, 0.0, 1.0],
        [1.0, 0.8, 0.1],
        [0.5, 1.0, 0.2]
]

test_table = [
        [0.0, 0.0, 1.0],
        [1.0, 0.0, 0.1],
        [0.0, 0.5, 0.2],
        [1.0, 0.0, 1.0],
        [0.0, 0.0, 0.9],
        [1.0, 0.0, 0.5],
        [0.0, 0.2, 0.0]
]

magma = [
        [6.0 / 255.0, 3.0 / 255.0, 23.0 / 255.0],
        [109.0 / 255.0, 15.0 / 255.0, 128.0 / 255.0],
        [237.0 / 255.0, 101.0 / 255.0, 93.0 / 255.0],
        [250.0 / 255.0, 242.0 / 255.0, 180.0 / 255.0]
    ]

viridis = [
        [67.0 / 255.0, 0.0 / 255.0, 84.0 / 255.0],
        [71.0 / 255.0, 42.0 / 255.0, 124.0 / 255.0],
        [62.0 / 255.0, 84.0 / 255.0, 139.0 / 255.0],
        [54.0 / 255.0, 115.0 / 255.0, 141.0 / 255.0],
        [50.0 / 255.0, 143.0 / 255.0, 139.0 / 255.0],
        [56.0 / 255.0, 172.0 / 255.0, 129.0 / 255.0],
        [87.0 / 255.0, 196.0 / 255.0, 109.0 / 255.0],
        [159.0 / 255.0, 220.0 / 255.0, 65.0 / 255.0],
        [246.0 / 255.0, 233.0 / 255.0, 51.0 / 255.0],
    ]

jet = [
        [19.0 / 255.0, 0.0 / 255.0, 130.05 / 255.0],
        [48.0 / 255.0, 12.0 / 255.0, 253.05 / 255.0],
        [68.0 / 255.0, 229.0 / 255.0, 245.05 / 255.0],
        [148.0 / 255.0, 255.0 / 255.0, 108.05 / 255.0],
        [250.0 / 255.0, 205.0 / 255.0, 34.05 / 255.0],
        [247.0 / 255.0, 49.0 / 255.0, 21.05 / 255.0],
        [127.0 / 255.0, 0.0 / 255.0, 5.05 / 255.0],
    ]

rainbow_pastel = [
        [255.0 / 255.0, 154.0 / 255.0, 162.0 / 255.0],
        [255.0 / 255.0, 183.0 / 255.0, 178.0 / 255.0],
        [255.0 / 255.0, 218.0 / 255.0, 194.0 / 255.0],
        [226.0 / 255.0, 240.0 / 255.0, 205.0 / 255.0],
        [180.0 / 255.0, 234.0 / 255.0, 216.0 / 255.0],
        [199.0 / 255.0, 206.0 / 255.0, 233.0 / 255.0],
    ]

function_body = ["    if(value < 0.0)",
"    {",
"        value = 0.0;",
"    }",
"    else if(value >= 1.0)",
"    {",
"        value = 0.99999;",
"    }",
]

def calculate_intermediate_val(bp_val, next_bp_val, fractional_part):
    if bp_val > next_bp_val:
        d = bp_val - next_bp_val
        return bp_val - fractional_part * d
    else:
        d = next_bp_val - bp_val
        return bp_val + fractional_part * d


def interpolate_colors(values, colormap_breakpoints):

    num_bp_values = len(colormap_breakpoints) - 1

    colormap_breakpoints.append(colormap_breakpoints[-1])

    r = np.linspace(0.0, 1.0, num=100)
    g = np.linspace(0.0, 1.0, num=100)
    b = np.linspace(0.0, 1.0, num=100)
    fr = np.linspace(0.0, 1.0, num=100)

    for val_idx, val in enumerate(values):
        full_range_value = val * float(num_bp_values)

        if full_range_value == float(num_bp_values):
            full_range_value = float(num_bp_values) - 0.00001

        integer_part = np.floor(full_range_value)
        fractional_part = full_range_value - integer_part
        fr[val_idx] = fractional_part

        bp = colormap_breakpoints[int(integer_part)]
        next_bp = colormap_breakpoints[int(integer_part) + 1]

        r_same = bp[0] == next_bp[0]
        g_same = bp[1] == next_bp[1]
        b_same = bp[2] == next_bp[2]

        if r_same:
            r[val_idx] = bp[0]
        else:
            r[val_idx] = calculate_intermediate_val(bp[0], next_bp[0], fractional_part)

        if g_same:
            g[val_idx] = bp[1]
        else:
            g[val_idx] = calculate_intermediate_val(bp[1], next_bp[1], fractional_part)

        if b_same:
            b[val_idx] = bp[2]
        else:
            b[val_idx] = calculate_intermediate_val(bp[2], next_bp[2], fractional_part)

    return r, g, b, fr

def print_diff(bp_val, next_bp_val, fs, color_char):

    if bp_val > next_bp_val:
        d = bp_val - next_bp_val
        if d == 1.0:
            d_string = ""
        else:
            d_string = f" * {d}"

        if bp_val == 0.0:
            bp_val_string = ""
        else:
            bp_val_string = f"{bp_val}"
        fs.append(f"        {color_char} = {bp_val_string} - fraction_part{d_string};")
    else:
        d = next_bp_val - bp_val
        if d == 1.0:
            d_string = ""
        else:
            d_string = f" * {d}"
        
        if bp_val == 0.0:
            bp_val_string = ""
        else:
            bp_val_string = f"{bp_val} + "
        fs.append(f"        {color_char} = {bp_val_string}fraction_part{d_string};")


def print_colormap_function(colormap_name, colormap_breakpoints):
    num_bp_values = len(colormap_breakpoints)

    fs = []
    fs.append(f"vec3 calculateColormap{colormap_name}(float value)")
    fs.append("{")
    fs.extend(function_body)
    fs.append("")

    fs.append(f"    float full_range_value = value * {float(num_bp_values - 1)};")
    fs.append(f"    float integer_part = floor(full_range_value);")
    fs.append(f"    float fraction_part = full_range_value - integer_part;")

    fs.append("")
    fs.append("    float r = 0.0, g = 0.0, b = 0.0;")
    fs.append("")

    fs.append(f"    switch(int(integer_part))")
    fs.append("    {")
    for idx, bp in enumerate(colormap_breakpoints[:-1]):

        fs.append(f"    case {idx}:")
        next_bp = colormap_breakpoints[idx + 1]
        r_same = bp[0] == next_bp[0]
        g_same = bp[1] == next_bp[1]
        b_same = bp[2] == next_bp[2]

        if r_same:
            fs.append(f"        r = {bp[0]};")
        else:
            print_diff(bp[0], next_bp[0], fs, "r")
        
        if g_same:
            fs.append(f"        g = {bp[1]};")
        else:
            print_diff(bp[1], next_bp[1], fs, "g")
        
        if b_same:
            fs.append(f"        b = {bp[2]};")
        else:
            print_diff(bp[2], next_bp[2], fs, "b")

        fs.append("        break;")

    fs.append("    }")

    fs.append("")
    fs.append("    return vec3(r, g, b);")
    fs.append("}")

    for l in fs:
        print(l)

if __name__ == "__main__":
    colormaps = {"Rainbow": rainbow, "Magma": magma, "Viridis": viridis, "Jet": jet, "RainbowPastel": rainbow_pastel}

    for colormap_name, colormap_breakpoints in colormaps.items():
        print_colormap_function(colormap_name, colormap_breakpoints)
    
    d = np.linspace(0.0, 1.0, num=100)

    r, g, b, fr = interpolate_colors(d, magma)

    """plt.plot(d, r, 'r')
    plt.plot(d, g, 'g')
    plt.plot(d, b, 'b')
    plt.plot(d, fr, 'y--')
    plt.show()"""
