'''
Plane boarding simulations (see tidbits.pdf)
'''
import numpy as np
import matplotlib.pyplot as plt

def inversions(N, num_sims=10000):
    inversion_count = np.zeros(N - 1)
    for _ in range(num_sims):
        sequence = np.random.permutation(N)
        inv_count = 0
        for idx in range(N - 1):
            if sequence[idx] < sequence[idx + 1]:
                inv_count += 1
        inversion_count[inv_count] += 1
    x = np.arange(N - 1)
    plt.bar(x, inversion_count, label='Data')
    plt.plot(x, num_sims / np.sqrt(np.pi * (N - 1) / 6)
                 * np.exp(-6 * (x - (N - 1)/2)**2 / (N - 1)), label='fit')
    plt.xlabel('Number of Inversions')
    plt.ylabel('Frequency')
    plt.legend()
    plt.savefig('inversions.png')
    plt.clf()

def seating_anal(N, cache=None):
    if cache == None:
        cache = np.zeros(N + 1) - 1
        cache[0] = 0
    if cache[N] != -1:
        return cache[N], cache
    val = sum([seating_anal(N - m - 1, cache)[0] + 1 for m in range(N)]) / N
    cache[N] = val
    return val, cache

def seating(N, num_sims=10000):
    seats = np.zeros(N)
    for _ in range(num_sims):
        sequence = np.random.permutation(N)
        num_seat = 0
        min_el = N + 1
        for el in sequence:
            if el < min_el:
                min_el = el
                num_seat += 1
        seats[num_seat - 1] += 1
    mean = sum(seats * (np.arange(N) + 1)) / num_sims
    mean_anal = seating_anal(N)[0]
    plt.bar(np.arange(N), seats, label='Data')
    plt.plot([mean_anal] * 2, [0, 1.2 * max(seats)], 'r-', label='Anal. Mean')
    plt.xlabel('Seatings')
    plt.ylabel('Frequency')
    plt.title('Number of Immediate Seatings (N=%d)' % N)
    plt.annotate('Mean/anal: %.3f/%.3f' % (mean, mean_anal),
                 (0.6, 0.5),
                 xycoords='figure fraction')
    plt.legend()
    plt.savefig('seatings.png')
    plt.clf()

def seat_trend(N):
    cache = seating_anal(N)[1]
    plt.plot(np.arange(N + 1), cache)
    plt.xlabel('Number of People')
    plt.ylabel('Number Seated')
    plt.title('Trend of Number Seatable People')
    plt.savefig('seating_trend.png')
    plt.clf()

if __name__ == '__main__':
    inversions(100)
    seating(10)
    seat_trend(100)
