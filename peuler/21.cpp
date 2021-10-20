/**
 * A number n is _amicable_ if another number m exists such that there are
 * m-many divisors of n and n-many divisors of m. Find the sum of the amicable
 * numbers below 1e4
 */
#include <cstdio>
#include <cstring>

const int N = 10000;
// const int N = 300; // should find at least 284 & 220
int main(int argc, const char *argv[])
{
    int amicable_sum = 0;
    int sum_divisors[N];
    memset(sum_divisors, 0, sizeof(sum_divisors));

    for (int i = 1; i < N; i++)
    {
        for (int j = 2; j * i < N; j++)
        {
            sum_divisors[j * i] += i;
        }
    }
    for (int i = 1; i < N; i++)
    {
        // printf("%d, %d\n", i, sum_divisors[i]);
        if (sum_divisors[i] < N && sum_divisors[sum_divisors[i]] == i)
        {
            if (sum_divisors[i] != i)
            {
                printf("%d is amicable (with %d)\n", i, sum_divisors[i]);
                amicable_sum += i;
            }
            else
            {
                printf("%d is a perfect number! Neato\n", i);
            }
        }
    }
    printf("Sum of amicables under %d is %d\n", N, amicable_sum);
    return 0;
}
