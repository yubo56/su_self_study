/**
 * a number is abundant if the sum of its proper divisors is greater than n. All
 * numbers above 28123 can be written as the sum of two abundant numbers. Find
 * the sum of all positive integers which cannot be written as the sum of two
 * abundant numbers
 */
#include <cstdio>
#include <cstring>

const int N = 28124;
int main(int argc, const char *argv[])
{
    int nonabundant_sum = 0;
    bool is_sum_abundant;
    int sum_divisors[N];
    memset(sum_divisors, 0, sizeof(sum_divisors));

    for (int i = 1; i < N; i++)
    {
        for (int j = 2; j * i < N; j++)
        {
            sum_divisors[j * i] += i;
        }
    }
    // for (int i = 1; i < N; i++)
    // {
    //     if (sum_divisors[i] > i)
    //     {
    //         printf("%d is abundant (sum %d)\n", i, sum_divisors[i]);
    //     }
    // }
    for (int cand = 1; cand < N; cand++)
    {
        is_sum_abundant = false;
        for (int sub = 1; sub < cand; sub++)
        {
            if (sum_divisors[sub] > sub &&
                    sum_divisors[cand - sub] > cand - sub)
            {
                is_sum_abundant = true;
                break;
            }
        }
        if (!is_sum_abundant)
        {
            nonabundant_sum += cand;
        }
    }
    printf("%d is the sum of non-abundant-sum numbers\n", nonabundant_sum);
    return 0;
}
