/**
 * Find the first 1000-digit Fib number
 */
#include <cstdio>
#include <cstring>
#include "util.hpp"
const int N_DIGITS = 1000;
int main(int argc, const char *argv[])
{
    int stop_digits = 1000;
    int a[N_DIGITS], b[N_DIGITS], c[N_DIGITS];
    memset(a, 0, sizeof(a));
    memset(b, 0, sizeof(b));
    memset(c, 0, sizeof(c));
    a[0] = 1;
    b[0] = 1;

    int fib_idx = 2; // last tried fib_idx-th fib number
    int *nums[3] = {a, b, c};
    while (nums[(fib_idx - 1) % 3][stop_digits - 1] == 0)
    {
        add_longnum(
                nums[(fib_idx + 1) % 3],
                nums[(fib_idx + 2) % 3],
                nums[fib_idx % 3],
                N_DIGITS
        );
        // print_longnum(nums[fib_idx % 3], N_DIGITS);
        fib_idx++;
    }
    printf("First term with %d digits is %d-th number\n", stop_digits, fib_idx);
    return 0;
}
