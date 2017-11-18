#!/usr/bin/env python
"""
Collection of steppers with folloowing prototype
    @param const xs - current positions
    @param f - function(const xs) => d(xs)/dt
    @param const dt - timestep
    @return xs + dx - new x positions, same as @xs
"""
import numpy as np
from scipy import integrate as intg

def f_euler(xs, f, dt):
    """ forward euler """
    return xs + dt * f(xs)

def f_mid(xs, f, dt):
    """ midpoint """
    dx = dt * f(xs) / 2
    return xs + dt * f(xs + dx)
