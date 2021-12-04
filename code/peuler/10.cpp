/**
 * Find the sum of primes below 2e6
 */
#include <cstdio>
int N = 2e6;
int main(int argc, const char *argv[])
{
    bool is_prime[N]; // whether i - 2 is prime
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
    long sum = 0;
    for (int i = 0; i < N - 2; i++)
    {
        if (is_prime[i]) { sum += i + 2; }
    }
    printf("Sum is %ld\n", sum);
    return 0;
}
