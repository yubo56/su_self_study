#!/usr/bin/env python
"""
Plots the following 2D maps in series: Henon
"""
from chaos import MathMap
import numpy as np

NUM_ITERS = 10000

# HENON CONSTANTS
H_A = 1.4
H_B = 0.3

# DUFFING CONSTANTS
D_A = 1
D_B = 1

# KAPLAN-YORKE CONSTANTS
KY_A = 1.5
KY_B = 0.7

def henon_iter(arg):
    """
    Evolves according to rule x[n+1] = y[n] + 1 - a * x[n]^2, y[n+1] = b * x[n]
    """
    return np.array([arg[1] + 1 - H_A * arg[0] * arg[0],
                     H_B * arg[0]])

def duffing_iter(arg):
    """
    Evolves per duffing rule
    """
    return np.array([arg[1],
                     -D_B * arg[0] + D_A * arg[1] - pow(arg[1], 3)])

def ky_iter(arg):
    """ Kaplan-Yorke map"""
    return np.array([KY_A * arg[0] % 1,
                     KY_B * arg[1] + np.cos(2 * np.pi * arg[0])])

if __name__ == '__main__':
    HENON_MAP = MathMap(np.array([1, 1]), henon_iter, max_iter=NUM_ITERS)
    HENON_MAP.run()
    HENON_MAP.scatter(title='Henon 2D map', write_file='Henon.png')

    DUFFING_MAP = MathMap(np.array([0.6, 0.6]), duffing_iter, max_iter=NUM_ITERS)
    DUFFING_MAP.run()
    DUFFING_MAP.scatter(title='Duffing 2D map', write_file='Duffing.png')

    KY_MAP = MathMap(np.array([0.6, 0.6]), ky_iter, max_iter=NUM_ITERS)
    KY_MAP.run()
    KY_MAP.scatter(title='Kaplan-Yorke 2D map',
                   xlabel='x',
                   ylabel='y',
                   write_file='KY.png')
