/**
 * Find all numbers that can be written as the sum of the 5th powers of their
 * digits. For 4th powers, there are only 3 such numbers (not including 1=1^N)
 *
 * NB: for Nth power, only need to check up through 10^N * N or so:
 *  for an M-digit number, its maximum digit sum is M * 10^N, and its minimum
 *  value is 10^(M - 1); for M < 10^(M - N - 1), solutions cannot exist.
 */
#include <cstdio>
#include <cmath>
int N = 5;
int main(int argc, const char *argv[])
{
    int sum_same = 0;
    // start summation at 2-digit nums
    for (int ndigits = 2;
            ndigits <= N || ndigits >= pow(10, ndigits - N - 1);
            ndigits++)
    {
        for (int i = pow(10, ndigits - 1); i < pow(10, ndigits); i++)
        {
            int digitsum = 0;
            int val = i;
            while (val > 0)
            {
                digitsum += pow(val % 10, N);
                val /= 10;
            }
            if (digitsum == i)
            {
                printf("%d\n", i);
                sum_same += i;
            }
        }
    }
    printf("Sum of numbers equal to their %d-power digit sums is %d\n",
            N, sum_same);

    return 0;
}
