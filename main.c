#include "buffer.h"
#include "todo.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>

#define UNIT 10

int main(int argc, const char *argv[])
{
    FILE *fp;

    if (NULL == (fp = fopen("test1", "r")))
        exit(1);

    buf_t * buf = buf_new(UNIT);
    size_t ret;
    buf_grow(buf, UNIT);

    while ((ret = fread(buf->data+buf->size, 1, buf->a_size-buf->size, fp)) > 0) {
        buf->size += ret;
        buf_grow(buf, buf->size+UNIT);
    }

    todo_t *td = todo_new();
    todo_parse(td, buf->data, buf->size);
    task_t *t;
    int i;

    for (i=0, t=td->head; t; t=t->next, i++) {
        printf("%d. %s  %.*s\n", i, t->state == done ? "✓": "✖", t->c_size, t->content);
    }

    buf_free(buf);
    return 0;
}
