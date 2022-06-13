/**
 * How many different ways to sum {1, 2, 5, 10, 20, 50, 100, 200} to 200? Actual
 * problem phrases using coins.
 *
 * We use dypo. Caveat: cannot just do sums[i + coin] = sums[i] + 1, since
 * ordering does not matter: 1+2 = 2+1 is the same
 *
 * Instead, build solution out by hand & iteratively: start with the number of
 * ways to do it with 1's only, then include number of 2s, etc. See code.
 */
#include <cstdio>
const int N = 200;
int main(int argc, const char *argv[])
{
    int num_sums[N + 1];
    int summands[] = {1, 2, 5, 10, 20, 50, 100, 200};
    int n_summands = 8;

    // initialize
    num_sums[0] = 1;
    for (int i = 1; i < N + 1; i++)
    {
        num_sums[i] = 0;
    }

    // recursive step: ordering of the loop is such that we only consider adding
    // coins of a single type at a time. This effectively sorts the summation &
    // avoids double counting
    for (int j = 0; j < n_summands; j++)
    {
        for (int i = 0; i < N + 1; i++)
        {
            if (i + summands[j] >= N + 1) continue;
            num_sums[i + summands[j]] += num_sums[i];
        }
    }

    printf("%d ways to sum to %d\n", num_sums[N], N);
    return 0;
}
