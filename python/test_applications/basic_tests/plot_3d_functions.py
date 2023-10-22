import numpy as np


def helix(t):
    radius = 1
    height = 0.1
    omega = 2 * np.pi
    x = radius * np.cos(omega * t)
    y = radius * np.sin(omega * t)
    z = height * t
    return x, y, z


def torus(t):
    R = 1.0  # Major radius (radius of the tube)
    r = 0.3  # Minor radius (radius of the torus itself)
    u = 2 * np.pi * t
    x = (R + r * np.cos(u)) * np.cos(u)
    y = (R + r * np.cos(u)) * np.sin(u)
    z = r * np.sin(u)
    return x, y, z


def trefoil_knot(t):
    x = np.sin(t) + 2 * np.sin(2 * t)
    y = np.cos(t) - 2 * np.cos(2 * t)
    z = -np.sin(3 * t)
    return x, y, z


def figure_eight(t):
    x = np.sin(t)
    y = np.sin(2 * t) / 2
    z = np.cos(2 * t) / 2
    return x, y, z


def spherical_helix(t):
    a = 1
    b = 0.1
    omega = 2 * np.pi
    x = a * np.cos(omega * t) * np.sin(b * t)
    y = a * np.sin(omega * t) * np.sin(b * t)
    z = a * np.cos(b * t)
    return x, y, z


def mobius_strip(t):
    width = 0.5
    twist = 2
    u = t - np.pi
    x = (1 + width * np.cos(u / 2)) * np.cos(u)
    y = (1 + width * np.cos(u / 2)) * np.sin(u)
    z = width * np.sin(u / 2) * np.cos(twist * u / 2)
    return x, y, z


def seashell(t):
    a = 1
    b = 0.4
    omega = 2 * np.pi / 3
    phi = 0.2 * t
    x = a * (1 - b * np.sin(omega * t + phi)) * np.cos(t)
    y = a * (1 - b * np.sin(omega * t + phi)) * np.sin(t)
    z = a * b * np.cos(omega * t + phi)
    return x, y, z


def knot(t):
    a = 0.5
    b = 0.5
    x = (a + b * np.cos(6 * t)) * np.cos(4 * t)
    y = (a + b * np.cos(6 * t)) * np.sin(4 * t)
    z = b * np.sin(6 * t)
    return x, y, z


def corkscrew(t):
    radius = 0.3
    pitch = 0.1
    omega = 2 * np.pi
    x = radius * np.cos(omega * t)
    y = radius * np.sin(omega * t)
    z = pitch * t
    return x, y, z


def trefoil_tube(t):
    a = 1
    b = 0.2
    x = (a + b * np.cos(3 * t)) * np.cos(2 * t)
    y = (a + b * np.cos(3 * t)) * np.sin(2 * t)
    z = b * np.sin(3 * t)
    return x, y, z


def horn(t):
    a = 1
    b = 0.2
    x = (a + b * np.cos(t)) * np.cos(t)
    y = (a + b * np.cos(t)) * np.sin(t)
    z = b * np.sin(t)
    return x, y, z


def pyramid(t):
    a = 0.3
    b = 0.2
    x = (a - b * t) * np.cos(t)
    y = (a - b * t) * np.sin(t)
    z = t
    return x, y, z


def spherical_knot(t):
    a = 1
    b = 0.3
    omega = 2 * np.pi
    x = a * np.cos(omega * t) * np.sin(b * t)
    y = a * np.sin(omega * t) * np.sin(b * t)
    z = a * np.cos(b * t)
    return x, y, z


def random_3d_curve(t):
    x = np.random.rand() * np.cos(t)
    y = np.random.rand() * np.sin(t)
    z = np.random.rand() * t
    return x, y, z


def helical_torus(t):
    R = 1.0  # Major radius (radius of the torus itself)
    r = 0.3  # Minor radius (radius of the tube)
    u = 2 * np.pi * t
    v = 2 * np.pi * t
    x = (R + r * np.cos(v)) * np.cos(u)
    y = (R + r * np.cos(v)) * np.sin(u)
    z = r * np.sin(v)
    return x, y, z


def pear_shape(t):
    a = 1
    b = 0.4
    omega = 2 * np.pi / 3
    x = np.sin(t)
    y = a * np.cos(omega * t) * np.exp(-b * t)
    z = np.cos(t)
    return x, y, z


def star_torus(t):
    R = 1.0
    r = 0.3
    u = 2 * np.pi * t
    v = 4 * np.pi * t
    x = (R + r * np.cos(v)) * np.cos(u)
    y = (R + r * np.cos(v)) * np.sin(u)
    z = r * np.sin(v)
    return x, y, z


def toroidal_helix(t):
    R = 1.0
    r = 0.3
    a = 0.1
    u = 2 * np.pi * t
    v = a * t
    x = (R + r * np.cos(v)) * np.cos(u)
    y = (R + r * np.cos(v)) * np.sin(u)
    z = r * np.sin(v)
    return x, y, z


def horn_torus(t):
    R = 1.0
    r = 0.3
    u = 2 * np.pi * t
    v = 0.2 * t
    x = (R + r * np.cos(v)) * np.cos(u)
    y = (R + r * np.cos(v)) * np.sin(u)
    z = r * np.sin(v)
    return x, y, z


def twisted_torus(t):
    R = 1.0
    r = 0.3
    u = 2 * np.pi * t
    v = 0.5 * np.sin(u)
    x = (R + r * np.cos(v)) * np.cos(u)
    y = (R + r * np.cos(v)) * np.sin(u)
    z = r * np.sin(v)
    return x, y, z


def star_knot(t):
    a = 1.0
    b = 0.4
    omega = 2 * np.pi / 3
    x = a * np.cos(t) * (np.cos(omega * t) + b)
    y = a * np.sin(t) * (np.cos(omega * t) + b)
    z = a * np.sin(omega * t)
    return x, y, z


def random_3d_spiral(t):
    a = np.random.rand() * 0.5
    b = np.random.rand() * 0.5
    c = np.random.rand() * 0.5
    x = a * np.cos(t)
    y = b * np.sin(t)
    z = c * t
    return x, y, z


def double_helix_3d(t):
    a = 1
    b = 0.1
    c = 5
    x = a * np.cos(b * t)
    y = a * np.sin(b * t)
    z = c * t
    return x, y, z


def knot_3d(t):
    a = 0.5
    b = 0.5
    c = 0.2
    x = (a + b * np.cos(6 * t)) * np.cos(4 * t)
    y = (a + b * np.cos(6 * t)) * np.sin(4 * t)
    z = c * np.sin(6 * t)
    return x, y, z


def twisted_cord(t):
    a = 1
    b = 0.1
    c = 0.2
    x = a * np.cos(b * t) * np.sin(c * t)
    y = a * np.sin(b * t) * np.sin(c * t)
    z = a * np.cos(c * t)
    return x, y, z


def random_3d_curve_2(t):
    x = np.random.rand() * np.cos(t)
    y = np.random.rand() * np.sin(t)
    z = np.random.rand() * np.cos(2 * t)
    return x, y, z
