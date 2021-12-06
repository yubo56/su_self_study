#include <cstdio>
#include <cstring>
#include <cmath>

// const char *fn = "4test.in";
// const int N_NUMS = 27;
// const int N = 5;
// const int N_BOARDS = 3;
const char *fn = "4.in";
const int N_NUMS = 100;
const int N = 5;
const int N_BOARDS = 100;

typedef unsigned int t_board; // 5x5 board = 25 bits, unsigned int is okay
// NB: if 6 <= N <= 8, use longs for masks

t_board pow2(int n)
{
    return 1 << n;
}
int check_win(const t_board *masks, const t_board *boards, bool *has_won)
{
    for (int board_idx = 0; board_idx < N_BOARDS; board_idx++)
    {
        if (has_won[board_idx]) { continue; }

        bool is_winning = false;
        for (int mask_idx = 0; mask_idx < 2 * N; mask_idx++)
        {
            if ((boards[board_idx] & masks[mask_idx]) == masks[mask_idx])
            {
                // for (int i = 0; i < N * N; i++)
                // {
                //     printf("%d, %u, %u\n",
                //             i,
                //             (boards[board_idx] & pow2(i)) > 0,
                //             (masks[mask_idx] & pow2(i)) > 0);
                // }
                is_winning = true;
                break;
            }
        }
        if (is_winning)
        {
            return board_idx;
        }
    }
    return -1;
}
void print_board(const t_board *boards, int *board_orders, int called, int winner)
{
    int sum_uncalled = 0;
    for (int i = 0; i < N_NUMS; i++)
    {
        unsigned int curr_mask = pow2(board_orders[winner * N_NUMS + i]);
        if (board_orders[winner * N_NUMS + i] > -1 &&
                (boards[winner] & curr_mask) != curr_mask)
        {
            sum_uncalled += i;
        }
    }
    printf("Winner is board %d on number %d, "
            "sum of uncalled is %d, product is %d\n",
            winner, called, sum_uncalled,
            called * sum_uncalled);
}
int main(int argc, const char *argv[])
{
    FILE *fptr;
    int n_nums = 1;
    t_board masks[2 * N];
    t_board boards[N_BOARDS]; // tracking which nums are hit, bitmasks
    memset(masks, 0, sizeof(masks));
    memset(boards, 0, sizeof(boards));

    int called_nums[N_NUMS];
    // For each board, orders[num] = location of bit to flip
    int board_orders[N_BOARDS * N_NUMS];
    memset(board_orders, -1, sizeof(board_orders));

    fptr = fopen(fn, "r");

    //
    // 1: parse out list of called numbers
    //
    fscanf(fptr, "%d", called_nums);
    // only leading comma stops correctly, not trailing comma
    while (fscanf(fptr, ",%d", called_nums + n_nums) > 0) { n_nums++; }
    // for (int j = 0; j < n_nums; j++)
    // {
    //     printf("%d\n", called_nums[j]);
    // }

    //
    // 2: parse out boards to be convenient
    //
    for (int board_idx = 0; board_idx < N_BOARDS; board_idx++)
    {
        for (int i = 0; i < N * N; i++)
        {
            int num;
            fscanf(fptr, "%d", &num);
            board_orders[board_idx * N_NUMS + num] = i;
        }
    }
    // for (int board_idx = 0; board_idx < N_BOARDS; board_idx++)
    // {
    //     for (int i = 0; i < N_NUMS; i++)
    //     {
    //         printf("%d\n", board_orders[board_idx * N_NUMS + i]);
    //     }
    //     printf("\n");
    // }


    //
    // 3: generate bitmasks
    //
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            // row masks first
            masks[i] += pow2(i * N + j);
            masks[i + N] += pow2(j * N + i);
        }
    }
    // for (int i = 0; i < 2 * N; i++)
    // {
    //     // printf("%u\n", masks[i]);
    //     for (int j = 0; j < N * N; j++)
    //     {
    //         printf("%d, %d, %u\n",
    //                 i, j,
    //                 (masks[i] & pow2(j)) > 0);
    //     }
    // }

    //
    // 4: play bingo!
    //
    int winner = -1;
    int num_winners = 0;
    bool has_won[N_BOARDS];
    memset(has_won, false, sizeof(has_won));
    for (int num_to_call_idx = 0; num_to_call_idx < N_NUMS; num_to_call_idx++)
    {
        int called = called_nums[num_to_call_idx];
        for (int i = 0; i < N_BOARDS; i++)
        {
            unsigned int curr_mask = pow2(board_orders[i * N_NUMS + called]);
            boards[i] = boards[i] | curr_mask;
        }
        printf("Calling %d\n", called);
        winner = check_win(masks, boards, has_won);
        while (winner > -1 && !has_won[winner])
        {
            has_won[winner] = true;
            num_winners++;
            if (num_winners == 1 || num_winners == N_BOARDS)
            {
                printf("Board %d won, %d/%d winners so far\n",
                        winner, num_winners, N_BOARDS);
                print_board(boards, board_orders, called, winner);
            }
            winner = check_win(masks, boards, has_won);
        }
    }

    return 0;
}
