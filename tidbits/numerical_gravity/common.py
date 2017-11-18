#!/usr/bin/env python
""" common routines """
import numpy as np
# constants
G_0 = 6.67E-11 # (kg * m/s^2) * m^2 / kg^2
AU = 1.496e11
M_E = 5.97e24
M_S = 1.99e30
YR = 3.15e7

# functions
def get_G(m, l, t):
    """ scale G with appropriate units """
    return G_0 * m * t**2 / l**3
def get_f(G, Ms):
    """
    returns a function to compute dx/dt = f(x)
    @param G - gravitational constant
    @param Ms - masses of bodies
    @return functionxs) => d(xs)/dt
        - @param xs = N x 2 x D 1D array
    """
    def f(xs, _):
        """
        computes dx/dt = v, dv/dt = sum_j (G * Mj / |r_j - r|^3 * (x_j - x))
        """
        xs = np.array(xs)
        dx_dt = np.zeros(np.shape(xs))
        D = 2
        num_masses = xs.size // (2 * D)
        for i in range(num_masses):
            dx_dt[i * 2 * D: (i * 2 + 1) * D] +=\
                xs[(i * 2 + 1) * D: (i + 1) * 2 * D]
            for j in range(num_masses):
                # skip self interactions
                if i == j:
                    continue
                dr_j = (xs[j * 2 * D: (j * 2 + 1) * D] -
                        xs[i * 2 * D: (i * 2 + 1) * D])
                dx_dt[(i * 2 + 1) * D: (i + 1) * 2 * D] +=\
                        G * Ms[j] * dr_j / sum(dr_j**2) ** (3/2)
        return dx_dt
    return f
def l2_norm(x, y):
    """ l2 norm between two arrays """
    assert np.shape(x) == np.shape(y),\
        'l2 norm passed two different shapes %s, %s' %\
        (np.shape(x), np.shape(y))
    return np.sqrt(sum((x - y)**2)) / x.size
