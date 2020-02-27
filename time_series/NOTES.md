# NOTES

### Autoregressive process (order p)
- y[j] = Sum[a[j - i] * y[j - i], {i, 1, p}] + eps[j]
    - Typically, eps = white noise
- Stationary process implies PD does not change in time
    - Includes all moments, e.g. mean + var
- Characteristic equation: m^p - m^{p - 1}a[1] - m^{p - 2}a[2] - ... - a[p] = 0
    - If m = 1 is a root with multiplicity r, the process has a "unit root", or
      is "integrated of order r", I(r).
    - Same characteristic equation as for a matrix (if abs(eigenvalues) > 1,
      divergent). In fact, is just matrix characteristic equation (a la
      Cayley-Hamilton) for matrix mapping y[j-p:j] -> y[j-p+1:j+1]
    - NB: autocorrelation of AR(p) is rho(t) = Sum[a[k]y[k]^(-t), {k,1,p}],
      where y[k] are roots of characteristic equation. Thus, AR/AC are connected
- Augmented Dicker-Fuller test seeks statistic of a unit root for p >= 1 AR
  process
    - In `statsmodels`, using autolag explores different order AR
- PACF is autocorrelation but controlling for correlations w/ intermediate
  values, is better for determining order of an AR process
