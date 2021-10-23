/**
 * test cases for util.hpp functions
 */
#include "util.hpp"
#include <cstdio>
#include <cmath>
int main(int argc, const char *argv[])
{
    // int a = 2;
    // int pow = 2;
    // printf("%d\n", my_pow<int>(a, pow));

    printf("%.15f, %.15f\n", my_root<int>(23, 2, 4), pow(23, 1./2));
    printf("%.15f, %.15f\n", my_root<int>(23, 3, 4), pow(23, 1./3));

    // bool is_prime[100];
    // populate_primes(is_prime, 100);
    // for (int i = 0; i < 100; i++)
    // {
    //     if (is_prime[i]) { printf("%d\n", i + 2); }
    // }

    return 0;
}
