#!/usr/bin/env python
''' runs math problem '''
import matplotlib.pyplot as plt
import numpy as np

TRIALS_POW = 6
BIN_SIZE = 100
if __name__ == '__main__':
    nums = np.zeros(10 ** TRIALS_POW)
    while not all(nums > 1):
        rand_incs = np.random.random(10 ** TRIALS_POW)
        mask = 1 - (nums > 1) # 0 if nums is greater than 1
        nums += rand_incs * mask

    n, bins, patches = plt.hist(nums, BIN_SIZE)

    bin_centers = (bins[:-1] + bins[1:]) / 2
    unnormalized_fit = np.e - np.exp(bin_centers - 1)
    fit = (unnormalized_fit / sum(unnormalized_fit)) * (10 ** TRIALS_POW)

    plt.title('Histogram of Ending Values with 10E{} trials'.format(TRIALS_POW))
    plt.xlabel('Ending Value. Mean is {}'
               .format(round(sum(nums) / 10 ** TRIALS_POW, 4))
              )
    plt.ylabel('Occurrence Count')
    plt.plot(bin_centers, fit, 'r-', label='Fit')
    plt.legend()
    plt.savefig('trials10e{}.png'.format(int(TRIALS_POW)))
