#include "Complex.hpp"
Complex::Complex(Complex *c) {
    re = c->re;
    im = c->im;
    str = 0;
}

//
Complex Complex::operator-() {
    return Complex(-re, -im);
}
Complex Complex::operator+(Complex c1) {
    return Complex(re + c1.re, im + c1.im);
}
Complex Complex::operator-(Complex c1) {
    return Complex(re - c1.re, im - c1.im);
}
Complex Complex::operator*(Complex c1) {
    return Complex(re * c1.re - im * c1.im, re * c1.im + im * c1.re);
}

Complex Complex::conj() {
    return Complex(re, -im);
}
char* Complex::to_str() {
    str = new char[24];
    sprintf(str, "(%0.3e, %0.3ei)", re, im);
    return str;
}
double Complex::abs() {
    return std::sqrt(re * re + im * im);
}
Complex iexp(double t) {
    return Complex(cos(t), sin(t));
}
