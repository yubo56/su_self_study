/**
 * If all the numbers from 1-1000 inclusive are written out in words, how many
 * letters would be used? No spaces, no hyphens, but count "and"'s
 *
 * .... this is so grungy
 */
#include <cstdio>
int TEENS_COUNTS[10] = {
    3, // ten
    6, // eleven
    6, // twelve
    8, // thirteen
    8, // fourteen
    7, // fifteen
    7, // sixteen
    9, // seventeen
    8, // eighteen
    8 // nineteen
};
int ONES_COUNTS[10] = {
    0, // (zero, but no count)
    3, // one
    3, // two
    5, // three
    4, // four
    4, // five
    3, // six
    5, // seven
    5, // eight
    4, // nine
};
int TENS_COUNTS[10] = {
    0, // (zero, do not count)
    3, // ten
    6, // twenty
    6, // thirty
    5, // forty
    5, // fifty
    5, // sixty
    7, // seventy
    6, // eighty
    6, // ninety
};
int get_char_count(int i)
{
    int hundreds = i / 100;
    int rest = i % 100;
    int rest_num;
    if (rest < 20 && rest >= 10) // special cases
    {
        rest_num = TEENS_COUNTS[rest - 10];
    }
    else
    {
        int ones = rest % 10;
        int tens = rest / 10;
        rest_num = TENS_COUNTS[tens] + ONES_COUNTS[ones];
        // printf("%d, %d, %d\n", rest_num, TENS_COUNTS[tens], ONES_COUNTS[ones]);
    }
    if (hundreds > 0)
    {
        // 7 = "hundred", 3 = "and" (only if not exactly N x 100)
        return rest_num + ONES_COUNTS[hundreds] + 7 + (rest ? 3 : 0);
    }
    else
    {
        return rest_num;
    }
}
int main(int argc, const char *argv[])
{
    int total_count = 0;
    // printf("%d, %d\n", get_char_count(342), get_char_count(115));
    // should be 23, 20
    for (int i = 1; i < 1000; i++)
    {
        printf("%d, %d\n", i, get_char_count(i));
        total_count += get_char_count(i);
    }
    total_count += 11; // "one thousand"
    printf("Total char count is %d\n", total_count);
    return 0;
}
