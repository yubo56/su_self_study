# https://medium.com/@stallonejacob/time-series-forecast-a-basic-introduction-using-python-414fcb963000
# NB: since I made up my own data, I didn't take the log of the ts first, since
# it's only linear anyway

import os
import csv
from datetime import datetime

from statsmodels.tsa.stattools import adfuller
from statsmodels.tsa.seasonal import seasonal_decompose
import pandas as pd

import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
plt.rc('text', usetex=True)
plt.rc('font', family='serif', size=16)
plt.rc('lines', lw=3.5)
plt.rc('xtick', direction='in', top=True, bottom=True)
plt.rc('ytick', direction='in', left=True, right=True)

WINDOW=12
NUM_MONTHS=200
AIR_FN='airpassengers.csv'

def do_adf(ts):
    adf, pval, usedlag, nobs, critvals, icbest = adfuller(ts, autolag='AIC')
    print('''
Test Statistic: %f
p-value: %f
#Lags: %f
Number of observations: %f
Information Criterion: %f
Critical Values: %s
''' % (adf, pval, usedlag, nobs, icbest, str(critvals)))

def plot_seasonal_decomp(ts, fn='airpassengers_seasonal'):
    # apparently this is the "naive decomposition" and "more sophisticated
    # models should eb preferred", see docs
    ts_seasonal_decomp = seasonal_decompose(ts)

    trend = ts_seasonal_decomp.trend
    seasonal = ts_seasonal_decomp.seasonal
    residual = ts_seasonal_decomp.resid
    fig, ((ax1, ax2), (ax3, ax4)) = plt.subplots(2, 2, figsize=(8, 8),
                                                 sharex=True)
    ax1.set_ylabel('Data')
    ax1.plot(ts, 'k')
    ax2.set_ylabel('Trend')
    ax2.plot(trend, 'k--')
    ax3.set_ylabel('Seasonal')
    ax3.plot(seasonal, 'b--')
    ax4.set_ylabel('Residual')
    ax4.plot(residual, 'r--')

    for ax in ax2, ax4:
        ax.yaxis.set_label_position('right')
    plt.draw()
    stride=2
    ax3.set_xticks(ax3.get_xticks()[::stride])
    for ax in ax3, ax4:
        ax.set_xticklabels(ax.get_xticklabels(), rotation=45)

    plt.tight_layout()
    plt.savefig(fn)
    plt.clf()

    residual.dropna(inplace=True)
    return residual

def gen_airdata():
    if os.path.exists(AIR_FN):
        return

    def get_months(num_months_ago=100):
        now = datetime.now()
        months = []
        for tot_months_ago in range(100, 0, -1):
            years_ago, months_ago = divmod(tot_months_ago, 12)
            new_month_provis = now.month - months_ago
            new_year = now.year - years_ago
            # coerce month to 1..12
            new_month = (new_month_provis + 12 - 1) % 12 + 1
            new_year -= (new_month - new_month_provis) // 12
            date_ago = now.replace(year=new_year, month=new_month)
            months.append(datetime.strftime(date_ago, '%Y-%m'))
        return months

    with open(AIR_FN, 'w', newline='') as f:
        csv_writer = csv.writer(f)
        csv_writer.writerow(['Month', 'Passengers'])
        month_strs = get_months(NUM_MONTHS)
        for idx, month in enumerate(month_strs):
            num_passengers = int(80 + 40 * np.random.rand())
            # non-stationarity (linear growth)
            num_passengers += int(idx / 5)
            # seasonality (seasonal modulation)
            num_passengers += 5 * (idx % 12)
            csv_writer.writerow([month, num_passengers])

def parse_airdata():
    data = pd.read_csv(AIR_FN)

    # to_datetime seems really powerful, format=None by default can parse
    # intelligently
    data['Month'] = pd.to_datetime(data['Month'])

    # uses Month for index instead of 0..len, seems powerful
    data.set_index('Month', inplace=True)
    ts = data['Passengers']
    # print(ts['2019-01-01':'2019-03-01'])

    def plot(ts, fn='airpassengers'):
        # builtins for smoothing
        rolling_ts = ts.rolling(window=WINDOW)
        rolling_mean = rolling_ts.mean()
        rolling_std = rolling_ts.std()

        fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(6, 9), sharex=True,
                                       gridspec_kw={'height_ratios': [2, 1]})
        ax1.plot(ts, 'k', label='Data')
        ax1.plot(rolling_mean, 'b:', label='Rolling Mean')
        ax1.set_ylabel('Passengers')
        ax2.plot(rolling_std, 'r:')
        ax2.set_xlabel('Date')
        ax2.set_ylabel('Stdev')
        # need to draw so that the tick labels get populated??
        plt.draw()
        ax2.set_xticklabels(ax2.get_xticklabels(), rotation=45)
        ax1.legend(loc='upper left')

        plt.tight_layout()
        plt.savefig(fn)
        plt.close()
    plot(ts)

    # adfuller test, if test stat > critical values then non-stationary
    # auto-determine lag via Akaike Information Criterion
    do_adf(ts)

    def rm_trend():
        # subtract mean, tradition is to throw data out
        # dropna(): sledgehammers & coconuts...
        ts_rm_mean = (ts - ts.rolling(window=WINDOW).mean())
        # ts_rm_mean = ts_rm_mean[WINDOW: ]
        ts_rm_mean.dropna(inplace=True)

        # smooth over seasonality, should be stationary now
        # ts_rm_mean_avg = ts_rm_mean.rolling(window=WINDOW).mean()
        ts_rm_mean_avg = ts_rm_mean.ewm(halflife=WINDOW).mean()
        ts_rm_mean_avg.dropna(inplace=True)
        do_adf(ts_rm_mean_avg)
        plot(ts_rm_mean_avg, fn='airpassengers_clean')
    # rm_trend()

    def rm_trend_delta():
        # other way to rm trend is to model only the deltas/derivatives
        ts_diff = ts - ts.shift()
        ts_diff.dropna(inplace=True)
        do_adf(ts_diff)
    # rm_trend_delta()

    def seasonal_decomp_test():
        residual = plot_seasonal_decomp(ts)
        do_adf(residual)

    # seasonal_decomp_test()

if __name__ == '__main__':
    gen_airdata()
    parse_airdata()
