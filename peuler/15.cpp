/**
 * How many paths from TL to BR of a 20x20 grid, along grid lines?
 *
 * This is analytic... 40C20. Ah, but it overflows when calculating
 */
#include <cstdio>
#include <cmath>
int N = 20;
int main(int argc, const char *argv[])
{
    bool is_prime[2 * N - 2]; // whether i - 2 is prime
    for (int i = 0; i < 2 * N - 2; i++)
    {
        is_prime[i] = true;
    }
    for (int i = 0; i < 2 * N - 2; i++)
    {
        if (!is_prime[i]) { continue; }
        for (int j = 2; j * (i + 2) < 2 * N; j++)
        {
            is_prime[j * (i + 2) - 2] = false;
        }
    }

    int divs[2 * N - 2];
    for (int i = 0; i < 2 * N - 2; i++)
    {
        divs[i] = 0;
    }
    for (int i = 0; i < N; i++)
    {
        // check prime factorizations for i + 1, N + i + 1
        int target = i + 1;
        for (int div = 0; target > 2 && !is_prime[target - 2]; div++)
        {
            if (!is_prime[div]) { continue; }
            while (target % (div + 2) == 0)
            {
                divs[div] -= 1;
                target /= (div + 2);
            }
        }
        if (target - 2 >= 0)
        {
            divs[target - 2] -= 1;
        }

        target = N + i + 1;
        for (int div = 0; target > 2 && !is_prime[target - 2]; div++)
        {
            if (!is_prime[div]) { continue; }
            while (target % (div + 2) == 0)
            {
                divs[div] += 1;
                target /= (div + 2);
            }
        }
        divs[target - 2] += 1;
        // now contains prime factorization of (N + i + 1) / (i + 1)
    }
    long prod = 1;
    for (int j = 0; j < 2 * N - 2; j++)
    {
        if (is_prime[j])
        {
            prod *= pow(j + 2, divs[j]);
        }
    }
    printf("%ld paths (%d C %d)\n", prod, 2 * N, N);
    return 0;
}
