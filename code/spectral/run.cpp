#include <cstdio>
#include "Tests.hpp"
void test() {
    int passes = 0;

    passes += test_complex();
    passes += test_fourier();

    printf("%d tests passed!\n", passes);
}

int main(int argc, const char *argv[])
{
    test();
    return 0;
}
