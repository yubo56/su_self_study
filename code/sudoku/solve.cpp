#include <cstdio>
#include <iostream>
#include "board.hpp"

using namespace std;

void test_utils()
{
    Board b;
    // check print() and is_solved with a legal board
    b.from_string(
                "123456789"
                "456789123"
                "789123456"
                "234567891"
                "567891234"
                "891234567"
                "345678912"
                "678912345"
                "912345678"
            );
    b.print();
    cout << b.is_solved() << endl;

    // check horizontal dupe
    b.from_string(
                "123456781"
                "456789123"
                "789123456"
                "234567899"
                "567891234"
                "912345678"
                "345678912"
                "678912345"
                "891234567"
            );
    cout << b.is_solved() << endl;

    // check vertical dupes
    b.from_string(
                "123456789"
                "156789423"
                "789123456"
                "234567891"
                "567891234"
                "912345678"
                "345678912"
                "678912345"
                "891234567"
            );
    cout << b.is_solved() << endl;

    // check block dupe
    b.from_string(
                "123456789"
                "234567891"
                "345678912"
                "456789123"
                "567891234"
                "678912345"
                "789123456"
                "891234567"
                "912345678"
            );
    cout << b.is_solved() << endl;

    // test copy constructor
    Board c(b);
    b.print();
    cout << endl;
    c.print();

    // test check_legal_moves
    b.from_string(
                "010000000"
                "203000006"
                "046000000"
                "000000000"
                "000000000"
                "000000000"
                "000000000"
                "000000000"
                "000000000"
            );
    bool moves[N];
    int num_legals;
    b.print();
    num_legals = b.get_legal_moves(1, 1, moves);
    printf("Legals (%d) are: ", num_legals);
    for (int i = 0; i < N; i++)
    {
        if (moves[i])
        {
            cout << i + 1;
        }
    }
    cout << endl;
}
void test_solve()
{
    Board b;
    b.from_string(
                "123456789"
                "456789123"
                "789123456"
                "234067890"
                "567891234"
                "891234567"
                "345678912"
                "678012340"
                "912340678"
            );
    int move_locs[N * N];
    short move_vals[N * N] = {};
    b.print();
    b.print_legals();
    b.solve(move_locs, move_vals, 0, false);
    print_solve(move_locs, move_vals);
}
void run_interactive()
{
    string instr;
    cout << "Type in Sudoku puzzle below:" << endl;
    cin >> instr;
    Board b;
    b.from_string(instr);
    int move_locs[N * N];
    short move_vals[N * N] = {};
    b.print();
    b.solve(move_locs, move_vals, 0, true);
    print_solve(move_locs, move_vals);
    b.print();
}

int main(int argc, const char *argv[])
{
    // test_utils();
    // test_solve();
    run_interactive();
    return 0;
}
