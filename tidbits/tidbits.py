import math
import numpy as np

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

if __name__ == '__main__':
    run_borwein(13)
    run_borwein(15)
