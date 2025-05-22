from scipy.optimize import root
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
plt.rc('figure.constrained_layout', use=True)
plt.rc('savefig', bbox='tight')

def plot_H(e0, I0):
    jz = np.sqrt(1 - e0**2) * np.cos(I0)
    fig = plt.figure(figsize=(6, 5))

    xmin = jz**2

    x = np.linspace(xmin, 1, 101) # = 1-e^2
    w = np.linspace(0, 2 * np.pi, 100)
    xgrid, wgrid = np.meshgrid(x, w)
    cosigrid = jz / np.sqrt(xgrid)
    egrid = np.sqrt(1 - xgrid)
    Hgrid = ( # Shevchenko 3.41
        (2 + 3 * egrid**2) * (3 * cosigrid**2 - 1)
        + 15 * egrid**2 * (1 - cosigrid**2) * np.cos(2 * wgrid)
    )
    plt.xlabel(r'$\omega$ [$^\circ$]')
    # plt.contourf(np.degrees(wgrid), xgrid, Hgrid)
    # plt.ylabel(r'$1 - e^2$')
    # plt.axhline(1 - e0**2, c='r', ls='--')
    plt.contourf(np.degrees(wgrid), egrid, Hgrid)
    plt.ylabel(r'$e$')
    plt.axhline(e0, c='r', ls='--')
    plt.colorbar()
    plt.savefig('H_phase')
    plt.close()

if __name__ == '__main__':
    # plot_H(e0=0.95, I0=np.radians(50))

    e_arr = np.linspace(1e-3, 1 - 1e-3)
    x_arr = []
    def condition(x, e):
        return x**2 / (6 * x**2 - 3) - (1 - e**2) / (1 - 6 * e**2)
    for e in e_arr:
        x_val = root(condition, 0.5, args=e)
        x_arr.append(np.abs(x_val.x))

    I_arr = np.degrees(np.arccos(np.array(x_arr)))
    plt.plot(e_arr, I_arr)
    plt.savefig('/tmp/foo')
    plt.close()
