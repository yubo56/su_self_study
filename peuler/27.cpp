/**
 * For |a| < 1000, |b| <= 1000, find a, b such that n^2 + an + b has the maximum
 * number of consecutive prime solutions for n >= 0
 *
 * NB: Not sure how many primes I need, try 1e6 for now?
 */
#include "util.hpp"
const int N = 1e3; // maximum value of N to try
int main(int argc, const char *argv[])
{
    int max_consec = 0;
    int max_n = 0;
    const int num_primes = N * N + 1000 * N + 1001;
    bool is_prime[num_primes];
    populate_primes(is_prime, num_primes);
    // int a = -79;
    // int b = 1601;
    for (int a = -999; a < 1000; a++)
    {
        for (int b = -1000; b < 1001; b++)
        {
            int n = 0;
            for (; n < N && is_prime[n * n + a * n + b - 2]; n++)
            {
                // printf("%d is prime\n", n * n + a * n + b);
            }
            if (n > max_consec && n < N)
            {
                max_n = a * b;
                max_consec = n;
                // printf("(a=%d, b=%d) has %d consecutive primes\n", a, b, n);
            }
        }
    }
    printf("a * b = %d\n", max_n);

    return 0;
}
