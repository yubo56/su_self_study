#include <stdio.h> /* printf */
#include "newton.h"
#include <math.h> /* pow, acos */
#include <stdlib.h>

/* test function for minimizer */
double f(double arg)
{
    return pow(arg - M_PI, 2) + 10 * cos(arg / 2);
}

int main(int argc, const char *argv[])
{
    double res;
    double* init = malloc(sizeof(double));
    *init = 3;
    res = min(&f, init, 1e-5);
    printf("Min is %f\n\twith argument %f\n", res, *init);
    return 0;
}
