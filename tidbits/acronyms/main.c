/**
 * TODO:
 * - sort the outputs in some way (scoring? like the github project)
 * - don't allow "acronyms" to take all chars from one word
 */
#include <stdio.h>
#include <string.h>
#define N 100000 /* max number of words in list */
#define L 30 /* number of chars in each word, approximately */
#define M 1000 /* len of input string */
#define LEN (N * L)

/* const char *DICT_FN = "wordlist.txt"; */
const char *DICT_FN = "wordlist_short.txt";

void strtolower(char *str)
{
    int idx = 0;
    while (str[idx] != 0)
    {
        if (str[idx] < 91 && str[idx] > 64)
        {
            str[idx] += 32;
        }
        idx++;
    }
}
void strtoupper(char *str)
{
    int idx = 0;
    while (str[idx] != 0)
    {
        if (str[idx] > 96 && str[idx] < 123)
        {
            str[idx] -= 32;
        }
        idx++;
    }
}

int populate_flags(char *words, int *word_idxs, int nwords,
        char *inphrase, unsigned char *flags, char mode)
{
    unsigned char offsets[N];
    int phrase_len = strlen(inphrase);
    int i, j;
    int num_found = 0;
    char is_start;

    memset(offsets, 0, N);

    /* for char in phrase */
    is_start = 1; /* start of phrase is always start of new word */
    for (i = 0; i < phrase_len; i++)
    {
        /* loop over each word */
        for (j = 0; j < nwords; j++)
        {
            /* skip known found words */
            if (flags[j]) { continue; }

            /* seek word */
            if (inphrase[i] == words[word_idxs[j] + offsets[j]])
            {
                if (mode == 0)
                {
                    offsets[j]++;
                }
                else if (mode == 1 && is_start == 1)
                {
                    offsets[j]++;
                }
                else if (mode == 2 && is_start == 0)
                {
                    offsets[j]++;
                }
            }

            /* mark words as found if offset has reached end of word */
            if (words[word_idxs[j] + offsets[j]] == 0)
            {
                flags[j] = 1;
                num_found++;
            }
        }
        /* update is_start status:
         * if current char is start of word, next cannot be
         * if current char is not start of word & is space, next is */
        if (is_start == 1)
        {
            is_start = 0;
        }
        else if (inphrase[i] == ' ')
        {
            is_start = 1;
        }
    }
    return num_found;
}
void print_word(char *words, int *word_idxs, char *inphrase, int idx, char mode)
{
    char upperword[L];
    int curridx = 0;
    int i;
    int phrase_len = strlen(inphrase);
    char is_start = 1;

    memcpy(upperword, words + word_idxs[idx], L * sizeof(char));
    strtoupper(upperword);
    printf("[%s] ", upperword);

    for (i = 0; i < phrase_len; i++)
    {
        if (inphrase[i] == words[word_idxs[idx] + curridx] && (
                    (mode == 0) ||
                    (mode == 1 && is_start == 1) ||
                    (mode == 2 && is_start == 0)
        ))
        {
            printf("%c", upperword[curridx]);
            curridx++;
        }
        else
        {
            printf("%c", inphrase[i]);
        }

        if (is_start == 1)
        {
            is_start = 0;
        }
        else if (inphrase[i] == ' ')
        {
            is_start = 1;
        }
    }
    printf("\n");
}

void run(char mode)
{
    FILE *fptr;
    char words[LEN];
    int word_idxs[N];
    int nwords = 0;
    int currlen = 0;
    int len = 0;

    char inphrase[M];

    int i;

    int num_found;
    unsigned char flags[N]; /* whether a word is found */

    fptr = fopen(DICT_FN, "r");
    word_idxs[nwords] = len;
    fscanf(fptr, "%s%n", words + len, &currlen);
    len += currlen + 1;
    nwords++;
    while (!feof(fptr))
    {
        word_idxs[nwords] = len;
        fscanf(fptr, "%s%n", words + len, &currlen);
        strtolower(words + len);
        len += currlen + 1;
        nwords++;
    }
    nwords--; /* is now number of words read */
/* DEBUG: print whole dict
    for (i = 0; i < nwords; i++)
    {
        printf("(%d) %s\n", i, words + word_idxs[i]);
    }
*/
    while (1)
    {
        /* gets inphrase with max len M */
        /* fgets has max len & only breaks on whitespace */
        printf("Type your phrase:\n\t> ");
        fgets(inphrase, M, stdin);
        if (strlen(inphrase) <= 1)
        {
            printf("Empty input, exiting\n");
            break;
        }
        /* remove trailing newline which is always there? */
        inphrase[strlen(inphrase) - 1] = '\0';
        strtolower(inphrase);
        printf("Seeking words in: %s\n", inphrase);

        memset(flags, 0, N);
        num_found = populate_flags(words, word_idxs, nwords, inphrase, flags,
                mode);
        printf("Found %d matches!\n", num_found);
        for (i = 0; i < nwords; i++)
        {
            if (flags[i])
            {
                print_word(words, word_idxs, inphrase, i, mode);
            }
        }
        printf("\n");
    }
}

void printusage(const char *name)
{
    printf("Usage: %s [-h] [--any] [--starts] [--nostarts]\n"
            "\t-h: prints this message\n"
            "\t--any: matches any chars in target phrase\n"
            "\t--starts: matches only starts of words in target phrase\n"
            "\t--nostarts: matches only non-starts of words in target phrase\n"
            "NB: First requested mode takes precedence.\n", name);
}

int main(int argc, const char *argv[])
{
    int i;
    char mode = -1;

    for (i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "-h"))
        {
            printusage(argv[0]);
            return 0;
        }
        if (!strcmp(argv[i], "--any") && mode == -1)
        {
            mode = 0;
        }
        else if (!strcmp(argv[i], "--starts") && mode == -1)
        {
            mode = 1;
        }
        else if (!strcmp(argv[i], "--nostarts") && mode == -1)
        {
            mode = 2;
        }
        else
        {
            printf("Unknown option '%s', exiting.\n", argv[i]);
            printusage(argv[0]);
            return 0;
        }
    }
    if (mode == -1)
    {
        printf("No mode specified, defaulting to --any\n");
        mode = 0;
    }
    run(mode);
    return 0;
}
