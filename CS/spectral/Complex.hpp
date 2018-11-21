#ifndef COMPLEX_HPP_RQTOKG9Z
#define COMPLEX_HPP_RQTOKG9Z

#include <string>
#include <cstdio>
#include <cmath>
class Complex
{
    public:
        double re;
        double im;
        char *str; // alloc dynamically when to_str

        Complex (): re(0), im(0), str(0) {};
        Complex (double real, double imag): re(real), im(imag), str(0) {};
        Complex (Complex *c);
        ~Complex () { if (str) delete str; }

        Complex operator-();
        Complex operator+(Complex c1);
        Complex operator-(Complex c1);
        Complex operator*(Complex c1);
        Complex conj();
        char* to_str();
        double abs();
};

Complex iexp(double t); // e^(it) = cos(t) + isin(t)

#endif /* end of include guard: COMPLEX_HPP_RQTOKG9Z */
