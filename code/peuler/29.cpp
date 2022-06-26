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
 * We just do this by counting prime factors, which is ugly but works. No more
 * optimization since I'm sick of this problem lol
 */
#include <cstdio>
#include "util.hpp"
#define IDX(i, j, k, n) (i * n * n + j * n + k)
const int NMIN = 2;
// const int NMAX = 101; // exclusive upper bound

int get_distinct(int nmax)
{
    /* fact_list[i][j][k] is how many powers of k are in prime factorization of
     * i^j. NB: fact_list[:, 0, :] is unused, and composite k are also unused */
    int* fact_list = new int[nmax * nmax * nmax];
    for (int i = 0; i < nmax * nmax * nmax; i++)
    {
        fact_list[i] = 0;
    }
    /* populate fact_list[:,1,:] first, i.e. prime factorizations */
    for (int i = NMIN; i < nmax; i++)
    {
        int target = i;
        for (int factor = NMIN; factor <= i; factor++)
        {
            while (target % factor == 0)
            {
                fact_list[IDX(i, 1, factor, nmax)]++;
                target /= factor;
            }
            if (target == 1)
            {
                break;
            }
        }
    }
    /* populate powers */
    for (int i = NMIN; i < nmax; i++)
    {
        for (int j = NMIN; j < nmax; j++)
        {
            for (int k = NMIN; k < nmax; k++)
            {
                fact_list[IDX(i, j, k, nmax)] =
                    j * fact_list[IDX(i, 1, k, nmax)];
            }
        }
    }
    /* compare element-by-element & keep only uniques */
    int* dupe_counts = new int[nmax * nmax];
    for (int i = 0; i < nmax * nmax; i++)
    {
        dupe_counts[i] = 0;
    }
    for (int i1 = NMIN; i1 < nmax; i1++)
    {
        for (int i2 = NMIN; i2 < nmax; i2++)
        {
            int n_dupes = 0;
            for (int j1 = NMIN; j1 < nmax; j1++)
            {
                for (int j2 = NMIN; j2 < nmax; j2++)
                {
                    if (i1 == j1 && i2 == j2) continue;

                    bool is_same = true;
                    for (int k = NMIN; k < nmax; k++)
                    {
                        if (fact_list[IDX(i1, i2, k, nmax)]
                                != fact_list[IDX(j1, j2, k, nmax)])
                        {
                            is_same = false;
                            break;
                        }
                    }
                    if (is_same)
                    {
                        n_dupes++;
                    }
                }
            }
            dupe_counts[n_dupes + 1]++; // self is a single factorization
        }
    }
    int base_count = (nmax - NMIN) * (nmax - NMIN);
    // if there are N factorizations that repeat M times, count them as N /
    // M-many unique ones, or "subtract N and add N / M"
    for (int i = NMIN; i < nmax * nmax; i++)
    {
        if (dupe_counts[i] > 0)
        {
            base_count -= dupe_counts[i] - (dupe_counts[i] / i);
        }
    }
    delete[] fact_list;
    delete[] dupe_counts;
    return base_count;
}
void verify()
{
    /*
for nmax in range(6, 21): r = range(2, nmax); print(len(set([a**b for a in r for b in r])))
15
23
34
44
54
69
88
106
129
152
177
195
226
256
291
     */
    int num_distinct;
    for (int i = 6; i < 21; i++)
    {
        num_distinct = get_distinct(i);
        printf("(%d) Number of distinct terms is %d\n", i, num_distinct);
    }
    return;
}
int main(int argc, const char *argv[])
{
    // verify();
    int N = 101;
    printf("(%d) Number of distinct terms is %d\n", N, get_distinct(N));
    return 0;
}
