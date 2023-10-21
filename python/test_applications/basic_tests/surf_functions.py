import numpy as np


def saddle_surface(x, y):
    return x**2 - y**2


# 2. Paraboloid surface
def paraboloid_surface(x, y):
    return x**2 + y**2


# 3. Cone surface
def cone_surface(x, y):
    return np.sqrt(x**2 + y**2)


# 5. Hyperbolic cone
def hyperbolic_cone(x, y):
    return np.sqrt(x**2 - y**2)


# 9. Torus surface
def torus_surface(x, y):
    return np.cos(x) + np.cos(y)


# 10. Ripple surface
def ripple_surface(x, y):
    return np.sin(np.sqrt(x**2 + y**2))


# 11. Spiral surface
def spiral_surface(x, y):
    return np.sin(x + y)


# 12. Sine-cosine surface
def sine_cosine_surface(x, y):
    return np.sin(x) * np.cos(y)


# 13. Egg crate surface
def egg_crate_surface(x, y):
    return np.cos(x) * np.cos(y)


# 14. Sinc function surface
def sinc_surface(x, y):
    r = np.sqrt(x**2 + y**2)
    return np.sin(r) / r


# 15. Mexican hat surface (Ricker wavelet)
def mexican_hat_surface(x, y):
    r = np.sqrt(x**2 + y**2)
    return np.exp(-(r**2)) * (1 - r**2)


# 17. Möbius strip surface
def mobius_strip_surface(x, y):
    return np.sin(y) * (1 + x / (2 * np.pi))


# 18. Enneper surface
def enneper_surface(x, y):
    return (3 * x * (1 - x**2) - 3 * x * y**2) / (3 * (1 + x**2 + y**2) ** 2)


# 19. Klein bottle surface
def klein_bottle_surface(x, y):
    return (4 * (1 - y) * np.cos(x) - 2 * np.sin(2 * x) * np.sin(2 * y)) / 8


# 24. Boy's surface
def boys_surface(u, v):
    a, b = 2, 3  # Parameters for Boy's surface
    return np.sin(u) * (np.cos(v) + a * np.cos(2 * v) / 2) / b


# 25. Klein bottle (alternative parametrization)
def klein_bottle_alternative(u, v):
    a, b = 3, 1  # Parameters for Klein bottle
    return np.cos(u) * (1 + np.sin(u)) + a * np.cos(u) * np.cos(v)


# 26. Catenoid
def catenoid(u, v):
    a = 1  # Parameter for catenoid
    return a * np.cosh(v / a) * np.cos(u)


# 27. Helicoid
def helicoid(u, v):
    a = 1  # Parameter for helicoid
    return u * np.cos(v)


# 28. Roman surface
def roman_surface(u, v):
    a, b = 1, 1  # Parameters for Roman surface
    return a * u * (u**2 - 3 * v**2) / (u**2 + 3 * v**2)


# 30. Steiner surface
def steiner_surface(u, v):
    a, b = 1, 1  # Parameters for Steiner surface
    return a * np.cos(u) * np.sin(u) * np.cos(v)
