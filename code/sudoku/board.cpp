#include <cstdio>
#include "board.hpp"

Board::Board()
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            board[i * N + j] = 0;
        }
    }
}
Board::Board(Board &b)
{
    for (int i = 0; i < N * N; i++)
    {
        board[i] = b.board[i];
    }
}
void set_false(bool *flags, int len)
{
    for (int i = 0; i < len; i++)
    {
        flags[i] = false;
    }
}
bool Board::is_solved()
{
    for (int i = 0; i < N * N; i++)
    {
        // if board has any unfilled values, return false
        if (!board[i]) return false;
    }
    // if all filled, just a question of legality
    return is_legal();
}
bool Board::is_legal()
{
    bool flags[N + 1];
    set_false(flags, N + 1);
    // check rows
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            short val = board[i * N + j];
            if (val) // if board has a value there
            {
                if (flags[val]) // is dupe
                {
                    return false;
                }
                else // first time seeing
                {
                    flags[val] = true;
                }
            }
        }
        set_false(flags, N + 1);
    }

    // check cols
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            short val = board[j * N + i];
            if (val) // if board has a value there
            {
                if (flags[val]) // is dupe
                {
                    return false;
                }
                else // first time seeing
                {
                    flags[val] = true;
                }
            }
        }
        set_false(flags, N + 1);
    }

    // check grids
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            int grid_row = i / M;
            int grid_col = i % M;
            int subgrid_row = j / M;
            int subgrid_col = j % M;
            int idx = (grid_row * M + subgrid_row) * N +
                grid_col * M + subgrid_col;

            short val = board[idx];
            if (val) // if board has a value there
            {
                if (flags[val]) // is dupe
                {
                    return false;
                }
                else // first time seeing
                {
                    flags[val] = true;
                }
            }
        }
        set_false(flags, N + 1);
    }

    // passed all checks
    return true;
}
void Board::make_move(short row, short col, short val)
{
    board[row * N + col] = val;
}
void Board::undo_move(short row, short col)
{
    board[row * N + col] = 0;
}
void Board::from_string(string s)
{
    if (s.length() != N * N)
    {
        cout << "Invalid string for Board::from_string, is:" << s << endl;
    }
    for (int i = 0; i < N * N; i++)
    {
        board[i] = s[i] - 48;
    }
}
void Board::print()
{
    cout << '+';
    for (int i = 0; i < N + (M - 1); i++)
    {
        cout << '-';
    }
    cout << '+' << endl;
    for (int i = 0; i < N; i++)
    {
        cout << '|';
        for (int j = 0; j < N; j++)
        {
            int idx = i * N + j;
            if (board[idx])
            {
                cout << board[i * N + j];
            }
            else
            {
                cout << '_';
            }
            if (j % M == (M - 1) && j != N - 1)
            {
                cout << ' ';
            }
        }
        if (i % M == (M - 1) && i != N - 1)
        {
            cout << '|' << endl;
            cout << '|';
            for (int i = 0; i < N + (M - 1); i++)
            {
                cout << ' ';
            }
        }
        cout << '|' << endl;
    }
    cout << '+';
    for (int i = 0; i < N + (M - 1); i++)
    {
        cout << '-';
    }
    cout << '+' << endl;
}

short Board::get_legal_moves(short row, short col, bool *moves)
{
    short num_legals = N;
    if (board[row * N + col]) return 0; // fail fast if board is filled
    for (int i = 0; i < N; i++)
    {
        moves[i] = true;
    }
    for (int i = 0; i < N; i++)
    {
        short row_val = board[row * N + i];
        if (row_val && moves[row_val - 1])
        {
            moves[row_val - 1] = false;
            num_legals--;
        }

        short col_val = board[i * N + col];
        if (col_val && moves[col_val - 1])
        {
            moves[col_val - 1] = false;
            num_legals--;
        }

        int grid_row = row / M;
        int grid_col = col / M;
        int subgrid_row = i / M;
        int subgrid_col = i % M;
        int idx = (grid_row * M + subgrid_row) * N +
            grid_col * M + subgrid_col;

        int grid_val = board[idx];
        if (grid_val && moves[grid_val - 1])
        {
            moves[grid_val - 1] = false;
            num_legals--;
        }
    }
    return num_legals;
}
void Board::print_legals()
{
    bool moves[N];
    for (int i = 0; i < N * N; i++)
    {
        if (board[i]) continue;
        short row = i / N;
        short col = i % N;
        get_legal_moves(row, col, moves);

        for (int j = 0; j < N; j++)
        {
            if (moves[j])
            {
                printf("Can move at (%d, %d) with %d\n", row, col, j + 1);
            }
        }
    }
}
bool Board::fill_simple(int *loc_idxs, short *vals, int *move_idx)
{
    /**
     * Approach: try to fill in everything with only one value first
     *  will need to loop until converged
     *
     * then, if not solved, we make a move, try to solve the resulting board,
     * and undo if failed
     */
    short num_legals;
    short move_val;
    bool moves[N];
    bool has_changes = true;
    while (has_changes)
    {
        has_changes = false;
        for (int i = 0; i < N * N; i++)
        {
            if (board[i]) continue;
            short row = i / N;
            short col = i % N;
            num_legals = get_legal_moves(row, col, moves);

            // fail fast if any square has no moves
            if (num_legals == 0)
            {
                return false;
            }
            // else, if an only move, make the move and restart loop
            else if (num_legals == 1)
            {
                for (int j = 0; j < N; j++)
                {
                    if (moves[j])
                    {
                        move_val = j + 1;
                        break;
                    }
                }
                loc_idxs[*move_idx] = i;
                vals[*move_idx] = move_val;
                (*move_idx)++;
                make_move(row, col, move_val);
                has_changes = true;
                break;
            }
        }
    }
    return true;
}
bool Board::solve(int *loc_idxs, short *vals, int move_idx, bool brute_force)
{
    bool moves[N];
    short num_legals;
    short move_val;
    fill_simple(loc_idxs, vals, &move_idx);
    if (!brute_force)
    {
        return is_solved();
    }

    while (!is_solved())
    {
        for (int i = 0; i < N * N; i++)
        {
            if (board[i]) continue;
            short row = i / N;
            short col = i % N;
            num_legals = get_legal_moves(row, col, moves);

            if (num_legals <= 1)
            {
                return false;
            }
            // else, just make the first legal move
            else
            {
                for (int j = 0; j < N; j++)
                {
                    if (moves[j])
                    {
                        move_val = j + 1;
                        break;
                    }
                }
                loc_idxs[move_idx] = i;
                vals[move_idx] = move_val;
                move_idx++;
                make_move(row, col, move_val);
                break;
            }
        }
        fill_simple(loc_idxs, vals, &move_idx);
    }
    return true;
}
void print_solve(int *loc_idxs, short *vals)
{
    for (int i = 0; i < N * N; i++)
    {
        int row = loc_idxs[i] / N;
        int col = loc_idxs[i] % N;
        if (vals[i] > 0)
        {
            printf("Move at (%d, %d) with %d\n", row, col, vals[i]);
        }
    }
}
