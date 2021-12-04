#include <cstdio>
/**
 * Sum of all numbers below 1000 that are multiples or 3 or 5
 */
int main(int argc, const char *argv[])
{
    int i;
    int N = 1000;
    int sum = 0;
    for (i = 0; i < N; i++)
    {
        if (i % 3 == 0 || i % 5 == 0)
        {
            sum += i;
        }
    }
    printf("Sum below %d is %d\n", N, sum);
    return 0;
}
