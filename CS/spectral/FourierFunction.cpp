#include "FourierFunction.hpp"
#include <cstdio>

void FourierFunction::clear() {
    if (f_coord) delete [] f_coord;
    if (f_k) delete [] f_k;
}
FourierFunction::FourierFunction(FourierFunction *f) {
    len = f->len;
    if (f->f_coord) {
        f_coord = new Complex[len];
        std::memcpy(f_coord, f->f_coord, len * sizeof(Complex));
    } else {
        f_coord = 0;
    }

    if (f->f_k) {
        f_k = new Complex[len];
        std::memcpy(f_k, f->f_k, len * sizeof(Complex));
    } else {
        f_k = 0;
    }
}
FourierFunction::~FourierFunction() {
    clear();
}

//
void FourierFunction::set_coord(Complex *f_c, int new_len) {
    clear();
    len = new_len;
    f_coord = f_c;
    f_k = 0;
}
void FourierFunction::set_ft(Complex *f_ft, int new_len) {
    clear();
    len = new_len;
    f_coord = 0;
    f_k = f_ft;
}

void FourierFunction::fft() {
    // TODO implement
    if (f_k) delete [] f_k;
    f_k = new Complex[len];
    std::memcpy(f_k, f_coord, len * sizeof(Complex));
}
void FourierFunction::ifft() {
    // TODO implement
    if (f_coord) delete [] f_coord;
    f_coord = new Complex[len];
    std::memcpy(f_coord, f_k, len * sizeof(Complex));
}
