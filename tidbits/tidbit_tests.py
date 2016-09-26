#!/usr/bin/env python
"""
Various testing modules for results obtained in tidbits
"""
import math
import numpy as np
import matplotlib.pyplot as plt

def weird_new_func(n_val, increment):
    """
    returns the weird sum over reciprocals of factorials, with increment being
    either 1 or 3
    """
    assert n_val % 2 == 1 # assert odd n
    m_val = int((n_val - 1) / 2)
    return sum([
        (-1)**k / (math.factorial(k) * (2 * k + increment))
        for k in range(m_val + 1)
    ])
def var_median(fig,
               num_samples=100,
               root=5,
               max_power=10):
    """
    Computes the variance of the median of (2m+1) samples from a uniform
    distribution as a function of m

    Samples n logarithmically in powers of root^{[1, max_power]}

    @param int num_samples: how many samples to try at each n
    @param int max_power: how high to try n

    @return list(int), list(list(float))
        list of n values tried
        list of medians found at each n
    """
    medians = list()
    n_vals = [pow(root, i) for i in range(1, max_power + 1)]
    for n_val in n_vals:
        medians.append([np.median(np.random.random(n_val))
                        for _ in range(num_samples)])
    # expected fit, a = 1/2 here
    true_vars = [np.var(trial) for trial in medians]
    median_vars = [1 / (10 * (n - 1)) for n in n_vals]
    new_median_vars = [1 / (2 * (n - 1)) *
                       weird_new_func(n, 3) / weird_new_func(n, 1)
                       for n in n_vals]
    mean_vars = [1 / (12 * n) for n in n_vals]

    # plot asymptotic behavior
    axes = fig.add_subplot(1, 1, 1)
    axes.loglog(n_vals, true_vars, 'b+', label='Actual')
    axes.loglog(n_vals, median_vars, 'r--', label='My Predicted')
    axes.loglog(n_vals, new_median_vars, 'k-', label='My New Predicted')
    axes.loglog(n_vals, mean_vars, 'g-.', label='Variance of Mean-estimator')
    axes.set_title('Root = {}, Max_power = {}, Num_samples = {}'.format(\
        root, max_power, num_samples))
    axes.set_xlabel('n')
    axes.set_ylabel('Variance')
    axes.legend()
    fig.savefig('medians_asymptotic.png')
    fig.clf()

    # plot a few ratios
    axes = fig.add_subplot(1, 1, 1)
    axes.semilogx(n_vals,
                  [b / a for a, b in zip(true_vars, median_vars)],
                  label='Old')
    axes.semilogx(n_vals,
                  [b / a for a, b in zip(true_vars, new_median_vars)],
                  label='New')
    axes.set_title('Ratio of predicted median variances to actual')
    axes.set_xlabel('n')
    axes.legend()
    fig.savefig('medians_ratio_to_real.png')
    fig.clf()

    axes = fig.add_subplot(1, 1, 1)
    axes.semilogx(n_vals, [b / a for a, b in zip(true_vars, mean_vars)])
    axes.set_title('Ratio of mean variance to median')
    axes.set_xlabel('n')
    fig.savefig('medians_ratio_to_mean.png')
    fig.clf()

    return n_vals, medians

if __name__ == '__main__':
    # globals
    FIG = plt.figure()
    ROOT = 3

    _, BAR = var_median(FIG, root=ROOT, max_power=9, num_samples=10000)
    plt.hist(BAR[4])
    plt.title('Histogram of 10000 medians at n={}**4'.format(ROOT))
    plt.savefig('medians_slice_hist.png')
