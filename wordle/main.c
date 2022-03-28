#include <stdio.h>
#include <string.h>
#define M 5 /* word length */
#define N 2350 /* upper bound on number of possible solutions */
#define N2 10700 /* upper bound on number of non-solution guesses */
#define A 26 /* total number of alphabetic chars */
#define N_GUESSES 1
#define MAX_GUESSES 6
#define FLIPALL -1 /* all bits are 1 */
#define FN "wordle_list.txt"
#define FN2 "wordle_others.txt"

/* constr = [cands for letter 0, 1, 2, 3, 4, MUST_EXISTS] */
void read_words(int *words, int *other_guesses, int *nwords, int *nwords2);
int pop_constraints(
        int *target, int *guesses, int n_guesses,
        int *constr);
void print_word(int *word);
void print_constraints(
        int *guesses, int n_guesses,
        int *constr);
void pop_bitmask(char *word, int *arr, int len);
int check_guess(int *guess, int *constr);
void clear_constr(int *constr);
int count_possible(int *target, int *guesses, int nguesses,
        int *all_words, int n_words, int quiet);
void process_guess(int argc, char *argv[]);
void best_guesses(char *guess, int num_print);
void qsort_scores(int *scores, int *score_idxs, int len);
void check_allwords(void);

int to_int(char a) { return a - 97; }
char to_char(int a) { return a + 97; }

void read_words(int *words, int *other_guesses, int *nwords, int *nwords2)
{
    /* (M + 1) chars to give each string a null-terminator */
    FILE *fptr;
    char word[M + 1];
    int idx = 0;
    *nwords = 0;
    *nwords2 = 0;

    /* read in all words */
    fptr = fopen(FN, "r");
    for (; !feof(fptr); (*nwords)++)
    {
        fscanf(fptr, "%s", word);
        pop_bitmask(word, words + idx, M);
        idx += M;
    }
    (*nwords)--; /* fix off-by-one error, last word is empty */

    idx = 0;
    fptr = fopen(FN2, "r");
    for (; !feof(fptr); (*nwords2)++)
    {
        fscanf(fptr, "%s", word);
        pop_bitmask(word, other_guesses + idx, M);
        idx += M;
    }
    (*nwords2)--; /* fix off-by-one error, last word is empty */
}
/**
 * Given a target word & guesses, populates all constraints
 *
 * target: 5-letter word
 * guesses: list of 5-letter guesses (null-terminated)
 * n_guesses: number of guesses so far
 * corrects: 5-letter word where a letter in a position means it is guessed
 *  correctly, otherwise \0's
 * contains: 5-letter word where each letter is a misplaced character (in the
 *  wrong place in the guesses)
 *
 * NB: assumes misplaced's is memset to zero!
 */
/* returns # exact matches */
int pop_constraints(int *target, int *guesses, int n_guesses, int *constr)
{
    int i, j, k, l, found, num_matches;
    int *guess;

    num_matches = 0;
    for (i = 0; i < n_guesses; i++) /* loop over guesses */
    {
        guess = guesses + i * M;
        for (j = 0; j < M; j++) /* loop over guess chars */
        {
            /* first, check for exact matches */
            if ((guess[j] & target[j]) != 0)
            {
                constr[j] = target[j];
                num_matches++;
                continue;
            }

            found = 0;
            for (k = 0; k < M; k++) /* loop over target chars */
            {
                if (j == k)
                {
                    continue;
                }
                if ((guess[j] & target[k]) != 0)
                {
                    found++;
                    /* found elsewhere, zero out the current idx + add to MUSTs */
                    constr[j] = constr[j] & (guess[j] ^ FLIPALL);
                    constr[M] = constr[M] | guess[j];

                    break;
                }
            }
            if (found == 0)
            {
                /* zero out every index in constr for guess[j]*/
                for (l = 0; l < M; l++)
                {
                    constr[l] = constr[l] & (guess[j] ^ FLIPALL);
                }
            }
        }
    }
    return num_matches;
}
/* prints the constraints populated by pop_constraints */
void print_word(int *word)
{
    int j, k;
    for (j = 0; j < M; j++)
    {
        for (k = 0; k < A; k++)
        {
            if (word[j] & (1 << k))
            {
                printf("%c", to_char(k));
                break;
            }
        }
    }
}
void print_constraints(
        int *guesses, int n_guesses,
        int *constr)
{
    int i, j;
    /* print cands per letter */
    for (i = 0; i < M; i++)
    {
        printf("%d: ", i);
        for (j = 0; j < A; j++)
        {
            if (constr[i] & (1 << j))
            {
                printf("%c", to_char(j));
            }
            else
            {
                printf("_");
            }
        }
        printf("\n");
    }
    /* print MUSTs */
    printf("MUST: ");
    for (i = 0; i < A; i++)
    {
        if (constr[M] & (1 << i))
        {
            printf("%c", to_char(i));
        }
    }
    printf("\n");
    for (i = 0; i < n_guesses; i++)
    {
        printf("- ");
        print_word(guesses + i * M);
        printf("\n");
    }
    return;
}
void pop_bitmask(char *word, int *arr, int len)
{
    int i;
    for (i = 0; i < len; i++)
    {
        arr[i] = (1 << to_int(word[i]));
    }
}
/* check whether a guess satisfies the constraints returned by pop_constraints
 * */
int check_guess(int *guess, int *constr)
{
    int i;
    int musts = constr[M];
    for (i = 0; i < M; i++)
    {
        /* check that each individual char is permitted */
        if ((constr[i] & guess[i]) == 0)
        {
            return 0;
        }
        /* turn off bits in musts for each included char */
        musts = musts & (FLIPALL ^ guess[i]);
    }
    return musts == 0;
}
void clear_constr(int *constr)
{
    int i;
    for (i = 0; i < M; i++)
    {
        constr[i] = FLIPALL;
    }
    constr[M] = 0;
}

int count_possible(int *target, int *guesses, int nguesses,
        int *all_words, int n_words, int quiet)
{
    int constr[M + 1];
    int n_cands;
    int i, is_guess;
    clear_constr(constr);

    n_cands = 0;
    if (pop_constraints(target, guesses, nguesses, constr) == M)
    {
        if (!quiet)
        {
            printf("Exact Match\n");
        }
        return 0;
    }
    if (!quiet)
    {
        print_constraints(guesses, nguesses, constr);
        printf("Cands are:\n");
    }
    for (i = 0; i < n_words; i++)
    {
        is_guess = check_guess(all_words + i * M, constr);
        n_cands += is_guess;
        if (is_guess && !quiet)
        {
            printf("* ");
            print_word(all_words + i * M);
            printf("\n");
        }
    }
    if (!quiet)
    {
        printf("Total %d possible\n", n_cands);
    }
    return n_cands;
}
void process_guess(int argc, char *argv[])
{
    int target[M];
    char guesses_str[MAX_GUESSES * M];
    int guesses[MAX_GUESSES * M];
    int all_words[M * N];
    int other_guesses[M * N2];
    int n_guesses = argc - 2;
    int i, n_words, n_words2;

    for (i = 0; i < n_guesses; i++)
    {
        memcpy(guesses_str + i * M, argv[i + 2], M);
    }

    read_words(all_words, other_guesses, &n_words, &n_words2);

    pop_bitmask(argv[1], target, M);
    pop_bitmask(guesses_str, guesses, n_guesses * M);
    count_possible(target, guesses, 2, all_words, n_words, 0);
}

void best_guesses(char *target_str, int num_print)
{
    int target[M];
    int all_words[M * N];
    int other_guesses[M * N2];
    int i, n_words, n_words2;
    int scores[N + N2], score_idxs[N + N2];

    pop_bitmask(target_str, target, M);
    read_words(all_words, other_guesses, &n_words, &n_words2);

    for (i = 0; i < n_words; i++)
    {
        scores[i] = count_possible(
                target, all_words + (i * M), 1,
                all_words, n_words, 1);
    }
    for (i = 0; i < n_words2; i++)
    {
        scores[n_words + i] = count_possible(
                target, other_guesses + (i * M), 1,
                all_words, n_words, 1);
    }

    /* argsort scores */
    for (i = 0; i < n_words + n_words2; i++)
    {
        score_idxs[i] = i;
    }
    qsort_scores(scores, score_idxs, n_words + n_words2);
    for (i = 0; i < num_print; i++)
    {
        printf("%d remaining when guessing ", scores[score_idxs[i]]);
        if (score_idxs[i] < n_words)
        {
            print_word(all_words + (score_idxs[i] * M));
        }
        else
        {
            print_word(other_guesses + ((score_idxs[i] - n_words) * M));
        }
        printf("\n");
    }
}
void qsort_scores(int *scores, int *score_idxs, int len)
{
    int i, end, swap;
    int pivot = scores[score_idxs[0]];

    if (len <= 1)
    {
        return;
    }

    end = len - 1;
    for (i = 1; i <= end;)
    {
        /* if less than, swap with immediate left (pivot) */
        if (scores[score_idxs[i]] < pivot)
        {
            swap = score_idxs[i];
            score_idxs[i] = score_idxs[i - 1];
            score_idxs[i - 1] = swap;
            i++;
        }
        /* else swap with end */
        else
        {
            swap = score_idxs[end];
            score_idxs[end] = score_idxs[i];
            score_idxs[i] = swap;
            end--;
        }
    }
    /* pivot is at i - 1 */
    qsort_scores(scores, score_idxs, i - 1);
    qsort_scores(scores, score_idxs + i,
            len - i);
}

/* prints list of (N, w), where N words remain when guessing w on average */
void check_allwords(void)
{
    int all_words[M * N];
    int other_guesses[M * N2];
    int n_words, n_words2;
    int i, j, score;
    int score_idxs[N + N2], scores[N + N2];

    read_words(all_words, other_guesses, &n_words, &n_words2);
    for (i = 0; i < n_words; i++)
    {
        score = 0;
        /* total number of remaining words if target = j and guess = i */
        for (j = 0; j < n_words; j++)
        {
            score += count_possible(
                    all_words + (j * M), all_words + (i * M), 1,
                    all_words, n_words, 1);
        }
        scores[i] = score;
    }
    for (i = 0; i < n_words2; i++)
    {
        score = 0;
        /* total number of remaining words if target = j and guess = i */
        for (j = 0; j < n_words; j++)
        {
            score += count_possible(
                    all_words + (j * M), other_guesses + (i * M), 1,
                    all_words, n_words, 1);
        }
        scores[n_words + i] = score;
    }

    /* argsort scores */
    for (i = 0; i < n_words + n_words2; i++)
    {
        score_idxs[i] = i;
    }
    qsort_scores(scores, score_idxs, n_words + n_words2);

    for (i = 0; i < n_words + n_words2; i++)
    {
        printf("%.7f ", scores[score_idxs[i]] * 1.0 / n_words);
        if (score_idxs[i] < n_words)
        {
            print_word(all_words + (score_idxs[i] * M));
        }
        else
        {
            print_word(other_guesses + ((score_idxs[i] - n_words) * M));
        }
        printf("\n");
    }
}
int main(int argc, char *argv[])
{
    if (argc > 2)
    {
        process_guess(argc, argv);
    }
    else if (argc == 2)
    {
        best_guesses(argv[1], 30);
    }
    else
    {
        check_allwords();
    }

    return 0;
}
