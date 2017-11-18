#!/usr/bin/env python
import common
import matplotlib.pyplot as plt
import numpy as np
import steppers

def run(stepper):
    # numerical params
    NUM_SAMPLES = 80
    T = 0
    dt = 3e-4

    # physical params
    MS = np.array([4, 5, 3]) * 30000
    G = common.get_G(common.M_E, common.AU, common.YR)
    f = common.get_f(G, MS)

    T_F = 5
    ys = np.array([
        0, 0, 0, 0,
        3, 0, 0, 0,
        0, 4, 0, 0
    ], dtype=np.float64)
    state = [np.copy(ys)]
    solver = stepper(f, T, ys, T_F, max_step=dt, G=G, Ms=MS,
                     get_jerk=common.get_jerk, get_accel=common.get_accel
                    )
    times = [T]
    while solver.status == 'running':
        solver.step()
        state.append(np.copy(solver.y))
        times.append(solver.t)
    print('Done Running')
    num_pts = len(times)
    state_arr = np.array(state[::num_pts // NUM_SAMPLES])
    times = times[::num_pts // NUM_SAMPLES]

    lims = (1.2 * np.min(state_arr[:, 0::4]),
            1.2 * np.max(state_arr[:, 0::4]),
            1.2 * np.min(state_arr[:, 1::4]),
            1.2 * np.max(state_arr[:, 1::4]))
    for i, t in enumerate(times):
        plt.clf()
        plt.axis(lims)
        plt.scatter(state_arr[i, 0], state_arr[i, 1], label='p1')
        plt.scatter(state_arr[i, 4], state_arr[i, 5], label='p2')
        plt.scatter(state_arr[i, 8], state_arr[i, 9], label='p3')
        plt.xlabel('x')
        plt.ylabel('y')
        plt.title('T = %.3f' % t)
        plt.legend(loc=4) # bottom right
        plt.annotate('Energy: %.3e' % common.get_E(state_arr[i], G, MS),
                     xy=(0.75, 0.95),
                     xycoords='axes fraction',
                     fontsize=8
                    )
        plt.savefig('plots/%03d.png' % i)

if __name__ == '__main__':
    run(steppers.RK4Solver)
