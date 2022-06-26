/*
 * numbers are printed in a spiral into an N x N grid, what is the sum of the
 * diagonals? 5 ==> 101
 *
 * The increment along the i-th layer of the spiral is 2 * i. 4 numbers per
 * spiral, except zeroth, which only has one
 */
#include <cstdio>
const int N = 1001;
int main(int argc, const char *argv[])
{
    unsigned long prev = 1;
    unsigned long sum = 1;
    for (int i = 1; i < N / 2 + 1; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            prev += 2 * i;
            sum += prev;
        }
    }
    printf("For %dx%d spiral, sum is %lu\n", N, N, sum);
    return 0;
}
