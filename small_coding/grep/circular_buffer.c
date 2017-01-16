#include "circular_buffer.h"

circular_buffer new_buf(int len, FILE *stream)
{
    /* creates a new circular_buffer from stream */
    circular_buffer buf;
    int i;

    buf.stream = stream;
    buf.buf = (char *) malloc((len + 1) * sizeof(char));
    buf.end_index = len - 1;
    buf.length = len;
    buf.ended = 0;

    for (i = 0; i < len; i++)
    {
        buf.buf[i] = fgetc(stream);
    }
    buf.buf[len] = '\0';

    return buf;
}

void close_buf(circular_buffer *buf)
{
    free(buf->buf);
}

char _read(circular_buffer *buf)
{
    /* reads a char into buf from stream */
    int idx = (++buf->end_index) % buf->length;
    return buf->buf[idx] = fgetc(buf->stream);
}

char buf_get(circular_buffer *buf, int idx)
{
    /**
     * gets the character at index idx for buf, returns (-1) if none or out of scope
     * */
    if (buf->length == 0)
    {
        buf->ended = 1;
        return -1;
    }
    while (idx > buf->end_index)
    {
        if (buf->ended || _read(buf) == EOF)
        {
            buf->ended = 1;
            return -1;
        }
    }

    /* check idx is in circular_buffer */
    if (idx + buf->length < buf->end_index)
    {
        return -1;
    }

    return buf->buf[idx % buf->length];
}

void print_buf(circular_buffer *buf)
{
    printf("{\n\t"
            "buf.buf: \"%s\",\n\t"
            "buf.end_index: %d,\n\t"
            "buf.length: %d,\n\t"
            "buf.ended: %d\n"
        "}\n",
        buf->buf,
        buf->end_index,
        buf->length,
        buf->ended
    );
}
