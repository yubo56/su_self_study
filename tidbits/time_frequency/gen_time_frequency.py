#!/usr/bin/env python
import numpy as np
from scipy.signal import gaussian
import matplotlib.pyplot as plt

LEN = 10 * np.pi
NUM_PTS = 3000
PERIOD_1 = np.pi / 11
PERIOD_2 = np.pi / 15
DEFAULT_WIDTH = 15

def windowed_signal(orig_signal, center, width=DEFAULT_WIDTH):
    """
    get a windowed signal centered at @orig_signal[@center] of length @width.
    zero pad if extends past boundaries of @orig_signal
    """
    kernel = gaussian(width, width // 8)
    offset = len(kernel) // 2
    for i in range(len(kernel)):
        if center - offset + i < 0 or center - offset + i >= len(orig_signal):
            kernel[i] = 0
        else:
            kernel[i] *= orig_signal[center - offset + i]
    return kernel

def amp(t_0):
    """ get amplitude of signal as function of t """
    t_0 = t_0
    return 1

def omega(t_0):
    """ get frequency of signal as function of t """
    return (
        (2 * np.pi / PERIOD_2) * t_0 + (2 * np.pi / PERIOD_1) * (LEN - t_0)
    ) / LEN

def max_frequencies(signal, dt, width=DEFAULT_WIDTH):
    """
    return an array of length @signal containing the max frequency of the signal
    at each bucket assuming the bins in @signal are spaced by @dt
    """
    maxes = np.zeros(len(signal))
    freqs = np.fft.fftfreq(width, dt)[ : width // 2]
    for i in range(len(signal)):
        windowed = windowed_signal(signal, i, width)
        # only need positive freqs
        windowed_sed = abs(np.fft.fft(windowed))[ : width // 2]
        maxes[i] = sum(freqs * windowed_sed) / sum(windowed_sed)
    return maxes

def time_frequency(signal, dt, width=DEFAULT_WIDTH):
    """
    returns:
        @maxes: an array of length @signal containing all frequencies of the
            signal windowed with @width assuming the bins in @signal are spaced
            by @dt
        @freqs: the frequencies found by the transform at each time
    """
    maxes = np.zeros([len(signal), width // 2])
    freqs = np.fft.fftfreq(width, dt)
    for i in range(len(signal)):
        windowed = windowed_signal(signal, i, width)
        windowed_sed = abs(np.fft.fft(windowed))
        maxes[i] = windowed_sed[ : width // 2]
    return maxes, freqs[ : width // 2]

if __name__ == '__main__':
    t = np.arange(0, LEN, LEN/NUM_PTS)
    f = np.fft.fftfreq(NUM_PTS, LEN/NUM_PTS)
    sig_orig = [amp(val) * np.sin(omega(val) * val) for val in t]
    sig = [amp(val) * np.sin(omega(val) * val) for val in t]
    sig_fft = np.fft.fft(sig)

    window = 3 * int(max(PERIOD_1, PERIOD_2) * NUM_PTS / LEN)
    max_freq = max_frequencies(sig, LEN/NUM_PTS, window)
    print(1 / PERIOD_2, 1 / PERIOD_1)
    plt.plot(t[window : -window - 1], max_freq[window : -window - 1])
    plt.plot(
        t[window : -window - 1],
        [omega(_t) / (2 * np.pi) for _t in t[window : -window - 1]],
        'r-'
    )
    plt.show()

    # builtin, I don't like it
    # plt.specgram(sig, Fs=LEN/NUM_PTS, NFFT=64, noverlap=48)
    # plt.show()
