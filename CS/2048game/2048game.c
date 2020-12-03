/* gcc board.c 2048game.c -Ofast -lcurses -o 2048game
 * */
#include "board.h"
#include <unistd.h>
#include <string.h>

void autoplay_rand(GameState* game, int to_display)
{
    int num_legals, move;
    int directions[4];

    num_legals = get_legal_moves(game, directions);
    while (num_legals > 0)
    {
        move = directions[arc4random_uniform(num_legals)];
        move_board(game, move);
        num_legals = get_legal_moves(game, directions);
        if (to_display == 1)
        {
            print_board(game);
            printw("Playing %c\n", DIRECTIONS[move]);
            refresh();
            usleep(100000);
        }
    }
}

void play()
{
    int ch;
    GameState game;
    initscr();
    cbreak();
    noecho();
    nonl();
    keypad(stdscr, TRUE);

    reset_game(&game);
    print_board(&game);
    refresh();

    while ((ch = getch()))
    {
        if (ch == 'q')
        {
            break;
        }
        else if (ch == 'h')
        {
            printw("(q) to quit\n(h) to close\n(r) to restart\n(l) for legal moves\n(p) for autoplay_rand\n");
            continue;
        }
        else if (ch == 'l')
        {
            print_legals(&game);
            refresh();
            continue;
        }
        else if (ch == 'p')
        {
            autoplay_rand(&game, 1);
            continue;
        }
        clear();

        if (ch == 'r')
        {
            reset_game(&game);
        }
        else if (ch == KEY_LEFT)
        {
            move_board(&game, 0);
        }
        else if (ch == KEY_RIGHT)
        {
            move_board(&game, 1);
        }
        else if (ch == KEY_UP)
        {
            move_board(&game, 2);
        }
        else if (ch == KEY_DOWN)
        {
            move_board(&game, 3);
        }
        print_board(&game);
        refresh();
    }

    endwin();
}

void run_many_rand()
{
    int num_runs = 5000;
    int i, j, max, max_all;
    int counts[LEN * LEN];
    GameState game;

    memset(counts, 0, LEN * LEN * sizeof(int));

    max_all = 0;
    for (i = 0; i < num_runs; i++)
    {
        reset_game(&game);
        autoplay_rand(&game, 0);

        max = 0;
        for (j = 0; j < LEN * LEN; j++)
        {
            max = MAX(max, game.board[j]);
        }
        counts[max - 1]++;
        max_all = MAX(max_all, max - 1);
    }

    /* don't print the first few zero ones */
    i = 0;
    while (counts[i] == 0) { i++; }

    for (; i <= max_all; i++)
    {
        printf("%d runs with max cell %d (%.2f%%)\n",
                counts[i], (int) pow(2, i + 1), 100.0 * counts[i] / num_runs);
    }
}

void print_help(const char* name)
{
    printf("%s usage:\n", name);
    printf("\t--help for help\n");
    printf("\t--play to play\n");
    printf("\t--run-many-rand to run many randoms\n");
}
int main(int argc, const char *argv[])
{
    if (argc == 1 || strcmp(argv[1], "--help") == 0)
    {
        print_help(argv[0]);
    }
    else if (strcmp(argv[1], "--play") == 0)
    {
        play();
    }
    else if (strcmp(argv[1], "--run-many-rand") == 0)
    {
        run_many_rand();
    }
    else
    {
        printf("Invalid usage\n");
        print_help(argv[0]);
    }
    return 0;
}
