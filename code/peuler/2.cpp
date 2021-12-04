/**
 * Sum of all even Fibonacci numbers below 4 million.
 *  Sol 1: do iteratively
 *  Sol 2: use the explicit formula
 */
#include <cstdio>
#include <cmath>
int sol1(int N)
{
    int fib1 = 1;
    int fib2 = 1;
    int sum = 0;
    while (fib2 < N)
    {
        if (fib2 % 2 == 0)
        {
            sum += fib2;
        }
        fib2 += fib1;
        fib1 = fib2 - fib1;
    }
    return sum;
}
int sol2(int N)
{
    /**
     * Explicit solution, re-derive.
     *
     * Solve: a_{i + 2} = a_{i + 1} + a_i, a_0 = a_1 = 1
     * - Ansatz: a_k = r^k, then r^2 = r + 1 => r = (1 +- sqrt(5)) / 2
     * - General solution is linear combination of two lin-indep sols, so
     *   a_k = Ar_1^k + Br_2^k
     *
     * - Use ICs, A + B = 1, (A + B) / 2 + (A - B) * sqrt(5) / 2 = 1
     *   => (1 - 2B) * sqrt(5) = 1
     *   => B = (1 - 1 / sqrt(5)) / 2
     *   => A = (1 + 1 / sqrt(5)) / 2
     *   => a_k = (r_1^(k + 1) + r_2^(k + 1)) / sqrt(5)
     */
    int sum = 0;
    int k = 0;
    int fib_k = round(
            (pow((1 + sqrt(5)) / 2, k + 1) - pow((1 - sqrt(5)) / 2, k + 1))
            / sqrt(5));
    while (fib_k < N)
    {
        if (fib_k % 2 == 0)
        {
            sum += fib_k;
        }
        k++;
        fib_k = round(
                (pow((1 + sqrt(5)) / 2, k + 1) - pow((1 - sqrt(5)) / 2, k + 1))
                / sqrt(5));
    }
    return sum;
}
int main(int argc, const char *argv[])
{
    int N = 4e4;
    printf("Sum of Fibonacci numbers below %d is %d\n", N, sol1(N));
    printf("Sum of Fibonacci numbers below %d is %d (explicit)\n", N, sol2(N));
    return 0;
}
