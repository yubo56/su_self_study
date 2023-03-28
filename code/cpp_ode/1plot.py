import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
plt.rc('text', usetex=True)
plt.rc('font', family='serif', size=20)
plt.rc('lines', lw=2.5)
plt.rc('xtick', direction='in', top=True, bottom=True)
plt.rc('ytick', direction='in', left=True, right=True)
plt.rc('figure', figsize=(8.0, 8.0), dpi=300)

def plot_pendulum(fn):
    t, q, qp = np.loadtxt('%s.txt' % fn, delimiter=',').T
    fig, (ax1, ax2) = plt.subplots(
        2, 1,
        figsize=(8, 8),
        gridspec_kw={'height_ratios': [1, 1]},
        sharex=True)
    ax1.plot(t, q)
    ax2.plot(t, qp)
    plt.savefig(fn)
    plt.close()

if __name__ == '__main__':
    plot_pendulum("1tests/pendulum01")
    plot_pendulum("1tests/pendulum1")
    plot_pendulum("1tests/pendulum2")
