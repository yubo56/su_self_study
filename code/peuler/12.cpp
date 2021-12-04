/**
 * What is the first triangle number to have over 500 divisors?
 */
#define MAX(a, b) ((a) > (b)) ? (a) : (b)
#include <cstdio>
// int NUM_D = 5; // 28
int NUM_D = 500;
void brute_force()
{
    int i;
    int max_divisors = 1;
    int num_divisors = 0;
    for (i = 1; num_divisors < NUM_D; i++)
    {
        num_divisors = 0;
        int triangle_i = i * (i + 1) / 2;
        for (int j = 0; j < triangle_i; j++)
        {
            num_divisors += (triangle_i % (j + 1) == 0);
        }
        max_divisors = MAX(num_divisors, max_divisors);
        printf("%d-th triangle number (%d) has %d divisors (so far max=%d)\n",
                i - 1, i * (i - 1) / 2, num_divisors, max_divisors);
    }
    printf("%d-th triangle number (%d) has %d divisors\n",
            i - 1, i * (i - 1) / 2, num_divisors);
}
int N = 1e5;
void prime_alg()
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
    int divs[N];
    for (int i = 2; i < N - 3; i++)
    {
        for (int j = 0; j < N; j++)
        {
            divs[j] = 0;
        }
        // check prime factorizations for i, i + 1
        int target = i;
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

        target = i + 1;
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
        divs[0] -= 1; // now contains prime factorization of i * (i + 1) / 2

        int num_factors = 1;
        for (int j = 0; j < i; j++)
        {
            num_factors *= divs[j] + 1;
        }
        printf("%d-th triangle number (%d) has %d factors\n",
                i, i * (i + 1) / 2, num_factors);
        if (num_factors >= NUM_D)
        {
            printf("Done!\n");
            break;
        }
    }
}
int main(int argc, const char *argv[])
{
    // brute force is too slow, prime-factorize the triangle numbers to count #
    // divisors
    // brute_force();
    prime_alg();
    return 0;
}
