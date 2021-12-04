/**
 * Which starting number under 1e6 produces the longest Collatz chain?
 */
#include <cstdio>
int UP_TO = 1e6;
int get_num_collatz(long num)
{
    int num_moves = 0;
    while (num > 1)
    {
        if (num % 2 == 0)
        {
            num /= 2;
        }
        else
        {
            num = 3 * num + 1;
        }
        num_moves += 1;
    }
    return num_moves;
}
int main(int argc, const char *argv[])
{
    int max_moves = 0;
    int max_i = 0;
    for (int i = 0; i < UP_TO; i++)
    {
        int num_moves = get_num_collatz(i);
        if (num_moves > max_moves)
        {
            max_i = i;
            max_moves = num_moves;
        }
        // printf("%d requires %d Collatz moves\n", i, num_moves);
    }
    printf("Max number of Collatz moves under %d is %d, requiring %d moves\n",
            UP_TO, max_i, max_moves);
    return 0;
}
