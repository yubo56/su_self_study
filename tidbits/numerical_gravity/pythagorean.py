#!/usr/bin/env python
import common
import matplotlib.pyplot as plt
import numpy as np
import steppers
if __name__ == '__main__':
    # numerical params
    NUM_SAMPLES=80
    T = 0
    dt=3e-3

    # physical params
    MS = np.array([4, 5, 3]) * 10000
    G = common.get_G(common.M_E, common.AU, common.YR)
    f = common.get_f(G, MS)

    T_F = 5
    ys = np.array([
        0, 0, 0, 0,
        3, 0, 0, 0,
        0, 4, 0, 0
    ], dtype=np.float64)
    p1_pos = [np.copy(ys[0:2])]
    p2_pos = [np.copy(ys[4:6])]
    p3_pos = [np.copy(ys[8:10])]
    solver = steppers.RK4Solver(f, T, ys, T_F, max_step=dt)
    times = [T]
    while solver.status == 'running':
        solver.step()
        p1_pos.append(np.copy(solver.y[0:2]))
        p2_pos.append(np.copy(solver.y[4:6]))
        p3_pos.append(np.copy(solver.y[8:10]))
        times.append(solver.t)
    num_pts = len(times)
    p1_arr = np.array(p1_pos[::num_pts // NUM_SAMPLES])
    p2_arr = np.array(p2_pos[::num_pts // NUM_SAMPLES])
    p3_arr = np.array(p3_pos[::num_pts // NUM_SAMPLES])
    times = times[::num_pts // NUM_SAMPLES]

    lims = (1.2 * np.min([p1_arr[:, 0], p2_arr[:, 0], p3_arr[:, 0]]),
            1.2 * np.max([p1_arr[:, 0], p2_arr[:, 0], p3_arr[:, 0]]),
            1.2 * np.min([p1_arr[:, 1], p2_arr[:, 1], p3_arr[:, 1]]),
            1.2 * np.max([p1_arr[:, 1], p2_arr[:, 1], p3_arr[:, 1]]))
    for i, t in enumerate(times):
        plt.clf()
        plt.axis(lims)
        plt.scatter(p1_arr[i, 0], p1_arr[i, 1], label='p1')
        plt.scatter(p2_arr[i, 0], p2_arr[i, 1], label='p2')
        plt.scatter(p3_arr[i, 0], p3_arr[i, 1], label='p3')
        plt.xlabel('x')
        plt.ylabel('y')
        plt.title('T = %.3f' % t)
        plt.legend(loc=4)
        plt.savefig('plots/%03d.png' % i)
