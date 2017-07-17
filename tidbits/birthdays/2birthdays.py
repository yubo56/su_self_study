#!/usr/bin/env python
"""
plot of predicted shape vs data points for birthday problem of size N as
function of k points chosen
"""
import math
import matplotlib.pyplot as plt

def approx(n, k, order=1):
    """
    generate the approximate value of P(N, k)

    order=1 generates the $k**2 / 2N$
    """

    exp_factor = 0
    next_exp = -k**2 / (2 * (n - k))
    for i in range(order):
        exp_factor += next_exp
        next_exp *= -(i + 2) * k / (n - k)
    return math.exp(exp_factor) * math.sqrt(1 + k / (n - k))

def exact(n, k):
    """
    compute the exact value of P(N, k) combinatorially
    """
    return math.factorial(n) / (n ** k * math.factorial(n - k))

def genplot(n, FN='birthdays_approx', order=1):
    """
    generate plot for a given value of N
    """
    if order <= 1:
        k_vals = range(0, n, 10)
    else:
        k_vals = range(int(n/10))
    p_exact = []
    p_approx = [list() for _ in range(order)]
    styles = ['g--', 'r--', 'c--', 'm--']
    max_index = n

    for i, k in enumerate(k_vals):
        try:
            p_exact.append(exact(n, k))
            for idx in range(order):
                p_approx[idx].append(approx(n, k, idx + 1))
        except Exception:
            max_index = i - 1
            break

    plt.semilogy(k_vals[ :max_index], p_exact[ :max_index], 'b+', label='Exact')
    for i, p_approx_k in enumerate(p_approx):
        plt.semilogy(
            k_vals[ :max_index], p_approx_k[ :max_index],
            styles[i % len(styles)],
            label='Approximate order {}'.format(i + 1))
    # plt.title('Exact and Approximate Solutions to Birthday Problem')
    plt.xlabel('k')
    plt.ylabel('N')
    plt.legend()
    plt.savefig('{}{}.png'.format(
        FN,
        'order_' + str(order) if order != 1 else ''
        ))
    plt.clf()

if __name__ == '__main__':
    N = 365
    genplot(N)
    # higher order terms seem to decrease in accuracy...
    genplot(N, order=3)
