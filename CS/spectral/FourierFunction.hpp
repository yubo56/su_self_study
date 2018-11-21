#ifndef FOURIERFUNCTION_HPP_XCMAUQFT
#define FOURIERFUNCTION_HPP_XCMAUQFT

// stores function + its own FT
// public vars are intended to be test-only
// use limited setters to guarantee coord/FT are never stale
#include <cstring>
#include "Complex.hpp"
class FourierFunction
{
    private:
        void clear();

    public:
        int len;
        Complex *f_coord;
        Complex *f_k;

        FourierFunction (): len(0), f_coord(0), f_k(0) {};
        FourierFunction (FourierFunction *f);
        ~FourierFunction ();

        void set_coord(Complex *f_c, int new_len);
        void set_ft(Complex *f_ft, int new_len);

        void fft();
        void ifft();
};

#endif /* end of include guard: FOURIERFUNCTION_HPP_XCMAUQFT */
