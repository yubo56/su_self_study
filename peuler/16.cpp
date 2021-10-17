/**
 * What is the sum of the digits of 2^1000?
 *
 * Sol: multiplication by hand? # digits < 1000
 */
#include <cstdio>
const int N = 1000;
// int N = 1000;
int digits[N]; // 2^N has fewer than N digits, so this should work
int main(int argc, const char *argv[])
{
    for (int i = 0; i < N - 1; i++)
    {
        digits[i] = 0;
    }
    digits[N - 1] = 1;
    for (int pow = 0; pow < N; pow++)
    {
        int carry = 0;
        for (int j = N - 1; j >= 0; j--)
        {
            digits[j] = 2 * digits[j] + carry;
            carry = 0;
            if (digits[j] >= 10)
            {
                carry = 1;
                digits[j] -= 10;
            }
        }
    }
    // bool to_print = false;
    // printf("2^%d = ", N);
    // for (int i = 0; i < N; i++)
    // {
    //     if (digits[i] > 0) { to_print = true; }
    //     if (to_print)
    //     {
    //         printf("%d", digits[i]);
    //     }
    // }
    // printf("\n");

    int sum_digits = 0;
    for (int i = 0; i < N; i++)
    {
        sum_digits += digits[i];
    }
    printf("Sum of digits of 2^%d is %d\n", N, sum_digits);
    return 0;
}
