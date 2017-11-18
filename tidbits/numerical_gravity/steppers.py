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

class EulerSolver(intg.OdeSolver):
    """ Simple Forward Euler Solver """
    def __init__(self, f, t0, y0, tf, max_step=1e-4, **_):
        self.f, self.t, self.y, self.tf, self.dt = f, t0, y0, tf, max_step
        self.status = 'running'
        super(EulerSolver, self).__init__(f, t0, y0, tf, True)

    def _dense_output_impl(self):
        return None

    def _step_impl(self):
        if self.status == 'finished':
            return False, 'EulerSolver is finished'
        self.y += self.dt * self.f(self.y, self.t)
        self.t += self.dt
        if self.t > self.tf:
            self.status = 'finished'
        return True, ''

class MidSolver(intg.OdeSolver):
    """ Simple midpoint Solver """
    def __init__(self, f, t0, y0, tf, max_step=1e-4, **_):
        self.f, self.t, self.y, self.tf, self.dt = f, t0, y0, tf, max_step
        self.status = 'running'
        super(MidSolver, self).__init__(f, t0, y0, tf, True)

    def _dense_output_impl(self):
        return None

    def _step_impl(self):
        if self.status == 'finished':
            return False, 'MidSolver is finished'
        dy = self.dt * self.f(self.y, self.t)
        self.y += self.dt * self.f(self.y + dy / 2, self.t)
        self.t += self.dt
        if self.t > self.tf:
            self.status = 'finished'
        return True, ''

class RK4Solver(intg.OdeSolver):
    """ Simple RK4 Solver """
    def __init__(self, f, t0, y0, tf, max_step=1e-4, **_):
        self.f, self.t, self.y, self.tf, self.dt = f, t0, y0, tf, max_step
        self.status = 'running'
        super(RK4Solver, self).__init__(f, t0, y0, tf, True)

    def _dense_output_impl(self):
        return None

    def _step_impl(self):
        if self.status == 'finished':
            return False, 'RK4Solver is finished'
        k1 = self.f(self.y, self.t)
        k2 = self.f(self.y + (k1 * self.dt / 2), self.t + (self.dt / 2))
        k3 = self.f(self.y + (k2 * self.dt / 2), self.t + (self.dt / 2))
        k4 = self.f(self.y + (k3 * self.dt), self.t + self.dt)
        self.y += self.dt * (k1 + 2 * k2 + 2 * k3 + k4) / 6
        self.t += self.dt
        if self.t > self.tf:
            self.status = 'finished'
        return True, ''

class HermiteSolver(intg.OdeSolver):
    """ Hermite Integrator (specialized, lots of extra kwargs) """
    def __init__(self,
                 f,
                 t0,
                 y0,
                 tf,
                 max_step=1e-4,
                 get_accel=None,
                 get_jerk=None,
                 G=None,
                 Ms=None,
                 **_
                ):
        assert get_accel is not None and get_jerk is not None and G is not None\
                and Ms is not None,\
                'HermiteSolver must have get_accel, get_jerk, G, Ms'
        self.f, self.t, self.y, self.tf, self.dt, self.get_accel,\
                self.get_jerk, self.G, self.Ms \
                = f, t0, y0, tf, max_step, get_accel, get_jerk, G, Ms
        self.status = 'running'
        super(HermiteSolver, self).__init__(f, t0, y0, tf, True)

    def _dense_output_impl(self):
        return None

    def _step_impl(self):
        if self.status == 'finished':
            return False, 'HermiteSolver is finished'
        D = len(self.y) // (2 * len(self.Ms))
        # predict
        accels_o = self.get_accel(self.y, self.G, self.Ms) # original
        jerks_o = self.get_jerk(self.y, self.G, self.Ms)
        yp = np.copy(self.y)
        for i in range(len(self.Ms)):
            yp[i * 2 * D: (i * 2 + 1) * D] +=\
                self.dt * yp[(i * 2 + 1) * D: (i + 1) * 2 * D] +\
                self.dt**2 * accels_o[i] / 2 + self.dt**3 * jerks_o[i] / 6
            yp[(i * 2 + 1) * D: (i + 1) * 2 * D] +=\
                self.dt * accels_o[i] + self.dt**2 * jerks_o[i] / 2

        # correct
        accels_p = self.get_accel(yp, self.G, self.Ms) # predicted
        jerks_p = self.get_jerk(yp, self.G, self.Ms)
        for i in range(len(self.Ms)):
            old_velocity = np.copy(self.y[(i * 2 + 1) * D: (i + 1) * 2 * D])
            self.y[(i * 2 + 1) * D: (i + 1) * 2 * D] +=\
                self.dt * (accels_o[i] + accels_p[i]) / 2 -\
                self.dt**2 * (jerks_p[i] - jerks_o[i]) / 12
            self.y[i * 2 * D: (i * 2 + 1) * D] +=\
                self.dt * (self.y[(i * 2 + 1) * D: (i + 1) * 2 * D] +
                           old_velocity) -\
                self.dt**2 * (accels_p[i] - accels_o[i]) / 10

        self.t += self.dt
        if self.t > self.tf:
            self.status = 'finished'
        return True, ''
