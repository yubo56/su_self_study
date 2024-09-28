from scipy.stats import binom
import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
# plt.rc('text', usetex=True)
plt.rc('font', family='serif', size=20)
plt.rc('lines', lw=2.5)
plt.rc('xtick', direction='in', top=True, bottom=True)
plt.rc('ytick', direction='in', left=True, right=True)
plt.rc('xtick.minor', visible=True)
plt.rc('ytick.minor', visible=True)
plt.rc('figure', figsize=(8.0, 8.0), dpi=300)
plt.rc('savefig', bbox='tight')

def get_overlaps_base(starts, ends, flags, n_parallel, end_final=1):
    '''
    could easily clean this up even more: remove parallelization (useless &
    obfuscates), support values being occasionally equal, support using
    start/end of interval only over starts/ends array (and not fixed to
    [0,1])... but some other day
    '''
    # flags will support N-D generalization in future
    N_tot = len(starts)
    assert len(ends) == N_tot
    N = N_tot // n_parallel
    start_args = np.argsort(starts)
    end_args = np.argsort(ends)

    areas = np.zeros((n_parallel, N + 1))
    overlap_count = np.zeros(n_parallel, dtype=int)
    prev = np.zeros(n_parallel)
    start_ptr = 0
    end_ptr = 0

    while end_ptr < N_tot:
        # TODO generalize for equality case
        if (start_ptr == N_tot) or \
            (starts[start_args[start_ptr]] > ends[end_args[end_ptr]]):
            # this interval should stop being accounted for
            curr = ends[end_args[end_ptr]]
            parallel_idx = end_args[end_ptr] // N
            areas[parallel_idx, overlap_count[parallel_idx]] += curr - prev[parallel_idx]
            prev[parallel_idx] = curr
            overlap_count[parallel_idx] -= 1
            end_ptr += 1
        else:
            # this interval should be accounted for
            curr = starts[start_args[start_ptr]]
            parallel_idx = start_args[start_ptr] // N
            areas[parallel_idx, overlap_count[parallel_idx]] += curr - prev[parallel_idx]
            prev[parallel_idx] = curr
            overlap_count[parallel_idx] += 1
            start_ptr += 1

    # account for trailing interval (between last end and end_final) on all
    # parallel_idxs
    for i in range(n_parallel):
        areas[i, 0] += end_final - prev[i]

    return areas

def get_overlaps(starts, ends, n_parallel):
    return get_overlaps_base(starts, ends, [True] * len(starts), n_parallel)

def test_1D():
    # starts = [0.1, 0.4]
    # ends = [0.6, 1]
    # n_parallel = 3
    # for i in range(1, n_parallel):
    #     starts.append(starts[0] - 0.01 * i)
    #     starts.append(starts[1] - 0.01 * i)
    #     ends.append(ends[0] - 0.01 * i)
    #     ends.append(ends[1] - 0.01 * i)
    # print(starts, ends)
    # print(get_overlaps(starts, ends, n_parallel))

    n_parallel = 400
    total_len = 10
    N = 500
    l = total_len / N
    title = f'{N:d} intervals of length {l:.3f}, {n_parallel} trials'
    starts = np.random.uniform(0, 1 - l, N * n_parallel)
    ends = starts + l

    # naive estimate of zero-occupancy fraction via binomial
    expected_zero = (1 - l)**N
    mean_lens = np.mean(get_overlaps(starts, ends, n_parallel), axis=0)
    agg_lens = get_overlaps(starts, ends, 1)[0]
    rv = binom(N, l)
    # expected zero due to boundary FX
    expected_zero_b = 2 / N

    x = np.arange(N + 1)
    plot_idx = np.where(mean_lens > 1e-15)[0][-1]
    x_agg = np.arange(N * n_parallel + 1) / n_parallel
    plt.plot(x_agg, agg_lens * n_parallel, label='Agg', alpha=0.3)
    plt.plot(x[ :plot_idx], mean_lens[ :plot_idx], label='Dat')
    plt.plot(x[ :plot_idx], rv.pmf(x[ :plot_idx]), label='Binom')

    plt.xlabel(r'Occupancy')
    plt.xlim(0, plot_idx)
    plt.ylabel(r'Expected Aggregate Length')
    # plt.axhline(expected_zero, c='k', ls='--')
    plt.axhline(expected_zero_b, c='k', ls='--')
    plt.yscale('log')
    # plt.ylim(bottom=0)
    plt.legend()
    plt.title(title, fontsize=16)
    plt.tight_layout()
    plt.savefig('1D')

if __name__ == '__main__':
    test_1D()
