#!/usr/bin/env python
"""
Test our steppers
"""
from collections import defaultdict
import common
import matplotlib.pyplot as plt
import numpy as np
import scipy.integrate as intg
import steppers

def harmonic_oscillator_test(stepper_type, dt):
    """
    really simple SHO test, returns l2 norm of diff of solution
    """
    def f(ys, _):
        dy = np.zeros(np.shape(ys))
        dy[0] = ys[1]
        dy[1] = -ys[0]
        return dy
    T = 0
    T_F = 10
    ys = np.array([0, 1], dtype=np.float64)
    ylist = [0]
    solver = stepper_type(f, T, ys, T_F, max_step=dt)
    while solver.status == 'running':
        solver.step()
        ylist.append(np.copy(solver.y[0]))
    yarr = np.array(ylist)
    exact = np.array([np.sin(i * dt) for i in range(len(yarr))])
    return common.l2_norm(yarr, exact)

def sun_earth_test(stepper_type, dt):
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
    V_E = np.sqrt(G * R / (1 + 1/R))
    ys = np.array([
        -1 / R, 0, 0, -V_E / R,
        1, 0, 0, V_E
    ], dtype=np.float64)
    earth_pos = [ys[4:6]]
    solver = stepper_type(f, T, ys, T_F, max_step=dt)
    times = [T]
    while solver.status == 'running':
        solver.step()
        earth_pos.append(np.copy(solver.y[4:6]))
        times.append(solver.t)
    earth_arr = np.array(earth_pos)
    times_arr = np.array(times)
    exact_earth = np.array(list(zip(
        np.cos(2 * np.pi / period * times_arr),
        np.sin(2 * np.pi / period * times_arr)
    )))
    return np.sqrt(sum(common.l2_norm(earth_arr, exact_earth))**2)

if __name__ == '__main__':
    # 1e-1 to 1e-4
    t_steps = np.exp(np.linspace(-1, -5, 10) * np.log(10))
    errs = defaultdict(list)
    for t_step in t_steps:
        print('Computing SHO for %.3e' % t_step)
        errs['SHO Euler'].append(
            harmonic_oscillator_test(steppers.EulerSolver, t_step)
        )
        errs['SHO Mid'].append(
            harmonic_oscillator_test(steppers.MidSolver, t_step)
        )
        errs['SHO RK4'].append(
            harmonic_oscillator_test(steppers.RK4Solver, t_step)
        )
    plt.clf()
    for name, err_list in errs.items():
        plt.loglog(t_steps, err_list, label=name)
    plt.legend()
    plt.xlabel('Delta t')
    plt.ylabel('L2 Norm of Solution Difference')
    plt.savefig('SHO_errs.png')

    plt.clf()
    errs = defaultdict(list)
    for t_step in t_steps:
        print('Computing Sun Earth for %.3e' % t_step)
        errs['Sun Earth Euler'].append(
            sun_earth_test(steppers.EulerSolver, t_step)
        )
        errs['Sun Earth Mid'].append(
            sun_earth_test(steppers.MidSolver, t_step)
        )
        errs['Sun Earth RK4'].append(
            sun_earth_test(steppers.RK4Solver, t_step)
        )
        errs['Sun Earth scipy RK45'].append(
            sun_earth_test(intg.RK45, t_step)
        )
    plt.clf()
    for name, err_list in errs.items():
        plt.loglog(t_steps, err_list, label=name)
    plt.legend()
    plt.xlabel('Delta t')
    plt.ylabel('L2 Norm of Solution Difference')
    plt.savefig('Sun_Earth_errs.png')
