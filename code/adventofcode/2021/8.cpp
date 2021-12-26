// oh boy another grungy one
//  a       b       c       d       e       f       g
// true  , true  , true ,  false , true  , true ,  true , // 0 (6)
// false , false , true ,  false , false , true ,  false, // 1 (2)
// true  , false , true ,  true  , true  , false,  true , // 2 (5)
// true  , false , true ,  true  , false , true ,  true , // 3 (5)
// false , true  , true ,  true  , false , true ,  false, // 4 (4)
// true  , true  , false,  true  , false , true ,  true , // 5 (5)
// true  , true  , false,  true  , true  , true ,  true , // 6 (6)
// true  , false , true ,  false , false , true ,  false, // 7 (3)
// true  , true  , true ,  true  , true  , true ,  true , // 8 (7)
// true  , true  , true ,  true  , false , true ,  true , // 9 (6)
#include <cstdio>
#include <iostream>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <string>
using namespace std;
string NUMSTRS[10] = {
    "abcefg",
    "cf",
    "acdeg",
    "acdfg",
    "bcdf",
    "abdfg",
    "abdefg",
    "acf",
    "abcdefg",
    "abcdfg",
};
const int N_SIG = 10;
const int N_OUT = 4;
const int STR_SIZE = 8;

// brute force try permutations
int get_output(char *strs, char *outs, int *n_uniq)
{
    for (int i = 0; i < N_OUT; i++)
    {
        int len = strlen(outs + i * STR_SIZE);
        if (len == 2 || // 1
                len == 4 || // 4
                len == 3 || // 7
                len == 7) // 8
        {
            (*n_uniq)++;
        }
    }
    string char_map = "abcdefg";
    do {
        bool all_words_exist = true;
        for (int i = 0; i < N_SIG && all_words_exist; i++)
        {
            string cand;
            for (int j = 0; strs[i * STR_SIZE + j] > 0; j++)
            {
                cand.push_back(char_map[(int) strs[i * STR_SIZE + j] - 97]);
            }
            bool word_found = false;
            sort(cand.begin(), cand.end());
            // cout << cand << ',' << cand.size() << endl;
            for (int j = 0; j < 10; j++)
            {
                // cout << cand.compare(NUMSTRS[j]) << NUMSTRS[j] << endl;
                if (cand.compare(NUMSTRS[j]) == 0)
                {
                    word_found = true;
                }
            }
            if (!word_found)
            {
                all_words_exist = false;
            }
        }
        if (all_words_exist)
        {
            // cout << char_map << endl;
            int sum = 0;
            for (int i = 0; i < N_OUT; i++)
            {
                string cand;
                for (int j = 0; outs[i * STR_SIZE + j] > 0; j++)
                {
                    cand.push_back(char_map[(int) outs[i * STR_SIZE + j] - 97]);
                }
                sort(cand.begin(), cand.end());
                // cout << cand << ',' << cand.size() << ',' << sum << endl;
                for (int j = 0; j < 10; j++)
                {
                    if (cand.compare(NUMSTRS[j]) == 0)
                    {
                        sum += j * pow(10, N_OUT - i - 1);
                        break;
                    }
                }
            }
            return sum;
        }
    } while (next_permutation(char_map.begin(), char_map.end()));
    return 0;
}
int main(int argc, const char *argv[])
{
    FILE *fptr;
    const char *fn = "8.in";
    // for reading in individual rows
    char strs[N_SIG * STR_SIZE];
    char outs[N_OUT * STR_SIZE];

    fptr = fopen(fn, "r");
    fscanf(fptr, "%s %s %s %s %s %s %s %s %s %s | %s %s %s %s",
            strs, strs + STR_SIZE, strs + 2 * STR_SIZE, strs + 3 * STR_SIZE,
            strs + 4 * STR_SIZE, strs + 5 * STR_SIZE, strs + 6 * STR_SIZE, strs
            + 7 * STR_SIZE, strs + 8 * STR_SIZE, strs + 9 * STR_SIZE, outs, outs
            + STR_SIZE, outs + 2 * STR_SIZE, outs + 3 * STR_SIZE);
    int n_uniq = 0;
    int tot_sum = 0;
    while (!feof(fptr))
    {
        tot_sum += get_output(strs, outs, &n_uniq);
        fscanf(fptr, "%s %s %s %s %s %s %s %s %s %s | %s %s %s %s",
                strs, strs + STR_SIZE, strs + 2 * STR_SIZE, strs + 3 * STR_SIZE,
                strs + 4 * STR_SIZE, strs + 5 * STR_SIZE, strs + 6 * STR_SIZE,
                strs + 7 * STR_SIZE, strs + 8 * STR_SIZE, strs + 9 * STR_SIZE,
                outs, outs + STR_SIZE, outs + 2 * STR_SIZE, outs + 3 *
                STR_SIZE);
    }
    printf("%d unique ints in output (1, 4, 7, 8)\n", n_uniq);
    printf("Sum of outputs is %d\n", tot_sum);
    return 0;
}
