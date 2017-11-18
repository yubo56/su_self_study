#!/usr/bin/env python
"""
Test our steppers
"""
from collections import defaultdict
import common
import matplotlib.pyplot as plt
import numpy as np
import steppers

def harmonic_oscillator_test(stepper, dt):
    """
    really simple SHO test, returns l2 norm of diff of solution
    """
    def f(xs):
        dx = np.zeros(np.shape(xs))
        dx[0] = xs[1]
        dx[1] = -xs[0]
        return dx
    T = 0
    T_F = 10
    xs = np.array([[0], [1]])
    xlist = [0]
    while T < T_F:
        xs = stepper(xs, f, dt)
        T += dt
        xlist.append(xs[0])
    xarr = np.array(xlist)
    exact = np.array([np.sin(i * dt) for i in range(len(xarr))])
    return common.l2_norm(xarr, exact)

def sun_earth_test(stepper, dt):
    """
    earth orbiting sun, returns l2 norm of diff of solution
    """
    # numerical params
    T = 0

    # physical params
    R = common.M_S/common.M_E
    MS = [R, 1]
    G = common.get_G(common.M_E, common.AU, common.YR)
    f = common.get_f(G, MS)
    period = np.sqrt(4 * np.pi**2 / (G * sum(MS)) * (1 + 1 / R)**3)

    T_F = 2 * period
    N = int(round((T_F - T) / dt))
    xs = np.array([
        [[-1 / R, 0], [0, -2 * np.pi / R]],
        [[1, 0], [0, 2 * np.pi]]
    ])
    earth_pos = [xs[1][0]]
    times = [T]
    for i in range(N):
        xs = stepper(xs, f, dt)
        earth_pos.append(xs[1][0])
        times.append(i * dt)
    earth_arr = np.array(earth_pos)
    times_arr = np.array(times)
    exact_earth = np.array(list(zip(
        np.cos(2 * np.pi / period * times_arr),
        np.sin(2 * np.pi / period * times_arr)
    )))
    return np.sqrt(sum(common.l2_norm(earth_arr, exact_earth))**2)

if __name__ == '__main__':
    # 1e-1 to 1e-4
    t_steps = np.exp(np.linspace(-1, -4, 10) * np.log(10))
    errs = defaultdict(list)
    for t_step in t_steps:
        print('Computing for %.3e' % t_step)
        errs['SHO Euler'].append(
            harmonic_oscillator_test(steppers.f_euler, t_step)
        )
        errs['SHO Mid'].append(
            harmonic_oscillator_test(steppers.f_mid, t_step)
        )
        errs['Sun Earth Euler'].append(
            sun_earth_test(steppers.f_euler, t_step)
        )
        errs['Sun Earth Mid'].append(
            sun_earth_test(steppers.f_mid, t_step)
        )
    plt.clf()
    for name, err_list in errs.items():
        plt.loglog(t_steps, err_list, label=name)
    plt.legend()
    plt.xlabel('Delta t')
    plt.ylabel('Error')
    plt.show()
