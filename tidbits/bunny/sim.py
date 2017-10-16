#!/usr/bin/env python
"""
compute for [2, 10] unit-radius jumps in N (CLI argument, default 2) dimensions
whether a bunny remains in a radius-1 distance of the origin
"""
import sys
import numpy as np
TRIALS = 1000000

if __name__ == '__main__':
    dim = 2 if len(sys.argv) < 2 else int(sys.argv[1])
    out = open('%dsim.dat' % dim, 'w')
    for jumps in range(2, 11):
        count = 0
        start = np.zeros([dim, TRIALS])
        for _ in range(jumps):
            rands = np.random.rand(dim, TRIALS) - 0.5
            start += (rands / np.sqrt(sum(rands ** 2)))
        dist = np.sqrt(sum(start ** 2))
        out.write("%d jumps: %f\n" % (jumps, sum(dist < 1) / TRIALS))
