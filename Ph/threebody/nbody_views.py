#!/usr/bin/env python3
'''
Module for (eventually) plotting videos in the Trisolaris world (or maybe even
more bodies!)

Units: lengths are in units of 0.1AU, weights in M_\odot, times in 1000s. In
    such units, G ~ 4.5E-5 AU^3 / M_s ks^2

Mathemagics: quartic potential well centered on the origin, bound orbits
'''
import numpy as np
import os
from scipy.integrate import odeint
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
plt.style.use('ggplot')

G = 4.5E-5
BOX = 5 # length scale at which cubic restoring force = G
SOFT = 0.02 # soften gravitational potential when too close

def get_traj(_star_init, star_mass, _planet_init, tf, dt, t0=0):
    '''
    3D, all positions/velocities 3 components

    _star_init: [[x1, v1] ... [xN, vN]]
    star_mass: [m1 ... mN]
    _planet_init: same as star_init, M many
        - massless
    tf: end time
    dt: timestep
    t0: start time (defaullt = 0)

    returns:
        [[[x1(t=0), v1(t=0)] ... [x(N+M)(t=0), v(N+M)(t=0)]],
         [[x1(t=1), v1(t=1)] ... [x(N+M)(t=1), v(N+M)(t=1)]],... ]
        times - simulation times
    '''
    N = len(_star_init)
    M = len(_planet_init)
    star_init = _star_init.flatten()
    planet_init = _planet_init.flatten()

    def _dxdt(pos_arr, t):
        '''
        computes dx/dt = v, dv/dt = GM_i/r_i^2, summed over sun_i
        pos_arr = flatten([[x1, v1] ... [xN, vN], [x1, v1] ... [xM, vM]])
            - First N are suns, last M positions
        '''
        dxdt_arr = np.zeros(6 * (N + M))

        for body in range(N + M):
            # dxdt = v
            dxdt_arr[6 * body: 6 * body + 3] = \
                pos_arr[6 * body + 3: 6 * body + 6]

            dvdt = dxdt_arr[6 * body + 3: 6 * body + 6]
            x = pos_arr[6 * body: 6 * body + 3]

            dvdt -= G * (x / BOX)**3

            for idx_sun in range(N):
                if idx_sun == body:
                    continue
                # dv/dt = -GM/|dx|^3 * dx
                dx = pos_arr[6 * idx_sun: 6 * idx_sun + 3] - x
                dvdt += G * star_mass[idx_sun] * dx /\
                        (sum(dx * dx) + SOFT**2)**(3 / 2)
        return dxdt_arr

    times = np.arange(t0, tf, dt)
    trajectory = odeint(_dxdt,
                        np.concatenate((star_init, planet_init)),
                        times)
    return np.reshape(trajectory, (len(times), N + M, 2, 3)), times

def get_proj(star_arr, planet_pos, facing, up):
    '''
    From star positions, planet positions and orientation of observer,
    calculates (theta, phi) projection of the stars for the observer

    star_arr: [[x1, v1] ... [xN, vN]] - vN are ignored
    planet_pos: (x, y, z)
    facing: (x, y, z) - need not be a unit vector
    up: (x, y, z) - need not be a unit vector

    return: [[theta1, phi1] ... [thetaN, phiN]]

    Note: np.dot(star_pos, planet_pos) / norms = cos theta
    '''
    pass

if __name__ == '__main__':
    star_init = np.array([
        [[0, 1, 0], [-1e-3, 0, 0]],
        [[0, -1, 0], [+1e-3, 0, 0]],
        [[0.5, 0, 0], [+2e-3, 0, 0]],
    ])
    star_mass = [1, 1, 1]
    planet_init = np.array([
        [[0, 0.01, 0], [0, 0, 0]],
    ])

    N = len(star_init)
    M = len(planet_init)

    T_F = 3000
    stride = 500
    traj, times = get_traj(star_init, star_mass, planet_init, T_F, 0.02)
    plot_pos = traj[::stride, :N + M, 0, :2]
    for i, (pts, time) in enumerate(zip(plot_pos, times[::stride])):
        for pts_i in pts[ :N]:
            plt.scatter(*(pts_i.T), c='y')
        for pts_i in pts[N: ]:
            plt.scatter(*(pts_i.T), c='g')
        plt.xlim(-BOX, BOX)
        plt.ylim(-BOX, BOX)
        plt.title('t = %d ks' % i)
        plt.xlabel('0.1AU')
        plt.ylabel('0.1AU')
        plt.savefig('plots/t_%d.png' % time)
        plt.clf()
        print('Saved %d' % i)
    os.system('ffmpeg -y -framerate 12 -i plots/t_%d.png traj.mp4')
