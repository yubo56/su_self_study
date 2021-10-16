/**
 * Largest palindrome that is the product of 2 3-digit numbers
 */
#include <cstdio>
#include <cmath>
/* int N_DIGITS = 2; /1* 2 digits => 9009 *1/ */
int N_DIGITS = 3;
int get_palindrome(int base)
{
    /* assert: 10^N_DIGITS <= base < 10^(N_DIGITS + 1) */
    int ret = 0;
    for (int i = 0; i < N_DIGITS; i++)
    {
        int digit = base / ((int) round(pow(10, i))) % 10;
        ret += digit * pow(10, N_DIGITS - i - 1);
    }
    ret += base * ((int) round(pow(10, N_DIGITS)));
    return ret;
}
int main(int argc, const char *argv[])
{
    for (int base = ((int) pow(10, N_DIGITS)) - 1;
            base >= pow(10, N_DIGITS - 1);
            base--)
    {
        int cand = get_palindrome(base);
        int success_div = -1;
        for (int div = ((int) pow(10, N_DIGITS)) - 1;
                div >= pow(10, N_DIGITS - 1);
                div--)
        {
            if (cand % div == 0 &&
                    cand / div < ((int) pow(10, N_DIGITS)) &&
                    cand / div >= pow(10, N_DIGITS - 1))
            {
                success_div = div;
                break;
            }
        }
        if (success_div > 0)
        {
            printf("%d has quotient %d x %d\n",
                    cand, success_div, cand / success_div);
            break;
        }
    }
    return 0;
}
