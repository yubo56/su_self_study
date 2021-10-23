#include <cmath>
#include <cstdio>
#include <cstring>
template <typename T>
T my_pow(T a, int pow)
{
    T ret = 1;
    for (int i = 0; i < pow; i++) { ret *= a; }
    return ret;
}

// take root-th order root (e.g. root=2 => sqrt), for fun
const double ROOT_TOL = 1e-15;
template <typename T>
double my_root(T a, int root, double guess)
{
    double left, right, guess_new;
    // bracket the root first
    if (my_pow(guess, root) > a)
    {
        right = guess;
        left = guess / 2;
        while (my_pow(left, root) > a)
        {
            right = left;
            left /= 2;
        }
    }
    else
    {
        right = guess * 2;
        left = guess;
        while (my_pow(right, root) < a)
        {
            left = right;
            right *= 2;
        }
    }

    // binary search seems most performant...
    while ((right - left) > ROOT_TOL)
    {
        guess_new = (right + left) / 2;
        if (my_pow(guess_new, root) > a)
        {
            right = guess_new;
        }
        else
        {
            left = guess_new;
        }
    }
    return (left + right) / 2;
}

// get primes up through N + 2 (is_prime[i] = whether (i + 2) is prime)
void populate_primes(bool *is_prime, int N)
{
    memset(is_prime, 1, N * sizeof(bool));
    for (int i = 0; i < N; i++)
    {
        if (is_prime[i])
        {
            for (int j = 2; (i + 2) * j - 2 < N; j++)
            {
                is_prime[(i + 2) * j - 2] = false;
            }
        }
    }
}
