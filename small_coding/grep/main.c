#include <stdio.h>
#include <string.h>

#include "tests.h"
#include "naive.h"
#include "rk.h"
#include "kmp.h"

void print_usage(void)
{
    char *usage = "Usage: ./match [ALGORITHM] [TARGET] < [TEXT]\n"
        "\t[ALGORITHM]: one of ['naive', 'rk', 'kmp', 'test']\n"
        "\t[TARGET]: if ALGORITHM is not 'test', the target string to seek\n"
        "\t[TEXT]: pipe the search text into stdin";
    printf("%s\n", usage);
}

int main(int argc, const char *argv[])
{
    const char *target;
    circular_buffer buf;
    if (argc < 2)
    {
        print_usage();
    }
    else if (strcmp(argv[1], "test") == 0)
    {
        match_tests();
        circbuf_tests();
    }
    else
    {
        if (argc != 3)
        {
            print_usage();
        }
        target = argv[2];
        buf = new_buf(strlen(target), stdin);

        if (strcmp(argv[1], "naive") == 0)
        {
            printf("Naive algorithm returned %d\n", match_naive(&buf, target));
        }
        else if (strcmp(argv[1], "rk") == 0)
        {
            printf("Rk algorithm returned %d\n", match_rk(&buf, target));
        }
        else if (strcmp(argv[1], "kmp") == 0)
        {
            printf("KMP algorithm returned %d\n", match_kmp(&buf, target));
        }
        else
        {
            print_usage();
        }
    }
    return 0;
}
