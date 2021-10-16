/**
 * Largest prime factor of 600851475143?
 */
#include <cstdio>
#include <cmath>
/* int N = 13195; /1* test case, 3*7*13*29, => 29 *1/ */
long N = 600851475143;
int main(int argc, const char *argv[])
{
    int len = ceil(sqrt(N));
    bool is_prime[len]; /* whether [idx] is prime */
    /* use sieve to generate list of primes up to sqrt(N) */
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i < len; i++)
    {
        is_prime[i] = true;
    }
    for (int i = 2; i < len; i++)
    {
        if (!is_prime[i]) { continue; }
        for (int j = 2; j * i < len; j++)
        {
            is_prime[j * i] = false;
        }
    }
    /* checking */
    /* for (int i = 0; i < len; i++) { if (is_prime[i]) { printf("%d\n", i); } } */
    long curr_n = N;
    int largest_prime = 2;
    for (int i = largest_prime; i < len; i++)
    {
        if (!is_prime[i]) { continue; }
        int divs = 0;
        while (curr_n % i == 0)
        {
            curr_n /= i;
            divs += 1;
            largest_prime = i;
        }
        if (divs > 0)
        {
            printf("%d^%d\n", i, divs);
        }
    }
    printf("Largest prime divisor of %ld is %d\n", N, largest_prime);
    return 0;
}
