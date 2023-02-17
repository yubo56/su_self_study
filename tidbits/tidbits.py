import math
import numpy as np
from scipy.optimize import brenth

def run_borwein(kmax, stride=2):
    '''
    runs Borwein integral up to kmax with stride
    '''
    x = 1 / np.arange(1, kmax + stride, stride)
    N = len(x)
    sum = 0
    prod = np.prod(x)

    for i in range(2**N):
        j = i
        flags = np.zeros(N)
        for k in range(N):
            if j >= 2**(N - k - 1):
                flags[k] = 1
                j -= 2**(N - k - 1)
            else:
                flags[k] = -1
        sum += (np.sum(flags * x) / 2)**(N - 1) * np.sign(np.sum(flags * x)) * np.product(flags)
        # print('\t', np.sum(flags * x))
    print('Sum of coeffs is', np.sum(x))
    print('\tSum of series (divided by pi) is',
          sum / (4 * prod * math.factorial(N - 1)))

def get_periods(N, npts=1000, tf=10):
    '''
    This is an application of Linear Predictive Coding, courtesy of Jeremy
    Goodman

    For a given sequence of time data {y}, we can model it with an
    "autoregressive" model, so that
    y[k] = a[1] * y[k - 1] + a[2] * y[k - 2] + ... a[N] * y[k - N]

    These a[N] can be computed for each y[k]. But if we expect them to be the
    same throughout the time series (as we do for our very sinusoidal data),
    then we can just assign the entire dataset a single a[N] by averaging over
    all the a[N] we compute.

    Then, for a given sequence of a[N], we can write down the characteristic
    equation (a[0] = 0)
    a[0] * p^0 + a[1] * p^1 + ... + a[N] * p^n = 0
    This is an n-th order polynomial, so it has n roots. It turns out that the
    roots are of form e^(i * omega * dt), where omega are the frequencies in the
    initial model. The details of this are hard to write down here.
    '''
    t = np.linspace(0, tf, npts)
    dt = tf / npts
    y = 0 * t
    # random frequency in 5-105
    ws = 50 * np.random.random(N) + 5
    for w in ws:
        y += np.sin(w * t)

    ### compute the linear regression: y[k] = A[k, j] . y[k - j]
    A = np.zeros((npts - 2 * N, 2 * N))
    for k in range(npts - 2 * N):
        A[k] = y[k:k + 2 * N]
    yvec = y[2 * N: ]
    # a[k] are the coefficients of the averaged autoregressive model
    a = np.linalg.lstsq(A, yvec, rcond=None)[0]

    ### solutions of Sum[a[n]x^n] = 0 are the Omegas, so find them
    # we need to include the y[k] coefficient to solve for the zeros, and
    # there's a sign flip since: 0 = -y[k] + a[j] . y[k - j]
    a_full = np.concatenate((a, [-1]))
    # we expect the roots to be roots of unity, and it's easier to do
    # root-finding in 1D / with real roots, so we instead solve for:
    # Sum[a[n] * cos(omega * n * dt)] = 0
    def f(w_guess):
        n_arr = np.arange(0, 2 * N + 1)
        cosines = np.cos(w_guess * n_arr * dt)
        return np.sum(cosines * a_full)

    ### scan frequencies from omega = 1 / dt -> 1 / tf and try to find a root in
    # every frequency (except aliased ones!)
    all_roots = []
    for w_guess in range(1, int(npts / (2 * np.pi))):
        try:
            ret = brenth(f, w_guess, w_guess + 1)
            alias_check = ret / np.pi * tf
            if abs(alias_check - round(alias_check)) > 1e-3:
                all_roots.append(ret)
        except ValueError:
            # means that no zero exists in range
            continue
    all_roots = np.array(all_roots)
    print('Original frequencies (%d):' % len(ws), sorted(ws))
    print('Found frequencies (%d):' % len(all_roots), sorted(all_roots))

if __name__ == '__main__':
    # run_borwein(13)
    # run_borwein(15)
    get_periods(2)
