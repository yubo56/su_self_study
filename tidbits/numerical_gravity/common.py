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
    @return function(const xs) => d(xs)/dt
        - @param xs = N x 2 x D array: [N bodies: x + x':dimensions]
    """
    def f(xs):
        """
        computes dx/dt = v, dv/dt = sum_j (G * Mj / |r_j - r|^3 * (x_j - x))
        """
        dx_dt = np.zeros(np.shape(xs))
        for i in range(len(dx_dt)):
            dx_dt[i][0] = xs[i][1]
            for j in range(len(xs)):
                if i == j:
                    continue
                dr_j = (xs[j][0] - xs[i][0])
                dx_dt[i][1] += G * Ms[j] * dr_j / sum(dr_j**2) ** (3/2)
        return dx_dt
    return f
def l2_norm(x, y):
    """ l2 norm between two arrays """
    assert np.shape(x) == np.shape(y),\
        'l2 norm passed two different shapes %s, %s' %\
        (np.shape(x), np.shape(y))
    return np.sqrt(sum((x - y)**2)) / x.size
