#include "Tests.hpp"
#include "TestUtils.hpp"
int test_complex() {
    int passes = 0;
    Complex c1(1, 2);

    //
    TEST(is_equal(c1.re, 1), passes);
    TEST(is_equal(c1.im, 2), passes);

    //
    TEST(is_equal((-c1).re, -c1.re), passes);
    TEST(is_equal((-c1).im, -c1.im), passes);

    //
    Complex c2(2, 3);
    Complex c3 = c1 + c2;
    TEST(is_equal(c3.re, c1.re + c2.re), passes);
    TEST(is_equal(c3.im, c1.im + c2.im), passes);

    //
    Complex c4(2, 3);
    Complex c5 = c1 - c2;
    TEST(is_equal(c5.re, c1.re - c2.re), passes);
    TEST(is_equal(c5.im, c1.im - c2.im), passes);

    //
    Complex c6(4, 5);
    Complex c7 = c1 * c6;
    TEST(is_equal(c7.re, c1.re * c6.re - c1.im * c6.im), passes);
    TEST(is_equal(c7.im, c1.re * c6.im + c1.im * c6.re), passes);

    //
    Complex c8 = c1.conj();
    TEST(is_equal(c8.re, c1.re), passes);
    TEST(is_equal(c8.im, -c1.im), passes);

    //
    TEST(!strcmp(c1.to_str(), "(1.000e+00, 2.000e+00i)"), passes);

    //
    TEST(is_equal(c1.abs(), sqrt(c1.re * c1.re + c1.im * c1.im)), passes);

    //
    double exp = 0.1;
    Complex c9 = iexp(exp);
    TEST(is_equal(c9.re, cos(exp)), passes);
    TEST(is_equal(c9.im, sin(exp)), passes);

    return passes;
}

Complex *test_arr(int len) {
    Complex *arr = new Complex[len];
    for (int i = 0; i < len; i++)
    {
        arr[i] = Complex(1, 2);
    }
    return arr;
}
int test_fourier() {
    int passes = 0;

    //
    FourierFunction f1;
    int len = 5;
    Complex *arr = test_arr(len);
    f1.set_coord(arr, len);
    TEST(f1.len == len, passes);
    TEST(f1.f_coord != 0, passes);
    TEST(f1.f_k == 0, passes);

    //
    f1.fft();
    TEST(f1.len == len, passes);
    TEST(f1.f_coord != 0, passes);
    TEST(f1.f_k != 0, passes);

    //
    Complex *arr2 = test_arr(len);
    f1.set_ft(arr2, len);
    TEST(f1.len == len, passes);
    TEST(f1.f_coord == 0, passes);
    TEST(f1.f_k != 0, passes);

    //
    f1.ifft();
    TEST(f1.len == len, passes);
    TEST(f1.f_coord != 0, passes);
    TEST(f1.f_k != 0, passes);

    //
    FourierFunction f2(&f1);
    TEST(f2.len == len, passes);
    TEST(f2.f_coord != 0, passes);
    TEST(f2.f_k != 0, passes);

    return passes;
}
