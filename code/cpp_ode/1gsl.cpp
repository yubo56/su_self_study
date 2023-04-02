#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include <filesystem>

#include <gsl/gsl_errno.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_odeiv2.h>

using namespace std;

// calculates dydt = f(t, y)
int dydt(double t, const double y[], double f[], void *params)
{
    f[0] = y[1];
    f[1] = -sin(y[0]);
    return GSL_SUCCESS;
}
// calculates jacobian dfdy[i, j] = df_i/dy_j
int jac(double t, const double y[], double *dfdy, double dfdt[], void *params)
{
    gsl_matrix_view dfdy_mat = gsl_matrix_view_array (dfdy, 2, 2);
    gsl_matrix *m = &dfdy_mat.matrix;
    gsl_matrix_set(m, 0, 0, 0.0);
    gsl_matrix_set(m, 0, 1, 1.0);
    gsl_matrix_set(m, 1, 0, -cos(y[0]));
    gsl_matrix_set(m, 1, 1, 0);
    dfdt[0] = 0.0;
    dfdt[1] = 0.0;
    return GSL_SUCCESS;
}

/**
 * Integrates a simple pendulum with initial amplitude amp0 (radians)
 */
void run_pendulum(float amp0, string fldr, string filefn)
{
    ostringstream _fn;
    _fn << fldr << "/" << filefn;
    string fn = _fn.str();

    ofstream outfile;
    outfile.open(fn);

    gsl_odeiv2_system sys = {dydt, jac, 2};
    gsl_odeiv2_driver *d = gsl_odeiv2_driver_alloc_y_new(
            &sys, gsl_odeiv2_step_rk8pd, 1e-9, 1e-9, 0.0);

    int step;
    int nsteps = 1000;
    double t = 0.0, t1 = 100.0;
    double y[2] = { amp0, 0.0 };

    for (step = 0; step < nsteps; step++)
    {
        double ti = (step + 1) * t1 / nsteps;
        int status = gsl_odeiv2_driver_apply (d, &t, ti, y);

        if (status != GSL_SUCCESS)
        {
            printf ("error, return value=%d\n", status);
            break;
        }

        outfile << t << ", " << y[0] << ", " << y[1] << endl;
    }

    gsl_odeiv2_driver_free(d);
    outfile.close();
}

int main(int argc, const char *argv[])
{
    string fldr = "1tests";
    std::filesystem::create_directory(fldr);
    run_pendulum(0.1, fldr, "pendulum01.txt");
    run_pendulum(1, fldr, "pendulum1.txt");
    run_pendulum(2, fldr, "pendulum2.txt");
    return 0;
}
