#ifndef TESTUTILS_HPP_HMRA9B4F
#define TESTUTILS_HPP_HMRA9B4F

#include <cmath>
#include <cassert>
bool is_equal(double d1, double d2);

#define TEST(exp, inc) assert(exp); inc++;

#endif /* end of include guard: TESTUTILS_HPP_HMRA9B4F */
