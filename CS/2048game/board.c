#include "board.h"

const char DIRECTIONS[5] = {'L', 'R', 'U', 'D', 'X'};

int new_square(void)
{
    if (arc4random_uniform(10) == 0)
    {
        return 2;
    }
    return 1;
}

void init_board(Board board)
{
    int i;

    memset(board, 0, sizeof(Board));
    for (i = 0; i < 2; i++)
    {
        board[arc4random_uniform(LEN * LEN)] = new_square();
    }
}

void reset_game(GameState* game)
{
    init_board(game->board);
    game->score = 0;
    game->last_direction = 4;
}

void copy_game(const GameState* src, GameState* dest)
{
    memcpy(dest, src, sizeof(GameState));
}

void print_board(GameState* game)
{
    int i, j;

    mvprintw(0, 0,
            "Score: %d, Last Move: %c\n|----------------------------|\n|",
            game->score, DIRECTIONS[game->last_direction]
            );

    for (i = 0; i < LEN; i++)
    {
        for (j = 0; j < LEN; j++)
        {
            if (game->board[i * LEN + j] > 0)
            {
                printw(" %5d ", (int) pow(2, game->board[i * LEN + j]));
            }
            else
            {
                printw("     _ ");
            }
        }
        printw("|\n|                            |\n|");
    }
    printw("----------------------------|\n");
}

void print_legals(GameState* game)
{
    int num_legals, i;
    int directions[4];
    num_legals = get_legal_moves(game->board, directions);
    printw("%d legal moves", num_legals);
    if (num_legals == 0)
    {
        printw(".");
        return;
    }
    printw(": [");
    for (i = 0; i < num_legals - 1; i++)
    {
        printw("%c, ", DIRECTIONS[directions[i]]);
    }
    printw("%c]\n", DIRECTIONS[directions[num_legals - 1]]);
}

/* In each row, first shift all blocks, then try merging
 * returns -1 if no merge
 */
int merge_row(char** prow)
{
    int score = -1;
    int i, j;
    int pcurr_idx = 0;
    /* coalesce blanks first */
    for (i = 0; i < LEN; i++)
    {
        if (*prow[pcurr_idx] == 0 && *prow[i] != 0)
        {
            score = MAX(score, 0);
            *prow[pcurr_idx] = *prow[i];
            *prow[i] = 0;
            pcurr_idx++;
        }
        else if (*prow[pcurr_idx] != 0)
        {
            pcurr_idx++;
        }
    }

    /* pcurr_idx points to first zero entry in the row, coalesce everything
     * before
     * */
    for (i = 0; i < pcurr_idx - 1; i++)
    {
        if (*prow[i] != *prow[i + 1])
        {
            continue;
        }
        score = MAX(score, 0);
        (*prow[i])++;
        score += (int) pow(2, *prow[i]);

        for (j = i + 1; j < pcurr_idx - 1; j++)
        {
            *prow[j] = *prow[j + 1];
        }
        *prow[pcurr_idx - 1] = 0;
        pcurr_idx--;
    }

    return score;
}

/* populates empties array w/ pointers to board pieces, returns length */
int get_empties(Board board, char** empties)
{
    int empty_idx = 0;
    int i, j;
    for (i = 0; i < LEN; i++)
    {
        for (j = 0; j < LEN; j++)
        {
            if (board[i * LEN + j] == 0)
            {
                empties[empty_idx] = &board[i * LEN + j];
                empty_idx++;
           }
        }
    }
    return empty_idx;
}
/* pass game_score = NULL to avoid updating */
char merge_board(Board board, int direction, int* game_score)
{
    char moved = false;
    int row_score;
    int score = 0;
    int row, col;
    char* prow[LEN]; /* pointers to entries in the row */
    if (direction == 0)
    {
        for (row = 0; row < LEN; row++)
        {
            for (col = 0; col < LEN; col++)
            {
                prow[col] = board + (row * LEN + col);
            }
            row_score = merge_row(prow);
            moved = moved || row_score >= 0;
            score += MAX(row_score, 0);
        }
    }
    else if (direction == 1)
    {
        for (row = 0; row < LEN; row++)
        {
            for (col = LEN - 1; col >= 0; col--)
            {
                prow[LEN - col - 1] = board + (row * LEN + col);
            }
            row_score = merge_row(prow);
            moved = moved || row_score >= 0;
            score += MAX(row_score, 0);
        }
    }
    else if (direction == 2)
    {
        for (col = 0; col < LEN; col++)
        {
            for (row = 0; row < LEN; row++)
            {
                prow[row] = board + (row * LEN + col);
            }
            row_score = merge_row(prow);
            moved = moved || row_score >= 0;
            score += MAX(row_score, 0);
        }
    }
    else if (direction == 3)
    {
        for (col = 0; col < LEN; col++)
        {
            for (row = LEN - 1; row >= 0; row--)
            {
                prow[LEN - row - 1] = board + (row * LEN + col);
            }
            row_score = merge_row(prow);
            moved = moved || row_score >= 0;
            score += MAX(row_score, 0);
        }
    }

    if (game_score != NULL)
    {
        (*game_score) += score;
    }
    return moved;
}
void add_piece(Board board)
{
    char* empties[LEN * LEN];
    int num_empty = get_empties(board, empties);
    int i = arc4random_uniform(num_empty);
    (*empties[i]) = 1;
}

int get_legal_moves(const Board board, int* directions)
{
    Board temp;
    char moved;
    int i;
    int directions_idx = 0;
    for (i = 0; i < 4; i++)
    {
        memcpy(temp, board, sizeof(Board));
        moved = merge_board(temp, i, NULL);
        if (moved)
        {
            directions[directions_idx] = i;
            directions_idx++;
        }
    }
    return directions_idx;
}

void move_board(GameState* game, int direction)
{
    char moved = merge_board(game->board, direction, &game->score);
    if (moved) /* add a piece if moved */
    {
        add_piece(game->board);
        game->last_direction = direction;
    }
}
