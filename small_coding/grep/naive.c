#include "naive.h"

int match_naive(circular_buffer *text, const char *target)
{
    /*
     * Performs O(n*m) naive matching
     */
    int target_idx = 0;
    int text_idx = 0;;

    while (!text->ended)
    {
        for(target_idx = 0; *(target + target_idx); target_idx++)
        {
            if (buf_get(text, text_idx + target_idx) != *(target + target_idx))
            {
                break;
            }
        }
        if (*(target + target_idx) == 0) /* end of target */
        {
            return 1;
        }
        else /* more text to check */
        {
            text_idx++;
        }
    }
    return 0;
}
