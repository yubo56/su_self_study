/**
 * What is the sum of the digits in (100!) ?
 */
#include <cstdio>
#include <cstring>
const int N = 100;
const int N_D = 300; // num digits, ~> log10(N) * N
int main(int argc, const char *argv[])
{
    int digits[N_D];
    int carry;
    int new_val;

    memset(digits, 0, sizeof digits);
    digits[10] = 1; // store in reverse order, so 0 is one-digit
    for (int i = 1; i <= N; i++)
    {
        carry = 0;
        for (int j = 0; j < N_D; j++)
        {
            new_val = digits[j] * i + carry;
            digits[j] = new_val % 10;
            carry = new_val / 10;
        }
        // printf("%d! = ", i);
        // bool to_print = false;
        // for (int i = N_D - 1; i >= 0; i--)
        // {
        //     if (digits[i] > 0) { to_print = true; }
        //     if (to_print)
        //     {
        //         printf("%d", digits[i]);
        //     }
        // }
        // printf("\n");
    }
    int digit_sum = 0;
    for (int i = N_D - 1; i >= 0; i--)
    {
        digit_sum += digits[i];
    }
    printf("Sum of digits is %d\n", digit_sum);
    return 0;
}
