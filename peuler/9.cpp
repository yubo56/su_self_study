/**
 * There is exactly one pythagorean triplet for which a^2 + b^2 = c^2, a + b + c
 * = 1000. Find its product
 *
 * I discovered two sets of PTriples when I was teeny tiny. If a < b < c, then
 * if a is odd, b + c = a^2, c - b = 1, and if a is even, then b + c = a^2/4, b
 * - c = 2. Do either of these work? We would need perfect squares a^2 ~< 1000,
 *   odd, or a^2 ~< 4000, even. Checking by hand:
 *
 *   - 31^2 = 961, + 31 = 992
 *   - 62^2 = 3844, +4*62 = 4092
 *
 * So it's none of these trivial ones. I guess we can check it by hand then...
 */
#include <cstdio>
int SUM = 1000;
int main(int argc, const char *argv[])
{
    int squares[SUM]; // squares[i] = (i + 1)^2
    for (int i = 0; i < SUM; i++)
    {
        squares[i] = (i + 1) * (i + 1);
    }
    for (int i = 0; i < SUM; i++)
    {
        for (int j = 0; j < i; j++)
        {
            // avoid some basic double counting
            if (j < (SUM - i - j - 2) || (SUM - i - j - 2) < 0) { continue; }
            if (squares[i] == squares[j] + (SUM - i - j - 2) * (SUM - i - j - 2))
            {
                printf("Triple is (%d, %d, %d), product is %d\n",
                        i + 1, j + 1, SUM - i - j - 2,
                        (i + 1) * (j + 1) * (SUM - i - j - 2));
            }
        }
    }
    return 0;
}
