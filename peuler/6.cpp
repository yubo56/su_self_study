/**
 * Find the difference between the sum of the squares of the first 100 natural
 * numbers and the square of the sum
 */
#include <cstdio>
// int N = 10; // answer is 2640
int N = 100;
int main(int argc, const char *argv[])
{
    int sum = 0;
    int sum_sq = 0;
    for (int i = 0; i <= N; i++)
    {
        sum += i;
        sum_sq += i * i;
    }
    printf("Difference of sum-sq and sq-sum for N=%d is %d\n",
            N, sum*sum - sum_sq);
    return 0;
}
