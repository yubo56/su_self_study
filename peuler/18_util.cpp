/**
 * Shared util for 18 and 67
 */
#include <cstdio>
#include <cstring>
#define MAX(a, b) ((a) > (b)) ? (a) : (b)
using namespace std;
int get_el(int* nums, int row, int col)
{
    return nums[row * (row + 1) / 2 + col];
}
void do_sol(const char* fn, int n)
{
    int nums[n * (n + 1) / 2];
    int sums[n];
    FILE* datfile;

    datfile = fopen(fn, "r");
    for (int i = 0; i < n * (n + 1) / 2; i++)
    {
        fscanf(datfile, "%d", nums + i);
    }
    memset(sums, 0, sizeof sums);
    for (int row = 0; row < n; row++)
    {
        for (int col = row; col > 0; col--)
        {
            sums[col] = MAX(
                    sums[col - 1] + get_el(nums, row, col),
                    sums[col] + get_el(nums, row, col)
            );
        }
        sums[0] += get_el(nums, row, 0);
        // for (int i = 0; i <= row; i++)
        // {
        //     printf("%d ", sums[i]);
        // }
        // printf("\n");
    }
    int max_sum = sums[0];
    for (int i = 1; i < n; i++)
    {
        max_sum = MAX(sums[i], max_sum);
    }
    printf("Max sum is %d\n", max_sum);
}
