'''
Numerical integration to solve the following problem: choose points on a unit
sphere, then select points with |y| < sin(i0). What is the distribution of z?

Has applications to ZLK, which has applications to everything
'''
import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
plt.rc('text', usetex=True)
plt.rc('font', family='serif', size=20)
plt.rc('lines', lw=1.5)
plt.rc('xtick', direction='in', top=True, bottom=True)
plt.rc('ytick', direction='in', left=True, right=True)
plt.rc('xtick.minor', visible=True)
plt.rc('ytick.minor', visible=True)
plt.rc('figure', figsize=(8.0, 8.0), dpi=300)
plt.rc('savefig', bbox='tight')

def norm_hist(counts, dx):
    return (counts / np.sum(counts)) / dx

def hist_for_i0(
    N_REPEAT=1000,
    N_PTS=100000,
    nbins=300,
    i0_d=50,
):
    '''
    get numerical distribution for cutoff {i0_d} by drawing {N_PTS} random
    points on a sphere {N_REPEAT} times, and binning them into {nbins}

    using N_REPEAT is so that we don't run into memory issue drawing 10^8 points
    '''
    ycrit = np.sin(np.radians(i0_d))
    bin_edges = np.linspace(-1, 1, nbins + 1)
    bin_centers = (bin_edges[ :-1] + bin_edges[1: ]) / 2
    bin_width = bin_edges[1] - bin_edges[0]

    counts = np.zeros(nbins)
    for _ in range(N_REPEAT):
        # points uniformly sampled in z = cos(phi), theta are uniformly
        # distributed on the surface of a sphere
        # only use theta \in (0, pi), so y is always positive (avoids taking the
        # absolute value later)
        z = np.random.uniform(-1, 1, N_PTS)
        theta = np.random.uniform(0, np.pi, N_PTS)
        y = np.sqrt(1 - z**2) * np.sin(theta)
        counts += np.histogram(
            z[y < ycrit],
            bins=bin_edges,
        )[0]
    return bin_centers, counts, bin_width

def hist_theory(cos_i, dx, i0_d=50):
    '''
    calculate analytical distribution
    '''
    sin_i = np.sqrt(1 - cos_i**2)
    sin_i_crit = np.sin(np.radians(i0_d))
    dist = np.zeros_like(cos_i)
    dist[sin_i < sin_i_crit] = 2 * np.pi
    dist[sin_i > sin_i_crit] = (
        4 * np.arcsin(sin_i_crit / sin_i[sin_i > sin_i_crit])
    )
    return dist

def calc_mean_sini(x, dist, dx):
    '''
    given {x} in [-1, 1] the bin centers, and {dist} the distribution of cos(i)
    in those bins, what is the <|sin i|>?
    '''
    sin_i = np.sqrt(1 - x**2)
    return np.sum(sin_i * dist * dx)

if __name__ == '__main__':
    fig, (ax1, ax2) = plt.subplots(
        1, 2,
        figsize=(10, 6),
        sharex=True, sharey=True)
    for i0_d in [1, 20, 50, 80, 90]:
        x, counts, dx = hist_for_i0(i0_d=i0_d)
        ax1.plot(
            x,
            norm_hist(counts, dx),
            label=(r'$i_0 = %d^\circ$' % i0_d),
        )
        counts_th = hist_theory(x, dx, i0_d=i0_d)
        norm_th = norm_hist(counts_th, dx)
        ax2.plot(x, norm_th)

        mean_sini = calc_mean_sini(x, norm_th, dx)
        print(r'Mean sin(i) for i0=%d is: %.7f' % (i0_d, mean_sini))
    ax1.legend(fontsize=14, ncol=2)
    ax1.set_ylim(0, 1)
    ax1.set_xlim(-1, 1)
    ax1.set_xlabel(r'$\cos \phi$')
    ax2.set_xlabel(r'$\cos \phi$')
    ax1.set_ylabel(r'$P(\cos \phi)$')
    ax1.set_title('Numerical')
    ax2.set_title('Analytical')
    plt.tight_layout()
    plt.savefig('sample_on_sphere.png')
