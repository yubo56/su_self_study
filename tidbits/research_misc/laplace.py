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

if __name__ == '__main__':
    fig = plt.figure(figsize=(6, 5))
    eps_arr = np.concatenate((
        np.linspace(0, np.pi / 2 * 0.9, 100)[1: ],
        np.linspace(np.pi / 2 * 0.9, np.pi / 2, 300)[1: ]
    ))
    phi = np.linspace(0, np.pi, 1000)
    areas = []
    for eps in eps_arr[:-1]:
        integrand = np.sqrt(
            np.sin(phi)**2 /
            (1 / np.tan(eps / 2)**2 - np.cos(phi)**2)
        )
        integ = np.trapz(integrand, phi)
        areas.append(integ / (np.pi))
    areas.append(1) # for the eps=pi / 2 case
    plt.plot(eps_arr, areas)
    plt.plot(eps_arr, 2 / np.pi * np.arctan(np.sqrt(np.sin(eps_arr / 2)**2 /
                                        np.cos(eps_arr))))
    plt.xlabel(r'$\epsilon$')
    plt.ylabel(r'$A_{\rm sep, \max} / 4\pi$')
    plt.xticks([0, np.pi / 4, np.pi / 2],
               labels=['0', r'$\pi / 4$', r'$\pi / 2$'])
    plt.yticks([0, 0.5, 1], labels=['0', '0.5', '1'])
    plt.xlim(0, np.pi / 2)
    plt.ylim(0, 1)
    plt.tight_layout()
    plt.savefig('laplace', dpi=400)
    plt.close()
