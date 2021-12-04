/**
 * What is the smallest positive number that is evenly divisible by all of the
 * numbers from 1-20?
 */
#define MAX(a, b) ((a) > (b)) ? (a) : (b)
#include <cstdio>
#include <cmath>
// int N = 10; // 2520
int N = 20;
int main(int argc, const char *argv[])
{
    /**
     * (i) find all primes up to N
     * (ii) find prime factorizations of all numbers up to N as prod_i p_i^q_i,
     *  where i runs over the primes
     * smallest num is prod_i max(q_i)
     */
    bool is_prime[N - 2]; // whether i - 2 is prime
    for (int i = 0; i < N - 2; i++)
    {
        is_prime[i] = true;
    }
    for (int i = 0; i < N - 2; i++)
    {
        if (!is_prime[i]) { continue; }
        for (int j = 2; j * (i + 2) < N; j++)
        {
            is_prime[j * (i + 2) - 2] = false;
        }
    }
    /* for (int i = 0; i < N - 2; i++) { if (is_prime[i]) { printf("%d\n", i + 2); } } */

    int facts[N - 2];
    for (int i = 0; i < N - 2; i++)
    {
        if (is_prime[i])
        {
            facts[i] = 1;
        }
    }
    for (int i = 0; i < N - 2; i++)
    {
        int target = i + 2;
        for (int div = 0; div < i; div++)
        {
            if (!is_prime[div]) { continue; }
            int num_divs = 0;
            while (target % (div + 2) == 0)
            {
                target /= div + 2;
                num_divs++;
            }
            facts[div] = MAX(num_divs, facts[div]);
        }
    }
    int max_prod = 1;
    for (int i = 0; i < N - 2; i++)
    {
        if (is_prime[i])
        {
            max_prod *= pow(i + 2, facts[i]);
        }
    }
    printf("Smallest number that is divisible by all of [1, %d] is %d\n",
            N, max_prod);
    return 0;
}
