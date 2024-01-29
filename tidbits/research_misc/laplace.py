import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
plt.rc('text', usetex=True)
plt.rc('font', family='serif', size=20)
plt.rc('lines', lw=2.5)
plt.rc('xtick', direction='in', top=True, bottom=True)
plt.rc('ytick', direction='in', left=True, right=True)
plt.rc('xtick.minor', visible=True)
plt.rc('ytick.minor', visible=True)
plt.rc('figure', figsize=(8.0, 8.0), dpi=300)

def sqrt(x):
    return np.sqrt(np.maximum(x, 0))
def get_area(u, eps):
    ''' returns fractional area (A / 4pi) '''
    if u > 1/0.15**5:
        return 0
    eps = min(eps, np.pi - eps)
    acosw_p = np.arccos(sqrt(
        (1 + u - sqrt(1 + u**2 + 2 * u * np.cos(2 * eps)))
        / (2 * np.sin(eps)**2)))
    acosw_m = np.pi - acosw_p
    phi1, dphi1 = np.linspace(0, acosw_p, 500, retstep=True)
    phi2, dphi2 = np.linspace(acosw_p, acosw_m, 500, retstep=True)
    phi3, dphi3 = np.linspace(acosw_m, np.pi, 500, retstep=True)
    phi1 = phi1[1:-1]
    phi2 = phi2[1:-1]
    phi3 = phi3[1:-1]
    def get_x_sign(phi, sign=+1):
        return (
            (np.cos(phi) * np.sin(2 * eps)
                + sign * np.sin(phi) * np.sin(eps) * sqrt(
                    2 * (u - 1 + sqrt(1 + u**2 + 2 * u * np.cos(2 * eps)))))
            / (
                u + 1 - 2 * np.cos(phi)**2 * np.sin(eps)**2
                - sqrt(1 + u**2 + 2 * u * np.cos(2 * eps))))
    integ_p1 = -1 / sqrt(1 + get_x_sign(phi1, +1)**2)
    integ_m1 = -1 / sqrt(1 + get_x_sign(phi1, -1)**2)
    integ_p2 = -1 / sqrt(1 + get_x_sign(phi2, +1)**2)
    integ_m2 = -1 / sqrt(1 + get_x_sign(phi2, -1)**2)
    integ_p3 = -1 / sqrt(1 + get_x_sign(phi3, +1)**2)
    integ_m3 = -1 / sqrt(1 + get_x_sign(phi3, -1)**2)
    Afrac = (
        np.sum(integ_p1 - integ_m1) * dphi1
        + np.sum(-integ_p2 - integ_m2) * dphi2
        + np.sum(-integ_p3 + integ_m3) * dphi3) / (2 * np.pi)
    return Afrac

def plot_u1():
    fig = plt.figure(figsize=(6, 5))
    eps_arr = np.concatenate((
        np.linspace(0, np.pi / 2 * 0.9, 100)[1: ],
        np.linspace(np.pi / 2 * 0.9, np.pi / 2, 300)[1: ]
    ))
    plt.plot(eps_arr,
             2 / np.pi * np.arctan(
                 sqrt(np.sin(eps_arr / 2)**2 / np.cos(eps_arr))),
             'k',
             label='Simple')
    areas = [get_area(1, eps) for eps in eps_arr]
    plt.plot(eps_arr, areas, 'g--', label='Full')

    plt.xlabel(r'$\epsilon$')
    plt.ylabel(r'$A_{\rm sep, \max} / 4\pi$')
    plt.xticks([0, np.pi / 4, np.pi / 2],
               labels=['0', r'$\pi / 4$', r'$\pi / 2$'])
    plt.yticks([0, 0.5, 1], labels=['0', '0.5', '1'])
    plt.xlim(0, np.pi / 2)
    plt.ylim(0, 1)
    plt.legend()
    plt.tight_layout()
    plt.savefig('laplace', dpi=400)
    plt.close()

def plot_2d_test():
    eps = np.linspace(0, np.pi, 1000)[1:-1] # skip eps = 90
    u_base = np.linspace(0, 2.5, 200)
    u_base_grid, eps_grid = np.meshgrid(u_base, eps)
    areas_grid = np.reshape(
        [get_area(1 / _u**5, _eps)
         for _u, _eps in zip(u_base_grid.flat, eps_grid.flat)],
        eps_grid.shape)

    plt.contourf(u_base_grid, eps_grid, areas_grid, levels=100)
    plt.colorbar()
    plt.contour(u_base_grid, eps_grid, areas_grid,
                levels=np.arange(0.1, 1, 0.1), colors='k')
    xticks = np.arange(0, 2.6, 0.5)
    plt.xlabel(r'$a / r_{\rm M}$')
    plt.xticks(xticks, labels=[repr(x) for x in xticks])
    yticks = np.arange(0, np.pi + 0.1, np.pi / 6)
    yticklabels = [repr(r) for r in np.arange(0, 181, 30)]
    plt.axhline(np.pi / 2, c='g', lw=2)
    plt.axvline(1, c='g', lw=2)
    plt.yticks(yticks, labels=yticklabels)
    plt.ylabel(r'$\varepsilon$ (deg)')
    plt.tight_layout()
    plt.savefig('laplace_2d', dpi=200)
    plt.close()

if __name__ == '__main__':
    plot_u1()
    # plot_2d_test()
