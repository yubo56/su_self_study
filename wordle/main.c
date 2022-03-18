/* gcc -Ofast main.c -o main && ./main > main.out */
#include <stdio.h>
#include <string.h>
#define M 5 /* word length */
#define N 3000 /* upper bound on number of words */
#define A 26 /* total number of alphabetic chars */
#define N_GUESSES 1
void pop_constraints(
        char *target, char *guesses, int n_guesses,
        char *corrects, char *contains, char *missings);
void print_constraints(
        char *guesses, int n_guesses,
        char *corrects, char *contains, char *missings);
int check_guess(char *guess, char *corrects, char *contains,
        char *missings);
void run(void);

int to_int(char a) { return a - 97; }
char to_char(int a) { return a + 97; }
void run()
{
    /* (M + 1) chars to give each string a null-terminator */
    char words[N * (M + 1)];
    FILE *fptr;
    const char *fn = "wordle_list.txt";
    int i,
        n_words, target_idx, word1, guess_idx,
        best_word, best_cands, num_cands;

    char *target;
    char *guess;
    char corrects[M + 1];
    char contains[A + 1];
    char missings[A + 1];
    char guesses[N_GUESSES * (M + 1)];
    /* for each potential first guess, store the number of candidates it permits
     * on the next guess */
    int n_cands[N];
    /* for each potential word, store the number of candidates remaining after
     * each potential first guess */
    int n_targetcands[N];

    memset(words, 0, sizeof(words));
    memset(n_cands, 0, sizeof(n_cands));
    memset(n_targetcands, 0, sizeof(n_targetcands));

    /* read in all words */
    fptr = fopen(fn, "r");
    for (n_words = 0; !feof(fptr); n_words++)
    {
        fscanf(fptr, "%s", words + (n_words * (M + 1)));
    }
    n_words--; /* fix off-by-one error, last word is empty */

    for (target_idx = 0; target_idx < n_words; target_idx++)
    {
        target = words + (target_idx * (M + 1));
        best_word = -1; /* find the best first guess for the target */
        best_cands = -1; /* number of candidates permitted by first guess */
        for (word1 = 0; word1 < n_words; word1++)
        {
            if (word1 == target_idx)
            {
                continue;
            }
            /* clear constraints, set up guesses for word1, word2... */
            memset(corrects, 0, sizeof(corrects));
            memset(contains, 0, sizeof(contains));
            memset(missings, 0, sizeof(missings));
            memcpy(guesses, words + (word1 * (M + 1)), M * sizeof(char));
            guesses[M] = 0;

            pop_constraints(target, guesses, N_GUESSES,
                    corrects, contains, missings);

            /* count how many words in dict are compatible w/ constrs */
            num_cands = 0;
            for (guess_idx = 0; guess_idx < n_words; guess_idx++)
            {
                if (guess_idx == word1)
                {
                    continue;
                }
                guess = words + (guess_idx * (M + 1));
                num_cands += check_guess(guess, corrects, contains, missings);
            }
            n_cands[word1] += num_cands;
            n_targetcands[target_idx] += num_cands;
            if (best_cands < 0 || num_cands < best_cands)
            {
                best_word = word1;
                best_cands = num_cands;
            }
        }
        printf("Best word for %s is %s with %d cands\n",
                target, words + (best_word * (M + 1)), best_cands);
    }
    /* search for best average word */
    best_word = -1;
    best_cands = -1;
    for (i = 0; i < n_words; i++)
    {
        printf("%f, %s\n", n_cands[i] * 1.0 / n_words,
                words + (i * (M + 1)));
        if (best_cands < 0 || n_cands[i] < best_cands)
        {
            best_word = i;
            best_cands = n_cands[i];
        }
    }
    printf("Best word is %s with %f average cands\n",
            words + (best_word * (M + 1)), best_cands * 1.0 / n_words);
    /* search for hardest word */
    best_word = -1;
    best_cands = -1;
    for (i = 0; i < n_words; i++)
    {
        printf("%f, %s\n", n_targetcands[i] * 1.0 / n_words,
                words + (i * (M + 1)));
        if (best_cands < 0 || n_targetcands[i] > best_cands)
        {
            best_word = i;
            best_cands = n_cands[i];
        }
    }
    printf("Hardest word is %s with %f average remaining cands\n",
            words + (best_word * (M + 1)), best_cands * 1.0 / n_words);
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
void pop_constraints(char *target, char *guesses, int n_guesses,
        char *corrects, char *contains, char *missings)
{
    int i, j, k, found;
    char *guess;
    for (i = 0; i < n_guesses; i++)
    {
        guess = guesses + i * (M + 1);
        for (j = 0; j < M; j++) /* loop over guess chars */
        {
            found = 0;
            for (k = 0; k < M; k++)
            {
                if (guess[j] == target[k])
                {
                    found++;
                    if (k == j) /* exact match */
                    {
                        corrects[j] = guess[j];
                    }
                    contains[to_int(guess[j])] = 1;
                    break;
                }
            }
            if (found == 0)
            {
                missings[to_int(guess[j])] = 1;
            }
        }
    }
}
/* prints the constraints populated by pop_constraints */
void print_constraints(
        char *guesses, int n_guesses,
        char *corrects, char *contains, char *missings)
{
    int i;
    printf("\"");
    for (i = 0; i < M; i++)
    {
        if (corrects[i] > 0)
        {
            printf("%c", corrects[i]);
        }
        else
        {
            printf("_");
        }
    }
    printf("\" - (");
    for (i = 0; i < A; i++)
    {
        if (contains[i] > 0)
        {
            printf("%c", to_char(i));
        }
    }
    printf(", ");
    for (i = 0; i < A; i++)
    {
        if (missings[i] > 0)
        {
            printf("%c", to_char(i));
        }
    }
    printf(")\n");
    for (i = 0; i < n_guesses; i++)
    {
        printf("- %s\n", guesses + i * (M + 1));
    }
    return;
}
/* check whether a guess satisfies the constraints returned by pop_constraints
 * */
int check_guess(char *guess, char *corrects, char *contains,
        char *missings)
{
    int i, j, found;
    /* check known positions first */
    for (i = 0; i < M; i++)
    {
        if (corrects[i] != 0 && corrects[i] != guess[i])
        {
            return 0;
        }
    }
    /* second, check whether all contains's are in guess */
    for (i = 0; i < A; i++)
    {
        found = 0;
        if (contains[i] == 0)
        {
            continue;
        }
        for (j = 0; j < M; j++)
        {
            if (guess[j] == to_char(i))
            {
                found++;
                break;
            }
        }
        if (found == 0)
        {
            return 0;
        }
    }

    /* third, check whether all missings are NOT in guess */
    for (i = 0; i < A; i++)
    {
        found = 0;
        if (missings[i] == 0)
        {
            continue;
        }
        for (j = 0; j < M; j++)
        {
            if (guess[j] == to_char(i))
            {
                found++;
                break;
            }
        }
        if (found == 1)
        {
            return 0;
        }
    }
    return 1;
}
int test_word2(char *target, char *guesses)
{
    char words[N * (M + 1)];
    FILE *fptr;
    const char *fn = "wordle_list.txt";
    char corrects[M + 1];
    char contains[A + 1];
    char missings[A + 1];
    int i, is_guess, n_words, n_cands;
    char *guess;

    memset(words, 0, sizeof(words));
    memset(corrects, 0, sizeof(corrects));
    memset(contains, 0, sizeof(contains));
    memset(missings, 0, sizeof(missings));

    /* read in all words */
    fptr = fopen(fn, "r");
    for (n_words = 0; !feof(fptr); n_words++)
    {
        fscanf(fptr, "%s", words + (n_words * (M + 1)));
    }
    n_words--;

    n_cands = 0;
    pop_constraints(target, guesses, 1, corrects, contains, missings);
    for (i = 0; i < n_words; i++)
    {
        guess = words + (i * (M + 1));
        is_guess = check_guess(guess, corrects, contains, missings);
        n_cands += is_guess;
        if (is_guess)
        {
            printf("%s\n", guess);
        }
    }
    printf("Guessing %s with (%s) leaves %d guesses\n",
            target, guesses, n_cands);
    return n_cands;
}
int test_word3(char *target, char *word1, char *word2)
{
    char words[N * (M + 1)];
    FILE *fptr;
    const char *fn = "wordle_list.txt";
    char guesses[2 * M + 2];
    char corrects[M + 1];
    char contains[A + 1];
    char missings[A + 1];
    int i, is_guess, n_words, n_cands;
    char *guess;

    memset(words, 0, sizeof(words));
    memset(guesses, 0, sizeof(guesses));
    memset(corrects, 0, sizeof(corrects));
    memset(contains, 0, sizeof(contains));
    memset(missings, 0, sizeof(missings));
    memcpy(guesses, word1, M * sizeof(char));
    memcpy(guesses + M + 1, word2, M * sizeof(char));

    /* read in all words */
    fptr = fopen(fn, "r");
    for (n_words = 0; !feof(fptr); n_words++)
    {
        fscanf(fptr, "%s", words + (n_words * (M + 1)));
    }
    n_words--;

    n_cands = 0;
    pop_constraints(target, guesses, 2, corrects, contains, missings);
    for (i = 0; i < n_words; i++)
    {
        guess = words + (i * (M + 1));
        is_guess = check_guess(guess, corrects, contains, missings);
        n_cands += is_guess;
        if (is_guess)
        {
            printf("%s\n", guess);
        }
    }
    printf("Guessing %s with (%s, %s) leaves %d guesses\n",
            target, word1, word2, n_cands);
    return n_cands;
}
int main(int argc, const char *argv[])
{
    /* run(); */
    test_word2("smelt", "dream");
    /* test_word3("shard", "raise", "solar"); */
    return 0;
}
