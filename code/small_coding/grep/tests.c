#include "tests.h"

#define NUM_MATCHES 3
#define BUF_LENGTH 10
void match_test(char *text_str, char *target, char correct)
{
    /*
     * Prints test results for each matching type for a text and target string
     */
    int i;
    FILE *text;
    circular_buffer buf;
    int m = strlen(text_str);
    int (*matches[NUM_MATCHES])(circular_buffer *, const char *) =
    {
        match_naive,
        match_rk,
        match_kmp
    };

    printf("(\"%s\", \"%s\"):\n\t%d", text_str, target, correct);

    for (i = 0; i < NUM_MATCHES; i++)
    {
        text = fmemopen(text_str, m, "r");
        buf = new_buf(m, text);

        printf(", %d", (matches[i])(&buf, target));

        close_buf(&buf);
        fclose(text);
    }
    printf("\n");
}

void match_tests(void)
{
    printf("(\"%s\", \"%s\"):\n\t%s, %s, %s, %s\n", "[text]", "[target]",
        "[correct]",
        "[naive]",
        "[rk]",
        "[kmp]"
    );

    /* test cases */
    match_test("abac", "abac", 1); /* trivial */
    match_test("abcabcabc", "abcd", 0); /* terminate midway through a match */
    match_test("ababac", "abac", 1); /* correct lookback */
    match_test("", "abac", 0); /* text is empty */
    match_test("abadac", "", 1); /* target is empty */

    match_test("i am a foo bar baz", "foo bar", 1); /* generic test */
    match_test("abcdeabcdfabcdabcaba", "abcdabcaba", 1); /* tricky KMP table */
}

void circbuf_tests(void)
{
    char *str = "abcdefghijklmnopqrstuvwxyz";
    FILE *stream = fmemopen(str, strlen(str), "r");
    circular_buffer buf;

    buf = new_buf(BUF_LENGTH, stream);
    printf("String is %s\n", str);

    printf("Expect %c to be %c\n", buf_get(&buf, 5), str[5]);
    printf("Expect %c to be %c\n", buf_get(&buf, 11), str[11]);
    printf("Expect %c to be %c\n", buf_get(&buf, 23), str[23]);
    printf("Expect out of index: %d\n", buf_get(&buf, 0));
    printf("Expect cannot read past end of buff: %d\n", buf_get(&buf, 30));
    printf("Expect can still read buff after stream closed: %c to be %c\n",
        buf_get(&buf, 25), str[25]);
    printf("Prints:\n");
    print_buf(&buf);

    close_buf(&buf);
    fclose(stream);
}
