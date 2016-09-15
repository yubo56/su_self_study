#include "newton.h"
#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */
#define ABS(x) ((x) < 0 ? -(x) : (x))
double min(double (*f)(double arg), double* init, double precision)
/* minimizer function
 *
 * Inputs;
 *  double (*f)(double arg) - function pointer to function that returns a double
 *                            f should take a single argument, a double
 *  double* init - initial guess for function minimum
 *                 minimizing argument to f will be stored to pointer location
 *  double precision - compute minimum argument to f within +- this precision
 *                     set to -1 for default, 1e-10
 *
 * Output:
 *  double - minimum value of f
 *
 * Newton's method applied to the f' tells us that x_{n+1} = x_n -
 * \frac{f'(x_n)}{f''(x_n)} brings us closer to a stationary point of f,
 * or a zero of f'.
 *
 * */
{
    double vals[3]; 
    double xs[3]; /* statically allocated arrays for vals, x values
                            no need to dynamically allocate b/c scope is ok */
    double *params; /* store malloc'd array for parameters, returned from
                       helper */
    double diff; /* discrepancy between successive iterations */

    if (precision <= 0) 
    {
        precision = 1e-10; /* default precision */
    }
    /* compute three points locally around init */
    /* diff is initally infinite */
    diff = precision + 1;

    while(ABS(diff) > precision)
    {
        /* use precision/10 to compute derivatives, so computing derivatives at
         * scales much smaller than required precision in argument */
        vals[0] = (*f)(*init - precision / 10);
        vals[1] = (*f)(*init);
        vals[2] = (*f)(*init + precision / 10);
        xs[0] = - precision / 10;
        xs[1] = 0;
        xs[2] = precision / 10;
        params = quadfit(vals, xs);

        /* compute next estimate */
        diff = -(params[1]) / (2 * params[0]);
        *init += diff;
    }

    /* cleanup */
    free(params);
    return vals[1]; /* vals[1] = (*f)(*init) */
}

double* quadfit(double* vals, double* xs)
/* quadratic fitter
 *
 * Inputs:
 *  double* vals - three points to fit; USER FREE
 *  double* xs - x values of the three points
 *
 * Outputs:
 *  double* - fit parameters, a[0]x^2 + a[1]x + a[2] ; USER FREE
 *
 * Quadratic fitter; since only fitting three points, probably most useful to
 * compute derivatives
 *
 * We make a design decision to hard-code a quadratic fitter. A more general
 * fitter would probably prove to be of better general utility but for the
 * purposes of minimization only the first two derivatives are required to be
 * calculated
 *
 * */
{
    double* params = malloc(3 * sizeof(double));
    double det;

    /* three points is sufficient to uniquely determine an intersecting
     * parabola, by solving the matrix equation
     * x[0]^2    x[0]     1               a[0]                vals[0]
     * x[1]^2    x[1]     1       *       a[1]        =       vals[1]
     * x[2]^2    x[2]     1               a[2]                vals[2]
     *
     * Without resorting to an implementation of matrix/vector algebra, we can
     * invert the matrix explicitly and solve for the fit parameters
     * */

    /* determinant for inversion */
    det = xs[1] * xs[0] * xs[0] + xs[0] * xs[2] * xs[2] + xs[2] * xs[1] * xs[1]
    - xs[1] * xs[1] * xs[0] - xs[0] * xs[0] * xs[2] - xs[2] * xs[2] * xs[1];
    /* matrix inversion, coded explicitly */
    params[0] = ((xs[1] - xs[2]) * vals[0] + (xs[2] - xs[0]) * vals[1] + (xs[0]
                - xs[1]) * vals[2]) / det;
    params[1] = ((xs[2] * xs[2] - xs[1] * xs[1]) * vals[0] + (xs[0] * xs[0] -
                xs[2] * xs[2]) * vals[1] + (xs[1] * xs[1] - xs[0] * xs[0]) *
            vals[2]) / det;
    params[2] = (xs[1] * xs[2] * (xs[1] - xs[2]) * vals[0] + xs[0] * xs[2] *
            (xs[2] - xs[0]) * vals[1] + xs[0] * xs[1] * (xs[0] - xs[1]) *
            vals[2]) / det;
    /* printf("(%e,%e),(%e,%e),(%e,%e)\n\t(%e,%e,%e)\n\n", vals[0], xs[0], vals[1], xs[1], vals[2], xs[2], params[0], params[1], params[2]); */
    return params;
}
