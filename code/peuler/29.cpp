/**
 * How many distinct terms in the sequence {a^b; a, b \in [2, 100]}?
 * Note that for the interval [2, 5], the answer is 15 (2^4 = 4^2 is dupe)
 *
 * NB: If a^b = c^d, for integers a, b, c, d, then c *MAY NOT* (oops) be a power
 * of a, and b a multiple of d, e.g. 4^6 = 8^4. However, a and c must both be
 * expressable as m^n in this case; is it possible to do this without
 * factorization? For reference, in Python:
r = range(2, 101); print(len(set([a**b for a in r for b in r])))
>>> 9183
 *
 * To do this programmatically, we note that we can always write a^b = c^d as:
 * ((m^n)^p)^x = ((m^p)^n)^x
 *
 * This will work as long as
 * NMIN < m^n, m^p, px, nx < NMAX
 * WLOG, p > n, so just "px, * m^p < NMAX" AND "nx > 1" suffices
 * NB2: x > 1 is required, since x = 1 yields double counting
 *
 * This seems difficult to do programmatically: the above double-counts the case
 * 4^12 = 8^8 = 16^6 for m=2. We'd have to carefully do PIE...
 */
#include <cstdio>
#include "util.hpp"
const int NMIN = 2;
// const int NMAX = 101; // exclusive upper bound
const int NMAX = 17;
int main(int argc, const char *argv[])
{
    int num_base = my_pow(NMAX - NMIN, 2); // length of set including dupes
    // do not double count, skip values of m2 = m1^n
    bool to_skip[NMAX];
    for (int i = 0; i < NMAX; i++)
    {
        to_skip[i] = false;
    }

    for (int m = NMIN; m < NMAX; m++)
    {
        if (to_skip[m]) continue;

        for (int n = 1; n < NMAX; n++)
        {
            if (n < my_logfloor(NMAX, m))
            // if (my_pow(m, n) < NMAX)
            {
                to_skip[my_pow(m, n)] = true;
            }

            for (int p = n + 1; p < NMAX; p++)
            {
                if (p > my_logfloor(NMAX, m)) break;
                // if (my_pow(m, p) >= NMAX) break;

                int num_xs = (NMAX - 1) / p - 1;
                if (num_xs > 0)
                {
                    printf("%d, %d, %d, %d\n", m, n, p, num_xs);
                }
                num_base -= num_xs;
            }
        }
    }
    printf("WRONG number of distinct terms is %d\n", num_base);
    return 0;
}
