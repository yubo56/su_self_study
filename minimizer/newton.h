/* newton.hpp
 * 
 * contains function to numerically minimize arbitrary function
 *
 * */

#ifndef NEWTON_H

#define NEWTON_H

double min(double (*f)(double arg), double* init, double precision);
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
 * Function that will perform Newton's method for minimization and return the
 * minimum value
 * and the minimizing argument in the pointer double* init.
 *
 * */

double* quadfit(double* vals, double* xs);
/* quadratic fitter
 *
 * Inputs:
 *  double* vals - three points to fit
 *  double* xs - x values of the three points
 *
 * Outputs:
 *  double* - fit parameters, a[0]x^2 + a[1]x + a[2]
 *
 * Quadratic fitter; since only fitting three points, probably most useful to
 * compute derivatives
 *
 * */

#endif /* end of include guard: NEWTON_H */
