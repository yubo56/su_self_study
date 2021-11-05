/**
 * for d < 1000, what d has the longest repeating fractional part?
 */
#include <cstdio>
#include <cstring>
// #define DEBUG
const int N = 1000;
int main(int argc, const char *argv[])
{
    int max_len = 0;
    int curr_len = 0;
    int max_div = 0;
    int remainder;
    int has_passed[N];

    for (int div = 2; div < N; div++)
    {
        memset(has_passed, 0, sizeof(has_passed));
        int digit = 0;
        remainder = 1;
#ifdef DEBUG
        printf("(%d) 0.", div);
#endif
        while (remainder > 0 && has_passed[remainder] == 0)
        {
            has_passed[remainder] = digit;
            remainder *= 10;
            digit++;
#ifdef DEBUG
            printf("%d", remainder / div);
#endif
            remainder = remainder % div;
        }
        curr_len = digit - has_passed[remainder];
        if (curr_len > max_len)
        {
            max_div = div;
            max_len = curr_len;
        }
#ifdef DEBUG
        if (remainder > 0)
        {
            printf("; Cycle of length %d\n", curr_len);
        }
        else
        {
            printf("; No cycle\n");
        }
#endif
    }
    printf("Max length cycle is %d for div %d\n", max_len, max_div);
    return 0;
}
