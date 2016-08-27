#!/usr/bin/env python
# Plots the following 2D maps: Henon
from lib.math_map import math_map
import numpy as np

CONST_A = 1.4
CONST_B = 0.3
NUM_ITERS=10000

def henon_iter(arg):
    """
    Evolves according to rule x[n+1] = y[n] + 1 - a * x[n]^2, y[n+1] = b * x[n]
    """
    return np.array([arg[1] + 1 - CONST_A * arg[0] * arg[0],
        CONST_B * arg[0]])

test_map = math_map(np.array([1, 1]), henon_iter, max_iter=NUM_ITERS)
test_map.run()
test_map.scatter()
