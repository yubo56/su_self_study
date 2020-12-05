#include "board.h"
#include "players.h"
#include <string.h>

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
            printw("(q) to quit\n");
            printw("(r) to restart\n");
            printw("(l) for legal moves\n");
            printw("(p) for autoplay_rand\n");
            printw("(b) for autoplay_basic (2ply)\n");
            printw("(B) for autoplay_basic (3ply)\n");
            printw("(a) for autoplay_heuristic (2ply)\n");
            printw("(A) for autoplay_heuristic (3ply)\n");
            printw("(h) to close this menu\n");
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
            autoplay_rand(&game, 1, 0);
            continue;
        }
        else if (ch == 'b')
        {
            autoplay_basic(&game, 1, 2);
            continue;
        }
        else if (ch == 'B')
        {
            autoplay_basic(&game, 1, 3);
            continue;
        }
        else if (ch == 'a')
        {
            autoplay_heuristic(&game, 1, 2);
            continue;
        }
        else if (ch == 'A')
        {
            autoplay_heuristic(&game, 1, 3);
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

void run_many
    (
        void (*runner)(GameState* game, int to_display, int lookahead),
        int num_runs,
        int lookahead,
        int to_display
    )
{
    int i, j, max, max_all;
    int counts[LEN * LEN];
    GameState game;

    memset(counts, 0, LEN * LEN * sizeof(int));

    max_all = 0;
    for (i = 0; i < num_runs; i++)
    {
        reset_game(&game);
        runner(&game, 0, lookahead);

        max = 0;
        for (j = 0; j < LEN * LEN; j++)
        {
            max = MAX(max, game.board[j]);
        }
        counts[max - 1]++;
        if (to_display)
        {
            printf("Ran %d, got max %d\n", i, (int) pow(2, max));
        }
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
    printf("\t--run-many-basic to run many using basic heuristic\n");
}
void run_main(int argc, const char *argv[])
{
    if (argc == 1 || strcmp(argv[1], "--play") == 0)
    {
        play();
    }
    else if (strcmp(argv[1], "--help") == 0)
    {
        print_help(argv[0]);
    }
    else if (strcmp(argv[1], "--run-many-rand") == 0)
    {
        run_many(autoplay_rand, 10000, 0, 0);
    }
    else if (strcmp(argv[1], "--run-many-basic-2") == 0)
    {
        run_many(autoplay_basic, 300, 2, 0);
    }
    else if (strcmp(argv[1], "--run-many-basic-3") == 0)
    {
        run_many(autoplay_basic, 100, 3, 1);
    }
    else if (strcmp(argv[1], "--run-many-heuristic-2") == 0)
    {
        run_many(autoplay_heuristic, 300, 2, 0);
    }
    else if (strcmp(argv[1], "--run-many-heuristic-3") == 0)
    {
        run_many(autoplay_heuristic, 100, 3, 1);
    }
    else
    {
        printf("Invalid usage\n");
        print_help(argv[0]);
    }
}

int main(int argc, const char *argv[])
{
    run_main(argc, argv);
    return 0;
}
