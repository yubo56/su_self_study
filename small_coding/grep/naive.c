#include "naive.h"

int match_naive(char *text, char *target)
{
    /*
     * Performs O(n*m) naive matching
     */
    int target_idx = 0;
    char *text_char = text;

    while (*text_char)
    {
        for(target_idx = 0; *(target + target_idx); target_idx++)
        {
            if (*(text_char + target_idx) != *(target + target_idx))
            {
                break;
            }
        }
        if (*(target + target_idx) == 0) /* end of target */
        {
            return 1;
        }
        else if (*(text + target_idx) == 0) /* end of text */
        {
            return 0;
        }
        else /* more text to check */
        {
            text_char++;
        }
    }
    return 0;
}
