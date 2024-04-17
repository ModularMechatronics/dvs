import numpy as np


def parametric_spiral(theta):
    r = np.exp(0.1 * theta)
    x = r * np.cos(theta)
    y = r * np.sin(theta)
    return x, y


def cardioid_with_decay(theta):
    r = 1 - np.exp(-0.2 * theta)
    x = r * np.cos(theta)
    y = r * np.sin(theta)
    return x, y


def lissajous_curve(t):
    x = np.sin(3 * t + np.pi / 2)
    y = np.cos(2 * t)
    return x, y


def logarithmic_spiral(theta):
    a = 0.1
    b = 0.2
    r = np.exp(a + b * theta)
    x = r * np.cos(theta)
    y = r * np.sin(theta)
    return x, y


def butterfly_curve(t):
    x = np.sin(t) * (np.exp(np.cos(t)) - 2 * np.cos(4 * t) - np.sin(t / 12) ** 5)
    y = np.cos(t) * (np.exp(np.cos(t)) - 2 * np.cos(4 * t) - np.sin(t / 12) ** 5)
    return x, y


def rose_curve(theta):
    k = 6
    r = np.cos(k * theta)
    x = r * np.cos(theta)
    y = r * np.sin(theta)
    return x, y


def hypotrochoid(theta):
    R, r, d = 5, 3, 2
    x = (R - r) * np.cos(theta) + d * np.cos((R - r) / r * theta)
    y = (R - r) * np.sin(theta) - d * np.sin((R - r) / r * theta)
    return x, y


def epicycloid(theta):
    R, r, d = 5, 3, 2
    x = (R + r) * np.cos(theta) - d * np.cos((R + r) / r * theta)
    y = (R + r) * np.sin(theta) - d * np.sin((R + r) / r * theta)
    return x, y


def trifolium_curve(theta):
    r = np.sin(3 * theta)
    x = r * np.cos(theta)
    y = r * np.sin(theta)
    return x, y


def archimedean_spiral(theta):
    a = 0.1
    b = 2
    r = a + b * theta
    x = r * np.cos(theta)
    y = r * np.sin(theta)
    return x, y


def logarithmic_helix(theta):
    a = 0.1
    b = 0.2
    c = 0.5
    r = a * np.exp(-b * theta) * np.sin(c * theta)
    x = r * np.cos(theta)
    y = r * np.sin(theta)
    return x, y


def parabolic_spiral(theta):
    a = 0.1
    b = 0.01
    r = a + b * theta**2
    x = r * np.cos(theta)
    y = r * np.sin(theta)
    return x, y


def exponential_spiral(theta):
    a = 0.1
    b = 0.2
    r = a * np.exp(b * theta)
    x = r * np.cos(theta)
    y = r * np.sin(theta)
    return x, y


def limacon(theta):
    a = 0.4
    b = 0.3
    r = a + b * np.cos(theta)
    x = r * np.cos(theta)
    y = r * np.sin(theta)
    return x, y


def conchoid(theta):
    a = 0.5
    b = 0.1
    d = 0.2
    r = a + b / np.cos(theta - d)
    x = r * np.cos(theta)
    y = r * np.sin(theta)
    return x, y


def logarithmic_spiral_2(theta):
    a = 0.02
    b = 0.2
    r = np.exp(a * theta) / b
    x = r * np.cos(theta)
    y = r * np.sin(theta)
    return x, y


def square_wave(theta):
    frequency = 5
    amplitude = 1
    x = np.linspace(0, 2 * np.pi, len(theta))
    y = amplitude * np.sign(np.sin(frequency * x))
    return x, y


def hypotrochoid_2(theta):
    R, r, d = 5, 3, 2
    x = (R - r) * np.cos(theta) + d * np.cos((R - r) / r * theta)
    y = (R - r) * np.sin(theta) - d * np.sin((R - r) / r * theta)
    return x, y


def conic_spiral(theta):
    a = 0.02
    b = 0.05
    r = a / (theta + b)
    x = r * np.cos(theta)
    y = r * np.sin(theta)
    return x, y


def logarithmic_wave(theta):
    a = 0.2
    b = 0.5
    c = 2
    r = a * np.exp(-b * theta) * np.sin(c * theta)
    x = r * np.cos(theta)
    y = r * np.sin(theta)
    return x, y


def exponential_wave(theta):
    a = 0.3
    b = 0.1
    c = 0.2
    r = a * np.exp(-b * theta) * np.cos(c * theta)
    x = r * np.cos(theta)
    y = r * np.sin(theta)
    return x, y


def cardioid_with_twist(theta):
    a = 1
    b = 0.2
    twist = 0.3
    r = a * (1 + b * np.cos(theta + twist))
    x = r * np.cos(theta)
    y = r * np.sin(theta)
    return x, y


def logarithmic_spiral_3(theta):
    a = 0.1
    b = 0.2
    c = 0.3
    r = a * np.exp(b * theta) * np.sin(c * theta)
    x = r * np.cos(theta)
    y = r * np.sin(theta)
    return x, y


def sinusoidal_spiral(theta):
    a = 0.1
    b = 0.2
    r = a + b * np.sin(theta)
    x = r * np.cos(theta)
    y = r * np.sin(theta)
    return x, y


def square_wave_spiral(theta):
    frequency = 5
    amplitude = 1
    x = np.linspace(0, 2 * np.pi, len(theta))
    y = amplitude * np.sign(np.sin(frequency * x))
    r = np.linspace(0.1, 1, len(theta))
    x = r * x * np.cos(x)
    y = r * y * np.sin(y)
    return x, y

