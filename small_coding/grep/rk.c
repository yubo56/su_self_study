#include "rk.h"
#include <string.h>
#include <limits.h>

int BASE = 101;
unsigned int OVERFLOW_RISK = UINT_MAX >> 7; /* *101 <= 7 bitshifts */

unsigned int shift_hash(unsigned int hash)
{
    if (hash >= OVERFLOW_RISK)
    {
        /* OVERFLOW_RISK is also a bitmask with top 7 bits zeroed out */
        hash = OVERFLOW_RISK & hash;
    }
    return hash * BASE;
}

unsigned int rabin_hash(char *str, int m)
{
    /* compute the base (BASE) Rabin fingerprint of str with length m */
    unsigned int hash = 0;
    int i;

    for (i = 0; i < m; i++)
    {
        hash = shift_hash(hash);
        hash += *(str + i);
    }
    return hash;
}

unsigned int _update_rabin_hash(unsigned int hash, int m, char pop, char push)
{
    /*
     * Update the rabin hash by popping the third argument and pushing the
     * fourth on a string of length m
     */
    unsigned int pop_hash = (unsigned int) pop; /* store hash to be popped */
    unsigned int ret_hash = hash;
    int i;

    for (i = 1; i < m; i++)
    {
        pop_hash = shift_hash(pop_hash);
    }
    ret_hash = ret_hash - pop_hash;
    ret_hash = shift_hash(ret_hash);
    return ret_hash + push;
}

int match_rk(char *text, char *target)
{
    /*
     * Performs O(n*m) naive matching
     */
    int m = strlen(target);
    int i;

    unsigned int text_hash = rabin_hash(text, m);
    unsigned int target_hash = rabin_hash(target, m);

    while (*(text + m))
    {
        if (text_hash == target_hash)
        {
            for (i = 0; i < m; i++)
            {
                if (*(text + i) != *(target + i))
                {
                    break;
                }
            }
            if (i == m)
            {
                return 1;
            }
        }
        text_hash = _update_rabin_hash(text_hash, m, *text, *(text + m));
        text++;
    }
    return text_hash == target_hash;
}
