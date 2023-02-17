#ifndef BOARD_HPP_3PHIJVTR
#define BOARD_HPP_3PHIJVTR

#include <iostream>
#include <cstring>
#define N 9
#define M 3

using namespace std;

class Board
{
    private:
        short board[N * N];

    public:
        Board();
        Board(Board &b);
        bool is_solved();
        bool is_legal();
        void make_move(short row, short col, short val);
        void undo_move(short row, short col);
        void from_string(string s);
            // accepts strings with values [0,9], 0 = empty
            // always overrides every cell
        void print();
        short get_legal_moves(short row, short col, bool *moves);
            // moves[9], moves[i] = True if board[row,col] can be = i - 1
        void print_legals();
        bool fill_simple(int *loc_idxs, short *vals, int *move_idx);
        bool solve(int *loc_idxs, short *vals, int move_idx, bool brute_force);
};

void print_solve(int *loc_idxs, short *vals);


#endif /* end of include guard: BOARD_HPP_3PHIJVTR */
