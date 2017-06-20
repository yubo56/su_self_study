#include <assert.h> /* for some domain checking */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kmp.h"

/* printable characters between 0x20 and 0x7E */
#define PRINT_MAX 127 /* exclusive */
#define PRINT_MIN 32 /* inclusive */
int PRINT_RANGE = PRINT_MAX - PRINT_MIN;

int is_legal(char ch)
{
    return ch >= PRINT_MIN && ch < PRINT_MAX;
}
int table_idx(char ch)
{
    /* compute what index to use for a given character (32-127 range) */
    assert(is_legal(ch)); /* in case illegal chars */
    return ch - PRINT_MIN; /* guaranteed >= 0, < PRINT_RANGE */
}

int get_idx(int i, char j)
{
    /* utility function for 2D array as 1D array */
    return i * PRINT_RANGE + table_idx(j);
}

int *kmp_jump_table(const char *target, int m)
{
    /**
     * precompute kmp jump table. This is a map saying "if I'm at index i of
     * target and reading character c from text, what is the next index of
     * target I look at?"
     *
     * more precisely for each substr_len prefix of target, does a prefix of
     * length prefix_len - 1 match a suffix of (prefix_len - 1)? If so, compute
     * the next index of target to compare
     *
     *      jumps[current index of substr (substr_len)]
     *          [next character (target[prefix_len]] =
     *          next index to examine (prefix_len + 1)
     *
     * Return: user-free (strlen(target) * PRINT_RANGE * sizeof(int))
     *      containing jump table
     */
    /* we use calloc to zero the memory */
    int *jumps = (int *) calloc(m * PRINT_RANGE, sizeof(int));
    int substr_len, prefix_len, i;

    for (substr_len = 0; substr_len < m; substr_len++)
    {
        for (prefix_len = 1; prefix_len < substr_len; prefix_len++)
        {
            /* compare prefix_len - 1 prefix and suffix of substr_len */
            for (i = 0; i < prefix_len; i++)
            {
                if (*(target + i) !=
                    *(target + substr_len - (prefix_len - 1) + i))
                {
                    break;
                }
            }
            if (i == prefix_len - 1)
            {
                /* match */
                jumps[get_idx(substr_len, target[prefix_len])] = prefix_len + 1;
            }
        }
        /* special case since we don't need string comparison here */
        jumps[get_idx(substr_len, target[substr_len])] = substr_len + 1;
    }
    return jumps;
}

int match_kmp(circular_buffer *text, const char *target) {
    int m = strlen(target);
    int *jumps = kmp_jump_table(target, m);
    int target_idx = 0;
    int text_idx = 0;
    char text_char = 0;

    while ((text_char = buf_get(text, text_idx)) != -1)
    {
        if (target_idx >= m) /* match condition */
        {
            return 1;
        }

        if (!is_legal(text_char))
        {
            target_idx = 0;
        }
        else
        {
            target_idx = jumps[get_idx(target_idx, text_char)];
        }
        text_idx++;
    }

    free(jumps);
    return target_idx >= m;
}
