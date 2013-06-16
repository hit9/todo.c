#include "buffer.h"
#include <stdio.h>
#include <stdlib.h>

#define UNIT 10

int main(int argc, const char *argv[])
{
    FILE *fp;

    if (NULL == (fp = fopen("test", "r")))
        exit(1);

    buf_t * buf = buf_new(UNIT);
    size_t ret;
    buf_grow(buf, UNIT);

    while ((ret = fread(buf->data+buf->size, 1, buf->a_size-buf->size, fp)) > 0) {
        buf->size += ret;
        buf_grow(buf, buf->size+UNIT);
    }

    printf("%s", buf->data);

    buf_free(buf);
    return 0;
}
