#ifndef TESTS_H_LURAG9HF
#define TESTS_H_LURAG9HF

#include <stdio.h>
#include <string.h>

#include "circular_buffer.h"
#include "naive.h"
#include "rk.h"
#include "kmp.h"

void match_test(char *text_str, char *target, const char correct);
void match_tests(void);
void circbuf_tests(void);

#endif /* end of include guard: TESTS_H_LURAG9HF */
