#include <cmath>
#include <cstdio>
#include <cstring>
#define MAX(a, b) ((a) > (b)) ? (a) : (b)
template <typename T>
T my_pow(T a, int pow)
{
    T ret = 1;
    for (int i = 0; i < pow; i++) { ret *= a; }
    return ret;
}

int my_logfloor(int a, int base)
{
    int ret = 0;
    while (a >= base)
    {
        a /= base;
        ret++;
    }
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

void add_longnum(const int *a, const int *b, int *c, const int n_digits)
{
    int carry = 0;
    int new_val;
    for (int j = 0; j < n_digits; j++)
    {
        new_val = a[j] + b[j] + carry;
        c[j] = new_val % 10;
        carry = new_val / 10;
    }
}
void print_longnum(const int *digits, const int n_digits)
{
    bool to_print = false;
    for (int i = n_digits - 1; i >= 0; i--)
    {
        if (digits[i] > 0) { to_print = true; }
        if (to_print)
        {
            printf("%d", digits[i]);
        }
    }
    if (!to_print)
    {
        printf("0");
    }
    printf("\n");
}
