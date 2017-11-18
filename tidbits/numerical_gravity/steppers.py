#!/usr/bin/env python
"""
Collection of steppers with folloowing prototype
    @param const xs - current positions
    @param f - function(const xs) => d(xs)/dt
    @param const dt - timestep
    @return xs + dx - new x positions, same as @xs
"""
from scipy import integrate as intg

class EulerSolver(intg.OdeSolver):
    """ Simple Forward Euler Solver """
    def __init__(self, f, t0, y0, tf, max_step=1e-4):
        super(EulerSolver, self).__init__(f, t0, y0, tf, True)
        self.f, self.t, self.y, self.tf, self.dt = f, t0, y0, tf, max_step
        self.status = 'running'

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
    def __init__(self, f, t0, y0, tf, max_step=1e-4):
        super(MidSolver, self).__init__(f, t0, y0, tf, True)
        self.f, self.t, self.y, self.tf, self.dt = f, t0, y0, tf, max_step
        self.status = 'running'

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
    def __init__(self, f, t0, y0, tf, max_step=1e-4):
        super(RK4Solver, self).__init__(f, t0, y0, tf, True)
        self.f, self.t, self.y, self.tf, self.dt = f, t0, y0, tf, max_step
        self.status = 'running'

    def _dense_output_impl(self):
        return None

    def _step_impl(self):
        if self.status == 'finished':
            return False, 'MidSolver is finished'
        k1 = self.f(self.y, self.t)
        k2 = self.f(self.y + (k1 * self.dt / 2), self.t + (self.dt / 2))
        k3 = self.f(self.y + (k2 * self.dt / 2), self.t + (self.dt / 2))
        k4 = self.f(self.y + (k3 * self.dt), self.t + self.dt)
        self.y += self.dt * (k1 + 2 * k2 + 2 * k3 + k4) / 6
        self.t += self.dt
        if self.t > self.tf:
            self.status = 'finished'
        return True, ''
