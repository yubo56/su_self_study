#ifndef CIRCULAR_BUFFER_H_P20SV8KP
#define CIRCULAR_BUFFER_H_P20SV8KP

#include <stdio.h>
#include <stdlib.h>


typedef struct _circular_buffer
{
    FILE *stream;
    char *buf;
    int end_index;
    int length;
    char ended;
} circular_buffer;

circular_buffer new_buf(int len, FILE *stream);
void close_buf(circular_buffer*);

char buf_get(circular_buffer *buf, int idx);
void print_buf(circular_buffer *buf);

#endif /* end of include guard: CIRCULAR_BUFFER_H_P20SV8KP */
