#include "TestUtils.hpp"
bool is_equal(double d1, double d2) {
    return abs(d2 - d1) < 1e-10;
}
