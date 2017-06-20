#include <string.h>
#include <limits.h>

#include "rk.h"

/* we don't need to worry about overflow logic since overflowing is basically
 * just a 32-bit bitmask */

int BASE = 101;

unsigned int rabin_hash(const char *str, int m)
{
    /* compute the base (BASE) Rabin fingerprint of str with length m */
    unsigned int hash = 0;
    int i;

    for (i = 0; i < m; i++)
    {
        hash *= BASE;
        hash += *(str + i);
    }
    return hash;
}

unsigned int _update_rabin_hash(
    unsigned int hash,
    unsigned int shift,
    char pop,
    char push
)
{
    /*
     * Update the rabin hash by popping the third argument and pushing the
     * fourth on a string of length m
     */
    unsigned int pop_hash = (unsigned int) pop; /* store hash to be popped */
    unsigned int ret_hash = hash;
    int i;

    pop_hash *= shift;
    ret_hash = ret_hash - pop_hash;
    ret_hash *= BASE;
    return ret_hash + push;
}

int match_rk(circular_buffer *text, const char *target)
{
    /*
     * Performs O(n*m) naive matching
     */
    int m = strlen(target);
    int i;
    int text_idx = 0;
    unsigned int shift = 1;

    unsigned int text_hash = rabin_hash(text->buf, m);
    unsigned int target_hash = rabin_hash(target, m);

    /* compute BASE^{m-1} for hash */
    for (i = 1; i < m; i++)
    {
        shift *= BASE;
    }

    while (!text->ended)
    {
        if (text_hash == target_hash)
        {
            /* hash collision, check whether match */
            for (i = 0; i < m; i++)
            {
                if (buf_get(text, text_idx + i) != *(target + i))
                {
                    break;
                }
            }
            if (i == m)
            {
                return 1;
            }
        }
        text_hash = _update_rabin_hash(
            text_hash,
            shift,
            buf_get(text, text_idx),
            buf_get(text, text_idx + m)
        );
        text_idx++;
    }
    return text_hash == target_hash;
}
