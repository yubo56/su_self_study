/**
 * What is the 10001st prime number?
 */
#include <cstdio>
#include <cmath>
int TARGET_N = 10001;
// should only need TOTAL * ln(TOTAL) = TARGET, but oh well
int TOTAL_N = ((int) pow(TARGET_N, 1.5));
int main(int argc, const char *argv[])
{
    bool is_prime[TOTAL_N]; // whether i - 2 is prime
    for (int i = 0; i < TOTAL_N - 2; i++)
    {
        is_prime[i] = true;
    }
    for (int i = 0; i < TOTAL_N - 2; i++)
    {
        if (!is_prime[i]) { continue; }
        for (int j = 2; j * (i + 2) < TOTAL_N; j++)
        {
            is_prime[j * (i + 2) - 2] = false;
        }
    }
    int count = 0;
    int idx = 0;
    while (count < TARGET_N)
    {
        count += is_prime[idx++];
    }
    printf("%d-th prime is %d\n", TARGET_N, idx + 1);
    return 0;
}
