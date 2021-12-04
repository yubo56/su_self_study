/**
 * Find the product of the thirteen adjacent digits in the given number that
 * have the greatest product
 *
 * LMAO problem was that it overflowed "int" and I had to use "long"
 */
#define MAX(a, b) ((a) > (b)) ? (a) : (b)
#include <cstdio>
#include <cstring>
// unsigned int N = 4; // 5832
unsigned int N = 13;
char str[] = "73167176531330624919225119674426574742355349194934"
    "96983520312774506326239578318016984801869478851843"
    "85861560789112949495459501737958331952853208805511"
    "12540698747158523863050715693290963295227443043557"
    "66896648950445244523161731856403098711121722383113"
    "62229893423380308135336276614282806444486645238749"
    "30358907296290491560440772390713810515859307960866"
    "70172427121883998797908792274921901699720888093776"
    "65727333001053367881220235421809751254540594752243"
    "52584907711670556013604839586446706324415722155397"
    "53697817977846174064955149290862569321978468622482"
    "83972241375657056057490261407972968652414535100474"
    "82166370484403199890008895243450658541227588666881"
    "16427171479924442928230863465674813919123162824586"
    "17866458359124566529476545682848912883142607690042"
    "24219022671055626321111109370544217506941658960408"
    "07198403850962455444362981230987879927244284909188"
    "84580156166097919133875499200524063689912560717606"
    "05886116467109405077541002256983155200055935729725"
    "71636269561882670428252483600823257530420752963450";
int to_int(char a) { return ((int) a) - 48; }
void sol_rolling()
{
    long prod = 1;
    for (unsigned int i = 0; i < N; i++)
    {
        prod *= to_int(str[i]);
    }
    long max_prod = prod;
    for (unsigned int i = N; i < strlen(str); i++)
    {
        if (to_int(str[i]) == 0)
        {
            prod = 1;
            for (unsigned int j = 1; j <= N && (i + j) < strlen(str); j++)
            {
                if (to_int(str[i + j]) == 0)
                {
                   i += j;
                   j = 0;
                   prod = 1;
                   continue;
                }
                prod *= to_int(str[i + j]);
            }
            i += N;
        }
        else
        {
            prod /= to_int(str[i - N]);
            prod *= to_int(str[i]);
        }
        max_prod = MAX(max_prod, prod);
    }
    printf("Max product of %d digits is %ld\n", N, max_prod);
}
void sol_dumb()
{
    long max_prod = 1;
    for (unsigned int i = 0; i < strlen(str) - N; i++)
    {
        long prod = 1;
        for (unsigned int j = 0; j < N; j++)
        {
            prod *= to_int(str[i + j]);
        }
        max_prod = MAX(max_prod, prod);
    }
    printf("Max product of %d digits is %ld\n", N, max_prod);
}
int main(int argc, const char *argv[])
{
    sol_rolling();
    sol_dumb();
    return 0;
}
