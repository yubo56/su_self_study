#include <stdio.h>
#include "naive.h"
#include "rk.h"

void test(char *text, char *target)
{
    /*
     * Prints test results for each matching type for a text and target string
     */
    printf("(%s, %s): %d, %d\n", text, target,
        match_naive(text, target),
        match_rk(text, target)
    );
}

int main(int argc, const char *argv[])
{
    printf("(%s, %s): %s\n", "[text]", "[target]", "[naive], [rk]");

    /* test cases */
    test("abac", "abac"); /* trivial */
    test("i am a foo bar baz", "foo bar"); /* generic test */
    test("abcabcabc", "abcd"); /* terminate midway through a match */
    test("ababac", "abac"); /* correct lookback */
    test("", "abac"); /* target is empty */
    test("abadac", ""); /* text is empty */

    return 0;
}
